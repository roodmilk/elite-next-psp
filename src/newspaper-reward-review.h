/* Included inside newspaper_review: exercise the real top-level dispatcher. */
 game_init(&game);story_complete(&game);game.opening.stage=OP_FREE;game.system=7;game.docked=1;game.credits=100000;
 page=GALNET;galnet_tab=0;newspaper_reset();newspaper_load();memset(&newspaper_answers,0,sizeof(newspaper_answers));
 int lave_puzzle=-1;for(int r=0;r<newspaper_local_identity()->page_count;r++)if(newspaper_pages[newspaper_local_offset()+r].puzzle){lave_puzzle=r;break;}
 row=lave_puzzle;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 NCHECK(lave_puzzle>=0&&newspaper_mode==1&&page==GALNET,"actual X input opens Lave puzzle after opening story, instead of being swallowed");
 preview_time+=.2f;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("news-lave-puzzle-open.bmp");
 int lave_kind=newspaper_current()->puzzle,prize_before=game.credits;
 input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);
 NCHECK(!newspaper_reward_card&&game.credits==prize_before&&galnet_tab==0,"incomplete puzzle has no reward and captures section navigation through actual input");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);preview_time+=.2f;news_screen();
 NCHECK(page==GALNET&&!newspaper_mode&&game.opening.stage==OP_FREE,"actual Circle zooms back to the paper without leaving or advancing the story");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);preview_time+=.2f;
 if(lave_kind==1){
  for(int cell=0;cell<81;cell++)newspaper_answers.sudoku[cell]=newspaper_sudoku_solution(cell);
  newspaper_cell=2;newspaper_answers.sudoku[2]=0;newspaper_notes=0;newspaper_number=newspaper_sudoku_solution(2);newspaper_entry=0;
  input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 }else{
  for(int cell=0;cell<49;cell++)newspaper_answers.crossword[cell]=newspaper_cross_solution(cell);
  newspaper_cell=46;newspaper_answers.crossword[48]=0;newspaper_down=0;newspaper_entry=1;newspaper_slot=2;newspaper_letter='N'-'A';
 }
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 NCHECK(newspaper_complete(lave_kind)&&newspaper_reward_card&&game.credits==prize_before+10000&&(game.newspaper_rewards[7]&(1u<<(lave_kind-1))),"finishing Lave through actual input opens congratulations and pays exactly 1,000 units");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 NCHECK(newspaper_reward_card,"final answer press cannot immediately dismiss the completion animation");
 preview_time=newspaper_reward_started+.08f;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("news-prize-opening.bmp");
 preview_time=newspaper_reward_started+.45f;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("news-prize-congratulations.bmp");
 preview_time=newspaper_reward_started+1.1f;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("news-prize-continue.bmp");
 input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);NCHECK(newspaper_reward_card&&row==lave_puzzle,"completion card captures navigation without switching page");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);newspaper_claim_reward(lave_kind);
 NCHECK(!newspaper_reward_card&&game.credits==prize_before+10000,"dismissing or rechecking the completed puzzle cannot pay twice");
 newspaper_entry=0;input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);preview_time+=.2f;news_screen();input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 NCHECK(!newspaper_reward_card&&game.credits==prize_before+10000,"closing and reopening a solved puzzle preserves its claimed state");
 static Game prize_loaded;
 int prize_saved=save_game(&game,"puzzle-reward-v32.sav")&&load_game(&prize_loaded,"puzzle-reward-v32.sav");
 NCHECK(prize_saved&&prize_loaded.credits==game.credits&&!memcmp(prize_loaded.newspaper_rewards,game.newspaper_rewards,NEWSPAPER_REWARD_SAVE_BYTES),"V32 saves prize claims and the matching credit balance in one CRC-protected commander record");
 FILE *prize_file=fopen("puzzle-reward-v32.sav","rb");int migrated_prize=0,bad_prize=0;
 if(prize_file){fseek(prize_file,0,SEEK_END);long length=ftell(prize_file);rewind(prize_file);unsigned char *bytes=malloc(length);
  if(bytes&&fread(bytes,1,length,prize_file)==(size_t)length){
   /* Invalid claim masks must fail even with an otherwise valid checksum. */
   long reward_start=length-4-ERL_PROGRESS_BYTES-RA_SAVE_BYTES-EAX_SAVE_BYTES-DB_SAVE_BYTES-ERL_SAVE_BYTES-EXPEDITION_SAVE_BYTES-NEWSPAPER_REWARD_SAVE_BYTES;bytes[reward_start]=4;
   uint32_t crc=0xffffffffu;for(long i=0;i<length-4;i++){crc^=bytes[i];for(int b=0;b<8;b++)crc=(crc>>1)^((crc&1)?0xedb88320u:0);}crc^=0xffffffffu;
   for(int b=0;b<4;b++)bytes[length-4+b]=(unsigned char)(crc>>(b*8));
   FILE *bad=fopen("puzzle-reward-bad.sav","wb");if(bad){fwrite(bytes,1,length,bad);fclose(bad);bad_prize=!load_game(&prize_loaded,"puzzle-reward-bad.sav")&&prize_loaded.credits==game.credits;}
   long module_end=reward_start-RUMOUR_SAVE_BYTES;memmove(bytes+module_end-2,bytes+module_end,length-module_end);reward_start-=2;bytes[4]=31;bytes[5]=bytes[6]=bytes[7]=0;crc=0xffffffffu;
   for(long i=0;i<reward_start;i++){crc^=bytes[i];for(int b=0;b<8;b++)crc=(crc>>1)^((crc&1)?0xedb88320u:0);}crc^=0xffffffffu;
   for(int b=0;b<4;b++)bytes[reward_start+b]=(unsigned char)(crc>>(b*8));
   FILE *old=fopen("puzzle-reward-v31.sav","wb");if(old){fwrite(bytes,1,reward_start+4,old);fclose(old);migrated_prize=load_game(&prize_loaded,"puzzle-reward-v31.sav")&&prize_loaded.credits==game.credits;for(int i=0;i<NEWSPAPER_REWARD_SAVE_BYTES;i++)migrated_prize&=!prize_loaded.newspaper_rewards[i];}
  }free(bytes);fclose(prize_file);
 }
 NCHECK(migrated_prize,"V31 migration preserves credits and begins with no invented puzzle claims");
 NCHECK(bad_prize,"invalid V32 reward masks reject loading without changing the commander");
 for(int s=0;s<2;s++){
  game.system=s;newspaper_reset();newspaper_load();memset(&newspaper_answers,0,sizeof(newspaper_answers));int kind=s==0?1:2;prize_before=game.credits;
  if(kind==1){for(int i=0;i<81;i++)newspaper_answers.notes[i]=511;newspaper_claim_reward(kind);NCHECK(game.credits==prize_before,"Sudoku notes never count as a completed answer");for(int i=0;i<81;i++)newspaper_answers.sudoku[i]=newspaper_sudoku_solution(i);}
  else for(int i=0;i<49;i++)newspaper_answers.crossword[i]=newspaper_cross_solution(i);
  game.credits=100000000;newspaper_claim_reward(kind);NCHECK(!game.newspaper_rewards[s]&&!newspaper_reward_card,"a full credit balance cannot consume an unpaid puzzle prize");game.credits=prize_before;
  newspaper_claim_reward(kind);NCHECK(newspaper_reward_card&&game.credits==prize_before+10000,"other systems and both puzzle types receive their own exact 1,000-unit prize");
  newspaper_reward_card=0;if(kind==1)newspaper_answers.sudoku[2]=0;else newspaper_answers.crossword[0]=0;
  newspaper_claim_reward(kind);if(kind==1)newspaper_answers.sudoku[2]=newspaper_sudoku_solution(2);else newspaper_answers.crossword[0]=newspaper_cross_solution(0);newspaper_claim_reward(kind);
  NCHECK(!newspaper_reward_card&&game.credits==prize_before+10000,"erasing and completing a claimed puzzle again never repeats the payment");
 }
 game.opening.stage=0;page=GALNET;galnet_tab=0;game.system=0;row=newspaper_identity[0].page_count-2;newspaper_reset();input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 NCHECK(newspaper_mode==1,"X still opens puzzles for commanders without an opening story");
 newspaper_reset();game.opening.stage=OP_EARTH_NET;opening_world_apply(&game);page=GALNET;galnet_tab=0;row=0;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);NCHECK(game.opening.stage==OP_EARTH_NET&&page==GALNET&&!newspaper_mode,"Earth newspaper X does not prematurely advance onboarding");
 input(PSP_CTRL_CIRCLE,PSP_CTRL_CIRCLE,.016f,0,0);NCHECK(game.opening.stage!=OP_EARTH_NET,"leaving the Earth paper still completes the existing story beat");
 newspaper_reset();remove("puzzle-reward-v32.sav");remove("puzzle-reward-v31.sav");remove("puzzle-reward-bad.sav");
