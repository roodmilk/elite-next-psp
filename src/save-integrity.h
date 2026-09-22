/* V8 adds a little-endian campaign extension and CRC32 over the whole payload.
 * V9 appends saga progress. V10 appends the manual route goal.
 * The older V1-V7 core layout remains unchanged for migration, not portability.
 */
static int save_u32(FILE *f,uint32_t v){unsigned char b[4]={(unsigned char)v,(unsigned char)(v>>8),(unsigned char)(v>>16),(unsigned char)(v>>24)};return fwrite(b,1,4,f)==4;}
static int load_u32(FILE *f,uint32_t *v){unsigned char b[4];if(fread(b,1,4,f)!=4)return 0;*v=(uint32_t)b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);return 1;}
static int save_crc(FILE *f,long bytes,uint32_t *out){
 if(bytes<0||fseek(f,0,SEEK_SET))return 0;
 uint32_t crc=0xffffffffu;unsigned char buf[256];
 while(bytes){size_t n=bytes>256?256:(size_t)bytes;if(fread(buf,1,n,f)!=n)return 0;
  for(size_t i=0;i<n;i++){crc^=buf[i];for(int bit=0;bit<8;bit++)crc=(crc>>1)^((crc&1)?0xedb88320u:0);}
  bytes-=(long)n;
 }
 *out=crc^0xffffffffu;return 1;
}
static int save_seal(const char *path){
 FILE *f=fopen(path,"rb+");if(!f)return 0;uint32_t crc;int ok=!fseek(f,0,SEEK_END);long n=ok?ftell(f):-1;
 if(ok)ok=save_crc(f,n,&crc);
 if(ok)ok=!fseek(f,0,SEEK_END)&&save_u32(f,crc);
 if(fflush(f))ok=0;
 if(fclose(f))ok=0;
 return ok;
}
static int save_verify_end(FILE *f,int version){
 if(version>=8){long n=ftell(f);uint32_t stored,actual;
  if(n<0||!load_u32(f,&stored)||fgetc(f)!=EOF||ferror(f))return 0;
  return save_crc(f,n,&actual)&&actual==stored;
 }
 /* Exact lengths prevent a damaged V8 header downgrading itself to V7. */
 return fgetc(f)==EOF&&!ferror(f);
}
