/* Pure bar mechanics. Mutate a tentative snapshot, never live Game/save/audio.
 * The persistence owner must atomically commit phase/result AND credits before
 * publishing the returned event or revealing dice. No durability is provided here. */
#ifndef ELITE_BAR_GAMEPLAY_H
#define ELITE_BAR_GAMEPLAY_H
#include <stdint.h>
#include <math.h>

#define BAR_CREDIT_MAX 100000000
#define BAR_MISSION_CAPACITY 5
#define BAR_RECOVERY_DISTANCE_SQUARED 400.0f /* 20 metres, inclusive */
#define BAR_RECOVERY_EYE_ERROR 2.0f
enum { BAR_JOB_NONE, BAR_JOB_ACCEPTED, BAR_JOB_RECOVERED, BAR_JOB_PAID };
enum { BAR_LOW, BAR_HIGH };
enum { BAR_EVENT_NONE, BAR_EVENT_JOB_ACCEPTED, BAR_EVENT_JOB_RECOVERED,
       BAR_EVENT_JOB_PAID, BAR_EVENT_JOB_ABANDONED, BAR_EVENT_DICE_SETTLED };
enum { BAR_NO_CHANGE=0, BAR_APPLIED=1, BAR_ERR_STATE=-1, BAR_ERR_CONTEXT=-2,
       BAR_ERR_CAPACITY=-3, BAR_ERR_ACCESS=-4, BAR_ERR_CREDITS=-5,
       BAR_ERR_ARGUMENT=-6, BAR_ERR_TOKEN=-7, BAR_ERR_EVIDENCE=-8 };

typedef struct { uint32_t phase; } BarJobState;
typedef struct {
 uint32_t mission_id,bar_id,origin_system,origin_hub,recipient_id;
 uint32_t target_system,target_body,site_id,reward;
} BarJobSpec;
typedef struct {
 uint32_t bar_id,system,hub,recipient_id;
 int docked,alive,can_land,site_enabled;
} BarVisit;
/* Planetary alone projects the actual Game/site into this context. */
typedef struct {
 uint32_t mission_id,system,body,site_id;
 int alive,on_foot,grounded,pressed_recover;
 float planar_distance_squared,eye_ground_error;
} BarRecovery;
typedef struct { uint32_t round_id,rng,choice,stake,die1,die2; } BarDiceState;
typedef struct { int status,event; } BarResult;
static inline BarResult bar_result(int status,int event){BarResult r={status,event};return r;}
static inline int bar_job_valid(const BarJobState *s){return s&&s->phase<=BAR_JOB_PAID;}
static inline int bar_job_spec_valid(const BarJobSpec *s){
 return s&&s->mission_id&&s->bar_id&&s->recipient_id&&s->site_id&&
  s->origin_system<256&&s->origin_hub<3&&s->target_system<256&&
  s->target_body>=1&&s->target_body<5&&s->reward>0&&s->reward<=BAR_CREDIT_MAX;
}
static inline int bar_job_occupies_slot(const BarJobState *s){
 return bar_job_valid(s)&&(s->phase==BAR_JOB_ACCEPTED||s->phase==BAR_JOB_RECOVERED);
}
/* Use from BOTH ordinary-board and bar acceptance adapters. Invalid state fails closed. */
static inline int bar_mission_capacity_available(const BarJobState *s,int ordinary_jobs){
 return bar_job_valid(s)&&ordinary_jobs>=0&&ordinary_jobs<BAR_MISSION_CAPACITY&&
  ordinary_jobs+bar_job_occupies_slot(s)<BAR_MISSION_CAPACITY;
}
static inline int bar_job_exact_visit(const BarJobSpec *s,const BarVisit *v){
 return v&&v->docked==1&&v->alive==1&&v->bar_id==s->bar_id&&
  v->system==s->origin_system&&v->hub==s->origin_hub&&v->recipient_id==s->recipient_id;
}
static inline BarResult bar_job_accept(BarJobState *s,const BarJobSpec *spec,const BarVisit *v,int ordinary_jobs){
 if(!bar_job_valid(s)||!bar_job_spec_valid(spec))return bar_result(BAR_ERR_ARGUMENT,0);
 if(!bar_job_exact_visit(spec,v))return bar_result(BAR_ERR_CONTEXT,0);
 if(s->phase!=BAR_JOB_NONE)return bar_result(BAR_ERR_STATE,0);
 if(v->can_land!=1||v->site_enabled!=1)return bar_result(BAR_ERR_ACCESS,0);
 if(!bar_mission_capacity_available(s,ordinary_jobs))return bar_result(BAR_ERR_CAPACITY,0);
 s->phase=BAR_JOB_ACCEPTED;return bar_result(BAR_APPLIED,BAR_EVENT_JOB_ACCEPTED);
}
static inline BarResult bar_job_recover(BarJobState *s,const BarJobSpec *spec,const BarRecovery *e){
 if(!bar_job_valid(s)||!bar_job_spec_valid(spec)||!e)return bar_result(BAR_ERR_ARGUMENT,0);
 if(e->mission_id!=spec->mission_id||e->system!=spec->target_system||e->body!=spec->target_body||e->site_id!=spec->site_id||
    e->alive!=1||e->on_foot!=1||e->grounded!=1||e->pressed_recover!=1||
    !isfinite(e->planar_distance_squared)||e->planar_distance_squared<0||e->planar_distance_squared>BAR_RECOVERY_DISTANCE_SQUARED||
    !isfinite(e->eye_ground_error)||fabsf(e->eye_ground_error)>BAR_RECOVERY_EYE_ERROR)return bar_result(BAR_ERR_CONTEXT,0);
 if(s->phase==BAR_JOB_RECOVERED)return bar_result(BAR_NO_CHANGE,0);
 if(s->phase!=BAR_JOB_ACCEPTED)return bar_result(BAR_ERR_STATE,0);
 s->phase=BAR_JOB_RECOVERED;return bar_result(BAR_APPLIED,BAR_EVENT_JOB_RECOVERED);
}
static inline BarResult bar_job_hand_in(BarJobState *s,const BarJobSpec *spec,const BarVisit *v,int *credits){
 if(!bar_job_valid(s)||!bar_job_spec_valid(spec)||!credits)return bar_result(BAR_ERR_ARGUMENT,0);
 if(!bar_job_exact_visit(spec,v))return bar_result(BAR_ERR_CONTEXT,0);
 if(s->phase==BAR_JOB_PAID)return bar_result(BAR_NO_CHANGE,0);
 if(s->phase!=BAR_JOB_RECOVERED)return bar_result(BAR_ERR_STATE,0);
 if(*credits<0||*credits>BAR_CREDIT_MAX-(int)spec->reward)return bar_result(BAR_ERR_CREDITS,0);
 *credits+=(int)spec->reward;s->phase=BAR_JOB_PAID;return bar_result(BAR_APPLIED,BAR_EVENT_JOB_PAID);
}
/* Cancel only before recovery. Never discard a recovered recorder or reopen Paid. */
static inline BarResult bar_job_abandon(BarJobState *s){
 if(!bar_job_valid(s))return bar_result(BAR_ERR_ARGUMENT,0);
 if(s->phase==BAR_JOB_RECOVERED)return bar_result(BAR_ERR_EVIDENCE,0);
 if(s->phase!=BAR_JOB_ACCEPTED)return bar_result(BAR_ERR_STATE,0);
 s->phase=BAR_JOB_NONE;return bar_result(BAR_APPLIED,BAR_EVENT_JOB_ABANDONED);
}
static inline int bar_dice_stake_valid(uint32_t stake){return stake==10||stake==50||stake==100;}
/* Return includes the original stake. -1 means invalid input. */
static inline int bar_dice_return(uint32_t choice,uint32_t stake,uint32_t d1,uint32_t d2){
 if(choice>BAR_HIGH||!bar_dice_stake_valid(stake)||d1<1||d1>6||d2<1||d2>6)return -1;
 uint32_t sum=d1+d2;if(sum==7)return (int)stake;
 return ((choice==BAR_LOW&&sum<7)||(choice==BAR_HIGH&&sum>7))?(int)(2*stake):0;
}
static inline int bar_dice_valid(const BarDiceState *s){
 if(!s)return 0;
 if(s->round_id==0)return s->choice==0&&s->stake==0&&s->die1==0&&s->die2==0;
 return bar_dice_return(s->choice,s->stake,s->die1,s->die2)>=0;
}
/* Dedicated 32-bit counter/permutation stream. No global or simulation RNG.
 * Reject the four excess uint32 values instead of introducing modulo-six bias. */
static inline uint32_t bar_dice_random(uint32_t *state){
 uint32_t x=(*state+=UINT32_C(0x9e3779b9));
 x=(x^(x>>16))*UINT32_C(0x21f0aaad);x=(x^(x>>15))*UINT32_C(0x735a2d97);return x^(x>>15);
}
static inline uint32_t bar_dice_roll(uint32_t *state){uint32_t x;do{x=bar_dice_random(state);}while(x>=UINT32_C(4294967292));return 1+x%6;}
/* expected_round_id is captured by the explicit confirmation UI. Reuse it on
 * retries; never regenerate it from new state for a repeated button/reveal. */
static inline BarResult bar_dice_confirm(BarDiceState *s,int *credits,uint32_t expected_round_id,uint32_t choice,uint32_t stake){
 if(!bar_dice_valid(s)||!credits||choice>BAR_HIGH||!bar_dice_stake_valid(stake))return bar_result(BAR_ERR_ARGUMENT,0);
 if(s->round_id==UINT32_MAX||expected_round_id!=s->round_id+1)return bar_result(BAR_ERR_TOKEN,0);
 if(*credits<(int)stake||*credits>BAR_CREDIT_MAX-(int)stake)return bar_result(BAR_ERR_CREDITS,0);
 BarDiceState next=*s;next.choice=choice;next.stake=stake;next.die1=bar_dice_roll(&next.rng);next.die2=bar_dice_roll(&next.rng);next.round_id=expected_round_id;
 int balance=*credits-(int)stake+bar_dice_return(choice,stake,next.die1,next.die2);
 *s=next;*credits=balance;return bar_result(BAR_APPLIED,BAR_EVENT_DICE_SETTLED);
}
/* Free preview activity. This type has no stake, balance, mission or save data.
 * Keep one instance for the app session; back/exit/reveal never call confirm.
 * UI labels: "FREE PRACTICE" / "No credits or rewards. Resets on restart." */
typedef struct { uint32_t round_id,rng,choice,die1,die2; } BarPracticeState;
enum { BAR_PRACTICE_LOSS=-1, BAR_PRACTICE_DRAW=0, BAR_PRACTICE_WIN=1 };
static inline int bar_practice_valid(const BarPracticeState *s){
 if(!s)return 0;
 if(!s->round_id)return !s->choice&&!s->die1&&!s->die2;
 return s->choice<=BAR_HIGH&&s->die1>=1&&s->die1<=6&&s->die2>=1&&s->die2<=6;
}
/* Call outcome only for a valid state with round_id > 0. */
static inline int bar_practice_outcome(const BarPracticeState *s){
 uint32_t sum=s->die1+s->die2;
 if(sum==7)return BAR_PRACTICE_DRAW;
 return ((s->choice==BAR_LOW&&sum<7)||(s->choice==BAR_HIGH&&sum>7))?BAR_PRACTICE_WIN:BAR_PRACTICE_LOSS;
}
static inline BarResult bar_practice_confirm(BarPracticeState *s,uint32_t expected_round_id,uint32_t choice){
 if(!bar_practice_valid(s)||choice>BAR_HIGH)return bar_result(BAR_ERR_ARGUMENT,0);
 if(s->round_id==UINT32_MAX||expected_round_id!=s->round_id+1)return bar_result(BAR_ERR_TOKEN,0);
 BarPracticeState next=*s;next.choice=choice;
 next.die1=bar_dice_roll(&next.rng);next.die2=bar_dice_roll(&next.rng);next.round_id=expected_round_id;
 *s=next;return bar_result(BAR_APPLIED,BAR_EVENT_DICE_SETTLED);
}
#endif
