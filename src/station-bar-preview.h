/* Bounded playable preview: no job, credit, cargo or persistent state writes. */
#ifndef ELITE_STATION_BAR_PREVIEW_H
#define ELITE_STATION_BAR_PREVIEW_H
#include "bar-gameplay.h"
enum { SC_ACT_BAR_LYSA=20, SC_ACT_BAR_PELL, SC_ACT_BAR_DAX };
static BarPracticeState bar_preview_practice={0,0x53484946u,0,0,0};
static uint32_t bar_preview_token;
static char bar_preview_notes[3][96]={
 "Pell knows REORTE I. Ask about the world.",
 "Recorder work is not enabled in this preview.",
 "Free practice: LOW 2-6; HIGH 8-12; 7 draw."
};
static int bar_preview_at(void){
 return game.system==39&&game.station_variant==0&&game.docked&&!game.dead;
}
static int bar_preview_action(int act){return act>=SC_ACT_BAR_LYSA&&act<=SC_ACT_BAR_DAX;}
static void bar_preview_choose(int act,int choice){
 if(!bar_preview_at()||!bar_preview_action(act))return;
 int who=act-SC_ACT_BAR_LYSA;
 if(act==SC_ACT_BAR_DAX){
  BarResult applied=bar_practice_confirm(&bar_preview_practice,bar_preview_token,(uint32_t)choice);
  if(applied.status!=BAR_APPLIED)return;
  unsigned a=bar_preview_practice.die1,b=bar_preview_practice.die2,sum=a+b;
  int outcome=bar_practice_outcome(&bar_preview_practice);
  const char *result=outcome==0?"DRAW":outcome>0?"WIN":"LOSS";
  snprintf(bar_preview_notes[who],96,"%u + %u = %u: %s. Free; no credits changed.",a,b,sum,result);
 }else if(choice==0){message(&game,bar_preview_notes[who]);return;}
 else if(!story_landing_ready(&game)){
  snprintf(bar_preview_notes[who],96,"Get landing kit: finish Kei's power lesson.");
 }else{
  selected_target=2;nav_body=1;scan_cat=0;autoaim=0;
  snprintf(bar_preview_notes[who],96,"REORTE I targeted. Return to ship and launch.");
 }
 message(&game,bar_preview_notes[who]);game.cue=SFX_UI;
}
#endif
