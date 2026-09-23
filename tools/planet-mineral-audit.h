/* Diagnostic only: actual generated pools, no life edits/ticks/scans. */
{
 FILE *audit=fopen("planet-mineral-audit.txt","w");
 int total=0,zero=0,valid=1,control=0,examples=0,reachable=1;
 for(int sys=0;sys<256;sys++)for(int body=1;body<BODY_COUNT;body++){
  game_init(&g);g.system=sys;launch(&g);g.contract=-1;g.job_n=0;
  if(g.bodies[body].type==GAS||g.bodies[body].type==SUN)continue;
  g.pos=add(g.bodies[body].pos,(Vec3){0,0,-g.bodies[body].radius-500});g.yaw=g.pitch=0;
  int entry=approach_planet(&g,body)&&enter_planet(&g);
  int alive=0,mineral=0,scanned=0;Vec3 pad=surface_site(&g,1);
  for(int i=0;i<LIFE_COUNT;i++){
   Lifeform *l=&g.life[i];alive+=l->alive!=0;scanned+=l->scanned!=0;
   mineral+=l->alive&&l->kind==LIFE_MINERAL;
   float dx=l->pos.x-pad.x,dz=l->pos.z-pad.z;
   reachable &= dx*dx+dz*dz<EVA_FIELD_RADIUS*EVA_FIELD_RADIUS&&!terrain_is_water(&g,l->pos.x,l->pos.z);
  }
  valid &= entry&&g.planet==body&&g.surface==0&&alive==LIFE_COUNT&&!scanned;
  total++;zero+=mineral==0;
  if(!mineral||(sys==7&&body==1)){
   if(audit){fprintf(audit,"SYSTEM %d BODY %d SEED %u ALIVE %d MINERAL %d SCANNED %d KINDS",sys,body,g.bodies[body].seed,alive,mineral,scanned);for(int i=0;i<LIFE_COUNT;i++)fprintf(audit," %d",g.life[i].kind);fprintf(audit,"\n");}
  }
  if((sys==0&&body==3)||(sys==42&&body==4)||(sys==48&&body==3)||(sys==73&&body==3)||(sys==83&&body==2))examples+=entry&&!mineral&&alive==8&&!scanned;
  if(sys==7&&body==1)control=mineral==2&&g.life[0].kind==LIFE_MINERAL&&g.life[5].kind==LIFE_MINERAL;
 }
 if(audit){fprintf(audit,"SUMMARY solid=%d zero_mineral=%d valid=%d examples=%d control=%d inside_dry_field=%d\n",total,zero,valid,examples,control,reachable);fclose(audit);}
 CHECK(audit&&total==768&&zero==19&&valid&&examples==5&&control&&reachable,"diagnostic: runtime confirms 19 of 768 solid worlds lack minerals, five examples and Lave control match");
 game_init(&g);launch(&g);
}
