/* Layered cosmic fields: bounded additive motes and fluid filaments, not meshes.
 * No extra target objects, particle allocation or unbounded close-up loops. */
static void rift_glow(int x,int y,int radius,unsigned color,int top,int bot){
 if(high_contrast){if(x>=0&&x<W&&y>=top&&y<=bot)pixel(x,y,color);return;}
 for(int dy=-radius;dy<=radius;dy++)for(int dx=-radius;dx<=radius;dx++){
  int d=dx*dx+dy*dy,rr=radius*radius+1;if(d>=rr)continue;
  int k=(rr-d)*100/rr;unsigned c=RGB((color&255)*k/220,((color>>8)&255)*k/220,((color>>16)&255)*k/220);
  sun_bloom_dot(x+dx,y+dy,c,0,top,W,bot);
 }
}
static void rift_portrait(int cx,int cy,float radius,int type,float time,int top,int bot){
 static const unsigned colors[][3]={{0xffb14b,0xe755ba,0xfcebb9},{0x79d9f5,0xd4d650,0xdaf8ff},{0xad50ed,0x438cfa,0xc4e7ff},{0xffb878,0xe3daba,0xfff8e0}};
 const unsigned *c=colors[type&3];if(radius<3)radius=3;if(radius>160)radius=160;
 int segments=radius<15?16:48,glow=radius<20?1:radius<60?2:3;
 /* Eight differently inclined braided lobes form a porous, asymmetric field. */
 for(int layer=0;layer<8;layer++)for(int j=0;j<segments;j++){
  float a=j*6.2831853f/segments,phase=time*(.09f+layer*.007f)+layer*.73f;
  float r=radius*(.32f+layer*.079f)*(1+.14f*sinf(a*(3+type)+phase));
  float tilt=.32f+layer*.39f;
  float u=cosf(a+phase)*r,v=sinf(a+phase)*r*(.24f+.065f*(layer%4));
  int x=cx+(int)(u*cosf(tilt)-v*sinf(tilt)),y=cy+(int)(u*sinf(tilt)+v*cosf(tilt));
  rift_glow(x,y,glow,c[layer%2],top,bot);
  if((j+layer)%13==0)rift_glow(x,y,glow+2,c[2],top,bot);
 }
 /* Sparse drifting dust outside the filaments, plus a breathing diffuse heart. */
 for(int i=0;i<36;i++){float a=i*2.399963f+time*.025f,r=radius*(.18f+(i%13)*.083f);int x=cx+cosf(a)*r,y=cy+sinf(a)*r*.77f;rift_glow(x,y,i%9?1:3,c[i%3],top,bot);}
 for(int i=0;i<9;i++){float a=i*.698f+time*.1f;rift_glow(cx+cosf(a)*radius*.13f,cy+sinf(a)*radius*.09f,glow+3,c[0],top,bot);}
}
static void rift_fields(void){
 for(int i=0;i<ANOMALY_COUNT;i++){Anomaly *a=&game.anomaly[i];if(!a->alive||occluded(a->pos))continue;
  Vec3 v=camera(&game,a->pos);if(v.z<=8||length(v)>18000)continue;Point p=project(v);
  float radius=fminf(150,240*260/fmaxf(80,v.z));
  if(p.x<-radius||p.x>W+radius||p.y<view_top()-radius||p.y>view_bot()+radius)continue;
  rift_portrait(p.x,p.y,radius,rift_type(game.system,i),game.time+i*7,view_top(),view_bot());
 }
}
static void rift_report_view(void){
 int slot=game.rift_report-1;if(slot<0||slot>=ANOMALY_COUNT)return;
 int type=rift_type(game.system,slot);
 rect(0,0,W,H,BG);dialogue_begin("ANALYSIS / SHIP COMPUTER");
 dialogue_speech("SHIP COMPUTER",EXPLORERS,17,rift_reports[type],rift_lore[type]);
 dialogue_context(rift_names[type],"Scan filed under Discovery Codex > system > Space Signals. This is an instrument report, not an incoming conversation.");
 rift_portrait(242,211,30,type,preview_time,181,242);
 footer("L/R READ   TRI CLOSE   O CLOSE");
}
