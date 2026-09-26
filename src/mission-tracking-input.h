/* Keep the selected contract attached to its job when another job is removed. */
static void tracked_game_input(unsigned pressed,unsigned held,float dt,float ax,float ay){
 int before=tracked_mission,count=game.job_n,valid=before>=2&&before<2+count;
 Job job;if(valid)job=game.jobs[before-2];
 game_input(pressed,held,dt,ax,ay);
 if(valid&&before==tracked_mission&&game.job_n<count){
  int found=-1;
  for(int i=0;i<game.job_n;i++){const Job *j=&game.jobs[i];if(j->type==job.type&&j->origin==job.origin&&j->dest==job.dest&&j->reward==job.reward){found=i;break;}}
  tracked_mission=found>=0?found+2:0;
 }
}
