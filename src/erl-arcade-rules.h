/* Shared single-player ERL/contract rules. Integer damage prevents a 25th
 * bullet caused by floating-point rounding. No ordinary-flight resources. */
#ifndef ERL_ARCADE_RULES_H
#define ERL_ARCADE_RULES_H
#include <stdint.h>
#include <string.h>
#include <math.h>
enum {RA_CLEAN,RA_COMBAT,RA_TIME_TRIAL,RA_MODE_COUNT};
enum {RA_SOLO,RA_PACK,RA_HEADSTART,RA_FORMAT_COUNT};
enum {RA_RINGS,RA_CORRIDOR,RA_COURSE_COUNT};
enum {RA_FINISH,RA_MEDALS,RA_TAKEDOWNS,RA_ELIMINATOR,RA_HUNTER,RA_ESCAPE,RA_ENDURANCE,RA_OBJECTIVE_COUNT};
enum {RA_EMPTY,RA_BOOST,RA_MISSILE,RA_MINIGUN,RA_SHIELD,RA_ITEM_COUNT};
#define RA_POOL_CLEAN ((1u<<RA_BOOST)|(1u<<RA_SHIELD))
#define RA_POOL_ALL (RA_POOL_CLEAN|(1u<<RA_MISSILE)|(1u<<RA_MINIGUN))
static inline int ra_shuffle_icon(uint32_t pool,float remaining){
 int allowed[4],count=0;for(int i=1;i<RA_ITEM_COUNT;i++)if(pool&(1u<<i))allowed[count++]=i;
 return count?allowed[(int)(fmaxf(0,remaining)*30)%count]:RA_EMPTY;
}
#define RA_HEALTH 2400
#define RA_REPAIR_HALF_LENGTH 3000.f
#define RA_BULLET_DAMAGE 100
#define RA_MISSILE_DAMAGE 1200
typedef struct {
 uint32_t mode,format,course,objective,pool,field,laps,seconds,required,expert;
 uint32_t seed,sponsor,context;
 float gold,silver,bronze;
} RaceRules;
typedef struct {
 int health,item,pending,ammo,require_release,previous_x,eliminated;
 float shuffle,shield,rocket,protection,spin,fire_clock,shield_break,flash;
 float capture,contact,decision,respawn;
} RaceItemActor;
enum {RA_DAMAGE_NONE,RA_DAMAGE_HIT,RA_DAMAGE_BLOCKED,RA_DAMAGE_SHIELD_BREAK,RA_DAMAGE_TAKEDOWN};
static inline int ra_rules_valid(const RaceRules *q,int licence){
 if(q->mode>=RA_MODE_COUNT||q->format>=RA_FORMAT_COUNT||q->course>=RA_COURSE_COUNT||q->objective>=RA_OBJECTIVE_COUNT||q->field<1||q->field>15||q->laps<1||q->laps>8||q->seconds<30||q->seconds>900||q->required>100||q->expert>1||q->sponsor>4||q->context>3||!q->pool||(q->pool&~RA_POOL_ALL))return 0;
 if(!isfinite(q->gold)||!isfinite(q->silver)||!isfinite(q->bronze)||q->gold<20||q->silver<q->gold||q->bronze<q->silver||q->bronze>900)return 0;
 if(q->mode==RA_TIME_TRIAL)return q->format==RA_SOLO&&q->field==1&&q->course==RA_RINGS&&q->objective==RA_MEDALS&&q->pool==(1u<<RA_BOOST)&&!q->expert;
 if(q->mode==RA_CLEAN&&((q->pool&~RA_POOL_CLEAN)||q->objective==RA_TAKEDOWNS||q->course!=RA_RINGS||q->expert))return 0;
 if(q->mode==RA_COMBAT&&q->course==RA_RINGS&&(!q->expert||licence<2))return 0;
 if(q->expert&&(q->mode!=RA_COMBAT||q->course!=RA_RINGS))return 0;
 if(q->objective==RA_MEDALS)return 0;
 if(q->objective==RA_TAKEDOWNS)return q->mode==RA_COMBAT&&q->format==RA_SOLO&&q->course==RA_CORRIDOR&&q->required>0&&(q->pool&((1u<<RA_MISSILE)|(1u<<RA_MINIGUN)));
 if(q->format==RA_SOLO)return 0;
 if(q->objective==RA_HUNTER||q->objective==RA_ESCAPE)return q->format==RA_HEADSTART&&q->field==2;
 if(q->format!=RA_PACK||q->field<2)return 0;
 return q->objective!=RA_ENDURANCE||q->seconds>=240;
}
/* Catalogue axes are independent, subject to validity, rather than a fixed
 * list of ten implementations. Callers curate these combinations by licence. */
static inline RaceRules ra_rules_make(unsigned mode,unsigned objective,unsigned pool,unsigned seed){
 RaceRules q={0};q.mode=mode;q.objective=objective;q.pool=pool;q.seed=seed;
 q.format=objective==RA_MEDALS||objective==RA_TAKEDOWNS?RA_SOLO:objective==RA_HUNTER||objective==RA_ESCAPE?RA_HEADSTART:RA_PACK;
 q.field=q.format==RA_SOLO?1:q.format==RA_HEADSTART?2:7;
 q.course=mode==RA_COMBAT?RA_CORRIDOR:RA_RINGS;q.laps=2;q.seconds=objective==RA_ENDURANCE?360:120;q.required=objective==RA_TAKEDOWNS?5:0;
 q.gold=q.seconds;q.silver=q.gold*1.15f;q.bronze=q.gold*1.3f;return q;
}
static inline void ra_actor_reset(RaceItemActor *a){memset(a,0,sizeof(*a));a->health=RA_HEALTH;}
static inline int ra_roll(unsigned random,unsigned pool,int place,int field){
 unsigned weights[5]={0,100,100,100,100};
 /* Slight bias, never a guarantee; every permitted item retains weight. */
 if(field>1&&place>field/2){weights[RA_BOOST]+=12;weights[RA_MISSILE]+=8;}
 unsigned total=0;for(int k=1;k<RA_ITEM_COUNT;k++)if(pool&(1u<<k))total+=weights[k];
 if(!total)return RA_EMPTY;unsigned pick=random%total;
 for(int k=1;k<RA_ITEM_COUNT;k++)if(pool&(1u<<k)){if(pick<weights[k])return k;pick-=weights[k];}return RA_EMPTY;
}
static inline void ra_collect(RaceItemActor *a,int item,int held){
 a->item=RA_EMPTY;a->pending=item;a->ammo=0;a->shuffle=.3f;
 a->require_release=!!held;a->fire_clock=0;a->previous_x=!!held;
 /* Activated shield/rocket effects deliberately remain untouched. */
}
static inline void ra_actor_tick(RaceItemActor *a,float dt){
 if(dt<=0)return;
 if(a->shuffle>0){a->shuffle=fmaxf(0,a->shuffle-dt);if(!a->shuffle){a->item=a->pending;a->pending=RA_EMPTY;a->ammo=a->item==RA_MINIGUN?48:0;a->flash=.2f;}}
 a->shield=fmaxf(0,a->shield-dt);a->rocket=fmaxf(0,a->rocket-dt);a->protection=fmaxf(0,a->protection-dt);a->spin=fmaxf(0,a->spin-dt);
 a->shield_break=fmaxf(0,a->shield_break-dt);a->flash=fmaxf(0,a->flash-dt);a->contact=fmaxf(0,a->contact-dt);a->decision=fmaxf(0,a->decision-dt);a->respawn=fmaxf(0,a->respawn-dt);
}
static inline int ra_damage(RaceItemActor *a,int damage,int breaks_shield){
 if(damage<=0||a->eliminated||a->health<=0||a->protection>0)return RA_DAMAGE_NONE;
 if(a->shield>0){if(breaks_shield){a->shield=0;a->shield_break=.35f;return RA_DAMAGE_SHIELD_BREAK;}return RA_DAMAGE_BLOCKED;}
 a->health=a->health>damage?a->health-damage:0;a->flash=.12f;
 if(!a->health){a->item=a->pending=RA_EMPTY;a->ammo=0;a->shield=a->rocket=a->shuffle=0;a->require_release=1;return RA_DAMAGE_TAKEDOWN;}
 return RA_DAMAGE_HIT;
}
/* Returns the actual action (or bullet count through *rounds). Missile lock is
 * eligibility, not normal targeting. A failed lock does not consume the item. */
static inline int ra_use(RaceItemActor *a,int held,int available,int missile_lock,float dt,int *rounds){
 int edge=held&&!a->previous_x;a->previous_x=!!held;*rounds=0;
 if(!held){a->require_release=0;a->fire_clock=0;}
 if(!available||a->require_release||a->shuffle>0||a->eliminated||a->health<=0)return RA_EMPTY;
 if(a->item==RA_MINIGUN){
  if(!held||dt<=0)return RA_EMPTY;
  a->fire_clock+=dt*12;int n=(int)(a->fire_clock+1.e-5f);if(n>a->ammo)n=a->ammo;
  if(n){a->fire_clock-=n;a->ammo-=n;*rounds=n;a->flash=.06f;if(!a->ammo)a->item=RA_EMPTY;return RA_MINIGUN;}return RA_EMPTY;
 }
 if(!edge)return RA_EMPTY;
 int item=a->item;if(item==RA_MISSILE&&!missile_lock)return RA_EMPTY;
 if(item==RA_BOOST)a->rocket=.65f;else if(item==RA_SHIELD)a->shield=3;
 if(item){a->item=RA_EMPTY;a->flash=.2f;}return item;
}
static inline void ra_respawn(RaceItemActor *a){ra_actor_reset(a);a->protection=3;a->require_release=1;}
static inline int ra_medal_points(float time,const RaceRules *q){return time<=q->gold?10:time<=q->silver?7:time<=q->bronze?5:1;}
static inline float ra_rival_trial_time(const RaceRules *q,int id,int division){
 uint32_t h=q->seed^(uint32_t)(id+1)*2654435761u;h^=h>>16;h*=2246822519u;h^=h>>13;
 /* Fixed seed and persistent driver ID: reloads cannot reroll the field. */
 return q->gold*(.97f+(h%2801)*.0001f+fmaxf(0,3-division)*.015f);
}
#endif
