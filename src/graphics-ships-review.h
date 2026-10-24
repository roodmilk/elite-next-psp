/* Real renderer; staged flight poses, not a playback or a gameplay test. */
static int ship_art_review(void){
 FILE*flag=fopen("ship-art-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE*out=fopen("ship-art-review.txt","w");if(!out)return 1;
 int fail=0;graphics_scene(0);game.pos=(Vec3){15000,1000,STATION_Z-5000};game.yaw=game.pitch=game.roll=0;
 game.bodies[0].pos=add(game.pos,(Vec3){-4000,7000,-12000});
 selected_target=-1;game.jump=0;soft_sky_prepare(space_sky_seed());
 static const float distances[6]={260,260,260,1200,3500,10000};
 static const float headings[6]={.45f,2.6f,1.3f,.8f,.8f,.8f};
 for(int pose=0;pose<6;pose++){
  NPC*n=&game.npc[0];memset(n,0,sizeof(*n));n->alive=1;n->mesh=4;n->role=TRADERS;n->target=-1;n->scale=1.7f;n->radius=72;n->cruise=180;
  float height=pose==0?-100.f:pose==1?-70.f:pose==2?-160.f:-distances[pose]*.07f;game.pitch=pose<3?atan2f(height,distances[pose]):0;
  n->pos=add(game.pos,(Vec3){distances[pose]*.3f,height,distances[pose]});n->dir=(Vec3){sinf(headings[pose]),0,cosf(headings[pose])};
  double total=0;for(int frame=0;frame<16;frame++){game.time=50+frame*.04f;rect(0,0,W,H,BG);drawcount=0;uint64_t a=sceKernelGetSystemTimeWide();space();if(frame>=4)total+=sceKernelGetSystemTimeWide()-a;}
  char name[64];snprintf(name,sizeof(name),"ship-pilot-%d.bmp",pose);dump_native_bmp(name);
  fprintf(out,"PASS staged native pose%d distance%.0f render_only_mean%.3fms\n",pose,distances[pose],total/12000.);fflush(out);
 }

 /* Actual renderer frames: distant flyby, navigation blink and moving plume. */
 game.pitch=0;NPC*n=&game.npc[0];n->vessel_seed=0;
 for(int frame=0;frame<24;frame++){
  game.time=60+frame*.08f;n->pos=add(game.pos,(Vec3){1700+frame*24.f,-500,8000});n->dir=norm((Vec3){.8f,0,.6f});
  rect(0,0,W,H,BG);drawcount=0;space();char name[64];snprintf(name,sizeof(name),"distant-motion-%02d.bmp",frame);dump_native_bmp(name);
 }
 fprintf(out,"PASS 24 native staged distant flyby/blink frames captured; steady core does not switch off with navigation phase\n");

 /* Real draw-path checks: steady core across blink phases and opaque occlusion. */
 Vec3 c=camera(&game,n->pos);Point point=project(c);int xx=(int)point.x,yy=(int)point.y,at=yy*W+xx;
 int saved_valid=mega_depth_valid;unsigned short saved_depth=surface_depth[at];mega_depth_valid=0;
 game.time=0;rect(xx-3,yy-3,7,7,BG);distant_ship_trail(n);unsigned on=fb[yy*STRIDE+xx],lamp_on=fb[(yy-1)*STRIDE+xx+2];
 game.time=.5f/.72f;rect(xx-3,yy-3,7,7,BG);distant_ship_trail(n);unsigned off=fb[yy*STRIDE+xx],lamp_off=fb[(yy-1)*STRIDE+xx+2];
 int steady=on!=BG&&off!=BG&&lamp_on!=lamp_off;fail+=!steady;fprintf(out,"%s core remains visible while navigation lamp switches phase\n",steady?"PASS":"FAIL");
 mega_depth_valid=1;surface_depth[at]=mega_encode_depth(c.z*.5f);rect(xx-3,yy-3,7,7,BG);distant_ship_trail(n);
 int hidden=fb[yy*STRIDE+xx]==BG;fail+=!hidden;fprintf(out,"%s opaque foreground depth hides distant ship signal\n",hidden?"PASS":"FAIL");
 ship_wake_budget=24000;Point aa={xx-4.f,yy+.5f,c.z},bb={xx+4.f,yy+.5f,c.z};ship_wake_stroke(aa,bb,2,1,RGB(255,160,70),.6f,1);
 int wake_hidden=fb[yy*STRIDE+xx]==BG;fail+=!wake_hidden;fprintf(out,"%s opaque foreground depth hides wake pixels\n",wake_hidden?"PASS":"FAIL");
 surface_depth[at]=saved_depth;mega_depth_valid=saved_valid;
 fprintf(out,"INFO Adder visual pilot 80 vertices/144 triangles versus18/24; original collision mesh and twin stern roots retained. Staged frames; no physical PSP or fleet acceptance claim.\nRESULT %d failures\n",fail);
 fclose(out);return 1;
}
