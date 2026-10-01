/* Compare native frames with identical world/time/camera state. */
static int surface_perf_review(void){
 FILE *flag=fopen("surface-perf-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("surface-perf-review.txt","w");if(!f)return 1;
 unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned)),*reference=malloc(STRIDE*H*sizeof(unsigned));
 if(!pixels||!reference){free(pixels);free(reference);fclose(f);return 1;}
 fb=pixels+16;for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 int failures=0,count=0;double old_total=0,new_total=0;unsigned long old_worst=0,new_worst=0;
 #define FCHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
 static const int systems[]={7,0,24,128};
 for(int si=0;si<4;si++)for(int body=1;body<=4;body++){
  pilot_review_reset(body);game.system=systems[si];system_bodies(&game);game.approach=body;enter_planet(&game);
  Vec3 pad=surface_site(&game,1);game.ship_pos=pad;game.ship_pos.y-=4;
  for(int mode=0;mode<9;mode++){
   planet_seat.active=0;planet_entry_body=0;planet_orbit_veil=0;
   game.time=20;game.world_clock=0;game.voice_time=game.message_time=0;game.rover_driving=0;game.roll=0;
   game.surface=mode==3?1:mode<4?0:2;game.planet_sequence=0;
   game.pos=add(pad,(Vec3){-120,22,400});
   game.yaw=mode==5?1.5707963f:mode==6?3.14159265f:0;
   game.pitch=mode==7?-.45f:mode==8?.40f:0;
   game.pos.y=terrain_height(&game,game.pos.x,game.pos.z)+22;
   if(mode<4){planet_sequence_begin(mode==3?2:1);game.planet_sequence_time=mode==0?1:mode==1?4:mode==2?8:1.2f;}
   surface_fast=0;drawcount=0;space();
   uint64_t t=sceKernelGetSystemTimeWide();
   for(int i=0;i<3;i++){drawcount=0;rect(0,0,W,H,BG);space();}
   unsigned long old=(sceKernelGetSystemTimeWide()-t)/3;memcpy(reference,fb,STRIDE*H*sizeof(unsigned));
   surface_fast=1;t=sceKernelGetSystemTimeWide();
   for(int i=0;i<3;i++){drawcount=0;rect(0,0,W,H,BG);space();}
   unsigned long fast=(sceKernelGetSystemTimeWide()-t)/3;unsigned different=0;
   for(int y=0;y<H;y++)for(int x=0;x<W;x++)different+=fb[y*STRIDE+x]!=reference[y*STRIDE+x];
   fprintf(f,"FRAME sys %d body %d mode %d old %.3fms fast %.3fms changed %u pixels\n",game.system,body,mode,old*.001f,fast*.001f,different);
   FCHECK(different==0,"fast path preserves native frame pixels");
   if(si==0){old_total+=old;new_total+=fast;count++;if(old>old_worst)old_worst=old;if(fast>new_worst)new_worst=fast;}
   if(si==0&&body==1&&(mode==1||mode==4||mode==7)){char path[64];snprintf(path,sizeof(path),"perf-mode-%d.bmp",mode);dump_native_bmp(path);}
  }
 }
 fprintf(f,"SUMMARY Lave render old %.3fms fast %.3fms worst old %.3f fast %.3f\n",old_total/count*.001,new_total/count*.001,old_worst*.001,new_worst*.001);
 int guard=1;for(int i=0;i<16;i++)guard&=pixels[i]==0xa55ac33cu&&pixels[STRIDE*H+16+i]==0xa55ac33cu;
 FCHECK(guard,"all framebuffer guards preserved");
 surface_fast=1;fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(pixels);free(reference);
 #undef FCHECK
 return 1;
}
