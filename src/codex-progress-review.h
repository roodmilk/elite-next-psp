/* Pixel checks of the actual native System Record bar and reached arrows. */
static int codex_progress_review(void){
 FILE *flag=fopen("codex-progress-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *out=fopen("codex-progress-review.txt","w");if(!out)return 1;
 unsigned *old=fb,*pixels=calloc(STRIDE*H+32,sizeof(unsigned));if(!pixels){fprintf(out,"RESULT 1 failures\n");fclose(out);return 1;}fb=pixels+16;
 for(int i=0;i<16;i++)pixels[i]=pixels[STRIDE*H+16+i]=0x17ce5a83u;int failures=0;
#define CRCHECK(c,s) do{int ok=!!(c);fprintf(out,"%s %s\n",ok?"PASS":"FAIL",s);failures+=!ok;fflush(out);}while(0)
 game_init(&game);story_complete(&game);game.system=7;system_bodies(&game);page=CODEX;atlas_reset();row=0;preview_time=18;unsigned paint=ship_paint[game.ship];
 atlas_record_bar(0,0);CRCHECK(fb[203*STRIDE+300]==RGB(8,16,29),"zero percent has an empty recessed track");
 atlas_record_bar(50,0);CRCHECK(fb[203*STRIDE+300]==UI_CYAN&&fb[203*STRIDE+400]==RGB(8,16,29),"half progress visibly fills only the left half instead of being painted over");
 atlas_record_bar(100,0);CRCHECK(fb[203*STRIDE+450]==UI_CYAN,"100 percent fills the entire inner track");
 int spans=1,borders=1;
 for(int t=0;t<DECORATOR_COUNT;t++){ship_paint[game.ship]=decorator_finishes[t];for(int pct=0;pct<=100;pct++){
  atlas_record_bar(pct,0);for(int x=257;x<451;x++){
   int marker=0;for(int l=1;l<=10;l++)marker|=x==256+196*l/10;
   if(!marker)for(int y=202;y<205;y++)spans&=fb[y*STRIDE+x]==(x<257+194*pct/100?UI_CYAN:RGB(8,16,29));
  }borders&=fb[201*STRIDE+257]==UI_EDGE&&fb[205*STRIDE+257]==UI_EDGE;
 }}CRCHECK(spans&&borders,"every percentage from 0 through 100 has the exact visible fill and intact frame in all sixteen ship themes");
 atlas_record_bar(60,0);int lit=1;for(int l=1;l<=6;l++)lit&=fb[196*STRIDE+256+196*l/10]==UI_CYAN;
 CRCHECK(lit,"all six reached arrows light up even when none of the rewards has been claimed");
 atlas_record_bar(60,2);CRCHECK(fb[196*STRIDE+275]==UI_GOLD&&fb[196*STRIDE+295]==UI_GOLD&&fb[196*STRIDE+373]==UI_CYAN,"claimed arrows stay gold while other reached arrows remain illuminated");
 int future=1;for(int l=7;l<=10;l++)future&=fb[196*STRIDE+256+196*l/10]==UI_EDGE;CRCHECK(future,"future checkpoint arrows remain dim");
 atlas_record_bar(-5,0);int clamped=fb[203*STRIDE+300]==RGB(8,16,29);atlas_record_bar(105,0);clamped&=fb[203*STRIDE+450]==UI_CYAN;
 CRCHECK(clamped,"out-of-range percentages clamp safely to the track");
 ship_paint[game.ship]=paint;int production=1,unchanged=1;
 for(int k=0;k<4;k++){
  game.landed_planets[7]=k?3:0;game.rift_logged[7]=0;game.bounty_claimed[7]=0;
  if(k>=2){game.landed_planets[7]=0x0fff;game.rift_logged[7]=15;game.bounty_claimed[7]=31;for(int b=1;b<BODY_COUNT;b++)game.surface_progress[7][b]=0xffffffffu;}
  if(k==3)atlas_set_claimed_level(7,10);
  int pct=atlas_progress_percent(7),credits=game.credits;unsigned landed=game.landed_planets[7];rect(0,0,W,H,BG);codex_screen();
  for(int x=257;x<451;x++){int marker=0;for(int l=1;l<=10;l++)marker|=x==256+196*l/10;if(!marker)production&=fb[203*STRIDE+x]==(x<257+194*pct/100?UI_CYAN:RGB(8,16,29));}
  unchanged&=game.credits==credits&&game.landed_planets[7]==landed;
  char file[64];snprintf(file,sizeof(file),"codex-progress-%d-%d-percent.bmp",k,pct);dump_native_bmp(file);
 }
 CRCHECK(production,"the actual Discovery Codex dossier displays the correct fill for charted, explored and completed systems");
 CRCHECK(unchanged,"drawing the bar never claims rewards or changes saved exploration progress");
 int guards=1;for(int i=0;i<16;i++)guards&=pixels[i]==0x17ce5a83u&&pixels[STRIDE*H+16+i]==0x17ce5a83u;
 CRCHECK(guards,"native bar and dossier renders preserve framebuffer guard words");
 fprintf(out,"RESULT %d failures\n",failures);fclose(out);fb=old;free(pixels);
#undef CRCHECK
 return 1;
}
