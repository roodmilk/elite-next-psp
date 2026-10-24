static int online_return_page=HOME;
static int online_list(int *ids){int n=0;for(int local=1;local>=0;local--)for(int i=0;i<ONLINE_LIMIT;i++)if(online_peer_ready(i)&&((online.peer[i].pilot.system==game.system)==local))ids[n++]=i;return n;}
static void online_space_draw(void){
 if(!online.stage){wifi_ship();return;}int selected=online.selected;WifiPilot remote=wifi.remote,display=wifi.display;
 for(int i=0;i<ONLINE_LIMIT;i++)if(online_peer_ready(i)){online_apply(i);wifi_ship();}
 online.selected=selected;wifi.remote=remote;wifi.display=display;
}
static void wifi_surface_draw(void){
 if(!online.stage){wifi_surface_one();return;}int selected=online.selected;WifiPilot remote=wifi.remote,display=wifi.display;
 for(int i=0;i<ONLINE_LIMIT;i++)if(online_peer_ready(i)){online_apply(i);wifi_surface_one();}
 online.selected=selected;wifi.remote=remote;wifi.display=display;
}
static void online_page_input(unsigned pressed){multiplayer_input(pressed,.016f);}
static void online_screen(void){multiplayer_draw();}
