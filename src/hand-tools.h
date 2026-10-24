/* On-foot tool gestures never share spacecraft weapons or rover controls. */
static const char *hand_label(void){return hand_selected==0?"SCANNER":"TOOLS";}
static int hand_context(void){
 return page==FLIGHT&&game.planet>=1&&game.surface==2&&!game.rover_driving&&!game.dead&&!paused&&!ps_open&&!ps_release&&!eva_map_open&&!eva_map_release&&!eva_map_codex&&!planet_transfer_busy()&&planet_controls_ready;
}
static void hand_context_tick(unsigned pressed,unsigned held){
 if(!hand_context()){
  hand_open=hand_armed=hand_consumed=hand_chosen=0;hand_hold=0;
  if((pressed|held)&PSP_CTRL_CIRCLE)hand_wait=1;
 }else if(hand_wait&&!(held&PSP_CTRL_CIRCLE)&&!(pressed&PSP_CTRL_CIRCLE))hand_wait=0;
}
static void hand_use(void){
 if(hand_selected!=0){message(&game,"Hold Circle and choose the handheld scanner.");return;}
 if(surface_target_open){
  int ids[19],n=surface_target_list(ids);
  if(n&&surface_target_row<n&&ids[surface_target_row]>=11)surface_target_lock=ids[surface_target_row];
 }
 if(surface_target_lock<11){
  float best=1e9f;int found=-1;
  for(int i=0;i<LIFE_COUNT;i++)if(game.life[i].alive){
   Vec3 v=camera(&game,game.life[i].pos);if(v.z<4)continue;Point q=project(v);
   float dx=q.x-240,dy=q.y-110,score=dx*dx+dy*dy;
   if(score<90*90&&score<best&&length(sub(game.life[i].pos,game.pos))<=module_survey_range(&game)){best=score;found=11+i;}
  }if(found>=0)surface_target_lock=found;
 }
 surface_scan_selected();
}
static int hand_input(unsigned pressed,unsigned held,float dt){
 if(!hand_context())return 0;
 if(hand_wait){hand_open=hand_armed=0;return (pressed|held)&PSP_CTRL_CIRCLE;}
 if(pressed&PSP_CTRL_CIRCLE){
  hand_armed=1;hand_hold=0;hand_consumed=hand_chosen=0;
  /* Synthetic one-frame taps use the same action; real hardware uses release. */
  if(!(held&PSP_CTRL_CIRCLE)){hand_armed=0;hand_use();return 1;}
 }
 if(held&PSP_CTRL_CIRCLE){
  if(!hand_armed)return 1;hand_hold+=fmaxf(0,dt);
  unsigned dirs=held&(PSP_CTRL_UP|PSP_CTRL_DOWN|PSP_CTRL_LEFT|PSP_CTRL_RIGHT);
  if(dirs){hand_consumed=1;hand_open=0;
   if(!(dirs&(dirs-1))&&(pressed&dirs)){
    if(dirs==PSP_CTRL_UP){hand_chosen=1;hand_selected=0;message(&game,"Handheld scanner equipped.");}
    else if(dirs==PSP_CTRL_DOWN){hand_chosen=1;hand_selected=1;message(&game,"Empty hands.");}
   }
  }
  if(hand_hold>=.20f)hand_consumed=1;
  hand_open=hand_hold>=.20f&&!hand_chosen&&!dirs;
  if(hand_open)surface_target_open=0;
  return 1;
 }
 if(hand_armed){int tap=!hand_consumed&&hand_hold<.20f;hand_open=hand_armed=0;hand_hold=0;if(tap)hand_use();return 1;}
 hand_open=0;return 0;
}
static void hand_draw(void){
 if(!hand_context())return;
 if(hand_selected==0){
  int bob=game.eva_running?(int)(sinf(game.time*8)*2):0,x=356,y=174+bob;
  unsigned glove=RGB(62,74,83),edge=RGB(162,158,133),caseink=RGB(37,55,69);
  /* Gloved forearm behind a foreshortened survey handset. */
  rect(x+17,y+39,42,26,glove);rect(x+25,y+46,43,19,RGB(30,41,53));
  rect(x+2,y+8,49,50,RGB(16,27,35));rect(x,y,45,55,edge);rect(x+3,y+3,39,47,caseink);
  rect(x+7,y+7,30,25,RGB(8,30,36));rect(x+9,y+9,26,2,UI_CYAN);
  for(int i=0;i<3;i++){rect(x+10,y+15+i*4,14+i*3,1,RGB(62,164,134));}
  rect(x+9,y+37,6,5,UI_GOLD);rect(x+21,y+37,6,5,UI_CYAN);rect(x+33,y+37,4,5,RGB(120,134,145));
  rect(x+17,y-9,9,10,RGB(54,74,85));rect(x+19,y-12,5,3,surface_scan_fx>0?WHITE:UI_CYAN);
  if(surface_scan_fx>0){rect(x+6,y+30,32,2,UI_CYAN);rect(x+37,y+7,2,26,WHITE);}
 }
 if(!hand_open)return;
 rect(137,64,206,103,RGB(13,22,31));rect(137,64,206,1,UI_GOLD);
 text_px(188,72,UI_CYAN,"HAND TOOLS");
 rect(225,99,30,35,RGB(39,56,68));rect(212,110,56,14,RGB(39,56,68));
 button_icon(234,111,'O',UI_GOLD);button_icon(234,94,'U',UI_CYAN);button_icon(234,135,'D',UI_CYAN);
 text_px(151,93,hand_selected==0?UI_GOLD:WHITE,"SCANNER");
 text_px(151,138,hand_selected==1?UI_GOLD:WHITE,"EMPTY HANDS");
 }
