/* Exercise the reported Circle transition with every purchasable hull/body. */
{
 unsigned *saved_fb=fb,*guarded=malloc((STRIDE*H+128)*sizeof(unsigned));
 INPUT_CHECK(guarded!=NULL,"landing render: guarded framebuffer allocated");
 if(guarded){
  int safe=1,walked=0;
  for(int i=0;i<64;i++)guarded[i]=guarded[64+STRIDE*H+i]=0x13579bdf;
  fb=guarded+64;
  for(int ship=0;ship<player_ship_count;ship++)for(int body=1;body<BODY_COUNT;body++){
   TEST_INIT();game.ship=ship;launch(&game);page=FLIGHT;game.approach=body;
   if(!enter_planet(&game)){safe=0;continue;}
   game.pos=surface_site(&game,1);game.speed=8;
   land_planet(&game);planet_sequence_begin(2);
   safe&=game.surface==1&&game.planet_sequence==2;
   for(int frame=0;frame<3;frame++){game.planet_sequence_time=frame*.6f;Vec3 before=game.pos;drawcount=0;space();safe&=length(sub(before,game.pos))<.001f&&game.surface==1;}
   game.planet_sequence_time=0;
   for(int j=0;j<45&&game.planet_sequence;j++)input(0,0,.05f,0,0);
   walked+=game.surface==2&&!game.planet_sequence;
   drawcount=0;space();
   safe&=game.planet==body&&game.surface==2&&!game.planet_sequence&&isfinite(game.pos.x)&&isfinite(game.pos.y)&&isfinite(game.pos.z);
   for(int i=0;i<64;i++)safe&=guarded[i]==0x13579bdf&&guarded[64+STRIDE*H+i]==0x13579bdf;
  }
  INPUT_CHECK(safe&&walked==player_ship_count*(BODY_COUNT-1),"landing render: all hulls and local planet types land/disembark without stale sequence, invalid position or framebuffer overrun");
  fb=saved_fb;free(guarded);
 }
 TEST_INIT();
}
