/* Surface visor deliberately owns its controls: no spacecraft weapons/boost. */
static int surface_target_open=0,surface_target_cat=0,surface_target_row=0,surface_target_lock=-1;
static int surface_turn_active=0;
static int surface_scan_prompt=-1,surface_scan_slot=-1;
static float surface_scan_fx=0;
static void surface_target_reset(void){surface_target_open=0;surface_target_cat=surface_target_row=0;surface_target_lock=-1;surface_turn_active=0;surface_scan_prompt=surface_scan_slot=-1;surface_scan_fx=0;}
static int surface_target_list(int *ids){int n=0;if(surface_target_cat==0){for(int i=0;i<11;i++)ids[n++]=i;}else for(int i=0;i<LIFE_COUNT;i++)if(game.life[i].alive&&game.life[i].kind==surface_target_cat-1)ids[n++]=11+i;return n;}
static Vec3 surface_target_position(int id){return id==0?game.ship_pos:id<=10?surface_poi(&game,id-1):game.life[id-11].pos;}
static void surface_target_name(int id,char *out,int cap){if(id==0)snprintf(out,cap,"YOUR SHIP");else if(id==7)snprintf(out,cap,"ROVER");else if(id<=10)snprintf(out,cap,"%d %s",field_nav_number(id-1),surface_site_name(&game,game.system,game.planet,id-1));else field_species_name(game.system,game.planet,id-11,out,cap);}
static void surface_scan_tick(float dt){if(surface_scan_fx>0){surface_scan_fx-=dt;if(surface_scan_fx<0)surface_scan_fx=0;}}
static int surface_scan_selected(void){
 if(surface_target_lock<11||surface_target_lock>=11+LIFE_COUNT){message(&game,"Target nearby flora, fauna or minerals first.");return 0;}
 int slot=surface_target_lock-11;if(!game.life[slot].alive){surface_target_lock=-1;message(&game,"Survey target is no longer present.");return 0;}
 float distance=length(sub(game.life[slot].pos,game.pos)),range=module_survey_range(&game);
 if(distance>range){char note[80];snprintf(note,sizeof(note),"Move within %d m to scan this target.",(int)range);message(&game,note);return 0;}
 char name[40];field_species_name(game.system,game.planet,slot,name,sizeof(name));
 int fresh=!game.life[slot].scanned;if(fresh)survey_scan_target(&game,slot);else game.cue=SFX_SCAN;
 surface_scan_prompt=surface_scan_slot=slot;surface_scan_fx=.72f;
 {char note[96];snprintf(note,sizeof(note),fresh?"SCAN COMPLETE: %s. TRIANGLE OPENS CODEX.":"ALREADY LOGGED: %s. TRIANGLE OPENS CODEX.",name);message(&game,note);}
 return 1;
}
static void surface_target_cycle_front(void){
 int ids[19],count=0;
 for(int id=0;id<11+LIFE_COUNT;id++){
  if(id>=11&&!game.life[id-11].alive)continue;
  Vec3 v=camera(&game,surface_target_position(id));if(v.z<=15)continue;Point q=project(v);
  if(q.x<10||q.x>W-10||q.y<view_top()+4||q.y>view_bot()-4)continue;
  float dx=q.x-240,dy=q.y-110,score=dx*dx+dy*dy+fminf(v.z*.002f,80);
  int at=count;while(at>0){Vec3 pv=camera(&game,surface_target_position(ids[at-1]));Point pq=project(pv);float px=pq.x-240,py=pq.y-110,prior=px*px+py*py+fminf(pv.z*.002f,80);if(prior<=score)break;ids[at]=ids[at-1];at--;}
  ids[at]=id;count++;
 }
 if(!count){surface_target_lock=-1;surface_scan_prompt=-1;message(&game,"No survey targets in view.");return;}
 int next=0;for(int i=0;i<count;i++)if(ids[i]==surface_target_lock){next=(i+1)%count;break;}
 surface_target_lock=ids[next];surface_scan_prompt=-1;if(surface_target_lock<=10)surface_nav_poi=surface_target_lock-1;
 game.cue=SFX_SELECT;
}
static void surface_target_input(unsigned pressed){
 if(pressed&PSP_CTRL_LEFT){surface_target_cat=(surface_target_cat+3)%4;surface_target_row=0;}
 if(pressed&PSP_CTRL_RIGHT){surface_target_cat=(surface_target_cat+1)%4;surface_target_row=0;}
 int ids[19],n=surface_target_list(ids);if(!n)return;
 if(pressed&PSP_CTRL_UP)surface_target_row=(surface_target_row+n-1)%n;
 if(pressed&PSP_CTRL_DOWN)surface_target_row=(surface_target_row+1)%n;
 if(surface_target_row>=n)surface_target_row=0;int id=ids[surface_target_row];
 if(pressed&PSP_CTRL_RTRIGGER){surface_target_lock=id;surface_scan_prompt=-1;if(id<=10)surface_nav_poi=id-1;surface_turn_active=1;game.eva_jump_held=1;game.cue=SFX_SELECT;}
 if((pressed&PSP_CTRL_CIRCLE)&&id>=11){surface_target_lock=id;surface_scan_prompt=-1;surface_scan_selected();}
}
static void surface_target_turn(float dt){
 if(!surface_turn_active)return;
 if(game.surface!=2||surface_target_lock<0||surface_target_lock>=19){surface_turn_active=0;return;}
 Vec3 d=sub(surface_target_position(surface_target_lock),game.pos);float h=sqrtf(d.x*d.x+d.z*d.z);
 float yaw=h>.01f?atan2f(d.x,d.z):game.yaw,pitch=fmaxf(-.7f,fminf(.7f,atan2f(d.y,fmaxf(.01f,h))));
 float delta=atan2f(sinf(yaw-game.yaw),cosf(yaw-game.yaw)),t=1-expf(-18*fmaxf(0,fminf(.1f,dt)));
 game.yaw+=delta*t;game.pitch+=(pitch-game.pitch)*t;game.roll=0;
 if(fabsf(delta)<.002f&&fabsf(pitch-game.pitch)<.002f){game.yaw=yaw;game.pitch=pitch;surface_turn_active=0;}
}
static void surface_target_hud(void){
 if(surface_scan_fx>0&&surface_scan_slot>=0&&surface_scan_slot<LIFE_COUNT&&game.life[surface_scan_slot].alive){
  Vec3 v=camera(&game,game.life[surface_scan_slot].pos);if(v.z>12){Point q=project(v);int tx=(int)q.x,ty=(int)q.y;if(tx>4&&tx<W-4&&ty>view_top()&&ty<view_bot()){
   unsigned pulse=((int)(surface_scan_fx*24)&1)?UI_CYAN:WHITE;line(240,view_bot()-7,tx,ty,pulse);line(238,view_bot()-5,tx-2,ty,RGB(58,140,155));line(242,view_bot()-5,tx+2,ty,RGB(58,140,155));
   int r=7+(int)(surface_scan_fx*12);line(tx-r,ty-r,tx-3,ty-r,UI_CYAN);line(tx+r,ty-r,tx+3,ty-r,UI_CYAN);line(tx-r,ty+r,tx-3,ty+r,UI_CYAN);line(tx+r,ty+r,tx+3,ty+r,UI_CYAN);
  }}
 }
 if(surface_target_lock>=0&&surface_target_lock<19){
  Vec3 p=surface_target_position(surface_target_lock),v=camera(&game,p),d=sub(p,game.pos);char name[48];surface_target_name(surface_target_lock,name,sizeof(name));
  int x=240,y=110,onscreen=0;if(v.z>15){Point q=project(v);x=(int)q.x;y=(int)q.y;onscreen=x>14&&x<W-14&&y>42&&y<206;}
  if(onscreen){for(int i=0;i<4;i++){int sx=(i&1)?1:-1,sy=(i&2)?1:-1;line(x+sx*12,y+sy*12,x+sx*5,y+sy*12,UI_GOLD);line(x+sx*12,y+sy*12,x+sx*12,y+sy*5,UI_GOLD);}}
  (void)d;(void)name;
 }
 if(!surface_target_open)return;
 rect(8,36,192,200,RGB(17,27,38));rect(8,36,192,2,UI_GOLD);
 static const char *cats[]={"SITES / SHIP","FLORA","FAUNA","MINERALS"};
 text(2,5,UI_CYAN,"EXPLORATION PC");text(2,7,UI_GOLD,"< %s >",cats[surface_target_cat]);
 int ids[19],n=surface_target_list(ids);if(surface_target_row>=n)surface_target_row=0;int first=surface_target_row/6*6;
 for(int i=first;i<n&&i<first+6;i++){char name[48];surface_target_name(ids[i],name,sizeof(name));int y=9+(i-first)*2;if(i==surface_target_row)rect(12,y*8-2,184,13,RGB(63,71,76));text(2,y,i==surface_target_row?UI_GOLD:WHITE,"%.22s",name);}
 if(!n)text(2,10,UI_MUTED,"NO LOCAL CONTACTS");
 else text(2,22,UI_CYAN,"%d M  / %d OF %d",(int)length(sub(surface_target_position(ids[surface_target_row]),game.pos)),surface_target_row+1,n);
 button_icon(16,192,'L',UI_MUTED);button_icon(28,192,'R',UI_MUTED);text(6,24,UI_MUTED,"TYPE");
 button_icon(96,192,'U',UI_MUTED);button_icon(108,192,'D',UI_MUTED);text(16,24,UI_MUTED,"SELECT");
 button_icon(16,208,'r',UI_CYAN);text(4,26,UI_CYAN,"TRACK");button_icon(104,208,'O',UI_CYAN);text(15,26,UI_CYAN,"SCAN");
 text(2,28,UI_MUTED,"RELEASE");button_icon(80,224,'S',UI_MUTED);text(12,28,UI_MUTED,"TO CLOSE");
}
