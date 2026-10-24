static void graphics_scene(int view){
 game_init(&game);story_complete(&game);game.system=7;system_bodies(&game);launch(&game);
 page=FLIGHT;paused=high_contrast=hud_mode=hud_hidden=third_person=0;preview_reset();camera_override_clear();flight_camera_seen=-1;
 game.voice_time=game.message_time=game.jump=0;game.dock_stage=0;game.planet=game.approach=-1;game.police_stop=0;game.time=50;game.speed=0;game.yaw=game.pitch=game.roll=0;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;for(int i=0;i<DEBRIS_COUNT;i++)game.debris[i].alive=0;
 game.pos=(Vec3){0,200,STATION_Z-4500};selected_target=BODY_COUNT;autoaim=0;
}

#include "graphics-stations-review.h"
#include "graphics-ships-review.h"

#include "soft-sky-review.h"


static int station_perf_review(void){
 FILE*f=fopen("station-perf.flag","r");if(!f)return 0;fclose(f);f=fopen("station-perf.txt","w");graphics_scene(0);soft_sky_prepare(space_sky_seed());render_profile_enabled=1;
 unsigned *saved=malloc(STRIDE*H*4);int fail=0;
 for(int pose=0;pose<3;pose++){
  game.pos=(Vec3){0,200,STATION_Z-(pose==0?4500:pose==1?2300:1400)};game.yaw=game.pitch=game.roll=0;
  for(int mode=0;mode<2;mode++){
   mc_span_fast=mode;memset(render_phase_ms,0,sizeof(render_phase_ms));
   for(int i=0;i<24;i++){drawcount=0;space();}
   fprintf(f,"POSE %d mode%d",pose,mode);for(int j=0;j<7;j++)fprintf(f," phase%d=%.3f",j,render_phase_ms[j]/24);fprintf(f,"\n");
   if(!mode)memcpy(saved,fb,STRIDE*H*4);else{int changed=0,worst=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++){unsigned a=saved[y*STRIDE+x],b=fb[y*STRIDE+x];int d=abs((int)(a&255)-(int)(b&255))+abs((int)((a>>8)&255)-(int)((b>>8)&255))+abs((int)((a>>16)&255)-(int)((b>>16)&255));changed+=d!=0;if(d>worst)worst=d;}fprintf(f,"INFO changed pixels%d largest RGB delta%d\n",changed,worst);fail+=changed>W*H/1000;}
   char name[50];snprintf(name,sizeof(name),"station-speed-%d-%d.bmp",pose,mode);dump_native_bmp(name);fflush(f);
  }
 }
 free(saved);mc_span_fast=1;fprintf(f,"RESULT %d failures\n",fail);fclose(f);return 1;
}

static int deep_art_review(void){
 FILE*f=fopen("deep-art-review.flag","r");if(!f)return 0;fclose(f);f=fopen("deep-art-review.txt","w");graphics_scene(0);soft_sky_prepare(space_sky_seed());
 int fail=0;unsigned*old=fb,*mem=calloc(STRIDE*H+32,4);if(!mem){fclose(f);return 1;}fb=mem+16;for(int i=0;i<16;i++)mem[i]=mem[STRIDE*H+16+i]=0xb5712441;
 float heading=0,phase=0;Vec3 pos;for(int second=0;second<500;second++){game.time=second;pos=deep_traffic_position(0,&heading,&phase);if(phase>.38f&&phase<.62f)break;}
 float base=game.time;Vec3 facing={sinf(heading),0,cosf(heading)},side={facing.z,0,-facing.x};double worst=0;
 for(int pose=0;pose<6;pose++){
  game.time=base;pos=deep_traffic_position(0,&heading,&phase);
  Vec3 offset=pose==0?(Vec3){-40000,7000,-50000}:pose==1?mul(side,42000):pose==2?mul(facing,-42000):pose==3?mul(facing,42000):pose==4?add(mul(side,26000),(Vec3){0,14000,0}):mul(side,120000);
  game.pos=(Vec3){150000,0,0};deep_staged_override=1;deep_staged_pos=add(game.pos,(Vec3){pose==4?7000:9000,2500,pose==5?120000:pose==4?30000:65000});deep_staged_heading=pose==2?0:pose==3?3.14159265f:pose==4?1.0f:1.5707963f;game.yaw=game.pitch=0;game.roll=pose==4?.15f:0;
  uint64_t t=sceKernelGetSystemTimeWide();rect(0,0,W,H,BG);drawcount=0;space();double ms=(sceKernelGetSystemTimeWide()-t)*.001;if(ms>worst)worst=ms;char name[64];snprintf(name,sizeof(name),"freighter-pose-%d.bmp",pose);dump_native_bmp(name);fprintf(f,"PASS native freighter pose%d captured render%.3fms\n",pose,ms);fflush(f);
  if(pose==2)for(int frame=0;frame<24;frame++){game.time=base+frame*.08f;rect(0,0,W,H,BG);drawcount=0;space();snprintf(name,sizeof(name),"freighter-motion-%02d.bmp",frame);dump_native_bmp(name);}
 }
 int safe=1;for(int i=0;i<16;i++)safe&=mem[i]==0xb5712441&&mem[STRIDE*H+16+i]==0xb5712441;fail+=!safe;fprintf(f,"%s framebuffer guards intact; wake budget6500 shared samples\n",safe?"PASS":"FAIL");fprintf(f,"INFO six staged cameras; no physical PSP/fleet acceptance claim; worst render%.3fms\nRESULT %d failures\n",worst,fail);fclose(f);fb=old;free(mem);deep_staged_override=0;return 1;
}
