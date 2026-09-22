/* Original PSP pixel art. Cheap integer drawing, no NES assets, no runtime decode. */
static unsigned art_tint(unsigned c,int r,int g,int b){
 int cr=(c&255)+r,cg=((c>>8)&255)+g,cb=((c>>16)&255)+b;
 if(cr<0)cr=0;
 if(cr>255)cr=255;
 if(cg<0)cg=0;
 if(cg>255)cg=255;
 if(cb<0)cb=0;
 if(cb>255)cb=255;
 return RGB(cr,cg,cb);
}
static unsigned art_mix(unsigned a,unsigned b,int t){
 int ar=a&255,ag=(a>>8)&255,ab=(a>>16)&255,br=b&255,bg=(b>>8)&255,bb=(b>>16)&255;
 if(t<0)t=0;
 if(t>256)t=256;
 return RGB((ar*(256-t)+br*t)>>8,(ag*(256-t)+bg*t)>>8,(ab*(256-t)+bb*t)>>8);
}
static unsigned art_hash(unsigned x){x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);}
static void fill_disc(int cx,int cy,int r,unsigned c){
 int r2=r*r;
 for(int y=-r;y<=r;y++){int yy=cy+y;if(yy<0||yy>=H)continue;int span=0;while(span*span+y*y<=r2)span++;rect(cx-span,yy,span*2+1,1,c);}
}
static const char *race_name(int system){
 static const char *names[]={"VAHRI","KELN","ORRITH","SAURAN","ITHARI","MORVAK","QUEL","DRUUN"};
 return names[art_hash((unsigned)system*9973u)%8];
}
static const char *race_body(int system){
 static const char *names[]={"bipedal traders","insectile cartels","amphibious clans","reptilian guilds","avian surveyors","crystalbound hosts","fungal collectives","masked raiders"};
 return names[art_hash((unsigned)system*9973u)%8];
}
static unsigned race_skin(int system){
 static const unsigned pal[]={RGB(214,168,120),RGB(92,186,140),RGB(120,164,214),RGB(186,92,84),RGB(228,208,164),RGB(164,120,214),RGB(90,210,186),RGB(236,180,92)};
 return pal[art_hash((unsigned)system*4243u)%8];
}
static void draw_planet_disc(int cx,int cy,int r,unsigned land,unsigned accent,int seed,int type){
 (void)accent;
 if(type==SUN){draw_sun_sprite(cx,cy,r,land,(unsigned)seed,game.time,0,0,W,H);return;}
 draw_planet_sprite(cx,cy,r,(unsigned)seed,type,0,0,W,H);
}
static int faction_portrait_index(unsigned seed,int role){return (role>=0&&role<FACTION_COUNT?role:EXPLORERS)+4*(art_hash(seed*9973u)&1);}
static void draw_next_art_fit(const uint16_t *data,int sw,int sh,int x,int y,int w,int h){
 /* Portraits are square source tiles. Fit them inside the requested PSP box;
  * never stretch one axis, even when a caller supplies a card-shaped box. */
 if(w<=0||h<=0)return;
 int side=w<h?w:h;int ox=x+(w-side)/2,oy=y+(h-side)/2;
 draw_next_art(data,sw,sh,ox,oy,side,side);
}
static void draw_kei(int x,int y,int size,int expression){
 int i=expression>=0&&expression<8?expression:0;
 if(size<=32)draw_next_art_fit(kei_faces_small[i],32,32,x,y,size,size);
 else draw_next_art_fit(kei_faces[i],64,64,x,y,size,size);
}
static void draw_portrait(int x,int y,int w,int h,int system,int role){
 if(w>=12&&h>=12){int race=faction_portrait_index((unsigned)system,role);rect(x,y,w,h,RGB(8,16,28));if(w<=32&&h<=32)draw_next_art_fit(faction_portraits_small[race],32,32,x,y,w,h);else draw_next_art_fit(faction_portraits[race],64,64,x,y,w,h);rect(x,y+h-1,w,1,faction_colors[role%FACTION_COUNT]);return;}
 unsigned skin=race_skin(system),cloth=art_tint(faction_colors[role%FACTION_COUNT],-50,-40,-20),bg=RGB(8,16,28),kind=art_hash((unsigned)system*7919u)%8;
 rect(x,y,w,h,bg);rect(x,y,w,1,CYAN);rect(x,y+h-1,w,1,RGB(30,60,80));
 int cx=x+w/2,hw=w/6+3,hh=h/5+4,facey=y+h/5+2,bodyy=facey+hh+5;
 rect(cx-hw,facey,hw*2,hh,skin);
 rect(cx-hw+2,facey+2,hw*2-4,hh-4,art_tint(skin,22,14,8));
 rect(cx-hw/3,facey+hh,hw*2/3,4,art_tint(skin,-24,-18,-12));
 rect(cx-hw-3,bodyy,hw*2+6,h/3,cloth);
 rect(cx-hw,bodyy,hw*2,3,art_tint(cloth,30,20,10));
 rect(cx-hw/2,facey+hh/3,2,2,RGB(20,24,28));rect(cx+hw/3,facey+hh/3,2,2,RGB(20,24,28));
 rect(cx-2,facey+hh-4,5,2,art_tint(skin,-50,-36,-24));
 if(kind==0){rect(cx-hw-2,facey+2,3,hh/2,skin);rect(cx+hw-1,facey+2,3,hh/2,skin);}
 else if(kind==1){rect(cx-hw/2,facey-5,2,6,skin);rect(cx+hw/3,facey-5,2,6,skin);pixel(cx-hw/2,facey-6,CYAN);pixel(cx+hw/3,facey-6,CYAN);}
 else if(kind==2){rect(cx-hw-2,facey+hh/3,3,hh/3,art_tint(skin,-10,20,30));rect(cx+hw-1,facey+hh/3,3,hh/3,art_tint(skin,-10,20,30));}
 else if(kind==3){rect(cx-2,facey+hh-3,7,4,art_tint(skin,-20,-10,0));}
 else if(kind==4){rect(cx-3,facey-5,7,5,art_tint(skin,20,10,0));rect(cx-1,facey-7,3,3,skin);}
 else if(kind==5){rect(cx-2,facey-3,5,4,CYAN);pixel(cx,facey-1,WHITE);}
 else if(kind==6){pixel(cx-hw/2,facey+2,RGB(80,180,90));pixel(cx+hw/3,facey+4,RGB(70,160,80));pixel(cx,facey+hh/2,RGB(90,200,110));rect(cx-hw-1,facey+hh/4,3,3,art_tint(skin,10,40,10));}
 else {rect(cx-hw,facey+hh/4,hw*2,3,RGB(24,28,36));rect(cx-hw/2,facey+hh/4,hw,2,CYAN);}
 if(role==LAW)rect(cx-hw,facey-2,hw*2,2,CYAN);
 if(role==PIRATES)rect(cx-hw/2,facey+hh/2,hw,2,RED);
 if(role==EXPLORERS)rect(cx-1,facey-4,3,4,GOLD);
 /* Tiny identity lights keep small PSP portraits alive without adding
  * detail that collapses at 32 pixels. Each faction gets its own pulse. */
 if(w>=24&&h>=24){int pulse=(int)(preview_time*(role==PIRATES?7:role==LAW?5:3));if((pulse&3)<2){unsigned glow=faction_colors[role%FACTION_COUNT];pixel(x+w-3,y+2,glow);pixel(x+w-2,y+2,glow);}}
}
static void draw_world_card(int x,int y,int w,int h,const Body *b,int seed){
 rect(x,y,w,h,RGB(6,10,20));
 for(int i=0;i<18;i++){unsigned n=art_hash(seed+i*91u);pixel(x+(n% (w-2))+1,y+((n>>8)%(h-2))+1,((n>>16)&3)?DIM:WHITE);}
 int r=h/2-4;if(r<8)r=8;if(r>w/2-4)r=w/2-4;
 draw_planet_disc(x+w/2,y+h/2,r,b->color,b->accent,seed,b->type);
}
static void draw_icon(int x,int y,int id,int on){
 unsigned c=on?GOLD:DIM;
 rect(x,y,10,8,RGB(8,18,28));
 if(id==0){rect(x+2,y+3,6,3,c);rect(x+4,y+1,2,2,c);}
 else if(id==1){rect(x+2,y+2,6,5,c);rect(x+4,y+3,2,3,BG);}
 else if(id==2){pixel(x+5,y+2,c);pixel(x+3,y+4,c);pixel(x+7,y+4,c);pixel(x+5,y+6,c);}
 else if(id==3){rect(x+1,y+3,8,3,c);rect(x+7,y+2,2,2,c);}
 else if(id==4){rect(x+2,y+1,6,6,c);rect(x+4,y+3,2,2,BG);}
 else if(id==5){rect(x+3,y+1,4,6,c);rect(x+2,y+5,6,2,c);}
 else if(id==6){rect(x+1,y+2,8,4,c);pixel(x+3,y+4,BG);}
 else if(id==7){rect(x+2,y+2,6,4,c);rect(x+4,y+1,2,6,c);}
 else if(id==8){rect(x+2,y+2,6,4,c);pixel(x+4,y+3,WHITE);}
 else if(id==9){rect(x+3,y+1,4,6,c);rect(x+2,y+3,6,2,c);}
 else if(id==10){rect(x+2,y+3,6,2,c);rect(x+4,y+1,2,6,c);}
 else if(id==11){fill_disc(x+5,y+4,3,c);}
 else if(id==12){rect(x+2,y+2,6,5,c);rect(x+3,y+3,4,1,BG);}
 else if(id==13){rect(x+2,y+1,6,6,c);rect(x+4,y+3,2,3,BG);}
 else if(id==14){rect(x+1,y+2,8,4,c);rect(x+3,y+3,4,2,BG);}
 else {rect(x+2,y+2,6,4,c);pixel(x+4,y+4,GOLD);}
}
static void draw_flora_icon(int x,int y,int seed){
 unsigned leaf=RGB(70+(seed&31),160,70);rect(x+4,y+2,2,8,RGB(90,70,40));rect(x+1,y+3,4,3,leaf);rect(x+5,y+5,4,3,art_tint(leaf,20,10,0));
}
static void draw_fauna_icon(int x,int y,int seed){
 unsigned c=RGB(120,210-(seed&40),90);rect(x+2,y+4,8,4,c);rect(x+7,y+2,3,3,c);pixel(x+8,y+3,RGB(20,20,20));rect(x+1,y+8,3,2,c);rect(x+6,y+8,3,2,c);
}
static void draw_mineral_icon(int x,int y,int seed){
 unsigned c=RGB(190,160-(seed&30),90);rect(x+3,y+2,5,8,c);rect(x+2,y+5,7,4,art_tint(c,-20,-10,10));
}
static void draw_anomaly_icon(int x,int y,int kind){
 unsigned c=kind?CYAN:GOLD;fill_disc(x+6,y+6,5,RGB(12,24,36));circle(x+6,y+6,4,c);pixel(x+6,y+6,c);
}
static void draw_station_badge(int x,int y){
 fill_disc(x+18,y+16,14,RGB(24,48,64));circle(x+18,y+16,12,CYAN);rect(x+16,y+4,5,24,GOLD);rect(x+6,y+14,24,5,GOLD);
}
static inline void draw_menu_chrome(void){
 rect(8,24,464,1,RGB(18,40,52));
 for(int i=0;i<8;i++)rect(8+i*58,24,12,1,i&1?CYAN:GOLD);
}
static void lore_line(int system,int line,char *out,int n){
 const char *r=race_name(system),*b=race_body(system);
 if(line==0)snprintf(out,n,"%s CONTACT: %s",game.systems[system].name,r);
 else if(line==1)snprintf(out,n,"%s dominate local culture.",b);
 else if(line==2)snprintf(out,n,game.systems[system].economy<4?"Heavy industry stains the inner worlds.":"Orbital farms and greenhouses ring the hub.");
 else snprintf(out,n,system_rock_belt(system)?"A rock belt rings the inner worlds.":system_ice_belt(system)?"A cold ice belt hangs off the gas giant.":system_whales(system)?"Migrating space-whales graze the outer orbit.":"Open space. Traffic is light.");
}
