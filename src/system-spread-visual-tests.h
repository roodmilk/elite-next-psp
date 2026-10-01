{
 FILE *flag=fopen("eva-capture.flag","r");int capture=flag!=NULL;if(flag)fclose(flag);
 int hot=1,clipped=1,animated=0;unsigned samples[2]={0};
 for(int frame=0;frame<2;frame++){
  rect(0,0,W,H,RGB(2,4,12));
  for(int family=0;family<8;family++){
   int x=60+(family%4)*120,y=68+(family/4)*134;
   draw_sun_sprite(x,y,35,RGB(255,200,80),family,frame*.2f,0,0,W,H-1);
   unsigned c=pixels[y*STRIDE+x];hot&=(c&255)>=240&&((c>>8)&255)>=200&&((c>>16)&255)>=120;
   samples[frame]^=pixels[(y+8)*STRIDE+x+11];text((x-24)/8,(y+51)/8,UI_GOLD,"STAR %d",family+1);
  }
  if(capture)dump_native_bmp(frame?"sun-families-animated.bmp":"sun-families-hot.bmp");
 }
 animated=samples[0]!=samples[1];
 unsigned bg=RGB(2,4,12);rect(0,0,W,H,bg);draw_sun_sprite(101,101,45,WHITE,7,1,100,100,160,159);
 for(int y=0;y<H;y++)for(int x=0;x<W;x++)if((x<100||x>=160||y<100||y>=160)&&pixels[y*STRIDE+x]!=bg)clipped=0;
 INPUT_CHECK(hot&&animated,"sun art: all eight star families have luminous warm centres and animated granulation");
 INPUT_CHECK(clipped,"sun art: disc, corona and flares stay within preview clipping bounds");
 TEST_INIT();launch(&game);game.system=129;game.destination=7;game.jump=.001f;page=FLIGHT;input(0,0,.016f,0,0);
 float yaw=game.yaw;int front=0,back=0;for(int b=1;b<BODY_COUNT;b++){front+=camera(&game,game.bodies[b].pos).z>0;back+=camera(&game,game.bodies[b].pos).z<0;}
 INPUT_CHECK(game.system==7&&!autoaim&&front&&back,"warp input: arrival keeps contacts around the ship instead of auto-turning to the station");
 if(capture){
  game.voice_time=game.message_time=0;
  for(int view=0;view<4;view++){game.yaw=yaw+view*1.5707963f;drawcount=0;space();char file[64];snprintf(file,sizeof(file),"wide-arrival-direction-%d.bmp",view);dump_native_bmp(file);}
  Body *sun=&game.bodies[0];game.pos=add(sun->pos,(Vec3){0,0,-sun->radius*3.5f});game.yaw=game.pitch=game.roll=0;drawcount=0;space();dump_native_bmp("lave-hot-sun.bmp");
  NPC *n=&game.npc[8];if(n->alive){game.pos=freight_world(n,(Vec3){2400,850,-1400});Vec3 d=norm(sub(n->pos,game.pos));game.yaw=atan2f(d.x,d.z);game.pitch=asinf(d.y);drawcount=0;space();dump_native_bmp("wide-freight-route.bmp");}
 }
 TEST_INIT();
}
