/* Reusable graph/layout audit. Does not change game or frontend state.
 * Call failures += opening_dialogue_validate(log) from the opening runner.
 */
#ifndef ELITE_OPENING_DIALOGUE_TESTS_H
#define ELITE_OPENING_DIALOGUE_TESTS_H
static int opening_dialogue_test_rows(const char *s,int cols){
 int rows=0;while(s&&*s){int len=(int)strlen(s),cut=len<cols?len:cols;
  if(len>cols)for(int k=cut;k>cols/3;k--)if(s[k]==' '){cut=k;break;}
  s+=cut;while(*s==' ')s++;rows++;
 }return rows;
}
static int opening_dialogue_test_index(int id){
 for(int i=0;i<opening_dialogue_count();i++)if(opening_dialogue_nodes[i].id==id)return i;return -1;
}
static void opening_dialogue_test_reach(int id,unsigned char *seen){
 int at=opening_dialogue_test_index(id);if(at<0||seen[at])return;seen[at]=1;
 const OpeningDialogueNode *n=&opening_dialogue_nodes[at];
 if(n->choice_count){for(int c=0;c<n->choice_count;c++)opening_dialogue_test_reach(n->choice_next[c],seen);}
 else if(n->next)opening_dialogue_test_reach(n->next,seen);
}
static int opening_dialogue_test_effect(int id){
 int effect=0;for(int guard=0;id&&guard<opening_dialogue_count();guard++){
  const OpeningDialogueNode *n=opening_dialogue_node(id);if(!n||n->choice_count)return -1;
  if(n->effect){if(effect)return -1;effect=n->effect;}id=n->next;
 }return id?-1:effect;
}
static int opening_dialogue_validate(FILE *log){
 int failures=0,count=opening_dialogue_count();unsigned char seen[512];memset(seen,0,sizeof(seen));
#define OD_CHECK(c,msg,id) do{if(!(c)){failures++;if(log)fprintf(log,"FAIL dialogue %d %s\n",id,msg);}}while(0)
 OD_CHECK(count==282,"authored node count",0);OD_CHECK(count<512,"bounded audit storage",0);
 if(count>=512)return failures;
 for(int stage=OP_DREAM;stage<=OP_AFTER_NEWS;stage++){
  const OpeningDialogueNode *entry=opening_dialogue_entry(stage);
  OD_CHECK(entry&&entry->id==stage*100+1,"every stage has stable entry",stage*100+1);
  if(entry)opening_dialogue_test_reach(entry->id,seen);
 }
 for(int i=0;i<count;i++){
  const OpeningDialogueNode *n=&opening_dialogue_nodes[i];
  OD_CHECK(opening_dialogue_node(n->id)==n,"node lookup identity and unique id",n->id);
  OD_CHECK(seen[i],"node reachable from stage entry",n->id);
  OD_CHECK(n->stage>=OP_DREAM&&n->stage<=OP_AFTER_NEWS,"valid stage",n->id);
  OD_CHECK(n->id/100==n->stage&&n->id%100>0,"stable stage-local id",n->id);
  OD_CHECK(n->id*10+2<=100000,"saved counter encoding bounded",n->id);
  OD_CHECK(n->speaker&&n->speaker[0]&&n->text&&n->text[0],"speaker and dialogue present",n->id);
  if(n->text){OD_CHECK(strlen(n->text)<=191,"reveal buffer fits",n->id);OD_CHECK(opening_dialogue_test_rows(n->text,48)<=5,"48 columns by 5 rows fits",n->id);}
  OD_CHECK(n->choice_count==0||n->choice_count==2||n->choice_count==3,"two or three choices",n->id);
  OD_CHECK(!n->choice_count||!n->next,"choice node has no implicit advance",n->id);
  int edges=n->choice_count?n->choice_count:(n->next?1:0);
  for(int c=0;c<edges;c++){
   int target=n->choice_count?n->choice_next[c]:n->next;
   const OpeningDialogueNode *next=opening_dialogue_node(target);
   OD_CHECK(next&&next->stage==n->stage,"edge resolves in same stage",n->id);
   OD_CHECK(target>n->id,"forward edge prevents conversation loops",n->id);
   if(n->choice_count){OD_CHECK(n->choices[c]&&n->choices[c][0],"choice label present",n->id);
    if(n->choices[c])OD_CHECK(strlen(n->choices[c])<=44,"choice label fits row",n->id);
    OD_CHECK(202+c*17+15<=251,"choice hit row inside panel",n->id);
   }
  }
  OD_CHECK(n->effect>=OD_EFFECT_NONE&&n->effect<=OD_EFFECT_HIDE_OBJECT,"valid effect",n->id);
  OD_CHECK(!n->effect||n->stage==OP_OBJECT_CHOICE,"object effect only on object decision",n->id);
 }
 const OpeningDialogueNode *object=opening_dialogue_entry(OP_OBJECT_CHOICE);
 OD_CHECK(object&&object->choice_count==2,"show or hide decision present",OP_OBJECT_CHOICE*100+1);
 if(object&&object->choice_count==2){
  OD_CHECK(opening_dialogue_test_effect(object->choice_next[0])==OD_EFFECT_SHOW_OBJECT,"show branch applies show exactly once then terminates",object->id);
  OD_CHECK(opening_dialogue_test_effect(object->choice_next[1])==OD_EFFECT_HIDE_OBJECT,"hide branch applies hide exactly once then terminates",object->id);
 }
 if(log)fprintf(log,"%s dialogue graph: %d nodes / entries, edges, terminals, effects, layout, save bounds\n",failures?"FAIL":"PASS",count);
#undef OD_CHECK
 return failures;
}
#endif
