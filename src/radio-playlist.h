/* A bounded randomized candidate order; no filesystem/decoder ownership. */
#ifndef ELITE_RADIO_PLAYLIST_H
#define ELITE_RADIO_PLAYLIST_H
static int radio_candidate_order(int count,int previous,unsigned *seed,int *order){
 if(count<0||count>RADIO_TRACKS_PER_STATION)return 0;
 int used=0;
 for(int i=0;i<count;i++)if(i!=previous)order[used++]=i;
 for(int i=used-1;i>0;i--){
  *seed=*seed*1664525u+1013904223u;
  int j=(int)((*seed>>8)%(unsigned)(i+1));
  int swap=order[i];order[i]=order[j];order[j]=swap;
 }
 /* Repeat only when every alternative failed (or there is a single track). */
 if(previous>=0&&previous<count)order[used++]=previous;
 return used;
}
#endif
