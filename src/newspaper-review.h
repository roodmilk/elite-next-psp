static int newspaper_review(void){
 FILE *flag=fopen("newspaper-review.flag","r");if(!flag)return 0;int capture=0;fscanf(flag,"%d",&capture);fclose(flag);
 FILE *f=fopen("newspaper-review.txt","w");if(!f)return 1;int failures=0;unsigned *pixels=malloc(STRIDE*H*sizeof(unsigned)),*old_fb=fb;
 if(!pixels){fprintf(f,"RESULT 1 failures\n");fclose(f);return 1;}fb=pixels;
#define NCHECK(ok,label) do{int pass=!!(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(f);}while(0)
 game_init(&game);story_complete(&game);game.opening.stage=OP_FREE;deck_reset();change_page(GALNET);galnet_tab=0;preview_time=10;int total=0,fit=1,visible=1,canonical=1;unsigned layouts=0,palettes=0,families=0;unsigned long long draw_us=0,worst_us=0;
 static System original[256];galaxy(original);
 for(int s=0;s<256;s++){
  game.system=s;system_bodies(&game);newspaper_reset();int count=newspaper_identity[s].page_count;canonical&=count>=4&&count<=24;canonical&=!strcmp(original[s].name,game.systems[s].name);families|=1u<<newspaper_identity[s].publication_family;palettes|=1u<<newspaper_identity[s].palette;
  int suns=0;for(int body=0;body<system_body_slots(s);body++)suns+=system_body_type_slot(s,body)==SUN;char fact[128];snprintf(fact,sizeof(fact),"%s has %d %s and %d %s.",original[s].name,suns,suns==1?"star":"stars",system_body_slots(s)-suns,system_body_slots(s)-suns==1?"world":"worlds");
  for(row=0;row<count;row++){const NewspaperPage *p=newspaper_current();layouts|=1u<<p->layout;canonical&=!strcmp(fact,newspaper_strings[p->fact]);
   rect(0,0,W,H,BG);unsigned long long start=sceKernelGetSystemTimeWide();fit&=news_screen();draw_us=sceKernelGetSystemTimeWide()-start;if(draw_us>worst_us)worst_us=draw_us;galnet_chrome();
   for(int i=0;i<newspaper_line_count;i++){NewspaperLine *l=&newspaper_lines[i];fit&=l->x>=20&&l->y>=84&&l->x+(int)strlen(l->text)*8<=464&&l->y+8<=240;const NewspaperPrintIdentity *pi=newspaper_print_local();unsigned expected=(pi?pi->palette:newspaper_palette[newspaper_identity[s].palette])[l->ink?2:1];int ink_pixels=0;for(int yy=0;yy<8;yy++)for(int xx=0;xx<8;xx++)ink_pixels+=fb[(l->y+yy)*STRIDE+l->x+xx]==expected;visible&=ink_pixels>0;}
   if(capture||newspaper_composed_page()){char path[48];snprintf(path,sizeof(path),"news-s%03d-p%02d.bmp",s,row);dump_native_bmp(path);}total++;
  }
  if(s%16==15){fprintf(f,"Rendered systems through %d; pages %d\n",s,total);fflush(f);}
 }
 int composed_ink=1,print_static=1,paint_fixed=1,puzzle_apertures=1;unsigned long long print_worst=0;unsigned saved_print_paint=ship_paint[game.ship];
 for(unsigned j=0;j<sizeof(newspaper_compositions)/sizeof(*newspaper_compositions);j++){
  const NewspaperComposition *cp=&newspaper_compositions[j];game.system=cp->system;row=cp->page;newspaper_reset();preview_time=20;news_screen();
  for(int i=0;i<cp->count;i++){const NewspaperPrintOp *op=&newspaper_print_ops[cp->first+i];if(op->kind==1){int ink=0;for(int yy=0;yy<op->h;yy++)for(int xx=0;xx<op->w;xx++)ink+=fb[(op->y+yy)*STRIDE+op->x+xx]==op->ink;composed_ink&=ink>0;}}
  unsigned hash=2166136261u;for(int y=42;y<248;y++)for(int x=0;x<480;x++)hash=(hash^fb[y*STRIDE+x])*16777619u;
  preview_time=40;unsigned long long start=sceKernelGetSystemTimeWide();news_screen();unsigned long long us=sceKernelGetSystemTimeWide()-start;if(us>print_worst)print_worst=us;
  unsigned after=2166136261u;for(int y=42;y<248;y++)for(int x=0;x<480;x++)after=(after^fb[y*STRIDE+x])*16777619u;print_static&=hash==after;
  ship_paint[game.ship]=decorator_finishes[13];news_screen();unsigned tinted=2166136261u;for(int y=42;y<248;y++)for(int x=0;x<480;x++)tinted=(tinted^fb[y*STRIDE+x])*16777619u;paint_fixed&=hash==tinted;ship_paint[game.ship]=saved_print_paint;
  NewspaperBlock aperture;newspaper_puzzle_aperture(&aperture);puzzle_apertures&=(cp->puzzle!=0)==(aperture.w!=0);
  newspaper_input(PSP_CTRL_CROSS);puzzle_apertures&=(newspaper_mode!=0)==(cp->puzzle!=0);newspaper_mode=0;
 }
 NCHECK(composed_ink,"authored print lines produce their exact native ink colour");NCHECK(print_static,"authored printed pages remain static over time");NCHECK(paint_fixed,"all authored paper pixels stay identical when the ship is repainted red");NCHECK(puzzle_apertures,"X opens only printed puzzles and zooms from their actual aperture");fprintf(f,"Authored compositions %u; worst warm software draw %llu us\n",(unsigned)(sizeof(newspaper_compositions)/sizeof(*newspaper_compositions)),print_worst);
 NCHECK(canonical,"256 deterministic editions expose configured lengths and canonical names, star/world counts");NCHECK(fit,"all page text fits its native rectangle and the 480x272 safe frame");NCHECK(visible,"every body, headline and caption line produces visible contrasting ink pixels");NCHECK(layouts==65535&&families==65535&&palettes==16777215,"all 16 layouts, 16 families and 24 palettes render");
 static const short art_rect[][4]={{20,84,208,156},{252,84,208,156},{20,158,272,82},{172,84,144,156},{20,84,96,96},{20,84,176,92},{20,84,264,156},{20,186,440,54},{20,84,112,156},{20,84,264,76},{20,84,440,76}};
 int art_safe=1;for(unsigned r=0;r<sizeof(art_rect)/sizeof(art_rect[0]);r++)for(int art=0;art<12;art++){NewspaperBlock b={0};b.x=art_rect[r][0];b.y=art_rect[r][1];b.w=art_rect[r][2];b.h=art_rect[r][3];b.kind=1;b.art=art;rect(0,0,W,H,RGB(255,0,255));newspaper_art(&b,7,newspaper_palette[0]);for(int yy=0;yy<H;yy++)for(int xx=0;xx<W;xx++)if(xx<b.x+4||xx>=b.x+b.w-4||yy<b.y+4||yy>=b.y+b.h-28)art_safe&=fb[yy*STRIDE+xx]==RGB(255,0,255);}
 NCHECK(art_safe,"all 12 illustration subjects stay inside every native art aperture and out of captions");
 game.system=7;game.opening.stage=OP_EARTH_NET;opening_world_apply(&game);galnet_tab=0;newspaper_reset();int earth_ok=galnet_rows()==4&&!strcmp(newspaper_local_identity()->masthead,"The Earth Daily");for(row=0;row<4;row++){rect(0,0,W,H,BG);earth_ok&=news_screen();galnet_chrome();if(capture){char path[48];snprintf(path,sizeof(path),"news-earth-p%02d.bmp",row);dump_native_bmp(path);}}
 NCHECK(earth_ok,"tutorial Sol retains its separate four-page Earth paper without changing the 256 galaxy editions");game.opening.stage=OP_FREE;opening_world_apply(&game);
 game.system=7;system_bodies(&game);galnet_tab=0;row=0;newspaper_reset();newspaper_input(PSP_CTRL_UP);NCHECK(row==15,"Up on front page loops to last page");newspaper_input(PSP_CTRL_DOWN);NCHECK(row==0,"Down on last page loops to front");
 input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);NCHECK(galnet_tab==0&&row==15&&newspaper_turn_direction==-1&&game.cue==SFX_PAPER,"NEWS Left turns backwards and wraps without changing section");
 input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);NCHECK(galnet_tab==0&&row==0&&newspaper_turn_direction==1,"NEWS Right wraps from the last page to the front");
 preview_time+=.2f;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("news-navigation-front.bmp");
 input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);NCHECK(galnet_tab==0&&row==1,"NEWS Right advances the page through actual flight/menu input");
 preview_time+=.2f;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("news-navigation-next.bmp");
 input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);NCHECK(galnet_tab==0&&row==0,"NEWS Left reverses the page turn through actual input");
 row=4;input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);NCHECK(galnet_tab==5&&row==0,"NEWS L changes category to Rumours and resets the page");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);NCHECK(galnet_tab==0&&row==0,"Rumours R returns to NEWS");
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);NCHECK(galnet_tab==1&&row==0,"NEWS R changes category to Market");
 galnet_tab=0;row=5;newspaper_input(PSP_CTRL_CIRCLE);change_page(GALNET);galnet_tab=0;NCHECK(row==0&&!newspaper_mode,"leaving and reopening NEWS resets to front page");
 const NewspaperPage *fixed=newspaper_current();const char *copy=newspaper_strings[newspaper_blocks[fixed->first].body];char original_copy[768];snprintf(original_copy,sizeof(original_copy),"%s",copy);game.credits=999999;game.legal=99;game.campaign_stage=6;game.saga_chapter=20;game.price[0]=999;game.npc[0].alive=0;NCHECK(!strcmp(original_copy,newspaper_strings[newspaper_blocks[newspaper_current()->first].body]),"missions, prices, wanted status and traffic cannot alter immutable print copy");
 /* Sudoku editable cell, remembered digits, notes, deletion and real disk IO. */
 game.system=0;galnet_tab=0;row=newspaper_identity[0].page_count-2;newspaper_reset();newspaper_load();memset(&newspaper_answers,0,sizeof(newspaper_answers));newspaper_mode=1;newspaper_cell=2;
 newspaper_input(PSP_CTRL_CROSS);newspaper_number=newspaper_sudoku_solution(2);newspaper_input(PSP_CTRL_CROSS);int digit=newspaper_number;
 NCHECK(newspaper_answers.sudoku[2]==digit&&!newspaper_entry,"Sudoku strip places a valid number");newspaper_input(PSP_CTRL_CROSS);NCHECK(newspaper_number==digit,"Sudoku strip remembers its last number");newspaper_input(PSP_CTRL_CIRCLE);newspaper_input(PSP_CTRL_SQUARE);NCHECK(!newspaper_answers.sudoku[2],"Square erases editable Sudoku cell");
 newspaper_input(PSP_CTRL_TRIANGLE);newspaper_input(PSP_CTRL_CROSS);newspaper_input(PSP_CTRL_CROSS);NCHECK(newspaper_answers.notes[2]&(1<<(digit-1)),"pencil mode stores cell notes");newspaper_input(PSP_CTRL_CIRCLE);NCHECK(newspaper_mode==2,"Circle starts a smooth zoom back without leaving NEWS");preview_time+=.14f;news_screen();NCHECK(!newspaper_mode,"zoom-back completes and restores newspaper controls");
 newspaper_loaded_system=-1;newspaper_load();NCHECK(newspaper_answers.notes[2]&(1<<(digit-1)),"puzzle notes survive a disk reload");
 newspaper_mode=1;newspaper_cell=2;newspaper_notes=0;newspaper_entry=1;newspaper_number=newspaper_sudoku_value(0);newspaper_input(PSP_CTRL_CROSS);NCHECK(!newspaper_answers.sudoku[2]&&newspaper_entry,"conflicting Sudoku input is rejected without interrupting play");newspaper_entry=0;
 int before=game.credits;newspaper_input(PSP_CTRL_RTRIGGER);NCHECK(galnet_tab==0&&newspaper_mode&&game.credits==before,"Incomplete puzzle captures section buttons without a reward");
 preview_time=20;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("news-sudoku-zoom.bmp");newspaper_mode=0;
 game.system=1;row=newspaper_identity[1].page_count-2;newspaper_load();memset(&newspaper_answers,0,sizeof(newspaper_answers));newspaper_mode=1;newspaper_cell=0;newspaper_input(PSP_CTRL_CROSS);
 const char *word="SUN";for(int i=0;i<3;i++){newspaper_letter=word[i]-'A';newspaper_input(PSP_CTRL_CROSS);}
 NCHECK(newspaper_answers.crossword[0]=='S'&&newspaper_answers.crossword[1]=='U'&&newspaper_answers.crossword[2]=='N',"alphabet entry fills crossword cells");newspaper_input(PSP_CTRL_SQUARE);NCHECK(!newspaper_answers.crossword[2],"crossword Square deletes previous answer letter");
 newspaper_input(PSP_CTRL_TRIANGLE);NCHECK(newspaper_down,"Triangle switches crossword direction");newspaper_input(PSP_CTRL_RTRIGGER);NCHECK(newspaper_slot==1,"shoulder buttons select crossword answer slots");newspaper_letter=17;newspaper_input(PSP_CTRL_DOWN);int edge=newspaper_letter==25;newspaper_letter=18;newspaper_input(PSP_CTRL_UP);NCHECK(edge&&newspaper_letter==9,"alphabet rows retain their column and clamp the short final row");newspaper_input(PSP_CTRL_CIRCLE);NCHECK(!newspaper_entry&&newspaper_mode,"first Circle closes alphabet entry");newspaper_input(PSP_CTRL_CIRCLE);NCHECK(newspaper_mode==2,"second Circle starts returning to newspaper");preview_time+=.14f;news_screen();NCHECK(!newspaper_mode,"crossword zoom-back restores newspaper controls");
 newspaper_mode=1;newspaper_entry=1;preview_time=20;newspaper_zoom_started=19;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("news-crossword-zoom.bmp");newspaper_mode=0;
#include "newspaper-reward-review.h"
 fprintf(f,"Pages %d; worst cold software draw %llu us; puzzle state %u bytes; text cache %u bytes\n",total,worst_us,(unsigned)sizeof(newspaper_answers),(unsigned)sizeof(newspaper_lines));
 fprintf(f,"Editorial certification PENDING: draft corpus and individual native inspections are not approved by these tests.\nRESULT %d failures\n",failures);fclose(f);fb=old_fb;free(pixels);
#undef NCHECK
 return 1;
}
