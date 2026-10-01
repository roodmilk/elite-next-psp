/* Station-local reading state. Hovering never replaces committed text.
 * Bounded buffers; six 58-column lines/page, controls at y=258. */
static char sc_read_title[48],sc_read_text[2048];
static int sc_read_page=0;
static void sc_read_set(const char *title,const char *body){
 snprintf(sc_read_title,sizeof(sc_read_title),"%s",title?title:"");
 snprintf(sc_read_text,sizeof(sc_read_text),"%s",body?body:"");sc_read_page=0;
}
static void sc_read_pair(const char *title,const char *body,const char *after){
 sc_read_set(title,body);size_t n=strlen(sc_read_text);
 if(after&&*after&&n+2<sizeof(sc_read_text))snprintf(sc_read_text+n,sizeof(sc_read_text)-n,"  %s",after);
}
static const char *sc_read_advance(const char *p,int rows,int cols){
 while(rows--&&p&&*p){int len=(int)strlen(p),cut=len<cols?len:cols;
  if(len>cols)for(int k=cut;k>cols/3;k--)if(p[k]==' '){cut=k;break;}
  p+=cut;while(*p==' ')p++;
 }return p;
}
static int sc_read_pages(void){int n=0;const char *p=sc_read_text;do{n++;p=sc_read_advance(p,6,58);}while(p&&*p);return n;}
static void sc_read_room(void){sc_read_set(sc_room_title(sc_room),sc_room_blurb(sc_room));}
static const char *sc_contact_detail(const ScNpc *p){
 if(sc_lave()&&p->act>=40&&p->act<47){
  static const char *life[]={
   "Most people only notice this hall when something goes wrong. They see a delay on the board and think nobody is working. What they do not see is the mechanic under a loading arm, or a pilot trying to make an old freighter hold pressure. I try to remember that before I raise my voice. I do not always manage it.",
   "I used to throw damaged parts away. Then a freighter came in with a failed pump and a crew who could not afford a new one. We found the piece they needed in my scrap bin. These days I keep almost everything. My shelves look dreadful, but once in a while someone gets home because of them.",
   "You learn the rhythm of a place serving meals. The early shift wants quiet. The late shift wants someone to ask how their day went, provided you do not expect an honest answer straight away. Sit here long enough and they usually tell you. I think that matters as much as the food.",
   "My crew knows the difference between a delay and a disaster. A broken lift is a delay. A medicine crate that disappears into the wrong warehouse can become a disaster before anyone notices. That is why I keep asking about the paperwork. It is not the paper I am worried about; it is whoever is waiting at the other end.",
   "A good field report is not a list of impressive names. Tell me where you found something, what was around it and whether you have seen it before. Small observations become useful when somebody else can compare them with their own. That is how an archive becomes more than a room full of confident guesses.",
   "People apologise for coming in with small problems. I wish they would not. An uncomfortable seal or an untreated burn is much easier to deal with here than halfway through a long flight. You spend enough time looking after your ship. Remember there is someone inside it who needs looking after too.",
   "There are days when everybody thinks my job is to stop them leaving. Mostly I am trying to make sure the cargo is what the label says it is. The honest crews are usually tired, and the dishonest ones are often very polite. You learn not to confuse either of those things with evidence."
  };return life[p->act-40];
 }
 if(p->act==SC_ACT_SHOP)return "Tell me what your ship needs before you spend anything. A replacement is only useful if it fits the right slot, and the cheapest part is not a bargain if you have to come straight back. You can look through the stock without committing to a purchase; I would rather you asked a question than bought the wrong thing.";
 if(p->act==SC_ACT_MANIFEST)return "Every crate has passed through somebody's hands before it reaches mine. Usually the paperwork travels with it. When it does not, I cannot simply guess where it belongs. Have a look at the loose sheet in the bay, then bring it back to me. We can get this berth moving again once the record is straight.";
 if(p->act==SC_ACT_TAXI)return "I do not need a luxury liner. A proper passenger berth and a pilot who knows where they are going will do. I have spent enough evenings watching departure boards change their minds. If your cabin is free, we can arrange the journey here; otherwise I will keep looking.";
 if(p->role==EXPLORERS)return "The useful discoveries are not always spectacular. Sometimes it is a plant growing where an old report said nothing could survive. Sometimes it is a familiar mineral in an unexpected place. Keep a record when you are out there. A careful observation is something another pilot can actually use.";
 if(p->role==LAW)return "Most of the people who pass this desk are simply trying to finish their day and get home. I would prefer to help them do that. Keep your cargo declarations honest and sort out any outstanding trouble before you leave; it is easier to talk here than across an open channel with a patrol waiting for an answer.";
 return "You get used to recognising people by their routines. Someone always checks the departure board twice. Someone else sits down before they have taken their gloves off. Pilots call this a stop between journeys, but for the people on shift it is where the day happens. We remember who takes a moment to speak to us.";
}
static void sc_read_contact(const ScNpc *p){
 if(sc_lave()&&p->act>=40&&p->act<47)sc_read_set(p->name,p->line);
 else sc_read_pair(p->name,p->line,sc_contact_detail(p));
}
static const char *sc_landmark_detail(int room){
 static const char *notes[]={
 "The glass gives you a view of the berth traffic without any of its noise. A freighter dominates the near distance, its shape broken up by loading structures and lights. Beyond it, the planet curves away into darkness. It is an impressive view, though the worn seats nearby suggest that regular travellers mostly use it as somewhere to wait.",
 "The counter is kept clear enough to work on, even though almost every surface behind it carries stock. You can see parts that have been cleaned and checked alongside boxes still waiting their turn. This is where a vague complaint about a ship becomes a conversation about an actual fitting. Speak to the Chandler if you want to browse the available deck stock.",
 "The bar has been wiped down, but years of glasses and elbows have left a finish no amount of cleaning will quite remove. It feels used rather than neglected. From this side of the counter you can see the shelves, the stools and the places where somebody has stopped for a few minutes between jobs. The bartender is close enough to speak without raising your voice.",
 "The lifting rig hangs over the loading area, heavy enough that you find yourself looking at its supports before walking beneath it. Its working surfaces are marked by repeated use. For now the surrounding cargo is stationary. The loose manifest near the rail looks far less substantial than the equipment, but paperwork can hold up a berth just as effectively as a failed hoist.",
 "Routes and reference points cover the star chart. Seen from across the room, the lines make travel look almost simple; standing closer, you notice how much interpretation is hidden in every connection. The specimens below it give the display a useful sense of scale. Those distant marks represent places where someone has actually stood and looked around.",
 "The treatment couch has enough space around it for the doctor to work without stepping over equipment. Nearby displays and supplies are positioned for use, not for show. You can imagine how different the quiet room would feel with a patient arriving in a hurry. If you need the clinic's service, select TREATMENT rather than disturbing the equipment yourself.",
 "The scanner gate is wide enough for the loads moving through this part of the station. Its frame separates cargo waiting for inspection from cargo ready to leave, a small physical boundary with a great deal of paperwork behind it. The officer's desk is within sight of the passage. If you have a question about clearance, that is where the answer will come from."
 };return notes[room>=0&&room<7?room:0];
}

