static void home(void){
 int group=deck_group(row);deck_clamp_row();group=deck_group(row);
 header(game.docked?"STATION / COMMAND DECK":"COCKPIT / PAUSED");
 for(int i=0,col=0;i<5;i++){int visible[6];if(!deck_fill(i,visible))continue;int x=8+col++*94;rect(x,30,90,20,i==group?RGB(41,54,70):RGB(21,28,39));if(i==group)rect(x,48,90,2,RGB(240,180,91));text((x+8)/8,4,i==group?RGB(229,210,163):RGB(155,154,165),"%s",deck_groups[i]);}
 const char *labels[]={game.docked?(tutorial_active(&game)?"FLY / Launch":"Launch"):"Resume flight",game.docked?"Cargo & market":"Cargo","Galaxy map","Shipyard","Outfitting","Save / status","Controls","Factions","Targeting computer","Debug tools","Comms panel","System details","Mission board","Mission log","GalacticNet","Discovery Codex","Radio & audio","Tracked mission","Explorers Guild","Display & chatter","Disembark","Ship loadout","Galactic Lore","Ship decorator","Engineers"};
 static const char *hints[][2]={
 {"FLY INTO SPACE!","Fly at your own pace."},{"Your hold and local goods.","Station prices while docked."},{"Choose your next system.","Check range before jumping."},
 {"BUY NEW SHIPS!","Requires station services."},{"GET SHIP UPGRADES!","No locked tech teases."},{"Save, load, records.","Save at a station."},
 {"view game controls","Four short reference pages."},{"Meet the local factions.","Learn their colours and roles."},{"Select ships or worlds.","Choose a target to follow."},
 {"Change cash or world state.","Debug changes affect saves."},{"Hail station or get help.","Request guided docking."},{"Economy, risk and planets.","Know where you are flying."},
 {"Find work at this station.","Dock to accept a contract."},{"Review jobs and route.","Reading pauses job clocks."},{"News and local SpaceBook.","Take a break from flying."},
 {"Review your discoveries.","Keep a record of your travels."},{"cruise to sweet tunes!","Set music and effects levels."},{"Guide for tracked mission.","Choose tracking in Mission Log."},
 {"Optional Guild assignments.","Also listed in Mission Log."},{"Choose HUD and chatter.","Keep the view comfortable."},{"SEE WHATS AROUND!","LOOK SPEAK GO TAKE on hotspots."},{"SEE WHATS ON YOUR SHIP","See what your ship carries."},{"A readable history of the galaxy.","Explore the major eras, peoples and powers."},{"REPAINT YOUR HULL","Choose a finish or pattern."},{"RESTORE SHIP CONDITION","Station engineers repair damage for a fee."}};
 panel(8,58,222,132);panel(238,58,234,132);
 int vis[6],vn=deck_fill(group,vis);
 for(int i=0;i<vn;i++){int id=vis[i],y=8+i*2;int locked=!game.docked&&id==20;
  if(id==row){rect(10,y*8-2,218,15,RGB(41,54,70));rect(10,y*8-2,3,15,RGB(240,180,91));}
  int story_row=id==17&&(game.campaign_stage<6||game.guild_chapter<4||game.job_n>0);unsigned ink=id==row?RGB(229,210,163):locked?RGB(155,154,165):story_row?RGB(240,180,91):RGB(229,210,163);
  text(3,y,ink,"%s%s%.21s",id==row?">":" ",story_row?"! ":"",labels[id]);
 }
 /* Top-right: live third-person ship in local space. */
 menu_space_view(246,64,218,92);
 int is_story=row==17&&(game.campaign_stage<6||game.guild_chapter<4||game.job_n>0);
 text(31,20,RGB(240,180,91),"%.27s",labels[row]);
 /* The detail pane is 27 native columns wide; keep future copy inside it. */
 text(31,22,RGB(229,210,163),"%.27s",hints[row][0]);
 if(!game.docked&&row==20)text(31,23,RGB(240,180,91),"Dock first to open this.");
 if(is_story){rect(246,178,218,2,RGB(193,139,77));text(31,23,RGB(240,180,91),"Open to see your next step.");}
 text(2,25,RGB(85,212,212),"System: %.12s",game.systems[game.system].name);
 {int wl=wanted_level(&game);text(2,26,wl?RED:RGB(155,154,165),wl?"Wanted [%s]":"Clear warrant",stars(wl));}
 text(31,25,RGB(229,210,163),"%s",game.docked?"DOCKED":"PAUSED");credits_badge();
 footer("");
}
static void help(void){
 static const char *titles[]={"FLIGHT","TARGETS / TRAVEL","STATIONS / SURFACES","MENUS / COMFORT","PLANET EVA"};
 static const char *keys[][6]={{"D-pad / analog","R / L","2xR boost / 2xL brake","L + Left / Right","X / L + X","Start + D-pad"},
 {"Tap / hold Square","Square + Left/Right","Square + Up/Down","Square + R","Steer manually","Circle"},
 {"Hold Triangle","Circle near a hub","Circle near a world","X / Circle prompt","Circle on surface","Triangle on surface"},
 {"Select in flight","Left / Right on deck","X / Circle","Hold Triangle","L + Select in flight","L on this screen"},
 {"Nub / L + D-pad","D-pad Up / Down","D-pad Left / Right","Hold R / release","Triangle / Circle","Square"}};
 static const char *actions[][6]={{"Steer the ship","Accelerate / slow down","Boost or hard brake","Roll the ship","Laser / locked missile","Power banks SYS/ENG/WEP"},
 {"Reticle target / browser","Change target category","Choose target","Lock + auto-turn","Cancel auto-turn","Use the object ahead"},
 {"Comms and docking request","Request guided docking","Ask to approach","Enter / turn away","Land or leave the ship","Take off / return to orbit"},
 {"Open the paused deck","Change service category","Open / return","Chatter and comms options","Full / minimal / scenic HUD","Toggle analog steering"},
 {"Look (L stops walking)","Walk forward / back","Strafe left / right","Jet lift / fall","Face ship / board on ground","Survey nearby life"}};
 header("COMMANDER / CONTROLS");panel(8,32,464,190);
 text(3,5,RGB(240,180,91),"%d / 5   %s",help_tab+1,titles[help_tab]);
 for(int i=0;i<6;i++){int y=8+i*3;text(3,y,RGB(85,212,212),"%s",keys[help_tab][i]);text(29,y,RGB(229,210,163),"%s",actions[help_tab][i]);}
 text(3,29,RGB(155,154,165),"Analog %s. Centre the nub after enabling.",analog_enabled?"ON":"OFF");
 footer("LEFT/RIGHT PAGE   L ANALOG ON/OFF   O BACK");
}
static void comfort_screen(void){
 header("COMMANDER / DISPLAY & CHATTER");panel(8,32,464,156);
 const char *labels[]={"HUD layout","Text chatter","High contrast focus","Radio and audio","Controls","Third-person view"};
 for(int i=0;i<6;i++){int y=6+i*3;if(row==i)selected(y);text(3,y,RGB(229,210,163),"%s",labels[i]);}
 text(34,6,RGB(85,212,212),"%s",hud_mode==0?"FULL":hud_mode==1?"MINIMAL":"SCENIC");
 text(34,9,RGB(85,212,212),"%s",quiet_comms?"QUIET":"ON");
 text(34,12,RGB(85,212,212),"%s",high_contrast?"ON":"OFF");
 text(34,21,RGB(85,212,212),"%s",third_person?"ON":"OFF");
 text(3,23,RGB(155,154,165),"Safety prompts stay visible. HUD lasts this session.");
 footer("UP/DOWN   X CHANGE / OPEN   O BACK");
}
static void decorator_screen(void){
 static const char *names[]={"ORIGINAL GOLD","COCKPIT CYAN","SUNSET RED","DEEP VIOLET","EXPLORER GREEN","SOLAR AMBER","NEON GRID","STARFALL THEME"};
 static const int fees[]={120,180,240,320,400,520,700,900};
 int finish=row<0?0:row>7?7:row;
 decorator_monitor_draw();
 /* The artwork contains its own screen frame, browser bar, icons and ad.
  * Dynamic UI is kept within the two intentionally empty screen windows. */
 text_px(108,26,RGB(210,235,229),"PIMP-MY-SHIP.NET");
 text(14,11,RGB(106,215,218),"PAINT FINISHES");
 for(int i=0;i<8;i++){
  int y=(13+i)*8;
  if(i==finish){rect(111,y-1,130,10,RGB(42,76,88));rect(111,y-1,2,10,RGB(246,186,92));}
  if(ship_paint[game.ship]==decorator_finishes[i]){rect(114,y-1,12,10,RGB(246,186,92));rect(116,y+1,8,6,decorator_finishes[i]);pixel(109,y+2,RGB(246,186,92));pixel(108,y+3,RGB(246,186,92));pixel(109,y+4,RGB(246,186,92));}
  else rect(116,y+1,8,6,decorator_finishes[i]);
  text(16,13+i,i==finish?WHITE:RGB(171,194,198),"%s",names[i]);
 }
 preview_clip(329,116,252,82,408,155);
 {int pm=mesh_id(player_ships[game.ship].name);Vec3 pc;float ps;float yaw=preview_time*.5f+.55f,roll=preview_time*.22f;ship_preview_layout(pm,&pc,&ps);ps*=1.8f;Vec3 pp=sub((Vec3){0,0,320},mul(rotate(pc,yaw,roll),ps));shipmesh(pm,pp,yaw,roll,ps,decorator_finishes[finish],1);flush_meshes();shipmesh_preview_edges(pm,pp,yaw,roll,ps,RGB(174,196,193));preview_reset();}
 for(int i=0;i<8;i++){int x=249+i*20;rect(x,171,17,10,decorator_finishes[i]);if(i==finish){rect(x,168,17,2,WHITE);rect(x,182,17,2,WHITE);}}
 rect(247,194,166,27,RGB(18,31,44));rect(247,194,166,1,RGB(106,215,218));
 text(32,25,WHITE,"%.19s",player_ships[game.ship].name);
 text(32,26,ship_paint[game.ship]==decorator_finishes[finish]?CYAN:GOLD,
      ship_paint[game.ship]==decorator_finishes[finish]?"EQUIPPED":"%d U TO PAINT",fees[finish]);
 if(decorator_feedback&&game.message_time>0&&game.message[0]){rect(108,200,132,21,RGB(18,31,44));text_wrap(14,25,16,2,RGB(246,186,92),game.message,0);}
 rect(84,240,226,17,RGB(39,50,59));text_px(100,244,WHITE,game.docked?"UP/DOWN X PAINT O BACK":"DOCK TO PAINT O BACK");
 rect(318,240,116,17,RGB(39,50,59));text(40,30,GOLD,"%.1f U",game.credits*.1f);
}
/* Action feedback gets its own reserved band, not a talking-character card. */
static void menu_notice(void){
 if(game.message_time<=0||!game.message[0])return;
 int narrative=page==CAMPAIGN||page==GUILD||page==STORY;
 int y=page==HOME?216:narrative?224:192;int h=narrative?24:32;
 /* Charcoal plate + ochre rail — action feedback, not a gold debug card. */
 rect(8,y,464,h,RGB(21,28,39));rect(8,y,464,1,RGB(193,139,77));rect(8,y,3,h,RGB(193,139,77));
 text_wrap(2,(y+2)/8,56,narrative?2:3,RGB(229,210,163),game.message,0);
}
