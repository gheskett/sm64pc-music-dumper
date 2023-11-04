#pragma once

/*****************************
 * COMPATIBILITY SAFE GUARDS *
 *****************************/

/**
 * NOTE: Only mess with these if you know what you're doing!
 * These are put in place to insure that connected functionality works as intended.
 */


/*****************
 * config_audio.h
 */

// Anything higher than 64 will most likely crash on boot. Even if it doesn't, it's still dangerous.
#if (MAX_SIMULTANEOUS_NOTES > 64)
    #undef MAX_SIMULTANEOUS_NOTES
    #define MAX_SIMULTANEOUS_NOTES 64
#elif (MAX_SIMULTANEOUS_NOTES < 0)
    #undef MAX_SIMULTANEOUS_NOTES
    #define MAX_SIMULTANEOUS_NOTES 0
#endif

#if defined(BETTER_REVERB) && !(defined(VERSION_US) || defined(VERSION_JP))
    #undef BETTER_REVERB
#endif

#ifndef VIBRATO_MODIFIER
    #define VIBRATO_MODIFIER 0
#endif
#ifndef ADSR_MODIFIER
    #define ADSR_MODIFIER 0
#endif
