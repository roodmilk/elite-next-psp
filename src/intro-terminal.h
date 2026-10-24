static void intro_terminal_ship(int x,int y,int w,int h,int ship,float t,int wire){
 int ox=proj_ox,oy=proj_oy,cx0=clipx0,cy0=clipy0,cx1=clipx1,cy1=clipy1;
 preview_clip(x+w/2,y+h/2,x,y,x+w,y+h);
 int id=mesh_id(player_ships[ship].name);const Mesh *m=&meshes[id];Vec3 centre;float scale;menu_hull_layout(id,h,&centre,&scale);
 float yaw=t*.7f+.5f,tilt=-.38f;drawcount=0;
 for(int i=0;i<m->triangles;i++){const MeshTri *q=&m->t[i];Vec3 v[3];int ids[]={q->a,q->b,q->c};for(int k=0;k<3;k++)v[k]=add((Vec3){0,0,320},mul(menu_hull_rotate(sub(m->v[ids[k]],centre),yaw,tilt,0),scale));Vec3 n=menu_hull_rotate(q->normal,yaw,tilt,0);if(dot(n,mul(add(add(v[0],v[1]),v[2]),-1.f/3))<=0)continue;
 if(wire){for(int k=0;k<3;k++){Point a=project(v[k]),b=project(v[(k+1)%3]);line(a.x,a.y,b.x,b.y,RGB(26,154,175));}}
 else{float l=.28f+.72f*fmaxf(0,dot(n,norm((Vec3){-.3f,.8f,-.6f})));queue_triangle(v[0],v[1],v[2],RGB(100*l,200*l,220*l));}}
 if(!wire)flush_meshes();preview_clip(ox,oy,cx0,cy0,cx1,cy1);
}
/* Native pixel symbols, bounded to a 12 by 10 cell. */
static void intro_terminal_symbol(int x,int y,int location,unsigned ink){
 if(location){line(x+3,y,x+8,y,ink);line(x+1,y+2,x+1,y+5,ink);line(x+10,y+2,x+10,y+5,ink);line(x+2,y+6,x+5,y+9,ink);line(x+9,y+6,x+6,y+9,ink);rect(x+4,y+3,3,3,ink);}
 else {line(x+1,y+7,x+5,y+1,ink);line(x+5,y+1,x+10,y+7,ink);line(x+1,y+7,x+5,y+5,ink);line(x+10,y+7,x+5,y+5,ink);rect(x+4,y+7,3,2,CYAN);}
}

static void intro_terminal(int slot,int saved,int browsing){
 int fresh=!browsing&&intro_choice==1;unsigned ink=GOLD;
 int ship=saved&&!fresh?profile_info[slot].ship:0;if(ship<0||ship>=player_ship_count)ship=0;
 intro_strip_panel(258,145,206,99,ink,225);
 for(int i=0;i<3;i++)rect(435+i*8,153,4,3,i==(int)(intro_time*2)%3?GOLD:RGB(28,63,72));
 rect(266,163,190,13,RGB(36,29,20));rect(266,163,2,13,GOLD);
 char name[25];intro_name(name,sizeof(name),saved?profile_info[slot].name:"");
 text_px(273,166,GOLD,"%.15s",fresh?"NEW COMMANDER":saved?name:"EMPTY SLOT");
 text_px(411,166,CYAN,saved&&!fresh?"LINK":"----");
 for(int k=0;k<4;k++)line(270,184+k*10,343,184+k*10,RGB(10,38,49));
 intro_terminal_ship(268,181,78,38,ship,intro_time,browsing&&!saved);
 int sy=182+(int)(intro_time*15)%36;line(270,sy,343,sy,RGB(20,109,124));line(349,182,349,224,RGB(31,75,83));
 intro_terminal_symbol(358,181,0,GOLD);text_px(376,182,CYAN,"%.10s",saved&&!fresh?player_ships[ship].name:fresh?"UNASSIGNED":"--");
 int system=saved?profile_info[slot].system:7;if(system<0||system>255)system=7;
 intro_terminal_symbol(358,195,1,GOLD);text_px(376,196,WHITE,"%.10s",fresh?"EARTH":saved?profile_info[slot].opening_stage>0&&profile_info[slot].opening_stage<OP_WAKE?"SOL":game.systems[system].name:"NO DATA");
 rect(356,209,99,14,RGB(20,38,42));rect(356,209,2,14,CYAN);
 if(saved&&!fresh)text_px(364,213,GOLD,"%.9g U",profile_info[slot].credits*.1f);else text_px(364,213,GOLD,"-- U");
 if(!fresh)text_px(271,222,RGB(85,151,164),saved?"RECORDED":"NO LINK");
 line(266,232,456,232,RGB(48,65,66));
 if(fresh)text_px(271,235,GOLD,"X BEGIN YOUR STORY");
 else {text_px(269,235,GOLD,"L <");for(int i=0;i<3;i++){int xx=309+i*25,selected=i==(browsing?intro_load_slot:slot);rect(xx,234,20,8,selected?RGB(60,45,20):RGB(12,31,40));text_px(xx+6,235,selected?GOLD:RGB(62,105,116),"%d",i+1);}text_px(390,235,GOLD,"> R");text_px(428,235,CYAN,"LOAD");}
}
