/* Native ERL motion: bounded integer geometry, no allocation or framebuffer copy.
 * Input commits immediately; effects follow the current state and can be interrupted. */
typedef struct {int x,y,w,h;} ErlMotionRect;
static struct {
 int view,choice,confirm,dialogue,garage,ready,valid;
 ErlMotionRect from,to;
 float clock,focus,portrait,pulse;
} erl_motion={.view=-1,.focus=10,.portrait=10,.pulse=10};
static float erl_motion_ease(float t){t=fmaxf(0,fminf(1,t));float a=1-t;return 1-a*a*a;}
static int erl_motion_view(void){
 if(game.erl.event_kind){if(game.erl.result)return 100;if(game.erl.paused&&race_is_active(&game))return 101;if(race_is_active(&game))return -1;}
 return page==ERL_PAGE?erl_screen:-1;
}
static ErlMotionRect erl_motion_rect(int view,int choice){
 ErlMotionRect r={0,0,0,0};
 if(view==100){int two=game.erl.event_kind==ERL_OFFICIAL||game.erl.result_place!=1;int win=game.erl.result==1&&game.erl.result_place==1;r=win?(ErlMotionRect){12+(two&&choice?232:0),246,two?224:456,24}:(ErlMotionRect){8+(two&&choice?236:0),240,two?228:464,24};}
 else if(view==101)r=(ErlMotionRect){80+((erl_confirm?!erl_confirm_yes:choice)?164:0),erl_confirm?152:155,148,24};
 else if(erl_confirm)r=(ErlMotionRect){80+(erl_confirm_yes?0:164),155,148,24};
 else if(view==ERL_HOME&&choice==5)r=(ErlMotionRect){102,214,124,24};
 else if(view==ERL_HOME)r=(ErlMotionRect){8+94*(choice<0?0:choice>4?4:choice),240,88,24};
 else if(view==ERL_EVENTS)r=(ErlMotionRect){7+158*(choice<0?0:choice>2?2:choice),193,148,52};
 else if(view==ERL_RIVALS){choice=choice<0?0:choice>5?5:choice;r=(ErlMotionRect){7+158*(choice%3),191+25*(choice/3),148,22};}
 else if(view==ERL_GARAGE)r=(ErlMotionRect){8,240,464,24};
 else if(view==ERL_SEASON)r=(ErlMotionRect){8+(choice?236:0),240,228,24};
 else if(view==ERL_BRIEF||view==ERL_MECHANIC)r=(ErlMotionRect){choice?322:8,240,choice?150:306,24};
 else if(view==ERL_DRIVER)r=(ErlMotionRect){8,220,464,27};
 else if(view==ERL_TALK||view==ERL_WELCOME||view==ERL_RIVAL_TALK){
  int count=dialogue.count;if(count<1)count=view==ERL_TALK?2:1;if(count>4)count=4;choice=choice<0?0:choice>=count?count-1:choice;
  if(count==4)r=(ErlMotionRect){12+(choice%2)*232,146+(choice/2)*52,224,44};
  else if(dialogue.context_page&&count==3)r=(ErlMotionRect){12,184+choice*20,456,19};
  else{int w=(456-8*(count-1))/count;r=(ErlMotionRect){12+choice*(w+8),dialogue.context_page?194:154,w,dialogue.context_page?48:88};}
 }
 return r;
}
static void erl_motion_reset(void){
 memset(&erl_motion,0,sizeof(erl_motion));erl_motion.view=-1;erl_motion.focus=erl_motion.portrait=erl_motion.pulse=10;erl_motion_entry_age=10;
}
static void erl_motion_sync(void){
 int view=erl_motion_view(),choice=erl_confirm?erl_confirm_yes:erl_choice;
 if(view<0){erl_motion.view=-1;erl_motion.valid=0;erl_motion_entry_age=10;return;}
 int changed=!erl_motion.valid||view!=erl_motion.view;
 ErlMotionRect next=erl_motion_rect(view,erl_choice);
 if(changed){
  /* Leave the authored victory tunnel/reveal untouched. */
  erl_motion_entry_age=view<100?0:10;erl_motion.focus=10;
  erl_motion.portrait=view<100?-.09f:10;erl_motion.from=erl_motion.to=next;
 }else{
  if(choice!=erl_motion.choice||erl_confirm!=erl_motion.confirm||dialogue_page!=erl_motion.dialogue||erl_garage_row!=erl_motion.garage){
   float p=erl_motion_ease(erl_motion.focus/.12f);ErlMotionRect a=erl_motion.from,b=erl_motion.to;
   erl_motion.from=(ErlMotionRect){a.x+(int)((b.x-a.x)*p),a.y+(int)((b.y-a.y)*p),a.w+(int)((b.w-a.w)*p),a.h+(int)((b.h-a.h)*p)};
   erl_motion.to=next;erl_motion.focus=0;
   if(view==ERL_RIVALS&&choice!=erl_motion.choice)erl_motion.portrait=0;
  }
 }
 erl_motion.view=view;erl_motion.choice=choice;erl_motion.confirm=erl_confirm;erl_motion.dialogue=dialogue_page;erl_motion.garage=erl_garage_row;erl_motion.valid=1;
}
static void erl_motion_tick(float dt){
 erl_motion_sync();if(!(dt>0))dt=0;if(dt>.25f)dt=.25f;
 erl_motion.clock=fmodf(fmaxf(0,erl_motion.clock)+dt,4096);erl_motion.focus=fminf(10,erl_motion.focus+dt);erl_motion.portrait=fminf(10,erl_motion.portrait+dt);erl_motion.pulse=fminf(10,erl_motion.pulse+dt);erl_motion_entry_age=fminf(10,erl_motion_entry_age+dt);
}
static void erl_motion_rim(ErlMotionRect r,unsigned ink){
 if(r.w<14||r.h<14)return;
 line(r.x+6,r.y,r.x+r.w-7,r.y,ink);line(r.x+6,r.y+r.h-1,r.x+r.w-7,r.y+r.h-1,ink);
 line(r.x,r.y+6,r.x,r.y+r.h-7,ink);line(r.x+r.w-1,r.y+6,r.x+r.w-1,r.y+r.h-7,ink);
 line(r.x,r.y+6,r.x+6,r.y,ink);line(r.x+r.w-7,r.y,r.x+r.w-1,r.y+6,ink);
 line(r.x,r.y+r.h-7,r.x+6,r.y+r.h-1,ink);line(r.x+r.w-7,r.y+r.h-1,r.x+r.w-1,r.y+r.h-7,ink);
}
static void erl_motion_draw(void){
 int view=erl_motion_view();if(view<0||!erl_motion.valid||view!=erl_motion.view)return;
 /* Dialogue lays out replies while drawing; use its freshly resolved geometry. */
 if(view==ERL_TALK||view==ERL_WELCOME||view==ERL_RIVAL_TALK){erl_motion.to=erl_motion_rect(view,erl_choice);if(erl_motion.focus>=.12f)erl_motion.from=erl_motion.to;}
 /* Broadcast Neon: focus travels for 120ms; its light stays off the text. */
 if(erl_motion.focus<.12f||erl_motion.pulse<.18f){
  float p=erl_motion_ease(erl_motion.focus/.12f);ErlMotionRect a=erl_motion.from,b=erl_motion.to;
  ErlMotionRect r={a.x+(int)((b.x-a.x)*p),a.y+(int)((b.y-a.y)*p),a.w+(int)((b.w-a.w)*p),a.h+(int)((b.h-a.h)*p)};
  erl_motion_rim(r,ET);if(r.w>24){int x=r.x+8+(int)((r.w-24)*p);line(x,r.y+2,x+8,r.y+2,EC);}
 }
 if(view<100){
  /* Very sparse star shimmer only touches existing backdrop pixels. */
  for(int i=0;i<16;i++){int x=(i*167+13)%480,y=(i*83+17)%272;unsigned c=fb[y*STRIDE+x];if(y>25&&y<240&&(c==RGB(36,50,78)||c==RGB(92,119,153))){int light=(int)(erl_motion.clock*6+i)%24;if(light<4)pixel(x,y,planet_rgb_mix(c,ET,36+light*9));}}
  if(view==ERL_SEASON){
   static const int px[]={45,166,291,427},py[]={105,157,104,155};float travel=fmodf(fmaxf(0,erl_motion.clock),2)*1.5f;int i=(int)travel;if(i<0)i=0;if(i>2)i=2;float p=fmaxf(0,fminf(1,travel-i));
   int x=px[i]+(px[i+1]-px[i])*p,y=py[i]+(py[i+1]-py[i])*p;
   if(p>.20f&&p<.80f)rect(x-1,y-1,3,3,EC);
  }
  /* Paddock Signal: veil aligned portrait bands, then restore the original pixels.
   * The actual DNA, masks and palettes remain entirely in portrait_draw. */
  if(view==ERL_RIVALS||view==ERL_TALK||view==ERL_WELCOME||view==ERL_RIVAL_TALK){
   int x=368,y=view==ERL_RIVALS?38:34,w=view==ERL_RIVALS?94:96,h=view==ERL_RIVALS?94:96;
   float p=erl_motion_ease(erl_motion.portrait/.16f);
   if(p<1){for(int band=0;band<8;band++){int top=h*band/8,bottom=h*(band+1)/8;if(p<(band+1)/8.f)rect(x,y+top,w,bottom-top,RGB(16,20,28));}int scan=(int)(p*(h-1));line(x,y+scan,x+w-1,y+scan,ET);}
   else{float phase=fmodf(erl_motion.clock,3.6f);if(phase<1.2f){int scan=y+(int)(phase*(h-1)/1.2f);for(int xx=x;xx<x+w;xx++)fb[scan*STRIDE+xx]=planet_rgb_mix(fb[scan*STRIDE+xx],ET,14);}}
  }
  /* Slipstream: one 220ms incoming diagonal. The page is live underneath. */
  if(erl_motion_entry_age<.22f){
   float p=erl_motion_ease(erl_motion_entry_age/.22f);int front=-120+(int)(p*720);
   for(int y=25;y<253;y++){int edge=front-(y-25)*80/228,left=edge<0?0:edge;if(left<W)rect(left,y,W-left,1,EB);int a=edge-14,b=edge;if(a<0)a=0;if(b>=W)b=W-1;if(a<=b)line(a,y,b,y,ER);if(edge>=0&&edge<W)pixel(edge,y,ET);}
  }
 }
}
