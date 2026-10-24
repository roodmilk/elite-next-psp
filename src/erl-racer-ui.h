static void erl_racer_open(int quick){
 erl_racer_picker=1;erl_racer_picker_quick=quick;erl_racer_cursor=quick?erl_quick_racer_choice:erl_racer_id(&game,0);
}
static int erl_racer_input(unsigned pressed){
 if(!erl_racer_picker)return 0;
 if(pressed&PSP_CTRL_CIRCLE){erl_racer_picker=0;return 1;}
 if(pressed&(PSP_CTRL_UP|PSP_CTRL_LEFT))erl_racer_cursor=(erl_racer_cursor+11)%12;
 if(pressed&(PSP_CTRL_DOWN|PSP_CTRL_RIGHT))erl_racer_cursor=(erl_racer_cursor+1)%12;
 if(pressed&PSP_CTRL_CROSS){
  if(erl_racer_picker_quick){erl_quick_racer_choice=erl_racer_cursor;erl_racer_picker=0;erl_quick_race_start();}
  else{int before=game.erl.inbox;if(erl_racer_select(&game,erl_racer_cursor)){if(erl_commit())erl_racer_picker=0;else game.erl.inbox=before;}}
 }
 return 1;
}
static void __attribute__((noinline,optimize("Os"))) erl_racer_picker_draw(void){
 rect(0,0,W,H,RGB(8,12,24));rect(0,0,W,28,RGB(17,26,44));text_px(12,10,CYAN,"ERL / RACER CLASS");text_px(366,10,GOLD,"%02d / 12",erl_racer_cursor+1);
 for(int i=0;i<12;i++){int y=40+i*15;if(i==erl_racer_cursor)rect(6,y-3,154,15,RGB(48,43,72));rect(12,y,3,7,erl_racers[i].hull);text_px(22,y,i==erl_racer_cursor?WHITE:DIM,"%02d %s",i+1,erl_racers[i].name);}
 int old_min=surface_y_min,old_max=surface_y_max,old_depth=surface_depth_on,old_enc=mega_depth_encoding,old_fast=race_opaque_quads,old_ref=render_reference;
 int x0=clipx0,x1=clipx1,y0=clipy0,y1=clipy1;float ox=proj_ox,oy=proj_oy,focal=proj_focal;
 proj_ox=321;proj_oy=139;proj_focal=290;clipx0=166;clipx1=480;clipy0=30;clipy1=217;surface_y_min=30;surface_y_max=216;surface_depth_on=mega_depth_encoding=race_opaque_quads=1;render_reference=0;memset(surface_depth,255,sizeof(surface_depth));
 Vec3 eye={240,175,330},dir=norm(mul(eye,-1));camera_override_set(eye,atan2f(dir.x,dir.z),asinf(dir.y),0);
 drawcount=0;erl_racer_draw(erl_racer_cursor,(Vec3){0,0,0},(Vec3){1,0,0},(Vec3){0,1,0},(Vec3){0,0,1},1.1f,0,0);flush_solid_meshes();camera_override_clear();
 proj_ox=ox;proj_oy=oy;proj_focal=focal;clipx0=x0;clipx1=x1;clipy0=y0;clipy1=y1;surface_y_min=old_min;surface_y_max=old_max;surface_depth_on=old_depth;mega_depth_encoding=old_enc;race_opaque_quads=old_fast;render_reference=old_ref;
 text_px(183,215,CYAN,"%s",erl_racers[erl_racer_cursor].name);text_px(183,229,DIM,"EQUAL CLASS / TWIN BOOST ENGINES");
 rect(0,246,W,26,RGB(17,26,44));int locked=!erl_racer_picker_quick&&(game.erl.event_kind||game.erl.championship||race_is_active(&game));
 text_px(10,255,locked?GOLD:WHITE,locked?"RACER LOCKED FOR EVENT / O BACK":erl_racer_picker_quick?"D-PAD SELECT / X RACE / O BACK":"D-PAD SELECT / X REGISTER / O BACK");
}
