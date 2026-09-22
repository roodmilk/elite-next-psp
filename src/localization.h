/* Small, ASCII-safe localization layer for the fixed 8x8 PSP font. */
enum { LOCALE_ENGLISH, LOCALE_FRENCH, LOCALE_GERMAN, LOCALE_COUNT };
enum { LOC_OUTFITTING, LOC_IN_STOCK, LOC_LOADOUT, LOC_MISSILES, LOC_HOLD, LOC_BALANCE, LOC_NO_STOCK, LOC_COUNT };
static int locale_current=LOCALE_ENGLISH;
static char locale_override[LOC_COUNT][32];
static void locale_set_code(const char *code){
 if(!code)return;
 if((code[0]=='f'||code[0]=='F')&&(code[1]=='r'||code[1]=='R'))locale_current=LOCALE_FRENCH;
 else if((code[0]=='d'||code[0]=='D')&&(code[1]=='e'||code[1]=='E'))locale_current=LOCALE_GERMAN;
 else locale_current=LOCALE_ENGLISH;
}
static void locale_load(const char *path){
 FILE *f=fopen(path,"r");char code[8]={0};
 if(f){if(fgets(code,sizeof(code),f))locale_set_code(code);fclose(f);}
}
/* Optional bounded content archive. Format: ENPK, version 1, count, then
 * key/length/ASCII bytes entries. Invalid archives are ignored atomically. */
static int locale_pack_load(const char *path){
 FILE *f=fopen(path,"rb");if(!f)return 0;unsigned char head[6];if(fread(head,1,6,f)!=6||memcmp(head,"ENPK",4)||head[4]!=1||head[5]>LOC_COUNT){fclose(f);return 0;}
 char next[LOC_COUNT][32]={{0}};for(unsigned i=0;i<head[5];i++){unsigned char key=0,len=0;if(fread(&key,1,1,f)!=1||fread(&len,1,1,f)!=1||key>=LOC_COUNT||len==0||len>=32){fclose(f);return 0;}for(unsigned j=0;j<len;j++){unsigned char c=0;if(fread(&c,1,1,f)!=1||c<32||c>126){fclose(f);return 0;}next[key][j]=(char)c;}next[key][len]=0;}
 fclose(f);memcpy(locale_override,next,sizeof(next));return head[5]>0;
}
static const char *locale_text(int key);
static void locale_pack_tests(void){
 FILE *f=fopen("content.pack","wb");if(f){unsigned char h[6]={'E','N','P','K',1,1},k=LOC_OUTFITTING,l=3;fwrite(h,1,6,f);fwrite(&k,1,1,f);fwrite(&l,1,1,f);fwrite("FIT",1,3,f);fclose(f);}int ok=locale_pack_load("content.pack");remove("content.pack");FILE *bad=fopen("content.pack","wb");if(bad){fwrite("BAD!",1,4,bad);fclose(bad);}int rejected=!locale_pack_load("content.pack");remove("content.pack");FILE *out=fopen("localization-check.txt","w");if(out){fprintf(out,"RESULT %d failures\n",(ok&&rejected&&strcmp(locale_text(LOC_OUTFITTING),"FIT")==0)?0:1);fclose(out);}memset(locale_override,0,sizeof(locale_override));
}
static const char *locale_text(int key){
 static const char *en[LOC_COUNT]={"OUTFITTING","IN STOCK","SHIP LOADOUT / HOLD","MISSILES","HOLD","BALANCE","No stock today."};
 static const char *fr[LOC_COUNT]={"EQUIPEMENT","EN STOCK","CHARGE / CARGO","MISSILES","SOUTE","SOLDE","Aucun stock."};
 static const char *de[LOC_COUNT]={"AUSRUESTUNG","AUF LAGER","SCHIFF / FRACHT","RAKETEN","LADERAUM","KONTO","Kein Vorrat."};
 if(key<0||key>=LOC_COUNT)return "?";
 if(locale_override[key][0])return locale_override[key];
 return locale_current==LOCALE_FRENCH?fr[key]:locale_current==LOCALE_GERMAN?de[key]:en[key];
}
