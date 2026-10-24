"""Compile the fixed NEWS collection. No prose is generated on the PSP.

The collection is an editorial draft, not a claim of 256 bespoke reviewed papers.
JSON is the editable source after --draft; normal invocation only compiles it.
Certification records never infer a manual review from an automated fit check.
"""
import argparse, hashlib, json, textwrap
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / 'content/newspapers/collection.json'
FAMILIES = ['Civic broadsheet', 'Dockworker bulletin', 'Financial review',
 'Gossip tabloid', 'Celebrity magazine', 'Frontier circular', 'Government record',
 'Independent investigation', 'Science journal', 'Pirate rag', 'Society review',
 'Cultural journal', 'School paper', 'Military circular', 'Satirical sheet', 'Personal journal']
# x,y,w,h,kind (0 text, 1 art, 2 puzzle). All rectangles are native pixels.
RECIPES = [
 [(20+i*89,84,85,156,0) for i in range(5)],
 [(20,84,440,48,0),(20,138,216,102,0),(244,138,216,102,0)],
 [(20,84,208,156,1),(236,84,224,76,0),(236,166,224,74,0)],
 [(20,84,224,76,0),(20,166,224,74,0),(252,84,208,156,1)],
 [(20,84,272,68,0),(20,158,272,82,1),(300,84,160,76,0),(300,166,160,74,0)],
 [(20,84,144,76,0),(20,166,144,74,0),(172,84,144,156,1),(324,84,136,156,0)],
 [(20,84,96,96,1),(124,84,336,96,0),(20,186,440,54,0)],
 [(20,84,176,92,1),(204,84,256,92,0),(20,182,216,58,0),(244,182,216,58,0)],
 [(20,84,264,156,1),(292,84,168,76,0),(292,166,168,74,0)],
 [(20,84,144,96,0),(172,84,144,96,0),(324,84,136,96,0),(20,186,440,54,1)],
 [(20,84,112,156,1),(140,84,156,156,0),(304,84,156,156,0)],
 [(20+(i%3)*149,84+(i//3)*80,142,76,0) for i in range(6)],
 [(20,84,216,156,0),(244,84,216,76,0),(244,166,216,74,0)],
 [(20,84,248,156,2),(276,84,184,76,0),(276,166,184,74,0)],
 [(20,84,264,76,1),(20,166,264,74,0),(292,84,168,156,0)],
 [(20,84,440,76,1),(72,176,336,64,0)]
]
PALETTES = [
 ('e8dfc4','242c35','425667','92774c','723d28','562c35'),
 ('ded4ac','292b24','4b573e','7e7154','424d28','7b302b'),
 ('d9e4dc','203834','435952','6d8a7c','31594d','7a3438'),
 ('e9d6d6','3b263a','65445c','aa7d92','71375d','86302b'),
 ('dce0ed','262b49','4b5272','8589a5','393e79','7a3548'),
 ('efd6af','3c3024','67503b','a7845a','7d461f','80382d'),
 ('d5e3e9','223744','40596b','7a949e','28536f','7b313b'),
 ('e2dccb','282827','565047','95866a','6d3827','7e2b2b'),
 ('ede9dd','1d3342','405763','8b9899','275c61','85352c'),
 ('cfc9b5','292b2b','4b4a42','8d806a','563936','792d2a'),
 ('eee3dc','342e39','65556a','ac958c','684168','7d3446'),
 ('e2e7cb','2b3829','4d6144','8a9772','4c592c','813b33'),
 ('eadfbf','313727','5d6042','a19a6d','60572b','783934'),
 ('d2d9ce','26372e','4e6351','82917d','3e5849','7a3834'),
 ('eee0cf','372d27','69584b','b59875','734c2e','85332d'),
 ('dfd6e8','302c40','57536a','9c89ac','674674','81384b'),
 ('e5e1d7','252525','505050','909090','3e3e3e','722c2c'),
 ('efe6c8','30352e','5e6553','aaa579','53633a','7d372b'),
 ('d7e4df','283c38','49675e','83a298','3e685b','793b38'),
 ('ecd9c1','3e2e2c','6c4d42','ae8774','784333','853a31'),
 ('d6dfec','26334b','4a6081','869bb4','355379','7d3543'),
 ('e9dce8','392d3e','64506d','a58ba8','6c4175','84334c'),
 ('e0e4e0','243739','466164','829797','326269','7d3636'),
 ('ebd9ca','3b302e','6e574c','b0947c','785040','853731')]

def galaxy():
    s=[0x5a4a,0x0248,0xb753]; pairs='..LEXEGEZACEBISOUSESARMAINDIREA.ERATENBERALAVETIEDORQUANTEISRION'
    result=[]
    for i in range(256):
        gov=(s[1]>>3)&7; econ=(s[0]>>8)&7
        if gov<2: econ|=2
        tech=((s[1]>>8)&3)+(econ^7)+(gov>>1)+(gov&1)
        long=s[0]&64; name=''
        for k in range(4):
            j=2*((s[2]>>8)&31)
            if k<3 or long: name+=''.join(c for c in pairs[j:j+2] if c not in '. ')
            s=[s[1],s[2],sum(s)&65535]
        a=0 if i==7 else i%12; count=[5,1,10,4,8,6,7,6,8,10,7,8][a]
        stars=0 if a==10 else 2 if a==5 else 3 if a==6 else 1
        result.append(dict(system=i,name=name,economy=econ,government=gov,tech=tech,archetype=a,stars=stars,worlds=count-stars))
    return result

def wrapped(s,w):
    # Match newspaper_wrap: whole words where possible, hard wrap only long words.
    lines=[]
    while s:
        n=min(len(s),w)
        if len(s)>w:
            pos=s.rfind(' ',0,w+1)
            if pos>0:n=pos
        lines.append(s[:n]); s=s[n:].lstrip(' ')
    return lines

def draft():
    first='Ada Bram Cora Dax Etta Finn Gita Huw Ivo Juno Kira Leon Mina Neri Oona Pell'.split()
    last='Vale Renn Sorn Beck Orr Voss Hale Marr Saye Thorne Dene Yarrow Kest Noll Ash Quill'.split()
    roots=['Lantern','Copper','Blueglass','Kestrel','Nightjar','Keel','Ember','Hearth','Tern','Spindle','Salt','Orchid','Meridian','Cinder','Peregrine','Moth']
    titles=['Record','Ledger','Echo','Observer','Whisper','Dispatch','Register','Enquiry','Review','Rag','Salon','Chorus','Notebook','Standard','Wink','Journal']
    mottos=['A place for every voice','The shift behind the ship','Follow the small print','Big ears, small secrets','Fame needs a good lamp','Written beyond the lights','For the public record','Ask the second question','Measure twice; publish once','No masters at the press','Good manners, sharp eyes','Culture needs breathing room','Small hands, large questions','Read before standing down','Taking nonsense seriously','A room of our own']
    subjects=['clock','garden','canteen','choir','lift','library','theatre','fountain','laundry','bench','mural','tram','bakery','observatory','workshop','nursery']
    angles=[
      ('A quiet victory','The committee wanted a monument. Residents asked for somewhere to sit. They got both when the old plinth became a bench.'),
      ('Who gets heard?','The hearing ran out of chairs before it ran out of objections. Its clerk moved everyone into the hall and stayed to listen.'),
      ('Behind the receipt','The cheapest quotation omitted maintenance. A cleaner noticed the missing line. The contract went back for another reading.'),
      ('A small scandal','The guest of honour arrived without an invitation. Nobody challenged her until she ate the ribbon meant for the ceremony.'),
      ('In good company','A celebrated singer asked for silence. The audience obliged. She used it to thank the technicians nobody had introduced.'),
      ('Keeping the lights','The lamp failed during supper. Two neighbours held torches while a third found the fault. Nobody let the food go cold.'),
      ('An open record','Officials published the minutes in plain language. Residents immediately found three questions the formal version had concealed.'),
      ('Follow the paper','Two invoices used the same serial number. The supplier blamed a printer. The accountant kept both originals for inspection.'),
      ('Try it again','The first experiment failed. The second failed differently. The team published both results rather than inventing a triumph.'),
      ('An honest thief?','The missing collection tin came back heavier. Its anonymous donor also returned a spoon. The committee declined to ask why.'),
      ('Seats at the table','A dinner planner separated guests by rank. The cook seated everyone by appetite instead. Conversation improved immediately.'),
      ('A song survives','The old recording was almost silent. A listener recognised the rhythm and tapped it out. The choir supplied the missing voices.'),
      ('Whose question?','A pupil asked who cleans the display cases. The class cancelled its ceremonial photograph and went to meet the cleaning crew.'),
      ('After the parade','The band left first. The people folding chairs stayed last. Their foreman says the next ceremony should start with their names.'),
      ('The grand opening','A dignitary unveiled the new curtain. Behind it was another curtain. The paper congratulates the supplier on a secure business model.'),
      ('A borrowed room','The tenant left a shelf of books. The next occupant added a kettle. A third left a note asking everyone to keep the room kind.')]
    govs=['Anarchy','Feudal','Multi-government','Dictatorship','Communist','Confederacy','Democracy','Corporate']
    econ=['Rich industrial','Average industrial','Poor industrial','Mainly industrial','Mainly agricultural','Rich agricultural','Average agricultural','Poor agricultural']
    quotes=[
     'A public bench is more useful than a private plaque.',
     'Listening costs less than another locked door.',
     'A cheap repair can become a very expensive habit.',
     'Ask who ate the ribbon before blaming the guests.',
     'Thank the people who make the microphone work.',
     'Fix the lamp before discussing a grand unveiling.',
     'Plain words leave fewer places to hide a mistake.',
     'Keep the receipt, especially when it looks perfect.',
     'A failed experiment is still a result worth sharing.',
     'Our missing spoon has a remarkably guilty owner.',
     'Good manners begin with feeding the kitchen staff.',
     'A quiet voice still deserves a place in the choir.',
     'The best question was asked after the lesson ended.',
     'Someone always stays behind to fold the chairs.',
     'Behind every great curtain is another invoice.',
     'A borrowed room should be left a little kinder.'
    ]
    notices=[
     'Warm seats are not a luxury.', 'Good questions deserve chairs.',
     'Read the small print twice.', 'The ribbon was edible. Sorry.',
     'Applaud the technicians too.', 'Neighbours shared their torches.',
     'The minutes now use plain words.', 'Both invoices were retained.',
     'Failure belongs in the report.', 'Please return the other spoon.',
     'The cook has the final word.', 'The choir kept the silence too.',
     'The cleaning crew came to lunch.', 'The last chair has a name.',
     'No more ceremonial curtains.', 'Someone has mended the kettle.'
    ]
    out=[]
    for f in galaxy():
        i=f['system']; a=i//16; b=i%16; family=(f['government']*3+f['economy']+a)%16
        name=f'{roots[a]} {titles[b]}'
        special={'LAVE':'The Lave Lantern','DISO':'The Orchard Voice','REORTE':'The Second Shift','LEESTI':'The Bright Ledger','QUATOR':'The Quiet Measure','RIEDQUAT':'The Loose Cannon','TIONISLA':'Names in the Dark'}
        name=special.get(f['name'],name)
        editor=first[a]+' '+last[b]; reporter=first[(a+5)%16]+' '+last[(b+9)%16]
        business=roots[b]+' & '+roots[a]; district=roots[(a+b)%16]+' Walk'
        institution=roots[a]+' Circle'; celebrity=first[(a+9)%16]+' '+last[(b+3)%16]
        count=16 if i==7 else 4 if i==1 else 24 if i==255 else 8+2*((a+b)%5)
        ident=dict(seed=0x4e455700+i,publication_family=family,layout_family=b,masthead_style=a%4,palette=i%24,
           ink_style=a%3,border_style=b%4,headline_style=(a+b)%3,illustration_style=family,editorial_voice=family,
           political_lean=f['government'],sensationalism=family in (3,4,9,14),humour_level=family%4,
           advert_density=b%3,wear_level=2 if family in (5,9,15) else i%2,page_order_variant=a,page_count=count,
           masthead=name,motto=mottos[family])
        pages=[]
        for p in range(count):
            layout=(b+p*5+a)%16
            if p==count-2:layout=13
            angle=(family+p)%16; title,story=angles[angle]
            role=['LOCAL REPORT','OPINION','ANALYSIS','RUMOUR','SOCIETY','COMMUNITY','OFFICIAL CLAIM','INVESTIGATION','SCIENCE','SATIRE','CULTURE','LETTERS','SCHOOL DESK','SERVICE','CORRECTION','PEOPLE'][angle]
            if p>=16:
                story=[
                 'The new bench is already scratched. Its maker calls that evidence of use, not grounds for a complaint.',
                 'The clerk has ordered folding chairs. She says listening should not depend on how long somebody can stand.',
                 'The revised contract includes maintenance. The cleaner has been asked to read the final copy before anyone signs.',
                 'A correction: the ribbon was edible. The uninvited guest was its baker, who has now submitted an invoice.',
                 'The technicians recorded their own names for the credits. The singer insisted that nobody turn down their microphones.',
                 'The repaired lamp now illuminates a spare fuse. Supper has become a small neighbourhood tradition.',
                 'The three missing questions have been restored. Their answers remain blank; the clerk has refused to hide that fact.',
                 'The duplicate invoice has been withdrawn. The accountant has accepted an apology, but kept her copies.',
                 'Another team repeated the experiment. It failed in the same way, giving both groups something useful to discuss.',
                 'The returned spoon belongs to the treasurer. He denies that this makes him a suspect, especially before lunch.',
                 'The cook has been asked to plan another dinner. She has requested fewer titles and considerably more potatoes.',
                 'The choir has recorded its reconstruction. The original silence remains on the other side of the recording.',
                 'The cleaning crew joined the class for lunch. The display cases have acquired a small sign crediting their work.',
                 'The chair-folding crew led the next rehearsal. The band discovered how difficult it is to march carrying furniture.',
                 'The second curtain has been removed. Behind it was a wall. We apologise for suggesting an endless supply of curtains.',
                 'Someone has mended the kettle. The shelf now holds a notebook recording what each visitor chose to leave.'
                ][angle]
            head=f"{f['name']}: {title}"+(' / After the debate' if p>=16 else '')
            factual=f"{f['name']} has {f['stars']} {'star' if f['stars']==1 else 'stars'} and {f['worlds']} {'world' if f['worlds']==1 else 'worlds'}."
            local=f"At {district}, {institution} is debating its {subjects[(b+p)%16]}."
            # Fixed complete copy, then compactly divided into self-contained print modules.
            adjective=['decentralised','feudal','regional','dictatorial','communal','federal','democratic','corporate'][f['government']]
            texts=[f"{local} {story}",f"{reporter} writes from {f['name']}. The economy is {econ[f['economy']].lower()}; {adjective} institutions shape local arguments.",
                   f"{business}: repairs with a written estimate. Tea is on the house; the kettle is not for sale.",
                   f"Letter from {celebrity}: Please keep the {subjects[(b+p)%16]} open to everyone. A public room needs more than a plaque.",
                   f"{institution} thanks its volunteers. The {district} noticeboard now credits the people who build it.",
                   f"{editor}: We print opinions as opinions. A loud claim is not a better fact."]
            blocks=[]; t=0
            for m,(x,y,w,h,kind) in enumerate(RECIPES[layout]):
                if kind:
                    art=(angle+m)%12
                    caption=['SKY STUDY','HUB SKETCH','COBRA MK III','PORTRAIT','CIVIC SEAL','MARKET CRATES','PUBLIC RECORD','NIGHT SHIFT','CLUB CIRCUIT','ROUTE STUDY','WORKSHOP','NIP & TUG'][art]
                    blocks.append(dict(kind=kind,title='NUMBER GARDEN' if kind==2 and i%2==0 else 'WORD DOCK' if kind==2 else caption,label='',byline='',body='',art=art));continue
                ht=head if t==0 else ['PRINT DESK','COMMERCIAL','LETTERS','COMMUNITY','EDITORIAL'][min(t-1,4)]+' / '+f['name']
                label=('OFFICIAL' if role=='OFFICIAL CLAIM' else role if role in ('OPINION','ANALYSIS','SATIRE','RUMOUR') else 'REPORT') if t==0 else 'ADVERT' if t==2 else 'OPINION'
                by=reporter if t==0 else editor
                by=by[0]+'. '+by.split()[-1]
                # Select complete sentences that fit; never cut a sentence to conceal overflow.
                cols=(w-8)//8; rows=(h-8)//9
                title_lines=wrapped(ht,cols)
                if len(title_lines)>2:ht=f['name']+' / '+subjects[(b+p+t)%16].upper()+(' II' if p>=16 else '')
                if len(wrapped(ht,cols))>2:ht=f['name']
                overhead=len(wrapped(ht,cols))+2
                available=rows-overhead
                short=[f"{editor.split()[-1]}: {quotes[angle]}",
                       f"{reporter.split()[-1]}: {notices[angle]}",
                       f"{business}: Estimates include labour.",
                       f"{celebrity.split()[-1]}: The {subjects[(b+p)%16]} belongs to everyone.",
                       f"{institution}: Thank the volunteers.",
                       f"{editor.split()[-1]}: Evidence before applause."][t%6]
                tiny=[f"{district}: {notices[angle]}",f"{reporter.split()[-1]}: Ask who pays.",
                      f"{business}: Tea included.",f"{celebrity.split()[-1]}: Keep it public.",
                      f"{institution}: All welcome.",f"{editor.split()[-1]}: Check the facts."][t%6]
                options=[texts[t%6],short,tiny,notices[angle]]
                body=next((v for v in options if len(wrapped(v,cols))<=available),'')
                if not body:raise ValueError((f['name'],p,m,cols,rows,ht))
                blocks.append(dict(kind=0,title=ht,label=label,byline=by,body=body,art=0));t+=1
            pages.append(dict(layout=layout,role=role,headline=head,lead=texts[0],fact=factual,blocks=blocks,
               puzzle=1 if layout==13 and i%2==0 else 2 if layout==13 else 0))
        out.append(dict(**f,identity=ident,editor=editor,reporter=reporter,business=business,district=district,
                        institution=institution,celebrity=celebrity,pages=pages,writer='Codex authored banks / fixed local draft',
                        editorial_review='pending individual prose review',native_review='pending individual 480x272 inspection'))
    DATA.parent.mkdir(parents=True,exist_ok=True);DATA.write_text(json.dumps(out,indent=2,ensure_ascii=True)+'\n')

def compile_collection():
    papers=json.loads(DATA.read_text()); assert len(papers)==256
    earth=json.loads((DATA.parent/'earth.json').read_text())
    tutorial=dict(system=256,name='SOL',identity=dict(papers[7]['identity']),pages=[],writer='Codex original Earth copy',editorial_review='source reviewed; native inspection pending',native_review='pending individual 480x272 inspection')
    tutorial['identity'].update(seed=0x4e50534f,publication_family=0,layout_family=12,masthead_style=1,palette=6,wear_level=0,page_count=4,masthead=earth['masthead'],motto=earth['motto'])
    for record in earth['pages']:
        title,body,side_title,side_body,ad_title,ad_body=record;bs=[]
        for k,(x,y,w,h,kind) in enumerate(RECIPES[12]):
            bs.append(dict(kind=0,art=0,title=[title,side_title,ad_title][k],body=[body,side_body,ad_body][k],label=['REPORT','OPINION','ADVERT'][k],byline=['S. Wren','J. Calder','Print desk'][k]))
        tutorial['pages'].append(dict(layout=12,blocks=bs,puzzle=0,headline=title,lead=body,fact='Earth is the home of this local paper.'))
    strings=[]; pool={}
    def sid(v):
        if v not in pool:pool[v]=len(strings);strings.append(v)
        return pool[v]
    ids=[];pages=[];blocks=[];manifest=[];heads=set();signatures=set(); fits=[]
    for i,paper in enumerate(papers+[tutorial]):
        d=paper['identity'];assert paper['system']==i and d['page_count']==len(paper['pages']) and 4<=d['page_count']<=24
        ids.append('{'+','.join(str(int(d[k])) for k in ['seed','publication_family','layout_family','masthead_style','palette','ink_style','border_style','headline_style','illustration_style','editorial_voice','political_lean','sensationalism','humour_level','advert_density','wear_level','page_order_variant','page_count'])+','+json.dumps(d['masthead'])+','+json.dumps(d['motto'])+'}')
        for n,page in enumerate(paper['pages']):
            assert page['headline'] not in heads;heads.add(page['headline'])
            signature=hashlib.sha256(json.dumps([d,page],sort_keys=True).encode()).hexdigest();assert signature not in signatures;signatures.add(signature)
            pages.append('{%d,%d,%d,%d,%d}'%(page['layout'],len(page['blocks']),len(blocks),page['puzzle'],sid(page['fact'])))
            for b,rect in zip(page['blocks'],RECIPES[page['layout']]):
                x,y,w,h,kind=rect
                if not kind:
                    rows=sum(len(wrapped(b[k],(w-8)//8)) for k in ('title','body'))+2
                    if rows*9>h-8:fits.append((i,n,b['title'],rows,h))
                elif kind==1 and len(wrapped(b['title'],(w-8)//8))>2:
                    fits.append((i,n,b['title'],'caption exceeds two native rows',h))
                blocks.append('{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}'%(x,y,w,h,kind,b['art'],sid(b['title']),sid(b['label']),sid(b['byline']),sid(b['body']),0))
            manifest.append(dict(system=i,name=paper['name'],page=n,signature=signature,writer=paper['writer'],editorial_review=paper['editorial_review'],native_review=paper['native_review']))
    assert not fits,fits[:10]
    fields='unsigned seed,publication_family,layout_family,masthead_style,palette,ink_style,border_style,headline_style,illustration_style,editorial_voice,political_lean,sensationalism,humour_level,advert_density,wear_level,page_order_variant,page_count;char masthead[32],motto[48];'
    out=['/* Fixed compiled newspaper data. Edit content/newspapers/collection.json. */',f'typedef struct {{{fields}}} NewspaperIdentity;',
      'typedef struct {unsigned short x,y,w,h,kind,art,title,label,byline,body,reserved;} NewspaperBlock;',
      'typedef struct {unsigned short layout,count,first,puzzle,fact;} NewspaperPage;',
      'static const NewspaperIdentity newspaper_identity[256]={'+',\n'.join(ids[:256])+'};',
      'static const NewspaperIdentity newspaper_earth_identity='+ids[256]+';',
      'static const char *const newspaper_strings[]={'+',\n'.join(json.dumps(s) for s in strings)+'};',
      'static const NewspaperBlock newspaper_blocks[]={'+',\n'.join(blocks)+'};',
      'static const NewspaperPage newspaper_pages[]={'+',\n'.join(pages)+'};']
    offsets=[];n=0
    for p in papers:offsets.append(n);n+=len(p['pages'])
    out.append('static const unsigned short newspaper_offsets[256]={'+','.join(map(str,offsets))+'};')
    out.append('static const unsigned short newspaper_earth_offset='+str(n)+';')
    out.append('static const unsigned char newspaper_background_economy[256]={'+','.join(str(p['economy']) for p in papers)+'};')
    out.append('static const unsigned newspaper_palette[24][6]={'+','.join('{'+','.join('0xff'+c[4:6]+c[2:4]+c[:2]+'u' for c in pal)+'}' for pal in PALETTES)+'};')
    (ROOT/'src/newspaper-data.h').write_text('\n'.join(out)+'\n')
    (DATA.parent/'editorial-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(f'{len(papers)} papers; {len(pages)} complete page records; {len(blocks)} blocks; {len(strings)} pooled strings; fit checks pass. Editorial review remains pending.')

def apply_major_editions():
    papers=json.loads(DATA.read_text());major=json.loads((DATA.parent/'major-editions.json').read_text())
    for paper in papers:
        if paper['name'] not in major:continue
        m=major[paper['name']];paper.update({k:m[k] for k in ('editor','reporter','business','district','institution','celebrity')})
        paper['identity']['publication_family']=m['family'];paper['identity']['editorial_voice']=m['family']
        paper['identity']['motto']=['A place for every voice','The shift behind the ship','Follow the small print','Big ears, small secrets','Fame needs a good lamp','Written beyond the lights','For the public record','Ask the second question','Measure twice; publish once','No masters at the press','Good manners, sharp eyes','Culture needs breathing room','Small hands, large questions','Read before standing down','Taking nonsense seriously','A room of our own'][m['family']]
        # A complete feature with a separate editorial reply. No sentence clipping.
        page=paper['pages'][0];page.update(layout=12,headline=paper['name']+': '+m['headline'],lead=m['body'])
        blocks=[]
        for k,(x,y,w,h,kind) in enumerate(RECIPES[12]):
            author=m['reporter'] if k==0 else m['editor'];by=author[0]+'. '+author.split()[-1]
            title=m['headline'] if k==0 else 'EDITORIAL' if k==1 else m['business']
            body=m['body'] if k==0 else m['reply'] if k==1 else m['advert']
            # Side modules are shorter than the full-height lead column.
            if k==1:body=m['reply'].split(': ',1)[-1]
            if k==2:body=m['advert'].split(': ',1)[-1]
            rows=(h-8)//9;cols=(w-8)//8
            if len(wrapped(title,cols))+2+len(wrapped(body,cols))>rows:
                # Use a full-height second column rather than abbreviating final copy.
                page['layout']=12
                body=('Opinion: '+body.split('. ')[0]+'.') if k==1 else body.split('. ')[0]+'.'
            blocks.append(dict(kind=0,title=title,label='REPORT' if k==0 else 'OPINION' if k==1 else 'ADVERT',byline=by,body=body,art=0))
        page['blocks']=blocks
    DATA.write_text(json.dumps(papers,indent=2,ensure_ascii=True)+'\n')

def import_issue(name):
    """Explicitly import a complete authored issue; never shorten its copy."""
    papers=json.loads(DATA.read_text())
    issue=json.loads((DATA.parent/(name.lower()+'-issue.json')).read_text())
    paper=next(p for p in papers if p['name']==name.upper())
    assert len(issue['pages'])==paper['identity']['page_count']
    for n,record in enumerate(issue['pages']):
        page=paper['pages'][n];layout=record['layout'];blocks=[]
        assert len(record['blocks'])==len(RECIPES[layout])
        for raw,rect in zip(record['blocks'],RECIPES[layout]):
            x,y,w,h,kind=rect
            if kind:
                assert isinstance(raw,dict) and (kind==2)==('puzzle' in raw)
                blocks.append(dict(kind=kind,art=raw.get('art',0),title=raw['caption'],label='',byline='',body=''))
            else:
                title,label,byline,body=raw
                cols=(w-8)//8
                rows=len(wrapped(title,cols))+2+len(wrapped(body,cols))
                assert rows*9<=h-8,(n,title,rows,(h-8)//9)
                assert len(wrapped(label,cols))==1 and len(wrapped(byline,cols))==1
                blocks.append(dict(kind=0,art=0,title=title,label=label,byline=byline,body=body))
        lead=next(b for b in blocks if b['kind']==0)
        page.update(layout=layout,role=record['role'],blocks=blocks,puzzle=record.get('puzzle',0),headline=paper['name']+': '+lead['title'],lead=lead['body'])
    paper.update(writer=issue['writer'],editor=issue['editor'],reporter=issue['reporter'],editorial_review='original issue source reviewed; native inspection pending')
    DATA.write_text(json.dumps(papers,indent=2,ensure_ascii=True)+'\n')

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--draft',action='store_true');p.add_argument('--issue');a=p.parse_args()
    if a.draft:draft();apply_major_editions()
    if a.issue:import_issue(a.issue)
    compile_collection()
