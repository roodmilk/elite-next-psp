/* Keep version 7 payloads compatible; commit only a fully validated file.
 * A .bak survives interrupted replacement. Never promote an uncommitted .tmp.
 */
static int load_game_file(Game *g,const char *path);
static int save_side_path(char *out,size_t size,const char *path,const char *suffix){
 int n=snprintf(out,size,"%s%s",path,suffix);
 return n>=0&&(size_t)n<size;
}
static int save_commit(const char *path,const char *tmp,const char *bak,
                       int (*move_file)(const char *,const char *)){
 if(!load_game_file(NULL,tmp))return 0;
 if(load_game_file(NULL,path)){
  /* Primary is still good if deleting an older backup fails. */
  FILE *f=fopen(bak,"rb");if(f){fclose(f);if(remove(bak)!=0)return 0;}
  if(move_file(path,bak)!=0)return 0;
 }else {
  /* Keep any recovery backup when replacing an invalid primary. */
  FILE *f=fopen(path,"rb");if(f){fclose(f);if(remove(path)!=0)return 0;}
 }
 if(move_file(tmp,path)!=0){
  /* Backup remains readable if rollback itself fails. */
  if(load_game_file(NULL,bak))move_file(bak,path);
  return 0;
 }
 return 1;
}
int load_game(Game *g,const char *path){
 if(load_game_file(g,path))return 1;
 char bak[256];
 if(save_side_path(bak,sizeof(bak),path,".bak")&&load_game_file(g,bak)){
  message(g,"Commander recovered from backup.");return 1;
 }
 message(g,"No valid commander or backup found.");return 0;
}
