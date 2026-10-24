static int comms_return=FLIGHT,triangle_arm=0,comms_encounter_conversation=0,comms_trade_stage=0;static float triangle_hold=0;
/* The visible channel owns its speaker, text and actions independently of the
 * selected target or a background encounter. Mode 2 is contact-only speech. */
static struct {int kind,npc,system,role,seed;unsigned serial;char name[48],text[160];} comms_channel;
static void comms_channel_capture(int actionable);
#include "quiet-hails.h"
#include "comms-console.h"
static void comms_channel_capture(int actionable){
 static const char *names[]={"CONTACT","KEI","VENN","DOCK CONTROL","LOCAL LAW","COMPUTER","CONTACT"};
 comms_encounter_conversation=actionable?1:2;
 comms_channel.kind=actionable?game.encounter_kind:ENCOUNTER_NONE;
 comms_channel.npc=actionable?game.encounter_npc:-1;comms_channel.system=game.system;
 comms_channel.serial=game.voice_serial;comms_channel.role=game.voice_role;comms_channel.seed=game.voice_seed;
 int who=game.voice_who,role=game.voice_role;if(who<0||who>VOICE_CONTACT)who=0;if(role<0||role>EXPLORERS)role=EXPLORERS;
 const char *name=who==VOICE_CONTACT?(discovery_voice_label()?discovery_voice_label():faction_names[role]):names[who];
 if(actionable&&comms_channel.npc>=0&&comms_channel.npc<NPC_COUNT)name=target_name(NPC_ID_MIN+comms_channel.npc);
 if(actionable&&comms_channel.kind==ENCOUNTER_SMUGGLER&&comms_channel.npc>=0&&comms_channel.npc<NPC_COUNT){static char trader_name[40];trade_contact_name(&game,comms_channel.npc,trader_name,sizeof(trader_name));name=trader_name;}
 if(actionable&&comms_channel.kind==ENCOUNTER_BOUNTY){name="LOCAL BOUNTY NETWORK";comms_channel.role=LAW;comms_channel.seed=game.system*97+31;}
 snprintf(comms_channel.name,sizeof(comms_channel.name),"%s",name);snprintf(comms_channel.text,sizeof(comms_channel.text),"%s",game.voice[0]?game.voice:"Channel open.");
}
static int comms_channel_valid(void){
 if(comms_channel.system!=game.system||comms_channel.serial!=game.voice_serial)return 0;
 if(comms_encounter_conversation==2)return 1;
 if(game.encounter<=0||comms_channel.kind!=game.encounter_kind||comms_channel.npc!=game.encounter_npc)return 0;
 int n=comms_channel.npc;return n<0||(n<NPC_COUNT&&game.npc[n].alive);
}
static void comms_channel_refresh(void){comms_channel_capture(comms_encounter_conversation==1);}
static const char *comms_encounter_reply(int kind,int choice){
 if(choice==2){
  if(kind==ENCOUNTER_PIRATE)return "Close channel. Prepare for their attack.";
  if(kind==ENCOUNTER_SMUGGLER)return "Decline the exchange and close channel.";
  if(kind==ENCOUNTER_DISTRESS||kind==ENCOUNTER_CONVOY)return "Say you cannot assist, then close channel.";
  if(kind==ENCOUNTER_BOUNTY)return "Decline the lead and close channel.";
  if(kind==ENCOUNTER_ESCAPE_POD)return "Say you cannot take them aboard.";
  return "Close the channel.";
 }
 if(kind==ENCOUNTER_PIRATE)return choice?"Ask exactly what they are demanding.":"Refuse. Prepare to defend the ship.";
 if(kind==ENCOUNTER_SMUGGLER)return choice?"Ask how the exchange works.":"Hear the exact cargo offer.";
 if(kind==ENCOUNTER_DISTRESS||kind==ENCOUNTER_CONVOY)return choice?"Ask what they will pay for your help.":"Promise assistance.";
 if(kind==ENCOUNTER_BOUNTY)return choice?"Ask about payout and target threat.":"Accept lead and mark the wanted pilot.";
 if(kind==ENCOUNTER_ESCAPE_POD)return choice?"Ask about their condition.":"Bring the survivor aboard.";
 if(kind==ENCOUNTER_POLICE)return choice?"Ask why you were stopped.":"Acknowledge local law.";
 return choice?"Ask for clearer details.":"Respond to the transmission.";
}
static void comms_encounter_explain(void){
 int kind=comms_channel.kind;const char *line="Say that again in plain terms. What are you asking me to do?";int role=game.voice_role;
 char detail[160];
 if(kind==ENCOUNTER_PIRATE){line="No trade. We want two tonnes jettisoned now. Refuse, and this becomes a fight.";role=PIRATES;}
 else if(kind==ENCOUNTER_SMUGGLER){line="It's a direct cargo swap. I'll state both goods and quantities before anything changes hands. No credits are charged.";role=TRADERS;}
 else if(kind==ENCOUNTER_DISTRESS||kind==ENCOUNTER_CONVOY){
  static const char *urgent[]={
   "They're cutting through my shields and you're negotiating? Help us survive first!",
   "I cannot promise you a unit while the cargo deck is on fire. Please, drive them off!",
   "Seriously? They're shooting at us. If we live, you can argue with our insurer.",
   "Drive them off first. If the cargo arrives, I will make sure the company hears who saved it."
  };
  line=urgent[(unsigned)comms_channel.seed&3];role=TRADERS;
 }
 else if(kind==ENCOUNTER_BOUNTY){
  int n=comms_channel.npc,reward=150,threat=1;
  if(n>=0&&n<NPC_COUNT){NPC *target=&game.npc[n];if(target->bounty_slot>=0)reward=bounty_target_reward(&game,target->bounty_slot);threat=(int)((target->health+target->shield)/70.f);if(threat<1)threat=1;if(threat>5)threat=5;}
  snprintf(detail,sizeof(detail),"Verified payout: %.1f units. Target threat: %d of 5. Accept the lead to mark the ship; payment follows the confirmed takedown.",reward*.1f,threat);line=detail;role=LAW;
 }
 else if(kind==ENCOUNTER_ESCAPE_POD){line="Life support is failing. Bring the pod aboard and deliver the survivor to the nearest station.";role=TRADERS;}
 else if(kind==ENCOUNTER_POLICE){line="This is a lawful local stop. Hold position while we verify your ship and cargo record.";role=LAW;}
 encounter_speak(&game,kind==ENCOUNTER_POLICE?VOICE_LAW:VOICE_CONTACT,line);game.voice_role=role;comms_channel_refresh();
}
/* Declining must mean what the reply says. Pirates treat a closed channel as
 * refusal and attack; optional callers acknowledge the answer, then release
 * their encounter state so it cannot be triggered later by accident. */
static void comms_encounter_decline(void){
 int kind=comms_channel.kind,role=comms_channel.role;
 if(kind==ENCOUNTER_PIRATE){encounter_respond(&game);return;}
 encounter_ignore(&game);
 const char *line=kind==ENCOUNTER_DISTRESS||kind==ENCOUNTER_CONVOY?"Understood. We will keep broadcasting. Stay safe, Commander.":
  kind==ENCOUNTER_BOUNTY?"Lead declined. The warrant remains open to other pilots.":
  kind==ENCOUNTER_ESCAPE_POD?"Understood. I will keep the beacon transmitting. Please send help if you can.":
  kind==ENCOUNTER_SMUGGLER?"No deal. Nothing changes hands. Safe flight, Commander.":
  "Understood. Closing the channel.";
 speak(&game,kind==ENCOUNTER_BOUNTY?VOICE_LAW:VOICE_CONTACT,line);game.voice_role=kind==ENCOUNTER_BOUNTY?LAW:role;
}
static void comms_panel(void){
 if(night_open&&night_live()){night_panel();return;}
 if(comms_encounter_conversation){
  dialogue_begin(comms_encounter_conversation==1?"INCOMING CHANNEL":"CONTACT CHANNEL");
  dialogue_speech(comms_channel.name,comms_channel.role,comms_channel.seed,comms_channel.text,0);
  if(!comms_channel_valid()){dialogue_context("SIGNAL LOST","This channel is no longer available.");dialogue_reply(0,"Close the channel.");}
  else if(comms_encounter_conversation==2){dialogue_reply(0,"Close the channel.");}
  else if(comms_channel.kind==ENCOUNTER_SMUGGLER&&comms_trade_stage){
   if(comms_trade_stage==1){
    int need=game.trader_offer_need,reward=game.trader_offer_reward,qty=game.trader_offer_qty;
    char offer[192];snprintf(offer,sizeof(offer),"Offer unavailable. Close the channel to return.");
    if(need>=0&&need<GOODS&&reward>=0&&reward<GOODS)snprintf(offer,sizeof(offer),"Give %d%c %s. Receive %d%c %s.",qty,goods[need].unit,goods[need].name,qty,goods[reward].unit,goods[reward].name);
    dialogue_context("PROPOSED CARGO EXCHANGE",offer);
    dialogue_reply(0,"Accept and add this exchange to my Mission Log.");dialogue_reply(1,"Decline. No appointment and no cargo transfer.");
   }else dialogue_reply(0,"End transmission and return to flight.");
  }else if(comms_encounter_conversation){
   for(int i=0;i<3;i++)dialogue_reply(i,comms_encounter_reply(comms_channel.kind,i));
  }else{
   dialogue_reply(0,"Respond");dialogue_reply(1,"Ignore");
  }
  footer("UP/DOWN   X CHOOSE   O RETURN");return;
 }
 comms_console_draw();
}
static void comms_trade_depart(void){int n=game.encounter_npc;if(n>=0&&n<NPC_COUNT&&game.npc[n].alive){game.npc[n].target=-1;game.npc[n].waypoint=(game.npc[n].waypoint+1)%5;game.npc[n].route_wait=0;}game.encounter_kind=ENCOUNTER_NONE;game.encounter=0;comms_encounter_conversation=comms_trade_stage=0;change_page(comms_return);}
