int lave_world_tests(const char *path){
 FILE *f=fopen(path,"w");if(!f)return 1;int failures=0;
 #define WCHECK(ok,name) do{int pass=(ok);fprintf(f,"%s planet %d: %s\n",pass?"PASS":"FAIL",body,name);failures+=!pass;}while(0)
 enum {LAVE_GRID=528,LAVE_MID=264};
 Game *g=calloc(1,sizeof(Game));unsigned char *allowed=calloc(LAVE_GRID*LAVE_GRID,1),*seen=calloc(LAVE_GRID*LAVE_GRID,1);int *queue=calloc(LAVE_GRID*LAVE_GRID,sizeof(int));
 if(!g||!allowed||!seen||!queue){fclose(f);free(g);free(allowed);free(seen);free(queue);return 1;}
 for(int body=2;body<=4;body++){
  game_init(g);g->system=7;system_bodies(g);launch(g);story_complete(g);g->approach=body;
  WCHECK(enter_planet(g),"enter correct atmosphere");
  Vec3 pad=surface_site(g,1);g->pos=pad;g->pos.y=42;g->speed=0;g->story_flags|=STORY_EV_LANDING_TECH;
  WCHECK(land_planet(g)&&disembark_planet(g)&&g->surface==2,"land and disembark safely");
  WCHECK(eva_can_board(g)&&board_planet(g)&&takeoff_planet(g),"board and take off again");
  g->surface=2;g->ship_pos=pad;g->ship_pos.y=42;g->rover_pos=add(pad,(Vec3){FIELD_GARAGE_X,22,-10});
  int bad=0;for(int z=-10000;z<=10000;z+=240)for(int x=-10000;x<=10000;x+=240){float h=terrain_height(g,pad.x+x,pad.z+z),dh=terrain_height(g,pad.x+x+1,pad.z+z)-h;if(!isfinite(h)||fabsf(dh)>8.f)bad++;}
  WCHECK(!bad,"finite varied terrain without one-metre height tears");
  Vec3 reachable[10]={{0}};
  for(int driving=0;driving<2;driving++){
   g->rover_driving=driving;memset(seen,0,LAVE_GRID*LAVE_GRID);
   int start=-1;float closest=1e20f;
   for(int z=0;z<LAVE_GRID;z++)for(int x=0;x<LAVE_GRID;x++){
    float wx=pad.x+(x-LAVE_MID+.5f)*40,wz=pad.z+(z-LAVE_MID+.5f)*40;int at=z*LAVE_GRID+x;
    allowed[at]=eva_position_allowed(g,wx,wz);float d=(wx-pad.x-65)*(wx-pad.x-65)+(wz-pad.z)*(wz-pad.z);
    if(allowed[at]&&d<closest){closest=d;start=at;}
   }
   int head=0,tail=0;if(start>=0){queue[tail++]=start;seen[start]=1;}
   int reached[10]={0};
   while(head<tail){
    int at=queue[head++],x=at%LAVE_GRID,z=at/LAVE_GRID;float wx=pad.x+(x-LAVE_MID+.5f)*40,wz=pad.z+(z-LAVE_MID+.5f)*40;
    g->pos=(Vec3){wx,terrain_height(g,wx,wz)+22,wz};int id=surface_nearest_site(g,55);if(id>=0&&id<10){reached[id]=1;reachable[id]=g->pos;}
    for(int dir=0;dir<4;dir++){
     int nx=x+(dir==0?-1:dir==1?1:0),nz=z+(dir==2?-1:dir==3?1:0);if(nx<0||nz<0||nx>=LAVE_GRID||nz>=LAVE_GRID)continue;
     int n=nz*LAVE_GRID+nx;if(seen[n]||!allowed[n])continue;int clear=1;
     for(int step=1;step<4&&clear;step++)clear=eva_position_allowed(g,wx+(nx-x)*10*step,wz+(nz-z)*10*step);
     if(clear){seen[n]=1;queue[tail++]=n;}
    }
   }
   int sites=0;for(int id=0;id<10;id++)if(id!=6&&reached[id])sites++;
   fprintf(f,"Reachability %s: %d/9 sites, %d grid nodes\n",driving?"rover":"walker",sites,tail);
   WCHECK(sites==9,driving?"all site perimeters reachable by rover":"all site perimeters reachable on foot");
  }
  g->rover_driving=0;int living=0,unsafe=0;
  for(int i=0;i<LIFE_COUNT;i++)if(g->life[i].alive){living++;if(terrain_is_water(g,g->life[i].pos.x,g->life[i].pos.z))unsafe++;}
  WCHECK(living==LIFE_COUNT&&!unsafe,"all discovery subjects remain on terrain/decks");
  if(body==3){
   int water=0,deck=0;for(int z=-1000;z<=1000;z+=40)for(int x=-1000;x<=1000;x+=40){if(surface_cloud_deck(g,pad.x+x,pad.z+z))deck++;else water++;}
   WCHECK(deck>100&&water>100,"cloud settlement has real deck gaps, not a solid island");
   g->pos=add(pad,(Vec3){0,22,250});g->yaw=0;for(int i=0;i<100;i++)game_eva_tick(g,.05f,0,0,1,0,0);
   WCHECK(surface_cloud_deck(g,g->pos.x,g->pos.z)&&!terrain_is_water(g,g->pos.x,g->pos.z),"walking cannot cross exposed skyport edge");
  }
  int scanned=0;
  for(int slot=0;slot<LIFE_COUNT;slot++){
   float nearest=1e20f;Vec3 vantage={0};
   for(int at=0;at<LAVE_GRID*LAVE_GRID;at++)if(seen[at]){
    float x=pad.x+(at%LAVE_GRID-LAVE_MID+.5f)*40,z=pad.z+(at/LAVE_GRID-LAVE_MID+.5f)*40;Vec3 p={x,terrain_height(g,x,z)+22,z};
    float d=length(sub(p,g->life[slot].pos));if(d<nearest){nearest=d;vantage=p;}
   }
   g->pos=vantage;scanned+=survey_scan_target(g,slot);
  }
  WCHECK(scanned==LIFE_COUNT,"all eight subjects can actually be scanned from reachable ground");
  g->pos=reachable[0];surface_interact(g);int done=0;
  for(int id=2;id<10;id++)if(id!=6){g->pos=reachable[id];
   if(id==observatory_site(g,7,body)){g->surface_progress[7][body]|=OBS_CLUES;done+=observatory_resolve(g,id,1);}
   else done+=surface_interact(g);
  }
  WCHECK(done==7,"all seven optional field activities resolve from accessible perimeters");
  g->pos=reachable[1];int objective=surface_interact(g),before=g->credits;g->pos=reachable[0];surface_interact(g);int paid=g->credits;
  surface_interact(g);WCHECK(objective&&paid-before==field_job_reward(g,7,body)&&g->credits==paid,"local port contract pays exactly once");
  int credits=g->credits,cargo=g->cargo[12];for(int id=1;id<10;id++)if(id!=6){g->pos=reachable[id];surface_interact(g);}
  WCHECK(g->credits==credits&&g->cargo[12]==cargo,"completed sites do not duplicate cash or cargo");
  unsigned progress=g->surface_progress[7][body];g->docked=1;
  int saved=save_game(g,"lave-world-save.dat");g->surface_progress[7][body]=0;int loaded=load_game(g,"lave-world-save.dat");
  WCHECK(scanned&&saved&&loaded&&progress==g->surface_progress[7][body],"real scan and docked save/load preserve planet record");
 }
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);free(g);free(allowed);free(seen);free(queue);return failures;
 #undef WCHECK
}
