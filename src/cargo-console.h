/* Native 480x272 cargo console. Inventory and exchange share real commodity
 * identities, quantities and selection; only their right-hand columns differ. */
enum { CARGO_PAGE_ROWS=7 };
static const unsigned char cargo_order[GOODS]={0,1,11,8,9,12,2,4,5,7,3,6,10,13,14,15,16};
static int cargo_item(int selection){
 int n=0;for(int j=0;j<GOODS;j++){int i=cargo_order[j];
  int visible=game.docked?(station_market_item_visible(&game,i)||game.cargo[i]>0):game.cargo[i]>0;
  if(visible&&n++==selection)return i;
 }return -1;
}
static int cargo_rows(void){int n=0;for(int i=0;i<GOODS;i++)n+=game.docked?(station_market_item_visible(&game,i)||game.cargo[i]>0):game.cargo[i]>0;return n?n:1;}
static int cargo_can_trade(int item){return item>=0&&item<GOODS&&game.docked&&station_market_item_visible(&game,item);}
static const char *cargo_descriptions[GOODS]={
 "Sealed meals and fresh produce. A staple of station life, carried by traders and delivery crews.",
 "Woven cloth and tough fibres for workwear, furnishings and the less glamorous side of space travel.",
 "Shielded fuel materials for reactors and industry. Handle the sealed containers with care.",
 "People held in bondage. This trade is restricted, and carrying them can attract a police scan.",
 "Bottled wines and spirits from across the galaxy, packed to survive a less than gentle landing.",
 "Fine goods for wealthy buyers, from hand-made ornaments to things nobody really needs.",
 "Controlled substances sold through illicit markets. Local law may search your hold for them.",
 "Processors, terminals and control boards. Useful almost everywhere that has power and a socket.",
 "Industrial tools and machine parts for workshops, factories and keeping old stations running.",
 "Refined metal stock for building ships, repairing hulls and a thousand small engineering jobs.",
 "Weapons and ammunition. Their sale is restricted, so expect trouble if the police find them.",
 "Warm pelts and woven synth-furs, prized by tailors across the colder frontier.",
 "Raw stone and useful minerals, sorted and crated for refineries and local manufacturing.",
 "Precious gold, traded by the kilogram. Its compact size makes it easy to carry and easy to lose.",
 "Platinum for industry and fine jewellery. Small, dense bars sold by the kilogram.",
 "Small, valuable stones for collectors and jewellers. These consignments are measured in grams.",
 "Unusual objects of uncertain origin. Buyers are curious; sensible pilots keep the seals intact."
};
/* Each symbol is authored at its actual 9x9 display size, never resampled. */
static const unsigned short cargo_symbols[GOODS][9]={
 {16,54,20,120,16,54,20,16,16},{0,124,130,254,130,254,130,124,0},
 {16,56,146,130,56,130,146,56,16},{0,108,146,146,108,16,108,146,108},
 {24,24,24,60,36,36,36,60,0},{16,56,124,254,124,56,16,0,0},
 {0,60,98,194,132,72,48,0,0},{254,130,186,130,254,16,16,124,0},
 {56,146,254,214,198,214,254,146,56},{254,146,146,254,146,146,254,0,0},
 {0,0,254,240,60,36,36,0,0},{0,60,102,74,90,66,36,24,0},
 {24,60,126,126,254,254,124,56,0},{0,56,108,254,254,0,124,254,0},
 {16,84,56,254,124,254,56,84,16},{16,56,108,198,108,56,16,0,0},
 {56,124,214,130,186,130,214,124,56}
};
static void cargo_icon(int x,int y,int item,unsigned ink){if(item<0||item>=GOODS)return;for(int yy=0;yy<9;yy++)for(int xx=0;xx<9;xx++)if(cargo_symbols[item][yy]&(1u<<(8-xx)))pixel(x+xx,y+yy,ink);}
static void cargo_right(int edge,int y,unsigned ink,const char *value){text_px(edge-(int)strlen(value)*8,y,ink,"%s",value);}
static void cargo_quantity(char *out,int size,int amount,int item){snprintf(out,size,"%d%c",amount,goods[item].unit=='t'?'T':goods[item].unit=='k'?'K':'G');}
/* Bounded word wrap with a testable row count; all descriptions fit in full. */
static int cargo_description(int item,int draw,unsigned ink){
 if(item<0||item>=GOODS)return 0;
 const char *s=cargo_descriptions[item];int n=0;
 while(*s){int len=(int)strlen(s),cut=len<22?len:22;if(len>22){int k=cut;while(k>0&&s[k]!=' ')k--;if(k>0)cut=k;}char linebuf[23];memcpy(linebuf,s,cut);linebuf[cut]=0;if(draw&&n<5)text_px(68,54+n*9,ink,"%s",linebuf);s+=cut;while(*s==' ')s++;n++;}
 return n;
}
static void cargo_crate(int x,int y,int w,int h){
 const unsigned edge=RGB(126,111,74),dark=RGB(41,39,29),side=RGB(68,62,43);
 rect(x,y,w,h,edge);rect(x+3,y+3,w-6,h-5,dark);rect(x+w-5,y+2,3,h-3,side);rect(x+w/2-1,y,3,h,edge);
 line(x,y,x+4,y-4,edge);line(x+4,y-4,x+w+4,y-4,edge);line(x+w,y,x+w+4,y-4,side);rect(x+4,y+h-2,4,4,dark);rect(x+w-7,y+h-2,4,4,dark);
}
static void cargo_bay(int item){
 unsigned wall=RGB(29,29,22),frame=RGB(86,83,61),light=RGB(174,163,110);
 rect(268,50,196,50,frame);rect(270,52,192,46,wall);
 line(270,52,288,61,frame);line(462,52,444,61,frame);line(288,61,444,61,RGB(65,62,44));
 line(288,61,288,87,frame);line(444,61,444,87,frame);line(270,98,288,87,frame);line(462,98,444,87,frame);
 for(int i=0;i<6;i++)line(366,62,270+i*38,98,RGB(44,44,32));
 line(278,94,454,94,RGB(49,47,34));line(284,89,448,89,RGB(49,47,34));
 rect(282,55,16,2,light);rect(436,55,15,2,light);
 if(cargo_used(&game)>0){cargo_crate(296,74,29,19);cargo_crate(330,70,35,23);cargo_crate(403,72,35,21);}
 if(item>=0)draw_next_art(next_items[item],32,32,367,65,32,32);
}
static void market_screen(void){
 const unsigned bg=ui_paint_surface(RGB(15,15,11)),panelbg=ui_paint_surface(RGB(34,32,23)),rule=ship_theme_index()?UI_EDGE:RGB(103,91,64),cream=ship_theme_index()?UI_TEXT:RGB(227,216,181),muted=ship_theme_index()?UI_MUTED:RGB(166,155,124),gold=ship_theme_index()?UI_ACCENT:RGB(237,187,99),selected=ship_theme_index()?UI_ACCENT:RGB(205,162,95),dark=ui_paint_surface(RGB(25,23,16));
 preview_reset();rect(0,0,W,H,bg);rect(0,0,W,22,ui_paint_surface(RGB(29,27,21)));line(0,21,479,21,gold);
 draw_next_art(next_logo_small,100,18,5,2,100,18);text_px(112,8,cream,"/ CARGO / %s",game.docked?"EXCHANGE OVERVIEW":"HOLD OVERVIEW");
 int count=cargo_rows();if(row<0)row=0;if(row>=count)row=count-1;
 int first=(row/CARGO_PAGE_ROWS)*CARGO_PAGE_ROWS,item=cargo_item(row),used=cargo_used(&game),cap=cargo_capacity(&game);char value[40];
 rect(8,29,464,78,panelbg);line(8,29,471,29,gold);line(8,29,8,106,rule);line(471,29,471,106,rule);line(8,106,471,106,rule);line(253,36,253,100,rule);
 rect(16,40,40,44,bg);line(16,40,55,40,rule);line(16,83,55,83,rule);line(16,40,16,83,rule);line(55,40,55,83,rule);
 if(item>=0){draw_next_art(next_items[item],32,32,20,46,32,32);text_px(68,37,gold,"%s",goods[item].name);cargo_description(item,1,cream);}
 else {text_px(68,37,gold,"HOLD IS EMPTY");text_px(68,54,cream,"Your cargo will appear");text_px(68,63,cream,"here when you collect");text_px(68,72,cream,"or buy goods.");}
 if(!game.docked){text_px(268,37,gold,"CARGO HOLD");cargo_bay(item);}
 else if(item>=0){
  text_px(268,37,muted,"GALACTIC AVERAGE:");text_px(268,50,cream,"%.1f U",galactic_price(item)*.1f);
  text_px(268,64,muted,"SHOPS STOCK:");int available=cargo_can_trade(item);
  if(available)snprintf(value,sizeof(value),"%d",game.stock[item]);else snprintf(value,sizeof(value),"--");cargo_right(464,64,cream,value);line(268,76,463,76,rule);
  if(!available){text_px(268,82,gold,"NOT TRADED HERE");text_px(268,94,cream,"SELL AT ANOTHER HUB.");}
  else {int diff=game.price[item]-galactic_price(item);text_px(268,82,diff>0?RGB(231,153,113):diff<0?RGB(136,192,166):muted,diff>0?"ABOVE GALACTIC AVERAGE":diff<0?"BELOW GALACTIC AVERAGE":"AT GALACTIC AVERAGE");
   if(goods_restricted(item))text_px(268,94,RGB(245,104,91),"RESTRICTED GOODS!");
   else if(mission_cargo_reserved(&game,item)){cargo_quantity(value,sizeof(value),mission_cargo_reserved(&game,item),item);text_px(268,94,gold,"%s RESERVED FOR JOBS",value);}
  }
 }
 rect(8,112,464,18,panelbg);text_px(16,118,gold,"COMMODITIES");
 if(count>CARGO_PAGE_ROWS)text_px(120,118,muted,"%d/%d",row/CARGO_PAGE_ROWS+1,(count+CARGO_PAGE_ROWS-1)/CARGO_PAGE_ROWS);
 text_px(192,118,muted,game.docked?"HOLD":"HELD");
 if(game.docked){text_px(256,118,muted,"STOCK");text_px(368,118,muted,"PRICE HERE:");}
 else {text_px(272,118,muted,"SHARE OF HOLD");text_px(400,118,muted,"RESERVED");}
 line(8,131,471,131,gold);
 for(int j=0;j<CARGO_PAGE_ROWS&&first+j<count;j++){
  int i=cargo_item(first+j),y=139+j*13,sel=first+j==row;unsigned ink=sel?dark:cream,dim=sel?RGB(57,47,30):muted;
  rect(8,y-2,464,13,sel?selected:j%2?panelbg:bg);
  if(i<0){text_px(41,y,ink,"NO CARGO ABOARD");continue;}
  cargo_icon(19,y-1,i,sel?dark:gold);text_px(41,y,ink,"%s",goods[i].name);cargo_quantity(value,sizeof(value),game.cargo[i],i);cargo_right(224,y,ink,value);
  if(game.docked){int available=cargo_can_trade(i);if(available)snprintf(value,sizeof(value),"%d",game.stock[i]);else snprintf(value,sizeof(value),"--");cargo_right(296,y,dim,value);
   if(available)snprintf(value,sizeof(value),"%.1f U",game.price[i]*.1f);else snprintf(value,sizeof(value),"NOT TRADED");cargo_right(456,y,ink,value);
  }else {
   int width=used>0&&goods[i].unit=='t'?(int)(136.f*game.cargo[i]/used):0;if(width>136)width=136;
   rect(272,y+2,136,5,sel?RGB(108,86,50):RGB(85,80,60));if(width>0)rect(272,y+2,width,5,sel?RGB(246,210,133):gold);
   int reserved=mission_cargo_reserved(&game,i);if(reserved>0)cargo_quantity(value,sizeof(value),reserved,i);else snprintf(value,sizeof(value),"--");cargo_right(456,y,dim,value);
  }
 }
 if(count>CARGO_PAGE_ROWS){rect(8,137,3,90,rule);int thumb=90*CARGO_PAGE_ROWS/count;if(thumb<8)thumb=8;int offset=(90-thumb)*row/(count-1);rect(8,137+offset,3,thumb,gold);}
 int fill=cap>0?(int)(142.f*used/cap):0;if(fill>142)fill=142;if(fill<0)fill=0;rect(14,233,142,5,rule);if(fill)rect(14,233,fill,5,gold);text_px(164,231,cream,"%d/%dT",used,cap);
 snprintf(value,sizeof(value),"%.1f U",game.credits*.1f);int moneyw=(int)strlen(value)*8+12;if(moneyw<90)moneyw=90;rect(472-moneyw,228,moneyw,16,panelbg);line(472-moneyw,228,471,228,gold);cargo_right(466,232,cream,value);
 rect(0,248,480,24,ui_paint_surface(RGB(29,27,21)));line(0,248,479,248,gold);button_icon(9,254,'U',cream);button_icon(20,254,'D',cream);text_px(36,256,cream,"SELECT");
 if(game.docked){int available=cargo_can_trade(item);button_icon(121,254,'L',cream);text_px(137,256,available?cream:rule,"SELL");button_icon(225,254,'R',cream);text_px(241,256,available?cream:rule,"BUY");}
 if(!game.docked){button_icon(121,254,'S',cream);text_px(137,256,cream,"JETTISON");}
 button_icon(349,254,'O',cream);text_px(365,256,cream,"BACK");
}
