static void home(void){
 int group=deck_group(row);
 header(game.docked?"STATION / COMMAND DECK":"COCKPIT / PAUSED");
 for(int i=0;i<5;i++){int x=8+i*94;rect(x,30,90,20,i==group?RGB(25,65,77):RGB(10,23,33));if(i==group)rect(x,48,90,2,GOLD);text((x+8)/8,4,i==group?WHITE:DIM,"%s",deck_groups[i]);}
 const char *labels[]={game.docked?"Launch":"Resume flight","Cargo & market","Galaxy map","Shipyard","Outfitting","Save / status","Controls","Factions","Targeting computer","Debug tools","Comms / docking","System details","Mission board","Mission log","GalacticNet","Discovery Codex","Radio & audio","Tracked mission","Explorers Guild","Display & chatter","Disembark / walk station","Ship loadout"};
 static const char *hints[][2]={
 {"Return to your cockpit.","Fly at your own pace."},{"Your hold and local goods.","Station prices while docked."},{"Choose your next system.","Check range before jumping."},
 {"Compare and exchange hulls.","Requires station services."},{"Only what this hub stocks.","No locked tech teases."},{"Save, load and view records.","Save at a station."},
 {"Flight, travel and shortcuts.","Four short reference pages."},{"Meet the local factions.","Learn their colours and roles."},{"Select ships, worlds or hub.","Choose a target to follow."},
 {"Change cash or world state.","Debug changes affect saves."},{"Hail the station or get help.","Request guided docking."},{"Economy, risk and planets.","Know where you are flying."},
 {"Find work at this station.","Dock to accept a contract."},{"Review jobs and set a route.","Reading pauses job clocks."},{"News and local SpaceBook.","Take a break from flying."},
 {"Review your discoveries.","Keep a record of your travels."},{"Five offline music stations.","Set music and effects levels."},{"Guide for the tracked mission.","Choose tracking in Mission Log."},
 {"Optional Guild assignments.","Also listed in Mission Log."},{"Choose HUD and text chatter.","Keep the view comfortable."},{"Illustrated station rooms.","LOOK SPEAK GO TAKE on hotspots."},{"Fitted slots and cargo list.","See what your ship carries."}};
 panel(8,58,222,132);panel(238,58,234,132);
 for(int i=0;i<deck_sizes[group];i++){int id=deck_rows[group][i],y=8+i*2;int locked=!game.docked&&(id==3||id==4||id==12||id==20);
  if(id==row){rect(10,y*8-2,218,15,RGB(25,65,77));rect(10,y*8-2,3,15,GOLD);}
  int story_row=id==17&&(game.campaign_stage<6||game.guild_chapter<4||game.job_n>0);unsigned ink=id==row?WHITE:locked?DIM:story_row?GOLD:WHITE;
  text(3,y,ink,"%s%s%.21s",id==row?">":" ",story_row?"! ":"",labels[id]);
 }
 /* Top-right: live third-person ship in local space. */
 menu_space_view(246,64,218,92);
 int is_story=row==17&&(game.campaign_stage<6||game.guild_chapter<4||game.job_n>0);
 text(31,20,is_story?GOLD:GOLD,"%.27s",labels[row]);
 text(31,22,WHITE,"%s",hints[row][0]);
 if(!game.docked&&(row==3||row==4||row==12||row==20))text(31,23,AMBER,"Dock first to open this.");
 if(is_story){rect(246,178,218,2,GOLD);text(31,23,GOLD,"Open to see your next step.");}
 text(2,25,CYAN,"System: %.12s",game.systems[game.system].name);
 {int wl=wanted_level(&game);text(2,26,wl?RED:DIM,wl?"Wanted [%s]":"Clear warrant",stars(wl));}
 text(31,25,WHITE,"%.1f units   %s",game.credits*.1f,game.docked?"DOCKED":"PAUSED");
 footer("LEFT/RIGHT TAB   UP/DOWN   X OPEN   O BACK");
}
static void help(void){
 static const char *titles[]={"FLIGHT","TARGETS / TRAVEL","STATIONS / SURFACES","MENUS / COMFORT"};
 static const char *keys[][6]={{"D-pad / analog","R / L","2xR boost / 2xL brake","L + Left / Right","X / L + X","Start + D-pad"},
 {"Tap Square","Square + Left/Right","Square + Up/Down","Square + R","Steer manually","Circle"},
 {"Hold Triangle","Circle near a hub","Circle near a world","X / Circle prompt","Circle on surface","Triangle on surface"},
 {"Select in flight","Left / Right on deck","X / Circle","Hold Triangle","L + Select in flight","L on this screen"}};
 static const char *actions[][6]={{"Steer the ship","Accelerate / slow down","Boost or hard brake","Roll the ship","Laser / locked missile","Power banks SYS/ENG/WEP"},
 {"Open target list","Change target category","Choose target","Lock + auto-turn","Cancel auto-turn","Use the object ahead"},
 {"Comms and docking request","Request guided docking","Ask to approach","Enter / turn away","Land or leave the ship","Take off / return to orbit"},
 {"Open the paused deck","Change service category","Open / return","Chatter and comms options","Full / minimal / scenic HUD","Toggle analog steering"}};
 header("COMMANDER / CONTROLS");panel(8,32,464,190);
 text(3,5,GOLD,"%d / 4   %s",help_tab+1,titles[help_tab]);
 for(int i=0;i<6;i++){int y=8+i*3;text(3,y,CYAN,"%s",keys[help_tab][i]);text(29,y,WHITE,"%s",actions[help_tab][i]);}
 text(3,29,DIM,"Analog %s. Centre the nub after enabling.",analog_enabled?"ON":"OFF");
 footer("LEFT/RIGHT PAGE   L ANALOG ON/OFF   O BACK");
}
static void comfort_screen(void){
 header("COMMANDER / DISPLAY & CHATTER");panel(8,32,464,156);
 const char *labels[]={"HUD layout","Text chatter","High contrast focus","Radio and audio","Controls"};
 for(int i=0;i<5;i++){int y=6+i*3;if(row==i)selected(y);text(3,y,WHITE,"%s",labels[i]);}
 text(34,6,CYAN,"%s",hud_mode==0?"FULL":hud_mode==1?"MINIMAL":"SCENIC");
 text(34,9,CYAN,"%s",quiet_comms?"QUIET":"ON");
 text(34,12,CYAN,"%s",high_contrast?"ON":"OFF");
 text(3,23,DIM,"Safety prompts stay visible. HUD lasts this session.");
 footer("UP/DOWN   X CHANGE / OPEN   O BACK");
}
/* Action feedback gets its own reserved band, not a talking-character card. */
static void menu_notice(void){
 if(game.message_time<=0||!game.message[0])return;
 int narrative=page==CAMPAIGN||page==GUILD||page==STORY;
 int y=page==HOME?216:narrative?224:192;int h=narrative?24:32;
 rect(8,y,464,h,RGB(15,30,40));rect(8,y,3,h,GOLD);
 text_wrap(2,(y+2)/8,56,narrative?2:3,GOLD,game.message,0);
}
