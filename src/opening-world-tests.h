/* Included inside the core smoke runner; legacy setup stays opening.stage==0. */
{
 static Game home;game_init(&home);home.opening.stage=OP_YAW;home.system=7;game_spawn(&home);
 CHECK(!strcmp(home.systems[7].name,"SOL")&&!strcmp(home.bodies[1].name,"EARTH"),"Homeward opening creates Sol and playable Earth identity");
 CHECK(!strcmp(home.bodies[0].name,"SOL")&&!strcmp(home.bodies[2].name,"MARS")&&!strcmp(home.bodies[3].name,"JUPITER")&&!strcmp(home.bodies[4].name,"NEPTUNE"),"Sol physical body subset uses familiar names");
 int safe=1;for(int n=0;n<NPC_COUNT;n++)if(home.npc[n].alive&&(home.npc[n].role==PIRATES||home.npc[n].bounty_slot>=0))safe=0;
 CHECK(safe,"Sol flight tutorial suppresses hostile and bounty-board traffic");
 home.planet=1;Vec3 pad=surface_site(&home,1);float centre=terrain_height(&home,pad.x,pad.z),hill=terrain_height(&home,pad.x+2400,pad.z+1600),mountain=terrain_height(&home,pad.x+650,pad.z+3600);
 CHECK(isfinite(centre)&&isfinite(hill)&&mountain>centre+300&&fabsf(hill-centre)>5,"Earth countryside uses traversable rolling terrain and a distant mountain range");
 home.ship_pos=pad;home.surface=2;
 {int safe_trail=1;for(int i=0;i<EARTH_MEADOW_FLOWERS;i++){float x,z;earth_meadow_flower(i,&x,&z);safe_trail&=eva_position_allowed(&home,pad.x+x,pad.z+z);}
 CHECK(safe_trail,"flower trail follows walkable ground clear of water trunks outcrops and the ship");}
 {int solid=1;for(int i=0;i<EARTH_MEADOW_ROCKS;i++){float x,z,rx,rz,h;earth_meadow_rock(i,&x,&z,&rx,&rz,&h);solid&=!eva_position_allowed(&home,pad.x+x,pad.z+z);}
 CHECK(solid&&!eva_position_allowed(&home,pad.x-174,pad.z+132),"meadow visible outcrops and lake reject walking through them");}
 /* Connectivity through real collision, not just permissive radial bounds.
  * Check the optional banks/overlook as well as the required orb and ship. */
 {static unsigned char allowed[67*67],seen[67*67];static unsigned short todo[67*67];memset(seen,0,sizeof(seen));
 for(int z=0;z<67;z++)for(int x=0;x<67;x++)allowed[z*67+x]=eva_position_allowed(&home,pad.x-330+x*10,pad.z-330+z*10);
 int head=0,tail=0,start=25*67+41; /* Local (80,-80), by the landing gear. */
 if(allowed[start]){todo[tail++]=start;seen[start]=1;}
 while(head<tail){int at=todo[head++],x=at%67,z=at/67;for(int dir=0;dir<4;dir++){
  int nx=x+(dir==0)-(dir==1),nz=z+(dir==2)-(dir==3);if(nx<0||nx>=67||nz<0||nz>=67)continue;
  int next=nz*67+nx;if(seen[next]||!allowed[next])continue;
  if(!eva_position_allowed(&home,pad.x-330+(x+nx)*5,pad.z-330+(z+nz)*5))continue;
  seen[next]=1;todo[tail++]=next;
 }}
 CHECK(seen[39*67+43]&&seen[45*67+27]&&seen[57*67+46],"orb lake-bank and overlook remain reachable from the ship around real trees rocks and water");
 }
 fit_clear_all(&home);home.fit[FIT_WPN]=OPENING_MINIBOLT;fit_rebuild(&home);
 CHECK(home.laser&&fit_value_valid(FIT_WPN,OPENING_MINIBOLT)&&laser_shot_damage(&home)==2.f&&mine_shot_damage(&home)==54.f,"MiniBolt Lite mines efficiently while barely damaging ships");
 home.fit[FIT_WPN]=20;fit_rebuild(&home);CHECK(laser_shot_damage(&home)==18.f&&mine_shot_damage(&home)==54.f,"Legacy Mining Laser tuning and equipment ID remain unchanged");
 home.opening.stage=OP_WAKE;home.planet=-1;system_bodies(&home);
 CHECK(!strcmp(home.systems[7].name,"LAVE")&&home.systems[7].economy==5&&home.systems[7].tech==4&&strcmp(home.bodies[1].name,"EARTH"),"Displacement restores generated Lave without leaving Sol in the galaxy");
}
