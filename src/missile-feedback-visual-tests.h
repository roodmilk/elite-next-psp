{
 TEST_INIT();launch(&game);page=FLIGHT;hud_mode=hud_hidden=0;
 game.pos=(Vec3){0,100000,-100000};game.yaw=game.pitch=game.roll=0;game.speed=0;game.voice_time=game.message_time=0;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 game.npc[0].alive=1;game.npc[0].pos=add(game.pos,(Vec3){1200,0,0});game.fire_bearing_time[0]=2;
 float x,y,dx,dy;int position=fire_indicator_position(0,&x,&y,&dx,&dy);
 INPUT_CHECK(position&&x>440&&dx>0,"incoming fire: shooter to right produces right-bearing arrow");
 game.yaw=1.5707963f;position=fire_indicator_position(0,&x,&y,&dx,&dy);
 INPUT_CHECK(position==1&&fabsf(x-240)<2,"incoming fire: turning toward shooter moves indicator into forward view");
 game.yaw=-1.5707963f;INPUT_CHECK(fire_indicator_position(0,&x,&y,&dx,&dy)==2,"incoming fire: rear shooter uses double chevron");
 game.fire_bearing_time[0]=0;INPUT_CHECK(!fire_indicator_position(0,&x,&y,&dx,&dy),"incoming fire: expired shots leave no misleading indicator");
 game.yaw=0;game.fire_bearing_time[0]=2;game.npc[1].alive=1;game.npc[1].pos=add(game.pos,(Vec3){-1200,400,0});game.fire_bearing_time[1]=2;
 space();dump_native_bmp("incoming-fire-bearings.bmp");
 game.npc[1].alive=0;game.npc[0].pos=add(game.pos,(Vec3){900,0,2500});game.npc[0].health=1000;game.npc[0].shield=0;game.npc[0].cooldown=1000;game.npc[0].role=PIRATES;
 fire_missile(&game,NPC_ID_MIN);
 for(int frame=0;frame<20;frame++){
  game_tick(&game,.016f,0,0,0,0);space();
  if(frame==2||frame==9||frame==19){char file[64];snprintf(file,sizeof(file),"missile-launch-%02d.bmp",frame);dump_native_bmp(file);}
 }
 /* Compare the optimized enlarged-planet samples with the original formula. */
 int matching=1;
 for(int variant=0;variant<4;variant++){
  int radius=variant<2?240:700,cx=variant&1?-100:240,cy=110;
  unsigned seed=17u+variant*919u;int type=variant&1?OCEAN:ROCKY;
  rect(0,0,W,H,RGB(1,2,3));draw_planet_sprite(cx,cy,radius,seed,type,0,24,W,190);
  const uint16_t *data=planet_sprites[planet_sprite_index(seed,type)];
  int tr=224+((seed>>8)&31),tg=224+((seed>>13)&31),tb=224+((seed>>18)&31);
  for(int yy=24;yy<190;yy++)for(int xx=0;xx<W;xx++){
   unsigned expected=RGB(1,2,3);
   if(xx>=cx-radius&&xx<cx+radius&&yy>=cy-radius&&yy<cy+radius){
    int sx=2+(xx-cx+radius)*60/(radius*2),sy=2+(yy-cy+radius)*60/(radius*2);if(seed&16)sx=63-sx;
    unsigned p=data[sy*64+sx];if(p&0x8000){int r=(p>>10)&31,g=(p>>5)&31,b=p&31;expected=RGB(((r<<3)|(r>>2))*tr/255,((g<<3)|(g>>2))*tg/255,((b<<3)|(b>>2))*tb/255);}
   }
   matching&=pixels[yy*STRIDE+xx]==expected;
  }
 }
 INPUT_CHECK(matching,"planet close-up: optimized sampling is pixel-identical at large radii and clipped edges");
 TEST_INIT();launch(&game);page=FLIGHT;game.pos=(Vec3){0,100000,-100000};game.yaw=game.pitch=game.roll=0;game.voice_time=game.message_time=0;
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 for(int i=0;i<4;i++){NPC *n=&game.npc[i];n->alive=1;n->role=i;n->freighter=0;n->radius=30;n->cruise=500;n->dir=norm((Vec3){1,.15f,.4f});n->pos=add(game.pos,(Vec3){(i-1.5f)*3500,(i%2)*1200,14000});}
 space();dump_native_bmp("distant-ship-trails.bmp");
 for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;
 game.pos=add(game.bodies[0].pos,(Vec3){0,0,-game.bodies[0].radius-3000});game.yaw=game.pitch=0;
 space();dump_native_bmp("close-sun-flare.bmp");
 TEST_INIT();page=HOME;
}
