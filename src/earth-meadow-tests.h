/* Runs inside the isolated opening fixture. Deliberately tests the actual
 * raster/depth buffer, not just whether a ground drawing function was called. */
{
 int matching=1,covered=0,holes=0,frames=0,max_tiles=0;double elapsed=0;uint64_t start,end;
 for(int kind=0;kind<2;kind++){
  float x=-174,z=132,rx,rz,h;if(kind)earth_meadow_rock(8,&x,&z,&rx,&rz,&h);
  game.pos=meadow_ground_point(pad,x,z,22);opening_prepared=-1;opening_prepare();
  OPEN_CHECK(eva_position_allowed(&game,game.pos.x,game.pos.z)&&hypotf(game.pos.x-pad.x-75,game.pos.z-pad.z+80)<1,"old checkpoint in changed water or outcrop recovers beside the ship");
 }
 meadow_prepare_ground(pad);
 for(int z=MEADOW_OUTER;z<MEADOW_OUTER+32;z++)for(int x=MEADOW_OUTER;x<MEADOW_OUTER+32;x++){
  Vec3 a=meadow_ground_cache[z][x],b=meadow_ground_cache[z][x+1],c=meadow_ground_cache[z+1][x+1],d=meadow_ground_cache[z+1][x];
  for(int k=0;k<2;k++){float u=k?.21f:.73f,v=k?.71f:.28f;
   float h=u>=v?a.y+(b.y-a.y)*u+(c.y-b.y)*v:a.y+(c.y-d.y)*u+(d.y-a.y)*v;
   matching&=fabsf(h-terrain_mesh_height(&game,a.x+(b.x-a.x)*u,a.z+(d.z-a.z)*v))<.025f;
  }
 }
 OPEN_CHECK(matching,"all 1024 inner ground tiles match walking collision on both triangle halves");
 OPEN_CHECK(meadow_ground_cache[0][0].x<pad.x-59000&&meadow_ground_cache[MEADOW_GRID-1][MEADOW_GRID-1].z>pad.z+59000,"connected ground reaches sixty kilometres beyond the clearing");
 static const float places[][2]={{35,-110},{-86,118},{-265,132},{-174,218},{-265,-85},{125,245},{265,0},{0,-280},{185,185},{-80,-260}};
 static const float pitches[]={-.75f,-.3f,0,.75f};game.roll=0;
 for(int place=0;place<10;place++)for(int yaw=0;yaw<8;yaw++)for(int pitch=0;pitch<4;pitch++){
  game.pos=meadow_ground_point(pad,places[place][0],places[place][1],22);game.yaw=yaw*.78539816f;game.pitch=pitches[pitch];
  surface_cy=cosf(game.yaw);surface_sy=sinf(game.yaw);surface_cp=cosf(game.pitch);surface_sp=sinf(game.pitch);
  surface_sprite_tan=tanf(game.pitch);surface_y_min=28;surface_y_max=239;surface_depth_on=1;surface_light=1;drawcount=0;
  memset(surface_depth,255,sizeof(surface_depth));rect(0,0,W,H,RGB(240,0,220));
  sceRtcGetCurrentTick(&start);earth_meadow_ground(pad);sceRtcGetCurrentTick(&end);elapsed+=(double)(end-start)*1000/sceRtcGetTickResolution();frames++;
  if(meadow_visible_tiles>max_tiles)max_tiles=meadow_visible_tiles;
  /* Independent camera rays at the raster's 2x2 sample centres. If a ray
   * enters the shared ground, an untouched depth value is a real hole. */
  for(int y=61;y<239;y+=44)for(int x=21;x<W;x+=44){
   float ry=(proj_oy-y)/240.f,rx=(x-proj_ox)/240.f;
   float rz=surface_cp-surface_sp*ry,up=surface_sp+surface_cp*ry;
   Vec3 ray={surface_cy*rx+surface_sy*rz,up,-surface_sy*rx+surface_cy*rz};
   int hit=0;for(float t=8;t<=512;t*=2){Vec3 p=add(game.pos,mul(ray,t));if(p.y<terrain_mesh_height(&game,p.x,p.z)-.5f){hit=1;break;}}
   if(hit){covered++;if(surface_depth[(y-1)*W+x-1]==65535)holes++;}
  }
 }
 surface_depth_on=0;surface_sprite_z=0;
 fprintf(f,"INFO meadow coverage %d views / %d expected ground samples / %d holes / %d maximum visible tiles / %.3f ms ground average\n",frames,covered,holes,max_tiles,elapsed/frames);
 OPEN_CHECK(covered>4000&&holes==0,"no missing ground across 320 pond boundary and steep-pitch views");
 int seamless=1,invariant=1;
 for(int k=0;k<31;k++){
  game.pitch=-.75f+k*.05f;surface_cp=cosf(game.pitch);surface_sp=sinf(game.pitch);
  unsigned last=meadow_sky_ink((proj_oy-28)/240.f);
  for(int y=29;y<240;y++){unsigned c=meadow_sky_ink((proj_oy-y)/240.f);for(int n=0;n<3;n++)seamless&=abs((int)((c>>(n*8))&255)-(int)((last>>(n*8))&255))<=2;last=c;}
  unsigned c=meadow_sky_ink(tanf(.45f-game.pitch));surface_cp=1;surface_sp=0;unsigned same=meadow_sky_ink(tanf(.45f));
  for(int n=0;n<3;n++)invariant&=abs((int)((c>>(n*8))&255)-(int)((same>>(n*8))&255))<=1;
 }
 OPEN_CHECK(seamless&&invariant,"sky remains continuous through full pitch range and follows world elevation");
 game.voice_time=game.message_time=0;game.opening.flags|=OF_MEADOW_GLOW;
 static const float shots[][4]={{35,-110,.25f,0},{-65,118,-1.5707963f,-.1f},{125,245,0,-.2f},{235,0,1.5707963f,0},{35,-110,.25f,.75f},{-265,132,1.5707963f,-.75f},{0,-215,3.1415926f,0},{35,-110,3.1415926f,-.2f}};
 for(int k=0;k<8;k++){game.pos=meadow_ground_point(pad,shots[k][0],shots[k][1],22);game.yaw=shots[k][2];game.pitch=shots[k][3];char name[64];snprintf(name,sizeof(name),"meadow-282-view-%d.bmp",k);opening_test_capture(name);}
 /* The ordinary capture helper clears the entire screen, hiding stale UI
  * defects. Here render over real old dialogue and poisoned edge pixels. */
 game.opening.stage=OP_FIND;game.opening.flags|=OF_DIALOG_DONE;opening_notice=0;
 game.pos=meadow_ground_point(pad,-50,-100,22);game.pitch=0;game.yaw=0;
 int clean=1,owned=1,consistent=1;unsigned scene_hash=0;
 for(int mode=0;mode<3;mode++){
  hud_mode=mode;hud_hidden=mode==2;rect(0,0,W,H,RGB(241,3,239));
  opening_bottom("DAD","Take your time. I'll be here by the ship.","X CONTINUE");
  space();opening_draw();
  for(int y=0;y<H;y++)for(int x=0;x<W;x++)clean&=fb[y*STRIDE+x]!=RGB(241,3,239);
  for(int y=0;y<H;y++)if((y>=24&&y<28)||(y>=240&&y<258))for(int x=0;x<W;x++)owned&=fb[y*STRIDE+x]==RGB(21,28,39);
  unsigned hash=2166136261u;for(int y=28;y<238;y++)for(int x=0;x<W;x++)hash=(hash^fb[y*STRIDE+x])*16777619u;
  if(!mode)scene_hash=hash;else consistent&=scene_hash==hash;
  char file[64];snprintf(file,sizeof(file),"meadow-282-dialogue-cleared-%d.bmp",mode);dump_native_bmp(file);
 }
 hud_mode=hud_hidden=0;
 OPEN_CHECK(clean&&owned,"returning from Dad dialogue repaints every edge pixel in all HUD modes without a pre-clear");
 OPEN_CHECK(consistent,"meadow sky and ground retain the same colours across cockpit HUD modes without space bloom");
 /* Independent rays into the distant rendered lattice. The finite geometry
  * must cover fields below the overlook, not merely the first 512 metres. */
 int far_hits=0,far_holes=0,groves=0;
 for(int yaw=0;yaw<8;yaw++)for(int pitch=0;pitch<3;pitch++){
  game.pos=meadow_ground_point(pad,125,245,22);game.yaw=yaw*.78539816f;game.pitch=(pitch-1)*.35f;drawcount=0;planet_view();groves+=meadow_far_visible;
  for(int y=41;y<239;y+=18)for(int x=9;x<480;x+=22){
   float ry=(proj_oy-y)/240.f,rx=(x-proj_ox)/240.f,rz=surface_cp-surface_sp*ry;
   Vec3 ray={surface_cy*rx+surface_sy*rz,surface_sp+surface_cp*ry,-surface_sy*rx+surface_cy*rz};
   for(float t=600;t<10000;t*=1.3f){Vec3 p=add(game.pos,mul(ray,t)),land=meadow_landscape_point(pad,p.x-pad.x,p.z-pad.z);
    if(p.y<land.y-4){far_hits++;far_holes+=surface_depth[(y-1)*W+x-1]==65535;break;}
   }
  }
 }
 fprintf(f,"INFO meadow distant coverage %d ground rays / %d holes / %d grove instances\n",far_hits,far_holes,groves);
 OPEN_CHECK(far_hits>500&&far_holes==0&&groves>0,"distant fields cover the overlook in every bearing and giant tree groves enter view");
 int grounded=1,giants=0;for(int i=0;i<MEADOW_DISTANT_TREES;i++){
  Vec3 p=meadow_far_trees[i].p,q=meadow_landscape_point(pad,p.x-pad.x,p.z-pad.z);
  grounded&=fabsf(p.y-q.y)<.01f&&hypotf(p.x-pad.x,p.z-pad.z)>400;
  giants+=meadow_far_trees[i].height>=190;
 }
 OPEN_CHECK(grounded&&giants>=12,"distant trees sit on displayed terrain beyond walking boundaries with twelve giant landmarks");
 elapsed=0;game.pos=meadow_ground_point(pad,35,-110,22);game.pitch=0;
 for(int k=0;k<12;k++){game.yaw=k*.5235988f;sceRtcGetCurrentTick(&start);rect(0,0,W,H,BG);drawcount=0;planet_view();sceRtcGetCurrentTick(&end);elapsed+=(double)(end-start)*1000/sceRtcGetTickResolution();}
 fprintf(f,"INFO meadow complete scene %.3f ms average across twelve bearings (emulator, not PSP FPS)\n",elapsed/12);
 OPEN_CHECK(isfinite(elapsed)&&elapsed>0&&max_tiles<1500,"meadow rendering has a bounded connected mesh and finite measured frame cost");
}
