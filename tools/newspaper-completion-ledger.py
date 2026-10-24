"""Build an honest, hash-bound page ledger. Captures never imply acceptance."""
from pathlib import Path
import argparse,hashlib,json

ROOT=Path(__file__).resolve().parents[1]
CONTENT=ROOT/'content/newspapers'

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--captures',type=Path);args=ap.parse_args()
    corpus=json.loads((CONTENT/'collection.json').read_text())
    approved=json.loads((CONTENT/'approved-issues.json').read_text())['issues']
    reviews_path=CONTENT/'native-page-reviews.json'
    reviews=json.loads(reviews_path.read_text()) if reviews_path.exists() else {}
    audit=json.loads((CONTENT/'composition-audit.json').read_text())
    checks={(x['system'],x['page']):x for x in audit}
    authored={i['system']:i for i in approved}
    editions=[]
    earth=json.loads((CONTENT/'earth.json').read_text())
    for system,issue in list(enumerate(corpus))+[(256,earth)]:
        first=authored.get(system);pages=[]
        for n,legacy in enumerate(issue['pages']):
            page=next((p for p in first['pages'] if p['page']==n),None) if first else None
            check=checks.get((system,n),{})
            source=check.get('source_sha256') if page else hashlib.sha256(json.dumps(legacy,sort_keys=True).encode()).hexdigest()
            file=args.captures/('news-earth-p%02d.png'%n if system==256 else 'news-s%03d-p%02d.png'%(system,n)) if args.captures else None
            image_hash=sha(file) if file and file.exists() else None
            review=reviews.get('%03d:%02d'%(system,n),{})
            inspected=bool(image_hash and review.get('image_sha256')==image_hash and review.get('source_sha256')==source)
            accepted=bool(inspected and review.get('accepted') and review.get('edited') and review.get('proofread') and review.get('corrections_resolved') and page and first['plan_complete'] and all(check.get(k) for k in ('automated_bounds','automated_overlap','font_safe','contrast_pass','duplication_pass')))
            pages.append(dict(page=n,role=page['role'] if page else (legacy.get('role','DRAFT') if isinstance(legacy,dict) else 'TUTORIAL'),source_sha256=source,
                identity_plan_complete=bool(first and first['plan_complete']),
                prose=dict(authored=bool(page),edited=bool(inspected and review.get('edited')),proofread=bool(inspected and review.get('proofread'))),
                native_assets_complete=bool(page),layout_implemented=bool(page),
                automated=dict(bounds=bool(check.get('automated_bounds')),overlap=bool(check.get('automated_overlap')),
                               font_safety=bool(check.get('font_safe')),contrast=bool(check.get('contrast_pass')),duplication=bool(check.get('duplication_pass'))),
                runtime_capture=str(file.resolve()) if image_hash else None,image_sha256=image_hash,
                visually_inspected=inspected,reviewer=review.get('reviewer') if inspected else None,
                reviewer_notes=review.get('notes',[]) if inspected else [],
                corrections_resolved=bool(inspected and review.get('corrections_resolved')),accepted=accepted))
        editions.append(dict(system=system,name=issue.get('name','EARTH'),page_count=len(pages),
            identity_plan_complete=bool(first and first['plan_complete']),complete=all(p['accepted'] for p in pages),pages=pages))
    flat=[p for e in editions for p in e['pages']]
    result=dict(schema=1,scope='256 galaxy editions plus retained four-page Earth tutorial paper',
        summary=dict(editions=len(editions),galaxy_editions=256,pages=len(flat),authored=sum(p['prose']['authored'] for p in flat),
            captured=sum(bool(p['image_sha256']) for p in flat),visually_inspected=sum(p['visually_inspected'] for p in flat),
            accepted_pages=sum(p['accepted'] for p in flat),complete_editions=sum(e['complete'] for e in editions)),editions=editions)
    (CONTENT/'completion-ledger.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result['summary'],indent=2))

if __name__=='__main__':main()


