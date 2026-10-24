/* Native 480x272, 30fps MJPEG stream in EBOOT.PBP's DATA.PSAR. Only one
 * decoded frame and one compressed frame are resident. No game/save writes. */
#include <pspjpeg.h>
enum { INTRO_FILM_LIMIT=1800,INTRO_FILM_JPEG_LIMIT=131072 };
static FILE *intro_movie_file;
static uint32_t intro_movie_index[INTRO_FILM_LIMIT+1];
static unsigned char *intro_movie_jpeg;
static unsigned *intro_movie_pixels;
static uint32_t intro_movie_count,intro_movie_loop,intro_movie_max,intro_movie_base;
static int intro_movie_attempted,intro_movie_context,intro_movie_library,intro_movie_module;
static int intro_movie_frame=-1,intro_movie_error,intro_movie_decodes;
static unsigned intro_movie_memory;
static void intro_movie_close(void){
 if(intro_movie_file)fclose(intro_movie_file);intro_movie_file=NULL;
 if(intro_movie_context)sceJpegDeleteMJpeg();intro_movie_context=0;
 if(intro_movie_library)sceJpegFinishMJpeg();intro_movie_library=0;
 if(intro_movie_module)sceUtilityUnloadModule(PSP_MODULE_AV_MPEGBASE);intro_movie_module=0;
 free(intro_movie_jpeg);free(intro_movie_pixels);intro_movie_jpeg=NULL;intro_movie_pixels=NULL;
 intro_movie_memory=0;intro_movie_frame=-1;
}
static int intro_movie_fail(int error){intro_movie_error=error;intro_movie_close();return 0;}
static uint32_t intro_movie_fnv(const void *data,size_t length){
 const unsigned char *p=data;uint32_t h=2166136261u;while(length--)h=(h^*p++)*16777619u;return h;
}
static int intro_movie_header(const uint32_t *h,long remaining){
 return h[0]==1&&h[1]==W&&h[2]==H&&h[3]==30&&h[4]>=60&&h[4]<=INTRO_FILM_LIMIT&&h[5]<h[4]&&h[6]>=4&&h[6]<=INTRO_FILM_JPEG_LIMIT&&remaining>40+(long)(h[4]+1)*4;
}
static int intro_movie_open(void){
 if(intro_movie_attempted)return intro_movie_pixels!=NULL;
 intro_movie_attempted=1;intro_movie_error=0;
 intro_movie_file=fopen("EBOOT.PBP","rb");
 if(!intro_movie_file)return intro_movie_fail(-1);
 uint32_t pbp[10];
 if(fread(pbp,1,sizeof(pbp),intro_movie_file)!=sizeof(pbp)||pbp[0]!=0x50425000u||pbp[9]<40)return intro_movie_fail(-2);
 if(fseek(intro_movie_file,0,SEEK_END))return intro_movie_fail(-3);
 long length=ftell(intro_movie_file);intro_movie_base=pbp[9];
 if(length<0||length>(128L<<20)||intro_movie_base>(uint32_t)length-40||fseek(intro_movie_file,intro_movie_base,SEEK_SET))return intro_movie_fail(-4);
 char magic[8];uint32_t h[8];
 if(fread(magic,1,8,intro_movie_file)!=8||memcmp(magic,"ENINTRO1",8)||fread(h,4,8,intro_movie_file)!=8||!intro_movie_header(h,length-intro_movie_base))return intro_movie_fail(-5);
 intro_movie_count=h[4];intro_movie_loop=h[5];intro_movie_max=h[6];
 size_t table=(intro_movie_count+1)*4;
 if(fread(intro_movie_index,1,table,intro_movie_file)!=table||intro_movie_fnv(intro_movie_index,table)!=h[7])return intro_movie_fail(-6);
 if(intro_movie_index[0]!=40+table||intro_movie_index[intro_movie_count]!=(uint32_t)length-intro_movie_base)return intro_movie_fail(-7);
 for(uint32_t i=0;i<intro_movie_count;i++){
  uint32_t begin=intro_movie_index[i],end=intro_movie_index[i+1];
  if(end<=begin||end-begin>intro_movie_max||end>(uint32_t)length-intro_movie_base)return intro_movie_fail(-8);
 }
 unsigned capacity=(intro_movie_max+63u)&~63u;
 intro_movie_jpeg=memalign(64,capacity);intro_movie_pixels=memalign(64,W*H*4);
 if(!intro_movie_jpeg||!intro_movie_pixels)return intro_movie_fail(-9);
 intro_movie_memory=capacity+W*H*4;
 /* Audio may already own AVCODEC; do not unload its shared module. */
 sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC);
 int result=sceUtilityLoadModule(PSP_MODULE_AV_MPEGBASE);intro_movie_module=result>=0;
 result=sceJpegInitMJpeg();if(result<0)return intro_movie_fail(result);intro_movie_library=1;
 result=sceJpegCreateMJpeg(W,H);if(result<0)return intro_movie_fail(result);intro_movie_context=1;
 return 1;
}
static unsigned intro_movie_at(float seconds){
 unsigned index=seconds>0?(unsigned)(seconds*30.f):0;
 if(index>=intro_movie_count)index=intro_movie_loop+(index-intro_movie_loop)%(intro_movie_count-intro_movie_loop);
 return index;
}
static int intro_movie_draw(float seconds){
 if(!intro_movie_open())return 0;
 unsigned index=intro_movie_at(seconds);
 if((int)index!=intro_movie_frame){
  unsigned size=intro_movie_index[index+1]-intro_movie_index[index];
  if(fseek(intro_movie_file,intro_movie_base+intro_movie_index[index],SEEK_SET)||fread(intro_movie_jpeg,1,size,intro_movie_file)!=size)return intro_movie_fail(-10);
  if(intro_movie_jpeg[0]!=255||intro_movie_jpeg[1]!=216||intro_movie_jpeg[size-2]!=255||intro_movie_jpeg[size-1]!=217)return intro_movie_fail(-11);
  sceKernelDcacheWritebackInvalidateRange(intro_movie_jpeg,(size+63u)&~63u);
  int result=sceJpegDecodeMJpeg(intro_movie_jpeg,size,(unsigned char*)intro_movie_pixels,0);
  if(result!=((W<<16)|H))return intro_movie_fail(result<0?result:-12);
  sceKernelDcacheInvalidateRange(intro_movie_pixels,W*H*4);
  intro_movie_frame=(int)index;intro_movie_decodes++;
 }
 for(int y=0;y<H;y++)memcpy(fb+y*STRIDE,intro_movie_pixels+y*W,W*4);
 return 1;
}
