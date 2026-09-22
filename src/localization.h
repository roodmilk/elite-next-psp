/* Small, ASCII-safe localization layer for the fixed 8x8 PSP font. */
enum { LOCALE_ENGLISH, LOCALE_FRENCH, LOCALE_GERMAN, LOCALE_COUNT };
enum { LOC_OUTFITTING, LOC_IN_STOCK, LOC_LOADOUT, LOC_MISSILES, LOC_HOLD, LOC_BALANCE, LOC_NO_STOCK, LOC_COUNT };
static int locale_current=LOCALE_ENGLISH;
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
static const char *locale_text(int key){
 static const char *en[LOC_COUNT]={"OUTFITTING","IN STOCK","SHIP LOADOUT / HOLD","MISSILES","HOLD","BALANCE","No stock today."};
 static const char *fr[LOC_COUNT]={"EQUIPEMENT","EN STOCK","CHARGE / CARGO","MISSILES","SOUTE","SOLDE","Aucun stock."};
 static const char *de[LOC_COUNT]={"AUSRUESTUNG","AUF LAGER","SCHIFF / FRACHT","RAKETEN","LADERAUM","KONTO","Kein Vorrat."};
 if(key<0||key>=LOC_COUNT)return "?";
 return locale_current==LOCALE_FRENCH?fr[key]:locale_current==LOCALE_GERMAN?de[key]:en[key];
}
