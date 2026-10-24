#ifdef TRAVEL_POLISH_REVIEW
static int mega_yard_style_override=-1;
#endif
static int mega_yard_style(const Game*g){
#ifdef TRAVEL_POLISH_REVIEW
 if(mega_yard_style_override>=0)return mega_yard_style_override;
#endif
 /* Rotate primary dealers through all three styles. Each system's three
  * secondary dealer identities rotate too; visiting again stays consistent. */
 int ordinal=0;for(int system=0;system<g->system;system++)if(system==7||station_class_for_system(g,system)==STATION_MEGA)ordinal++;
 return (ordinal+g->station_variant)%3;
}
static const char *mega_yard_style_name(int style){return (const char*[]){"ROYAL OBSERVATORY","ORBITAL FOUNDRY","CELESTIAL PALACE"}[style%3];}
/* Three approved mega dealer interiors. Shared live hulls, planet sheets and
 * exact native layout; no purchase, stock, save or gameplay writes. */
static void mega_yard_glow(int x,int y,int r,unsigned c,int gain){
 for(int yy=-r;yy<=r;yy++)for(int xx=-r;xx<=r;xx++){int q=xx*xx+yy*yy;if(q>=r*r)continue;int f=(r*r-q)*gain/(r*r);sun_bloom_dot(x+xx,y+yy,dim_rgb(c,f,256),9,66,471,166);}
}
static void mega_yard_world(int x,int y,int r,unsigned seed,int type){
 planet_phase_init();planet_phase_cx=x;planet_phase_cy=y;planet_phase_inverse=1.f/r;planet_phase_lx=-.65f;planet_phase_ly=.13f;planet_phase_lz=.66f;planet_phase_active=planet_phase_ready;
 draw_planet_sprite_rolled(x,y,r,seed,type,0,9,66,471,166);planet_phase_active=0;
}
static void mega_yard_showroom(int style,int ship){
 static const unsigned sky[3]={0x291b0c,0x26150b,0x30251d};
 unsigned gold=style==1?RGB(104,207,227):style==2?RGB(245,224,169):RGB(238,181,95);
 unsigned metal=style==1?RGB(45,71,87):style==2?RGB(151,157,147):RGB(91,78,59);
 yc_box(8,65,464,102,RGB(6,17,27),gold);
 for(int y=66;y<166;y++){unsigned c=planet_rgb_mix(RGB(3,10,22),sky[style],(y-66)*2);line(9,y,470,y,c);}
 unsigned seed=47191;
 for(int i=0;i<210;i++){unsigned v=yc_hash(&seed);int x=12+v%456,y=69+(v>>12)%78;pixel(x,y,i%8?RGB(53,79,103):RGB(177,203,210));}
 if(style==0){
  mega_yard_world(72,107,29,body_art_seed(game.system,1),game.bodies[1].type);circle(72,107,30,RGB(46,139,174));circle(72,107,31,RGB(23,66,98));
  mega_yard_glow(431,90,36,RGB(255,188,99),40);
  for(int side=0;side<2;side++)for(int layer=0;layer<4;layer++){int x=side?464-layer*4:15+layer*4;rect(x,70,3,90,planet_rgb_mix(metal,gold,layer*60));}
  for(int k=0;k<4;k++){int yy=70+k*3;line(34,yy+15,61,yy,gold);line(61,yy,420,yy,gold);line(420,yy,447,yy+15,gold);}
  for(int i=0;i<7;i++){int x=379+i*9,h=10+(i*17%24);rect(x,137-h,7,h,RGB(30,44,54));line(x,137-h,x+6,137-h,RGB(153,151,121));for(int y=139-h;y<134;y+=4)rect(x+2,y,2,1,RGB(128,193,199));}
 }else if(style==1){
  mega_yard_world(404,98,23,body_art_seed(game.system,2),game.bodies[2].type);circle(404,98,24,RGB(96,114,181));
  for(int side=0;side<2;side++){int x=side?439:20;rect(x,68,19,91,RGB(22,39,56));rect(x+2,70,3,86,RGB(84,123,142));rect(x+14,71,2,83,gold);for(int y=73;y<156;y+=10){rect(x+7,y,5,6,RGB(7,19,31));line(x+7,y+7,x+11,y+7,RGB(198,157,76));}}
  line(39,78,95,68,RGB(98,145,168));line(95,68,383,68,gold);line(383,68,440,78,RGB(98,145,168));
  for(int k=0;k<10;k++){int x=67+k*35;rect(x,72,12,3,RGB(48,84,102));line(x,75,x+12,75,RGB(100,218,235));mega_yard_glow(x+6,75,8,gold,21);}
  for(int k=0;k<7;k++){int x=57+k*58;line(x,135,x+8,121,RGB(40,68,82));rect(x+7,118,17,4,RGB(66,102,118));rect(x+9,119,7,1,RGB(131,207,217));}
 }else{
  mega_yard_glow(69,98,45,RGB(255,158,91),64);mega_yard_world(78,102,28,body_art_seed(game.system,3),game.bodies[3].type);circle(78,102,29,RGB(216,170,135));
  for(int side=0;side<2;side++){int x=side?433:20;rect(x,72,24,87,RGB(109,123,125));rect(x+3,72,14,87,RGB(184,188,161));line(x+3,72,x+3,157,RGB(248,230,176));rect(x+21,74,2,82,RGB(249,190,101));for(int y=77;y<150;y+=12)rect(x+6,y,8,6,RGB(74,102,110));}
  for(int k=0;k<4;k++){int yy=69+k*3;line(44,yy+17,92,yy,RGB(216,212,171));line(92,yy,385,yy,RGB(216,212,171));line(385,yy,434,yy+17,RGB(216,212,171));}
  for(int k=0;k<5;k++){int x=346+k*15;rect(x,114,11,27,RGB(40,67,75));line(x,114,x+5,107,RGB(105,152,154));line(x+5,107,x+11,114,RGB(105,152,154));rect(x+2,119,7,13,RGB(69,128,132));line(x+3,120,x+3,130,RGB(140,210,193));}
 }
 for(int y=143;y<166;y++)line(10,y,469,y,planet_rgb_mix(RGB(9,24,32),metal,(y-143)*3));
 for(int x=24;x<469;x+=32)line(240,135,x,165,RGB(48,67,72));for(int y=149;y<166;y+=7)line(12,y,468,y,RGB(54,74,80));
 for(int yy=-8;yy<=8;yy++){float f=sqrtf(fmaxf(0,1-yy*yy/64.f));line(240-(int)(126*f),151+yy,240+(int)(126*f),151+yy,RGB(6,16,24));}
 for(int i=0;i<75;i++){float a=i*6.2831853f/74;pixel(240+126*cosf(a),151+8*sinf(a),gold);}
 yc_tug(339,94,gold);yc_tug(111,88,RGB(125,209,221));yc_crew(76,151);yc_crew(397,151);
 for(int side=0;side<2;side++){int x=side?418:59;rect(x,147,3,15,metal);rect(x-2,146,7,2,gold);mega_yard_glow(x+1,147,10,gold,25);}
 yc_ship(ship,113,76,262,78);
}
