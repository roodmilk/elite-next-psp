#ifndef ELITE_AUDIO_INTRO_H
#define ELITE_AUDIO_INTRO_H
#define AUDIO_INTRO_STATION 7
#define AUDIO_INTRO_SKIP_SECONDS 23
#define AUDIO_INTRO_FADE_FRAMES 44100u
/* Output is always 44.1 kHz, regardless of the music file's sample rate. */
static int audio_intro_gain(unsigned frame){
 return frame>=AUDIO_INTRO_FADE_FRAMES?1000:(int)(frame*1000u/AUDIO_INTRO_FADE_FRAMES);
}
#endif
