/* Original, allocation-free event sounds at 44.1 kHz. No recorded assets. */
#ifndef ELITE_AUDIO_SFX_H
#define ELITE_AUDIO_SFX_H
static int audio_sfx_length(int id){
 switch(id){
 case SFX_OBSERVATORY:return 10584; /* 240ms quiet receiver handshake */
 case SFX_SELECT:return 882;       /* 20 ms: quiet navigation tick */
 case SFX_UI:return 1764;
 case SFX_LASER:return 3528;
 case SFX_HIT:return 4410;
 case SFX_SCAN:return 7938;        /* 180 ms: rising confirmation */
 case SFX_COMM:return 5292;
 case SFX_TALK:return 6174;
 case SFX_LAND:return 8820;
 case SFX_DOCK:return 10584;
 case SFX_MINE:return 4410;
 case SFX_BOOST:return 7056;
 case SFX_MISSILE:return 7938;
 case SFX_ALERT:return 13230;      /* two softly edged warning pulses */
 case SFX_WARP:return 13230;
 case SFX_DEATH:return 13230;
 case SFX_ALIEN_PLAYER:return 2205;
 case SFX_ALIEN_FIRE:return 3087;
 case SFX_ALIEN_MISSILE:return 5292;
 case SFX_ALIEN_KILL:return 6174;
 case SFX_SPEED_SURGE:return 8820;
 default:return 0;
 }
}
static int audio_sfx_sample(int id,int age,unsigned *phase,unsigned *noise){
 int length=audio_sfx_length(id);if(age<0||age>=length)return 0;
 int remaining=length-1-age,freq=440,level=12,grit=0;
 *noise=*noise*1664525u+1013904223u;
 int n=(int)(*noise>>24)-128;
 switch(id){
 case SFX_OBSERVATORY:freq=age<length/3?420:age<length*2/3?630:470;level=5;break;
 case SFX_SELECT:freq=1100;level=8;break;
 case SFX_UI:freq=660;level=10;break;
 case SFX_LASER:freq=1400-age*1000/length;level=16;break;
 case SFX_HIT:freq=95;level=8;grit=n*20;break;
 case SFX_SCAN:freq=520+age*520/length;level=12;break;
 case SFX_COMM:freq=age<length/2?740:550;level=10;break;
 case SFX_TALK:freq=160+(age/882%4)*35;level=7;grit=n*4;break;
 case SFX_LAND:freq=105-age*45/length;level=14;grit=n*5;break;
 case SFX_DOCK:freq=age<length/2?220:330;level=14;break;
 case SFX_MINE:freq=180;level=8;grit=n*14;break;
 case SFX_BOOST:freq=90+age*100/length;level=12;grit=n*6;break;
 case SFX_MISSILE:freq=260+age*480/length;level=13;grit=n*3;break;
 case SFX_ALERT:freq=880;level=18;break;
 case SFX_WARP:freq=110+age*660/length;level=15;break;
 case SFX_DEATH:freq=100-age*65/length;level=10;grit=n*22;break;
 case SFX_ALIEN_PLAYER:freq=1750-age*750/length;level=12;break;
 case SFX_ALIEN_FIRE:freq=420+(age/220%3)*130;level=9;grit=n*3;break;
 case SFX_ALIEN_MISSILE:freq=240+age*620/length;level=11;grit=n*2;break;
 case SFX_ALIEN_KILL:freq=780-age*610/length;level=12;grit=n*9;break;
 case SFX_SPEED_SURGE:freq=95+age*260/length;level=10;grit=n*(4+age*5/length);break;
 }
 *phase+=(unsigned)freq*97391u; /* 2^32 / 44100, rounded */
 int x=(int)(*phase>>22),wave=x<512?x-256:768-x;
 int envelope=age<128?age:128;
 if(remaining<envelope)envelope=remaining;
 if(id==SFX_ALERT){
  int pulse=age%6615;
  if(pulse>=4410)envelope=0;
  else {if(pulse<envelope)envelope=pulse;if(4409-pulse<envelope)envelope=4409-pulse;}
 }
 return ((wave*level+grit)*remaining/length)*envelope/128;
}
#endif
