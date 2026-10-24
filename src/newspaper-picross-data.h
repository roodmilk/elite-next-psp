/* Authored 10x10 silhouettes. Clues are derived from these exact pixels. */
#define NEWS_PICROSS_SIZE 10
static const char newspaper_picross_pictures[6][101]={
 "0000110000" "0001111000" "0011111100" "0111111110" "1111111111" "1111111111" "0001111000" "0001111000" "0011001100" "0110000110",
 "0001111000" "0011111100" "0111111110" "1111111111" "1111111111" "1111111111" "0111111110" "0011111100" "0001111000" "0000000000",
 "0001111000" "0011111100" "0111111110" "0110110110" "1111111111" "1111111111" "0111111110" "0011111100" "0001111000" "0001111000",
 "0000110000" "0001111000" "0011111100" "0011111100" "0111111110" "1111111111" "1111111111" "0111111110" "0011111100" "0000110000",
 "0001111000" "0011111100" "0111111110" "1111111111" "0001111000" "0001111000" "0001111000" "0001111000" "0011111100" "0111111110",
 "0000110000" "0000110000" "0001111000" "0011111100" "0111111110" "1111111111" "1111111111" "0011111100" "0001111000" "0000110000"
};
static const char *const newspaper_picross_titles[]={"INTERCEPTOR","NEW WORLD","SURVEY ROBOT","FUEL CRYSTAL","RELAY BEACON","STARFIGHTER"};
static int newspaper_picross_solution(int system,int cell){
 return cell>=0&&cell<100&&newspaper_picross_pictures[((unsigned)system/3)%6][cell]=='1';
}
static int newspaper_picross_clues(int system,int column,int index,int out[5]){
 int count=0,run=0;
 for(int i=0;i<=10;i++){
  int filled=i<10&&newspaper_picross_solution(system,column?i*10+index:index*10+i);
  if(filled)run++;else if(run){out[count++]=run;run=0;}
 }
 if(!count)out[count++]=0;
 return count;
}
