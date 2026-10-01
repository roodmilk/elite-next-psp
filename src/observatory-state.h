#ifndef OBSERVATORY_STATE_H
#define OBSERVATORY_STATE_H
/* V26 uses five previously reserved bits; no payload or Game growth. */
#define OBS_CHOICE_SHIFT 20
#define OBS_CLUES (7u<<22)
#define SURFACE_V26_MASK (0x0fff7fu| (31u<<20))
static inline int observatory_site(const Game *g,int sys,int body){
 if(sys<0||sys>=256||body<1||body>=BODY_COUNT)return -1;
 for(int i=2;i<10;i++)if(i!=6&&field_site_kind(g,sys,body,i)==FIELD_DISH)return i;
 return -1;
}
static inline int observatory_choice(const Game *g,int sys,int body){return (g->surface_progress[sys][body]>>OBS_CHOICE_SHIFT)&3;}
static inline int observatory_flags_valid(const Game *g,int sys,int body,uint32_t f){
 if(f&~SURFACE_V26_MASK)return 0;
 unsigned state=f>>20;if(!state)return 1;
 int id=observatory_site(g,sys,body);if(id<0||(state&3)==3)return 0;
 return !(state&3)||((f&field_site_bit(id))&&(f&OBS_CLUES)==OBS_CLUES);
}
static inline const char *observatory_report(int choice){
 return choice==1?"You restored the public beacon and accepted a 20-unit service bonus. The receiver now carries a clean reference signal; the faint trace was lost during the reset.":choice==2?"You preserved the faint trace before recalibrating. Its origin remains unknown. The recording is filed as an extra discovery; no beacon-service bonus was claimed.":"This observatory was surveyed before the signal investigation was recorded. Its earlier completion remains valid; no new choice or reward has been invented.";
}
#endif
