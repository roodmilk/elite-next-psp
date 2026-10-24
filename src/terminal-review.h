static void terminal_ship(int x,int y,int w,int h,int ship,float t,int wire){
 int ox=proj_ox,oy=proj_oy,cx0=clipx0,cy0=clipy0,cx1=clipx1,cy1=clipy1;
 preview_clip(x+w/2,y+h/2,x,y,x+w,y+h);
 int id=mesh_id(player_ships[ship].name);const Mesh *m=&meshes[id];Vec3 centre;float scale;menu_hull_layout(id,h,&centre,&scale);
 float yaw=t*.7f+.5f,tilt=-.38f;drawcount=0;
 for(int i=0;i<m->triangles;i++){const MeshTri *q=&m->t[i];Vec3 v[3];int ids[]={q->a,q->b,q->c};for(int k=0;k<3;k++)v[k]=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(m->v[ids[k]],centre),yaw,tilt,0),scale));Vec3 n=menu_hull_rotate(q->normal,yaw,tilt,0);if(dot(n,mul(add(add(v[0],v[1]),v[2]),-1.f/3))<=0)continue;
 if(wire){for(int k=0;k<3;k++){Point a=project(v[k]),b=project(v[(k+1)%3]);line(a.x,a.y,b.x,b.y,RGB(26,154,175));}}
 else{float l=.28f+.72f*fmaxf(0,dot(n,norm((Vec3){-.3f,.8f,-.6f})));queue_triangle(v[0],v[1],v[2],RGB(100*l,200*l,220*l));}}
 if(!wire)flush_meshes();preview_clip(ox,oy,cx0,cy0,cx1,cy1);
}
/* Native pixel symbols, bounded to a 12 by 10 cell. */
static void terminal_symbol(int x,int y,int location,unsigned ink){
 if(location){line(x+3,y,x+8,y,ink);line(x+1,y+2,x+1,y+5,ink);line(x+10,y+2,x+10,y+5,ink);line(x+2,y+6,x+5,y+9,ink);line(x+9,y+6,x+6,y+9,ink);rect(x+4,y+3,3,3,ink);}
 else {line(x+1,y+7,x+5,y+1,ink);line(x+5,y+1,x+10,y+7,ink);line(x+1,y+7,x+5,y+5,ink);line(x+10,y+7,x+5,y+5,ink);rect(x+4,y+7,3,2,CYAN);}
}
static void terminal_panel(int style,int state,float t){
 /* Clear only the old right overlay with the unchanged video pixels. */
 for(int y=145;y<245;y++)for(int x=255;x<468;x++)fb[y*STRIDE+x]=intro_movie_pixels[y*W+x];
 unsigned ink=style==2?GOLD:CYAN;intro_strip_panel(258,145,206,99,ink,225);
 text_px(266,151,ink,style==0?"FLIGHT RECORD":style==1?"PILOT TERMINAL":"SHIP / COMMAND LINK");
 for(int i=0;i<3;i++){rect(435+i*8,153,4,3,i==(int)(t*2)%3?GOLD:RGB(28,63,72));}
 if(style==0){
  terminal_ship(265,164,83,48,0,t,state==2);line(350,165,350,230,RGB(25,65,78));
  text_px(359,167,GOLD,state==0?"NEW PILOT":state==1?"BEN":"EMPTY");
  text_px(359,183,WHITE,state==1?"LAVE":"EARTH");text_px(359,199,CYAN,state==1?"1250 U":state==0?"2050":"--");
  text_px(267,218,WHITE,state==1?"ADDER / DOCKED":state==0?"FIRST FLIGHT AWAITS":"NO FLIGHT RECORD");
 }else if(style==1){
  draw_commander_portrait(266,167,32,portrait_seeded(7,EXPLORERS,0));
  text_px(306,166,GOLD,state==1?"CMDR BEN":state==0?"NEW IDENTITY":"UNASSIGNED");
  text_px(306,180,WHITE,state==1?"LAVE / 1250 U":"REGISTRATION PENDING");
  terminal_ship(267,198,73,32,0,t,state!=1);
  for(int k=0;k<3;k++){text_px(348,198+k*11,CYAN,k==0?"HULL":k==1?"FUEL":"LINK");rect(389,200+k*11,57,3,RGB(22,48,59));if(state!=2)rect(389,200+k*11,45+k*4,3,k==2?GOLD:CYAN);}
 }else{
  /* Compact dossier: identity, ship scan, numbered records, wallet and slots. */
  rect(266,163,190,13,RGB(36,29,20));rect(266,163,2,13,GOLD);
  text_px(273,166,GOLD,state==1?"CMDR BEN":state==0?"NEW COMMANDER":"UNASSIGNED RECORD");
  if(state==1)text_px(403,166,CYAN,"ONLINE");
  
  for(int k=0;k<4;k++){int yy=184+k*10;line(270,yy,343,yy,RGB(10,38,49));}
  terminal_ship(268,181,78,38,0,t,state==2);
  int sy=182+(int)(t*15)%36;line(270,sy,343,sy,RGB(20,109,124));
  line(349,182,349,224,RGB(31,75,83));
  terminal_symbol(358,181,0,GOLD);text_px(376,182,CYAN,state==2?"--":"ADDER");
  terminal_symbol(358,195,1,GOLD);text_px(376,196,WHITE,state==1?"LAVE":state==0?"EARTH":"NO DATA");
  rect(356,209,99,14,RGB(20,38,42));rect(356,209,2,14,CYAN);
  text_px(364,213,GOLD,state==1?"1250.0 U":"-- U");
  text_px(271,222,RGB(85,151,164),state==1?"DOCKED":state==0?"STANDBY":"NO LINK");
  if(state==1){line(327,222,343,222,GOLD);pixel(343,221,GOLD);}

 }
 if(style!=2)for(int side=0;side<2;side++){int x=side?341:267;line(x,166,x+6,166,ink);line(x,166,x,172,ink);}
 if(style==2){
  line(266,232,456,232,RGB(48,65,66));
  if(state==0)text_px(271,235,GOLD,"X BEGIN YOUR STORY");
  else {text_px(269,235,GOLD,"L <");for(int i=0;i<3;i++){int xx=309+i*25;rect(xx,234,20,8,i==0?RGB(60,45,20):RGB(12,31,40));text_px(xx+6,235,i==0?GOLD:RGB(62,105,116),"%d",i+1);}text_px(390,235,GOLD,"> R");text_px(428,235,CYAN,"LOAD");}
 }else text_px(267,234,ink,state==0?"X CREATE YOUR COMMANDER":"L < SLOT 1 / 3 > R");
}
static int terminal_review(void){FILE *flag=fopen("terminal-review.flag","r");if(!flag)return 0;fclose(flag);FILE *out=fopen("terminal-review.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);
 unsigned *old=fb;fb=calloc(STRIDE*H,sizeof(unsigned));if(!fb){fclose(out);return 1;}page=INTRO;intro_profile_ready=1;intro_menu_age=1;intro_extras=0;intro_movie_attempted=0;int ok=intro_movie_open();
 if(ok)for(int s=2;s<3;s++)for(int state=0;state<3;state++){for(int f=0;f<24;f++){intro_time=40+f/8.f;intro_choice=state?2:1;intro_load_slot=0;intro_screen();terminal_panel(s,state,f/8.f);char p[70];snprintf(p,sizeof(p),"terminal-%d-%d-%02d.bmp",s,state,f);dump_native_bmp(p);}fprintf(out,"PASS design %d state %d native animation captured\n",s,state);fflush(out);}
 intro_movie_close();free(fb);fb=old;fprintf(out,"RESULT %d failures\n",!ok);fclose(out);return 1;}
