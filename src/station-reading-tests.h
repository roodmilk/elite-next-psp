{
 TEST_INIT();game.docked=1;walk_kind=0;page=WALK;sc_built_for=-1;sc_build_map();
 char read_before[2048];snprintf(read_before,sizeof(read_before),"%s",sc_read_text);
 ScHot initial_hot[24];int initial_n=sc_hotspots(initial_hot,24);
 INPUT_CHECK(initial_n>1&&initial_hot[0].kind==SC_H_HERE&&!strcmp(initial_hot[0].label,"HERE")&&initial_hot[0].w==340&&initial_hot[0].h==168,"station explore: HERE is the first room-wide option");
 INPUT_CHECK(!strcmp(sc_read_title,sc_room_title(SC_R_ARRIVALS))&&strlen(sc_read_text)>300,"station reading: entry shows full room description, not hovered NPC");
 sc_input(PSP_CTRL_DOWN);INPUT_CHECK(sc_hot==1,"station explore: moving down from HERE selects the first room contact");game.message_time=0;
 sc_draw_ui();INPUT_CHECK(1,"station explore: first highlighted-contact frame renders");
 INPUT_CHECK(!strcmp(sc_read_text,read_before),"station reading: hovering and message expiry do not replace committed description");
 sc_hot=sc_find_hot(SC_H_EXIT,SC_R_GUILD);sc_input(PSP_CTRL_CROSS);
 INPUT_CHECK(sc_room==SC_R_GUILD&&!strcmp(sc_read_title,"GUILD DESK")&&strlen(sc_read_text)>300,"station reading: actual doorway enters Guild with room prose");
 sc_hot=sc_find_hot(SC_H_FEATURE,54);sc_input(PSP_CTRL_CROSS);
 INPUT_CHECK(!strcmp(sc_read_title,"STAR CHART")&&strstr(sc_read_text,"Routes and reference")!=NULL,"station reading: Cross explicitly inspects the highlighted object");
 sc_hot=sc_find_hot(SC_H_HERE,sc_room);sc_input(PSP_CTRL_CROSS);
 INPUT_CHECK(!strcmp(sc_read_title,"GUILD DESK")&&strstr(sc_read_text,"Guild office")!=NULL,"station explore: HERE restores the current room arrival description");
 int pages=sc_read_pages();sc_input(PSP_CTRL_RIGHT);
 INPUT_CHECK(pages>1&&sc_read_page==1,"station reading: long prose has accessible next page");
 sc_input(PSP_CTRL_LEFT);INPUT_CHECK(sc_read_page==0,"station reading: previous page restores the start");
 sc_hot=sc_find_hot(SC_H_PERSON,0);sc_input(PSP_CTRL_CROSS);
 INPUT_CHECK(sc_menu==SC_MENU_TALK&&!strcmp(sc_read_title,"IONA"),"station reading: Cross opens the person and reply rail");
 snprintf(read_before,sizeof(read_before),"%s",sc_read_text);sc_input(PSP_CTRL_DOWN);
 INPUT_CHECK(sc_talk_row==1&&!strcmp(sc_read_text,read_before),"station reading: changing reply highlight does not prematurely speak it");
 sc_talk_row=0;sc_input(PSP_CTRL_CROSS);
 INPUT_CHECK(sc_menu==SC_MENU_NONE&&strlen(sc_read_text)>300,"station reading: hear-them-out response is a full persistent conversation");
 sc_input(PSP_CTRL_RIGHT);int oldpage=sc_read_page;sc_input(PSP_CTRL_DOWN);
 INPUT_CHECK(sc_read_page==oldpage,"station reading: hover preserves position within the paragraph");
 int complete=1;for(int r=0;r<7;r++){sc_room=r;sc_read_room();const char *p=sc_read_text;int n=sc_read_pages();for(int i=0;i<n;i++)p=sc_read_advance(p,6,58);if(*p||n<2)complete=0;}
 INPUT_CHECK(complete,"station reading: all seven long room descriptions paginate to their final character");
 sc_room=SC_R_SHOP;sc_menu=0;sc_read_room();snprintf(read_before,sizeof(read_before),"%s",sc_read_text);sc_hot=sc_find_hot(SC_H_FEATURE,101);sc_input(PSP_CTRL_CROSS);input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==WALK&&!strcmp(sc_read_text,read_before),"station reading: returning from outfitting preserves the room paragraph");
 /* The scene-focus halo must not leak into the text panel or right-hand rail. */
 rect(0,0,W,H,0x12345678);ScHot edge={SC_H_PROP,0,SC_VX,SC_VY,SC_VW,SC_VH,"EDGE",""};sc_focus_glow(&edge);
 int bounded=1;for(int y=0;y<H;y++)for(int x=0;x<W;x++)if(x<SC_VX||x>=SC_VX+SC_VW||y<SC_VY||y>=SC_VY+SC_VH)if(fb[y*STRIDE+x]!=0x12345678)bounded=0;
 INPUT_CHECK(bounded,"station focus: glow clipped to 340x168 artwork");
 for(int r=0;r<7;r++){sc_room=r;sc_menu=0;sc_hot=0;sc_read_room();sc_draw_ui();char path[64];snprintf(path,sizeof(path),"station-reading-room-%d.bmp",r);dump_native_bmp(path);}
 sc_room=SC_R_CARGO;sc_menu=0;sc_hot=sc_find_hot(SC_H_PROP,17);sc_input(PSP_CTRL_CROSS);sc_draw_ui();dump_native_bmp("station-reading-manifest.bmp");
 sc_hot=sc_find_hot(SC_H_PERSON,0);sc_input(PSP_CTRL_CROSS);sc_talk_row=1;sc_draw_ui();dump_native_bmp("station-reading-conversation.bmp");
 sc_input(PSP_CTRL_RIGHT);sc_draw_ui();dump_native_bmp("station-reading-conversation-page2.bmp");
 high_contrast=1;sc_draw_ui();dump_native_bmp("station-reading-contrast.bmp");high_contrast=0;
 sc_room=SC_R_SHOP;sc_menu=SC_MENU_SHOP;sc_shop_row=0;int stock[8],stock_n=sc_exclusive_catalog(stock,8);sc_input(PSP_CTRL_UP);
 INPUT_CHECK(stock_n>0&&sc_shop_row==stock_n-1,"station shop: right-rail selection wraps to the actual last stock item");
 sc_draw_ui();dump_native_bmp("station-reading-shop.bmp");sc_input(PSP_CTRL_DOWN);
 INPUT_CHECK(sc_shop_row==0,"station shop: next selection wraps back to the first stock item");
 TEST_INIT();sc_built_for=-1;
}
