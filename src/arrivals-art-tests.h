/* Opt-in, disposable framebuffer test; production drawing owns no test state. */
{
 FILE *flag=fopen("arrivals-art.flag","r");
 if(flag){
  fclose(flag);Game *saved=malloc(sizeof(Game));
  INPUT_CHECK(saved!=0,"arrivals art: test snapshot allocated");
  if(saved){
   memcpy(saved,&game,sizeof(Game));
   int oldroom=sc_room,oldhot=sc_hot,oldmenu=sc_menu,oldbuilt=sc_built_for,oldhc=high_contrast;
   FILE *out=fopen("arrivals-art-check.txt","w");
   double sum=0,worst=0;int count=0,contained=1,stable=1,repeat=1,palette_ok=1;
   const int systems[]={7,39,0,255};
   for(int s=0;s<4;s++)for(int mode=0;mode<2;mode++){
    game.system=systems[s];system_bodies(&game);game.docked=1;
    sc_built_for=game.system;sc_room=SC_R_ARRIVALS;sc_hot=0;sc_menu=0;high_contrast=mode;
    for(int frame=0;frame<12;frame++){
     game.time=frame*8.f;Game before=game;
     for(int k=0;k<STRIDE*H;k++)pixels[k]=0x123456;
     uint64_t a,b;sceRtcGetCurrentTick(&a);sc_illust_arrivals(6,20,340,168);sceRtcGetCurrentTick(&b);
     double ms=(b-a)*1000.0/sceRtcGetTickResolution();sum+=ms;if(ms>worst)worst=ms;count++;
     if(memcmp(&before,&game,sizeof(Game)))stable=0;
     for(int yy=0;yy<H;yy++)for(int xx=0;xx<STRIDE;xx++)
      if((xx<6||xx>=346||yy<20||yy>=188)&&pixels[yy*STRIDE+xx]!=0x123456)contained=0;
     #ifdef ARRIVALS_COVER_ART
     /* Shared unchanged orbit sprite is an explicit palette exception. */
     const unsigned inks[]={SC_VOID,SC_CHAR,SC_SLATE,SC_OLIVE,SC_RUST,SC_OCHRE,SC_CREAM,SC_LAV,SC_CYAN,SC_AMBER,SC_DANGER};
     for(int yy=20;yy<188;yy++)for(int xx=6;xx<346;xx++){
      if(xx>=213&&xx<=273&&yy>=34&&yy<=94)continue;
      int found=0;for(int i=0;i<11;i++)if(pixels[yy*STRIDE+xx]==inks[i])found=1;
      if(!found)palette_ok=0;
     }
     #endif
    }
    unsigned hashes[2];
    for(int pass=0;pass<2;pass++){
     game.time=0;sc_illust_arrivals(6,20,340,168);unsigned hash=2166136261u;
     for(int yy=20;yy<188;yy++)for(int xx=6;xx<346;xx++)hash=(hash^pixels[yy*STRIDE+xx])*16777619u;
     hashes[pass]=hash;
    }
    if(hashes[0]!=hashes[1])repeat=0;
    memset(pixels,0,STRIDE*H*sizeof(unsigned));sc_draw_ui();
    char path[64];snprintf(path,sizeof(path),"arrivals-%03d-%s.bmp",systems[s],mode?"contrast":"normal");dump_native_bmp(path);
   }
   INPUT_CHECK(contained,"arrivals art: exact native viewport containment");
   INPUT_CHECK(stable,"arrivals art: every Game byte unchanged");
   INPUT_CHECK(repeat,"arrivals art: deterministic repeated frame");
   #ifdef ARRIVALS_COVER_ART
   INPUT_CHECK(palette_ok,"arrivals art: authored scenery uses shared eleven colours");
   INPUT_CHECK(sum/count<4.0&&worst<6.0,"arrivals art: emulator draw budget below 4ms mean and 6ms worst");
   #endif
   if(out){fprintf(out,"Samples %d; mean %.4f ms; worst %.4f ms\n%s containment\n%s Game unchanged\n%s deterministic\n",count,sum/count,worst,contained?"PASS":"FAIL",stable?"PASS":"FAIL",repeat?"PASS":"FAIL");fclose(out);}
   memcpy(&game,saved,sizeof(Game));free(saved);
   sc_room=oldroom;sc_hot=oldhot;sc_menu=oldmenu;sc_built_for=oldbuilt;high_contrast=oldhc;
  }
 }
}
