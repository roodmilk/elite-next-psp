/* Opt-in native renderer review. Disposable flag, no saves/audio/input. */
static unsigned soft_sky_frame_hash(void){unsigned h=2166136261u;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++)h=(h^fb[y*STRIDE+x])*16777619u;return h;}
static int soft_sky_review(void){
 FILE *flag=fopen("soft-sky-review.flag","r");if(!flag)return 0;int system=7;fscanf(flag,"%d",&system);fclose(flag);
 if(system==256){
  FILE *catalog=fopen("soft-sky-review.txt","w");if(!catalog)return 1;
  unsigned hashes[256];int duplicates=0,flat=0,minspan=1000,maxspan=0;uint64_t start,end;sceRtcGetCurrentTick(&start);
  for(int id=0;id<256;id++){
   soft_sky_prepare(space_sky_seed_for(id));unsigned hash=2166136261u;int lo=1000,hi=0;
   for(int face=0;face<6;face++)for(int y=0;y<SOFT_SKY_FACE;y++)for(int x=0;x<SOFT_SKY_FACE;x++){
    unsigned c=soft_sky_faces[face][y][x];hash=(hash^c)*16777619u;int light=(c&255)+((c>>8)&255)+((c>>16)&255);if(light<lo)lo=light;if(light>hi)hi=light;
   }
   for(int previous=0;previous<id;previous++)if(hashes[previous]==hash)duplicates++;hashes[id]=hash;
   int span=hi-lo;flat+=span<12;if(span<minspan)minspan=span;if(span>maxspan)maxspan=span;
  }
  sceRtcGetCurrentTick(&end);
  fprintf(catalog,"%s all 256 generated sky maps have distinct content hashes (%d duplicates)\n",duplicates?"FAIL":"PASS",duplicates);
  fprintf(catalog,"%s all 256 skies contain non-uniform clouds/dark gaps (%d flat; brightness-span %d..%d)\n",flat?"FAIL":"PASS",flat,minspan,maxspan);
  fprintf(catalog,"INFO complete generation sweep %.2f seconds; fixed cache %u bytes\n",(end-start)/(float)sceRtcGetTickResolution(),(unsigned)sizeof(soft_sky_faces));
  fprintf(catalog,"RESULT %d failures\n",(duplicates!=0)+(flat!=0));fclose(catalog);return 1;
 }
 if(system<0||system>255)system=7;
 FILE *report=fopen("soft-sky-review.txt","w");unsigned *saved=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned)),*reference=calloc(STRIDE*H,sizeof(unsigned));
 if(!report||!pixels||!reference){if(report)fclose(report);free(pixels);free(reference);return 1;}
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0xa55ac33cu;
 fb=pixels+16;game.system=system;system_bodies(&game);launch(&game);story_complete(&game);page=FLIGHT;hud_mode=hud_hidden=high_contrast=0;game.voice_time=game.message_time=0;game.dock_stage=0;preview_reset();
 game.yaw=.37f;game.pitch=-.11f;game.roll=.08f;game.time=10;int failures=0;
#define SKY_CHECK(ok,label) do{int pass=(ok);fprintf(report,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;}while(0)
 fprintf(report,"SYSTEM %d %s; renderer=soft-celestial-v1; cache=%u bytes\n",system,game.systems[system].name,(unsigned)sizeof(soft_sky_faces));
 uint64_t a,b;sceRtcGetCurrentTick(&a);sector_background();space_fx_nebula();sceRtcGetCurrentTick(&b);
 fprintf(report,"Cold sky generation and draw %.2f ms\n",(b-a)*1000.f/sceRtcGetTickResolution());
 unsigned base=soft_sky_frame_hash();memcpy(reference,fb,STRIDE*H*sizeof(unsigned));Vec3 pos=game.pos;
 game.pos=add(pos,(Vec3){120000,-48000,92000});sector_background();space_fx_nebula();SKY_CHECK(soft_sky_frame_hash()==base,"translation leaves distant clouds pixel-identical");game.pos=pos;
 game.time+=500;sector_background();space_fx_nebula();SKY_CHECK(soft_sky_frame_hash()==base,"clouds do not reseed or pulse into different patterns with time");game.time=10;
 game.yaw+=6.283185307f;sector_background();space_fx_nebula();
 int delta=0;for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++){unsigned p=fb[y*STRIDE+x],q=reference[y*STRIDE+x];delta+=abs((int)(p&255)-(int)(q&255))+abs((int)((p>>8)&255)-(int)((q>>8)&255))+abs((int)((p>>16)&255)-(int)((q>>16)&255));}
 SKY_CHECK(delta<W*(view_bot()-view_top()+1),"full heading rotation returns to the same continuous sky");game.yaw=.37f;
 game.pitch+=.017453293f;sector_background();space_fx_nebula();delta=0;int edges=0,maxjump=0;
 for(int y=view_top();y<=view_bot();y++)for(int x=0;x<W;x++){unsigned p=fb[y*STRIDE+x],q=reference[y*STRIDE+x];delta+=abs((int)(p&255)-(int)(q&255))+abs((int)((p>>8)&255)-(int)((q>>8)&255))+abs((int)((p>>16)&255)-(int)((q>>16)&255));if(x){unsigned l=fb[y*STRIDE+x-1];int d=abs((int)(p&255)-(int)(l&255))+abs((int)((p>>8)&255)-(int)((l>>8)&255))+abs((int)((p>>16)&255)-(int)((l>>16)&255));if(d>maxjump)maxjump=d;edges+=d>40;}}
 fprintf(report,"One-degree pitch mean RGB change %.3f; largest adjacent-pixel RGB step %d; hard edges %d\n",delta/(3.f*W*(view_bot()-view_top()+1)),maxjump,edges);
 SKY_CHECK(delta<W*(view_bot()-view_top()+1)*12,"small pitch change stays smooth without texture scrambling");SKY_CHECK(edges==0,"continuous shading has no isolated hard rectangular edges");game.pitch=-.11f;
 sceRtcGetCurrentTick(&a);for(int i=0;i<36;i++){game.pitch=-.3f+i*.017f;game.roll=i*.02f;space_fx_nebula();}sceRtcGetCurrentTick(&b);
 fprintf(report,"Warm moving sky draw %.2f ms\n",(b-a)*1000.f/sceRtcGetTickResolution()/36.f);
 for(int view=0;view<4;view++){
  game.yaw=.37f+view*1.45f;game.pitch=-.11f;game.roll=0;drawcount=0;rect(0,0,W,H,BG);space();char file[48];snprintf(file,sizeof(file),"soft-sky-%03d-view-%d.bmp",system,view);dump_native_bmp(file);
 }
 /* Close sun/planet passes exercise the other softened flight-light layers. */
 for(int body=0;body<2;body++){game.pos=add(game.bodies[body].pos,(Vec3){0,0,-game.bodies[body].radius*(body?3.f:5.f)});game.yaw=game.pitch=game.roll=0;drawcount=0;rect(0,0,W,H,BG);space();char file[48];snprintf(file,sizeof(file),"soft-sky-%03d-near-%d.bmp",system,body);dump_native_bmp(file);}game.pos=pos;
 if(system==7)for(int frame=0;frame<32;frame++){
  game.yaw=.37f;game.pitch=-.28f+frame*.018f;game.roll=.07f;game.time=10+frame/30.f;rect(0,0,W,H,BG);drawcount=0;space();char file[48];snprintf(file,sizeof(file),"soft-sky-motion-%02d.bmp",frame);dump_native_bmp(file);
 }
 high_contrast=1;memset(fb,0,STRIDE*H*sizeof(unsigned));space_fx_nebula();int untouched=1;for(int i=0;i<STRIDE*H;i++)if(fb[i])untouched=0;SKY_CHECK(untouched,"high-contrast setting omits decorative clouds");high_contrast=0;
 memset(fb,0,STRIDE*H*sizeof(unsigned));preview_clip(240,110,53,39,317,167);space_fx_nebula();int contained=1;for(int y=0;y<H;y++)for(int x=0;x<STRIDE;x++)if((x<53||x>=317||y<39||y>=167)&&fb[y*STRIDE+x])contained=0;SKY_CHECK(contained,"preview rendering respects all clip boundaries");preview_reset();
 int safe=1;for(int i=0;i<16;i++)if(pixels[i]!=0xa55ac33cu||pixels[STRIDE*H+16+i]!=0xa55ac33cu)safe=0;SKY_CHECK(safe,"framebuffer guard words intact");
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);fb=saved;free(pixels);free(reference);return 1;
#undef SKY_CHECK
}
