#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <math.h>
#endif

#ifdef TARGET_WEB
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "sm64.h"

#include "gfx_dimensions.h"
#include "config.h"
#include "game/memory.h"
#include "game/game_init.h"
#include "game/print.h"
#include "audio/external.h"

#include "gfx/gfx_pc.h"
#include "gfx/gfx_opengl.h"
#include "gfx/gfx_direct3d11.h"
#include "gfx/gfx_direct3d12.h"
#include "gfx/gfx_dxgi.h"
#include "gfx/gfx_glx.h"
#include "gfx/gfx_sdl.h"
#include "gfx/gfx_dummy.h"

#include "audio/audio_api.h"
#include "audio/audio_wasapi.h"
#include "audio/audio_pulse.h"
#include "audio/audio_alsa.h"
#include "audio/audio_sdl.h"
#include "audio/audio_null.h"

#include "controller/controller_keyboard.h"

#include "configfile.h"

#include "compat.h"

#define CONFIG_FILE "sm64config.txt"

OSMesg D_80339BEC;
OSMesgQueue gSIEventMesgQueue;

struct timeval nextFrame;
struct timeval currentTime;
u8 countToThree = 0;
const u32 FRAMERATE = 30;
const u32 US_PER_FRAME = 1000000 / FRAMERATE;
u32 US_PER_FRAME_MIN;

s8 gResetTimer;
s8 D_8032C648;
s8 gDebugLevelSelect;
s8 gShowProfiler;
s8 gShowDebugText;

FILE* audioDump;
s16 dumpStrFrameCounter = 0;

static struct AudioAPI *audio_api;
static struct GfxWindowManagerAPI *wm_api;
static struct GfxRenderingAPI *rendering_api;

extern void gfx_run(Gfx *commands);
extern void thread5_game_loop(void *arg);
extern void create_next_audio_buffer(s16 *samples, u32 num_samples);
void game_loop_one_iteration(void);

void dispatch_audio_sptask(UNUSED struct SPTask *spTask) {
}

void set_vblank_handler(UNUSED s32 index, UNUSED struct VblankHandler *handler, UNUSED OSMesgQueue *queue, UNUSED OSMesg *msg) {
}

static uint8_t inited = 0;

#include "game/game_init.h" // for gGlobalTimer
void send_display_list(struct SPTask *spTask) {
    if (!inited) {
        return;
    }
    gfx_run((Gfx *)spTask->task.t.data_ptr);
}

#define printf

#ifdef VERSION_EU
#define SAMPLES_HIGH 656
#define SAMPLES_LOW 640
#else
#define SAMPLES_HIGH 816
#define SAMPLES_LOW 800
#endif

void print_debug() {
    if (dumpStrFrameCounter <= 0)
        return;

    dumpStrFrameCounter--;
    if (audioDump)
        print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(22), 197 - BORDER_HEIGHT, "AUDIO DUMP STARTED");
    else
        print_text(GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(22), 197 - BORDER_HEIGHT, "AUDIO DUMP STOPPED");
}

u8 open_audio_file(char *buffer) {
    sprintf(buffer, "dump.wav");
    audioDump = fopen(buffer, "r");
    if (audioDump) {
        for (s32 i = 0; i >= 0; ++i) {
            fclose(audioDump);

            sprintf(buffer, "dump_%d.wav", i);
            audioDump = fopen(buffer, "r");

            if (!audioDump)
                break;
        }
    }

    if (audioDump) {
        fclose(audioDump);
        audioDump = NULL;
        return FALSE;
    }

    audioDump = fopen(buffer, "wb");
    if (audioDump)
        return TRUE;

    return FALSE;
}

u8 open_audio_dump() {
    char nameBuffer[128];
    // RIFF WAV Header Data
    u8 buff[0x2C] = {0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00,
    0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
    0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00,
    0x80, 0xBB, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00,
    0x04, 0x00, 0x10, 0x00, 0x64, 0x61, 0x74, 0x61,
    0x00, 0x00, 0x00, 0x00};

    if (audioDump)
        return FALSE;

    open_audio_file(nameBuffer);

    if (!audioDump)
        return FALSE;

    fseek(audioDump, 0, SEEK_END);
    fwrite(buff, 1, 0x2C, audioDump);

    dumpStrFrameCounter = 60;
    return TRUE;
}

u8 close_audio_dump() {
    if (!audioDump)
        return FALSE;

    fclose(audioDump);
    audioDump = NULL;

    dumpStrFrameCounter = 60;
    return TRUE;
}

void on_l_pressed() {
    if (audioDump) {
        close_audio_dump();
        return;
    }

    open_audio_dump();
}

void dump_audio(s16 *audioBuffer, size_t size) {
    u32 fileSize;

    if (gPlayer1Controller->buttonPressed & L_TRIG)
        on_l_pressed();

    if (!audioDump)
        return;

    fseek(audioDump, 0, SEEK_END);
    fwrite(audioBuffer, 2, size, audioDump);
    fseek(audioDump, 0, SEEK_END);

    fileSize = ftell(audioDump);

    fseek(audioDump, 0x04, SEEK_SET);
    fwrite(&fileSize, 4, 1, audioDump);

    fseek(audioDump, 0x28, SEEK_SET);
    fileSize -= 0x2C;
    fwrite(&fileSize, 4, 1, audioDump);

    if (fileSize >= 0x20000000) // 512 MB
        close_audio_dump();
}

s64 get_time_diff(struct timeval *old, struct timeval *new) {
    return ((s64) new->tv_sec - (s64) old->tv_sec) * 1000000L
     + (s64) new->tv_usec - (s64) old->tv_usec;
}

void sleep_before_frame(void) {
    gettimeofday(&currentTime, NULL);

    // Force at FRAMERATE FPS unless holding speedup key
    if (get_time_diff(&nextFrame, &currentTime) < 0) {
        s64 diff = get_time_diff(&currentTime, &nextFrame);
#if defined(_WIN32) || defined(_WIN64)
        Sleep(ceil((double) diff / 1000.0));
#else
        struct timeval sleepTime;
        sleepTime.tv_sec = diff / 1000000;
        sleepTime.tv_usec = diff % 1000000;
        select(0, NULL, NULL, NULL, &sleepTime);
#endif
    }
}

void calculate_wait_next_frame(void) {
    if (get_keyboard_buttons_down() & 0x100000) {
        nextFrame.tv_usec += US_PER_FRAME_MIN;
    }
    else {
        if (FRAMERATE == 30)
            countToThree++;
        else if (FRAMERATE == 60)
            countToThree += 2;
        nextFrame.tv_usec += US_PER_FRAME;
    }

    if (countToThree >= 3) {
        nextFrame.tv_usec++;
        countToThree -= 3;
    }

    nextFrame.tv_sec += nextFrame.tv_usec / 1000000;
    nextFrame.tv_usec %= 1000000;

    // Max framerate exceeded
    if (get_time_diff(&nextFrame, &currentTime) > 0) {
        nextFrame.tv_usec = currentTime.tv_usec;
        nextFrame.tv_sec = currentTime.tv_sec;
        countToThree = 0;
    }
}

void produce_one_frame(void) {
    sleep_before_frame();

    gfx_start_frame();
    game_loop_one_iteration();
    
    int samples_left = audio_api->buffered();
    u32 num_audio_samples = samples_left < audio_api->get_desired_buffered() ? SAMPLES_HIGH : SAMPLES_LOW;
    //printf("Audio samples: %d %u\n", samples_left, num_audio_samples);
    s16 audio_buffer[SAMPLES_HIGH * 2 * 2];
    for (int i = 0; i < 2; i++) {
        /*if (audio_cnt-- == 0) {
            audio_cnt = 2;
        }
        u32 num_audio_samples = audio_cnt < 2 ? 528 : 544;*/
        create_next_audio_buffer(audio_buffer + i * (num_audio_samples * 2), num_audio_samples);
    }
    //printf("Audio samples before submitting: %d\n", audio_api->buffered());
    audio_api->play((u8 *)audio_buffer, 2 * num_audio_samples * 4);

    dump_audio(audio_buffer, num_audio_samples * 4);

    print_debug();
    
    gfx_end_frame();

    calculate_wait_next_frame();
}

#ifdef TARGET_WEB
static void em_main_loop(void) {
}

static void request_anim_frame(void (*func)(double time)) {
    EM_ASM(requestAnimationFrame(function(time) {
        dynCall("vd", $0, [time]);
    }), func);
}

static void on_anim_frame(double time) {
    static double target_time;

    time *= 0.03; // milliseconds to frame count (33.333 ms -> 1)

    if (time >= target_time + 10.0) {
        // We are lagging 10 frames behind, probably due to coming back after inactivity,
        // so reset, with a small margin to avoid potential jitter later.
        target_time = time - 0.010;
    }

    for (int i = 0; i < 2; i++) {
        // If refresh rate is 15 Hz or something we might need to generate two frames
        if (time >= target_time) {
            produce_one_frame();
            target_time = target_time + 1.0;
        }
    }

    request_anim_frame(on_anim_frame);
}
#endif

static void save_config(void) {
    configfile_save(CONFIG_FILE);
}

static void on_fullscreen_changed(bool is_now_fullscreen) {
    configFullscreen = is_now_fullscreen;
}

void main_func(void) {
#ifdef USE_SYSTEM_MALLOC
    main_pool_init();
    gGfxAllocOnlyPool = alloc_only_pool_init();
#else
    static u64 pool[0x165000/8 / 4 * sizeof(void *)];
    main_pool_init(pool, pool + sizeof(pool) / sizeof(pool[0]));
#endif
    gEffectsMemoryPool = mem_pool_init(0x4000, MEMORY_POOL_LEFT);

    configfile_load(CONFIG_FILE);
    atexit(save_config);

    US_PER_FRAME_MIN = (configMaxSpeedupFrameRate > (s64) FRAMERATE) ? (1000000U / (u32) configMaxSpeedupFrameRate) : US_PER_FRAME;
    if (configMaxSpeedupFrameRate < 0)
        US_PER_FRAME_MIN = 0;

#ifdef TARGET_WEB
    emscripten_set_main_loop(em_main_loop, 0, 0);
    request_anim_frame(on_anim_frame);
#endif

#if defined(ENABLE_DX12)
    rendering_api = &gfx_direct3d12_api;
    wm_api = &gfx_dxgi_api;
#elif defined(ENABLE_DX11)
    rendering_api = &gfx_direct3d11_api;
    wm_api = &gfx_dxgi_api;
#elif defined(ENABLE_OPENGL)
    rendering_api = &gfx_opengl_api;
    #if defined(__linux__) || defined(__BSD__)
        wm_api = &gfx_glx;
    #else
        wm_api = &gfx_sdl;
    #endif
#elif defined(ENABLE_GFX_DUMMY)
    rendering_api = &gfx_dummy_renderer_api;
    wm_api = &gfx_dummy_wm_api;
#endif

    gfx_init(wm_api, rendering_api, "Super Mario 64 PC-Port", configFullscreen);
    
    wm_api->set_fullscreen_changed_callback(on_fullscreen_changed);
    wm_api->set_keyboard_callbacks(keyboard_on_key_down, keyboard_on_key_up, keyboard_on_all_keys_up);
    
#if HAVE_WASAPI
    if (audio_api == NULL && audio_wasapi.init()) {
        audio_api = &audio_wasapi;
    }
#endif
#if HAVE_PULSE_AUDIO
    if (audio_api == NULL && audio_pulse.init()) {
        audio_api = &audio_pulse;
    }
#endif
#if HAVE_ALSA
    if (audio_api == NULL && audio_alsa.init()) {
        audio_api = &audio_alsa;
    }
#endif
#ifdef TARGET_WEB
    if (audio_api == NULL && audio_sdl.init()) {
        audio_api = &audio_sdl;
    }
#endif
    if (audio_api == NULL) {
        audio_api = &audio_null;
    }

    audio_init();
    sound_init();

    thread5_game_loop(NULL);
#ifdef TARGET_WEB
    /*for (int i = 0; i < atoi(argv[1]); i++) {
        game_loop_one_iteration();
    }*/
    inited = 1;
#else
    inited = 1;

    gettimeofday(&nextFrame, NULL);

    while (1) {
        wm_api->main_loop(produce_one_frame);
    }
#endif
}

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
int WINAPI WinMain(UNUSED HINSTANCE hInstance, UNUSED HINSTANCE hPrevInstance, UNUSED LPSTR pCmdLine, UNUSED int nCmdShow) {
    main_func();
    return 0;
}
#else
int main(UNUSED int argc, UNUSED char *argv[]) {
    main_func();
    return 0;
}
#endif
