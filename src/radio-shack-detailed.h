/* Continuous side-specific facade paintings, cut into native32px storage tiles.
 * No texture repeats within a facade or rounded pressure-hull skin. */
#include "radio-shack-art.h"
static Vec3 radio_lerp(Vec3 a,Vec3 b,float t){return add(a,mul(sub(b,a),t));}
static Vec3 radio_surface(Vec3 a,Vec3 b,Vec3 c,Vec3 d,float u,float v){return radio_lerp(radio_lerp(a,b,u),radio_lerp(d,c,u),v);}
static void radio_paint_surface(Vec3 centre,Vec3 a,Vec3 b,Vec3 c,Vec3 d,float yaw,unsigned paint,int first,int columns,int rows){
 for(int y=0;y<rows;y++)for(int x=0;x<columns;x++){
  float u=x/(float)columns,v=y/(float)rows,uu=(x+1)/(float)columns,vv=(y+1)/(float)rows;
  discovery_design_quad(centre,radio_surface(a,b,c,d,u,v),radio_surface(a,b,c,d,uu,v),radio_surface(a,b,c,d,uu,vv),radio_surface(a,b,c,d,u,vv),yaw,paint,first+y*columns+x,0);
 }
}
static Vec3 radio_ball_vertex(int column,int row){
 static const float latitude[8]={.75f,1.08f,1.41f,1.74f,2.07f,2.4f,2.73f,3.08f};float a=column*.39269908f,p=latitude[row];
 return (Vec3){cosf(a)*sinf(p)*870,-650+cosf(p)*870,sinf(a)*sinf(p)*870};
}
static void radio_pressure_ball(Vec3 centre,float yaw){
 unsigned hull=RGB(147,164,170);
 for(int r=0;r<7;r++)for(int k=0;k<16;k++){
  discovery_design_quad(centre,radio_ball_vertex(k,r),radio_ball_vertex(k+1,r),radio_ball_vertex(k+1,r+1),radio_ball_vertex(k,r+1),yaw,hull,RADIO_SPHERE+r*16+k,0);
 }
 for(int k=0;k<16;k++){
  Vec3 pole={0,-1520,0};discovery_design_quad(centre,radio_ball_vertex(k,7),radio_ball_vertex(k+1,7),pole,pole,yaw,hull,RADIO_SPHERE+96+k,0);
 }
}
static void radio_platform(Vec3 centre,float yaw){
 unsigned deck=RGB(120,139,146),edge=RGB(164,174,169);
 for(int k=0;k<12;k++){
  float a=k*.52359878f,b=(k+1)*.52359878f;
  Vec3 aa={cosf(a)*1230,0,sinf(a)*1050},bb={cosf(b)*1230,0,sinf(b)*1050};
  Vec3 lowa=aa,lowb=bb;lowa.y=lowb.y=-110;
  discovery_design_quad(centre,(Vec3){0,0,0},bb,aa,(Vec3){0,0,0},yaw,deck,RADIO_DECK+k,0);
  discovery_design_quad(centre,aa,bb,lowb,lowa,yaw,edge,RADIO_RIM+k,0);
  /* Actual raised edge rails; a clear gap marks the small front landing apron. */
  if(k!=8&&k!=9){
   Vec3 post=mul(aa,.96f);post.y=0;Vec3 top=post;top.y=100;
   encounter_arm(centre,post,top,9,yaw,edge);
   Vec3 next=mul(bb,.96f);next.y=100;encounter_arm(centre,top,next,7,yaw,edge);
  }
 }
}
static void radio_occupied_cabin(Vec3 centre,float yaw){
 unsigned wall=RGB(187,190,169),roof=RGB(128,151,159);
 /* Roof slopes across the full building; the side art follows that trapezoid. */
 radio_paint_surface(centre,(Vec3){-670,540,-420},(Vec3){490,540,-420},(Vec3){490,40,-420},(Vec3){-670,40,-420},yaw,wall,RADIO_FRONT,3,2);
 radio_paint_surface(centre,(Vec3){490,650,380},(Vec3){-670,650,380},(Vec3){-670,40,380},(Vec3){490,40,380},yaw,wall,RADIO_REAR,3,2);
 radio_paint_surface(centre,(Vec3){-670,650,380},(Vec3){-670,540,-420},(Vec3){-670,40,-420},(Vec3){-670,40,380},yaw,wall,RADIO_LEFT,2,2);
 radio_paint_surface(centre,(Vec3){490,540,-420},(Vec3){490,650,380},(Vec3){490,40,380},(Vec3){490,40,-420},yaw,wall,RADIO_RIGHT,2,2);
 radio_paint_surface(centre,(Vec3){-700,660,405},(Vec3){520,660,405},(Vec3){520,550,-445},(Vec3){-700,550,-445},yaw,roof,RADIO_ROOF,3,2);
 /* A physical doorway canopy and a low equipment bay break the box outline. */
 discovery_design_box(centre,(Vec3){-490,410,-505},(Vec3){165,30,95},yaw,wall,RADIO_EQUIPMENT+5,RADIO_EQUIPMENT+6,RADIO_EQUIPMENT+7,0);
 discovery_design_box(centre,(Vec3){-860,140,210},(Vec3){165,140,300},yaw,roof,RADIO_EQUIPMENT,RADIO_EQUIPMENT+1,RADIO_EQUIPMENT+2,0);
 discovery_design_box(centre,(Vec3){-470,22,-650},(Vec3){230,22,210},yaw,roof,RADIO_EQUIPMENT+5,RADIO_EQUIPMENT+6,RADIO_EQUIPMENT+7,0);
}
static void radio_receiving_dish(Vec3 centre,float yaw){
 unsigned paint=RGB(192,199,179);float turn=yaw+.13f*sinf(game.time*.12f);
 Vec3 p={820,470,230},dish_world=add(centre,discovery_yaw(p,yaw));
 discovery_design_box(centre,(Vec3){820,160,230},(Vec3){90,160,100},yaw,paint,RADIO_EQUIPMENT+3,RADIO_EQUIPMENT+4,RADIO_EQUIPMENT+5,0);
 encounter_arm(centre,(Vec3){820,240,230},p,38,yaw,paint);
 for(int k=0;k<8;k++){
  float a=k*.78539816f,b=(k+1)*.78539816f;
  Vec3 aa={cosf(a)*65,sinf(a)*65,0},bb={cosf(a)*335,sinf(a)*335,-115},cc={cosf(b)*335,sinf(b)*335,-115},dd={cosf(b)*65,sinf(b)*65,0};
  discovery_design_quad(dish_world,aa,dd,cc,bb,turn,paint,RADIO_DISH_FRONT+k,0);
  discovery_design_quad(dish_world,bb,cc,dd,aa,turn,paint,RADIO_DISH_BACK+k,0);
 }
 Vec3 focus={0,0,-245};
 for(int k=0;k<3;k++){float a=k*2.0943951f;encounter_arm(dish_world,(Vec3){cosf(a)*300,sinf(a)*300,-115},focus,8,turn,paint);}
 discovery_design_box(dish_world,focus,(Vec3){30,30,45},turn,paint,RADIO_EQUIPMENT+3,RADIO_EQUIPMENT+4,RADIO_EQUIPMENT+5,0);
}
static void radio_shack_detailed(Vec3 centre,float yaw){
 radio_pressure_ball(centre,yaw);radio_platform(centre,yaw);radio_occupied_cabin(centre,yaw);radio_receiving_dish(centre,yaw);
 unsigned mast=RGB(158,172,164);
 /* A short braced mast transfers its load to the deck behind the building. */
 Vec3 peak={-380,1090,630};
 encounter_arm(centre,(Vec3){-550,0,720},peak,19,yaw,mast);
 encounter_arm(centre,(Vec3){-190,0,720},peak,19,yaw,mast);
 encounter_arm(centre,(Vec3){-380,0,470},peak,19,yaw,mast);
 for(int k=0;k<3;k++){
  float y=650+k*180;encounter_arm(centre,(Vec3){-570+k*35,y,630},(Vec3){-190-k*35,y,630},10,yaw,mast);
 }
 encounter_lamp(centre,peak,yaw,((int)(game.time*1.3f)&1)?RGB(227,145,83):RGB(86,70,54));
 for(int k=0;k<3;k++)encounter_lamp(centre,(Vec3){-660+k*190,55,-895},yaw,RGB(108,206,210));
}
