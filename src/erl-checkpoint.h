/* A single immutable snapshot keeps Memory Stick writes off the flight thread.
 * Menus, results, suspend and shutdown join before any later save/load. */
static Game *erl_checkpoint_copy;
static SceUID erl_checkpoint_thread=-1;
static volatile int erl_checkpoint_done,erl_checkpoint_ok;
static char erl_checkpoint_path[64];
static unsigned erl_checkpoint_queued,erl_checkpoint_completed;
/* save_commit validates the entire V35 payload. load_game_file alone uses
 * over 100 KiB for Spacebook, terrain and equipment records; the old 64 KiB
 * worker stack overflowed into adjacent memory on physical PSP hardware. */
#define ERL_CHECKPOINT_STACK_BYTES (256*1024)
static volatile int erl_checkpoint_stack_free;
static volatile int erl_checkpoint_min_stack_free=ERL_CHECKPOINT_STACK_BYTES;
#ifdef ELITE_ENABLE_REVIEW
static int erl_checkpoint_test_delay;
#endif
static int erl_checkpoint_worker(SceSize size,void *arg){
 (void)size;(void)arg;
#ifdef ELITE_ENABLE_REVIEW
 if(erl_checkpoint_test_delay)sceKernelDelayThread(erl_checkpoint_test_delay);
#endif
 erl_checkpoint_ok=save_game(erl_checkpoint_copy,erl_checkpoint_path);
 erl_checkpoint_stack_free=sceKernelCheckThreadStack();
 if(erl_checkpoint_stack_free<erl_checkpoint_min_stack_free)erl_checkpoint_min_stack_free=erl_checkpoint_stack_free;
 erl_checkpoint_done=1;return 0;
}
static int erl_checkpoint_collect(int wait){
 if(erl_checkpoint_thread<0||(!wait&&!erl_checkpoint_done))return 0;
 sceKernelWaitThreadEnd(erl_checkpoint_thread,NULL);
 sceKernelDeleteThread(erl_checkpoint_thread);erl_checkpoint_thread=-1;
 free(erl_checkpoint_copy);erl_checkpoint_copy=NULL;
 int result=erl_checkpoint_ok?1:-1;if(erl_checkpoint_ok)erl_checkpoint_completed++;erl_checkpoint_done=0;return result;
}
static int erl_checkpoint_queue(void){
 if(erl_quick_race)return 0;
 if(erl_checkpoint_thread>=0)return 0;
 erl_checkpoint_copy=malloc(sizeof(Game));if(!erl_checkpoint_copy)return 0;
 *erl_checkpoint_copy=game;
 snprintf(erl_checkpoint_path,sizeof(erl_checkpoint_path),"%s",commander_save_path(&game));
 erl_checkpoint_done=erl_checkpoint_ok=0;erl_checkpoint_stack_free=-1;
 /* Below flight and audio priority; one writer and one bounded snapshot. */
 erl_checkpoint_thread=sceKernelCreateThread("ERL checkpoint",erl_checkpoint_worker,0x38,ERL_CHECKPOINT_STACK_BYTES,PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU,NULL);
 if(erl_checkpoint_thread<0||sceKernelStartThread(erl_checkpoint_thread,0,NULL)<0){
  if(erl_checkpoint_thread>=0)sceKernelDeleteThread(erl_checkpoint_thread);
  erl_checkpoint_thread=-1;free(erl_checkpoint_copy);erl_checkpoint_copy=NULL;return 0;
 }
 erl_checkpoint_queued++;return 1;
}
