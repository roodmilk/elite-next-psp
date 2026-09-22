/* Regression cases run in the PSP smoke harness, using disposable paths. */
static int fail_save_move(const char *from,const char *to){
 if(strstr(from,".tmp"))return -1;
 return rename(from,to);
}
static int fail_all_moves(const char *from,const char *to){(void)from;(void)to;return -1;}
static void reliability_tests(FILE *f,int *failures){
 int fails=0;Game g,loaded;const char *path="test-recovery.sav";
 remove(path);remove("test-recovery.sav.bak");remove("test-recovery.sav.tmp");
 game_init(&g);g.credits=1234;
 CHECK(save_game(&g,path),"recovery: first save validates before commit");
 g.credits=2345;CHECK(save_game(&g,path),"recovery: replacement keeps previous save");
 CHECK(load_game_file(&loaded,"test-recovery.sav.bak")&&loaded.credits==1234,"recovery: backup contains previous commander");
 FILE *bad=fopen(path,"wb");if(bad){fputs("broken",bad);fclose(bad);}
 CHECK(load_game(&loaded,path)&&loaded.credits==1234,"recovery: corrupt primary falls back to backup");
 remove(path);CHECK(load_game(&loaded,path)&&loaded.credits==1234,"recovery: missing primary falls back to backup");
 CHECK(save_game(&g,path),"recovery: saving after fallback preserves recovery chain");
 /* Prepare a fully valid pending file, then fail only the commit rename. */
 g.credits=3456;CHECK(save_game(&g,"test-recovery.sav.tmp"),"recovery: prepare pending commander");
 CHECK(!save_commit(path,"test-recovery.sav.tmp","test-recovery.sav.bak",fail_save_move),"recovery: failed commit reports failure");
 CHECK(load_game(&loaded,path)&&loaded.credits==2345,"recovery: failed commit rolls back previous commander");
 CHECK(!save_commit(path,"test-recovery.sav.tmp","test-recovery.sav.bak",fail_all_moves)&&load_game(&loaded,path)&&loaded.credits==2345,"recovery: failed backup rename leaves primary intact");
 remove(path);remove("test-recovery.sav.bak");
 CHECK(!load_game(&loaded,path),"recovery: uncommitted temporary file is never promoted");
 remove("test-recovery.sav.tmp");remove("test-recovery.sav.tmp.bak");
 char longpath[300];memset(longpath,'x',sizeof(longpath)-1);longpath[sizeof(longpath)-1]=0;
 CHECK(!save_game(&g,longpath),"recovery: oversized path rejected without truncation");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,3350};g.speed=100;
 world_collision(&g,(Vec3){0,0,3360});
 CHECK(g.dead&&!g.dock_stage,"docking: movement from inside cannot masquerade as entry");
 game_init(&g);launch(&g);g.pos=(Vec3){53,0,3400};g.speed=100;
 world_collision(&g,(Vec3){53,0,3300});
 CHECK(g.dead,"docking: ship clearance catches entrance edge");
 game_init(&g);launch(&g);g.pos=(Vec3){0,0,3500};
 CHECK(!dock(&g)&&!g.dock_stage,"docking: guidance refuses a position inside solid hull");
 for(int side=-1;side<=1;side+=2){
  game_init(&g);launch(&g);g.pos=(Vec3){side*500.f,200,3600};g.time=48;
  int safe=dock(&g);for(int i=0;i<1600&&!g.docked&&!g.dead;i++)game_tick(&g,1.f/60,0,0,0,0);
  CHECK(safe&&g.docked&&!g.dead,"docking: rotated side approach crosses shared aperture safely");
 }
 *failures+=fails;
}
