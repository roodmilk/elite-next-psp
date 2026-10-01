/* Rare, optional conversations with actual nearby civilian ships. Session-only. */
static int night_npc=-1,night_system=-1,night_story=0,night_stage=0,night_open=0,night_seen=0;
static float night_wait=480,night_timeout=0;
static const char *night_intro[]={
 "Anyone awake on this frequency? I am on the last watch. The kettle has stopped working and I have already apologised to it twice. A little company would help.",
 "Sorry to interrupt your flight. I have a cargo hold full of empty flowerpots and a question: do you ever get homesick for somewhere you have only visited once?",
 "Quiet-watch channel, if you have a moment. My father taught me to navigate by engine sounds. Tonight this old drive sounds exactly like his. Funny what follows you out here."
};
static const char *night_tale[]={
 "My copilot used to leave terrible riddles taped to the kettle. She retired planetside last month. I thought I would enjoy the silence. Turns out silence does not laugh when you get the answer wrong. I still make two cups. One gets cold beside her seat.",
 "There was a greenhouse on a relay where I sheltered during a drive repair. Nothing special: beans, damp soil, a chair with one short leg. An old gardener gave me an afternoon's work and refused payment. I remember that place more clearly than the world where I grew up.",
 "He could hear a loose pump through three bulkheads. I thought it was magic. Years later he admitted he had broken every part on that ship at least once. Experience, he said, is mostly remembering which noise becomes expensive. I caught myself saying it to an apprentice yesterday."
};
static const char *night_answer[]={
 "I could call her. I keep thinking retirement means I should leave her in peace. But she sent me a photograph of her new kettle yesterday. No message, just the kettle. Perhaps that was the riddle. Thank you. I think I finally have the answer.",
 "Maybe home is wherever somebody gives you a chair without asking when you are leaving. I am bringing these pots back to that relay. The gardener never asked for them. I just wanted an excuse to return. Saying it aloud makes it sound less foolish.",
 "That is what the apprentice said: 'You sound like him.' I almost corrected her. Then the pump settled down, and for a moment it felt like three generations were listening to the same engine. I suppose a person can leave you something that is not cargo."
};
static int night_live(void){return night_npc>=0&&night_npc<NPC_COUNT&&game.system==night_system&&!game.docked&&!game.dead&&game.planet<0&&game.jump<=0&&game.npc[night_npc].alive&&game.npc[night_npc].role==TRADERS&&!game.npc[night_npc].freighter&&game.npc[night_npc].target==-1&&length(sub(game.npc[night_npc].pos,game.pos))<12000;}
static void night_close(void){if(night_open||!strncmp(game.voice,night_intro[night_story],sizeof(game.voice)-1))game.voice_time=0;night_npc=-1;night_open=0;night_timeout=0;night_wait=600+(game.rng%301);}
static void night_begin(int npc,int story){night_npc=npc;night_system=game.system;night_story=story%3;night_stage=0;night_open=0;night_timeout=20;night_seen|=1<<night_story;game.encounter_kind=ENCOUNTER_NONE;game.npc[npc].name_known=1;speak(&game,VOICE_CONTACT,night_intro[night_story]);game.voice_role=TRADERS;game.voice_seed=game.system*NPC_COUNT+npc;game.voice_time=20;}
static int night_ready(void){return night_live()&&!night_open&&night_timeout>0&&game.message_time<=0&&game.voice_time>0;}
static void night_tick(float dt){
 if(night_npc>=0){if(!night_live()||game.attacked>0||game.police_stop||game.incoming_missile>0){night_close();return;}if(!night_open){night_timeout-=dt;if(night_timeout<=0||game.voice_time<=0||strncmp(game.voice,night_intro[night_story],sizeof(game.voice)-1))night_close();}return;}
 if(page!=FLIGHT||paused||quiet_comms||tutorial_active(&game)||game.docked||game.dead||game.planet>=0||game.jump>0||game.dock_stage||game.approach>=0||game.police_stop||game.attacked>0||game.incoming_missile>0)return;
 night_wait-=dt;if(night_wait>0||game.voice_time>0||game.message_time>0||game.encounter>0)return;
 night_wait=60;int npc=-1;float near=8000;
 for(int i=0;i<NPC_COUNT;i++){NPC *n=&game.npc[i];float d=length(sub(n->pos,game.pos));if(n->alive&&n->role==TRADERS&&!n->freighter&&n->target==-1&&d<near){near=d;npc=i;}}
 if(npc<0)return;
 if(night_seen==7)night_seen=0;int story=(game.system+(int)game.time)%3;for(int i=0;i<3&&((night_seen>>story)&1);i++)story=(story+1)%3;
 night_begin(npc,story);
}
static void night_panel(void){
 dialogue_begin("QUIET WATCH / PRIVATE CHANNEL");
 const char *body=night_stage==0?night_intro[night_story]:night_stage==1?night_tale[night_story]:night_stage==2?night_answer[night_story]:night_stage==3?game.voice:"Thanks for sharing the watch. I will let you get back to your flight. Clear skies, Commander.";
 dialogue_speech(target_name(NPC_ID_MIN+night_npc),TRADERS,game.voice_seed,body,0);
 dialogue_context("YOUR REPLY",night_stage==3?"Exchange only happens if you explicitly agree, within 2500 m. No credits are charged.":"A quiet conversation. You can leave the channel at any time.");
 dialogue_reply(0,night_stage==0?"I have a little time. Tell me.":night_stage==1?(night_story==0?"Perhaps she misses the second cup too.":night_story==1?"Sounds like someone made room for you.":"Sounds like he is still teaching you."):night_stage==2?"I am glad you called.":night_stage==3?"Agree to the quoted cargo exchange.":"Clear skies. End channel.");
 dialogue_reply(1,night_stage==4?"Thanks for the company.":night_stage==3?"Keep the offer for another time.":"Before you go, any cargo to trade?");dialogue_reply(2,"Sign off. Safe travels.");footer("UP/DOWN   X REPLY   O END CHANNEL");
}
static void night_reply(int choice){
 if(!night_live()){night_close();change_page(FLIGHT);message(&game,"Private channel lost.");return;}
 if(choice==2||night_stage==4||(choice==1&&night_stage==3)){night_close();change_page(FLIGHT);return;}
 if(choice==1){
  if(game.trader_offer_active){night_stage=4;message(&game,"An existing trader offer is already pending.");return;}
  trader_offer_hail(&game,night_npc);night_stage=3;game.message_time=0;row=0;return;
 }
 if(night_stage==3){
  if(length(sub(game.npc[night_npc].pos,game.pos))>2500){message(&game,"Move within 2500 m, then hail this trader for the exchange.");return;}
  if(!game.trader_offer_active||game.trader_offer_npc!=night_npc||game.trader_offer_system!=game.system){night_stage=4;return;}
  trader_offer_hail(&game,night_npc);if(!game.trader_offer_active)night_stage=4;return;
 }
 night_stage=night_stage==2?4:night_stage+1;row=0;
}
