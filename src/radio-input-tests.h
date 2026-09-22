{
 TEST_INIT();change_page(HOME);row=16;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(page==RADIO,"radio: visible Radio menu opens tuning screen");
 row=2;input(PSP_CTRL_CROSS,0,.016f,0,0);
 INPUT_CHECK(radio_station==2,"radio: X tunes selected station");
 row=RADIO_STATION_COUNT;radio_volume=5;int effects=sound_volume;
 input(PSP_CTRL_LEFT,0,.016f,0,0);
 INPUT_CHECK(radio_volume==4&&sound_volume==effects,"radio: music volume leaves effects unchanged");
 input(PSP_CTRL_TRIANGLE,0,.016f,0,0);
 INPUT_CHECK(radio_volume==0&&sound_volume==effects,"radio: Triangle mutes music without muting alerts");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(page==HOME&&!radio_dirty,"radio: returning saves preferences");
 remove("radio.cfg");remove("radio.cfg.bak");remove("radio.cfg.tmp");
 radio_station=0;radio_volume=5;sound_volume=8;radio_dirty=0;
}
