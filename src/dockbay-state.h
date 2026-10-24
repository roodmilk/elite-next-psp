/* V35 DockBay: bounded durable records; no dormant ship simulation. */
#ifndef DOCKBAY_STATE_H
#define DOCKBAY_STATE_H
#define DB_ADS 48
#define DB_SELLERS 24
#define DB_ITEMS 288
#define DB_RECEIPTS 32
#define DB_LIST_MAX (DB_ADS+DB_RECEIPTS)
#define DB_AD_WORDS 37
#define DB_ROLL_WORDS 8
#define DB_SAVE_BYTES ((8+DB_SELLERS*7+DB_ADS*DB_AD_WORDS+DB_ITEMS*8+48+5*48+10+2+DB_RECEIPTS*DB_AD_WORDS)*4)
enum {DB_AVAILABLE,DB_ROUTED,DB_MET,DB_PARTIAL,DB_COMPLETE,DB_CANCELLED,DB_REPORTED};
enum {DB_MODULE,DB_CARGO,DB_BUY_CARGO};
enum {DB_WATCH=1,DB_COUNTED=2,DB_REVEALED=4,DB_POST_ATTACK=8,DB_RECEIPT=16,DB_POINT=32,DB_ASSISTED=64,DB_DANGER_DONE=128,DB_TALKED=256,DB_ESCAPED=512,DB_LOOT_PENDING=1024,DB_LOOT_TAKEN=2048};
enum {DB_WORN,DB_NORMAL,DB_MODDED,DB_RARE};
typedef struct {uint32_t id,base,quality,generation,seed,power,secondary,value;} DockRoll;
typedef struct {uint32_t id,seed,portrait,meetings,trusted,blocked,counted_meeting;} DockSeller;
typedef struct {
 uint32_t id,seller,system,site,kind,item,stock,original,price,encounter,outcome,state,flags,revision;
 DockRoll roll;
 float x,y,z;
 uint32_t receipt_serial,receipt_qty,receipt_paid,receipt_seen,visits,meeting;
 float actor_health[3],actor_shield[3]; /* Seller and two possible attackers. */
} DockAd;
typedef struct {
 uint32_t ready,serial,route,receipt_serial,receipt_ad,dirty,resume,resume_system;
 DockSeller seller[DB_SELLERS];DockAd ad[DB_ADS];DockRoll owned[DB_ITEMS];
 uint32_t fitted[48],fleet[5][48];
 float px,py,pz,yaw,pitch,roll,speed,hull,shield,heat;
 uint32_t journal_n,journal_next;
 DockAd journal[DB_RECEIPTS]; /* Recent transaction receipts, independent of new stock. */
} DockBay;
#endif
