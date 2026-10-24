/* Rare, optional conversations with actual nearby civilian ships. Session-only. */
static int night_npc=-1,night_system=-1,night_story=0,night_stage=0,night_open=0,night_seen=0;
static float night_wait=480,night_timeout=0;
#define NIGHT_STORY_COUNT 12
#define NIGHT_STORY_MASK ((1<<NIGHT_STORY_COUNT)-1)
static const char *night_intro[]={
 "Anyone awake on this frequency? I am on the last watch. The kettle has stopped working and I have already apologised to it twice. A little company would help.",
 "Sorry to interrupt your flight. I have a cargo hold full of empty flowerpots and a question: do you ever get homesick for somewhere you have only visited once?",
 "Quiet-watch channel, if you have a moment. My father taught me to navigate by engine sounds. Tonight this old drive sounds exactly like his. Funny what follows you out here.",
 "Civilian hauler Celandine calling. Nothing urgent. I found an old impact scar under a replacement panel today, and I cannot remember the flight that put it there.",
 "You look as though you are between jobs. So am I, technically. Dispatch calls it mandatory rest; my hands keep reaching for controls that are already idle.",
 "Have you ever arrived at a station during a festival you knew nothing about? I loaded six tonnes of machine seals and left wearing paper wings.",
 "Private channel, Commander. My manifest says precision instruments. My hold sounds like a cupboard falling down a staircase. I am afraid to look.",
 "I am carrying one small tree to an orbital school. The children named it before launch. That has made every course correction feel alarmingly personal.",
 "Do you have a superstition about jump points? I always cross the marker from below. It is nonsense, but the one time I did not, the drive coughed for three systems.",
 "Sorry, stranger. I passed my brother's ship two jumps ago. We have not spoken in eight years, and both of us pretended not to recognise the other.",
 "My apprentice misread a loading code and sent twelve pressure suits to a beach colony. I should be furious. Mostly I am wondering what they will do with them.",
 "Long-haul pilot on a quiet lane. This route is older than the station at either end. Sometimes I wonder how many people followed it before anyone gave the systems names."
};
static const char *night_tale[]={
 "My copilot used to leave terrible riddles taped to the kettle. She retired planetside last month. I thought I would enjoy the silence. Turns out silence does not laugh when you get the answer wrong. I still make two cups. One gets cold beside her seat.",
 "There was a greenhouse on a relay where I sheltered during a drive repair. Nothing special: beans, damp soil, a chair with one short leg. An old gardener gave me an afternoon's work and refused payment. I remember that place more clearly than the world where I grew up.",
 "He could hear a loose pump through three bulkheads. I thought it was magic. Years later he admitted he had broken every part on that ship at least once. Experience, he said, is mostly remembering which noise becomes expensive. I caught myself saying it to an apprentice yesterday.",
 "The scar is deep enough to have frightened me. Somebody patched it by hand, then wrote GOOD ENOUGH inside the plate. The handwriting is mine. I remember the repair, but not what I survived to make it.",
 "For nineteen years a moving ship meant I was useful. Now the engines are cold and I keep inventing inspections. The ship is fine. I think I am the machinery making the noise.",
 "A dock worker pulled me into the procession before I could object. Nobody spoke my language, but everybody knew the same ridiculous dance. My delivery was late and the customer gave me extra food for the road.",
 "I opened the hold. The instruments are secure. The noise is a service robot trapped inside an empty shipping drum. It has been cleaning the same square centimetre for six hours.",
 "The school sent a recording of thirty children explaining how to care for it. Every child gave different instructions. I have flown through combat with less disagreement coming over comms.",
 "My first commander did it. Said the gravity gradient settled the injectors. I later learned she invented the explanation because I was frightened. It worked on me, if not the drive.",
 "We took over our parents' two ships and chose opposite routes. Every argument became easier at ten light-years. Seeing his patched engine cowling made the old anger feel embarrassingly well maintained.",
 "The colony replied that they will use the suits for underwater construction. They even offered another order. My apprentice now believes logistics rewards confidence more than accuracy.",
 "There are old rescue beacons along it, all dead. I still dip my wing at each one. No regulation says to. It just feels wrong to pass a place where somebody once waited without acknowledging them."
};
static const char *night_answer[]={
 "I could call her. I keep thinking retirement means I should leave her in peace. But she sent me a photograph of her new kettle yesterday. No message, just the kettle. Perhaps that was the riddle. Thank you. I think I finally have the answer.",
 "Maybe home is wherever somebody gives you a chair without asking when you are leaving. I am bringing these pots back to that relay. The gardener never asked for them. I just wanted an excuse to return. Saying it aloud makes it sound less foolish.",
 "That is what the apprentice said: 'You sound like him.' I almost corrected her. Then the pump settled down, and for a moment it felt like three generations were listening to the same engine. I suppose a person can leave you something that is not cargo.",
 "You are right. Forgetting the danger does not erase the courage. I trusted my own repair enough to keep flying. Perhaps GOOD ENOUGH was not an apology after all.",
 "A person can stop without becoming useless. That sounds obvious when somebody else says it. I will finish this rest period without dismantling anything important.",
 "Maybe being lost for an evening is why I remember it. I still have the wings folded behind my chart case. I think I will wear them on the return trip.",
 "I freed the robot. It immediately reported me for obstructing scheduled maintenance. Fair enough. At least the instruments are quiet and I have learned what guilt sounds like inside a drum.",
 "I will follow the written care sheet and tell the children it behaved perfectly. Some cargo needs truth; some needs a reassuring arrival photograph.",
 "Then I will keep the ritual and retire the explanation. Habits do not need to be physics to make a pilot feel steady.",
 "I think I wanted him to hail first so I could forgive him without risking anything. That is not much of an offer. I still have his channel code. I can do better at the next jump.",
 "I will correct the loading procedure before confidence sends medical ice to a refinery. But I may let the apprentice keep the thank-you message. One mistake does not have to become a permanent name.",
 "Thank you for understanding. I will keep dipping the wing. Remembering strangers costs almost nothing, and the lane feels less empty for it."
};
static const char *night_reflection[]={
 "Perhaps she misses the second cup too.","Sounds like someone made room for you.","Sounds like he is still teaching you.",
 "Forgetting the danger does not erase what you survived.","You are allowed to stop without becoming useless.","Being lost may be why it mattered.",
 "At least you found the noise before unloading.","Treat it like a passenger, not freight.","A ritual can still steady your hands.",
 "You could be the one who calls first.","Correct the mistake without crushing the apprentice.","Keep dipping the wing. Someone should remember."
};
static int night_live(void){return night_npc>=0&&night_npc<NPC_COUNT&&game.system==night_system&&!game.docked&&!game.dead&&game.planet<0&&game.jump<=0&&game.npc[night_npc].alive&&game.npc[night_npc].role==TRADERS&&!game.npc[night_npc].freighter&&game.npc[night_npc].target==-1&&length(sub(game.npc[night_npc].pos,game.pos))<12000;}
static void night_close(void){if(night_open||!strncmp(game.voice,night_intro[night_story],sizeof(game.voice)-1))game.voice_time=0;night_npc=-1;night_open=0;night_timeout=0;night_wait=600+(game.rng%301);}
static void night_begin(int npc,int story){night_npc=npc;night_system=game.system;night_story=story%NIGHT_STORY_COUNT;night_stage=0;night_open=0;night_timeout=20;night_seen|=1<<night_story;game.encounter_kind=ENCOUNTER_NONE;speak(&game,VOICE_CONTACT,night_intro[night_story]);game.voice_role=TRADERS;game.voice_seed=game.system*NPC_COUNT+npc;game.voice_time=20;}
static int night_ready(void){return night_live()&&game.voice_who==VOICE_CONTACT&&game.voice_seed==game.system*NPC_COUNT+night_npc&&!strncmp(game.voice,night_intro[night_story],sizeof(game.voice)-1)&&!night_open&&night_timeout>0&&game.message_time<=0&&game.voice_time>0&&game.attacked<=0&&!game.police_stop&&game.incoming_missile<=0;}
static void night_tick(float dt){
 if(night_npc>=0){if(!night_live()||game.attacked>0||game.police_stop||game.incoming_missile>0){night_close();return;}if(!night_open){night_timeout-=dt;if(night_timeout<=0||game.voice_time<=0||strncmp(game.voice,night_intro[night_story],sizeof(game.voice)-1))night_close();}return;}
 if(page!=FLIGHT||paused||quiet_comms||tutorial_active(&game)||game.docked||game.dead||game.planet>=0||game.jump>0||game.dock_stage||game.approach>=0||game.police_stop||game.attacked>0||game.incoming_missile>0)return;
 night_wait-=dt;if(night_wait>0||game.voice_time>0||game.message_time>0||game.encounter>0)return;
 night_wait=60;int npc=-1;float near=8000;
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];float d=length(sub(n->pos,game.pos));if(n->alive&&n->role==TRADERS&&!n->freighter&&n->target==-1&&d<near){near=d;npc=i;}}
 if(npc<0)return;
 if(night_seen==NIGHT_STORY_MASK)night_seen=0;int story=(game.system+(int)game.time)%NIGHT_STORY_COUNT;for(int i=0;i<NIGHT_STORY_COUNT&&((night_seen>>story)&1);i++)story=(story+1)%NIGHT_STORY_COUNT;
 night_begin(npc,story);
}
static void night_panel(void){
 dialogue_begin("QUIET WATCH / PRIVATE CHANNEL");
 const char *body=night_stage==0?night_intro[night_story]:night_stage==1?night_tale[night_story]:night_stage==2?night_answer[night_story]:night_stage==3?game.voice:"Thanks for sharing the watch. I will let you get back to your flight. Clear skies, Commander.";
 dialogue_speech(target_name(NPC_ID_MIN+night_npc),TRADERS,game.voice_seed,body,0);
 dialogue_context("YOUR REPLY",night_stage==3?"Accepting records an appointment in your Mission Log. No cargo or credits move until you hail this trader again.":"A quiet conversation. You can leave the channel at any time.");
 dialogue_reply(0,night_stage==0?"I have a little time. Tell me.":night_stage==1?night_reflection[night_story]:night_stage==2?"I am glad you called.":night_stage==3?"Accept and add the exchange to my Mission Log.":"Clear skies. End channel.");
 dialogue_reply(1,night_stage==4?"Thanks for the company.":night_stage==3?"Keep the offer for another time.":"Before you go, any cargo to trade?");dialogue_reply(2,"Sign off. Safe travels.");footer("UP/DOWN   X REPLY   O END CHANNEL");
}
static void night_reply(int choice){
 if(!night_live()){night_close();change_page(FLIGHT);message(&game,"Private channel lost.");return;}
 if(choice==2||night_stage==4||(choice==1&&night_stage==3)){night_close();change_page(FLIGHT);return;}
 if(choice==1){
  trader_offer_hail(&game,night_npc);night_stage=game.trader_offer_active?3:4;game.message_time=0;row=0;return;
 }
 if(night_stage==3){
  if(!game.trader_offer_active||game.trader_offer_npc!=night_npc||game.trader_offer_system!=game.system){night_stage=4;return;}
  accept_trader_offer(&game);if(!game.trader_offer_active)night_stage=4;return;
 }
 night_stage=night_stage==2?4:night_stage+1;row=0;
}
