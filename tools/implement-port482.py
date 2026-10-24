from pathlib import Path
import zipfile
files=['src/station-departure.h','src/station-departure-view.h','src/departure-hud-review.h','src/station-departure-tests.h','src/station-departure-review.h','src/main.c']
with zipfile.ZipFile('design/port482/baseline481.zip','w',zipfile.ZIP_DEFLATED) as z:
 for f in files:z.write(f,f)
p=Path('src/station-departure.h');s=p.read_text().replace('/* Five-second first-person launch.','/* Eight-second first-person launch.').replace('g->dock_duration=5;','g->dock_duration=8;')
a=s.index(' float previous=g->dock_timer;');b=s.index(' g->boost=0;',a)
s=s[:a]+''' float previous=g->dock_timer;g->dock_timer=fminf(8,g->dock_timer+dt);
 /* Slow the interesting port section, preserving the original axial endpoint
    and 40/100 m/s endpoint velocities. This is a time change, not a teleport. */
 float u=g->dock_timer/8,t=5*u+.4774648293f*sinf(6.283185307f*u),rate=.625f+.375f*cosf(6.283185307f*u),distance,velocity;
 if(previous<3&&g->dock_timer>=3)g->cue=SFX_BOOST;
 if(t<2){velocity=40+65*t*t;distance=40*t+(65.f/3)*t*t*t;}
 else if(t<3){float v=t-2;velocity=300+600*v;distance=253.333333f+300*v+300*v*v;}
 else {float v=(t-3)*.5f;velocity=900-800*v*v*(3-2*v);distance=853.333333f+1800*v-1600*(v*v*v-.5f*v*v*v*v);}
 float travelled=distance-110,half=station_half_for(g,g->station_variant),span=fmaxf(200,1743.333333f-half-100),bend=fmaxf(0,fminf(1,(travelled-half-100)/span));
 float amplitude=(g->station_variant?30:station_class(g)==STATION_MEGA?100:70)*(g->system&1?1:-1),offset=amplitude*bend*bend*(3-2*bend),slope=amplitude*6*bend*(1-bend)/span;
 Vec3 right=norm((Vec3){g->dock_to.z,0,-g->dock_to.x}),direction=norm(add(g->dock_to,mul(right,slope)));
 g->pos=add(add(g->dock_from,mul(g->dock_to,travelled)),mul(right,offset));g->yaw=atan2f(direction.x,direction.z);g->pitch=asinf(fmaxf(-1,fminf(1,direction.y)));g->roll=-.035f*sinf(3.141592654f*bend)*(amplitude>0?1:-1);
 g->speed=velocity*rate*sqrtf(1+slope*slope);
'''+s[b:];s=s.replace('if(t>=5){','if(g->dock_timer>=8){');p.write_text(s)
