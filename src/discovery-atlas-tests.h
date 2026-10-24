{
 TEST_INIT();page=CODEX;row=0;
 INPUT_CHECK(codex_rows()==1,"atlas: begins with current visited system only");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 int system_rows=codex_rows(),signal_row=codex_system_signal_row(game.system);
 INPUT_CHECK(codex_scope==ATLAS_SYSTEM&&system_rows==system_body_slots(game.system)+2,"atlas: every charted world appears while surface facts remain gated");
 row=signal_row;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(codex_scope==ATLAS_SIGNALS,"atlas: honest signal coverage category opens");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(codex_scope==ATLAS_SYSTEM&&row==signal_row,"atlas: back restores parent selection");
 int first_planet=signal_row+1;row=first_planet;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(codex_scope==ATLAS_WORLD&&codex_rows()==1,"atlas: unlanded planet hides all surface collections");input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 game.landed_planets[game.system]=1;row=first_planet;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(codex_scope==ATLAS_WORLD&&codex_body==2&&codex_rows()==4,"atlas: landed planet opens category tiles");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(codex_scope==ATLAS_CATEGORY&&!atlas_entry_count(),"atlas: unscanned flora collection honestly empty");
 input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(codex_scope==ATLAS_CATEGORY,"atlas: empty collection cannot open a fabricated record");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(codex_scope==ATLAS_GALAXY&&!atlas_depth,"atlas: full back path returns to galaxy");
 int reward_sys=game.system,reward_cash=game.credits,record=atlas_progress_percent(reward_sys);
 INPUT_CHECK(record>=10&&record<100&&atlas_reward_available(reward_sys),"atlas: charted system earns a real first survey milestone");
 atlas_claim_reward(reward_sys);
 INPUT_CHECK(atlas_claimed_level(reward_sys)==1&&game.credits==reward_cash+900&&atlas_reward_open(),"atlas: Triangle milestone claim pays once and opens its reward card");
 preview_time=atlas_reward_started+1.3f;atlas_input(PSP_CTRL_CROSS);
 INPUT_CHECK(!atlas_reward_open()&&!atlas_reward_available(reward_sys),"atlas: reward card closes with X and claimed milestone cannot repeat");
 int total=0,consistent=1;
 for(int sys=0;sys<256;sys++){
  game.visited[sys>>3]|=1u<<(sys&7);game.landed_planets[sys]=15;
  for(int body=1;body<BODY_COUNT;body++){
   game.surface_progress[sys][body]=0xff00;
   codex_system=sys;codex_body=body+1;
   int sum=0;for(int kind=0;kind<3;kind++){atlas_category=kind;int n=atlas_entry_count();sum+=n;for(int j=0;j<n;j++){int id=atlas_entry_at(j);consistent&=id>=0&&field_species_kind(sys,body,id)==kind&&field_species_logged(&game,sys,body,id);}}
   total+=sum;consistent&=sum==LIFE_COUNT;
  }
 }
 INPUT_CHECK(consistent&&total==256*(BODY_COUNT-1)*LIFE_COUNT,"atlas: all 1024 worlds and 8192 records enumerate in their exact categories");
 atlas_reset();row=255;codex_screen();dump_native_bmp("atlas-galaxy.bmp");
 INPUT_CHECK(codex_rows()==256,"atlas: all 256 visited systems remain browseable");
 input(PSP_CTRL_RTRIGGER,0,.016f,0,0);INPUT_CHECK(row==255,"atlas: page forward clamps safely at last system");
 input(PSP_CTRL_LTRIGGER,0,.016f,0,0);INPUT_CHECK(row==248,"atlas: shoulder paging skips seven systems");
 row=0;input(PSP_CTRL_CROSS,0,.016f,0,0);codex_screen();dump_native_bmp("atlas-system.bmp");
 row=codex_system_signal_row(codex_system)+1;input(PSP_CTRL_CROSS,0,.016f,0,0);codex_screen();dump_native_bmp("atlas-world.bmp");
 for(int kind=0;kind<4;kind++){
  row=kind;if(kind==3)game.surface_progress[codex_system][codex_body-1]|=field_site_bit(2);
  input(PSP_CTRL_CROSS,0,.016f,0,0);codex_screen();
  char path[64];snprintf(path,sizeof(path),"atlas-category-%d.bmp",kind);dump_native_bmp(path);
  if(atlas_entry_count()){int id=atlas_entry_at(0);input(PSP_CTRL_CROSS,0,.016f,0,0);
   INPUT_CHECK(codex_scope==ATLAS_RECORD&&atlas_entry==id,"atlas: category opens exact stable record ID");
   codex_screen();snprintf(path,sizeof(path),"atlas-record-%d.bmp",kind);dump_native_bmp(path);
   input(PSP_CTRL_CIRCLE,0,.016f,0,0);
   INPUT_CHECK(codex_scope==ATLAS_CATEGORY&&row==0,"atlas: record back restores collection cursor");
  }
  input(PSP_CTRL_CIRCLE,0,.016f,0,0);
  INPUT_CHECK(codex_scope==ATLAS_WORLD&&row==kind,"atlas: collection back restores category tile");
 }
 TEST_INIT();page=HOME;
}
