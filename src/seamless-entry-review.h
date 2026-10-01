static void entry_reference_cloud(float amount){
 amount=pilot_clamp(amount);if(amount<=0)return;
 unsigned ink=planet_veil_ink?planet_veil_ink:RGB(188,210,220);
 for(int y=0;y<H;y+=4)for(int x=0;x<W;x+=4){
  float wave=.5f+.25f*sinf(x*.026f+y*.015f+game.time*.8f)+.25f*sinf(y*.045f-x*.011f-game.time*.45f);
  float alpha=pilot_clamp(amount*(.7f+.4f*wave)+amount*amount*.3f);if(amount>=.999f)alpha=1;
  unsigned c=mix_rgb(ink,RGB(226,237,240),wave*.15f);
  int weight=(int)(alpha*256),inv=256-weight;unsigned rb=(c&0xff00ff)*weight,g=(c&0x00ff00)*weight;
  for(int yy=y;yy<y+4&&yy<H;yy++)for(int xx=x;xx<x+4&&xx<W;xx++){unsigned p=fb[yy*STRIDE+xx];fb[yy*STRIDE+xx]=0xff000000|((((p&0xff00ff)*inv+rb)>>8)&0xff00ff)|((((p&0x00ff00)*inv+g)>>8)&0x00ff00);}
 }
}

static int seamless_entry_review(void){
 FILE *flag=fopen("seamless-entry.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("seamless-entry.txt","w");if(!f)return 1;
 unsigned *saved=fb,*p=calloc(STRIDE*H+32,sizeof(unsigned)),*reference=calloc(STRIDE*H,sizeof(unsigned));
 if(!p||!reference){fclose(f);free(p);free(reference);return 1;}
 fb=p+16;for(int i=0;i<16;i++)p[i]=p[STRIDE*H+16+i]=0xa55ac33cu;int failures=0;
 #define ECHECK(ok,name) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",name);failures+=!pass;fflush(f);}while(0)
 for(int body=1;body<=4;body++)for(int fast=0;fast<2;fast++){
  pilot_review_reset(body);game.approach=-1;game.pos=add(game.bodies[body].pos,(Vec3){0,0,-game.bodies[body].radius-950});
  game.yaw=game.pitch=0;game.speed=fast?3000:300;Vec3 from=game.pos;float speed=game.speed;
  input(0,0,.016f,0,0);
  ECHECK(planet_entry_body==body&&game.planet<0&&game.speed==speed&&length(sub(from,game.pos))>0,"inward flight starts moving cloud entry without confirmation");
  float cover=planet_entry_cover;ECHECK(cover>0&&cover<=.85f,"boost entry accelerates cloud cover before safety shell");
  unsigned spam=PSP_CTRL_CROSS|PSP_CTRL_TRIANGLE|PSP_CTRL_CIRCLE|PSP_CTRL_SQUARE|PSP_CTRL_START|PSP_CTRL_SELECT|PSP_CTRL_RTRIGGER;
  for(int n=0;n<400&&planet_transfer_busy();n++)input(spam,spam,.05f,1,1);
  ECHECK(game.planet==body&&game.surface==2&&!game.dead&&!planet_transfer_busy()&&page==FLIGHT,"automatic arrival completes once despite button spam");
  input(PSP_CTRL_TRIANGLE,spam,.016f,0,0);ECHECK(game.surface==2,"carried entry buttons cannot immediately board");
  input(0,0,.016f,0,0);
 }
 pilot_review_reset(1);game.approach=-1;game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-950});game.speed=300;game.yaw=3.14159265f;game.pitch=0;
 input(0,0,.016f,0,0);ECHECK(!planet_entry_body&&game.planet<0,"outward flight near a planet does not force landing");
 game.yaw=1.5707963f;input(0,0,.016f,0,0);ECHECK(!planet_entry_body,"passing alongside a planet does not force landing");
 game.yaw=0;game.speed=0;input(0,0,.016f,0,0);ECHECK(!planet_entry_body,"stationary ship near a planet keeps control");
 pilot_review_reset(1);game.approach=-1;game.pos=add(game.bodies[0].pos,(Vec3){0,0,-game.bodies[0].radius-950});game.speed=300;game.yaw=game.pitch=0;
 input(0,0,.016f,0,0);ECHECK(!planet_entry_body&&game.planet<0,"sun never becomes a landing target");
 pilot_review_reset(1);game.story=STORY_BRIEF;game.story_flags=0;game.approach=-1;game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-950});game.speed=300;game.yaw=game.pitch=0;
 game_input(0,0,.016f,0,0);ECHECK(!planet_entry_body&&game.approach<0&&game.planet<0,"missing landing equipment preserves story gate without confirmation menu");
 for(int phase=0;phase<3;phase++){
  float amount=phase==0?.2f:phase==1?.65f:1;game.time=31.7f;planet_veil_ink=RGB(188,210,220);
  rect(0,0,W,H,RGB(17,51,90));entry_reference_cloud(amount);memcpy(reference,fb,STRIDE*H*sizeof(unsigned));
  rect(0,0,W,H,RGB(17,51,90));planet_cloud_cover(amount);int error=0;
  for(int y=0;y<H;y++)for(int x=0;x<W;x++){unsigned a=reference[y*STRIDE+x],b=fb[y*STRIDE+x];for(int channel=0;channel<3;channel++)if(abs((int)((a>>(channel*8))&255)-(int)((b>>(channel*8))&255))>2)error++;}
  ECHECK(!error,"optimised clouds preserve drifting veil within two colour levels");
  uint64_t a=sceKernelGetSystemTimeWide();for(int n=0;n<12;n++)entry_reference_cloud(amount);uint64_t b=sceKernelGetSystemTimeWide();for(int n=0;n<12;n++)planet_cloud_cover(amount);uint64_t c=sceKernelGetSystemTimeWide();
  fprintf(f,"PERF cloud %.2f old %.3fms new %.3fms\n",amount,(b-a)/12000.f,(c-b)/12000.f);
 }
 int guard=1;for(int i=0;i<16;i++)guard&=p[i]==0xa55ac33cu&&p[STRIDE*H+16+i]==0xa55ac33cu;
 ECHECK(guard,"framebuffer guards preserved");
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=saved;free(p);free(reference);return 1;
 #undef ECHECK
}
