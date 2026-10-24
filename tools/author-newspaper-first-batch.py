"""Authored first-batch issue sources and native layout recipes.

This is a finite editorial import, not a generator for the remaining galaxy.
Every article below is written for its named publication and page role.
"""
from pathlib import Path
import json

ROOT=Path(__file__).resolve().parents[1]
PATH=ROOT/'content/newspapers/approved-issues.json'
data=json.loads(PATH.read_text())
issues={i['name']:i for i in data['issues']}

def text(x,y,s,ink,scale=1):return dict(kind='text',x=x,y=y,text=s.upper(),ink=ink,scale=scale)
def para(x,y,cols,s,ink,bottom=240):return dict(kind='paragraph',x=x,y=y,cols=cols,text=s,ink=ink,bottom=bottom)
def box(x,y,w,h,ink):return dict(kind='box',x=x,y=y,w=w,h=h,ink=ink)
def page(issue,n,role,recipe,ops,thread='',continuation=None):
    p=dict(page=n,role=role,recipe=recipe,authored=True,edited=False,proofread=False,thread=thread,ops=ops)
    previous=next((old for old in issue['pages'] if old['page']==n and old['ops']==ops),None)
    if previous:p['edited']=previous.get('edited',False);p['proofread']=previous.get('proofread',False)
    if continuation:p['continuation']=continuation
    issue['pages']=[p0 for p0 in issue['pages'] if p0['page']!=n]+[p]
    issue['pages'].sort(key=lambda p:p['page'])

def spread(issue,n,role,recipe,title,byline,left,right,thread='',continuation=None):
    bg,fg,accent,rule,*_=issue['palette']
    titles=title.split('|');assert len(titles)<=2
    ops=[text(16,88+j*19,t,fg,2) for j,t in enumerate(titles)]
    y=110 if len(titles)==1 else 130
    ops += [text(16,y,byline,accent),box(239,y+14,1,240-y-14,rule),
            para(16,y+16,27,left,fg),para(248,y+16,27,right,fg)]
    page(issue,n,role,recipe,ops,thread,continuation)

l=issues['LAVE'];bg,fg,a,r,*_=l['palette']
spread(l,1,'LOCAL REPORT','local-report','THE WALK REOPENS','IONA REED / ARRIVAL WALK',
 "A cargo trolley used to fit the old ramp. A chair did not. The new ramp opened quietly after cleaners measured it with borrowed wheelchairs, rather than another committee's tape.",
 "Pell Sorn made the first crossing with his mother. He refused a ribbon. 'Leave the route clear,' he said. The Night Shift Circle will check the handrails after each heavy delivery.", 'Arrival Walk')
spread(l,2,'LONG FEATURE','feature-opening','THE OTHER SIDE|OF THE WELCOME','IONA REED / CONTINUES: AFTER THE LAST ARRIVAL',
 "At the end of the public walkway, a narrow door opens into a room without windows. Here, cleaners pin reports beside a drawing of the station. A dark lamp is a place somebody might fall.",
 "The Night Shift Circle began when three crews stopped leaving separate notes. They now hand the whole map to the next shift. Apprentices sign their repairs, and ask a second person to check them.", 'Night Shift Circle','AFTER THE LAST ARRIVAL')
spread(l,3,'FEATURE CONTINUATION','feature-conclusion','AFTER THE LAST ARRIVAL','IONA REED / CONCLUSION: THE OTHER SIDE OF THE WELCOME',
 "An apprentice shows me a lamp she repaired. It casts no brighter light than its neighbours. Her work is visible only on the map. That is why Pell asked for the names outside: ordinary success deserves a witness.",
 "Before I leave, the next crew arrives. Nobody makes a speech. They compare the map with the walkway, then begin again. The welcome is not the sign above the dock. It is the promise that somebody checked the way home.", 'Night Shift Circle')
page(l,4,'TRADE','trade-ledger',[
 text(16,88,'REPAIR BEFORE REPLACE',fg,2),text(16,111,'IONA REED / WORKSHOP DESK',a),
 para(16,128,29,"Lantern Repairs has opened its spare-parts shelf to apprentices. Borrowers leave a drawing of the fault and return a working part, or explain why it failed. The shelf already contains more notes than parts.",fg),
 box(268,124,196,113,r),box(270,126,192,109,bg),text(280,133,'ON THE SHELF',a),
 text(280,152,'LAMP CONTACTS',fg),text(280,166,'SEAL SAMPLES',fg),text(280,180,'TEST LEADS',fg),
 para(280,201,22,"Ask first. The tools belong to the next learner too.",fg,234)
 ],'Lantern Repairs')
page(l,5,'PUBLIC AFFAIRS','paired-notices',[
 text(16,88,'WHO GETS A KEY?',fg,2),text(16,111,'MARA BELL / EDITORIAL',a),
 para(16,128,27,"The Circle's map should not become a private club. Publish the inspection notes where residents can read them. A repaired handrail is useful. A public record lets us ask why it broke twice.",fg),
 box(245,124,219,116,r),box(247,126,215,112,bg),text(256,134,'NOTICE / WALKWAY WORKS',a),
 para(256,151,25,"The east service door remains closed during repairs. Follow the signed public route. Workers say the temporary path is accessible; readers can report a blockage at the arrival desk.",fg,235)
 ],'Arrival Walk')
page(l,6,'SCIENCE','field-diagram',[
 text(16,88,'A LAMP IS A PROMISE',fg,2),text(16,111,'NELL ARDEN / FIELD NOTES',a),
 para(16,127,29,"A bright lamp can hide a poor view. The Circle tests the floor beneath each fitting, not the bulb itself. A hood over one lamp removed a glare patch and made a worn step visible without using more power.",fg),
 box(286,126,178,103,r),box(288,128,174,99,bg),text(300,136,'LOOK AT THE FLOOR',a),
 box(370,154,20,7,fg),box(378,161,4,20,a),box(333,211,90,3,fg),
 box(356,190,48,2,a),box(344,201,72,2,a),text(296,234,'HOODED LIGHT / SKETCH',a)
 ],'Night Shift Circle')
spread(l,7,'PEOPLE','interview-columns','PELL SORN, OFF SHIFT','INTERVIEW / IONA REED',
 "What changed your mind about the lamp names? 'An apprentice thought her repair did not count because it was small. I remembered every small repair that got me safely through a shift. I wanted her to see that.'",
 "What will you do when the walkway is finished? 'Walk somewhere else. My mother wants to see the public gardens. I have spent years giving directions to places I have never visited.'", 'Pell Sorn')
page(l,8,'LETTERS','letters-three',[
 text(16,88,'LETTERS TO THE LANTERN',fg,2),text(16,111,'SELECTED BY MARA BELL / NAMES PRINTED WITH PERMISSION',a),
 para(16,130,17,"The new rail is low enough for my son. Please keep that height when you repair the next section. Useful things deserve copying. - E. Vale",fg),
 box(157,129,1,109,r),para(170,130,17,"I used to avoid the glare by looking down. The hooded light lets me look ahead. Thank the person who tested it. - M. Cale",fg),
 box(312,129,1,109,r),para(326,130,17,"Print the map, but leave private crew details out. A public repair record should not become a public work rota. - T. Moss",fg)
 ],'Arrival Walk')
page(l,9,'CULTURE','culture-poster',[
 text(16,88,'THE WALK HAS A VOICE',fg,2),
 box(16,115,190,123,fg),text(28,126,'NIGHT SHIFT',bg,2),text(28,147,'OPEN CHOIR',bg,2),
 para(28,174,21,"No audition. No uniform. Bring a song somebody taught you.",bg,226),
 text(224,116,'PELL SORN / CULTURE',a),para(224,133,30,"The Circle's choir will sing beside the repaired walkway. Crews have chosen songs from their homes, including a lullaby nobody can agree how to finish. The audience may supply the missing ending.",fg),
 text(224,224,'PUBLIC CONCOURSE / ALL WELCOME',a)
 ],'Night Shift Circle')
page(l,10,'TRAVEL','walking-map',[
 text(16,88,'TAKE THE LONG WAY',fg,2),text(16,111,'NELL ARDEN / ON FOOT',a),
 para(16,128,29,"Arrival Walk is more than a queue. Leave the cargo lane, follow the public rail and look through the observation windows. The new ramp joins the garden route. Service doors are not shortcuts.",fg),
 box(285,125,179,106,r),box(287,127,175,102,bg),text(298,135,'PUBLIC WALK / SKETCH',a),
 box(310,158,18,18,fg),box(326,165,98,3,a),box(421,165,3,43,a),box(411,205,23,17,fg),
 text(294,183,'ARRIVAL',a),text(350,220,'GARDEN',fg),text(290,234,'KEEP CARGO LANES CLEAR',a)
 ],'Arrival Walk')
page(l,11,'SPORT','sport-results',[
 text(16,88,'THE SLOWEST FINAL',fg,2),text(16,111,'DAREN FENN / CONCOURSE GAMES',a),
 para(16,128,28,"The maintenance crews' table-ball final stopped when a bearing rolled beneath the score desk. Both teams helped retrieve it. They finished the game after the evening handover, with the apprentice side winning by one.",fg),
 box(260,126,204,112,r),box(262,128,200,108,bg),text(274,137,'FINAL SCORE',a),
 text(274,159,'APPRENTICES      7',fg),text(274,178,'OLD HANDS        6',fg),para(274,201,22,"The losing side has offered to mend the table.",fg,232)
 ],'Night Shift Circle')
page(l,12,'OPINION','editorial-reply',[
 text(16,88,'KEEP THE MAP OPEN',fg,2),text(16,111,'MARA BELL / EDITOR',a),
 para(16,128,27,"A reader is right to defend the crews' privacy. We asked for public repair records, not private rotas. The arrival desk can publish faults and completed work without naming who will be alone on a late shift.",fg),
 text(248,128,'A REPLY FROM THE CIRCLE',a),para(248,146,27,"We will put a public copy beside the desk. It will show the route and any unfinished repairs. The crew copy stays in our room. Thank you for asking before somebody printed the wrong one. - Pell Sorn",fg)
 ],'Arrival Walk')
page(l,13,'CLASSIFIEDS','classified-columns',[
 text(16,88,'SMALL NOTICES',fg,2),text(16,111,'LOCAL CLASSIFIEDS / CONTACT THE ARRIVAL DESK',a),
 text(16,130,'LANTERN REPAIRS',a),para(16,146,26,"A flicker is a fault, not a mood. Bring the whole fitting if you can. We repair before recommending a replacement.",fg,202),
 text(16,211,'LOST / BLUE MITTEN',a),para(16,225,26,"One small owner misses it.",fg,242),
 box(239,128,1,113,r),text(248,130,'GARDEN WALK GROUP',a),para(248,146,27,"Steady pace, plenty of stops. Meet at the public rail. Anyone who has only seen the gardens through a window is especially welcome.",fg,211),
 text(248,221,'CHOIR NEEDS AN ENDING',a),text(248,234,'SINGERS NEED NOT AGREE.',fg)
 ],'Arrival Walk')
page(l,14,'PUZZLE','puzzle-editorial',[
 dict(kind='puzzle',x=16,y=84,w=208,h=156),text(248,88,'A QUIET CORNER',fg),
 para(248,105,27,"Fill each row, column and outlined square with the numbers one to nine. No number repeats. You can leave your notes here and return later; the paper keeps your place.",fg,185),
 text(248,193,'FROM THE PUZZLE DESK',a),para(248,209,27,"Pell claims this is harder than a lamp circuit. The apprentices disagree.",fg,241)
 ],'Night Shift Circle')
page(l,15,'BACK PAGE','backpage-invitation',[
 text(16,88,'YOUR NAME BELONGS HERE',fg,2),text(16,111,'THE LANTERN / NEXT EDITION',a),
 para(16,128,28,"Our next issue follows the first public garden walk. Tell us what you noticed, what blocked your way, or who made the journey easier. We want more witnesses than a departure board can hold.",fg),
 box(261,125,203,114,fg),text(275,138,'WRITE TO US',bg,2),para(275,166,22,"Leave a letter at the arrival desk. Say whether we may print your name. A useful correction is welcome too.",bg,233)
 ],'Arrival Walk')

d=issues['DISO'];bg,fg,a,r,*_=d['palette']
spread(d,1,'LOCAL REPORT','local-report','THE PACKETS COME HOME','TARIN QUILL / GREEN ARCADE',
 "The Seed Library has received its first returned harvest. Borrowers brought seeds in folded paper, with notes about shade and water. One packet contained only an apology: a child had eaten the last fruit before anyone saved its seeds.",
 "Nella Vane accepted the apology as a useful record. 'Now we know it tastes good,' she said. The empty packet will stay beside the others until the family tries again. Nobody has been fined.", 'Seed Library')
spread(d,2,'LONG FEATURE','feature-opening','BENEATH THE ORCHARD','TARIN QUILL / CONTINUES: A HARVEST NOBODY OWNS',
 "Green Arcade's library began with a drawer beneath a market stall. Growers left seeds they could spare. A handwritten card asked borrowers to return a little of their harvest. The drawer became a cupboard, then a room.",
 "The room has no silent section. Visitors compare failures as eagerly as flowers. Nella keeps the failed trials where newcomers can find them. A plant that died on one shaded balcony may thrive on another.", 'Seed Library','A HARVEST NOBODY OWNS')
spread(d,3,'FEATURE CONTINUATION','feature-conclusion','A HARVEST NOBODY OWNS','TARIN QUILL / CONCLUSION: BENEATH THE ORCHARD',
 "The difficult question is not what to grow. It is who gets to decide. A grower wanted her family's seed packet kept for neighbours. Nella asked her to meet a stranger who had borrowed it. They compared the two harvests.",
 "They agreed to keep the family name on the packet and let anyone borrow it. The name tells a story; it does not lock a door. A public garden, Nella says, grows badly if every visitor must first prove they belong.", 'Seed Library')
page(d,4,'TRADE','trade-ledger',[
 text(16,88,'ROOM IN THE COLD STORE',fg,2),text(16,111,'TARIN QUILL / GROWERS DESK',a),
 para(16,128,29,"Three small growers are sharing a chilled cupboard behind the Orchard Table. Each marks a shelf and records when food goes in. The restaurant gets first refusal on surplus, not ownership of the harvest.",fg),
 box(270,126,194,112,r),box(272,128,190,108,bg),text(282,138,'ORCHARD TABLE',a),
 para(282,157,21,"A meal made from what grew well. Ask whose garden supplied your plate. We can usually point to the table where they are sitting.",fg,232)
 ],'Orchard Table')
spread(d,5,'PUBLIC AFFAIRS','paired-notices','GROUND FOR EVERYONE','SELA MOSS / EDITORIAL',
 "A common garden needs rules people can see. Leave its public paths open, even when a crop is delicate. Explain a temporary fence. A visitor should not need to guess whether they are trespassing beside their own market.",
 "The growers have offered a compromise: a marked tending lane, with a wider route around it. Before approving the plan, take a chair, a delivery trolley and a child through it. A drawing cannot tell us who fits.", 'Green Arcade')
page(d,6,'SCIENCE','field-diagram',[
 text(16,88,'DO NOT WASTE THE SEED',fg,2),text(16,111,'NELLA VANE / PRACTICAL BOTANY',a),
 para(16,128,29,"Test a small sample before sowing the whole packet. Keep the paper damp, not flooded, and note how many seeds open. The library records the test conditions as well as the result. A bare percentage hides the useful part.",fg),
 box(290,128,174,101,r),box(292,130,170,97,bg),text(302,138,'A SMALL TRIAL',a),
 box(313,170,12,10,fg),box(342,170,12,10,fg),box(371,170,12,10,fg),box(400,170,12,10,fg),
 box(319,155,2,15,a),box(348,158,2,12,a),box(377,161,2,9,a),text(302,201,'RECORD WHAT HAPPENED',a),text(298,234,'NOT ALL SEEDS OPEN.',a)
 ],'Seed Library')
spread(d,7,'PEOPLE','interview-columns','THE FIRST EMPTY PACKET','SELA MOSS / MEETS A NEW GROWER',
 "Jori thought the library would refuse a second loan after the fruit disappeared at lunch. 'My daughter was proud of growing it,' he says. 'I was proud too, until I read the return instructions.'",
 "Nella gave them another packet and a larger label: SAVE ONE FRUIT. Jori has put it above the bowl. His daughter has added a drawing of a very stern tomato. The library wants a copy for its noticeboard.", 'Seed Library')
page(d,8,'CULTURE','culture-poster',[
 text(16,88,'A SONG FOR EACH NAME',fg,2),box(16,115,190,123,fg),
 text(28,127,'ORCHARD',bg,2),text(28,148,'SONGBOOK',bg,2),para(28,179,21,"Bring a tune named after a plant. Forgotten verses welcome.",bg,230),
 text(224,116,'TARIN QUILL / CULTURE',a),para(224,133,30,"The Arcade singers are collecting songs named after lost varieties. Some names survive only in a chorus. Growers will listen for clues, but the evening is a concert, not a promise to recover every vanished fruit.",fg),text(224,224,'GREEN ARCADE / OPEN REHEARSAL',a)
 ],'Green Arcade')
page(d,9,'LETTERS','letters-three',[
 text(16,88,'LETTERS FROM THE PATH',fg,2),text(16,111,'EDITED BY SELA MOSS',a),
 para(16,130,17,"Our family name on a packet means someone cared for it. Please keep the name when you share the seeds. - R. Vane",fg),box(157,129,1,109,r),
 para(170,130,17,"A public path cannot become private whenever a fruit looks tempting. Mark the beds, not the people. - O. Marr",fg),box(312,129,1,109,r),
 para(326,130,17,"The stern tomato worked. One fruit is now waiting beside the seed tray. My daughter guards it. - Jori",fg)
 ],'Seed Library')
page(d,10,'PUZZLE','puzzle-editorial',[
 dict(kind='puzzle',x=16,y=84,w=208,h=156),text(248,88,'THE ORCHARD DESK',fg),
 para(248,105,27,"The little word grid is a place to stop between errands. Open it to enter letters. Your answers remain when you close the paper, so there is no need to finish before dinner.",fg,185),
 text(248,193,'A NOTE FROM NELLA',a),para(248,209,27,"A wrong answer can be rubbed out. Growing things is less forgiving.",fg,241)
 ],'Seed Library')
page(d,11,'BACK PAGE','backpage-invitation',[
 text(16,88,'THERE IS ROOM AT TABLE',fg,2),text(16,111,'ORCHARD TABLE / LOCAL ADVERTISEMENT',a),
 dict(kind='art',asset='seed',x=16,y=134,w=150,h=100),
 para(184,132,35,"Borrowing a garden is easier when someone shows you where to begin. The Seed Library's growers meet here after the market. Bring a question, a spare packet, or the story of a plant that surprised you.",fg,220),
 text(184,228,'SOUP WITH A STORY. EVERYONE WELCOME.',a)
 ],'Orchard Table')

q=issues['QUATOR'];bg,fg,a,r,*_=q['palette']
spread(q,1,'LOCAL REPORT','local-report','THE GALLERY OPENS','BREN ASTER / QUIET GALLERY',
 "The Quiet Gallery has opened without a loudspeaker. A guide meets visitors at the door and offers a printed note: listen, notice, repeat. Nobody is asked to stop breathing or pretend they cannot hear the ventilation.",
 "One visitor returned to the first room after hearing it from the second. Another left after a minute. Guide Oren Fen says both visits count. The gallery measures attention, not endurance.", 'Quiet Gallery')
spread(q,2,'LONG FEATURE','feature-opening','LISTEN WITHOUT KEEPING','BREN ASTER / CONTINUES: THE RIGHT TO LEAVE NO TRACE',
 "The Listening Society has spent years making recordings. Its new exhibition makes none. Visitors hear a room directly, then describe it on paper. A disagreement about a footstep becomes a conversation, rather than a file to replay.",
 "Oren Fen calls this a limit, not a discovery. The exercise cannot tell us exactly what each person heard. It can show which details they noticed, and how another person's description changes the next attempt.", 'Listening Society','THE RIGHT TO LEAVE NO TRACE')
spread(q,3,'FEATURE CONTINUATION','feature-conclusion','THE RIGHT TO LEAVE NO TRACE','BREN ASTER / CONCLUSION: LISTEN WITHOUT KEEPING',
 "A visitor asked whether the written notes would be displayed. The Society had not decided. They stopped the exercise and asked everyone in the room. Two people wanted their notes back. They got them.",
 "The remaining notes are anonymous, but even that is a choice. A sound can be shared without being owned. The gallery's most useful rule now hangs beside the door: ask before keeping somebody else's experience.", 'Listening Society')
page(q,4,'SCIENCE','field-diagram',[
 text(16,88,'NO ROOM IS SILENT',fg,2),text(16,111,'OREN FEN / OBSERVATION NOTES',a),
 para(16,128,29,"The ventilation produces a steady low sound. Footsteps interrupt it; voices rise above both. This sketch shows a listening exercise, not a calibrated recording. A neat diagram should never claim more than its evidence.",fg),
 box(286,128,178,100,r),box(288,130,174,96,bg),text(298,138,'ILLUSTRATIVE ONLY',a),
 text(298,158,'AIR',a),box(354,162,96,1,a),text(298,179,'STEP',a),box(354,183,30,1,a),box(385,174,2,18,fg),box(388,183,62,1,a),text(298,206,'LISTEN AGAIN.',fg)
 ],'Quiet Gallery')
page(q,5,'TRADE','trade-ledger',[
 text(16,88,'BEFORE THE INSTRUMENT',fg,2),text(16,111,'ELIN VOSS / TRADE DESK',a),
 para(16,128,29,"Stillwater Optics now asks customers what they intend to observe before recommending a lens. A visitor who wanted to see a small mark was offered a lamp and a cleaning cloth first. The mark turned out to be on the glass.",fg),
 box(270,126,194,112,fg),text(282,138,'STILLWATER',bg,2),text(282,159,'OPTICS',bg,2),para(282,188,21,"Clean lenses. Clear estimates. Tell us what you need to see.",bg,232)
 ],'Stillwater Optics')
spread(q,6,'PUBLIC AFFAIRS','paired-notices','WHO OWNS THE RECORDING?','ELIN VOSS / EDITORIAL',
 "Consent is not a notice placed where nobody looks. If a public exercise makes a record of its visitors, say what will be kept, who can read it and how to withdraw it. Ask before the exercise, not after the publication.",
 "The Society's pause was a good correction. It should become normal practice. The Quiet Measure will ask the same questions before printing visitors' notes. An interesting result does not make a private person public property.", 'Listening Society')
spread(q,7,'PEOPLE','interview-columns','A GUIDE WHO WAITS','BREN ASTER / OREN FEN',
 "What do you say when a visitor hears nothing unusual? 'Nothing. I do not want them inventing an answer for me. Sometimes they notice a sound on the way out. Sometimes the room is simply a room.'",
 "Does that make the exhibition a failure? 'Only if we promise a revelation. We offer a chance to pay attention. People are allowed to spend that chance differently, or decide they would rather go outside.'", 'Quiet Gallery')
page(q,8,'CULTURE','culture-poster',[
 text(16,88,'ONE PERFORMANCE ONLY',fg,2),box(16,115,190,123,fg),text(28,127,'LISTENING',bg,2),text(28,148,'ROOM',bg,2),
 para(28,179,21,"No recording. No replay. A concert shared by its audience.",bg,230),
 text(224,116,'BREN ASTER / CULTURE',a),para(224,133,30,"The Society's musicians will perform without making an archive. The audience may talk about the concert afterwards. They are asked to leave recording devices off. The programme includes an explanation, not just a prohibition.",fg)
 ],'Listening Society')
page(q,9,'LETTERS','letters-three',[
 text(16,88,'WHAT WE HEARD',fg,2),text(16,111,'LETTERS / ELIN VOSS',a),
 para(16,130,17,"I heard my own boots and felt foolish. The guide did not laugh. That helped more than a clever explanation. - A. Rill",fg),box(157,129,1,109,r),
 para(170,130,17,"Without a recording, how can we check the claim? Please call these notes impressions, not measurements. - M. Fen",fg),box(312,129,1,109,r),
 para(326,130,17,"We agree. The gallery has changed the card to read 'listening notes'. Thank you for the correction. - Oren Fen",fg)
 ],'Quiet Gallery')
page(q,10,'PUZZLE','puzzle-editorial',[
 dict(kind='puzzle',x=16,y=84,w=208,h=156),text(248,88,'THE FIELD NOTEBOOK',fg),
 para(248,105,27,"A word puzzle makes a different promise from an experiment: every white square has a definite answer. Open the grid, enter letters and test the possibilities. Closing the paper keeps your work.",fg,194),
 para(248,208,27,"The gallery accepts uncertainty. This desk is rather stricter.",a,241)
 ],'Quiet Gallery')
page(q,11,'BACK PAGE','backpage-invitation',[
 text(16,88,'TAKE NOTHING BUT A NOTE',fg,2),text(16,111,'QUIET GALLERY / AN INVITATION',a),
 para(16,128,28,"You need no instrument to visit. Read the card, choose a room and stay as long as you wish. If you leave a note, tell the guide whether it may be shown. You can also leave nothing at all.",fg),
 box(261,125,203,114,fg),text(275,139,'LISTEN.',bg,2),text(275,162,'NOTICE.',bg,2),text(275,185,'REPEAT.',bg,2),para(275,213,22,"Or simply step outside.",bg,238)
 ],'Quiet Gallery')

v=issues['RIEDQUAT'];bg,fg,a,r,*_=v['palette']
spread(v,1,'LOCAL REPORT','local-report','THE WARDROBE RETURNS','VERA KNELL / CROOKED PASSAGE',
 "The Unlicensed Choir has returned most of the costumes it borrowed without asking. A sleeve, two hats and a magnificent false beard remain missing. Red Noll says the beard has accepted a better offer from another company.",
 "The owner of the wardrobe wants an inventory, not a review. The choir has agreed to mend the damaged hems before its next performance. Tickets will not be accepted as payment for the missing sleeve.", 'Unlicensed Choir')
spread(v,2,'LONG FEATURE','feature-opening','A CHOIR WITHOUT A PERMIT','VERA KNELL / CONTINUES: WHO GETS TO SING',
 "Red Noll says the choir is unlicensed because nobody could agree what licence a choir needs. Its singers meet behind the Bent Spoon, where the proprietor allows rehearsals provided they end before the washing-up starts.",
 "They sing borrowed songs in borrowed clothes. That sounds charming until the owner comes looking for a coat. Noll admits the show confused two meanings of public property. The next wardrobe will have a ledger.", 'Unlicensed Choir','WHO GETS TO SING')
spread(v,3,'FEATURE CONTINUATION','feature-conclusion','WHO GETS TO SING','VERA KNELL / CONCLUSION: A CHOIR WITHOUT A PERMIT',
 "A quiet singer shows me a coat she made herself. It has pockets for everyone else's dropped buttons. She joined because the choir let her stand at the back until she was ready to be heard. She now sings the opening line.",
 "The choir does not need stolen clothes to make a spectacle. It needs a stage, an audience and the courage to begin badly. Noll agrees. Then he asks whether the wardrobe owner might lend the beard, this time in writing.", 'Unlicensed Choir')
spread(v,4,'PUBLIC AFFAIRS','paired-notices','A STAGE IS NOT A DEBT','ROOK TALLOW / EDITORIAL',
 "The Bent Spoon can offer a stage without owning its singers. A rehearsal room is a gift only if the terms are clear. The proprietor should write down what is owed, including the washing-up, before another chorus becomes collateral.",
 "We also suggest asking before taking a coat. This paper favours public performance, private pockets and the return of all sleeves. A revolution in local theatre should not leave its neighbour cold.", 'Bent Spoon')
page(v,5,'TRADE','trade-ledger',[
 text(16,88,'PAYMENT IN BUTTONS?',fg,2),text(16,111,'VERA KNELL / BENT SPOON',a),
 para(16,128,29,"The Spoon's repair night has attracted more clothes than singers. Volunteers will mend small tears while owners learn the stitch. Red Noll has been given the easiest task: sorting buttons. None may be used as currency.",fg),
 box(270,126,194,112,a),text(282,138,'BENT SPOON',bg,2),para(282,168,21,"A hot plate. A firm chair. Bring your own coat. Leave with the same one.",bg,229)
 ],'Bent Spoon')
page(v,6,'CULTURE','culture-poster',[
 text(16,88,'THE COSTUME PARADE',fg,2),dict(kind='art',asset='pirates',x=16,y=136,w=180,h=90),
 text(214,117,'RED NOLL / A PROMISE',a),para(214,135,31,"Our next show has a new rule: every costume must have its owner's permission. Expect a smaller parade and better pockets. The returned coats will sit in the front row, occupied by the people who paid for them.",fg,226),
 text(16,234,'THE ENCORE REMAINS OPTIONAL.',a)
 ],'Unlicensed Choir')
page(v,7,'LETTERS','letters-three',[
 text(16,88,'FROM THE BACK ROW',fg,2),text(16,111,'LETTERS / ROOK TALLOW',a),
 para(16,130,17,"I liked the show. I liked my coat more. Return the left sleeve and I may come to the next one. - The owner",fg),box(157,129,1,109,r),
 para(170,130,17,"Nobody stole my hat. I lent it. Please stop describing me as a victim of musical crime. - E. Marr",fg),box(312,129,1,109,r),
 para(326,130,17,"The beard is in the repair basket. It did not accept another offer. It needed a wash. - Spoon staff",fg)
 ],'Unlicensed Choir')
page(v,8,'PUZZLE','puzzle-editorial',[
 dict(kind='puzzle',x=16,y=84,w=208,h=156),text(248,88,'A CROOKED LITTLE GRID',fg),
 para(248,105,27,"Fit one to nine in every row, column and outlined square, without repeating a number. Open the grid to enter answers or pencil notes. The paper remembers them if you go looking for your coat.",fg,194),
 para(248,208,27,"No answer requires a stolen sleeve. We checked.",a,241)
 ],'Unlicensed Choir')
page(v,9,'BACK PAGE','backpage-invitation',[
 text(16,88,'NEXT TIME, ASK FIRST',fg,2),text(16,111,'THE LOOSE CANNON / NEXT PERFORMANCE',a),
 para(16,128,28,"The choir's next show will open with an inventory. The wardrobe owner has agreed to count the hats. If the numbers match, there will be singing. If they do not, there may still be a remarkable evening.",fg),
 box(261,125,203,114,a),text(275,139,'WANTED:',bg,2),text(275,161,'ONE SLEEVE',bg,2),para(275,192,22,"No bounty. Just return it to the Bent Spoon, please.",bg,234)
 ],'Unlicensed Choir')

e=issues['LEESTI'];bg,fg,a,r,*_=e['palette']
spread(e,1,'TRADE','trade-ledger','THE COST OF STOPPING','EDDA SHAW / FOUNDRY GALLERY',
 "Blue Arc Tools has added waiting time to its repair estimates. A cheap replacement can cost more than a careful repair if it stops a workshop for a day. The new form asks customers to describe what cannot happen while the tool is away.",
 "Dara Flint says the form is not an excuse to charge more. It lets staff compare a repair, a substitute and a replacement honestly. The Open Ledger Club has asked to see estimates where the cheapest choice also proved the best.", 'Blue Arc Tools')
spread(e,2,'LONG FEATURE','feature-opening','OPEN THE REPAIR LEDGER','NIKO RILL / MONEY AND WORK',
 "The Open Ledger Club began by comparing receipts. Its members found that a receipt rarely explained the largest cost: a delayed job, a borrowed tool or another person's evening spent finding a replacement part.",
 "Their new ledger leaves room for those costs without pretending every favour has a market price. Dara Flint has supplied three repair records. One saved money, one saved time and one should never have been attempted.", 'Open Ledger Club')
spread(e,3,'PEOPLE','interview-columns','A TOOL WITH A HISTORY','EDDA SHAW / FOUNDRY GALLERY',
 "The Foundry Gallery has put a repaired drill beside a new one. The old casing carries three owners' marks. Its label lists the changes, including a replacement grip made for a smaller hand. It is not being sold as a relic.",
 "Dara Flint wants visitors to ask what still works. 'Age is not a specification,' she says. 'Neither is shine.' The gallery invites owners to bring a repair story, but asks them to keep anything sharp in its case.", 'Foundry Gallery')
page(e,4,'SCIENCE','field-diagram',[
 text(16,88,'THE SUBSTITUTE SCREW',fg,2),text(16,111,'DARA FLINT / WORKSHOP NOTES',a),
 para(16,128,29,"Two fasteners may look alike and behave differently. Match the thread, length and material before testing a substitute. The missing screw story is about checking a part, not making a dangerous repair with whatever fits.",fg),
 box(286,128,178,100,r),box(288,130,174,96,bg),text(300,138,'COMPARE THE THREAD',a),
 box(321,165,80,5,fg),box(316,158,10,20,fg),box(343,162,2,11,a),box(354,162,2,11,a),box(365,162,2,11,a),box(376,162,2,11,a),box(387,162,2,11,a),
 text(300,204,'LOOKS ARE NOT A TEST.',a)
 ],'Blue Arc Tools')
page(e,5,'LETTERS','letters-three',[
 text(16,88,'WHOSE TIME COUNTS?',fg,2),text(16,111,'LETTERS / NIKO RILL',a),
 para(16,130,17,"My neighbour fixed the grip for free. I want her named in the ledger, not given an invented invoice. - B. Vale",fg),box(157,129,1,109,r),
 para(170,130,17,"Print failed repairs too. We learn little from a ledger that only records clever decisions. - T. Arden",fg),box(312,129,1,109,r),
 para(326,130,17,"We will. One of our first three records ends with a replacement. The repair was the wrong choice. - Dara Flint",fg)
 ],'Open Ledger Club')
page(e,6,'PUZZLE','puzzle-editorial',[
 dict(kind='puzzle',x=16,y=84,w=208,h=156),text(248,88,'THE ACCOUNTS DESK',fg),
 para(248,105,27,"The word grid has no hidden service charge. Open it, enter letters and return when you like. Your answers stay with this paper. The editor promises that a tidy ledger is not required to solve it.",fg,194),
 para(248,208,27,"Correction: the editor has yet to solve it himself.",a,241)
 ],'Open Ledger Club')
page(e,7,'BACK PAGE','backpage-invitation',[
 text(16,88,'ASK WHAT STILL WORKS',fg,2),text(16,111,'BLUE ARC TOOLS / LOCAL ADVERTISEMENT',a),
 dict(kind='art',asset='tools',x=16,y=134,w=150,h=100),
 para(184,132,35,"Bring the fault, not just a shopping list. We will explain the repair and the alternatives before you choose. The Bright Ledger's next audit will revisit our estimates, including the jobs we advised customers to replace.",fg,220),
 text(184,228,'CHEAPER TO BUY IS NOT ALWAYS CHEAPER.',a)
 ],'Blue Arc Tools')

plans={
 'LAVE':['Front: the lamp names','Local: an accessible public ramp','Feature: invisible maintenance','Continuation: checking the way home','Trade: the shared parts shelf','Affairs: public repair records','Science: glare and light hoods','People: Pell off shift','Letters: access and privacy','Culture: the shift choir','Travel: a public garden walk','Sport: apprentice table-ball','Opinion: reply on crew privacy','Classifieds: local notices','Puzzle: Number Garden','Back: invitation to the next walk'],
 'DISO':['Front: borrow a garden','Local: the returning seed packets','Feature: the library beneath the orchard','Continuation: a harvest nobody owns','Trade: small growers share cold storage','Affairs: who may use common ground','Science: testing a seed without wasting it','People: Sela Moss meets a new grower','Culture: songs named after lost varieties','Letters: borrowed ground, public paths','Puzzle: the orchard desk','Back: the open table'],
 'QUATOR':['Front: an experiment in silence','Local: the Quiet Gallery opens','Feature: listening without recording','Continuation: the right to leave no trace','Science: a room is not silent','Trade: instruments for careful observers','Affairs: who owns the recording','People: a patient gallery guide','Culture: a concert with no recording','Letters: visitors answer the gallery','Puzzle: field notebook','Back: an invitation to listen'],
 'RIEDQUAT':['Front: stolen costumes, stolen show','Local: the wardrobe returns in pieces','Feature: the Unlicensed Choir','Continuation: who gets to sing','Affairs: a public stage and private debts','Trade: repairs at the Bent Spoon','Culture: the costume parade','Letters: objections from the back row','Puzzle: a crooked little grid','Back: the next show'],
 'LEESTI':['Front: the missing screw','Trade: the cost of a stopped workshop','Feature: the open repair ledger','People: Edda Shaw visits the Foundry Gallery','Science: testing a substitute fastener','Letters: whose time counts','Puzzle: the accounts desk','Back: Blue Arc Tools and the next audit']}
for name,issue in issues.items():
    issue['plan_complete']=True
    issue['issue_plan']={'voice':{'LAVE':'Civic, warm, willing to correct itself.','DISO':'Communal, botanical, curious about ownership.','QUATOR':'Measured, scientific, alert to consent.','RIEDQUAT':'Irreverent, theatrical, suspicious of authority.','LEESTI':'Financial, practical, concerned with invisible costs.'}[name],
                         'contributors':issue['contributors'],'local_threads':issue['threads'],'page_roles':plans[name]}

PATH.write_text(json.dumps(data,indent=2)+'\n')
print('Five complete issue plans; 58 authored pages; approved fronts retained.')
