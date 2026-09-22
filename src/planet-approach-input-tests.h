/* Included in input_tests; uses real PSP face-button dispatch. */
{
 TEST_INIT();launch(&game);page=FLIGHT;
 int gas=-1;for(int i=1;i<BODY_COUNT;i++)if(game.bodies[i].type==GAS)gas=i;
 game.approach=gas;game.pos=add(game.bodies[gas].pos,(Vec3){0,0,-game.bodies[gas].radius-900});
 game.speed=0;game.incoming_missile=.01f;game.energy=75;
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.1f,0,0);
 INPUT_CHECK(game.approach==gas&&game.planet<0&&game.energy==75&&strstr(game.message,"Circle"),"gas boundary: X explains unavailable entry without losing safe choice");
 input(PSP_CTRL_CIRCLE,0,.016f,0,0);
 INPUT_CHECK(game.approach<0&&dot(forward(&game),norm(sub(game.pos,game.bodies[gas].pos)))>.99f,"gas boundary: Circle restores outward flight");
 TEST_INIT();launch(&game);page=FLIGHT;game.approach=1;
 game.pos=add(game.bodies[1].pos,(Vec3){0,0,-game.bodies[1].radius-900});
 input(PSP_CTRL_CROSS,PSP_CTRL_CROSS,.016f,0,0);
 INPUT_CHECK(game.planet==1&&game.surface==0&&game.approach<0,"solid boundary: X enters surface flight, not instant landing");
}
