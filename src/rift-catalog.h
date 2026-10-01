#ifndef RIFT_CATALOG_H
#define RIFT_CATALOG_H
/* Stable system/slot taxonomy, shared by the field, scan report and archive. */
static inline int rift_type(int system,int slot){return (slot&1)?3:(system*17+slot*7)%3;}
static const char *rift_names[]={"AURORA VEIL","GRAVITY LACE","EMBER NURSERY","MERIDIAN ECHO"};
static const char *rift_reports[]={
 "Aurora Veil identified. Charged dust follows braided magnetic filaments, producing curtains of cyan and violet light. The bright knots drift through the veil without closing its hollow centre.",
 "Gravity Lace identified. Gold and turquoise filaments trace a folded-looking lattice around a dark centre. The pattern resembles a lens repeatedly trying to focus the same distant star.",
 "Ember Nursery identified. Rose-coloured clouds surround slow amber sparks. Each spark brightens and fades while the outer threads curl inward, like ash rising through a fire in reverse.",
 "Meridian Echo identified. Pale blue arcs overlap around a pearl-white core. Their uneven pulses resemble a signal arriving along several paths at once; no identifiable sender appears in the scan."
 };
static const char *rift_lore[]={
 "Old survey crews called these fields the curtains between nights. Their light looks solid until a ship crosses it. This scan is observational: the veil is not a gate, and no destination has been detected. Your flight controls remain unaffected.",
 "The Guild's nickname is a sailor's knot: a shape that looks impossible until viewed from another bearing. Pilots tell stories of lost minutes inside the lace, but this instrument pass confirms no clock shift. It does not transport or pull your ship.",
 "The name comes from the nursery-lamp glow, not a confirmed newborn star. Prospectors once followed its sparks hoping for precious ore; the light offered nothing they could scoop. This is a research discovery, not a mine or a fuel source.",
 "Relay folklore calls it a message that forgot where it was going. Researchers catalogue each echo separately to compare its structure. The repeating light is not a distress call or a conversation, and the field does not provide a jump route."
 };
#endif
