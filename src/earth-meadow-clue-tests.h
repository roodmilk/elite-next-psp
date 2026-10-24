{
 game.opening.stage=OP_FIND;game.opening.flags=OF_DIALOG_DONE;game.opening.elapsed=game.opening.distance=0;
 float fx,fz;earth_meadow_flower(4,&fx,&fz);game.pos=meadow_ground_point(pad,fx,fz,22);
 game.yaw=.1f;game.pitch=-.15f;opening_frontend_reset();opening_prepare();page=FLIGHT;
 opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);
 OPEN_CHECK(game.opening.stage==OP_FIND&&(game.opening.flags&OF_MEADOW_FLOWERS)&&(game.opening.flags&OF_MEADOW_GLOW)&&opening_notice&&!strcmp(opening_notice_speaker,"DAD"),"inspecting flower trail gives Dad's clue and awakens the stone without skipping exploration");
 opening_test_capture("meadow-280-flower-clue.bmp");
 {static Game restored;OPEN_CHECK(save_game(&game,"test-flower-trail.sav")&&load_game(&restored,"test-flower-trail.sav")&&(restored.opening.flags&OF_MEADOW_FLOWERS)&&(restored.opening.flags&OF_MEADOW_GLOW),"flower discovery and awakened glow survive actual save reload");}
 opening_test_step(PSP_CTRL_CROSS,0,0,0);opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);
 OPEN_CHECK(!opening_notice&&game.opening.stage==OP_FIND,"flower clue is optional and does not repeat after dismissal");
 game.pos=meadow_ground_point(pad,78,-58,22);int walked=1;
 for(int i=1;i<EARTH_MEADOW_FLOWERS;i++){
  earth_meadow_flower(i,&fx,&fz);int steps=0;
  while(hypotf(game.pos.x-pad.x-fx,game.pos.z-pad.z-fz)>3&&steps++<20){game.yaw=atan2f(pad.x+fx-game.pos.x,pad.z+fz-game.pos.z);opening_test_step(0,PSP_CTRL_UP,0,0);}
  if(steps>=20){walked=0;break;}
 }
 OPEN_CHECK(walked&&length(sub(game.pos,opening_object_pos()))<35,"player can actually walk the entire flower trail into stone interaction range");
 opening_test_step(PSP_CTRL_TRIANGLE,0,0,0);
 OPEN_CHECK(game.opening.stage==OP_OBJECT,"flower-led discovery enters the existing stone story and choices");
 game.opening.stage=OP_FIND;game.opening.flags=OF_DIALOG_DONE;game.opening.elapsed=17.99f;game.opening.distance=55;
 game.pos=meadow_ground_point(pad,80,-85,22);opening_frontend_reset();opening_prepare();opening_test_step(0,0,0,0);
 OPEN_CHECK((game.opening.flags&OF_MEADOW_GLOW)&&!(game.opening.flags&OF_MEADOW_FLOWERS),"ordinary exploration still awakens stone when optional flower clue is skipped");
 opening_notice=0;
 /* Native far/near views of the route, plus a gameplay-facing glow capture. */
 static const float shots[][4]={{78,-92,.1f,-.10f},{112,-9,-.25f,-.22f},{35,-195,.235f,-.08f}};
 for(int k=0;k<3;k++){game.pos=meadow_ground_point(pad,shots[k][0],shots[k][1],22);game.yaw=shots[k][2];game.pitch=shots[k][3];char name[64];snprintf(name,sizeof(name),"meadow-280-trail-%d.bmp",k);opening_test_capture(name);}
 game.pos=meadow_ground_point(pad,95,-195,22);game.pos.y=terrain_height(&game,pad.x+95,pad.z+55)+5;
 game.yaw=game.pitch=game.roll=0;surface_sprite_tan=0;surface_light=1;surface_depth_on=1;surface_y_min=28;surface_y_max=239;
 unsigned blank=RGB(5,12,18);int changed[4]={0};
 for(int mode=0;mode<4;mode++){
  rect(0,0,W,H,blank);memset(surface_depth,mode==1?0:255,sizeof(surface_depth));
  game.yaw=mode==2?3.14159265f:0;game.opening.stage=mode==3?OP_RETURN:OP_FIND;game.opening.flags|=OF_MEADOW_GLOW;
  earth_meadow_stone(pad);
  for(int y=28;y<240;y++)for(int x=0;x<W;x++)changed[mode]+=fb[y*STRIDE+x]!=blank;
 }
 OPEN_CHECK(changed[0]>=30,"awakened stone retains a readable luminous footprint at 250 metres");
 OPEN_CHECK(!changed[1]&&!changed[2]&&!changed[3],"stone glow cannot shine through occluders appear behind camera or remain after collection");
 surface_depth_on=0;surface_sprite_z=0;
}
