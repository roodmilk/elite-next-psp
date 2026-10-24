/* Station voices are varied flavour, never docking/warrant state changes. */
static const char *station_hail_line(int hub){
 static const char *clear[]={
 "Good flying, traveller. You're clear to dock. We'll keep a berth warm for you.",
 "How's it going? We, er... we've got space here if you need a berth.",
 "Welcome in, Commander. Nice to hear a friendly voice. Dock whenever you're ready.",
 "Station control here. Clear approach, open berth. Come on in.",
 "Looking for a quiet place to park? You've found one. Docking is available.",
 "Hello out there. The kettle's on and the approach is clear.",
 "Good to see you, traveller. Take your time on the way in; we're not going anywhere.",
 "You're looking steady from here. We've got a berth with your name on it. Well, a number, technically.",
 "Afternoon, Commander. Or morning. We stopped checking. You're welcome to dock.",
 "Signal received. Doors open, lights on. Follow the approach and we'll see you inside.",
 "Welcome to our little corner of the system. Docking is available if you'd like a break.",
 "You've reached station control. Yes, a real person. Yes, we have room.",
 "Hello, pilot. Nothing urgent on our end. Come in when you're ready.",
 "Clear to dock, Commander. Easy on the paintwork; we've only just finished the entrance.",
 "Station control receiving. Safe approach, clear berth. Glad you made it.",
 "Long flight? There's room here to put your feet up. Once you've parked, obviously.",
 "Good flying, Commander. Keep it smooth through the entrance and you're home.",
 "We see you, traveller. Plenty of room. Circle requests your docking approach.",
 "Welcome back to civilisation. We have supplies, repairs and a perfectly respectable roof.",
 "Docking available. No rush, Commander. We'd rather have you arrive in one piece."
 };
 static const char *far[]={"Station control receiving. Come a little closer and we'll arrange your approach.","Hello, traveller. We have room, but you're outside docking range. Keep coming.","We hear you, Commander. Approach the station before requesting docking.","Signal received. You're still a fair way out. We'll talk you in once you're closer."};
 static const char *wanted[]={"Station control. There's a local warrant on your ship. Law clearance is required before entry.","We hear you, Commander. Your local fine needs settling before we can admit you.","Hold outside the entrance. Local Law must clear your warrant before you dock.","Your ship is flagged by Local Law. Resolve the warrant before entering the station."};
 static const char *docked[]={"You're safely berthed, Commander. Welcome aboard.","Station control here. You're already parked. Enjoy your stay.","All secure at your berth. Let us know when you're ready to leave."};
 static unsigned serial[256][HUB_COUNT];static uint8_t last[256][HUB_COUNT];
 if(hub<0||hub>=HUB_COUNT)hub=0;
 const char **lines=clear;int group=0,n=sizeof(clear)/sizeof(clear[0]);
 if(game.wanted[game.system]>0||game.legal>0){lines=wanted;group=1;n=sizeof(wanted)/sizeof(wanted[0]);}
 else if(game.docked){lines=docked;group=2;n=sizeof(docked)/sizeof(docked[0]);}
 else if(length(sub(game.pos,hub_position(&game,hub)))>station_comms_range(&game,hub)){lines=far;group=3;n=sizeof(far)/sizeof(far[0]);}
 unsigned pick=(serial[game.system][hub]+++(unsigned)game.system*13u+(unsigned)hub*7u)%n;
 if(last[game.system][hub]==group*32+pick+1)pick=(pick+1)%n;
 last[game.system][hub]=group*32+pick+1;return lines[pick];
}
