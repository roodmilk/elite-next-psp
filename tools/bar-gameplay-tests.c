/* Standalone pure-mechanics tests. BAR_TEST_PSP builds an isolated diagnostic,
 * not the shipping game. Snapshot-copy tests do NOT establish disk durability. */
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "../src/bar-gameplay.h"
#ifdef BAR_TEST_PSP
#include <pspkernel.h>
PSP_MODULE_INFO("Bar mechanics diagnostic",0,1,0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
#endif
static int same_dice(BarDiceState a,BarDiceState b){return a.round_id==b.round_id&&a.rng==b.rng&&a.choice==b.choice&&a.stake==b.stake&&a.die1==b.die1&&a.die2==b.die2;}
static int tests(FILE *f){
 int failed=0,checks=0;
#define CHECK(c,n) do{int ok=(c);checks++;if(!ok){fprintf(f,"FAIL %s (line %d)\n",n,__LINE__);failed++;}}while(0)
 const BarJobSpec spec={1,1,39,0,1,39,1,1,1000};
 const BarVisit visit={1,39,0,1,1,1,1,1};
 const BarRecovery recovery={1,39,1,1,1,1,1,1,400.f,2.f};
 BarJobState job={BAR_JOB_NONE};BarResult r;int cash=1000;
 CHECK(bar_job_valid(&job)&&!bar_job_occupies_slot(&job),"new offer uses no mission slot");
 CHECK(bar_job_recover(&job,&spec,&recovery).status==BAR_ERR_STATE,"cannot recover before acceptance");
 CHECK(bar_job_hand_in(&job,&spec,&visit,&cash).status==BAR_ERR_STATE&&cash==1000,"cannot claim before recovery");
 CHECK(bar_job_accept(&job,&spec,&visit,5).status==BAR_ERR_CAPACITY&&job.phase==BAR_JOB_NONE,"five ordinary jobs block bar acceptance");
 for(int n=-1;n<7;n++)CHECK(bar_mission_capacity_available(&job,n)==(n>=0&&n<5),"offer capacity rejects invalid or full ordinary count");
 for(int k=0;k<8;k++){
  BarVisit wrong=visit;
  if(k==0)wrong.bar_id++;
  if(k==1)wrong.system++;
  if(k==2)wrong.hub++;
  if(k==3)wrong.recipient_id++;
  if(k==4)wrong.docked=0;
  if(k==5)wrong.alive=0;
  if(k==6)wrong.can_land=0;
  if(k==7)wrong.site_enabled=0;
  r=bar_job_accept(&job,&spec,&wrong,0);CHECK(r.status<0&&!r.event&&job.phase==BAR_JOB_NONE,"acceptance guards exact bar/live/access without mutation");
 }
 r=bar_job_accept(&job,&spec,&visit,4);CHECK(r.status==BAR_APPLIED&&r.event==BAR_EVENT_JOB_ACCEPTED&&job.phase==BAR_JOB_ACCEPTED,"four ordinary jobs allow one bar job");
 CHECK(!bar_mission_capacity_available(&job,4)&&bar_mission_capacity_available(&job,3),"active bar occupies fifth slot for ordinary acceptance");
 CHECK(bar_job_accept(&job,&spec,&visit,0).status<0,"repeat acceptance cannot duplicate state");
 r=bar_job_abandon(&job);CHECK(r.status==BAR_APPLIED&&r.event==BAR_EVENT_JOB_ABANDONED&&job.phase==BAR_JOB_NONE,"prepickup abandon reopens offer without a payment event");
 bar_job_accept(&job,&spec,&visit,0);
 for(int k=0;k<8;k++){
  BarRecovery wrong=recovery;
  if(k==0)wrong.mission_id++;
  if(k==1)wrong.system++;
  if(k==2)wrong.body++;
  if(k==3)wrong.site_id++;
  if(k==4)wrong.alive=0;
  if(k==5)wrong.on_foot=0;
  if(k==6)wrong.grounded=0;
  if(k==7)wrong.pressed_recover=0;
  r=bar_job_recover(&job,&spec,&wrong);CHECK(r.status<0&&!r.event&&job.phase==BAR_JOB_ACCEPTED,"recovery requires exact objective and actual live grounded pressed-edge EVA");
 }
 const float distances[]={19.99f,20.f,20.01f};
 for(int i=0;i<3;i++){BarJobState s={BAR_JOB_ACCEPTED};BarRecovery e=recovery;e.planar_distance_squared=distances[i]*distances[i];r=bar_job_recover(&s,&spec,&e);CHECK((r.status==BAR_APPLIED)==(i<2),"planar radius19.99/20/20.01 metres");}
 const float heights[]={-2.01f,-2.f,0.f,2.f,2.01f};
 for(int i=0;i<5;i++){BarJobState s={BAR_JOB_ACCEPTED};BarRecovery e=recovery;e.eye_ground_error=heights[i];r=bar_job_recover(&s,&spec,&e);CHECK((r.status==BAR_APPLIED)==(i>0&&i<4),"grounded eye-height tolerance inclusive plus/minus2m");}
 const float invalid[]={NAN,INFINITY,-INFINITY,-1.f};
 for(int i=0;i<4;i++){BarRecovery e=recovery;e.planar_distance_squared=invalid[i];r=bar_job_recover(&job,&spec,&e);CHECK(r.status<0&&!r.event&&job.phase==BAR_JOB_ACCEPTED,"invalid squared distance cannot recover");if(i<3){e=recovery;e.eye_ground_error=invalid[i];CHECK(bar_job_recover(&job,&spec,&e).status<0,"nonfinite eye height rejected");}}
 r=bar_job_recover(&job,&spec,&recovery);CHECK(r.status==BAR_APPLIED&&r.event==BAR_EVENT_JOB_RECOVERED&&cash==1000,"recovery records evidence with no money");
 r=bar_job_recover(&job,&spec,&recovery);CHECK(r.status==BAR_NO_CHANGE&&!r.event,"repeat recovery emits no new event");
 CHECK(bar_job_abandon(&job).status==BAR_ERR_EVIDENCE&&job.phase==BAR_JOB_RECOVERED,"recovered evidence cannot be abandoned away");
 for(int k=0;k<6;k++){BarVisit wrong=visit;
  if(k==0)wrong.bar_id++;
  if(k==1)wrong.system++;
  if(k==2)wrong.hub++;
  if(k==3)wrong.recipient_id++;
  if(k==4)wrong.docked=0;
  if(k==5)wrong.alive=0;
  r=bar_job_hand_in(&job,&spec,&wrong,&cash);CHECK(r.status<0&&!r.event&&cash==1000&&job.phase==BAR_JOB_RECOVERED,"only live exact Pell primary-bar visit can claim");}
 cash=BAR_CREDIT_MAX-999;r=bar_job_hand_in(&job,&spec,&visit,&cash);CHECK(r.status==BAR_ERR_CREDITS&&cash==BAR_CREDIT_MAX-999&&job.phase==BAR_JOB_RECOVERED,"reward overflow keeps evidence and money unchanged");
 cash=BAR_CREDIT_MAX-1000;r=bar_job_hand_in(&job,&spec,&visit,&cash);CHECK(r.status==BAR_APPLIED&&r.event==BAR_EVENT_JOB_PAID&&cash==BAR_CREDIT_MAX&&job.phase==BAR_JOB_PAID,"exact cap hand-in pays once");
 r=bar_job_hand_in(&job,&spec,&visit,&cash);CHECK(r.status==BAR_NO_CHANGE&&!r.event&&cash==BAR_CREDIT_MAX,"repeat hand-in does not pay or cue");
 CHECK(!bar_job_occupies_slot(&job)&&bar_job_accept(&job,&spec,&visit,0).status<0&&bar_job_abandon(&job).status<0,"Paid frees slot but never reoffers");
 BarJobState invalid_job={4};CHECK(!bar_mission_capacity_available(&invalid_job,0)&&bar_job_accept(&invalid_job,&spec,&visit,0).status<0,"invalid stored phase fails closed");
 /* Tentative snapshots can be discarded; actual save failure remains Systems' test. */
 {BarJobState live={BAR_JOB_RECOVERED},pending=live;int live_cash=1000,pending_cash=live_cash;bar_job_hand_in(&pending,&spec,&visit,&pending_cash);CHECK(live.phase==BAR_JOB_RECOVERED&&live_cash==1000&&pending.phase==BAR_JOB_PAID&&pending_cash==2000,"isolated pending hand-in does not mutate live snapshot");}
 const uint32_t stakes[]={10,50,100};
 for(uint32_t choice=BAR_LOW;choice<=BAR_HIGH;choice++)for(int st=0;st<3;st++){
  int wins=0,losses=0,draws=0,total_net=0;
  for(uint32_t d1=1;d1<=6;d1++)for(uint32_t d2=1;d2<=6;d2++){
   int back=bar_dice_return(choice,stakes[st],d1,d2),net=back-(int)stakes[st];
   int sum=(int)(d1+d2),expected=sum==7?(int)stakes[st]:((choice==BAR_LOW&&sum<=6)||(choice==BAR_HIGH&&sum>=8))?2*(int)stakes[st]:0;
   CHECK(back==expected,"every ordered pair returns correct inclusive stake");wins+=net>0;losses+=net<0;draws+=net==0;total_net+=net;
  }
  CHECK(wins==15&&losses==15&&draws==6&&total_net==0,"fair36-pair rules15wins15losses6refunds at every stake/side");
 }
 CHECK(bar_dice_return(2,10,1,1)<0&&bar_dice_return(0,11,1,1)<0&&bar_dice_return(0,10,0,1)<0&&bar_dice_return(0,10,1,7)<0,"invalid rule inputs rejected");
 {BarDiceState d={0,12345,0,0,0,0},old=d;cash=9;r=bar_dice_confirm(&d,&cash,1,BAR_LOW,10);CHECK(r.status==BAR_ERR_CREDITS&&!r.event&&cash==9&&same_dice(d,old),"poor player cannot debit or roll");
  cash=BAR_CREDIT_MAX;CHECK(bar_dice_confirm(&d,&cash,1,BAR_LOW,10).status==BAR_ERR_CREDITS&&same_dice(d,old),"possible win overflow rejected before rolling");
  cash=1000;CHECK(bar_dice_confirm(&d,&cash,1,2,10).status<0&&same_dice(d,old)&&cash==1000,"invalid choice does not consume RNG");
  CHECK(bar_dice_confirm(&d,&cash,1,BAR_LOW,0).status<0&&same_dice(d,old)&&cash==1000,"invalid stake does not consume RNG");
  CHECK(bar_dice_confirm(&d,&cash,0,BAR_LOW,10).status==BAR_ERR_TOKEN&&bar_dice_confirm(&d,&cash,2,BAR_LOW,10).status==BAR_ERR_TOKEN&&same_dice(d,old),"wrong confirmation token cannot roll");
  r=bar_dice_confirm(&d,&cash,1,BAR_LOW,10);CHECK(r.status==BAR_APPLIED&&r.event==BAR_EVENT_DICE_SETTLED&&d.round_id==1&&cash==990+bar_dice_return(0,10,d.die1,d.die2),"confirm settles final balance once before reveal");
  old=d;int settled=cash;r=bar_dice_confirm(&d,&cash,1,BAR_HIGH,100);CHECK(r.status==BAR_ERR_TOKEN&&!r.event&&same_dice(d,old)&&cash==settled,"repeat confirm cannot reroll or replay payout");
  BarDiceState restored=d;int restored_cash=cash;r=bar_dice_confirm(&restored,&restored_cash,1,BAR_LOW,10);CHECK(r.status==BAR_ERR_TOKEN&&!r.event&&same_dice(restored,d)&&restored_cash==cash,"restored committed snapshot refuses old confirmation");
  d.round_id=UINT32_MAX;old=d;CHECK(bar_dice_confirm(&d,&cash,0,BAR_LOW,10).status==BAR_ERR_TOKEN&&same_dice(d,old),"round token never wraps");}
 {BarDiceState a={0,0,0,0,0,0},b=a;int ca=500000,cb=ca;for(uint32_t n=1;n<=10000;n++){
   int before=ca;uint32_t stake=stakes[n%3],choice=n%2;r=bar_dice_confirm(&a,&ca,n,choice,stake);BarResult rb=bar_dice_confirm(&b,&cb,n,choice,stake);
   CHECK(r.status==BAR_APPLIED&&rb.status==BAR_APPLIED&&same_dice(a,b)&&ca==cb&&a.die1>=1&&a.die1<=6&&a.die2>=1&&a.die2<=6,"dedicated RNG reproducible and dice in range");
   CHECK(ca==before-(int)stake+bar_dice_return(choice,stake,a.die1,a.die2),"each confirmed balance equals net outcome");
  }}
 {BarDiceState live={0,77,0,0,0,0},pending=live;int live_cash=1000,pending_cash=live_cash;bar_dice_confirm(&pending,&pending_cash,1,BAR_HIGH,100);CHECK(live.round_id==0&&live.rng==77&&live_cash==1000&&pending.round_id==1,"discarded dice transaction leaves live RNG and money untouched");}
 fprintf(f,"CHECKS %d\nRESULT %d failures\n",checks,failed);return failed;
#undef CHECK
}
int main(void){
#ifdef BAR_TEST_PSP
 FILE *f=fopen("bar-gameplay-check.txt","w");if(f){tests(f);fclose(f);}sceKernelExitGame();return 0;
#else
 return tests(stdout)?1:0;
#endif
}
