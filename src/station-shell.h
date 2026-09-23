/* Shared Second Shift presentation shell for every station room. */
#ifndef ELITE_STATION_SHELL_H
#define ELITE_STATION_SHELL_H

typedef struct {
 unsigned ink, panel, rule, warm, cream, cyan;
} StationShellStyle;

static StationShellStyle station_shell_style(void){
 StationShellStyle s={SC_CHAR,SC_VOID,SC_OCHRE,SC_AMBER,SC_CREAM,SC_CYAN};
 return s;
}

/* The authored Arrivals plate is intentionally narrow: only Reorte primary. */
static int station_authored_arrivals_at(void){
 return game.system==39&&game.station_variant==0&&game.docked&&!game.dead;
}

static const char *station_shell_tag(int room){
 static const char *tags[]={"ROUTE / ARRIVALS","PARTS / COUNTER","RUMOUR / SERVICE","CRATES / LIFT","EVIDENCE / GUILD","MED / CLINIC","SCAN / LAW"};
 return room>=0&&room<7?tags[room]:"STATION / DECK";
}

static void station_shell_frame(int x,int y,int w,int h,int selected){
 StationShellStyle s=station_shell_style();
 unsigned c=selected?s.warm:s.rule;
 rect(x,y,w,1,c);rect(x,y+h-1,w,1,s.rule);
 rect(x,y,1,h,s.rule);rect(x+w-1,y,1,h,s.rule);
 rect(x+3,y+2,8,1,s.cream);rect(x+3,y+2,1,5,s.cream);
 rect(x+w-11,y+h-3,8,1,s.cyan);rect(x+w-3,y+h-7,1,5,s.cyan);
}

static void station_shell_rule(int x,int y,int w,unsigned accent){
 StationShellStyle s=station_shell_style();
 rect(x,y,w,1,s.rule);rect(x+2,y+1,w-4,1,mix_rgb(accent,s.ink,.35f));
}

#endif
