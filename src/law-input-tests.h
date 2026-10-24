{
 TEST_INIT();game.tutorial_step=0;game.opening.stage=OP_FREE;game.docked=1;game.system=7;game.legal=10;game.wanted[7]=10;game.wanted[8]=6;game.crime_record[7]=CRIME_ASSAULT;game.crime_record[8]=CRIME_ESCAPE;game.credits=10000;
 law_enter();INPUT_CHECK(law_count()==2&&law_system_at(0)==7&&law_system_at(1)==8,"law: ledger lists warrants from every issuing system");
 int before=game.credits;law_input(PSP_CTRL_CROSS);INPUT_CHECK(law_call_open&&!law_choice,"law: current jurisdiction opens a settlement call");law_input(PSP_CTRL_CROSS);
 INPUT_CHECK(!game.legal&&!game.wanted[7]&&game.wanted[8]==6&&game.credits<before,"law: payment clears only the local warrant and charges its fine");
 row=0;law_input(PSP_CTRL_CROSS);INPUT_CHECK(!law_call_open&&game.wanted[8]==6,"law: remote warrants cannot be paid from the current station");
 INPUT_CHECK(deck_service_visible(30),"law: docked Fly panel exposes the law terminal");game.docked=0;INPUT_CHECK(!deck_service_visible(30),"law: terminal is hidden while flying");TEST_INIT();
}
