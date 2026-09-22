/* The Open Channel: compact, data-driven main campaign.  Text is deliberately
 * short enough for the PSP display; the complete scripts live in the bible. */
enum { SAGA_DOCK, SAGA_SCAN, SAGA_HUNT, SAGA_HOME, SAGA_CHOICE };
typedef struct { const char *title,*speaker,*line,*objective; unsigned char kind,role; } SagaBeat;
static const SagaBeat saga_beats[]={
 {"Something Worth Carrying","KEI","Ryn left a receiver with Mara. Bring it home.","Deliver Mara's sealed receiver",SAGA_DOCK,0},
 {"The Quiet Between Pings","ORU","The animals answer a signal our charts ignore.","Scan a signal in the marked system",SAGA_SCAN,1},
 {"All Hands Accounted For","TAMSIN","A convoy vanished where the route says safe.","Find the convoy's last port",SAGA_DOCK,2},
 {"A Lawful Mistake","IONA","That theft report predates the cargo. Prove it.","Bring the records to Iona",SAGA_HOME,3},
 {"The Last Useful Thing","NADI","That wreck still has a recorder worth saving.","Recover a fresh scan from the wreck",SAGA_SCAN,1},
 {"The Price of Silence","SABLE","Meridian pays well for routes nobody questions.","Choose who receives the route ledger",SAGA_CHOICE,2},
 {"No Cargo Is Just Cargo","MARA","A clinic has power cells and no open lane.","Reach the isolated settlement",SAGA_DOCK,0},
 {"Ghosts of GalCop","VENN","Old GalCop beacons remember better days.","Inspect the abandoned beacon chain",SAGA_SCAN,1},
 {"The Mycoid Ledger","IONA","INRA won a war and buried the price of it.","Secure the sealed historical record",SAGA_DOCK,3},
 {"Where the Giants Turn","ORU","Cut your engines. We are visitors here.","Observe the migration signal",SAGA_SCAN,1},
 {"The Person on the Other End","RYN","I'm alive. The clever explanation can wait.","Reach Ryn's shelter",SAGA_DOCK,0},
 {"What We Carry Home","KEI","Ryn is safe. Her evidence makes us dangerous.","Decide how openly to publish",SAGA_CHOICE,0},
 {"A Map for Everyone","RYN","Three powers want one map. None should own it.","Take copies to a neutral archive",SAGA_DOCK,3},
 {"Federal Measure","TAMSIN","The Federation offers security with conditions.","Hear the Federal envoy",SAGA_DOCK,2},
 {"Imperial Courtesy","SABLE","The Empire never calls a debt a chain.","Hear the Imperial envoy",SAGA_DOCK,2},
 {"Alliance of Necessity","IONA","The Alliance offers witnesses, not certainty.","Hear the Alliance envoy",SAGA_DOCK,3},
 {"The Coldest Signal","PIP","Signal structure: nonhuman. Confidence: troubling.","Scan the silent contact; do not fire",SAGA_SCAN,1},
 {"No Easy Flag","RYN","Pick allies, not owners. We live with the difference.","Choose the relay coalition",SAGA_CHOICE,0},
 {"The Black Flight","VENN","Meridian interceptors crossed the public lane.","Break their blockade",SAGA_HUNT,2},
 {"Aegis Echo","ORU","Aegis learned that knowledge needs rescue ships too.","Recover the research buoy",SAGA_SCAN,1},
 {"The Open Channel","KEI","Disable the amplifiers. Keep the crews alive.","Reach and clear the primary relay",SAGA_HUNT,0},
 {"The Answering Dark","RYN","Something answered. It may never have meant us.","Record the distant Thargoid echo",SAGA_SCAN,1},
 {"Who Keeps the Light","IONA","Someone must hold the keys, and accept inspection.","Choose public, Guild, or lawful custody",SAGA_CHOICE,3},
 {"A Berth Kept Warm","KEI","You brought Ryn home. This berth is yours now.","Return to Lave System Hub",SAGA_HOME,0}
};
#define SAGA_COUNT ((int)(sizeof(saga_beats)/sizeof(saga_beats[0])))
static int saga_system(const Game *g,int chapter){
 int want=(chapter*37+19)&255,best=7,score=999999;
 for(int i=0;i<256;i++){if(i==7)continue;int d=(int)(distance_ly(g,7,i)*10);int s=abs(i-want)+d/4;if(d>=25&&d<=180&&s<score){score=s;best=i;}}
 return best;
}
/* Story navigation must remain explainable even with an almost empty tank.
 * Plan against the fitted drive, then let the map mark the next hop LOW until
 * the player refuels.  The selectable marker is always a visible local star. */
static int saga_next_hop(const Game *g,int *jumps){
 Game route=*g;route.fuel=(float)player_ships[g->ship].range;
 return route_next_hop(&route,g->saga_dest,jumps);
}
static void saga_begin(Game *g){
 if(g->campaign_stage<6||g->saga_chapter>=SAGA_COUNT||g->saga_step)return;
 const SagaBeat *b=&saga_beats[g->saga_chapter];g->saga_dest=(b->kind==SAGA_HOME)?7:saga_system(g,g->saga_chapter);
 g->saga_start=(b->kind==SAGA_HUNT)?g->npc_kills:g->discoveries;g->saga_step=1;
 message(g,b->objective);speak(g,b->role==0?VOICE_KEI:b->role==3?VOICE_LAW:VOICE_CONTACT,b->line);
}
static int saga_ready(const Game *g){
 if(!g->saga_step||g->saga_chapter>=SAGA_COUNT)return 0;
 const SagaBeat *b=&saga_beats[g->saga_chapter];
 if(b->kind==SAGA_CHOICE)return g->saga_choice>0;
 if(b->kind==SAGA_HOME)return g->docked&&g->system==7;
 if(b->kind==SAGA_DOCK)return g->docked&&g->system==g->saga_dest;
 if(b->kind==SAGA_SCAN)return g->system==g->saga_dest&&g->discoveries>g->saga_start;
 return g->system==g->saga_dest&&g->npc_kills>g->saga_start;
}
static int saga_advance(Game *g){
 if(!saga_ready(g))return 0;
 const SagaBeat *b=&saga_beats[g->saga_chapter];
 if(b->kind==SAGA_CHOICE){int c=g->saga_choice-1;if(c<4)g->saga_trust[c]++;g->saga_flags|=1u<<(g->saga_chapter==5?0:g->saga_chapter==11?1:g->saga_chapter==17?2:3);}
 int reward=1800+g->saga_chapter*120;if(g->credits<=100000000-reward)g->credits+=reward;
 g->saga_chapter++;g->saga_step=0;g->saga_choice=0;g->cue=SFX_SELECT;
 message(g,g->saga_chapter>=SAGA_COUNT?"The Open Channel complete. Free flight continues.":"Chapter complete. The next briefing is ready.");return 1;
}
