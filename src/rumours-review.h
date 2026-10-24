static int rumours_review(void){
 FILE *flag=fopen("rumours-review.flag","r");if(!flag)return 0;fclose(flag);
 FILE *report=fopen("rumours-review.txt","w");if(!report)return 1;
 int failures=0;unsigned *oldfb=fb,*pixels=malloc(STRIDE*H*sizeof(unsigned));
 if(!pixels){fprintf(report,"RESULT 1 failures\n");fclose(report);return 1;}fb=pixels;
#define RCHECK(ok,label) do{int pass=!!(ok);fprintf(report,"%s %s\n",pass?"PASS":"FAIL",label);failures+=!pass;fflush(report);}while(0)
 game_init(&game);story_complete(&game);game.system=7;game_spawn(&game);game.docked=1;game.credits=100000;identity_slot=-99;identity_sync();
 page=GALNET;galnet_tab=5;row=0;rumour_page_system=-1;rumour_layout();
 RCHECK(rumour_links_n>=4,"Lave offers actual canonical local discoveries");
 int k=rumour_links[0].slot,cash=game.credits,price=rumour_price(k)*10;
 RCHECK(!rumour_valid(-1)&&!rumour_valid(13)&&!rumour_valid(12),"invalid IDs and absent Lave whales cannot be sold");
 RCHECK(rumour_purchase(k)==1&&game.credits==cash-price&&rumour_owned(k)&&!game.rumours.active,"purchase deducts exact fixed-point cost, records ownership and does not track automatically");
 cash=game.credits;RCHECK(rumour_purchase(k)==2&&game.credits==cash,"duplicate lead entrances never charge twice");
 int other=rumour_links[1].slot;game.credits=0;
 RCHECK(rumour_purchase(other)==-2&&!rumour_owned(other)&&!game.credits,"insufficient units leaves all ownership and cash unchanged");game.credits=cash;
 tracked_mission=0;selected_target=0;int mission=tracked_mission,selected=selected_target;
 RCHECK(rumour_track(k)&&tracked_mission==mission&&selected_target==selected,"tracking a rumour preserves mission tracking and selected flight contact");
 game.pos=add(rumour_object_position(k),(Vec3){0,0,-100000});int ids[TARGET_CAPACITY],n=collect_scan_ids(ids,3),hasarea=0,hasobject=0;
 for(int i=0;i<n;i++){hasarea|=ids[i]==RUMOUR_TARGET_ID;hasobject|=ids[i]==rumour_target(k);}
 RCHECK(hasarea&&!hasobject&&!scanner_known(rumour_target(k)),"out-of-range data supplies only a search waypoint, not an exact contact or identity");
 RCHECK(!strcmp(target_status(RUMOUR_TARGET_ID),"SCAN LOCALLY")&&!strcmp(target_status(WHALE_TARGET_ID),"TRI TO OBSERVE")&&!strcmp(target_status(DISCOVERY_ID_MIN),"DISCOVERY"),"full targeting computer handles search regions, whales and discoveries without indexing NPC data");
 int previous_selected=selected_target;scan_cat=3;row=0;rect(0,0,W,H,BG);targeting_screen();dump_native_bmp("rumours-target-computer.bmp");selected_target=previous_selected;
 Vec3 centre=rumour_centre(k);
 RCHECK(length(sub(centre,rumour_object_position(k)))>600&&length(sub(centre,rumour_object_position(k)))<rumour_radius(k)&&rumour_safe(centre,180),"search centre is offset from the real object and stays navigable");
 game.docked=0;page=FLIGHT;game.pos=centre;game.message_time=0;rumour_arrived_key=-1;rumour_tick();
 RCHECK(strstr(game.message,"Watch your scanner")!=0,"area arrival gives the scanner hint once");
 game.message_time=0;rumour_tick();RCHECK(game.message_time==0,"remaining in a search area does not repeat arrival chatter");
 selected_target=rumour_target(k);game.pos=rumour_object_position(k);hail_target();
 RCHECK(rumour_found(k)&&scanner_known(selected_target),"real close interaction identifies the discovery and resolves the lead");
 game.docked=1;game.rumours.found[7]|=1u<<k;game.credits=cash;game.voice_time=0;
 static Game restored;
 RCHECK(save_game(&game,"rumours-test.sav")&&load_game(&restored,"rumours-test.sav")&&restored.credits==cash&&restored.rumours.active==game.rumours.active&&restored.rumours.owned[7]==game.rumours.owned[7]&&restored.rumours.paid[7][k]==price,"V32 CRC-protected save/load preserves payment, ownership, directions and discovery");
 FILE *sf=fopen("rumours-test.sav","rb");int migrated=0,corrupt=0;
 if(sf){fseek(sf,0,SEEK_END);long size=ftell(sf);rewind(sf);unsigned char *bytes=malloc(size);
  if(bytes&&fread(bytes,1,size,sf)==(size_t)size){
   bytes[size-9]^=1;FILE *bad=fopen("rumours-bad.sav","wb");if(bad){fwrite(bytes,1,size,bad);fclose(bad);Game before=restored;corrupt=!load_game(&restored,"rumours-bad.sav")&&restored.credits==before.credits&&restored.rumours.active==before.rumours.active;}bytes[size-9]^=1;
   long payload=size-4-ERL_PROGRESS_BYTES-RA_SAVE_BYTES-EAX_SAVE_BYTES-DB_SAVE_BYTES-ERL_SAVE_BYTES-EXPEDITION_SAVE_BYTES-NEWSPAPER_REWARD_SAVE_BYTES-(8+256*(3+RUMOUR_SLOTS)*4);memmove(bytes+payload-2,bytes+payload,size-payload);payload-=2;bytes[4]=30;bytes[5]=bytes[6]=bytes[7]=0;uint32_t crc=0xffffffffu;
   for(long i=0;i<payload;i++){crc^=bytes[i];for(int b=0;b<8;b++)crc=(crc>>1)^((crc&1)?0xedb88320u:0);}crc^=0xffffffffu;
   for(int b=0;b<4;b++)bytes[payload+b]=(unsigned char)(crc>>(b*8));FILE *old=fopen("rumours-v30.sav","wb");if(old){fwrite(bytes,1,payload+4,old);fclose(old);migrated=load_game(&restored,"rumours-v30.sav")&&!restored.rumours.active&&!restored.rumours.owned[7]&&restored.credits==cash;}
  }free(bytes);fclose(sf);
 }
 RCHECK(migrated,"V30 commanders migrate without changing credits or inventing purchases");
 RCHECK(corrupt,"corrupt rumour bytes reject load without mutating the commander");
 game.rumours.found[7]&=~(1u<<other);discovery_identified[7]|=1u<<other;
 cash=game.credits;int known=rumour_purchase(other)==2&&game.credits==cash&&!rumour_owned(other)&&rumour_track(other);
 RCHECK(known&&save_game(&game,"rumours-known.sav")&&load_game(&restored,"rumours-known.sav")&&(restored.rumours.found[7]&(1u<<other)),"naturally identified destinations offer free directions that can be saved immediately");
 remove("rumours-known.sav");
 remove("rumours-test.sav");remove("rumours-bad.sav");remove("rumours-v30.sav");
 game_init(&game);story_complete(&game);game.system=7;game_spawn(&game);game.docked=1;game.credits=100000;
 identity_slot=-99;identity_sync();memset(discovery_identified,0,sizeof(discovery_identified));memset(anomaly_identified,0,sizeof(anomaly_identified));
 page=GALNET;galnet_tab=5;rumour_page_system=-1;rumour_layout();preview_time=20;
 rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("rumours-home.bmp");
 float clock=game.time;Vec3 parked=game.pos;input(0,0,.05f,1,1);
 RCHECK(game.time==clock&&length(sub(parked,game.pos))<.001f,"browsing pauses world simulation and ignores flight axes");
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 RCHECK(spacepay_stage==1&&spacepay_choice==1,"X opens SpacePay with Cancel selected");
 cash=game.credits;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);RCHECK(spacepay_stage==1&&game.credits==cash,"held opening X cannot confirm a payment");
 input(0,0,.016f,0,0);input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);
 rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("rumours-spacepay-review.bmp");
 int chosen=spacepay_slot;input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);cash=game.credits;
 RCHECK(spacepay_stage==2&&rumour_owned(chosen)&&!game.rumours.active,"confirmed payment starts feedback only after one transaction");
 preview_time+=.35f;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("rumours-spacepay-transfer.bmp");
 input(0,0,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 RCHECK(spacepay_stage==3&&game.credits==cash&&!game.rumours.active,"skipping animation consumes its press and never starts tracking or charges again");
 preview_time+=.5f;rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("rumours-spacepay-receipt.bmp");
 input(0,0,.016f,0,0);input(PSP_CTRL_UP,PSP_CTRL_UP,.016f,0,0);input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 RCHECK(!spacepay_stage&&rumour_area_ready()&&game.credits==cash,"fresh explicit Track Search Area activates directions without another payment");
 input(0,0,.016f,0,0);input(PSP_CTRL_SQUARE,PSP_CTRL_SQUARE,.016f,0,0);RCHECK(!game.rumours.active,"Square stops directions without losing the purchase");
 int base=rumour_focus;input(PSP_CTRL_RIGHT,PSP_CTRL_RIGHT,.016f,0,0);int adjacent=rumour_focus;input(PSP_CTRL_LEFT,PSP_CTRL_LEFT,.016f,0,0);
 RCHECK(adjacent==1&&rumour_focus==base,"side-by-side website adverts have reversible horizontal navigation");
 int focus_ok=1;for(int i=0;i<rumour_links_n+1;i++){input(PSP_CTRL_DOWN,PSP_CTRL_DOWN,.016f,0,0);focus_ok&=rumour_focus>=0&&rumour_focus<rumour_links_n;}
 RCHECK(focus_ok&&rumour_focus==rumour_links_n-1,"vertical navigation reaches every offer and stops safely at the end");
 for(int i=2;i<rumour_links_n&&i<5;i++){rumour_focus=i;rumour_focus_scroll();rumour_scroll=rumour_scroll_goal;rect(0,0,W,H,BG);galnet_screen();char name[48];snprintf(name,sizeof(name),"rumours-section-%d.bmp",i);dump_native_bmp(name);}
 int offers=0,quiet=0,whales=0,valid=1,documents_ok=1;unsigned distinct=0;
 for(int s=0;s<256;s++){
  game.system=s;game_spawn(&game);rumour_page_system=-1;rumour_layout();int total=0;
  for(int i=0;i<RUMOUR_SLOTS;i++)if(rumour_valid(i)){total++;valid&=rumour_safe(rumour_centre(i),180);if(i==12)whales++;}
  for(int i=0;i<rumour_links_n;i++){RumourLink *l=&rumour_links[i];rumour_focus=i;rumour_focus_scroll();documents_ok&=rumour_doc_height>=l->y+l->h&&rumour_scroll_goal<=l->y&&rumour_scroll_goal+188>=l->y+l->h;}
  offers+=total;quiet+=!total;distinct|=1u<<(total<31?total:31);
 }
 RCHECK(valid&&offers>0&&(distinct&(distinct-1)),"256 systems derive varied offers from real valid local objects and safe search areas");
 RCHECK(documents_ok,"every local offer is fully reachable, including whale-only system reviews");
 RCHECK(whales>0&&whales<128,"whale experiences appear only in actual whale systems");
 fprintf(report,"INFO %d eligible offers across 256 systems; %d quiet systems; %d whale systems\n",offers,quiet,whales);
 game.system=2;game_spawn(&game);game.docked=0;rumour_page_system=-1;rumour_layout();int whale_valid=rumour_valid(12);game.credits=100000;rumour_purchase(12);rumour_track(12);Vec3 home=rumour_centre(12);int inside=1;
 for(int i=0;i<100;i++){game.time=i*31.f;for(int j=0;j<3;j++)inside&=length(sub(whale_position(&game,j),home))<=rumour_radius(12);}
 RCHECK(whale_valid&&inside,"the real moving whale pod stays inside its promised search region");
 game.pos=whale_position(&game,0);selected_target=WHALE_TARGET_ID;int discoveries=game.discoveries;hail_target();hail_target();
 RCHECK(rumour_found(12)&&game.discoveries==discoveries+1,"whale observation is a real close interaction and rewards discovery once");
 page=GALNET;galnet_tab=5;game.docked=1;rumour_page_system=-1;rumour_layout();
 for(int i=0;i<rumour_links_n;i++)if(rumour_links[i].slot==12&&rumour_links[i].style==3){rumour_focus=i;rumour_focus_scroll();rumour_scroll=rumour_scroll_goal;break;}
 rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("rumours-whales.bmp");
 game.system=7;game_spawn(&game);rumour_tick();RCHECK(!rumour_area_ready()&&!game.rumours.active,"leaving the local system suspends its rumour waypoint");
 game.system=2;game_spawn(&game);RCHECK(rumour_owned(12)&&!rumour_area_ready()&&rumour_track(12),"returning retains paid local directions for explicit tracking");
 /* Exceptional target removal refunds once, no repeated windfall. */
 int a=-1;for(int s=0;s<256&&a<0;s++){game.system=s;game_spawn(&game);for(int j=8;j<12;j++)if(rumour_valid(j)){a=j;break;}}
 if(a>=0){int sys=game.system;game.docked=0;game.rumours.found[sys]&=~(1u<<a);game.rumours.owned[sys]&=~(1u<<a);game.rumours.paid[sys][a]=0;game.credits=100000;rumour_purchase(a);rumour_track(a);cash=game.credits;int paid=game.rumours.paid[sys][a];game.anomaly[a-8].alive=0;rumour_tick();int once=game.credits;rumour_tick();RCHECK(once==cash+paid&&game.credits==once&&!game.rumours.active,"permanently unavailable paid data refunds once and clears its active waypoint");
  game.docked=1; /* Ordinary commanders save at stations, including refunds. */
  RCHECK(save_game(&game,"rumours-refund.sav")&&load_game(&restored,"rumours-refund.sav")&&(restored.rumours.refunded[sys]&(1u<<a))&&!restored.rumours.active,"refunded active directions remain valid through save/load");remove("rumours-refund.sav");}
 else RCHECK(0,"refund fixture has an eligible anomaly");
 game.opening.stage=OP_EARTH_NET;rumour_page_system=-1;rumour_layout();page=GALNET;galnet_tab=5;
 RCHECK(!rumour_links_n,"Earth tutorial never sells fictitious Lave encounters");
 rect(0,0,W,H,BG);galnet_screen();dump_native_bmp("rumours-empty-earth.bmp");
 u64 start,end;sceRtcGetCurrentTick(&start);for(int i=0;i<24;i++){preview_time+=.016f;galnet_screen();}sceRtcGetCurrentTick(&end);
 fprintf(report,"INFO empty website mean draw %.3f ms\n",(end-start)*1000.0/sceRtcGetTickResolution()/24.0);
 game.opening.stage=OP_FREE;game.system=7;game_spawn(&game);rumour_page_system=-1;rumour_layout();rumour_focus=2;rumour_focus_scroll();rumour_scroll=rumour_scroll_goal;
 sceRtcGetCurrentTick(&start);for(int i=0;i<24;i++){preview_time+=.016f;galnet_screen();}sceRtcGetCurrentTick(&end);
 double mean=(end-start)*1000.0/sceRtcGetTickResolution()/24.0;
 RCHECK(mean<33.34,"populated native website render stays within the 30fps emulator draw budget");fprintf(report,"INFO illustrated website mean draw %.3f ms\n",mean);
 /* A restrained beacon flashes once per 20 ticks: observe a full cycle,
    rather than accidentally comparing two frames from its steady phase. */
 unsigned h1=0,h2=0;for(int yy=60;yy<248;yy++)for(int xx=0;xx<464;xx++)h1=h1*33+pixels[yy*STRIDE+xx];
 for(int sample=0;sample<24;sample++){preview_time+=.16f;galnet_screen();h2=0;for(int yy=60;yy<248;yy++)for(int xx=0;xx<464;xx++)h2=h2*33+pixels[yy*STRIDE+xx];if(h1!=h2)break;}
 RCHECK(h1!=h2,"visible website art has real functional animation frames");
 fprintf(report,"RESULT %d failures\n",failures);fclose(report);fb=oldfb;free(pixels);
#undef RCHECK
 return 1;
}
