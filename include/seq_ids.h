#ifndef SEQ_IDS_H
#define SEQ_IDS_H

// Sometimes a sequence id is represented as one of the below ids (the base id),
// optionally OR'd with SEQ_VARIATION.
#define SEQ_BASE_ID 0x7f
#define SEQ_VARIATION 0x80

#define SEQ_MENU_GAME_OVER (SEQ_MENU_TITLE_SCREEN | SEQ_VARIATION)

enum SeqId {
    SEQ_SOUND_PLAYER,                 // 0x00
    SEQ_EVENT_CUTSCENE_COLLECT_STAR,  // 0x01
    SEQ_MENU_TITLE_SCREEN,            // 0x02
    SEQ_LEVEL_GRASS,                  // 0x03
    SEQ_LEVEL_INSIDE_CASTLE,          // 0x04
    SEQ_LEVEL_WATER,                  // 0x05
    SEQ_LEVEL_HOT,                    // 0x06
    SEQ_LEVEL_BOSS_KOOPA,             // 0x07
    SEQ_LEVEL_SNOW,                   // 0x08
    SEQ_LEVEL_SLIDE,                  // 0x09
    SEQ_LEVEL_SPOOKY,                 // 0x0A
    SEQ_EVENT_PIRANHA_PLANT,          // 0x0B
    SEQ_LEVEL_UNDERGROUND,            // 0x0C
    SEQ_MENU_STAR_SELECT,             // 0x0D
    SEQ_EVENT_POWERUP,                // 0x0E
    SEQ_EVENT_METAL_CAP,              // 0x0F
    SEQ_EVENT_KOOPA_MESSAGE,          // 0x10
    SEQ_LEVEL_KOOPA_ROAD,             // 0x11
    SEQ_EVENT_HIGH_SCORE,             // 0x12
    SEQ_EVENT_MERRY_GO_ROUND,         // 0x13
    SEQ_EVENT_RACE,                   // 0x14
    SEQ_EVENT_CUTSCENE_STAR_SPAWN,    // 0x15
    SEQ_EVENT_BOSS,                   // 0x16
    SEQ_EVENT_CUTSCENE_COLLECT_KEY,   // 0x17
    SEQ_EVENT_ENDLESS_STAIRS,         // 0x18
    SEQ_LEVEL_BOSS_KOOPA_FINAL,       // 0x19
    SEQ_EVENT_CUTSCENE_CREDITS,       // 0x1A
    SEQ_EVENT_SOLVE_PUZZLE,           // 0x1B
    SEQ_EVENT_TOAD_MESSAGE,           // 0x1C
    SEQ_EVENT_PEACH_MESSAGE,          // 0x1D
    SEQ_EVENT_CUTSCENE_INTRO,         // 0x1E
    SEQ_EVENT_CUTSCENE_VICTORY,       // 0x1F
    SEQ_EVENT_CUTSCENE_ENDING,        // 0x20
    SEQ_MENU_FILE_SELECT,             // 0x21
    SEQ_EVENT_CUTSCENE_LAKITU,        // 0x22 (not in JP)

    SEQ_COUNT
};

enum BnkId {
    BNK_SOUND_00,                     // 0x00
    BNK_SOUND_TERRAIN,                // 0x01
    BNK_SOUND_WATER,                  // 0x02
    BNK_SOUND_03,                     // 0X03
    BNK_SOUND_04,                     // 0X04
    BNK_SOUND_05,                     // 0X05
    BNK_SOUND_06,                     // 0X06
    BNK_SOUND_07,                     // 0X07
    BNK_SOUND_MARIO,                  // 0X08
    BNK_SOUND_09,                     // 0X09
    BNK_SOUND_MARIO_PEACH,            // 0X0A
    BNK_LEVEL_SNOW,                   // 0X0B
    BNK_UNUSED_0x0C,                  // 0X0C
    BNK_LEVEL_SLIDE,                  // 0x0D
    BNK_LEVEL_INSIDE_CASTLE,          // 0x0E
    BNK_EVENT_CUTSCENE_STAR_SPAWN = BNK_LEVEL_INSIDE_CASTLE, // 0x0E
    BNK_LEVEL_HOT,                    // 0x0F
    BNK_LEVEL_SPOOKY,                 // 0x10
    BNK_MENU_TITLE_SCREEN,            // 0x11
    BNK_LEVEL_BOSS_KOOPA,             // 0x12
    BNK_EVENT_KOOPA_MESSAGE = BNK_LEVEL_BOSS_KOOPA, // 0x12
    BNK_LEVEL_WATER,                  // 0x13
    BNK_EVENT_SOLVE_PUZZLE,           // 0x14
    BNK_EVENT_PIRANHA_PLANT = BNK_EVENT_SOLVE_PUZZLE, // 0x14
    BNK_LEVEL_UNDERGROUND,            // 0x15
    BNK_MENU_STAR_SELECT,             // 0x16
    BNK_EVENT_POWERUP,                // 0x17
    BNK_EVENT_METAL_CAP,              // 0x18
    BNK_LEVEL_KOOPA_ROAD,             // 0x19
    BNK_EVENT_RACE,                   // 0x1A
    BNK_EVENT_CUTSCENE_COLLECT_KEY = BNK_EVENT_RACE, // 0x1A
    BNK_EVENT_CUTSCENE_VICTORY = BNK_EVENT_RACE, // 0x1A
    BNK_EVENT_BOSS,                   // 0x1B
    BNK_EVENT_CUTSCENE_INTRO = BNK_EVENT_BOSS, // 0x1B
    BNK_EVENT_CUTSCENE_LAKITU = BNK_EVENT_BOSS, // 0x1B
    BNK_EVENT_ENDLESS_STAIRS,         // 0x1C
    BNK_LEVEL_BOSS_KOOPA_FINAL,       // 0x1D
    BNK_EVENT_PEACH_MESSAGE,          // 0x1E
    BNK_EVENT_HIGH_SCORE,             // 0x1F
    BNK_EVENT_TOAD_MESSAGE,           // 0x20
    BNK_EVENT_MERRY_GO_ROUND,         // 0x21
    BNK_LEVEL_SPOOKY_2 = BNK_EVENT_MERRY_GO_ROUND, // 0x21
    BNK_LEVEL_GRASS,                  // 0x22
    BNK_EVENT_COLLECT_STAR = BNK_LEVEL_GRASS, // 0x22
    BNK_EVENT_CUTSCENE_ENDING,        // 0x23
    BNK_MENU_FILE_SELECT,             // 0x24
    BNK_EVENT_CUTSCENE_CREDITS,       // 0x25

    BNK_COUNT
};

#endif // SEQ_IDS_H
