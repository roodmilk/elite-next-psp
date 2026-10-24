/* V27: six opening words plus a bounded, portable checkpoint snapshot.
 * Ordinary commanders retain the historical docked-load behaviour. */
#define OPENING_SAVE_BYTES (33*4)
typedef struct {OpeningState state;int docked,planet,surface,approach,damaged;float v[22];} OpeningSave;
int opening_save_allowed(const Game *g){
 return !g->dead&&(g->docked||(g->dockbay.ready&&g->dockbay.resume&&!g->jump&&!g->dock_stage&&g->planet<0)||((g->erl.event_kind||(g->race.arcade.ready&&g->race.arcade.rules.context==2&&g->race.phase>=RACE_COUNTDOWN&&g->race.job>=0&&g->race.system==g->system))&&g->planet<0&&!g->jump&&!g->dock_stage)||(g->opening.stage>0&&g->opening.stage<1000&&
 !g->jump&&!g->dock_stage&&!g->planet_sequence&&!g->local_pilot));
}
static int opening_save_write(FILE *f,const Game *g){
 uint32_t state[4]={(uint32_t)g->opening.stage,(uint32_t)g->opening.flags,(uint32_t)g->opening.choice,(uint32_t)g->opening.counter};
 for(int i=0;i<4;i++)if(!save_u32(f,state[i]))return 0;
 float progress[2]={g->opening.distance,g->opening.elapsed};
 for(int i=0;i<2;i++){uint32_t u;memcpy(&u,&progress[i],4);if(!save_u32(f,u))return 0;}
 int ints[5]={g->docked,g->planet,g->surface,g->approach,g->damaged};
 for(int i=0;i<5;i++)if(!save_u32(f,(uint32_t)(int32_t)ints[i]))return 0;
 float v[22]={g->pos.x,g->pos.y,g->pos.z,g->yaw,g->pitch,g->roll,g->speed,g->energy,g->hull,g->heat,
 g->orbit_pos.x,g->orbit_pos.y,g->orbit_pos.z,g->orbit_yaw,g->orbit_pitch,g->orbit_roll,g->orbit_speed,
 g->ship_pos.x,g->ship_pos.y,g->ship_pos.z,g->time,g->jetpack};
 for(int i=0;i<22;i++){uint32_t u;memcpy(&u,&v[i],4);if(!save_u32(f,u))return 0;}return 1;
}
static int opening_save_read(FILE *f,OpeningSave *s){
 uint32_t u[11];for(int i=0;i<11;i++)if(!load_u32(f,&u[i]))return 0;
 s->state.stage=(int)u[0];s->state.flags=(int)u[1];s->state.choice=(int)u[2];s->state.counter=(int)u[3];
 memcpy(&s->state.distance,&u[4],4);memcpy(&s->state.elapsed,&u[5],4);
 s->docked=(int32_t)u[6];s->planet=(int32_t)u[7];s->surface=(int32_t)u[8];s->approach=(int32_t)u[9];s->damaged=(int32_t)u[10];
 for(int i=0;i<22;i++){uint32_t bits;if(!load_u32(f,&bits))return 0;memcpy(&s->v[i],&bits,4);if(!isfinite(s->v[i])||fabsf(s->v[i])>10000000)return 0;}
 if((u[0]>OPENING_STAGE_COUNT&&u[0]!=OP_FREE)||(u[1]&~0xF1FFFu)||u[2]>2||u[3]>100000||!isfinite(s->state.distance)||s->state.distance<0||s->state.distance>10000000||!isfinite(s->state.elapsed)||s->state.elapsed<0||s->state.elapsed>10000000)return 0;
 if(s->docked<0||s->docked>1||s->planet< -1||s->planet>=BODY_COUNT||s->surface<0||s->surface>2||s->approach< -1||s->approach>=BODY_COUNT||s->damaged<0||s->damaged>1)return 0;
 if(s->state.stage>0&&s->state.stage<1000){
  if((s->surface&&s->planet<1)||(s->docked&&s->surface)||s->v[7]<0||s->v[7]>100||s->v[8]<=0||s->v[8]>100||s->v[9]<0||s->v[9]>1000)return 0;
 }return 1;
}
static void opening_save_restore(Game *g,const OpeningSave *s){
 g->opening=s->state;if(!g->opening.stage)return;
 opening_world_apply(g);system_bodies(g);
 if(g->opening.stage>=1000)return;
 g->docked=s->docked;g->planet=s->planet;g->surface=s->surface;g->approach=s->approach;g->damaged=s->damaged;
 g->pos=(Vec3){s->v[0],s->v[1],s->v[2]};g->yaw=s->v[3];g->pitch=s->v[4];g->roll=s->v[5];g->speed=s->v[6];g->energy=s->v[7];g->hull=s->v[8];g->heat=s->v[9];
 g->orbit_pos=(Vec3){s->v[10],s->v[11],s->v[12]};g->orbit_yaw=s->v[13];g->orbit_pitch=s->v[14];g->orbit_roll=s->v[15];g->orbit_speed=s->v[16];
 g->ship_pos=(Vec3){s->v[17],s->v[18],s->v[19]};g->time=s->v[20];g->jetpack=s->v[21];
 /* Arrival/docking animations and encounters are not replayed on resume. */
 g->dead=g->boost=g->dock_stage=g->planet_sequence=g->rover_driving=0;g->jump=g->incoming_missile=0;
}
