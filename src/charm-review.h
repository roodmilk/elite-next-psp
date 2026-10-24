#if defined(ELITE_ENABLE_REVIEW)||defined(ELITE_ENCOUNTER_REVIEW)
static int charm_review_select(int base){int a[EQUIPMENT_COUNT],n=module_owned_list(a,EQUIPMENT_COUNT);for(int i=0;i<n;i++)if(module_item_base(a[i])==base){module_inventory_row=i;return 1;}return 0;}
static void charm_review_footage(FILE*out){
 for(int item=0;item<4;item++){
  hc_test_setup();game.ship=0;game.docked=1;fit_clear_all(&game);game.fit[0]=1;fit_rebuild(&game);game.credits=10000;
  module_buy_to_store(CHARM_UFO+item);page=STORAGE;charm_review_select(CHARM_UFO+item);module_bank=0;module_swap_selected();page=FLIGHT;game.docked=0;game.speed=100;hard_brake=0;r_tap=l_tap=10;
  for(int i=0;i<NPC_COUNT;i++)game.npc[i].alive=0;game.pos=(Vec3){0,0,-20000};game.dockbay.dirty=0;game.message_time=game.voice_time=0;charm_tick();
  unsigned old=0;FILE*csv=0;char path[80];snprintf(path,sizeof(path),"charm-%d-motion.csv",item);csv=fopen(path,"w");
  for(int frame=0;frame<240;frame++){
   unsigned held=frame>=90&&frame<120?PSP_CTRL_RTRIGGER:frame>=120&&frame<150?PSP_CTRL_LTRIGGER:frame==200||frame==220?PSP_CTRL_SQUARE:0;
   float ax=frame>=30&&frame<60?.75f:frame>=60&&frame<90?-.75f:0;
   if(frame==150){NPC*n=&game.npc[0];memset(n,0,sizeof(*n));n->alive=1;n->mesh=mesh_id("ADDER");n->radius=30;n->scale=1;n->mega_hull=-1;n->pos=add(game.pos,mul(forward(&game),5));n->dir=forward(&game);n->role=TRADERS;n->target=-1;n->bounty_slot=n->traveller=-1;n->health=100;n->shield=40;n->cooldown=100;}
   if(frame==151)game.npc[0].alive=0;
   float energy=game.energy;input(held&~old,held,1.f/30,ax,0);old=held;charm_tick();if(frame==150)game.npc[0].alive=0;space();
   if(frame==150)fprintf(out,"IMPACT %d shield_loss %.5f collision %.5f\n",item,energy-game.energy,game.collision);
   if(csv)fprintf(csv,"%d,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%d\n",frame,game.speed,game.yaw,charm_motion.angle,charm_motion.depth,game.energy,game.collision,hc_browser);
   snprintf(path,sizeof(path),"charm-%d-frame-%03d.rgba",item,frame);FILE*f=fopen(path,"wb");if(f){fwrite(fb,4,STRIDE*H,f);fclose(f);}
  }
  if(csv)fclose(csv);
 }
}
static int charm_review(void){
 FILE*flag=fopen("charm-review.flag","r");if(!flag)return 0;int mode=1;fscanf(flag,"%d",&mode);fclose(flag);FILE*out=fopen("charm-review.txt","w");if(!out)return 1;setvbuf(out,0,_IONBF,0);int fails=0,checks=0;
 #define CC(c,label) do{int ok=!!(c);checks++;fails+=!ok;fprintf(out,"%s %s\n",ok?"PASS":"FAIL",label);}while(0)
 hc_test_setup();game.docked=1;page=EQUIP;game.credits=100000;fit_clear_all(&game);memset(game.module_store,0,sizeof(game.module_store));memset(&game.dockbay,0,sizeof(game.dockbay));game.cargo[0]=0;
 CC(charm_installed(&game)<0&&module_store_total()==0,"no free installed charm or free owned charms");
 for(int item=CHARM_UFO;item<=CHARM_DUCK;item++){
  int stock[EQUIP_COUNT],n=equipment_stock_list(stock,EQUIP_COUNT);row=-1;for(int i=0;i<n;i++)if(stock[i]==item)row=i;
  CC(row>=0&&equip_slot_for(item)==FIT_UTIL,"Lave stocks approved charm in the existing Utility category");int cash=game.credits,count=module_store_total();input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
  CC(game.credits==cash-250&&module_store_total()==count+1&&charm_installed(&game)<0,"real shop X charges25U and stores without auto-installing");
 }
 module_buy_to_store(CHARM_UFO);CC(module_store_total()==5,"multiple copies and all four types can be owned together");
 int cash=game.credits;game.credits=0;CC(!module_buy_to_store(CHARM_DUCK)&&module_store_total()==5,"insufficient funds cannot add a free charm");game.credits=cash;
 page=STORAGE;storage_sell_mode=storage_bank_focus=0;charm_review_select(CHARM_UFO);module_bank=0;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 CC(game.fit[FIT_UTIL]==CHARM_UFO&&module_store_total()==4&&db_slot_roll(&game,FIT_UTIL),"two real storage confirmations consume one Utility bank and preserve instance identity");
 unsigned installed=game.dockbay.fitted[FIT_UTIL];charm_review_select(CHARM_ALIEN);module_bank=5;CC(!module_swap_selected()&&game.dockbay.fitted[FIT_UTIL]==installed&&game.fit[FIT_UTIL+30]==FIT_EMPTY&&module_store_total()==4,"different charm in a different bank is rejected without overwriting either item");
 charm_review_select(CHARM_UFO);module_bank=1;CC(!module_swap_selected()&&module_store_total()==4,"a second copy of the same charm is also rejected");
 page=INVENTORY;row=FIT_UTIL;input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);CC(charm_installed(&game)<0&&module_store_total()==5&&db_instance_stored(&game,installed-1),"real loadout Square returns the exact charm to Item Storage");
 page=STORAGE;charm_review_select(CHARM_DUCK);module_bank=5;CC(module_swap_selected()&&game.fit[35]==CHARM_DUCK,"another type fits only after explicit removal");
 CC(tech_module(CHARM_UFO)==0&&tech_turn_multiplier(&game,1)==1&&equip_mask_for(CHARM_DUCK)==0,"cosmetics introduce no technical module or upgrade bonuses");
 CC(save_game(&game,"charm-roundtrip.sav"),"ownership and fitted charm save through the existing sealed save path");
 Game*saved=malloc(sizeof(Game));if(saved){*saved=game;CC(load_game(&game,"charm-roundtrip.sav")&&game.fit[35]==CHARM_DUCK&&!memcmp(game.dockbay.owned,saved->dockbay.owned,sizeof(game.dockbay.owned))&&!memcmp(game.dockbay.fitted,saved->dockbay.fitted,sizeof(game.dockbay.fitted)),"save/reload preserves all owned copies and the installed instance");free(saved);}else CC(0,"allocate roundtrip comparison");
 CC(load_game(&game,"legacy-v38.sav"),"actual pre-charm V38 legacy save loads with unchanged stream widths");
 hc_test_setup();game.docked=1;game.ship=0;fit_clear_all(&game);memset(&game.dockbay,0,sizeof(game.dockbay));game.credits=100000000;module_buy_to_store(CHARM_ALIEN);page=STORAGE;charm_review_select(CHARM_ALIEN);module_bank=0;module_swap_selected();unsigned ref=game.dockbay.fitted[5];int records=db_stored_count(&game);int exchange=0;for(int ship=1;ship<player_ship_count;ship++)if(shipyard_stocks_ship(&game,ship)&&buy_ship(&game,ship)){exchange=1;break;}
 CC(charm_store_normalise(&game),"legacy spare normalization safely uses existing instance records");
 CC(exchange&&game.fit[5]==CHARM_ALIEN&&game.dockbay.fitted[5]==ref&&db_stored_count(&game)==records,"real ship exchange retains the same single fitted charm without duplication");
 for(int rate=20;rate<=60;rate+=10){if(rate==40||rate==50)continue;CharmMotion m={0};for(int i=0;i<rate*3;i++)charm_step(&m,1.f/rate,i<rate?.7f:0,i<rate?.5f:0,29,2.8f);fprintf(out,"MOTION %d %.8f %.8f\n",rate,m.angle,m.depth);CC(fabsf(m.angle)<.05f&&fabsf(m.depth)<.05f,"20/30/60Hz bounded springs settle after acceleration stops");}
 hc_test_setup();fit_clear_all(&game);game.fit[5]=CHARM_UFO;game.speed=0;charm_tick();float t=game.time;game.time+=1.f/30;game.speed=100;charm_tick();CC(charm_motion.depth<0,"real forward acceleration produces opposite depth lag");game.time+=1.f/30;game.speed=0;charm_tick();CC(charm_motion.depth_velocity>0,"real braking reverses the depth response");
 game.time+=1.f/30;game.yaw+=.08f;charm_tick();CC(charm_motion.angular_velocity<0,"a right turn swings left, opposite actual movement");
 float v=charm_motion.angular_velocity;game.time+=1.f/30;game.energy-=10;charm_tick();CC(fabsf(charm_motion.angular_velocity-v)>.2f,"a real shield-loss state change produces one impact kick");
 CharmMotion held=charm_motion;paused=1;charm_tick();CC(!memcmp(&held,&charm_motion,sizeof(held)),"pause cannot integrate or jump the charm");paused=0;game.pos.x+=100000;game.time+=.03f;charm_tick();CC(charm_motion.angle==0&&charm_motion.angular_velocity==0,"teleport resets derivative history without a violent swing");
 unsigned*previous=fb,*pixels=malloc(STRIDE*H*4);if(pixels){fb=pixels;for(int item=0;item<4;item++){
  hc_test_setup();fit_clear_all(&game);game.fit[5]=CHARM_UFO+item;fit_rebuild(&game);charm_tick();space();char path[64];snprintf(path,sizeof(path),"charm-%d-idle.bmp",item);dump_native_bmp(path);
  int mismatch=0;for(int y=0;y<32;y++)for(int x=0;x<32;x++){unsigned c=charm_pixels[item][y*32+x];int px=71-(item==3?19:16)+x,py=33+(int)charm_length[item]-8+y;if((c>>24)==255&&pixels[py*STRIDE+px]!=c)mismatch++;}
  CC(!mismatch,"native idle drawing preserves every opaque supplied sprite pixel");
  page=EQUIP;game.docked=1;int stock[EQUIP_COUNT],n=equipment_stock_list(stock,EQUIP_COUNT);for(int i=0;i<n;i++)if(stock[i]==CHARM_UFO+item)row=i;rect(0,0,W,H,BG);equipment();snprintf(path,sizeof(path),"charm-%d-shop.bmp",item);dump_native_bmp(path);
 }
 hc_test_setup();game.docked=1;fit_clear_all(&game);module_buy_to_store(CHARM_DUCK);page=STORAGE;charm_review_select(CHARM_DUCK);storage_console_draw();dump_native_bmp("charm-storage.bmp");module_bank=0;module_swap_selected();page=INVENTORY;row=5;loadout_console_draw();dump_native_bmp("charm-fitted.bmp");if(mode==2)charm_review_footage(out);fb=previous;free(pixels);}else CC(0,"allocate native capture framebuffer");
 fprintf(out,"CHECKS %d\nRESULT %d failures\n",checks,fails);fclose(out);return 1;
 #undef CC
}
#endif
