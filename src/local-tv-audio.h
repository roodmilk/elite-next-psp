/* Render thread publishes one packed syllable; worker owns every oscillator.
 * Independent of radio tuning, no files, allocation or game RNG. */
static volatile int audio_tv_on=0,audio_tv_code=0;
typedef struct { unsigned phase,formant; int envelope,age,filter; } LocalTVVoice;
static int local_tv_voice_sample(LocalTVVoice *v,int code){
 int letter=code&255,story=(code>>8)&1;
 int voiced=(letter>='A'&&letter<='Z')||(letter>='a'&&letter<='z');
 int period=story?8820:6615; /* slower, softer storyteller */
 v->age=(v->age+1)%period;
 int pulse=v->age,gate=period*3/4;
 int target=voiced&&pulse<gate?640:0;
 if(v->envelope<target){v->envelope+=8;if(v->envelope>target)v->envelope=target;}
 if(v->envelope>target){v->envelope-=8;if(v->envelope<target)v->envelope=target;}
 int pitch=(story?160:205)+(letter%7)*13;
 v->phase+=(unsigned)pitch*97391u;
 v->formant+=(unsigned)(510+(letter%5)*115)*97391u;
 int a=(int)(v->phase>>22),b=(int)(v->formant>>22);
 a=a<512?a-256:768-a;b=b<512?b-256:768-b;
 int sample=(a*3+b)*v->envelope/640;
 v->filter+=(sample-v->filter)/5;
 if(!v->envelope&&v->filter>-5&&v->filter<5)v->filter=0;
 return v->filter; /* bounded to +/-1024 before the existing SFX master */
}
