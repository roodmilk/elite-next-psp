/* Rebuild the correspondence archive from durable story progress. No new save
 * fields, fabricated message timestamps or future chapter disclosures. */
enum { INBOX_SAGA,INBOX_FIRST_LIGHT,INBOX_DOCK,INBOX_HOME,INBOX_HOMEWARD,INBOX_RECEIPT,INBOX_ERL };
static int inbox_saga_count(void){return game.campaign_stage>=6?fminf(SAGA_COUNT,game.saga_chapter+1):0;}
static int inbox_light_count(void){return opening_active(&game)?0:1+(int)fminf(6,fmaxf(0,game.campaign_stage));}
static int inbox_homeward_count(void){return opening_exists(&game)?(game.opening.stage>=OP_GIFT)+(game.opening.stage>=OP_WAKE):0;}
static int story_inbox_count(void){
 return !!(game.erl.inbox&1)+!!(game.erl.inbox&2)+!!(game.erl.inbox&4)+inbox_saga_count()+inbox_light_count()+inbox_homeward_count()+(opening_exists(&game)&&game.opening.stage>=OP_LAVE_DOCK)+(opening_exists(&game)?5:0)+(tutorial_active(&game)?1:0);
}
static int story_inbox_record(int row,int *index){
 for(int bit=2;bit>=0;bit--)if(game.erl.inbox&(1<<bit)){if(!row){*index=bit;return INBOX_ERL;}row--;}
 int n=inbox_saga_count();if(row<n){*index=n-1-row;return INBOX_SAGA;}row-=n;
 n=inbox_light_count();if(row<n){*index=n-1-row;return INBOX_FIRST_LIGHT;}row-=n;
 if(opening_exists(&game)&&game.opening.stage>=OP_LAVE_DOCK){if(!row){*index=0;return INBOX_DOCK;}row--;}
 if(opening_exists(&game)&&row<5){*index=row;return INBOX_HOME;}row-=opening_exists(&game)?5:0;
 n=inbox_homeward_count();if(row<n){*index=n-1-row;return INBOX_HOMEWARD;}
 *index=0;return INBOX_RECEIPT;
}
static void story_inbox_text(int row,char *author,int acap,char *subject,int scap,char *body,int bcap){
 int index=0,kind=story_inbox_record(row,&index);
 if(kind==INBOX_ERL){int sys=game.erl.ref_system>=0?game.erl.ref_system:7;snprintf(author,acap,"%s",index?erl_organiser(&game,sys):"TORR BELL");snprintf(subject,scap,"%s",index==0?"Your racing referral":index==1?"Welcome to the league":"Championship result");if(index==0)snprintf(body,bcap,"Lovely flying, Commander. I have sent your referral to %s at %s Mega Capital. They can arrange your qualifier. Win it and you will earn a rookie racing licence.\n\nOpen Mission Log for the route. No entry fees, and you can practise as often as you like.",erl_organiser(&game,sys),game.systems[sys].name);else if(index==1)snprintf(body,bcap,"Congratulations on earning your racing licence. Our headquarters runs four-round championships, all in one system. Bring your own ship; we regulate performance to give everyone a fair race.\n\nYour ship stays registered for the season. Earn XP, win the next qualifier, and you can race in stronger leagues. Your permanent prize ships wait in the ERL Garage.");else snprintf(body,bcap,"Your latest championship is complete. You finished %d out of seven drivers. The standings, units and XP are recorded.\n\nAny new prize ship is waiting in your ERL Garage. Claim your exclusive module at headquarters with Square; if Item Storage is full, we will keep it safe until you have room.",game.erl.season_place);
 }else if(kind==INBOX_SAGA){const SagaBeat *b=&saga_beats[index];
  snprintf(author,acap,"%s",b->speaker);snprintf(subject,scap,"Chapter %02d: %s",index+2,b->title);
  if(index<game.saga_chapter)snprintf(body,bcap,"%s\n\nFiled briefing: %s\n\nThis chapter is complete. Your next step is in Mission Log.",b->line,b->objective);
  else snprintf(body,bcap,"%s\n\n%s\n\nOpen Mission Log to follow the current objective.",b->line,b->objective);
 }else if(kind==INBOX_FIRST_LIGHT){
  static const char *subject_line[]={"A berth and a missing pilot","Your first flight","Cleared for departure","Keep the hub in sight","Coming home","Report to Kei","Harbour badge earned"};
  static const char *letters[]={
   "I'm Kei Aven. Ryn has missed her check-ins, and I could use someone I can trust. I've kept a berth for you at Lave Hub. Come and talk when you're ready.",
   "Before we go looking for Ryn, let's make sure you feel at home in that cockpit. Launch when you're ready. I'll stay on the channel.",
   "You're clear of the berth. Select Lave Hub in your targeting computer, fly a short distance and bring the ship back safely. There's no need to rush.",
   "Good. Keep the hub selected and fly at least 600 metres from your launch point. Then turn back towards the station. I'll be here.",
   "When you're close enough to the hub, call the station with Triangle and request docking. Let the controller guide you in if you'd rather take it steady.",
   "You made it back. Come and see me at Lave Hub; I'd like to hear how the ship handled before we talk about Ryn.",
   "Harbour badge earned, Commander. You've shown me you can get out and get home. Now we can start following Ryn's trail. The next briefing is waiting in Mission Log."};
  snprintf(author,acap,"Kei Aven");snprintf(subject,scap,"First Light: %s",subject_line[index]);snprintf(body,bcap,"%s",letters[index]);
 }else if(kind==INBOX_HOME){
  static const char *people[]={"Dad","Mum","Kat Herrington","Scott Winters","Flight Journal"};
  static const char *titles[]={"Big day tomorrow","Back for dinner","First ship day!","Friday's photo","Your first ship"};
  static const char *letters[]={
   "Big day tomorrow. The dealer opens at nine. Get some sleep!\n\nYou: Can't sleep. I keep thinking about the ship.\n\nDad: We'll take it steady. I'm coming with you.",
   "Have a lovely first flight, both of you.\n\nYou: We'll be back for dinner.\n\nMum: Pizza, please. Tesco is closed, remember!",
   "First ship day! Please tell me your dad's driving. Send a photo when you're up there.\n\nYou: I'll send a picture.\n\nKat: Don't forget us when you're famous. Same place next Friday? Scott is banned from choosing the music.",
   "Dude, that party was sick. Photo from Friday attached. Kat still reckons she could have landed that thing.\n\nYou: Next Friday again?\n\nScott: Obviously. Tell us about the ship when you're back.",
   "Personal flight journal\n\nFinally getting my own ship today. Dad's coming for the first flight. Back for dinner!\n\nA saved note from home, kept with your private correspondence."};
  snprintf(author,acap,"%s",people[index]);snprintf(subject,scap,"%s",titles[index]);snprintf(body,bcap,"%s",letters[index]);
 }else if(kind==INBOX_HOMEWARD){snprintf(author,acap,"Ship Computer");snprintf(subject,scap,"%s",index?"An unfamiliar sky":"The object from the clearing");snprintf(body,bcap,"%s",index?"You woke alone after the attack. Dad is missing, and the stars do not match the chart from home. Earth messages are cached, but the relay cannot deliver your replies. Keep the unknown object safe. Find a friendly port, meet local people and look for a way back.":"You and Dad found a glowing object in the clearing. You returned to the ship with it. Its purpose is still unknown. Keep it safe while you find out what it is.");
 }else if(kind==INBOX_DOCK){snprintf(author,acap,"Lave Dock Services");snprintf(subject,scap,"Welcome to Lave Hub");snprintf(body,bcap,"Your berth is confirmed. Repairs, fuel and outfitting are available at the hub. Kei has asked us to keep the berth open for you. If you need a hand, ask at the dock.");
 }else{snprintf(author,acap,"Venn");snprintf(subject,scap,"Ryn's receipt");snprintf(body,bcap,"Kei kept the berth. The loader kept the receipt: 'If I miss supper, ask who kept the light on.' Lave. Two tonnes of food. Check the station people, not just the screens.");}
}
