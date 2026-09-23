/* Optional matched baseline/candidate benchmark. Allocations are test-only. */
{
 FILE *flag=fopen("menu-preview.flag","r");
 if(flag){
  fclose(flag);
  Game *saved=(Game*)malloc(sizeof(Game));
  INPUT_CHECK(saved!=0,"menu preview: test state allocated");
  if(saved){
   memcpy(saved,&game,sizeof(Game));
   float old_time=preview_time;int old_page=page,old_row=row,old_hc=high_contrast;
   FILE *report=fopen("menu-preview-check.txt","w");
   int contained=1,unchanged=1;double total=0,worst=0;int samples=0;
   for(int ship=0;ship<player_ship_count;ship++){
    game.ship=ship;
    for(int f=0;f<12;f++){
     preview_time=f*8.f;high_contrast=f&1;
     for(int k=0;k<STRIDE*H;k++)pixels[k]=0x123456;
     Game before=game;
     uint64_t a,b;sceRtcGetCurrentTick(&a);menu_space_view(246,64,218,92);sceRtcGetCurrentTick(&b);
     double ms=(b-a)*1000.0/sceRtcGetTickResolution();total+=ms;if(ms>worst)worst=ms;samples++;
     if(memcmp(&before,&game,sizeof(Game)))unchanged=0;
     for(int y=0;y<H;y++)for(int x=0;x<STRIDE;x++)
      if((x<246||x>=464||y<64||y>=156)&&pixels[y*STRIDE+x]!=0x123456)contained=0;
    }
   }
   if(report){fprintf(report,"Preview samples %d; mean %.4f ms; worst %.4f ms\n",samples,total/samples,worst);
    fprintf(report,"%s outside-panel sentinel\n%s simulation state unchanged\n",contained?"PASS":"FAIL",unchanged?"PASS":"FAIL");}
   INPUT_CHECK(contained,"menu preview: all owned hulls stay inside exact panel");
   INPUT_CHECK(unchanged,"menu preview: every Game byte stays unchanged");
   #ifdef MENU_BEAUTY_PREVIEW
   {
    int stable=1,fits=1,repeatable=1,loop=1;
    for(int state=0;state<3;state++){
     game.planet=state?1:-1;game.surface=state;game.docked=state==0;
     game.pitch=.31f;game.yaw=.73f;game.hazard=63;game.jetpack=-18;game.boost=0;
     for(int ship=0;ship<player_ship_count;ship++)for(int f=0;f<48;f++){
      game.ship=ship;preview_time=f*2.f;Game before=game;
      int id=mesh_id(player_ships[ship].name);Vec3 centre;float scale;menu_hull_layout(id,92,&centre,&scale);
      float ph=preview_time*6.283185307f/96.f;
      for(int v=0;v<meshes[id].vertices;v++){
       Vec3 p=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(meshes[id].v[v],centre),.8f+.65f*sinf(ph),-.48f-.08f*sinf(ph*2),.04f*sinf(ph)),scale));
       float sx=355+p.x*240/p.z,sy=113-p.y*240/p.z;
       if(sx<250||sx>460||sy<68||sy>152)fits=0;
      }
      preview_clip(199,91,11,12,470,250);
      menu_space_view(246,64,218,92);
      if(memcmp(&before,&game,sizeof(Game))||proj_ox!=199||proj_oy!=91||clipx0!=11||clipy0!=12||clipx1!=470||clipy1!=250)stable=0;
     }
    }
    preview_reset();game.ship=0;
    unsigned hashes[3];float times[]={0,96,0};
    for(int k=0;k<3;k++){
     preview_time=times[k];menu_space_view(246,64,218,92);unsigned hash=2166136261u;
     for(int yy=64;yy<156;yy++)for(int xx=246;xx<464;xx++)hash=(hash^pixels[yy*STRIDE+xx])*16777619u;
     hashes[k]=hash;
    }
    repeatable=hashes[0]==hashes[2];loop=hashes[0]==hashes[1];
    INPUT_CHECK(stable,"menu preview: landed/EVA menu preserves simulation and caller render state");
    INPUT_CHECK(fits,"menu preview: every hull vertex has margin across camera arc");
    INPUT_CHECK(repeatable&&loop,"menu preview: deterministic frames and exact 96 second loop");
    INPUT_CHECK(total/samples<2.0&&worst<4.0,"menu preview: emulator budget below 2ms mean and 4ms worst");
    if(report)fprintf(report,"%s landed/EVA state restoration\n%s all-hull geometric margin\n%s deterministic loop\n",stable?"PASS":"FAIL",fits?"PASS":"FAIL",repeatable&&loop?"PASS":"FAIL");
   }
   #endif
   /* Full menu evidence for two very different owned hulls, not cropped mockups. */
   page=HOME;row=0;high_contrast=0;game.message_time=game.voice_time=0;
   for(int s=0;s<2;s++)for(int f=0;f<4;f++){
    game.ship=s?player_ship_count-1:0;preview_time=f*24.f;
    memset(pixels,0,STRIDE*H*sizeof(unsigned));home();
    char path[64];snprintf(path,sizeof(path),"menu-ship-%d-frame-%d.bmp",game.ship,f);dump_native_bmp(path);
   }
   if(report)fclose(report);
   memcpy(&game,saved,sizeof(Game));free(saved);
   preview_time=old_time;page=old_page;row=old_row;high_contrast=old_hc;preview_reset();
  }
 }
}
