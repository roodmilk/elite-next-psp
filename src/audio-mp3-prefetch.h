/* Two bounded compressed chunks. Only the mixer touches decoder/ring state;
 * the PSP I/O worker fills the inactive chunk without blocking PCM mixing.
 * 256 KiB on PSP-3000; older memory profiles keep their existing stream path. */
#include <pspiofilemgr.h>
#ifdef ELITE_PSP3000
#define MUSIC_READ_CHUNK (128*1024)
static unsigned char music_read_cache[2][MUSIC_READ_CHUNK] __attribute__((aligned(64)));
static int music_read_fd=-1,music_read_pending=-1,music_read_offset[2],music_read_length[2],music_read_next,music_read_end;
static unsigned music_read_waits,music_read_errors,music_read_chunks;
static unsigned music_read_wait_worst_us;
#ifdef TRAVEL_POLISH_REVIEW
static int music_prefetch_disabled;
#endif
static int music_read_complete(void){
 if(music_read_pending<0)return 1;
 SceInt64 result=0;int index=music_read_pending,status=sceIoGetAsyncStat(music_read_fd,1,&result);
 if(status>0){uint64_t start=sceKernelGetSystemTimeWide();music_read_waits++;status=sceIoWaitAsync(music_read_fd,&result);unsigned us=sceKernelGetSystemTimeWide()-start;if(us>music_read_wait_worst_us)music_read_wait_worst_us=us;}
 music_read_pending=-1;
 if(status<0||result<0){music_read_errors++;music_read_length[index]=0;return 0;}
 music_read_length[index]=(int)result;music_read_next=music_read_offset[index]+(int)result;music_read_chunks++;return 1;
}
static void music_read_close(void){
 if(music_read_fd>=0){music_read_complete();sceIoClose(music_read_fd);}
 music_read_fd=music_read_pending=-1;music_read_length[0]=music_read_length[1]=0;
}
static int music_read_queue(int index){
 if(music_read_pending>=0&&!music_read_complete())return 0;
 if(music_read_next>=music_read_end){music_read_length[index]=0;return 1;}
 music_read_offset[index]=music_read_next;music_read_length[index]=0;
 int n=music_read_end-music_read_next;if(n>MUSIC_READ_CHUNK)n=MUSIC_READ_CHUNK;
 if(sceIoReadAsync(music_read_fd,music_read_cache[index],n)<0){music_read_errors++;return 0;}
 music_read_pending=index;return 1;
}
static int music_read_prime(int pos){
 if(music_read_pending>=0&&!music_read_complete())return 0;
 if(sceIoLseek32(music_read_fd,pos,PSP_SEEK_SET)<0)return 0;
 int n=music_read_end-pos;if(n>MUSIC_READ_CHUNK)n=MUSIC_READ_CHUNK;if(n<=0)return 0;
 int got=sceIoRead(music_read_fd,music_read_cache[0],n);if(got<=0){music_read_errors++;return 0;}
 music_read_offset[0]=pos;music_read_length[0]=got;music_read_length[1]=0;music_read_next=pos+got;music_read_chunks++;
 return music_read_queue(1);
}
static int music_read_open(const char*path,int pos,int end){
 music_read_close();
#ifdef TRAVEL_POLISH_REVIEW
 if(music_prefetch_disabled)return 0;
#endif
 music_read_fd=sceIoOpen(path,PSP_O_RDONLY,0);if(music_read_fd<0)return 0;
 sceIoChangeAsyncPriority(music_read_fd,0x1a);music_read_end=end;
 if(!music_read_prime(pos)){music_read_close();return 0;}return 1;
}
static int music_read_copy(unsigned char *dst,int size,int pos){
 int done=0;
 while(done<size&&pos<music_read_end){
  int index=-1;
  for(int i=0;i<2;i++)if(pos>=music_read_offset[i]&&pos<music_read_offset[i]+music_read_length[i]){index=i;break;}
  if(index<0&&music_read_pending>=0){if(!music_read_complete())break;continue;}
  if(index<0){if(!music_read_prime(pos))break;continue;}
  int available=music_read_offset[index]+music_read_length[index]-pos,n=size-done;if(n>available)n=available;
  memcpy(dst+done,music_read_cache[index]+pos-music_read_offset[index],n);done+=n;pos+=n;
  if(n==available&&pos<music_read_end&&!music_read_queue(index))break;
 }
 return done;
}
#else
static int music_read_fd=-1;
static void music_read_close(void){}
static int music_read_open(const char*path,int pos,int end){(void)path;(void)pos;(void)end;return 0;}
static int music_read_copy(unsigned char*dst,int size,int pos){(void)dst;(void)size;(void)pos;return 0;}
#endif
