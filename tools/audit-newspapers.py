"""Offline collection audit; never marks editorial/native inspection complete."""
import collections, hashlib, importlib.util, json, re
from pathlib import Path
R=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('news',R/'tools/compile-newspapers.py');news=importlib.util.module_from_spec(spec);spec.loader.exec_module(news)
papers=json.loads(news.DATA.read_text());errors=[];warnings=[];heads=collections.Counter();bodies=collections.Counter();visible_heads=collections.Counter();visible_leads=collections.Counter();layouts=set();palettes=set();families=set();signatures=set()
if len(papers)!=256:errors.append('Collection must contain exactly 256 canonical galaxy editions')
def lum(s):
 v=[int(s[i:i+2],16)/255 for i in (0,2,4)];v=[x/12.92 if x<=.04045 else ((x+.055)/1.055)**2.4 for x in v];return sum(x*y for x,y in zip(v,(.2126,.7152,.0722)))
for i,p in enumerate(papers):
 d=p['identity'];palettes.add(d['palette']);families.add(d['publication_family'])
 canonical=news.galaxy()[i]
 for key in ('system','name','economy','government','tech','archetype','stars','worlds'):
  if p[key]!=canonical[key]:errors.append(f'{i}: canonical {key} mismatch')
 if not(4<=len(p['pages'])<=24 and len(p['pages'])==d['page_count']):errors.append(f'{i}: configured length mismatch')
 if len(d['masthead'])>=32 or len(d['motto'])>=48:errors.append(f'{i}: identity string exceeds native buffer')
 if not(0<=d['palette']<24 and 0<=d['publication_family']<16):errors.append(f'{i}: invalid palette/family')
 for n,page in enumerate(p['pages']):
  layouts.add(page['layout']);rects=news.RECIPES[page['layout']]
  sig=hashlib.sha256(json.dumps([d,page],sort_keys=True).encode()).hexdigest()
  if sig in signatures:errors.append(f'{i}:{n}: duplicate signature')
  signatures.add(sig);heads[page['headline']]+=1;bodies[page['lead']]+=1
  lead=next(b for b in page['blocks'] if b['kind']==0)
  visible_heads[lead['title']]+=1;visible_leads[lead['body']]+=1
  for k,b in enumerate(page['blocks']):
   x,y,w,h,kind=rects[k]
   if b['kind']!=kind or not(0<=b['art']<12):errors.append(f'{i}:{n}:{k}: invalid module kind/art')
   if not(20<=x and 84<=y and x+w<=464 and y+h<=240):errors.append(f'{i}:{n}:{k}: bounds')
   for xx,yy,ww,hh,_ in rects[:k]:
    if x<xx+ww and x+w>xx and y<yy+hh and y+h>yy:errors.append(f'{i}:{n}:{k}: overlap')
   if not kind:
    cols=(w-8)//8
    for key in ('label','byline'):
     if len(news.wrapped(b[key],cols))>1:errors.append(f'{i}:{n}:{k}: {key} wraps')
    rows=sum(len(news.wrapped(b[key],cols)) for key in ('title','body'))+2
    if rows*9>h-8:errors.append(f'{i}:{n}:{k}: text overflow')
   elif kind==1 and len(news.wrapped(b['title'],(w-8)//8))>2:
    errors.append(f'{i}:{n}:{k}: caption overflow')
   for key in ('title','label','byline','body'):
    text=b[key]
    if re.search(r'\{\{|\}\}|\b(?:TODO|TBD|PLACEHOLDER|lorem|bastard)\b',text,re.I):errors.append(f'{i}:{n}:{k}: unresolved copy')
    if any(ord(c)>126 or ord(c)<32 for c in text):errors.append(f'{i}:{n}:{k}: unsupported font character')
for p,pal in enumerate(news.PALETTES):
 for ink in (1,2,4,5):
  contrast=(max(lum(pal[0]),lum(pal[ink]))+.05)/(min(lum(pal[0]),lum(pal[ink]))+.05)
  if contrast<4.5:errors.append(f'Palette {p} ink {ink}: contrast {contrast:.2f} below 4.5')
for h,n in heads.items():
 if n>1:errors.append(f'Duplicate headline: {h}')
for h,n in bodies.items():
 if n>1:errors.append(f'Duplicate complete lead paragraph: {h}')
if len(layouts)!=16 or len(families)!=16 or len(palettes)!=24:errors.append('Missing layout/family/palette coverage')
pending=sum(p['editorial_review'].startswith('pending') for p in papers)
warnings.append(f'{pending} editions still need bespoke prose review; bank-based drafts are not approved authored final copy.')
warnings.append('Individual 480x272 inspections remain pending until explicitly recorded by a reviewer.')
editorial_issues=dict(duplicate_visible_headlines=sum(n-1 for n in visible_heads.values() if n>1),duplicate_visible_lead_paragraphs=sum(n-1 for n in visible_leads.values() if n>1),bespoke_edition_review_pending=pending)
warnings.append('Uniqueness of metadata does not prove unique printed copy: inspect editorial_issues before approving the collection.')
report=dict(papers=len(papers),pages=len(signatures),errors=errors,warnings=warnings,technical_checks_pass=not errors,editorial_issues=editorial_issues,editorial_certified=False)
(R/'content/newspapers/audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2));raise SystemExit(bool(errors))
