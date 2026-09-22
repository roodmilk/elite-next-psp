/* The Open Channel: compact, data-driven main campaign.  Briefings now run a
 * short locked conversation before the player can leave for the objective. */
enum { SAGA_DOCK, SAGA_SCAN, SAGA_HUNT, SAGA_HOME, SAGA_CHOICE };
typedef struct { const char *title,*speaker,*line,*talk2,*talk3,*objective; unsigned char kind,role; } SagaBeat;
static const SagaBeat saga_beats[]={
 {"Something Worth Carrying","KEI","Ryn left a receiver with Mara. Bring it home.","Mara will not open it. Neither should you.","Dock, collect the sealed case, and come back.","Deliver Mara's sealed receiver",SAGA_DOCK,0},
 {"The Quiet Between Pings","ORU","The animals answer a signal our charts ignore.","Stay quiet. Do not paint the herd with lasers.","Scan the marked ping, then leave them the sky.","Scan a signal in the marked system",SAGA_SCAN,1},
 {"All Hands Accounted For","TAMSIN","A convoy vanished where the route says safe.","Their last port still logs the outbound stamp.","Find that stamp. We need a place, not a rumour.","Find the convoy's last port",SAGA_DOCK,2},
 {"A Lawful Mistake","IONA","That theft report predates the cargo. Prove it.","Bring me the timestamps. I will stand with you.","Return to Lave with the records intact.","Bring the records to Iona",SAGA_HOME,3},
 {"The Last Useful Thing","NADI","That wreck still has a recorder worth saving.","Scan before scavengers strip the useful parts.","Get a clean dump, then get clear.","Recover a fresh scan from the wreck",SAGA_SCAN,1},
 {"The Price of Silence","SABLE","Meridian pays well for routes nobody questions.","You can sell silence, or you can keep a conscience.","Choose who holds the ledger. Choose carefully.","Choose who receives the route ledger",SAGA_CHOICE,2},
 {"No Cargo Is Just Cargo","MARA","A clinic has power cells and no open lane.","People will die if this stays a paperwork problem.","Reach the settlement and open the dock.","Reach the isolated settlement",SAGA_DOCK,0},
 {"Ghosts of GalCop","VENN","Old GalCop beacons remember better days.","Walk the chain. Listen for what still answers.","Inspect each node before the signal fades.","Inspect the abandoned beacon chain",SAGA_SCAN,1},
 {"The Mycoid Ledger","IONA","INRA won a war and buried the price of it.","This record is heavy. Do not let it vanish again.","Secure it, then decide who may read it.","Secure the sealed historical record",SAGA_DOCK,3},
 {"Where the Giants Turn","ORU","Cut your engines. We are visitors here.","Observe. Do not herd. Do not fire.","Log the migration and leave their road clear.","Observe the migration signal",SAGA_SCAN,1},
 {"The Person on the Other End","RYN","I'm alive. The clever explanation can wait.","Shelter coordinates are rough. Come anyway.","Reach me. We talk when you are here.","Reach Ryn's shelter",SAGA_DOCK,0},
 {"What We Carry Home","KEI","Ryn is safe. Her evidence makes us dangerous.","Publish and burn bridges, or hide and stay small.","Decide how openly this goes out.","Decide how openly to publish",SAGA_CHOICE,0},
 {"A Map for Everyone","RYN","Three powers want one map. None should own it.","Copies for a neutral archive. No exclusives.","Take the packets and do not pick a flag.","Take copies to a neutral archive",SAGA_DOCK,3},
 {"Federal Measure","TAMSIN","The Federation offers security with conditions.","Listen. You do not have to sign anything yet.","Hear the envoy, then keep your own counsel.","Hear the Federal envoy",SAGA_DOCK,2},
 {"Imperial Courtesy","SABLE","The Empire never calls a debt a chain.","Courtesy is a contract with nicer stationery.","Hear them out. Remember who smiles.","Hear the Imperial envoy",SAGA_DOCK,2},
 {"Alliance of Necessity","IONA","The Alliance offers witnesses, not certainty.","Witnesses matter when the powerful rewrite maps.","Hear the Alliance case before you choose.","Hear the Alliance envoy",SAGA_DOCK,3},
 {"The Coldest Signal","PIP","Signal structure: nonhuman. Confidence: troubling.","Do not fire. Record. Leave a polite wake.","Scan the silent contact and withdraw.","Scan the silent contact; do not fire",SAGA_SCAN,1},
 {"No Easy Flag","RYN","Pick allies, not owners. We live with the difference.","A coalition can hold a relay without owning you.","Choose who stands on the channel with us.","Choose the relay coalition",SAGA_CHOICE,0},
 {"The Black Flight","VENN","Meridian interceptors crossed the public lane.","Break the blockade. Prefer lives over wreckage.","Clear the lane so the packets can move.","Break their blockade",SAGA_HUNT,2},
 {"Aegis Echo","ORU","Aegis learned that knowledge needs rescue ships too.","The buoy still sings. Recover it intact.","Bring the research home without a war.","Recover the research buoy",SAGA_SCAN,1},
 {"The Open Channel","KEI","Disable the amplifiers. Keep the crews alive.","This is the cut. Precision over revenge.","Reach the primary relay and clear it.","Reach and clear the primary relay",SAGA_HUNT,0},
 {"The Answering Dark","RYN","Something answered. It may never have meant us.","Record the echo. Do not chase it into myth.","Log the distant contact and come home.","Record the distant Thargoid echo",SAGA_SCAN,1},
 {"Who Keeps the Light","IONA","Someone must hold the keys, and accept inspection.","Public, Guild, or lawful archive — pick one.","Choose custody. Then live with the watchers.","Choose public, Guild, or lawful custody",SAGA_CHOICE,3},
 {"A Berth Kept Warm","KEI","You brought Ryn home. This berth is yours now.","Free flight stays. The channel stays open.","Return to Lave Hub when you are ready.","Return to Lave System Hub",SAGA_HOME,0}
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
