static int field_guide_tab=0;
static void field_guide_screen(void){
 if(game.planet<1)return;int sys=game.system,body=game.planet;FieldProfile p=field_profile(&game,sys,body);
 header("PLANET / FIELD GUIDE");panel(8,32,464,214);text(3,5,UI_GOLD,"%.22s / %s",game.bodies[body].name,field_culture(p.culture));text(3,7,UI_MUTED,"%s   < L/R %s >",field_weather(p.weather),field_guide_tab?"LIFE":"SITES");
 int count=field_guide_tab?8:10,first=row/5*5;
 for(int i=first;i<first+5&&i<count;i++){int y=10+(i-first)*3;char name[40];if(field_guide_tab){if(field_species_logged(&game,sys,body,i))field_species_name(sys,body,i,name,40);else snprintf(name,40,"UNRECORDED SPECIES %d",i+1);}else if(i==6)snprintf(name,40,"ROVER");else snprintf(name,40,"%d %s",field_nav_number(i),surface_site_name(&game,sys,body,i));if(i==row)rect(16,y*8-2,216,18,UI_EDGE);text(3,y,i==row?UI_GOLD:WHITE,"%.26s",name);}
 if(field_guide_tab){int known=field_species_logged(&game,sys,body,row);field_draw_species(350,133,58,sys,body,row,preview_time,70,145);text(31,18,UI_CYAN,known?"LOGGED / %03d-%d-%d":"NOT YET LOGGED / %03d-%d-%d",sys+1,body,row+1);text_wrap(31,21,27,4,WHITE,known?field_species_trait(sys,body,row):"Square scans nearby life. Every record keeps its actual planet and species identity.",0);}
 else {int id=row;unsigned bit=field_site_bit(id);int done=bit&&(game.surface_progress[sys][body]&bit);text(31,10,UI_CYAN,"%d M / %s",(int)length(sub(surface_poi(&game,id),game.pos)),done?"COMPLETE":"AVAILABLE");text_wrap(31,13,27,10,WHITE,surface_site_brief(&game,sys,body,id),0);}
 text(3,29,UI_MUTED,field_guide_tab?"Circle scans. Logged discoveries persist in the Codex.":"X tracks location. Return there and press X to interact.");footer("UP/DOWN   L/R PAGE   X TRACK   O RETURN");
}
static void field_guide_input(unsigned pressed){
 if(game.planet<1||game.surface!=2){change_page(HOME);return;}
 if(pressed&(PSP_CTRL_LTRIGGER|PSP_CTRL_RTRIGGER)){field_guide_tab^=1;row=0;}
 int count=field_guide_tab?8:10;
 if(pressed&PSP_CTRL_UP)row=(row+count-1)%count;if(pressed&PSP_CTRL_DOWN)row=(row+1)%count;
 if(pressed&(PSP_CTRL_CIRCLE|PSP_CTRL_START)){change_page(FLIGHT);return;}
 if((pressed&PSP_CTRL_CROSS)&&!field_guide_tab){surface_nav_poi=row;change_page(FLIGHT);message(&game,"Field site marked on compass. X interacts when nearby.");}
}
