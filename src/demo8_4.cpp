//
// DEMOII8_4.CPP - solid demo (no lighting)
//
#include "lib/retro.h"
#include "lib/retromain.h"
#include "lib/retrofont.h"
#include "lib/t3dlib1.h"
#include "lib/t3dlib4.h"
#include "lib/t3dlib5.h"
#include "lib/t3dlib6.h"

// DEFINES ////////////////////////////////////////////////

// defines for the game universe
#define UNIVERSE_RADIUS   4000

#define POINT_SIZE        200
#define NUM_POINTS_X      (2*UNIVERSE_RADIUS/POINT_SIZE)
#define NUM_POINTS_Z      (2*UNIVERSE_RADIUS/POINT_SIZE)
#define NUM_POINTS        (NUM_POINTS_X*NUM_POINTS_Z)

// defines for objects
#define NUM_TOWERS        64 // 96
#define NUM_TANKS         32 // 24
#define TANK_SPEED        15

// GLOBALS ////////////////////////////////////////////////

char buffer[2048];                        // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,40,0,1 };
POINT4D  cam_target = { 0,0,0,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

// all your initialization code goes here...
VECTOR4D vscale = { 1.0,1.0,1.0,1 },
vpos = { 0,0,0,1 },
vrot = { 0,0,0,1 };

CAM4DV1        cam;       // the single camera

OBJECT4DV1     obj_tower,    // used to hold the master tower
obj_tank,     // used to hold the master tank
obj_marker,   // the ground marker
obj_player;   // the player object             

POINT4D        towers[NUM_TOWERS],
tanks[NUM_TANKS];

RENDERLIST4DV1 rend_list; // the render list

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static MATRIX4X4 mrot;   // general rotation matrix

	// these are used to create a circling camera
	static float tank_speed;
	static float turning = 0;

	char work_string[256]; // temp string

	int index; // looping var

	// start the timing clock
	Start_Clock();

	// draw the sky
	Draw_Rectangle16(0, 0, screen_width - 1, screen_height / 2, RGB16Bit(0, 140, 192), framebuffer, framebuffer_pitch);

	// draw the ground
	Draw_Rectangle16(0, screen_height / 2, screen_width - 1, screen_height - 1, RGB16Bit(103, 62, 3), framebuffer, framebuffer_pitch);

	// game logic here...

	// reset the render list
	Reset_RENDERLIST4DV1(&rend_list);

	// allow user to move camera

	// turbo
	if (RETRO_KeyState(SDL_SCANCODE_RETURN)) {
		tank_speed = 5 * TANK_SPEED;
	} else {
		tank_speed = TANK_SPEED;
	}

	// forward/backward
	if (RETRO_KeyState(SDL_SCANCODE_UP)) {
		// move forward
		cam.pos.x += tank_speed * Fast_Sin(cam.dir.y);
		cam.pos.z += tank_speed * Fast_Cos(cam.dir.y);
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_DOWN)) {
		// move backward
		cam.pos.x -= tank_speed * Fast_Sin(cam.dir.y);
		cam.pos.z -= tank_speed * Fast_Cos(cam.dir.y);
	} // end if

	// rotate
	if (RETRO_KeyState(SDL_SCANCODE_RIGHT)) {
		cam.dir.y += 3;

		// add a little turn to object
		if ((turning += 2) > 15)
			turning = 15;
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_LEFT)) {
		cam.dir.y -= 3;

		// add a little turn to object
		if ((turning -= 2) < -15)
			turning = -15;

	} // end if
	else // center heading again
	{
		if (turning > 0) {
			turning -= 1;
		} else if (turning < 0) {
			turning += 1;
		}
	} // end else

	// generate camera matrix
	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

	// insert the player into the world
	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV1(&obj_player);

	// set position of tank
	obj_player.world_pos.x = cam.pos.x + 300 * Fast_Sin(cam.dir.y);
	obj_player.world_pos.y = cam.pos.y - 70;
	obj_player.world_pos.z = cam.pos.z + 300 * Fast_Cos(cam.dir.y);

	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(0, cam.dir.y + turning, 0, &mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV1(&obj_player, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

	// perform world transform
	Model_To_World_OBJECT4DV1(&obj_player, TRANSFORM_TRANS_ONLY);

	// insert the object into render list
	Insert_OBJECT4DV1_RENDERLIST4DV12(&rend_list, &obj_player, 0, 0);

	//////////////////////////////////////////////////////////

	// insert the tanks in the world
	for (index = 0; index < NUM_TANKS; index++) {
		// reset the object (this only matters for backface and object removal)
		Reset_OBJECT4DV1(&obj_tank);

		// generate rotation matrix around y axis
		Build_XYZ_Rotation_MATRIX4X4(0, tanks[index].w, 0, &mrot);

		// rotate the local coords of the object
		Transform_OBJECT4DV1(&obj_tank, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

		// set position of tank
		obj_tank.world_pos.x = tanks[index].x;
		obj_tank.world_pos.y = tanks[index].y;
		obj_tank.world_pos.z = tanks[index].z;

		// attempt to cull object   
		if (!Cull_OBJECT4DV1(&obj_tank, &cam, CULL_OBJECT_XYZ_PLANES)) {
			// if we get here then the object is visible at this world position
			// so we can insert it into the rendering list
			// perform local/model to world transform
			Model_To_World_OBJECT4DV1(&obj_tank, TRANSFORM_TRANS_ONLY);

			// insert the object into render list
			Insert_OBJECT4DV1_RENDERLIST4DV12(&rend_list, &obj_tank, 0, 0);
		} // end if

	} // end for

	////////////////////////////////////////////////////////

	// insert the towers in the world
	for (index = 0; index < NUM_TOWERS; index++) {
		// reset the object (this only matters for backface and object removal)
		Reset_OBJECT4DV1(&obj_tower);

		// set position of tower
		obj_tower.world_pos.x = towers[index].x;
		obj_tower.world_pos.y = towers[index].y;
		obj_tower.world_pos.z = towers[index].z;

		// attempt to cull object   
		if (!Cull_OBJECT4DV1(&obj_tower, &cam, CULL_OBJECT_XYZ_PLANES)) {
			// if we get here then the object is visible at this world position
			// so we can insert it into the rendering list
			// perform local/model to world transform
			Model_To_World_OBJECT4DV1(&obj_tower);

			// insert the object into render list
			Insert_OBJECT4DV1_RENDERLIST4DV12(&rend_list, &obj_tower, 0, 0);

		} // end if

	} // end for

	///////////////////////////////////////////////////////////////

	// seed number generator so that modulation of markers is always the same
	srand(13);

	// insert the ground markers into the world
	for (int index_x = 0; index_x < NUM_POINTS_X; index_x++) {
		for (int index_z = 0; index_z < NUM_POINTS_Z; index_z++) {
			// reset the object (this only matters for backface and object removal)
			Reset_OBJECT4DV1(&obj_marker);

			// set position of tower
			obj_marker.world_pos.x = RAND_RANGE(-100, 100) - UNIVERSE_RADIUS + index_x * POINT_SIZE;
			obj_marker.world_pos.y = obj_marker.max_radius;
			obj_marker.world_pos.z = RAND_RANGE(-100, 100) - UNIVERSE_RADIUS + index_z * POINT_SIZE;

			// attempt to cull object   
			if (!Cull_OBJECT4DV1(&obj_marker, &cam, CULL_OBJECT_XYZ_PLANES)) {
				// if we get here then the object is visible at this world position
				// so we can insert it into the rendering list
				// perform local/model to world transform
				Model_To_World_OBJECT4DV1(&obj_marker);

				// insert the object into render list
				Insert_OBJECT4DV1_RENDERLIST4DV12(&rend_list, &obj_marker, 0, 0);

			} // end if

		} // end for
	}

	////////////////////////////////////////////////////////////////////////

	// remove backfaces
	Remove_Backfaces_RENDERLIST4DV1(&rend_list, &cam);

	// apply world to camera transform
	World_To_Camera_RENDERLIST4DV1(&rend_list, &cam);

	// apply camera to perspective transformation
	Camera_To_Perspective_RENDERLIST4DV1(&rend_list, &cam);

	// apply screen transform
	Perspective_To_Screen_RENDERLIST4DV1(&rend_list, &cam);

	sprintf(work_string, "pos:[%f, %f, %f] heading:[%f] elev:[%f], polys[%d]",
		cam.pos.x, cam.pos.y, cam.pos.z, cam.dir.y, cam.dir.x, debug_polys_rendered_per_frame);

	Draw_Text_GDI16(work_string, 0, screen_height - 20, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	// draw instructions
	Draw_Text_GDI16("Press ESC to exit. Press Arrow Keys to Move. Space for TURBO.", 0, 0, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	// reset number of polys rendered
	debug_polys_rendered_per_frame = 0;

	// render the object
	Draw_RENDERLIST4DV1_Solid16(&rend_list, framebuffer, framebuffer_pitch);

	// sync to 30ish fps
	Wait_Clock(30);
}

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init16();

	Open_Error_File("error.txt");

	// initialize math engine
	Build_Sin_Cos_Tables();

	// initialize the camera with 90 FOV, normalized coordinates
	Init_CAM4DV1(&cam,      // the camera object
		CAM_MODEL_EULER, // the euler model
		&cam_pos,  // initial camera position
		&cam_dir,  // initial camera angles
		&cam_target,      // no target
		200.0,      // near and far clipping planes
		12000.0,
		120.0,      // field of view in degrees
		screen_width,   // size of final screen viewport
		screen_height);

	// load the master tank object
	VECTOR4D_INITXYZ(&vscale, 0.75, 0.75, 0.75);
	Load_OBJECT4DV1_PLG(&obj_tank, "assets/tank3.plg", &vscale, &vpos, &vrot);

	// load player object for 3rd person view
	VECTOR4D_INITXYZ(&vscale, 0.75, 0.75, 0.75);
	Load_OBJECT4DV1_PLG(&obj_player, "assets/tank2.plg", &vscale, &vpos, &vrot);

	// load the master tower object
	VECTOR4D_INITXYZ(&vscale, 1.0, 2.0, 1.0);
	Load_OBJECT4DV1_PLG(&obj_tower, "assets/tower1.plg", &vscale, &vpos, &vrot);

	// load the master ground marker
	VECTOR4D_INITXYZ(&vscale, 3.0, 3.0, 3.0);
	Load_OBJECT4DV1_PLG(&obj_marker, "assets/marker1.plg", &vscale, &vpos, &vrot);

	// position the tanks
	for (int index = 0; index < NUM_TANKS; index++) {
		// randomly position the tanks
		tanks[index].x = RAND_RANGE(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
		tanks[index].y = 0; // obj_tank.max_radius;
		tanks[index].z = RAND_RANGE(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
		tanks[index].w = RAND_RANGE(0, 360);
	} // end for

	// position the towers
	for (int index = 0; index < NUM_TOWERS; index++) {
		// randomly position the tower
		towers[index].x = RAND_RANGE(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
		towers[index].y = 0; // obj_tower.max_radius;
		towers[index].z = RAND_RANGE(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
	} // end for
}

void DEMO_Deinitialize(void)
{
	// this function is where you shutdown your game and
	// release all resources that you allocated

	Close_Error_File();
}

//////////////////////////////////////////////////////////
