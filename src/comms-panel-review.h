#include "conversation-routing-review.h"
/* Isolated native console checks, including the existing complete control suite. */
static int comms_panel_review(void){
 FILE *flag=fopen("comms-panel-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *f=fopen("comms-panel-review.txt","w");if(!f)return 1;
 int failures=0;unsigned *old=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));if(!pixels){fprintf(f,"RESULT 1 failures\n");fclose(f);return 1;}fb=pixels;
#define OPEN_CHECK(c,label) do{int ok=!!(c);fprintf(f,"%s %s\n",ok?"PASS":"FAIL",label);failures+=!ok;fflush(f);}while(0)
 #include "comms-console-tests.h"
 game_init(&game);story_complete(&game);game.opening.stage=0;game.tutorial_step=0;launch(&game);game.dock_stage=0;game.voice_time=game.message_time=0;game.encounter_kind=ENCOUNTER_NONE;selected_target=-1;target_explicit=0;comms_encounter_conversation=night_open=0;deck_reset();page=FLIGHT;change_page(COMMS_PANEL);comms_return=FLIGHT;comms_quiet=quiet_comms=radio_off=0;
 Vec3 pos=game.pos;float clock=game.time,speed=game.speed,heat=game.heat;int credits=game.credits;
 for(int id=0;id<16;id++)if(id!=5&&id!=9){
  row=id;radio_station=id%RADIO_STATION_COUNT;int before=radio_station;
  game_input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);int next=radio_station==(before+1)%RADIO_STATION_COUNT&&row==id&&page==COMMS_PANEL;
  game_input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);
  char label[96];snprintf(label,sizeof(label),"L/R tune both ways without moving focus or activating %s",comms_labels[id]);
  OPEN_CHECK(next&&radio_station==before&&row==id&&page==COMMS_PANEL&&credits==game.credits&&selected_target==-1,label);
 }
 OPEN_CHECK(game.time==clock&&game.speed==speed&&game.heat==heat&&length(sub(pos,game.pos))==0,"global radio shortcuts leave the paused simulation and flight controls untouched");
 row=1;radio_station=0;game_input(PSP_CTRL_LTRIGGER,PSP_CTRL_LTRIGGER,.016f,0,0);OPEN_CHECK(radio_station==RADIO_STATION_COUNT-1,"L wraps from the first station to the last on a disabled Contact button");
 game_input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);OPEN_CHECK(radio_station==0,"R wraps from the last station to the first");
 game_input(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER,PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER,.016f,0,0);OPEN_CHECK(radio_station==0,"simultaneous L and R cancel rather than choosing an arbitrary station");
 for(int i=0;i<30;i++)game_input(0,PSP_CTRL_RTRIGGER,.016f,0,0);OPEN_CHECK(radio_station==0&&row==1,"holding a shoulder button does not repeatedly skip stations or move focus");
 comms_toggle_quiet();row=7;game_input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);OPEN_CHECK(radio_station==1&&comms_quiet&&radio_off&&quiet_comms,"tuning from another control preserves Quiet Mode and its mute state");
 comms_toggle_quiet();OPEN_CHECK(!radio_off&&!quiet_comms&&!comms_quiet&&radio_station==1,"leaving Quiet Mode restores sound with the newly tuned station");
 row=10;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(radio_station==2&&page==COMMS_PANEL,"X on the tuner retains its existing next-station action");
 row=0;game_input(PSP_CTRL_RTRIGGER,PSP_CTRL_RTRIGGER,.016f,0,0);int saved=radio_station;game_input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 radio_station=0;OPEN_CHECK(page==FLIGHT&&radio_load_settings("radio.cfg")&&radio_station==saved,"a globally tuned station persists through Circle and the actual radio preference loader");
 change_page(COMMS_PANEL);row=1;int current=radio_station;comms_encounter_conversation=1;OPEN_CHECK(!comms_console_input(PSP_CTRL_RTRIGGER,.016f)&&radio_station==current,"conversation pages retain their existing shoulder controls");comms_encounter_conversation=0;
 night_open=1;OPEN_CHECK(!comms_console_input(PSP_CTRL_LTRIGGER,.016f)&&radio_station==current,"Quiet Watch dialogue does not consume shoulders as console tuning");night_open=0;
 /* Every selectable control has an unambiguous solid focus frame, across every
  * paint palette. The border stays inside existing physical control bounds. */
 unsigned paint=ship_paint[game.ship];int borders=1;
 for(int finish=0;finish<DECORATOR_COUNT;finish++){ship_paint[game.ship]=decorator_finishes[finish];
  for(int id=0;id<16;id++)if(id!=5&&id!=9){row=id;comms_press=0;comms_anim=1;comms_console_draw();CommsKey k=comms_keys[id];
   for(int x=k.x;x<k.x+k.w;x++)borders&=pixels[k.y*STRIDE+x]==UI_ACCENT&&pixels[(k.y+k.h-1)*STRIDE+x]==UI_ACCENT;
   for(int y=k.y;y<k.y+k.h;y++)borders&=pixels[y*STRIDE+k.x]==UI_ACCENT&&pixels[y*STRIDE+k.x+k.w-1]==UI_ACCENT;
   borders&=pixels[(k.y+1)*STRIDE+k.x+1]==comms_mix(UI_ACCENT,UI_TEXT,160);
  }
 }
 OPEN_CHECK(borders,"all fourteen controls have a complete accent frame and bright corners in all sixteen ship palettes");
 ship_paint[game.ship]=decorator_finishes[0];const int focus[]={10,1,7,13,4};
 for(int i=0;i<5;i++){row=focus[i];comms_console_draw();char file[64];snprintf(file,sizeof(file),"comms-focus-%d.bmp",row);dump_native_bmp(file);}
 ship_paint[game.ship]=decorator_finishes[13];row=12;comms_console_draw();dump_native_bmp("comms-focus-red-camera.bmp");ship_paint[game.ship]=paint;
 row=1;game.message_time=game.voice_time=0;game_input(PSP_CTRL_CROSS,0,.016f,0,0);OPEN_CHECK(page==COMMS_PANEL&&!comms_encounter_conversation&&comms_unavailable(1),"stronger highlight does not enable an unavailable control");
 conversation_routing_checks(f,&failures);
 fprintf(f,"RESULT %d failures\n",failures);fclose(f);fb=old;free(pixels);
#undef OPEN_CHECK
 return 1;
}
