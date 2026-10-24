/* Player-driven Race1 telemetry. No steering, scripted inputs, captures,
 * save changes, frame skips or file I/O during active racing. Pause/results
 * append the measured segment; the clock is the actual device kernel clock. */
static struct {
 unsigned serial,seed,work_frames,intervals,spans[5],slow100,dirty;
 uint64_t last,start,interval_us,work_us;
 unsigned worst_us,work_worst;
 int vcount,cpu_min,cpu_max;
 uint64_t sim_us,compose_us,mark;unsigned countdown_frames,running_frames;
 float age;
} erl_hw;
static int erl_hw_active(void){return page==FLIGHT&&!paused&&!game.erl.paused&&game.race.layout==5&&(game.race.phase==RACE_RUNNING||game.race.phase==RACE_COUNTDOWN);}
static void erl_hw_begin(void){
 if(!erl_hw_active()){erl_hw.last=erl_hw.start=0;return;}
 if(erl_hw.serial!=game.erl.serial||erl_hw.seed!=game.race.seed||game.race.elapsed<erl_hw.age){memset(&erl_hw,0,sizeof(erl_hw));erl_hw.serial=game.erl.serial;erl_hw.seed=game.race.seed;memset(erl_render_us,0,sizeof(erl_render_us));erl_render_frames=0;memset(neon_world_us,0,sizeof(neon_world_us));neon_world_frames=0;memset(neon_road_us,0,sizeof(neon_road_us));neon_road_frames=0;}
 erl_hw.age=game.race.elapsed;uint64_t now=sceKernelGetSystemTimeWide();int v=sceDisplayGetVcount();
 if(erl_hw.last){unsigned us=(unsigned)(now-erl_hw.last);erl_hw.interval_us+=us;erl_hw.intervals++;if(us>erl_hw.worst_us)erl_hw.worst_us=us;erl_hw.slow100+=us>100000;
  int span=v-erl_hw.vcount;erl_hw.spans[span<2?0:span==2?1:span==3?2:span==4?3:4]++;
 }
 erl_hw.last=erl_hw.start=now;erl_hw.vcount=v;
 if(!(erl_hw.work_frames&63)){int cpu=scePowerGetCpuClockFrequencyInt();if(!erl_hw.cpu_min||cpu<erl_hw.cpu_min)erl_hw.cpu_min=cpu;if(cpu>erl_hw.cpu_max)erl_hw.cpu_max=cpu;}
}
static void erl_hw_mark(int stage){
 if(!erl_hw.start)return;uint64_t now=sceKernelGetSystemTimeWide();
 if(stage==0)erl_hw.sim_us+=now-erl_hw.start;
 else if(stage==1)erl_hw.mark=now;
 else erl_hw.compose_us+=now-erl_hw.mark;
}
static void erl_hw_end(void){
 if(erl_hw_active()){
  if(erl_hw.start){unsigned us=(unsigned)(sceKernelGetSystemTimeWide()-erl_hw.start);erl_hw.work_us+=us;erl_hw.work_frames++;if(us>erl_hw.work_worst)erl_hw.work_worst=us;erl_hw.dirty=1;if(game.race.phase==RACE_COUNTDOWN)erl_hw.countdown_frames++;else erl_hw.running_frames++;}return;
 }
 erl_hw.last=erl_hw.start=0;
 if(!erl_hw.dirty||!erl_hw.intervals)return;
 erl_hw.dirty=0;FILE *f=fopen("erl-hardware-timing.txt","a");
 if(f){fprintf(f,"ERL 2.5.472 / player timing / cached_hud=%d\n",neon_hud_cached);
  fprintf(f,"CLOCK cpu_mhz=%d..%d bus_mhz=%d free_bytes=%u\n",erl_hw.cpu_min,erl_hw.cpu_max,scePowerGetBusClockFrequencyInt(),sceKernelTotalFreeMemSize());
  fprintf(f,"FRAMES n=%u total_us=%llu worst_us=%u over100ms=%u\n",erl_hw.intervals,(unsigned long long)erl_hw.interval_us,erl_hw.worst_us,erl_hw.slow100);
  fprintf(f,"WORK n=%u total_us=%llu worst_us=%u includes_GE_sync excludes_vblank\n",erl_hw.work_frames,(unsigned long long)erl_hw.work_us,erl_hw.work_worst);
  fprintf(f,"DISPLAY vblank_under2=%u vblank2=%u vblank3=%u vblank4=%u vblank5plus=%u\n",erl_hw.spans[0],erl_hw.spans[1],erl_hw.spans[2],erl_hw.spans[3],erl_hw.spans[4]);
  fprintf(f,"STAGES sim_us=%llu compose_us=%llu countdown_frames=%u running_frames=%u\n",(unsigned long long)erl_hw.sim_us,(unsigned long long)erl_hw.compose_us,erl_hw.countdown_frames,erl_hw.running_frames);
  fprintf(f,"RENDER n=%u sky=%llu world=%llu road=%llu phase_projectiles=%llu player=%llu effects=%llu hud=%llu\n",erl_render_frames,(unsigned long long)erl_render_us[0],(unsigned long long)erl_render_us[1],(unsigned long long)erl_render_us[2],(unsigned long long)erl_render_us[3],(unsigned long long)erl_render_us[4],(unsigned long long)erl_render_us[5],(unsigned long long)erl_render_us[6]);
  fprintf(f,"WORLD n=%u city=%llu infrastructure=%llu pickups=%llu racers=%llu\n",neon_world_frames,(unsigned long long)neon_world_us[0],(unsigned long long)neon_world_us[1],(unsigned long long)neon_world_us[2],(unsigned long long)neon_world_us[3]);
  fprintf(f,"ROAD n=%u geometry=%llu raster=%llu marks=%llu\n",neon_road_frames,(unsigned long long)neon_road_us[0],(unsigned long long)neon_road_us[1],(unsigned long long)neon_road_us[2]);
  fprintf(f,"RACE serial=%u gate=%d ms=%u racers=%d paused=%d phase=%d\n\n",erl_hw.serial,game.race.gate,(unsigned)(game.race.elapsed*1000.f),game.race.rivals+1,game.erl.paused||paused,game.race.phase);fclose(f);erl_hw.dirty=0;
 }
}
