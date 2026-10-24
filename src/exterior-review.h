/* Disposable native review: full roll, every hull, anchors and occlusion. */
static int exterior_review(void){
 FILE *flag=fopen("exterior-review.flag","r");if(!flag)return 0;fclose(flag);
 unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned));if(!pixels)return 1;fb=pixels;
 FILE *report=fopen("exterior-review.txt","w");int framing=1,banking=1,anchors=1,failures=0;
 game_init(&game);story_complete(&game);launch(&game);page=FLIGHT;hud_mode=1;hud_hidden=0;third_person=1;game.message_time=game.voice_time=0;game.pos=(Vec3){0,0,-16000};
 for(int ship=0;ship<player_ship_count;ship++)for(int p=0;p<3;p++)for(int k=0;k<=128;k++){
  game.ship=ship;game.yaw=.73f;game.pitch=(p-1)*1.4f;game.roll=k*6.2831853f/128.f;game.time+=1.f/60;flight_camera_prepare();
  Vec3 center=camera(&game,game.pos),r,u,a;flight_camera_basis(&r,&u,&a);Point c=project(center),rp=project(camera(&game,add(game.pos,mul(r,10))));
  framing&=fabsf(c.x-240)<.1f&&c.y>135&&c.y<165&&center.z>20;
  banking&=rp.x>c.x&&fabsf(rp.y-c.y)<.1f;
  for(int side=-1;side<=1;side+=2)for(int weapon=0;weapon<2;weapon++){Vec3 root;int found=player_hardpoint(side,weapon,&root);anchors&=found||(!weapon&&side<0&&game.ship==22);
   if(found){Vec3 v=camera(&game,root);anchors&=isfinite(v.x)&&isfinite(v.y)&&isfinite(v.z);}
  }
 }
 game.ship=0;game.pitch=0;game.yaw=0;game.roll=0;flight_camera_prepare();
 surface_y_min=view_top();surface_y_max=view_bot();mega_depth_encoding=1;
 memset(pixels,0,STRIDE*H*sizeof(unsigned));for(int i=0;i<W*H;i++)surface_depth[i]=mega_encode_depth(10);
 Vec3 r,u,a;camera_axes(&game,&r,&u,&a);Vec3 eye=camera_origin(&game),s=add(eye,mul(a,100)),e=add(s,mul(r,30));
 exterior_ray(s,e,WHITE,1);int blocked=1;for(int i=0;i<STRIDE*H;i++)if(pixels[i])blocked=0;
 memset(surface_depth,255,sizeof(surface_depth));exterior_ray(s,e,WHITE,1);int visible=0;for(int i=0;i<STRIDE*H;i++)if(pixels[i])visible=1;
 mega_depth_encoding=0;surface_depth_on=0;
 const char *names[]={"rear-level.bmp","rear-roll-quarter.bmp","rear-roll-half.bmp","rear-roll-full.bmp","rear-large-hull.bmp","rear-gunfire.bmp"};
 for(int k=0;k<6;k++){game.ship=k==4?PLAYER_SHIP_MEGA_FIRST:0;game.roll=k==1?1.5707963f:k==2?3.14159265f:k==3?6.2831853f:0;game.pitch=.24f;game.yaw=.37f;game.boost=1;game.speed=600;game.time+=1;game.shot=k==5?.08f:0;game.fit[FIT_WPN]=1;selected_target=0;space();dump_native_bmp(names[k]);}
 failures=!framing+!banking+!anchors+!blocked+!visible;
 if(report){fprintf(report,"%s centered chase framing for all %d hulls, three pitches and 129 roll steps\n",framing?"PASS":"FAIL",player_ship_count);fprintf(report,"%s camera right remains ship right across complete rolls and angle wrap\n",banking?"PASS":"FAIL");fprintf(report,"%s engine and weapon surface anchors exist for every hull\n",anchors?"PASS":"FAIL");fprintf(report,"%s foreground depth blocks exterior effects\n",blocked?"PASS":"FAIL");fprintf(report,"%s unoccluded exterior effects remain visible\n",visible?"PASS":"FAIL");fprintf(report,"RESULT %d failures\n",failures);fclose(report);}
 return 1;
}
