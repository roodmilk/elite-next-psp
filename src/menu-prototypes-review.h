/* Review-only menu exploration, rendered by the PSP executable. */
static void menu_proto_panel(int x,int y,int w,int h,unsigned edge,int alpha){
 for(int yy=y;yy<y+h&&yy<H;yy++)for(int xx=x;xx<x+w&&xx<W;xx++)if(xx>=0&&yy>=0)fb[yy*STRIDE+xx]=intro_mix(fb[yy*STRIDE+xx],RGB(3,14,25),alpha);
 line(x,y,x+w-1,y,edge);line(x,y+h-1,x+w-1,y+h-1,RGB(30,66,78));
}
static void menu_proto_draw(int style,float t){
 intro_movie_draw(40+t);intro_logo(1);
 float enter=fminf(1,t*3),ease=1-powf(1-enter,3);int sel=((int)(t*1.4f))%4;
 unsigned accent=style==4?RGB(255,157,67):style==5?RGB(170,154,255):CYAN;
 const char *labels[]={"CONTINUE","NEW COMMANDER","LOAD COMMANDER","EXTRAS"};
 int x=20,y=150,w=226,h=94,step=21;
 if(style==1){x=24-(int)((1-ease)*260);w=432;}
 if(style==2){x=24;y=157;w=208;h=83;step=19;}
 if(style==3){x=122;y=143;w=236;h=101;step=22;}
 if(style==4){x=18;y=148;w=246;h=96;}
 if(style==5){x=18;y=151;w=444;h=90;}
 if(style==6){x=28;y=150;w=212;h=94;}
 if(style==7){x=16;y=175;w=448;h=65;}
 if(style!=6)menu_proto_panel(x,y,w,h,accent,style==2?170:220);
 if(style==7){
  for(int i=0;i<4;i++){int bx=x+6+i*110;menu_proto_panel(bx,y+8,106,42,i==sel?GOLD:RGB(40,78,91),190);text_px(bx+7,y+17,i==sel?GOLD:WHITE,"%s",i==1?"NEW":i==2?"LOAD":labels[i]);if(i==sel)line(bx+7,y+40,bx+95,y+40,accent);}
 }else{
  for(int i=0;i<4;i++){int yy=y+9+i*step,xx=x+14;
   if(style==6){menu_proto_panel(x,yy-4,w,19,i==sel?accent:RGB(30,65,78),i==sel?230:170);xx+=i==sel?(int)(2+2*sinf(t*6)):0;}
   if(i==sel){
    if(style==2){line(x+5,yy-3,x+5,yy+10,accent);line(x+5,yy-3,x+11,yy-3,accent);line(x+w-6,yy+10,x+w-12,yy+10,accent);}
    else if(style==4){rect(x+4,yy-4,w-8,18,RGB(54,33,23));rect(x+4,yy-4,3,18,GOLD);}
    else if(style!=6){rect(x+4,yy-4,style==1?212:w-8,18,RGB(10,44,57));line(x+5,yy+13,x+(int)((style==1?210:w-10)*(.7f+.3f*sinf(t*3)*sinf(t*3))),yy+13,accent);}
   }
   text_px(xx,yy,i==sel?GOLD:WHITE,"%s",labels[i]);
  }
 }
 if(style!=3&&style!=7){int px=style==1||style==5?278:280;menu_proto_panel(px,151,180,90,accent,200);draw_commander_portrait(px+9,163,40,portrait_seeded(7,EXPLORERS,0));text_px(px+58,164,GOLD,"NEW PILOT");text_px(px+58,181,accent,"FIRST LIGHT");text_px(px+9,218,WHITE,"EARTH / 2050");}
 if(style==4)for(int k=0;k<6;k++)rect(252,156+k*13,3,7,k==((int)(t*5)%6)?GOLD:RGB(69,56,33));
 if(style==5){int scan=152+(int)(t*28)%86;line(20,scan,460,scan,RGB(39,48,70));}
 if(enter<1)for(int yy=143;yy<H;yy++)for(int xx=0;xx<W;xx++)fb[yy*STRIDE+xx]=intro_mix(intro_movie_pixels[yy*W+xx],fb[yy*STRIDE+xx],(int)(ease*256));
 rect(0,250,W,22,RGB(4,12,22));text_px(12,258,WHITE,"D-PAD");button_icon(67,254,'X',accent);text_px(83,258,WHITE,"OPEN");button_icon(150,254,'S',accent);text_px(166,258,WHITE,"MULTIPLAYER");text_px(330,258,accent,"SELECT INTRO");
}
static int menu_prototypes_review(void){
 FILE *flag=fopen("menu-prototypes.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("menu-prototypes.txt","w");if(!report)return 1;setvbuf(report,0,_IONBF,0);
 unsigned *old=fb;fb=calloc(STRIDE*H,sizeof(unsigned));if(!fb){fclose(report);return 1;}
 page=INTRO;intro_movie_attempted=0;int ok=intro_movie_open();fprintf(report,"%s actual embedded PSP cinematic decoded\n",ok?"PASS":"FAIL");
 if(ok)for(int s=0;s<8;s++){for(int f=0;f<32;f++){menu_proto_draw(s,f/8.f);char path[64];snprintf(path,sizeof(path),"menu-%02d-%02d.bmp",s,f);dump_native_bmp(path);}fprintf(report,"PASS style %d captured 32 native 480x272 animation frames\n",s);}
 intro_movie_close();free(fb);fb=old;fprintf(report,"RESULT %d failures\n",!ok);fclose(report);return 1;
}
