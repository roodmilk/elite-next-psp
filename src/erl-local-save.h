static int erl_small(FILE *f,void *value,int bytes,int reading){
 unsigned char data[2];if(reading){if(fread(data,1,bytes,f)!=(size_t)bytes)return 0;if(bytes==1)*(uint8_t*)value=data[0];else *(uint16_t*)value=data[0]|data[1]<<8;return 1;}
 unsigned v=bytes==1?*(uint8_t*)value:*(uint16_t*)value;data[0]=v;data[1]=v>>8;return fwrite(data,1,bytes,f)==(size_t)bytes;
}
static int erl_local_stream(FILE *f,ErlLocal *a,int reading){
 if(!erl_word(f,&a->ready,reading)||!erl_word(f,&a->current,reading))return 0;
 if(!erl_word(f,&a->selection,reading)||!erl_word(f,&a->rules_pending,reading))return 0;
 if(!erl_word(f,&a->pending_count,reading)||a->pending_count>32)return 0;for(int k=0;k<32;k++){DockRoll *r=&a->pending[k];uint32_t *words[]={&r->id,&r->base,&r->quality,&r->generation,&r->seed,&r->power,&r->secondary,&r->value};for(int i=0;i<8;i++)if(!erl_word(f,words[i],reading))return 0;if(k<(int)a->pending_count&&!db_roll_valid(r))return 0;}
 RaceRules *q=&a->event_rules;uint32_t *rules[]={&q->mode,&q->format,&q->course,&q->objective,&q->pool,&q->field,&q->laps,&q->seconds,&q->required,&q->expert,&q->seed,&q->sponsor,&q->context};for(int i=0;i<13;i++)if(!erl_word(f,rules[i],reading))return 0;
 if(!erl_float(f,&q->gold,reading)||!erl_float(f,&q->silver,reading)||!erl_float(f,&q->bronze,reading))return 0;
 if(a->ready>1||a->current>255||a->selection>1000000||a->rules_pending>1||(q->pool&&!ra_rules_valid(q,4)))return 0;
 for(int k=0;k<256;k++){ErlLocalSeason *s=&a->season[k];
  uint32_t *words[]={&s->number,&s->ship_id,&s->seed,&s->history_count,&s->history_wins,&s->history_units,&s->history_xp};
  for(int i=0;i<7;i++)if(!erl_word(f,words[i],reading))return 0;
  for(int i=0;i<7;i++)if(!erl_small(f,&s->points[i],2,reading)||s->points[i]>40)return 0;
  uint8_t *bytes[]={&s->started,&s->active,&s->hub,&s->round,&s->division,&s->place};
  for(int i=0;i<6;i++)if(!erl_small(f,bytes[i],1,reading))return 0;
  if(s->started>1||s->active>1||s->hub>=HUB_COUNT||s->round>4||s->division>3||s->place>7)return 0;
  for(int i=0;i<7;i++)if(!erl_small(f,&s->wins[i],1,reading)||s->wins[i]>4)return 0;
  for(int round=0;round<4;round++)for(int i=0;i<7;i++)if(!erl_small(f,&s->results[round][i],1,reading)||s->results[round][i]>8)return 0;
  for(int i=0;i<6;i++)if(!erl_float(f,&s->trial_times[i],reading)||s->trial_times[i]<0||s->trial_times[i]>900)return 0;
  uint32_t *prize[]={&s->prize.id,&s->prize.base,&s->prize.quality,&s->prize.generation,&s->prize.seed,&s->prize.power,&s->prize.secondary,&s->prize.value,&s->prize_state};
  for(int i=0;i<9;i++)if(!erl_word(f,prize[i],reading))return 0;
  if(s->prize_state>3||(s->prize_state&&(!db_roll_valid(&s->prize)||s->prize.base<1||s->prize.base>=EQUIPMENT_COUNT||s->prize.quality!=DB_RARE||s->prize.power<1000||s->prize.power>2000||s->prize.secondary<1000||s->prize.secondary>2000)))return 0;
 }return 1;
}
