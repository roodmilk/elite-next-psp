/* Authored Lave story rides the same seven-room graph and seeded art kit. */
static const char *lave_names[]={"VENN","ADA","NIKO","MARA","IONA","DR SEN","ORIN"};
static const char *lave_lines[]={
 "Welcome aboard. If you are waiting for the berth traffic to clear, you might be here a little while. Mara has a shipment stuck in Cargo and she has already had three people tell her it is somebody else's problem. She could use a pilot who is willing to listen before offering advice. Take the corridor through the shop or the canteen; both lead down to the bay. There is no rush to spend your units here. Get your bearings first.",
 "You are looking at the seals? They do not seem important until one is missing. After that, everybody who touches the crate wants proof that the last person left it alone. I keep replacements, but I record which shipment each one goes to. If this is about Mara's medicine crate, tell me what happened to it and I can help you put the record straight. Dr Sen will still need to verify the contents; that part is not something I can sign for.",
 "Yes, I remember that crate. A coolant pipe started dripping over the loading route, so we moved it before the packaging got soaked. There was no secret handover and nobody was trying to hide anything. The seal caught on the rail, and by the time we had dealt with the leak the paperwork had gone astray. I should have checked before the shift ended. Ada keeps replacement seals in the shop; explain what happened and ask her to log one against the manifest.",
 "Have you got a moment? That medicine shipment is supposed to leave through Berth Six, but the manifest has disappeared and I cannot release it on somebody's memory. I have checked the containers twice. What I need now is a fresh pair of eyes around the rail and the cargo records. If you find the sheet, we can work out who last handled the crate and get the clearance sorted. I can pay sixty units when it is ready to move. Would you be willing to help?",
 "Mara told me what you did for the shipment. Thank you. I have a different sort of job, if you are heading down to Lave I: bring back a recorded scan of a lifeform or mineral from the surface. We need observations we can compare with the archive, not something that merely looks unusual from orbit. An existing scan from that world is fine. Once I can verify the record, the Guild can pay you ninety units.",
 "Let me see the shipment details. These lot numbers belong to our medicine order; they are not restricted cargo disguised under a convenient label. I can verify that in writing, but the replacement seal needs to be recorded first or Customs will send you straight back. Talk to Ada if you have not already done that. Once the records agree, bring them here and I will add my confirmation. We have all lost enough time over this crate.",
 "I know the shipment you mean. At the moment I have a broken seal, an incomplete movement record and several people assuring me that everything is fine. That may be true, but it is not a clearance. Get the seal logged by the Chandlery and have Dr Sen verify the medicine lot. With those two records attached, I can stamp the manifest and Mara can release the crate. I am not asking you to solve the whole station's paperwork, just this one shipment."
};
static const char *lave_offers[]={"Where should I go next?","Request replacement seal","Ask about the missing crate","Help with Berth Six","Upload Lave I survey","Verify medicine list","Request cargo clearance"};
static const char *lave_line(int room){
 int stage=sc_lave_stage();
 if(room==SC_R_CARGO&&stage>=7)return "Berth Six is working again. Those medicines will reach the farms before the next shift. You did good work.";
 if(room==SC_R_CARGO&&stage==6)return "That is Orin's stamp. Finally! Hand it over and I can release the crate. Your sixty units are ready.";
 if(room==SC_R_GUILD&&stage<7)return "I map the worlds that feed this station. Help Mara clear that shipment first; then we can talk fieldwork.";
 if(room==SC_R_GUILD&&stage==9)return "Your Lave I record is in the archive. A pilot who notices small things is worth more than a hundred empty reports.";
 if(room==SC_R_SHOP&&stage>=4)return "The replacement seal is logged against your manifest. Dr Sen can verify the medicine lot, then Customs can clear it.";
 if(room==SC_R_CANTEEN&&stage>=3)return "The crate was moved to keep it out of a leaking coolant pipe. No theft, just a broken seal and terrible paperwork.";
 if(room==SC_R_CLINIC&&stage>=5)return "The lot numbers match our order. You have my verification. Customs should release the shipment now.";
 if(room==SC_R_CUSTOMS&&stage>=6)return "Shipment cleared. Tell Mara the medicine can move. A clean paper trail saves everybody another argument.";
 return lave_lines[room];
}
static const char *lave_offer(int room){
 int stage=sc_lave_stage();
 if(room==SC_R_CARGO)return stage==0?"I'll help find the manifest":stage==6?"Hand over cleared manifest":stage>=7?"Ask about the next lead":"Review the missing paperwork";
 if(room==SC_R_GUILD)return stage<7?"Ask about survey work":stage==7?"Accept the Lave I survey":stage==9?"Review completed fieldwork":"Upload Lave I survey";
 return lave_offers[room];
}
static int lave_npc_action(int act){return act>=40&&act<47;}
static void lave_action(int room){
 int stage=sc_lave_stage();game.cue=SFX_SELECT;
 if(room==SC_R_CARGO){
  if(stage==0){sc_lave_set(1);tracked_mission=TRACK_LAVE;}
  else if(stage==6){sc_lave_set(7);sc_manifest_set(2);game.credits+=600;message(&game,"Berth Six cleared. Mara pays 60 U. Iona at the Guild has a follow-up.");return;}
  else if(stage>=7){message(&game,"Mara: the medicine shipment is moving. Thank you. Guild has the next lead.");return;}
 }else if(room==SC_R_CANTEEN&&stage==2)sc_lave_set(3);
 else if(room==SC_R_SHOP&&stage==3)sc_lave_set(4);
 else if(room==SC_R_CLINIC&&stage==4)sc_lave_set(5);
 else if(room==SC_R_CUSTOMS&&stage==5)sc_lave_set(6);
 else if(room==SC_R_GUILD){
  if(stage==7){sc_lave_set(8);tracked_mission=TRACK_LAVE;message(&game,"Survey Lave I. Scan one lifeform or mineral, then bring Iona your record.");return;}
  if(stage==8){
   if(game.surface_progress[7][1]&0x0fff00u){sc_lave_set(9);game.credits+=900;message(&game,"Survey uploaded. 90 U paid. Berth Six and the Lave field report are complete.");return;}
   message(&game,"Iona needs a scan from Lave I. Archived scans count; visit the surface Codex.");return;
  }
 }
 message(&game,sc_lave_objective());
}
/* Full spoken responses to accepted actions; concise game messages stay as HUD summaries. */
static const char *lave_response(int room,int before){
 int after=sc_lave_stage();
 if(room==SC_R_CARGO&&before==0&&after==1)return "Thank you. Start beside the rail and work outwards from there; loose sheets have a habit of ending up where nobody thinks to look. If you find the manifest, check which shift last handled the medicine crate. Somebody must remember why it was moved. I will stay here and keep the rest of the bay working. You do not need to carry the shipment yourself, just help us get its record straight.";
 if(room==SC_R_CANTEEN&&before==2&&after==3)return "That is the sheet, then. Good. Tell Ada the seal was damaged when we moved the crate away from the coolant leak. She will want to record the replacement against the manifest rather than just hand you an unmarked spare. I know that sounds fussy, but it means the next person can see what happened without having to find everyone who was on shift. And tell Mara I am sorry I left her to untangle it.";
 if(room==SC_R_SHOP&&before==3&&after==4)return "Here you are. I have logged the replacement seal against the shipment, so there should not be any confusion about why the original is missing. Do not take it straight to Customs yet. Dr Sen needs to check the medicine lot and add the clinic's verification. After that, Orin should have everything he needs to clear it. You have done the running around; let us make sure the record actually saves you another trip.";
 if(room==SC_R_CLINIC&&before==4&&after==5)return "Yes, these lot numbers match our order. I have added my verification to the record. Nothing here suggests the contents were substituted; the problem was the handling record, not the medicine. Take this to Orin in Customs and ask him to clear the shipment. Mara will be relieved to see it moving again. So will the people who are waiting for it, even if they never hear about this little detour.";
 if(room==SC_R_CUSTOMS&&before==5&&after==6)return "The seal replacement is recorded, and Dr Sen has verified the lot. That is what I needed. I have stamped the clearance; take the manifest back to Mara and she can release the shipment. Thank you for bringing the complete record rather than asking me to ignore the missing pieces. It may not feel like much of an achievement, but there will be fewer questions at the next stop because you took the trouble here.";
 if(room==SC_R_CARGO&&before==6&&after==7)return "There it is. Orin's clearance, the clinic's verification, the replacement seal - everything in one place at last. I can release the crate now. Your sixty units have been transferred, as promised. You saved my crew another round of chasing people through the station. If you are looking for something beyond paperwork, speak to Iona at the Guild desk. She mentioned needing a pilot who could bring back a useful observation.";
 if(room==SC_R_GUILD&&before==7&&after==8)return "All right, let us make it a proper assignment. Visit Lave I and record a scan of a lifeform or mineral on the surface. One good record is enough for this job; I am not asking you to catalogue an entire world. If you already have a scan from Lave I in your archive, we can use that instead. Bring the record back here when you are ready, and I will check it before authorising the ninety-unit payment.";
 if(room==SC_R_GUILD&&before==8&&after==9)return "That record is from Lave I, and it contains what we need. I have accepted it into the archive and transferred your ninety units. Thank you for bringing back something we can actually compare with the other surveys. The manifest work kept a shipment moving; this is the quieter sort of work that helps us understand the places supplying it. You have finished both jobs. The next journey can be entirely your own.";
 if(room==SC_R_GUILD&&before==8)return "I cannot find a qualifying Lave I scan in the record yet. Check that it is a lifeform or mineral scanned on that world's surface, rather than a sighting from space or a discovery in another system. There is no need to rush back out if you are still preparing your ship. I will be here when you have something to upload, and an older record from Lave I is just as useful for this assignment.";
 return lave_line(room);
}

