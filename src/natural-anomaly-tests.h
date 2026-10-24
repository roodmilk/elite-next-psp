/* Native review only, included after production save/load helpers. */
int natural_anomaly_tests(const char*path){
 FILE*f=fopen(path,"w");if(!f)return 1;int fails=0;
#define NAT_CHECK(ok,label) do{int pass=(ok);fprintf(f,"%s %s\n",pass?"PASS":"FAIL",label);fails+=!pass;}while(0)
 static Game g;game_init(&g);story_complete(&g);int counts[RIFT_TYPE_COUNT]={0};unsigned regions[RIFT_TYPE_COUNT]={0};int sites=0,duplicate=0,clear=1,stable=1,legacy=1,spawned=1;
 NAT_CHECK(sizeof(rift_names)/sizeof(*rift_names)==RIFT_TYPE_COUNT&&sizeof(rift_reports)/sizeof(*rift_reports)==RIFT_TYPE_COUNT&&sizeof(rift_lore)/sizeof(*rift_lore)==RIFT_TYPE_COUNT,"32 aligned names, instrument reports and lore entries");
 for(int sys=0;sys<256;sys++){
  g.system=sys;game_spawn(&g);unsigned mask=anomaly_slots_mask(&g,sys);int n=0;if(sys==7||sys%11==0)n=1;if(danger_rating(&g,sys)>=4)n++;if(sys%23==0)n=2;
  legacy&=(mask&3u)==((1u<<n)-1u);Vec3 saved[4];
  for(int i=0;i<4;i++){
   saved[i]=g.anomaly[i].pos;spawned&=g.anomaly[i].alive==((mask>>i)&1);
   if(i<2){legacy&=rift_type(sys,i)==((i&1)?3:(sys*17+i*7)%3);if(i<n){unsigned h=sector_hash(sys*401u+i*9973u);float a=i*2.15f+sys*.27f+(h%400)*.001f,r=3200.f+((h>>8)%4200);Vec3 p={cosf(a)*r,((int)((h>>16)%1600)-800),sinf(a)*r*.75f+(int)((h>>20)%900)-200};legacy&=length(sub(p,g.anomaly[i].pos))<.01f;}continue;}
   if(!g.anomaly[i].alive)continue;int t=rift_type(sys,i);spawned&=t>=4&&t<RIFT_TYPE_COUNT;counts[t]++;sites++;regions[t]|=1u<<((g.systems[sys].x/64)+4*(g.systems[sys].y/64));
   for(int b=0;b<BODY_COUNT;b++)if(body_active(&g.bodies[b]))clear&=length(sub(g.anomaly[i].pos,g.bodies[b].pos))>=g.bodies[b].radius+2999.f;
   for(int b=0;b<HUB_COUNT;b++)clear&=length(sub(g.anomaly[i].pos,hub_position(&g,b)))>=6499.f;
   for(int j=0;j<i;j++)if(g.anomaly[j].alive)clear&=length(sub(g.anomaly[i].pos,g.anomaly[j].pos))>=3299.f;
  }
  duplicate+=g.anomaly[3].alive&&rift_type(sys,2)==rift_type(sys,3);
  game_spawn(&g);for(int i=0;i<4;i++)if(g.anomaly[i].alive)stable&=length(sub(saved[i],g.anomaly[i].pos))<.001f;
 }
 NAT_CHECK(legacy,"all 256 systems preserve legacy slot 0/1 existence, identity and exact positions");
 NAT_CHECK(spawned&&sites==384&&!duplicate,"384 new live sites, at least one per system, no repeated new type in a system");
 NAT_CHECK(clear,"every new field clears active body surfaces, all hubs and other anomalies");
 NAT_CHECK(stable,"revisiting all 256 systems preserves anomaly positions independently of traffic RNG");
 for(int t=4;t<RIFT_TYPE_COUNT;t++){int expected=t<16?20:t<24?13:5,n=0;for(int r=0;r<16;r++)n+=(regions[t]>>r)&1;char msg[120];snprintf(msg,sizeof(msg),"%s: %d sites across %d geographic regions",rift_names[t],counts[t],n);NAT_CHECK(counts[t]==expected&&n>=(t<16?12:t<24?10:5),msg);}
 game_init(&g);story_complete(&g);g.system=7;game_spawn(&g);launch(&g);g.planet=-1;g.dock_stage=0;g.jump=0;g.police_stop=0;g.pos=g.anomaly[2].pos;int credits=g.credits,discoveries=g.discoveries;
 NAT_CHECK(analysis_scan(&g,ANOMALY_ID_MIN+2)&&g.rift_report==3&&(g.rift_logged[7]&4)&&g.discoveries==discoveries+1&&g.credits>credits,"new slot scans through real analysis, opens its report and pays once");
 credits=g.credits;NAT_CHECK(!analysis_scan(&g,ANOMALY_ID_MIN+2)&&g.credits==credits&&g.discoveries==discoveries+1,"repeat analysis reopens the same report without paying twice");
 NAT_CHECK(strncmp(g.voice,rift_reports[rift_type(7,2)],strlen(g.voice))==0,"computer analysis uses the new type's actual instrument report");
 for(int sys=0;sys<256;sys++)g.rift_logged[sys]=(sys&3)|4|(natural_anomaly_sites[sys][1]?8:0);
 g.docked=1;NAT_CHECK(save_game(&g,"natural-test.sav"),"new records write through unchanged production save format");
 memset(g.rift_logged,0,256);int loaded=load_game(&g,"natural-test.sav"),same=1;
 for(int sys=0;sys<256;sys++)same&=g.rift_logged[sys]==((sys&3)|4|(natural_anomaly_sites[sys][1]?8:0));
 NAT_CHECK(loaded&&same,"legacy and new slot records survive save/reload in every system");
 remove("natural-test.sav");remove("natural-test.sav.bak");fprintf(f,"RESULT %d failures\n",fails);fclose(f);return fails;
#undef NAT_CHECK
}
