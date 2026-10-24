/* Shared simulation/render geometry. Race time freezes during pause and
 * advances with slow motion; no wall-clock phase or save-only approximation. */
int race_hazard(const SpaceRace *r,int section,float time,Vec3 *centre,Vec3 *half){
 if(r->layout==4)return 0;
 if(section<0||section>=RACE_GATES||section%8!=3||r->phase!=RACE_RUNNING)return 0;
 const RaceGate *q=&r->gates[section];Vec3 prev=section?r->gates[section-1].pos:r->start,mid=mul(add(prev,q->pos),.5f);
 int kind=section/8+1;float phase=time*.35f+(r->seed%100)*.0628f+section;
 float x=6200*sinf(phase),y=500+450*cosf(phase*.7f);*half=(Vec3){650,650,650};
 if(kind==2)*half=(Vec3){1000,550,850};
 if(kind==3){x=0;y=1500+1800*sinf(phase);*half=(Vec3){5200,110,320};}
 if(kind==4)*half=(Vec3){1700,550,850};
 *centre=add(mid,add(mul(q->right,x),mul(q->up,y)));return kind;
}
static int race_moving_hazard_hit(const SpaceRace *r,int gate,Vec3 from,Vec3 to,float radius,float dt,Vec3 *hit){
 int start=gate>1?gate-2:0,end=start+5;if(end>RACE_GATES)end=RACE_GATES;
 for(int k=start;k<end;k++){Vec3 c,old,half,unused;if(!race_hazard(r,k,r->elapsed,&c,&half))continue;race_hazard(r,k,fmaxf(0,r->elapsed-dt),&old,&unused);
  const RaceGate *q=&r->gates[k];Vec3 a=sub(from,old),b=sub(to,c);a=(Vec3){dot(a,q->right),dot(a,q->up),dot(a,q->normal)};b=(Vec3){dot(b,q->right),dot(b,q->up),dot(b,q->normal)};
  if(race_slab(a,b,half,radius)){if(hit)*hit=to;return 1;}
 }return 0;
}
