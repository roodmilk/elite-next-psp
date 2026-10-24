/* Circuit Console: approved native 480x272 loadout / separate Item Storage.
 * No inventory rules live here: fit[] and module_store[] remain authoritative. */
#define LC_BG (ship_theme_index()?ui_paint_surface(RGB(8,15,23)):RGB(8,15,23))
#define LC_METAL (ship_theme_index()?UI_RAISED:RGB(25,37,45))
#define LC_PANEL (ship_theme_index()?UI_PANEL:RGB(16,28,37))
#define LC_RAISE (ship_theme_index()?UI_RAISED:RGB(41,58,66))
#define LC_EDGE (ship_theme_index()?UI_EDGE:RGB(67,86,90))
#define LC_INK (ship_theme_index()?UI_TEXT:RGB(238,224,189))
#define LC_DIM (ship_theme_index()?UI_MUTED:RGB(147,167,167))
#define LC_FAINT (ship_theme_index()?UI_EDGE:RGB(75,101,108))
#define LC_GOLD (ship_theme_index()?UI_ACCENT:RGB(244,181,92))
#define LC_CYAN (ship_theme_index()?UI_SIGNAL:RGB(112,199,195))
#define LC_GREEN RGB(149,204,148)
static const char *lc_cats[]={"WPN","DEF","NAV","HOLD","FUEL","UTIL"};
static unsigned lc_cat_ink(int c){static const unsigned ink[]={RGB(233,135,117),RGB(134,180,220),RGB(112,199,195),RGB(221,189,121),RGB(221,160,115),RGB(179,160,208)};return c>=0&&c<6?ink[c]:LC_INK;}
static int module_selected_item(void);
static void lc_plate(int x,int y,int w,int h){rect(x,y,w,h,LC_METAL);rect(x,y,w,1,LC_EDGE);rect(x,y+h-1,w,1,LC_EDGE);rect(x,y,1,h,LC_EDGE);rect(x+w-1,y,1,h,LC_EDGE);line(x+1,y+1,x+w-2,y+1,RGB(103,116,109));line(x+1,y+h-2,x+w-2,y+h-2,LC_BG);}
static void lc_inset(int x,int y,int w,int h){rect(x,y,w,h,LC_BG);line(x,y,x+w-1,y,LC_FAINT);line(x,y,x,y+h-1,LC_FAINT);}
static void lc_screw(int x,int y){rect(x,y,4,4,RGB(51,67,74));line(x,y+1,x+2,y+1,RGB(116,130,126));pixel(x+3,y+3,LC_BG);}
static void lc_base(const char *title){
 rect(0,0,W,H,LC_BG);lc_plate(0,0,480,272);
 for(int y=24;y<248;y+=4)line(4,y,475,y,ui_paint_surface(RGB(27,41,49)));
 rect(6,4,468,18,LC_PANEL);rect(6,4,3,18,LC_GOLD);
 text_px(14,9,LC_GOLD,"ELITE: NEXT");text_px(118,9,LC_INK,"/ %s",title);line(8,24,471,24,LC_FAINT);
 for(int x=2;x<=474;x+=472){lc_screw(x,2);lc_screw(x,245);lc_screw(x,266);}
 rect(8,249,464,19,LC_PANEL);line(8,249,471,249,LC_FAINT);
}
static void lc_hint(int x,char button,const char *label){button_icon(x,254,button,LC_CYAN);text_px(x+17,257,LC_DIM,"%s",label);}
static int lc_fitted(void){int n=0;for(int s=0;s<FIT_SLOTS;s++)n+=fit_slot_open(&game,s)&&game.fit[s]!=FIT_EMPTY;return n;}
static int lc_banks(void){int n=0;for(int c=0;c<6;c++)n+=fit_capacity(game.ship,c);return n;}
static void lc_stamp_extent(Vec3 p,Vec3 *lo,Vec3 *hi){p=rotate(p,.55f,.12f);lo->x=fminf(lo->x,p.x);lo->y=fminf(lo->y,p.y);lo->z=fminf(lo->z,p.z);hi->x=fmaxf(hi->x,p.x);hi->y=fmaxf(hi->y,p.y);hi->z=fmaxf(hi->z,p.z);}
static void lc_ship_stamp(void){
 lc_inset(304,29,168,28);
 for(int x=309;x<470;x+=10)line(x,31,x,54,ui_paint_surface(RGB(23,46,54)));for(int y=32;y<55;y+=8)line(306,y,469,y,ui_paint_surface(RGB(23,46,54)));
 /* Fit the actual rotated geometry, not its much larger bounding sphere.
    Include perspective depth in the scale so every hull stays inside the
    aperture without reducing a long capital ship to a four-pixel fleck. */
 preview_clip(388,43,306,31,470,55);int art=mega_hull_art_id(player_ships[game.ship].name);unsigned paint=ship_paint[game.ship];
 float yaw=.55f,roll=.12f;Vec3 lo={1e9f,1e9f,1e9f},hi={-1e9f,-1e9f,-1e9f};int id=mesh_id(player_ships[game.ship].name);
 if(art>=0){const MegaHullArt *h=&mega_hull_art[art];for(int i=0;i<h->parts;i++){const MegaHullPart *b=&h->part[i];for(int k=0;k<8;k++){float nose=(k&4)?.62f:1.f;Vec3 p={b->x+((k&1)?b->hx:-b->hx)*nose,b->y+((k&2)?b->hy:-b->hy)*(.78f+.22f*nose),b->z+((k&4)?b->hz:-b->hz)};lc_stamp_extent(p,&lo,&hi);}}}
 else {const Mesh*m=fleet_visual_mesh(id);for(int i=0;i<m->vertices;i++)lc_stamp_extent(m->v[i],&lo,&hi);}
 Vec3 centre=mul(add(lo,hi),.5f),half=mul(sub(hi,lo),.5f);float scale=fminf(75.f*320.f/(240.f*fmaxf(1,half.x)+75.f*half.z),9.f*320.f/(240.f*fmaxf(1,half.y)+9.f*half.z));Vec3 pos=sub((Vec3){0,0,320},mul(centre,scale));
 if(art>=0){const MegaHullArt *h=&mega_hull_art[art];for(int i=0;i<h->parts;i++)mega_art_block(&h->part[i],yaw,roll,scale,pos,paint);}else shipmesh(id,pos,yaw,roll,scale,paint,1);
 flush_meshes();preview_reset();
}
static void lc_status(void){text_px(16,32,LC_CYAN,"%.29s",player_ships[game.ship].name);text_px(16,45,LC_DIM,"FITTED %d/%d",lc_fitted(),lc_banks());text_px(156,45,LC_DIM,"HOLD %d/%dT",cargo_used(&game),cargo_capacity(&game));lc_ship_stamp();}
/* Shared word wrapping reports overflow to the test harness, rather than
   hiding a cut-off stat. All catalogue effects fit two 34-character lines. */
static int lc_copy(int x,int y,int cols,int rows,unsigned ink,const char *s,int draw){
 int used=0;while(s&&*s&&used<rows){int len=(int)strlen(s),cut=len<cols?len:cols;
  if(len>cols){int k=cut;while(k>0&&s[k]!=' ')k--;if(k>0)cut=k;}
  if(draw)text_px(x,y+used*10,ink,"%.*s",cut,s);s+=cut;while(*s==' ')s++;used++;
 }return s&&*s?-1:used;
}
static void loadout_picker_draw(void){
 int list[EQUIPMENT_COUNT+DB_ITEMS],n=loadout_picker_list(list,EQUIPMENT_COUNT+DB_ITEMS);
 if(loadout_picker_row<0)loadout_picker_row=0;if(n&&loadout_picker_row>=n)loadout_picker_row=n-1;
 lc_plate(242,59,232,188);lc_inset(247,63,222,179);
 text_px(253,67,LC_GOLD,"ITEM STORAGE / %s",lc_cats[loadout_picker_slot%6]);
 text_px(253,80,LC_CYAN,"FIT TO BANK %d  / %d ITEMS",loadout_picker_slot/6+1,n);
 line(252,91,464,91,LC_EDGE);
 int first=loadout_picker_row<5?0:loadout_picker_row-4;
 for(int k=0;k<5&&first+k<n;k++){int token=list[first+k],y=98+k*19,sel=first+k==loadout_picker_row;
  if(sel){rect(251,y-2,213,18,LC_RAISE);rect(251,y-2,2,18,LC_GOLD);}
  tech_module_icon(256,y-1,module_item_base(token));module_rare_shine(256,y-1,16,16,token);
  text_px(277,y+3,cosmetic_ink(module_item_base(token),sel?LC_GOLD:LC_INK),"%.23s",module_item_short(token));
 }
 if(!n){lc_copy(253,101,25,3,LC_DIM,"No compatible stored items. Buy modules in Outfitting.",1);}
 line(252,195,464,195,LC_EDGE);
 if(preview_time<loadout_store_until)lc_copy(253,202,26,4,LC_GOLD,loadout_store_note,1);
 else if(n){int token=list[loadout_picker_row];lc_copy(253,202,26,3,cosmetic_ink(module_item_base(token),LC_INK),module_item_name(token),1);text_px(253,234,LC_DIM,"%d/%d   OWNED %d",loadout_picker_row+1,n,module_item_count(token));}
 rect(8,250,464,18,LC_PANEL);lc_hint(14,'P',"ITEM");if(n)lc_hint(120,'X',"FIT");lc_hint(220,'O',"CANCEL");
}
static void loadout_console_draw(void){
 if(row<0||row>=FIT_SLOTS||!fit_slot_open(&game,row))row=0;
 lc_base("SHIP LOADOUT");lc_status();text_px(16,64,LC_FAINT,"BANK");
 for(int b=0;b<8;b++)text_px(90+b*47,64,LC_DIM,"%d",b+1);
 for(int c=0;c<6;c++){
  int y=77+c*20;unsigned cat=lc_cat_ink(c);rect(10,y,60,17,LC_PANEL);rect(10,y,2,17,cat);text_px(18,y+5,c==row%6?LC_GOLD:cat,"%s",lc_cats[c]);
  for(int b=0;b<fit_capacity(game.ship,c);b++){
   int slot=c+b*6,x=76+b*47,item=game.fit[slot],sel=slot==row;
   if(sel)rect(x-2,y-2,47,21,LC_GOLD);
   rect(x,y,43,17,LC_EDGE);rect(x+1,y+1,41,15,item!=FIT_EMPTY?LC_RAISE:LC_BG);
   if(item!=FIT_EMPTY&&fit_value_valid(slot,item)){
    tech_module_icon(x+2,y+1,item);module_rare_shine(x+2,y+1,16,16,module_slot_token(slot));text_px(x+18,y+5,cosmetic_ink(item,LC_INK),"%.3s",equipment_list_names[item]);
    for(int k=0;k<4;k++)rect(x+4+k*9,y+15,3,1,cat);
    if(c==FIT_WPN&&slot==game.active_weapon)rect(x+38,y+2,3,3,LC_GREEN);
   }else{line(x+18,y+8,x+24,y+8,LC_FAINT);line(x+21,y+5,x+21,y+11,LC_FAINT);}
  }
 }
 int item=game.fit[row],active=row==game.active_weapon&&item!=FIT_EMPTY;
 lc_plate(8,201,294,45);lc_inset(12,205,286,40);
 if(dashboard_item(item)){text_px(18,207,CHARM_LILAC,"DASHBOARD COSMETIC MODULE");text_px(266,207,LC_DIM,"%d/%d",row/6+1,fit_capacity(game.ship,row%6));}else text_px(18,207,active?LC_GREEN:LC_GOLD,"%s %d/%d%s",lc_cats[row%6],row/6+1,fit_capacity(game.ship,row%6),active?" / ACTIVE":"");
 if(charm_item(item))text_px(154,207,CHARM_LILAC,"COSMETIC MODULE");
 if(preview_time<loadout_store_until)lc_copy(18,217,34,3,LC_INK,loadout_store_note,1);
 else if(item==FIT_EMPTY||!fit_value_valid(row,item)){text_px(18,217,LC_CYAN,"EMPTY BANK");text_px(18,228,LC_DIM,"FIT AN ITEM FROM STORAGE >>");}
 else {text_px(18,217,cosmetic_ink(item,LC_GOLD),"%.34s",module_item_name(module_slot_token(row)));lc_copy(18,226,34,2,LC_INK,module_item_effect(module_slot_token(row)),1);}
 int used=module_store_total(),cap=module_store_capacity(),full=used>=cap;
 lc_plate(310,201,162,45);button_icon(320,211,'T',LC_GREEN);text_px(337,208,LC_INK,"ITEM STORAGE");text_px(337,220,full?RED:LC_CYAN,"%d/%d ITEMS",used,cap);
 rect(337,235,123,3,LC_BG);if(cap&&used)rect(337,235,123*(used>cap?cap:used)/cap,3,full?RED:LC_GREEN);
 lc_hint(14,'P',"SLOT");lc_hint(96,'X',"FIT");lc_hint(168,'S',"STORE");lc_hint(260,'T',"STORAGE");lc_hint(396,'O',"BACK");
 if(loadout_picker_slot>=0)loadout_picker_draw();
}
static void storage_console_draw(void){
 int list[EQUIPMENT_COUNT],n=module_owned_list(list,EQUIPMENT_COUNT),item=module_selected_item();
 int cat=item>=0?equip_slot_for(module_item_base(item)):row>=0&&row<FIT_SLOTS?row%6:FIT_WPN,cap=fit_capacity(game.ship,cat);
 if(module_bank<0)module_bank=0;if(module_bank>=cap)module_bank=cap-1;
 lc_base("ITEM STORAGE");lc_status();lc_plate(8,64,184,136);lc_plate(200,64,272,136);
 text_px(17,70,lc_cat_ink(cat),"%s BANKS",lc_cats[cat]);text_px(210,70,LC_GOLD,"STORED ITEMS");
 char occupied[16];snprintf(occupied,sizeof(occupied),"%d/%d",module_store_total(),module_store_capacity());text_px(464-(int)strlen(occupied)*8,70,LC_DIM,"%s",occupied);
 for(int b=0;b<cap;b++){
  int y=85+b*14,slot=cat+b*6,mod=game.fit[slot],sel=storage_bank_focus&&b==module_bank;
  if(sel){rect(16,y-2,168,13,LC_RAISE);rect(16,y-2,2,13,LC_GOLD);}
  text_px(21,y,sel?LC_GOLD:LC_FAINT,"%d",b+1);text_px(39,y,cosmetic_ink(mod,sel?LC_INK:LC_DIM),"%.16s",mod==FIT_EMPTY||!fit_value_valid(slot,mod)?"EMPTY":equipment_list_names[mod]);
  if(cat==FIT_WPN&&slot==game.active_weapon&&mod!=FIT_EMPTY)rect(175,y+2,3,3,LC_GREEN);
 }
 int visible=6,first=module_inventory_row<visible?0:module_inventory_row-visible+1;if(first>n-visible)first=n-visible;if(first<0)first=0;
 if(n){int thumb=n<=visible?111:111*visible/n;if(thumb<8)thumb=8;rect(205,84,2,111,LC_EDGE);rect(205,84+(n>visible?(111-thumb)*first/(n-visible):0),2,thumb,LC_CYAN);}
 for(int j=0;j<visible&&first+j<n;j++){
  int id=list[first+j],y=84+j*19,sel=first+j==module_inventory_row;
  if(sel){rect(211,y-2,253,18,LC_RAISE);rect(211,y-2,2,18,storage_bank_focus?LC_CYAN:LC_GOLD);}
  char qty[8];snprintf(qty,sizeof(qty),"X%d",module_item_count(id));int qtyx=421-(int)strlen(qty)*8,cols=(qtyx-5-237)/8;
  tech_module_icon(217,y-1,module_item_base(id));module_rare_shine(217,y-1,16,16,id);text_px(237,y+3,cosmetic_ink(module_item_base(id),sel?(storage_bank_focus?LC_CYAN:LC_GOLD):LC_INK),"%.*s",cols,module_item_short(id));
  text_px(qtyx,y+3,LC_DIM,"%s",qty);text_px(430,y+3,lc_cat_ink(equip_slot_for(module_item_base(id))),"%s",lc_cats[equip_slot_for(module_item_base(id))]);
 }
 if(!n){text_px(218,88,LC_DIM,"NO STORED MODULES");lc_copy(218,109,29,5,LC_INK,"Buy modules through Outfitting, then fit them here.",1);}
 /* Approved lower strip: selected module only. No comparison or duplicate
    instructions/capacity; two lines allow every real effect to remain whole. */
 lc_plate(8,205,464,39);lc_inset(12,209,456,31);
 if(item>=0){text_px(18,211,cosmetic_ink(module_item_base(item),LC_GOLD),"%.55s",module_item_name(item));lc_copy(18,224,55,2,LC_INK,module_item_effect(item),1);}
 else text_px(18,213,LC_DIM,"ITEM STORAGE IS EMPTY");
 lc_hint(14,'P',storage_bank_focus?"BANK":"ITEM");if(game.docked)lc_hint(114,'X',storage_bank_focus?"FIT":"SELECT");lc_hint(226,'O',storage_bank_focus?"ITEMS":"BACK");
 char credits[24];snprintf(credits,sizeof(credits),"%.1f U",game.credits*.1f);lc_inset(341,252,123,13);text_px(464-(int)strlen(credits)*8,255,LC_GOLD,"%s",credits);
}
