/* Explicit little-endian V37 extension; never serialize struct padding. */
static void erl_progress_migrate(Game *g){
 /* Historical achievements are already earned. Do not announce them again,
    or invent recorded results/times when loading an older commander. */
 unsigned flags=(g->erl.prize_mask&15u)<<3;
 if(g->erl.wins)flags|=1;
 if(g->erl.wins>=10)flags|=2;
 if(g->erl.takedowns>=50)flags|=4;
 for(unsigned level=1;level<=g->erl.licence&&level<=4;level++)flags|=1u<<(7+level);
 g->erl_local.progress.milestones=flags;
}
static int erl_progress_stream(FILE *f,ErlProgress *p,int reading){
 for(int s=0;s<256;s++)for(int h=0;h<3;h++)if(!erl_word(f,&p->board_until[s][h],reading))return 0;
 uint32_t *words[]={&p->milestones,&p->rivals_beaten,&p->records,&p->latest_system,&p->latest_mode,&p->latest_objective,&p->latest_place,&p->latest_units,&p->latest_xp,&p->latest_sponsor};
 for(int i=0;i<10;i++)if(!erl_word(f,words[i],reading))return 0;
 if(p->milestones>4095||p->rivals_beaten>2305||p->latest_system>255||p->latest_mode>=RA_MODE_COUNT||p->latest_objective>=RA_OBJECTIVE_COUNT||p->latest_place>11||p->latest_units>1000000||p->latest_xp>10000||p->latest_sponsor>1)return 0;
 if(!erl_float(f,&p->latest_time,reading)||!isfinite(p->latest_time)||p->latest_time<0||p->latest_time>1000)return 0;
 for(int m=0;m<3;m++)for(int o=0;o<7;o++)if(!erl_float(f,&p->best[m][o],reading)||!isfinite(p->best[m][o])||p->best[m][o]<0||p->best[m][o]>1000)return 0;
 for(int s=0;s<256;s++)for(int c=0;c<4;c++)if(!erl_float(f,&p->course_best[s][c],reading)||!isfinite(p->course_best[s][c])||p->course_best[s][c]<0||p->course_best[s][c]>1000)return 0;
 for(int s=0;s<256;s++)if(!erl_small(f,&p->beaten[s],2,reading)||p->beaten[s]>1023)return 0;
 return 1;
}
