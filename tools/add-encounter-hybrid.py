from pathlib import Path
p=Path('src/encounter-custom.h');s=p.read_text();needle='encounter_box(c,(Vec3){0,r*1.02f,0},(Vec3){r*.37f,65,100},yaw+.3f*sinf(game.time*.2f),paint,family,2);';s=s.replace(needle,needle+'''
  Vec3 dish={0,r*1.25f,0};float turn=yaw+.3f*sinf(game.time*.2f);
  for(int k=0;k<12;k++){float a=k*.5235988f,b=(k+1)*.5235988f;Vec3 inner={cosf(a)*r*.12f,sinf(a)*r*.12f,0},outer={cosf(a)*r*.85f,sinf(a)*r*.85f,-r*.24f},next={cosf(b)*r*.85f,sinf(b)*r*.85f,-r*.24f},ni={cosf(b)*r*.12f,sinf(b)*r*.12f,0};discovery_design_quad(c,add(dish,inner),add(dish,outer),add(dish,next),add(dish,ni),turn,paint,ENCOUNTER_TILE(family,2),0);}
  encounter_box(c,add(dish,(Vec3){0,0,-r*.5f}),(Vec3){45,45,r*.18f},turn,paint,family,3);
''')
s+='''
/* Hybrid presentation keeps the physical silhouette in 3D. Small animated
 * sprite masks supply readable screens/fauna rather than expensive micro-meshes. */
static int encounter_sprite_enabled=1;
static void encounter_sprite(Vec3 world,int kind,float size,unsigned ink){
 static const unsigned short masks[4][8]={{0x0180,0x03c0,0x07e0,0x0db0,0x0990,0x03c0,0x0660,0x0c30},{0x0180,0x03c0,0x07e0,0x0990,0x0db0,0x03c0,0x0660,0x0c30},{0x0180,0x07e0,0x0ff0,0x1ff8,0x3ffc,0x0180,0x0180,0x03c0},{0x0660,0x0ff0,0x03c0,0x0180,0x03c0,0x0660,0x0c30,0x1818}};
 Vec3 v=camera(&game,world);if(v.z<30)return;Point p=project(v);int scale=(int)(size*proj_focal/v.z);if(scale<1)return;if(scale>4)scale=4;unsigned depth=surface_depth_on?surface_encode_depth(1.f/v.z):mega_encode_depth(v.z);
 for(int y=0;y<8;y++)for(int x=0;x<16;x++)if(masks[kind&3][y]&(1u<<(15-x)))for(int yy=0;yy<scale;yy++)for(int xx=0;xx<scale;xx++){int sx=p.x+(x-8)*scale+xx,sy=p.y+(y-4)*scale+yy;if(sx<0||sx>=W||sy<view_top()||sy>=view_bot())continue;if((surface_depth_on||mega_depth_valid)&&depth>surface_depth[sy*W+sx]+16)continue;pixel(sx,sy,ink);}
}
static void encounter_sprite_details(Vec3 c,int entry,float yaw){
 if(!encounter_sprite_enabled)return;
 if(entry==17)encounter_sprite(add(c,discovery_yaw((Vec3){0,140,-110},yaw)),(int)(game.time*2)&1,25,RGB(100,204,216));
 else if(entry==63)for(int i=0;i<3;i++)encounter_sprite(add(c,discovery_yaw((Vec3){(i-1)*370,sinf(game.time*.4f+i)*230, -140},yaw)),2,15,RGB(104,209,157));
 else if(entry==2&&((int)(game.time*5)&3)==0)encounter_sprite(add(c,discovery_yaw((Vec3){0,310,-100},yaw)),3,18,RGB(240,189,95));
}
''';p.write_text(s)
p=Path('src/flight-extras.h');s=p.read_text().replace('if(entry==60)discovery_listening_post(centre);','encounter_sprite_details(centre,entry,(discovery_seed(i)&255)*.006f);\n if(entry==60)discovery_listening_post(centre);');p.write_text(s)
