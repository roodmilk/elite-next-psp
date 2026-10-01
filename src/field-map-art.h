/* Native 480x272 Field Navigator. Terrain is sampled from the same functions
 * as walking, once per planet/visit; no external art or per-frame allocation.
 * 192x192 indexed samples cost 36 KiB. Fog remains the existing visit mask. */
static void chart_text(int x,int y,unsigned ink,const char *fmt,...);
#define FM_N 192
#define FM_SCALE 6.f
static unsigned char fm_land[FM_N*FM_N];
static unsigned fm_palette[32];
static unsigned fm_fog[2][32];
static int fm_system=-1,fm_body=-1,fm_builds=0,eva_map_codex=0;
static unsigned fm_seed;
static Vec3 fm_pad;
static unsigned fm_noise(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static void fm_cache(void){
 const Body *b=&game.bodies[game.planet];
 if(fm_system==game.system&&fm_body==game.planet&&fm_seed==b->seed)return;
 fm_system=game.system;fm_body=game.planet;fm_seed=b->seed;fm_builds++;
 int biome=planet_biome(b);Vec3 pad=surface_site(&game,1);fm_pad=pad;
 unsigned low=RGB(76,94,51),high=RGB(138,117,65),water=RGB(20,90,133);
 if(biome==BIOME_DESERT){low=RGB(121,74,49);high=RGB(195,143,79);}
 if(biome==BIOME_ICE){low=RGB(117,159,173);high=RGB(211,223,210);}
 if(biome==BIOME_VOLCANIC){low=RGB(67,48,49);high=RGB(154,79,47);water=RGB(180,64,29);}
 if(biome==BIOME_CLOUD){low=RGB(96,106,126);high=RGB(169,180,181);water=RGB(65,54,83);}
 for(int i=0;i<16;i++)fm_palette[i]=mix_rgb(low,high,i/20.f);
 for(int i=16;i<24;i++)fm_palette[i]=mix_rgb(water,RGB(66,153,180),(i-16)/20.f);
 fm_palette[24]=biome==BIOME_ICE?RGB(192,213,215):RGB(154,152,101);
 fm_palette[25]=mix_rgb(low,RGB(27,59,53),.4f);
 fm_palette[26]=mix_rgb(high,low,.2f);
 for(int i=0;i<32;i++){fm_fog[0][i]=mix_rgb(fm_palette[i],RGB(9,20,32),.74f);fm_fog[1][i]=mix_rgb(fm_palette[i],RGB(9,20,32),.87f);}
 float base=terrain_height(&game,pad.x,pad.z),cell=EVA_FIELD_RADIUS*2.f/FM_N;
 for(int z=0;z<FM_N;z++)for(int x=0;x<FM_N;x++){
  float wx=pad.x-EVA_FIELD_RADIUS+(x+.5f)*cell,wz=pad.z-EVA_FIELD_RADIUS+(z+.5f)*cell;
  unsigned noise=fm_noise((unsigned)(x/2+z/2*7919)^b->seed);
  int c;
  if(terrain_is_water(&game,wx,wz))c=16+(noise%8);
  else {
   float h=terrain_height(&game,wx,wz)-base,step=fmaxf(28.f,terrain_relief_scale(&game)/9.f);int band=(int)floorf((h+step*2.f)/step);
   if(band<0)band=0;
   if(band>14)band=14;
   c=band+(noise%3==0);
   if(biome==BIOME_FOREST&&(noise%11)<2)c=25;
   if(biome!=BIOME_CLOUD&&h>step&&fmodf(fabsf(h),step)<step*.12f)c=26;
  }
  fm_land[z*FM_N+x]=(unsigned char)c;
 }
 /* One-cell shoreline follows the real coasts and river valleys. */
 if(biome!=BIOME_CLOUD)for(int z=1;z<FM_N-1;z++)for(int x=1;x<FM_N-1;x++){
  int i=z*FM_N+x;if(fm_land[i]>=16&&fm_land[i]<24)continue;
  int a=fm_land[i-1],b0=fm_land[i+1],c=fm_land[i-FM_N],d=fm_land[i+FM_N];
  if((a>=16&&a<24)||(b0>=16&&b0<24)||(c>=16&&c<24)||(d>=16&&d<24))fm_land[i]=24;
 }
}
static unsigned fm_sample(float x,float z){
 float dx=x-fm_pad.x,dz=z-fm_pad.z;
 if(dx*dx+dz*dz>EVA_FIELD_RADIUS*EVA_FIELD_RADIUS)return RGB(8,17,27);
 int ix=(int)((dx+EVA_FIELD_RADIUS)*FM_N/(EVA_FIELD_RADIUS*2.f));
 int iz=(int)((dz+EVA_FIELD_RADIUS)*FM_N/(EVA_FIELD_RADIUS*2.f));
 if(ix<0||iz<0||ix>=FM_N||iz>=FM_N)return RGB(8,17,27);
 int index=fm_land[iz*FM_N+ix];
 /* Dim unwalked land retains orbital relief, but never discloses site IDs. */
 int cx=(int)((dx+EVA_FIELD_RADIUS)*EVA_MAP_GRID/(EVA_FIELD_RADIUS*2.f));
 int cz=(int)((dz+EVA_FIELD_RADIUS)*EVA_MAP_GRID/(EVA_FIELD_RADIUS*2.f));
 return eva_map_known_cell(cx,cz)?fm_palette[index]:fm_fog[!!high_contrast][index];
}
static void fm_xy(float x,float z,int *sx,int *sy){*sx=240+(int)lroundf((x-game.pos.x-eva_map_pan_x)/FM_SCALE);*sy=137-(int)lroundf((z-game.pos.z-eva_map_pan_z)/FM_SCALE);}
static int fm_visible(int x,int y){return x>=17&&x<462&&y>=40&&y<220&&!(x>=389&&y<112);}
static void fm_rect(int x,int y,int w,int h,unsigned c){
 int r=x+w,b=y+h;if(x<9)x=9;if(y<29)y=29;if(r>471)r=471;if(b>242)b=242;
 if(r>x&&b>y)rect(x,y,r-x,b-y,c);
}
static void fm_box(int x,int y,int w,int h,unsigned c){fm_rect(x,y,w,1,c);fm_rect(x,y+h-1,w,1,c);fm_rect(x,y,1,h,c);fm_rect(x+w-1,y,1,h,c);}
static void fm_building(float x,float z,float hw,float hd,unsigned c){
 int a,b,d,e;fm_xy(x-hw,z+hd,&a,&b);fm_xy(x+hw,z-hd,&d,&e);
 fm_rect(a,b,d-a+1,e-b+1,c);fm_box(a,b,d-a+1,e-b+1,mix_rgb(c,WHITE,.25f));
 if(d-a>5&&e-b>5)fm_box(a+2,b+2,d-a-3,e-b-3,mix_rgb(c,RGB(30,40,44),.3f));
}
static void fm_label(int x,int y,const char *s,unsigned ink){
 char label[29];snprintf(label,sizeof(label),"%.28s",s);int w=(int)strlen(label)*7+5;
 if(x+w>468)x=468-w;
 if(x<12)x=12;
 if(y>211)y=211;
 if(y<40)y=40;
 if(x<249&&x+w>231&&y<145&&y+11>129)x=251;
 if(x+w>468)x=231-w;
 if(x+w>390&&y<113)y=114;
 rect(x,y,w,11,RGB(6,17,27));chart_text(x+3,y+2,ink,"%s",label);
}
static void fm_arrow(int x,int y,float yaw,unsigned ink){
 float s=sinf(yaw),c=cosf(yaw);
 for(int v=-5;v<=5;v++)for(int u=-4;u<=4;u++)if(abs(u)<=((5-v)/2)&&!(v<-1&&abs(u)<(-v)/2))
  pixel(x+(int)lroundf(u*c+v*s),y+(int)lroundf(u*s-v*c),ink);
}
static void fm_marker(int x,int y,int kind,unsigned ink){
 if(kind==0){fm_arrow(x,y,0,ink);rect(x-4,y+2,9,2,ink);}
 else if(kind==1){rect(x-4,y-2,9,5,ink);rect(x-2,y-5,5,4,ink);rect(x-1,y-4,3,2,RGB(6,17,27));rect(x-6,y,2,5,ink);rect(x+5,y,2,5,ink);}
 else {fm_box(x-5,y-5,11,11,ink);rect(x-1,y-1,3,3,ink);}
}
static void eva_map_draw(void){
 fm_cache();rect(0,0,W,H,RGB(6,16,27));
 header("");chart_text(128,8,UI_GOLD,"%.22s / FIELD MAP",game.bodies[game.planet].name);
 rect(0,21,W,1,UI_CYAN);rect(7,27,466,217,UI_GOLD);
 /* Terrain is pixel art at a 2x2 shading rate; labels and icons remain native. */
 for(int y=29;y<242;y+=2)for(int x=9;x<471;x+=2){unsigned c=fm_sample(game.pos.x+eva_map_pan_x+(x-240)*FM_SCALE,game.pos.z+eva_map_pan_z+(137-y)*FM_SCALE);
  fb[y*STRIDE+x]=fb[y*STRIDE+x+1]=c;
  if(y+1<242)fb[(y+1)*STRIDE+x]=fb[(y+1)*STRIDE+x+1]=c;
 }
 Vec3 pad=surface_site(&game,1);
 fm_building(pad.x,pad.z,field_port_half(&game),field_port_half(&game),RGB(66,76,76));
 for(int i=0;i<FIELD_PORT_BUILDINGS;i++){FieldBuilding structure=field_port_building(&game,i);const FieldBuilding *b=&structure;fm_building(pad.x+b->x,pad.z+b->z,b->w,b->d,RGB(126,141,140));}
 for(int i=0;i<3;i++){const FieldBuilding *b=&field_garage_walls[i];fm_building(pad.x+b->x,pad.z+b->z,b->w,b->d,RGB(155,167,165));}
 fm_building(pad.x,pad.z,FIELD_PLAYER_PAD,FIELD_PLAYER_PAD,RGB(90,99,98));
 for(int i=0;i<2;i++)fm_building(pad.x+(i?FIELD_TRAFFIC_X:-FIELD_TRAFFIC_X),pad.z+FIELD_TRAFFIC_Z,FIELD_TRAFFIC_PAD,FIELD_TRAFFIC_PAD,RGB(87,102,106));
 /* Actual port pads get their own inset markings; roads are not invented. */
 for(int i=0;i<3;i++){
  float px=pad.x+(i==0?0:i==1?FIELD_TRAFFIC_X:-FIELD_TRAFFIC_X),pz=pad.z+(i?FIELD_TRAFFIC_Z:0);
  int a,b;fm_xy(px,pz,&a,&b);int r=(int)((i?FIELD_TRAFFIC_PAD:FIELD_PLAYER_PAD)/FM_SCALE)-5;
  fm_box(a-r,b-r,2*r+1,2*r+1,RGB(150,157,143));
  for(int j=-1;j<=1;j++){fm_rect(a-r+2,b+j*6,3,2,UI_GOLD);fm_rect(a+r-4,b+j*6,3,2,UI_GOLD);}
 }
 for(int i=0;i<FIELD_PORT_BUILDINGS;i++){FieldBuilding structure=field_port_building(&game,i);const FieldBuilding *b=&structure;int a,v,d,e;fm_xy(pad.x+b->x-b->w,pad.z+b->z+b->d,&a,&v);fm_xy(pad.x+b->x+b->w,pad.z+b->z-b->d,&d,&e);
  for(int j=a+4;j<d-3;j+=7)fm_rect(j,v+4,3,2,RGB(167,191,190));
  fm_rect(a+3,e-3,d-a-5,1,RGB(53,66,74));
 }
 int x,y;unsigned green=RGB(132,244,78);
 for(int id=1;id<10;id++)if(id!=6){Vec3 p=surface_poi(&game,id);if(!eva_map_known_world(p.x,p.z))continue;fm_xy(p.x,p.z,&x,&y);if(fm_visible(x,y)){
  fm_marker(x,y,2,id==surface_nav_poi?UI_GOLD:green);
  if(id==surface_nav_poi){fm_box(x-7,y-7,15,15,UI_GOLD);fm_label(x+10,y-5,surface_site_name(&game,game.system,game.planet,id),UI_GOLD);}
 }}
 fm_xy(game.ship_pos.x,game.ship_pos.z,&x,&y);if(fm_visible(x,y)){fm_marker(x,y,0,UI_GOLD);fm_label(x-15,y+9,"SHIP",UI_GOLD);}
 fm_xy(game.rover_pos.x,game.rover_pos.z,&x,&y);if(fm_visible(x,y)){fm_marker(x,y,1,UI_CYAN);fm_label(x-19,y+9,"ROVER",UI_CYAN);}
 fm_xy(game.pos.x,game.pos.z,&x,&y);if(fm_visible(x,y)){fm_arrow(x,y+1,game.yaw,RGB(5,15,22));fm_arrow(x,y,game.yaw,WHITE);}
 /* Overview is the same chart, not a decorative planet or a second dataset. */
 rect(391,31,78,78,UI_GOLD);rect(392,32,76,76,RGB(6,16,27));
 for(int v=-35;v<=35;v++)for(int u=-35;u<=35;u++)if(u*u+v*v<=35*35)
  pixel(430+u,70-v,fm_sample(pad.x+u*EVA_FIELD_RADIUS/35.f,pad.z+v*EVA_FIELD_RADIUS/35.f));
 circle(430,70,36,WHITE);
 for(int id=1;id<10;id++)if(id!=6){Vec3 p=surface_poi(&game,id);if(eva_map_known_world(p.x,p.z)){int a=430+(int)((p.x-pad.x)*35/EVA_FIELD_RADIUS),b=70-(int)((p.z-pad.z)*35/EVA_FIELD_RADIUS);fm_box(a-1,b-1,3,3,id==surface_nav_poi?UI_GOLD:green);}}
 int cx=430+(int)((game.pos.x+eva_map_pan_x-pad.x)*35/EVA_FIELD_RADIUS),cy=70-(int)((game.pos.z+eva_map_pan_z-pad.z)*35/EVA_FIELD_RADIUS);
 int vx=23,vy=10;
 for(int v=-vy;v<=vy;v++)for(int u=-vx;u<=vx;u++)if((abs(u)==vx||abs(v)==vy)&&(cx+u-430)*(cx+u-430)+(cy+v-70)*(cy+v-70)<35*35)pixel(cx+u,cy+v,WHITE);
 if((cx-430)*(cx-430)+(cy-70)*(cy-70)<35*35)rect(cx-1,cy-1,3,3,WHITE);
 rect(234,31,13,15,RGB(9,20,30));fm_arrow(240,35,0,WHITE);chart_text(238,40,WHITE,"N");
 rect(11,222,101,18,RGB(6,16,27));chart_text(15,224,WHITE,"0");chart_text(48,224,WHITE,"250");chart_text(82,224,WHITE,"500M");
 int end=15+(int)lroundf(500/FM_SCALE);line(15,237,end,237,WHITE);for(int i=0;i<=2;i++){int a=15+(int)lroundf(i*250/FM_SCALE);line(a,233,a,237,WHITE);}
 rect(280,224,188,16,RGB(6,16,27));fm_arrow(288,232,0,WHITE);chart_text(297,229,WHITE,"YOU");fm_marker(330,232,2,green);chart_text(339,229,WHITE,"SITE");fm_box(375,228,9,9,RGB(103,118,129));chart_text(389,229,WHITE,"UNEXPLORED");
 rect(0,249,W,1,UI_GOLD);button_icon(12,256,'A',WHITE);chart_text(27,258,UI_MUTED,"START / ");button_icon(85,256,'O',RGB(255,74,100));chart_text(101,258,UI_MUTED,"CLOSE");button_icon(159,256,'T',RGB(59,231,105));chart_text(176,258,UI_MUTED,"PLANET CODEX");chart_text(363,258,UI_CYAN,"NUB: PAN");
}
