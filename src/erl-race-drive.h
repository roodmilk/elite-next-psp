/* Race 1 propulsion contract. Pure helpers shared by player, rivals and QA.
 * Automatic sources never spend/refill reserve; brake wins without erasing them. */
#ifndef ERL_RACE_DRIVE_H
#define ERL_RACE_DRIVE_H
#define ERL_CRUISE .70f
#define ERL_AUTO_SURGE 1.33f
#define ERL_MANUAL_SURGE 1.48f
#define ERL_PAD_HALF_WIDTH 36.f
static inline int erl_service_clock(float clock){return clock>=57.f||clock<1.4f;}
#define ERL_SERVICE_LANE .76f
static inline float erl_drive_target(float top,float pace,int manual,int automatic,int brake){
 if(brake)return 0;
 return top*(manual?ERL_MANUAL_SURGE:automatic?ERL_AUTO_SURGE:fminf(ERL_CRUISE,pace));
}
static inline void erl_drive_reserve(float *reserve,int held,int spending,float dt){
 *reserve=fmaxf(0,fminf(100,*reserve+dt*(spending?-25.f:held?0:8.f)));
}
static inline float erl_trailing_gain(float seconds){
 float t=fmaxf(0,fminf(1,(seconds-.35f)/1.15f));return .05f*t*t*(3-2*t);
}
#endif
