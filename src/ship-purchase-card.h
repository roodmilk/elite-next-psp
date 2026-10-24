/* A native-resolution signed delivery card. Stroke paths are authored for
 * their displayed size; animation consumes ink length, never game RNG. */
static float ship_card_started=-100.f;
static int ship_card_variant=0,ship_card_last=-1,ship_card_sequence=0,ship_card_hull=0,ship_card_finished=0;
static const char *ship_card_lines[6][2]={
 {"Enjoy your","new ride!"},{"Make it","yours."},{"Here's to","new horizons!"},
 {"Go find","your stars."},{"Fly something","special."},{"Your next","chapter awaits."}
};
/* Each pair is a pen coordinate in a 6x9 cell; spaces lift the nib. */
static const char *ship_card_letters[26]={
 "5332130517375553 535767", /* a */
 "10071523435547271667",   /* b */
 "5332130517375667",       /* c */
 "50474323051737455767",   /* d */
 "0515355333130517375667", /* e */
 "17405130200103273757273729", /* f */
 "533213051737555357493929 5767", /* g */
 "1007152333454767",       /* h */
 "1314 162737",            /* i */
 "2324 2637271909",        /* j */
 "1007 5315354767",        /* k */
 "1004152737",            /* l */
 "0317071523333745555767", /* m */
 "031707152333455767",     /* n */
 "332313051737575533 5567", /* o */
 "031909 1723334557472717", /* p */
 "533213051737555359 5767", /* q */
 "0317071523334353",       /* r */
 "5333131525375757472717", /* s */
 "302617273747 042444",   /* t */
 "031527374553475767",     /* u */
 "0315374553536367",       /* v */
 "03152735374755636367",   /* w */
 "0315375767 531537",      /* x */
 "03152737455357493919 5767", /* y */
 "03335317375767"          /* z */
};
static float ship_card_advance(char c){return c==' '?4.f:c=='i'||c=='j'||c=='l'?4.f:c=='t'?5.f:c=='.'||c=='!'||c=='\''?3.f:7.5f;}
static const char *ship_card_path(char c){
 if(c>='A'&&c<='Z')c+=(char)('a'-'A');
 if(c>='a'&&c<='z')return ship_card_letters[c-'a'];
 if(c=='!')return "1014 1617";if(c=='.')return "1617";if(c=='\'')return "2012";return "";
}
static float ship_card_width(const char *s){float w=0;for(;*s;s++)w+=ship_card_advance(*s);return w;}
/* Measure or paint the same paths. Partial segments place the animated nib
 * precisely along a stroke instead of popping complete characters on. */
static float ship_card_script(const char *s,int y,float budget,int paint){
 const float scale=2.7f;float pen=240.f-ship_card_width(s)*scale*.5f,total=0;
 for(;*s;s++){
  const char *p=ship_card_path(*s);float px=0,py=0;int down=0;
  for(;*p;){
   if(*p==' '){down=0;p++;continue;}
   float gx=(float)(p[0]-'0'),gy=(float)(p[1]-'0');p+=2;
   float x=pen+(gx+(7.f-gy)*.16f)*scale,yy=y+gy*scale;
   if(down){float dx=x-px,dy=yy-py,len=sqrtf(dx*dx+dy*dy);
    if(paint&&budget>total&&len>0){float t=fminf(1.f,(budget-total)/len);int ex=(int)(px+dx*t),ey=(int)(py+dy*t);
     line((int)px,(int)py+1,ex,ey+1,RGB(158,159,145));line((int)px,(int)py,ex,ey,RGB(29,70,88));
     if(t<1.f){pixel(ex,ey,RGB(12,38,54));pixel(ex+1,ey,RGB(12,38,54));}
    }total+=len;
   }px=x;py=yy;down=1;
  }pen+=ship_card_advance(*s)*scale;
 }
 return total;
}
static void ship_purchase_reset(void){ship_card_started=-100.f;ship_card_finished=0;}
static int ship_purchase_open(void){return page==YARD&&ship_card_started>-50.f;}
static void ship_purchase_start(void){
 ship_card_variant=(game.ship+game.system+ship_card_sequence++)%6;
 if(ship_card_variant==ship_card_last)ship_card_variant=(ship_card_variant+1)%6;
 ship_card_last=ship_card_variant;ship_card_hull=game.ship;ship_card_started=preview_time;ship_card_finished=0;
 paused=0;
 game.message_time=0;game.message[0]=0;game.cue=SFX_SELECT;
}
static int ship_purchase_input(unsigned pressed){
 if(!ship_purchase_open())return 0;
 float age=preview_time-ship_card_started;if(age<.25f)return 1;
 if(pressed&PSP_CTRL_CIRCLE){ship_purchase_reset();game.cue=SFX_UI;}
 else if(pressed&PSP_CTRL_CROSS){if(age<3.4f&&!ship_card_finished)ship_card_finished=1;else ship_purchase_reset();game.cue=SFX_SELECT;}
 return 1;
}
static void ship_purchase_draw(void){
 if(!ship_purchase_open())return;
 float age=preview_time-ship_card_started,t=fminf(1.f,fmaxf(0,age/.4f));int oy=(int)(24.f*(1.f-t)*(1.f-t));
 unsigned paper=RGB(235,220,185),ink=RGB(30,62,76),gold=RGB(160,106,48);
 for(int y=24;y<248;y+=2)rect(0,y,W,1,RGB(3,7,14));
 rect(58,42+oy,376,204,RGB(3,8,15));rect(52,34+oy,376,204,gold);rect(54,36+oy,372,200,paper);
 rect(58,40+oy,364,1,RGB(251,239,209));rect(58,232+oy,364,1,RGB(197,174,133));
 text_px(240-(int)strlen("CONGRATULATIONS!")*4,49+oy,ink,"CONGRATULATIONS!");
 int n=(int)strlen(player_ships[ship_card_hull].name);text_px(240-n*4,70+oy,gold,"%s",player_ships[ship_card_hull].name);
 const char *a=ship_card_lines[ship_card_variant][0],*b=ship_card_lines[ship_card_variant][1];
 float first=ship_card_script(a,104+oy,0,0),second=ship_card_script(b,142+oy,0,0);
 float progress=ship_card_finished?1.f:fminf(1.f,fmaxf(0,(age-.55f)/2.65f)),budget=(first+second)*progress;
 ship_card_script(a,104+oy,budget,1);ship_card_script(b,142+oy,budget-first,1);
 if(progress>=1){float f=ship_card_finished?1.f:fminf(1.f,fmaxf(0,(age-3.2f)/.35f));int last=0;for(int i=0;i<=(int)(42*f);i++){int x=156+i*4,y=179+oy+(int)(sinf(i*.095f)*2);if(i)line(x-4,last,x,y,ink);last=y;}}
 /* Fireworks stay in the card's margins, clear of the handwriting and labels. */
 for(int burst=0;burst<4;burst++){
  float phase=fmodf(fmaxf(0,age-.45f)+burst*.53f,2.5f);int cx=burst&1?394:86,cy=burst<2?103:167;
  if(age<.45f)continue;
  if(phase<.35f){int sy=cy+oy+30-(int)(phase/.35f*30);line(cx,sy,cx,sy+5,RGB(191,122,49));continue;}
  float f=(phase-.35f)/1.2f;if(f>1)continue;unsigned c=mix_rgb(burst&1?RGB(193,107,56):RGB(42,134,142),paper,f);
  for(int i=0;i<16;i++){float angle=i*.392699f+burst*.31f,r=3+f*(19+i%3*3);int sx=cx+(int)(cosf(angle)*r),sy=cy+oy+(int)(sinf(angle)*r+f*f*7);
   if(sx<58||sx>421||sy<87+oy||sy>184+oy||(sx>112&&sx<368))continue;
   int tx=sx-(int)(cosf(angle)*3),ty=sy-(int)(sinf(angle)*3);
   if(tx>=58&&tx<=421&&ty>=87+oy&&ty<=184+oy&&(tx<=112||tx>=368))line(tx,ty,sx,sy,c);
   pixel(sx,sy,c);if(i%4==0){pixel(sx+1,sy,c);pixel(sx,sy+1,c);}
  }
 }
 circle(85,197+oy,11,gold);circle(85,197+oy,8,gold);line(79,199+oy,85,191+oy,gold);line(85,191+oy,91,199+oy,gold);line(79,199+oy,91,199+oy,gold);
 text_px(240-(int)strlen("THE SHIPYARD CREW")*4,197+oy,ink,"THE SHIPYARD CREW");
 button_icon(153,218+oy,'X',ink);text_px(171,220+oy,ink,progress>=1?"CONTINUE":"FINISH");
 button_icon(279,218+oy,'O',ink);text_px(297,220+oy,ink,"CLOSE");
 rect(0,248,W,24,RGB(3,7,14)); /* Hide the inactive shipyard footer. */
}
