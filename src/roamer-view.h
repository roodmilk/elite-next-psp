/* Transient, translucent windshield cracks. No damage to persistent assets. */
static void rover_crack_line(int x,int y,int x1,int y1,float alpha){
 int dx=abs(x1-x),dy=-abs(y1-y),sx=x<x1?1:-1,sy=y<y1?1:-1,err=dx+dy;
 for(;;){
  if(x>=0&&x<W&&y>=28&&y<240)fb[y*STRIDE+x]=mix_rgb(fb[y*STRIDE+x],RGB(197,226,235),alpha);
  if(x==x1&&y==y1)break;
  int e=err*2;if(e>=dy){err+=dy;x+=sx;}if(e<=dx){err+=dx;y+=sy;}
 }
}
static void rover_windshield(void){
 if(!game.rover_driving||game.rover_crack<=0)return;
 float a=fminf(.82f,game.rover_crack*.9f);
 static const short cracks[][4]={{52,211,64,180},{64,180,87,158},{87,158,82,140},{87,158,111,148},{64,180,40,166},{40,166,34,139},{52,211,27,195},{27,195,10,201},{52,211,79,219},{79,219,96,206},{439,70,415,89},{415,89,408,116},{408,116,390,128},{415,89,391,81},{391,81,373,91},{439,70,446,101},{446,101,462,116},{439,70,461,50},{461,50,478,43}};
 for(unsigned i=0;i<sizeof(cracks)/sizeof(cracks[0]);i++)rover_crack_line(cracks[i][0],cracks[i][1],cracks[i][2],cracks[i][3],a);
}
