/* Authored chapters have their own state; legacy Guild bonuses stay intact.
 * Only real docking arrival emits RETURN. Recovery and police custody do not.
 */
enum { CP_LAUNCH=1, CP_LOCK=2, CP_RETURN=3, CP_GUIDANCE=4 };
enum { CP_LOCKED=1, CP_FLEW=2, CP_GUIDED=4 };
typedef struct { int stage,event,next; } CampaignTransition;
static const CampaignTransition campaign_transitions[]={
 {1,CP_LAUNCH,2},{2,CP_LOCK,3},{2,CP_RETURN,5},
 {3,CP_RETURN,5},{4,CP_RETURN,5}
};
static inline int campaign_training(const Game *g){return g->campaign_stage>=1&&g->campaign_stage<5;}
static inline const char *campaign_task(const Game *g){
 static const char *tasks[]={"Meet Kei at Lave System Hub.","Launch when you are ready.",
 "Lock Lave Hub. Fly 600 m, then return.","Fly 600 m, then return to Lave Hub.",
 "Approach Lave Hub and press Circle to dock.","Report to Kei for your harbour badge.",
 "Harbour badge earned. The channel stays open."};
 int s=g->campaign_stage;return tasks[s>=0&&s<=6?s:0];
}
