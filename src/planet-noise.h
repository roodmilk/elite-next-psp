#ifndef PLANET_NOISE_H
#define PLANET_NOISE_H

/* PSP-safe surface noise adapter.
 *
 * This deliberately uses one bounded integer hash per sample rather than a
 * heap-backed octave/noise object. It is cheap enough for the shared terrain
 * and collision path, deterministic across revisits, and can later be
 * replaced behind this interface by a validated offline-baked field. The
 * current arithmetic is kept identical to the established surface contract.
 */
static unsigned planet_noise_hash(unsigned x){
 x^=x>>16;x*=0x7feb352du;x^=x>>15;x*=0x846ca68bu;return x^(x>>16);
}
static float planet_noise_sample(unsigned seed,float x,float z){
 unsigned u=planet_noise_hash(seed^(unsigned)(x*3.1f)^(unsigned)(z*5.7f));
 return ((u%1000)/1000.f)*2.f-1.f;
}
static int planet_noise_bounded(const float n){return n>=-1.f&&n<1.f;}
#endif
