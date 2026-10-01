/* Small branded lettering; stable baseline wobble, no flashing or extra assets. */
static void decorator_menu_label(int x,int y){
 const char *label="PIMP-MY-SHIP.NET";
 for(int i=0;label[i];i++){
  unsigned ink=high_contrast?UI_TEXT:i<4?RGB(116,225,235):i<12?RGB(246,165,212):RGB(250,210,124);
  text_px(x+i*8,y+(!high_contrast&&i%3==1?1:0),ink,"%c",label[i]);
 }
}
static void home(void){
 int group=deck_group(row);deck_clamp_row();group=deck_group(row);
 char local_tv_label[32];snprintf(local_tv_label,sizeof(local_tv_label),"%.20s LOCAL TV",game.systems[game.system].name);
 header(game.docked?"STATION / COMMAND DECK":"COCKPIT / PAUSED");
 for(int i=0,col=0;i<5;i++){int visible[6];if(!deck_fill(i,visible))continue;int x=8+col++*94;rect(x,30,90,20,i==group?UI_RAISED:UI_PANEL);if(i==group)rect(x,48,90,2,UI_ACCENT);text((x+8)/8,4,i==group?UI_TEXT:UI_MUTED,"%s",deck_groups[i]);}
 const char *labels[]={game.docked?(tutorial_active(&game)?"FLY / Launch":"Launch"):"Resume flight",game.docked?"Cargo & market":"Cargo","Galaxy map","Shipyard","Outfitting","Save / status","Controls","Factions","Targeting computer","Debug tools","Comms panel","System details","Mission board","Mission log","GalacticNet","Discovery Codex","Radio & audio","Tracked mission","Explorers Guild","Display & chatter","Disembark","Ship loadout","Galactic Lore","PIMP-MY-SHIP.NET","Mechanics","Quit to main menu","Local TV"};
 static const char *hints[][2]={
 {"FLY INTO SPACE!","Fly at your own pace."},{"Your hold and local goods.","Station prices while docked."},{"Choose your next system.","Check range before jumping."},
 {"BUY NEW SHIPS!","Requires station services."},{"GET SHIP UPGRADES!","No locked tech teases."},{"Save, load, records.","Save at a station."},
 {"view game controls","Four short reference pages."},{"Meet the local factions.","Learn their colours and roles."},{"Select ships or worlds.","Choose a target to follow."},
 {"Change cash or world state.","Debug changes affect saves."},{"Hail station or get help.","Request guided docking."},{"Station class, risk, worlds.","Know where you are flying."},
 {"Find work at this station.","Dock to accept a contract."},{"Review jobs and route.","Reading pauses job clocks."},{"News and local SpaceBook.","Take a break from flying."},
 {"Review your discoveries.","Keep a record of your travels."},{"cruise to sweet tunes!","Set music and effects levels."},{"Your selected mission.","Choose it in Mission Log."},
 {"Optional Guild assignments.","Also listed in Mission Log."},{"Choose HUD and chatter.","Keep the view comfortable."},{"SEE WHATS AROUND!","LOOK SPEAK GO TAKE on hotspots."},{"SEE WHATS ON YOUR SHIP","See what your ship carries."},{"A readable history of the galaxy.","Explore the major eras, peoples and powers."},{"REPAINT YOUR HULL","Choose a finish or pattern."},{"RESTORE SHIP CONDITION","Repairs and fuel at the station."},{"RETURN TO THE TITLE SCREEN","Unsaved progress is not saved."},{"LOCAL BROADCASTS","Watch three channels from this system."}};
 panel(8,58,222,132);panel(238,58,234,132);
 int vis[6],vn=deck_fill(group,vis);
 for(int i=0;i<vn;i++){int id=vis[i],y=8+i*2;int locked=!game.docked&&id==20;
  if(id==row){rect(10,y*8-2,218,15,UI_RAISED);rect(10,y*8-2,3,15,UI_ACCENT);}
  int story_row=id==17&&(game.campaign_stage<6||game.guild_chapter<4||game.job_n>0);unsigned ink=id==row?UI_TEXT:locked?UI_MUTED:story_row?UI_ACCENT:UI_TEXT;
  const char *label=id==3&&station_class(&game)==STATION_MEGA?"Mega Shipyard":id==26?local_tv_label:labels[id];
  if(id==23){text(3,y,ink,"%s",id==row?">":" ");decorator_menu_label(32,y*8);}else text(3,y,ink,"%s%s%.21s",id==row?">":" ",story_row?"! ":"",label);
 }
 /* Top-right: live third-person ship in local space. */
 menu_space_view(246,64,218,92);
 int is_story=row==17&&(game.campaign_stage<6||game.guild_chapter<4||game.job_n>0);
 if(row==23)decorator_menu_label(31*8,20*8);else text(31,20,UI_ACCENT,"%.27s",row==3&&station_class(&game)==STATION_MEGA?"Mega Shipyard":row==26?local_tv_label:labels[row]);
 /* The detail pane is 27 native columns wide; keep future copy inside it. */
 text(31,22,UI_TEXT,"%.27s",hints[row][0]);
 if(!game.docked&&row==20)text(31,23,UI_ACCENT,"Dock first to open this.");
 if(is_story){rect(246,172,218,2,UI_EDGE);text(31,23,UI_ACCENT,"Open to see your next step.");}
 text(2,25,UI_SIGNAL,"System: %.12s",game.systems[game.system].name);
 {int wl=wanted_level(&game);text(2,26,wl?RED:UI_MUTED,wl?"Wanted [%s]":"Clear warrant",stars(wl));}
 text(31,25,UI_TEXT,"%s / %s",game.docked?"DOCKED":"PAUSED",station_class_name(station_class(&game)));credits_badge();
 footer("");
}
static void help(void){
 static const char *titles[]={"FLIGHT","TARGETS / TRAVEL","STATIONS / SURFACES","MENUS / COMFORT","PLANET EVA"};
 static const char *keys[][6]={{"D-pad / analog","R / L","2xR boost / 2xL brake","L + Left / Right","X / tap Circle","Start + D-pad"},
 {"Tap / hold Square","Square + Left/Right","Square + Up/Down","Square + R","Steer manually","Circle + D-pad"},
 {"Hold Triangle","Triangle near a hub","Triangle near a world","X / Circle prompt","Triangle beside ship","R / X when aboard"},
 {"Select in flight","Left / Right on deck","X / Circle","Hold Triangle","L + Select in flight","L on this screen"},
 {"Nub / L + D-pad","D-pad Up / Down","D-pad Left / Right","Tap R / hold R","Circle / Triangle","Tap / hold Square"}};
 static const char *actions[][6]={{"Steer the ship","Accelerate / slow down","Boost or hard brake","Roll (also in boost)","Laser / selected tool","Power banks SYS/ENG/WEP"},
 {"Reticle target / browser","Change target category","Choose target","Lock + auto-turn","Cancel auto-turn","Ship tools: equip, tap O"},
 {"Comms and docking request","Choose REQUEST AUTO-DOCK","Ask to approach","Enter / turn away","Board the parked ship","Launch / step outside"},
 {"Open the paused deck","Change service category","Open / return","Chatter and comms options","Full / minimal / scenic HUD","Toggle analog steering"},
 {"Look (L stops walking)","Walk forward / back","Strafe left / right","Short jump / run","Scan / Codex or board","Cycle / target computer"}};
 header("COMMANDER / CONTROLS");panel(8,32,464,190);
 text(3,5,UI_ACCENT,"%d / 5   %s",help_tab+1,titles[help_tab]);
 for(int i=0;i<6;i++){int y=8+i*3;text(3,y,UI_SIGNAL,"%s",keys[help_tab][i]);text(29,y,UI_TEXT,"%s",actions[help_tab][i]);}
 if(help_tab==4)text(3,29,UI_MUTED,"Square cycles visible targets; Triangle boards nearby");
 else if(help_tab==1)text(3,29,UI_MUTED,"Hold O+Left: Tractor. Release; tap O within 500m.");
 else text(3,29,UI_MUTED,"Analog %s. Centre the nub after enabling.",analog_enabled?"ON":"OFF");
 footer("LEFT/RIGHT PAGE   L ANALOG ON/OFF   O BACK");
}
static void decorator_screen(void){
 int finish=row<0?0:row>=DECORATOR_COUNT?DECORATOR_COUNT-1:row,first=finish/DECORATOR_PAGE*DECORATOR_PAGE;
 decorator_monitor_draw();
 /* The artwork contains its own screen frame, browser bar, icons and ad.
  * Dynamic UI is kept within the two intentionally empty screen windows. */
 text_px(118,29,RGB(210,235,229),"PIMP-MY-SHIP.NET");
 text_px(115,93,RGB(106,215,218),"PAINT FINISHES");
 for(int i=first;i<first+DECORATOR_PAGE;i++){
  int y=(13+i-first)*8;
  if(i==finish){rect(111,y-1,130,10,RGB(42,76,88));rect(111,y-1,2,10,RGB(246,186,92));}
  if(ship_paint[game.ship]==decorator_finishes[i]){rect(114,y-1,12,10,RGB(246,186,92));rect(116,y+1,8,6,decorator_finishes[i]);pixel(109,y+2,RGB(246,186,92));pixel(108,y+3,RGB(246,186,92));pixel(109,y+4,RGB(246,186,92));}
  else rect(116,y+1,8,6,decorator_finishes[i]);
  text(16,13+i-first,i==finish?WHITE:RGB(171,194,198),"%s",decorator_names[i]);
 }
 preview_clip(329,116,252,82,408,155);
 {int pm=mesh_id(player_ships[game.ship].name);Vec3 pc;float ps;float yaw=preview_time*.5f+.55f,roll=preview_time*.22f;ship_preview_layout(pm,&pc,&ps);ps*=1.8f;Vec3 pp=sub((Vec3){0,0,320},mul(rotate(pc,yaw,roll),ps));shipmesh(pm,pp,yaw,roll,ps,decorator_finishes[finish],1);flush_meshes();shipmesh_preview_edges(pm,pp,yaw,roll,ps,RGB(174,196,193));preview_reset();}
 text_px(115,179,RGB(171,194,198),"FINISH %02d / %02d",finish+1,DECORATOR_COUNT);
 for(int i=first;i<first+DECORATOR_PAGE;i++){int x=249+(i-first)*20;rect(x,171,17,10,decorator_finishes[i]);if(i==finish){rect(x,168,17,2,WHITE);rect(x,182,17,2,WHITE);}}
 rect(247,194,166,27,RGB(18,31,44));rect(247,194,166,1,RGB(106,215,218));
 text(32,25,WHITE,"%.19s",player_ships[game.ship].name);
 text(32,26,ship_paint[game.ship]==decorator_finishes[finish]?UI_CYAN:UI_GOLD,
      ship_paint[game.ship]==decorator_finishes[finish]?"EQUIPPED":"%d U TO PAINT",decorator_fees[finish]);
 if(decorator_feedback&&game.message_time>0&&game.message[0]){rect(108,200,132,21,RGB(18,31,44));text_wrap(14,25,16,2,RGB(246,186,92),game.message,0);}
 rect(84,240,226,17,RGB(39,50,59));text_px(100,244,WHITE,game.docked?"UP/DOWN X PAINT O BACK":"DOCK TO PAINT O BACK");
 rect(318,240,116,17,RGB(39,50,59));text_px(330,243,UI_GOLD,"%.1f U",game.credits*.1f);
}
/* Action feedback gets its own reserved band, not a talking-character card. */
static void menu_notice(void){
 if(page==LOCALTV)return; /* Broadcast owns its caption and lower-third bands. */
 if(page==INVENTORY)return; /* Loadout owns one reserved feedback line. */
 if(page==WALK&&walk_kind==0&&game.docked)return; /* Station owns its dialogue/feedback band. */
 if(page==DETAILS)return; /* Almanac owns the full illustrated spread. */
 if(game.message_time<=0||!game.message[0])return;
 int narrative=page==CAMPAIGN||page==GUILD||page==STORY;
 int y=page==HOME?216:narrative?224:192;int h=narrative?24:32;
 /* Charcoal plate + ochre rail — action feedback, not a gold debug card. */
 rect(8,y,464,h,UI_PANEL);rect(8,y,464,1,UI_EDGE);rect(8,y,3,h,UI_EDGE);
 text_wrap(2,(y+2)/8,56,narrative?2:3,UI_TEXT,game.message,0);
}
