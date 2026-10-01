#ifndef ELITE_RADIO_VOICE_H
#define ELITE_RADIO_VOICE_H

/* Text actually spoken by the two generated presenter channels.  The audio
 * thread owns progression and publishes its cursor for the cockpit ticker. */
#define RADIO_VOICE_LINE_COUNT 24
static const char *const radio_voice_lines[2][RADIO_VOICE_LINE_COUNT]={
 {
  "SPACE TALK: Tonight we ask whether a quiet flight is peaceful or merely suspicious.",
  "SPACE TALK: A Lave mechanic says every engine has a voice, usually asking for money.",
  "SPACE TALK: Our first caller insists their navigation computer is holding a grudge.",
  "SPACE TALK: Freight crews report clear lanes, cold coffee and one extremely lost courier.",
  "SPACE TALK: The question tonight is simple. What makes a strange system feel like home?",
  "SPACE TALK: A survey pilot found singing ice and stayed long enough to learn the chorus.",
  "SPACE TALK: Local Law recommends calm flying. Our listeners recommend better hiding places.",
  "SPACE TALK: We asked five traders for market advice and received nine contradictory answers.",
  "SPACE TALK: A caller from the rim says the stars look closer when nobody else is awake.",
  "SPACE TALK: Tonight's argument concerns docking etiquette and who scratched berth twelve.",
  "SPACE TALK: One explorer says every map is a promise that somebody can find their way back.",
  "SPACE TALK: Traffic is light near the inner worlds, apart from a convoy carrying soup.",
  "SPACE TALK: Our guest studies alien weather and refuses to stand beneath ordinary clouds.",
  "SPACE TALK: Listener question. Is buying another ship a plan, or an expensive personality?",
  "SPACE TALK: A bounty hunter called to say the glamorous part is mostly filling out forms.",
  "SPACE TALK: We are opening the channel to anyone with a story and a functioning transmitter.",
  "SPACE TALK: The late shift wants to know why every mysterious signal begins near bedtime.",
  "SPACE TALK: A botanist reports that the glowing vine is harmless unless complimented badly.",
  "SPACE TALK: Tonight we salute the pilots who carry medicine instead of dramatic opinions.",
  "SPACE TALK: A station cook claims vacuum improves flavour. The medical desk strongly disagrees.",
  "SPACE TALK: Our panel says courage is fear with enough fuel for the return journey.",
  "SPACE TALK: Messages are arriving from three systems and one place not marked on any chart.",
  "SPACE TALK: Keep your shields charged, your receipts safe and your emergency snacks closer.",
  "SPACE TALK: This is the open channel. Wherever you are tonight, somebody else is listening."
 },
 {
  "VOID TALES: The first traveller found a door in the dark and knocked for seven years.",
  "VOID TALES: On the eighth year, the door opened inward, though there was no room behind it.",
  "VOID TALES: A voice asked the traveller to name the star they had left.",
  "VOID TALES: They gave three names. The door accepted all of them.",
  "VOID TALES: Beyond the threshold, an old moon was waiting with its lights on.",
  "VOID TALES: The moon remembered every ship that had passed, except one.",
  "VOID TALES: That missing ship was still travelling, somewhere between two breaths.",
  "VOID TALES: The traveller followed its wake and heard singing in the engine heat.",
  "VOID TALES: The song had no words, but it knew the shape of home.",
  "VOID TALES: When the traveller turned back, the door had become a window.",
  "VOID TALES: Outside the glass, the stars were moving like patient lanterns.",
  "VOID TALES: One lantern blinked twice. The traveller answered once.",
  "VOID TALES: The answer returned from behind the moon, older and warmer.",
  "VOID TALES: It said, carry a light for the ones who cannot cross.",
  "VOID TALES: So the traveller lit the smallest lamp and kept going.",
  "VOID TALES: Years later, another pilot found that lamp between the lanes.",
  "VOID TALES: They called it a beacon. The dark called it a promise.",
  "VOID TALES: The pilot followed it until the instruments forgot their numbers.",
  "VOID TALES: There they met a creature made of weather and unfinished maps.",
  "VOID TALES: It asked why humans keep returning to dangerous places.",
  "VOID TALES: The pilot said, because something wonderful may be waiting.",
  "VOID TALES: The creature considered this, then moved one star closer.",
  "VOID TALES: The beacon still burns for anyone willing to listen.",
  "VOID TALES: This tale is not finished. The next voice may be yours."
 }
};

/* Audio-worker writes; renderer reads. Atomic aligned ints are sufficient on
 * PSP and a stale frame is harmless. */
static volatile int radio_voice_station=-1;
static volatile int radio_voice_line=0;
static volatile int radio_voice_char=0;
static volatile int radio_voice_active=0;

static inline const char *radio_voice_script(int station,int line){
 if(station<5||station>6)return "";
 if(line<0)line=0;line%=RADIO_VOICE_LINE_COUNT;
 return radio_voice_lines[station-5][line];
}

#endif
