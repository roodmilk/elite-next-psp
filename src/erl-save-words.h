/* Shared finite, explicit little-endian words for ERL save extensions. */
#ifndef ERL_SAVE_WORDS_H
#define ERL_SAVE_WORDS_H
static int erl_float(FILE *f,float *v,int reading){uint32_t u;if(reading){if(!load_u32(f,&u))return 0;memcpy(v,&u,4);return isfinite(*v)&&fabsf(*v)<=1.e8f;}memcpy(&u,v,4);return save_u32(f,u);}
static int erl_word(FILE *f,void *v,int reading){uint32_t u;if(reading){if(!load_u32(f,&u))return 0;memcpy(v,&u,4);return 1;}memcpy(&u,v,4);return save_u32(f,u);}
#endif
