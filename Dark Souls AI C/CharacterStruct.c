#include "CharacterStruct.h"
#define PI 3.14159265

#pragma warning( disable: 4244 )//ignore dataloss conversion from double to float


void PrintPhantom(Character * c){
	printf("X: %.03f Y: %.03f Rotation: %.03f Animation id: %i SubId: %u\n", c->loc_x, c->loc_y, c->rotation, c->animation_id, c->subanimation);
}

void ReadPlayer(Character * c, HANDLE * processHandle){
	HANDLE processHandle_nonPoint = *processHandle;
    //TODO read large block that contains all data, then parse in process
	//read x location
	ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->location_x_address), &(c->loc_x), 4, 0);
	//read y location
	ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->location_y_address), &(c->loc_y), 4, 0);
	//read rotation of player
	ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->rotation_address), &(c->rotation), 4, 0);
	//Player rotation is pi. 0 to pi,-pi to 0. Same as atan2
	//convert to radians, then to degrees
	c->rotation = (c->rotation + PI) * (180.0 / PI);
	//read current animation id
	ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->animation_address), &(c->animation_id), 2, 0);
    //if (c->hurtboxActive_address){
    //    printf("animation %d", c->animation_id);
    //}
	//read hp
	ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->hp_address), &(c->hp), 4, 0);
    //read stamina
    if (c->stamina_address){
        ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->stamina_address), &(c->stamina), 4, 0);
    }
	//read what weapon they currently have in right hand
	ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->r_weapon_address), &(c->r_weapon_id), 4, 0);
	//read what weapon they currently have in left hand
	ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->l_weapon_address), &(c->l_weapon_id), 4, 0);

	//read the current subanimation
	ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->subanimation_address), &(c->subanimation), 4, 0);
    //if (c->hurtboxActive_address){
    //    printf("subanimation %d ", c->subanimation);
    //}
	//read if hurtbox is active on enemy weapon
    //player doesnt use this, and wont have the address set. enemy will
    if (c->hurtboxActive_address){
        unsigned char hurtboxState;
        ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->hurtboxActive_address), &hurtboxState, 1, 0);
        //if the hurtbox is active
        if (hurtboxState){
            c->subanimation = AttackSubanimationActiveDuringHurtbox;
        }
        //printf("hurtboxState %d ", hurtboxState);
    }
    //read if windup subanimation is closing, and about to transition to hurtbox
    unsigned char windupState;
    ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->windupClose_address), &windupState, 1, 0);
    if (windupState > 47){
        c->subanimation = AttackSubanimationWindupClosing;
    }
    //if (c->hurtboxActive_address){
    //    printf("windupState %d ", windupState);
    //    printf("rsubanimation %d\n", c->subanimation);
    //}

    //read the current velocity
    //player doesnt use this, and wont have the address set. enemy will
    if (c->velocity_address){
        ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->velocity_address), &(c->velocity), 4, 0);
    }
    //read if the player is locked on
    if (c->locked_on_address){
        ReadProcessMemory(processHandle_nonPoint, (LPCVOID)(c->locked_on_address), &(c->locked_on), 1, 0);
    }
}