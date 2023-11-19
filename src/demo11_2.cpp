//
// DEMOII11_2.CPP - Water simulation
//
#include "lib/retro.h"
#include "lib/retromain.h"
#include "lib/retrofont.h"
#include "lib/t3dlib1.h"
#include "lib/t3dlib4.h"
#include "lib/t3dlib5.h"
#include "lib/t3dlib6.h"
#include "lib/t3dlib7.h"
#include "lib/t3dlib8.h"
#include "lib/t3dlib9.h"

// DEFINES ////////////////////////////////////////////////

// create some constants for ease of access
#define AMBIENT_LIGHT_INDEX   0   // ambient light index
#define INFINITE_LIGHT_INDEX  1   // infinite light index
#define POINT_LIGHT_INDEX     2   // point light index

// terrain defines
#define TERRAIN_WIDTH         5000 
#define TERRAIN_HEIGHT        5000
#define TERRAIN_SCALE         700
#define MAX_SPEED             20

#define PITCH_RETURN_RATE (.50)   // how fast the pitch straightens out
#define PITCH_CHANGE_RATE (.045)  // the rate that pitch changes relative to height
#define CAM_HEIGHT_SCALER (.3)    // percentage cam height changes relative to height
#define VELOCITY_SCALER   (.03)   // rate velocity changes with height
#define CAM_DECEL         (.25)   // deceleration/friction
#define WATER_LEVEL         40    // level where wave will have effect
#define WAVE_HEIGHT         25    // amplitude of modulation
#define WAVE_RATE        (0.1f)   // how fast the wave propagates

#define MAX_JETSKI_TURN           25    // maximum turn angle
#define JETSKI_TURN_RETURN_RATE   .5    // rate of return for turn
#define JETSKI_TURN_RATE           2    // turn rate
#define JETSKI_YAW_RATE           .1    // yaw rates, return, and max
#define MAX_JETSKI_YAW            10 
#define JETSKI_YAW_RETURN_RATE    .02 

// game states
#define GAME_STATE_INIT            0
#define GAME_STATE_RESTART         1
#define GAME_STATE_RUN             2
#define GAME_STATE_INTRO           3
#define GAME_STATE_PLAY            4
#define GAME_STATE_EXIT            5

// delays for introductions
#define INTRO_STATE_COUNT1 (30*9)
#define INTRO_STATE_COUNT2 (30*11)
#define INTRO_STATE_COUNT3 (30*17)

// player/jetski states
#define JETSKI_OFF                 0 
#define JETSKI_START               1 
#define JETSKI_IDLE                2
#define JETSKI_ACCEL               3 
#define JETSKI_FAST                4

// GLOBALS ////////////////////////////////////////////////

char buffer[2048];              // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,500,-200,1 };
POINT4D  cam_target = { 0,0,0,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

// all your initialization code goes here...
VECTOR4D vscale = { 1.0,1.0,1.0,1 },
vpos = { 0,0,150,1 },
vrot = { 0,0,0,1 };

CAM4DV1         cam;           // the single camera
OBJECT4DV2      obj_terrain,   // the terrain object
obj_player;    // the player object
RENDERLIST4DV2  rend_list;     // the render list
RGBAV1 white, gray, lightgray, black, red, green, blue; // general colors

// physical model defines play with these to change the feel of the vehicle
float gravity = -.30;    // general gravity
float vel_y = 0;       // the y velocity of camera/jetski
float cam_speed = 0;       // speed of the camera/jetski
float sea_level = 30;      // sea level of the simulation
float gclearance = 75;      // clearance from the camera to the ground/water
float neutral_pitch = 1;       // the neutral pitch of the camera
float turn_ang = 0;
float jetski_yaw = 0;

float wave_count = 0;       // used to track wave propagation
int scroll_count = 0;

// ambient and in game sounds

// game imagery assets
BOB intro_image,
ready_image,
nice_one_image;

BITMAP_IMAGE background_bmp;   // holds the background

ZBUFFERV1 zbuffer;             // out little z buffer

int game_state = GAME_STATE_INIT;
int game_state_count1 = 0;

int player_state = JETSKI_OFF;
int player_state_count1 = 0;
int enable_model_view = 0;

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static MATRIX4X4 mrot;   // general rotation matrix

	// state variables for different rendering modes and help
	static int wireframe_mode = 1;
	static int backface_mode = 1;
	static int lighting_mode = 1;
	static int help_mode = -1;
	static int zsort_mode = -1;
	static int x_clip_mode = 1;
	static int y_clip_mode = 1;
	static int z_clip_mode = 1;
	static int z_buffer_mode = 1;
	static int display_mode = 1;
	char work_string[256]; // temp string

	static int nice_one_on = 0; // used to display the nice one text
	static int nice_count1 = 0;

	// what state is the game in?
	switch (game_state) {

	case GAME_STATE_INIT:
	{
		// perform any important initializations

		// transition to restart
		game_state = GAME_STATE_RESTART;

	} break;

	case GAME_STATE_RESTART:
	{
		// reset all variables
		game_state_count1 = 0;
		player_state = JETSKI_OFF;
		player_state_count1 = 0;
		gravity = -.30;
		vel_y = 0;
		cam_speed = 0;
		sea_level = 30;
		gclearance = 75;
		neutral_pitch = 10;
		turn_ang = 0;
		jetski_yaw = 0;
		wave_count = 0;
		scroll_count = 0;
		enable_model_view = 0;

		// set camera high atop mount aleph one
		cam.pos.x = 1550;
		cam.pos.y = 800;
		cam.pos.z = 1950;
		cam.dir.y = -140;

		// turn on water sounds
		// start the sounds

		// transition to introduction sub-state of run
		game_state = GAME_STATE_INTRO;

	} break;

	// in any of these cases enter into the main simulation loop
	case GAME_STATE_RUN:
	case GAME_STATE_INTRO:
	case GAME_STATE_PLAY:
	{

		// perform sub-state transition logic here
		if (game_state == GAME_STATE_INTRO) {
			// update timer
			++game_state_count1;

			// test for first part of intro
			if (game_state_count1 == INTRO_STATE_COUNT1) {
				// change amplitude of water

				// reposition camera in water
				cam.pos.x = 444; // 560;
				cam.pos.y = 200;
				cam.pos.z = -534; // -580;
				cam.dir.y = 45;// (-100);  

				// enable model
				enable_model_view = 1;
			} // end if
			else
				if (game_state_count1 == INTRO_STATE_COUNT2) {
					// change amplitude of water
				} // end if
				else
					if (game_state_count1 == INTRO_STATE_COUNT3) {
						// reset counter for other use
						game_state_count1 = 0;

						// change amplitude of water

						// transition to play state
						game_state = GAME_STATE_PLAY;

					} // end if

		} // end if

		// start the timing clock
		Start_Clock();

		// clear the drawing surface 
		//DDraw_Fill_Surface(framebuffer, framebuffer_pitch, 0);

		// draw the sky
		//Draw_Rectangle16(0, 0, screen_width, screen_height * .38, RGB16Bit(50, 100, 255), framebuffer, framebuffer_pitch);

		// draw the ground
		//Draw_Rectangle16(0, screen_height * .38, screen_width, screen_height, RGB16Bit(25, 50, 110), framebuffer, framebuffer_pitch);

		//Draw_BOB16(&background, framebuffer, framebuffer_pitch);

		// reset the render list
		Reset_RENDERLIST4DV2(&rend_list);

		// modes and lights

		// wireframe mode
		if (RETRO_KeyPressed(SDL_SCANCODE_W)) {
			// toggle wireframe mode
			if (++wireframe_mode > 1)
				wireframe_mode = 0;
		} // end if

		// backface removal
		if (RETRO_KeyPressed(SDL_SCANCODE_B)) {
			// toggle backface removal
			backface_mode = -backface_mode;
		} // end if

		// lighting
		if (RETRO_KeyPressed(SDL_SCANCODE_L)) {
			// toggle lighting engine completely
			lighting_mode = -lighting_mode;
		} // end if

		// toggle ambient light
		if (RETRO_KeyPressed(SDL_SCANCODE_A)) {
			// toggle ambient light
			if (lights2[AMBIENT_LIGHT_INDEX].state == LIGHTV2_STATE_ON)
				lights2[AMBIENT_LIGHT_INDEX].state = LIGHTV2_STATE_OFF;
			else
				lights2[AMBIENT_LIGHT_INDEX].state = LIGHTV2_STATE_ON;
		} // end if

		// toggle infinite light
		if (RETRO_KeyPressed(SDL_SCANCODE_I)) {
			// toggle ambient light
			if (lights2[INFINITE_LIGHT_INDEX].state == LIGHTV2_STATE_ON)
				lights2[INFINITE_LIGHT_INDEX].state = LIGHTV2_STATE_OFF;
			else
				lights2[INFINITE_LIGHT_INDEX].state = LIGHTV2_STATE_ON;
		} // end if

		// toggle point light
		if (RETRO_KeyPressed(SDL_SCANCODE_P)) {
			// toggle point light
			if (lights2[POINT_LIGHT_INDEX].state == LIGHTV2_STATE_ON)
				lights2[POINT_LIGHT_INDEX].state = LIGHTV2_STATE_OFF;
			else
				lights2[POINT_LIGHT_INDEX].state = LIGHTV2_STATE_ON;
		} // end if

		// help menu
		if (RETRO_KeyPressed(SDL_SCANCODE_H)) {
			// toggle help menu 
			help_mode = -help_mode;
		} // end if

		// z-sorting
		if (RETRO_KeyPressed(SDL_SCANCODE_S)) {
			// toggle z sorting
			zsort_mode = -zsort_mode;
		} // end if

		// z buffer
		if (RETRO_KeyPressed(SDL_SCANCODE_Z)) {
			// toggle z buffer
			z_buffer_mode = -z_buffer_mode;
		} // end if

		// display mode
		if (RETRO_KeyPressed(SDL_SCANCODE_D)) {
			// toggle display mode
			display_mode = -display_mode;
		} // end if

		// PLAYER CONTROL AREA ////////////////////////////////////////////////////////////

		// filter player control if not in PLAY state
		if (game_state == GAME_STATE_PLAY) {
			// forward/backward
			if (RETRO_KeyState(SDL_SCANCODE_UP) && player_state >= JETSKI_START) {
				// test for acceleration 
				if (cam_speed == 0) {

				} // end if

				// move forward
				if ((cam_speed += 1) > MAX_SPEED)
					cam_speed = MAX_SPEED;

			} // end if

		 /*
		 else
		 if (RETRO_KeyPressed(SDL_SCANCODE_DOWN))
			{
			// move backward
			if ((cam_speed-=1) < -MAX_SPEED) cam_speed = -MAX_SPEED;
			} // end if
		 */

			// rotate around y axis or yaw
			if (RETRO_KeyState(SDL_SCANCODE_RIGHT)) {
				cam.dir.y += 5;

				if ((turn_ang += JETSKI_TURN_RATE) > MAX_JETSKI_TURN)
					turn_ang = MAX_JETSKI_TURN;

				if ((jetski_yaw -= (JETSKI_YAW_RATE * cam_speed)) < -MAX_JETSKI_YAW)
					jetski_yaw = -MAX_JETSKI_YAW;

				// scroll the background
				Scroll_Bitmap(&background_bmp, -10);

			} // end if

			if (RETRO_KeyState(SDL_SCANCODE_LEFT)) {
				cam.dir.y -= 5;

				if ((turn_ang -= JETSKI_TURN_RATE) < -MAX_JETSKI_TURN)
					turn_ang = -MAX_JETSKI_TURN;

				if ((jetski_yaw += (JETSKI_YAW_RATE * cam_speed)) > MAX_JETSKI_YAW)
					jetski_yaw = MAX_JETSKI_YAW;

				// scroll the background
				Scroll_Bitmap(&background_bmp, 10);

			} // end if

			// is player trying to start jetski?
			if ((player_state == JETSKI_OFF) && RETRO_KeyPressed(SDL_SCANCODE_RETURN)) {
				// start jetski
				player_state = JETSKI_START;

			} // end if 

		} // end if controls enabled

		// turn JETSKI straight
		if (turn_ang > JETSKI_TURN_RETURN_RATE) turn_ang -= JETSKI_TURN_RETURN_RATE;
		else
			if (turn_ang < -JETSKI_TURN_RETURN_RATE) turn_ang += JETSKI_TURN_RETURN_RATE;
			else
				turn_ang = 0;

		// turn JETSKI straight
		if (jetski_yaw > JETSKI_YAW_RETURN_RATE) jetski_yaw -= JETSKI_YAW_RETURN_RATE;
		else
			if (jetski_yaw < -JETSKI_YAW_RETURN_RATE) jetski_yaw += JETSKI_YAW_RETURN_RATE;
			else
				jetski_yaw = 0;

		// reset the object (this only matters for backface and object removal)
		Reset_OBJECT4DV2(&obj_terrain);

		// perform world transform to terrain now, so we can destroy the transformed
		// coordinates with the wave function
		Model_To_World_OBJECT4DV2(&obj_terrain, TRANSFORM_LOCAL_TO_TRANS);

		// apply wind effect ////////////////////////////////////////////////////

		// scroll the background
		if (++scroll_count > 5) {
			Scroll_Bitmap(&background_bmp, -1);
			scroll_count = 0;
		} // end if

		// wave generator ////////////////////////////////////////////////////////

		// for each vertex in the mesh apply wave modulation if height < water level
		for (int v = 0; v < obj_terrain.num_vertices; v++) {
			// wave modulate below water level only
			if (obj_terrain.vlist_trans[v].y < WATER_LEVEL)
				obj_terrain.vlist_trans[v].y += WAVE_HEIGHT * sin(wave_count + (float)v / (2 * (float)obj_terrain.ivar2 + 0));
		} // end for v

		// increase the wave position in time
		wave_count += WAVE_RATE;

		// motion section /////////////////////////////////////////////////////////

		// terrain following, simply find the current cell we are over and then
		// index into the vertex list and find the 4 vertices that make up the 
		// quad cell we are hovering over and then average the values, and based
		// on the current height and the height of the terrain push the player upward

		// the terrain generates and stores some results to help with terrain following
		//ivar1 = columns;
		//ivar2 = rows;
		//fvar1 = col_vstep;
		//fvar2 = row_vstep;

		int cell_x = (cam.pos.x + TERRAIN_WIDTH / 2) / obj_terrain.fvar1;
		int cell_y = (cam.pos.z + TERRAIN_HEIGHT / 2) / obj_terrain.fvar1;

		static float terrain_height, delta;

		// test if we are on terrain
		if ((cell_x >= 0) && (cell_x < obj_terrain.ivar1) && (cell_y >= 0) && (cell_y < obj_terrain.ivar2)) {
			// compute vertex indices into vertex list of the current quad
			int v0 = cell_x + cell_y * obj_terrain.ivar2;
			int v1 = v0 + 1;
			int v2 = v1 + obj_terrain.ivar2;
			int v3 = v0 + obj_terrain.ivar2;

			// now simply index into table 
			terrain_height = 0.25 * (obj_terrain.vlist_trans[v0].y + obj_terrain.vlist_trans[v1].y +
				obj_terrain.vlist_trans[v2].y + obj_terrain.vlist_trans[v3].y);

			// compute height difference
			delta = terrain_height - (cam.pos.y - gclearance);

			// test for penetration
			if (delta > 0) {
				// apply force immediately to camera (this will give it a springy feel)
				vel_y += (delta * (VELOCITY_SCALER));

				// test for pentration, if so move up immediately so we don't penetrate geometry
				cam.pos.y += (delta * CAM_HEIGHT_SCALER);

				// now this is more of a hack than the physics model :) let move the front
				// up and down a bit based on the forward velocity and the gradient of the 
				// hill
				cam.dir.x -= (delta * PITCH_CHANGE_RATE);

			} // end if

		} // end if

		// decelerate camera
		if (cam_speed > (CAM_DECEL)) cam_speed -= CAM_DECEL;
		else
			if (cam_speed < (-CAM_DECEL)) cam_speed += CAM_DECEL;
			else
				cam_speed = 0;

		// force camera to seek a stable orientation
		if (cam.dir.x > (neutral_pitch + PITCH_RETURN_RATE)) cam.dir.x -= (PITCH_RETURN_RATE);
		else
			if (cam.dir.x < (neutral_pitch - PITCH_RETURN_RATE)) cam.dir.x += (PITCH_RETURN_RATE);
			else
				cam.dir.x = neutral_pitch;

		// apply gravity
		vel_y += gravity;

		// test for absolute sea level and push upward..
		if (cam.pos.y < sea_level) {
			vel_y = 0;
			cam.pos.y = sea_level;
		} // end if

		// move camera
		cam.pos.x += cam_speed * Fast_Sin(cam.dir.y);
		cam.pos.z += cam_speed * Fast_Cos(cam.dir.y);
		cam.pos.y += vel_y;

		// position point light in front of player
		lights2[POINT_LIGHT_INDEX].pos.x = cam.pos.x + 130 * Fast_Sin(cam.dir.y);
		lights2[POINT_LIGHT_INDEX].pos.y = cam.pos.y + 50;
		lights2[POINT_LIGHT_INDEX].pos.z = cam.pos.z + 130 * Fast_Cos(cam.dir.y);

		// position the player object slighty in front of camera and in water
		obj_player.world_pos.x = cam.pos.x + (130 + cam_speed * 1.75) * Fast_Sin(cam.dir.y);
		obj_player.world_pos.y = cam.pos.y - 25 + 7.5 * sin(wave_count);
		obj_player.world_pos.z = cam.pos.z + (130 + cam_speed * 1.75) * Fast_Cos(cam.dir.y);

		// sound effect section ////////////////////////////////////////////////////////////

		// make engine sound if player is pressing gas

		// make splash sound if altitude changed enough
		if ((fabs(delta) > 30) && (cam_speed >= (.75 * MAX_SPEED)) && ((rand() % 20) == 1)) {
			// play sound

			// display nice one!
			nice_one_on = 1;
			nice_count1 = 60;
		} // end if 

		// begin 3D rendering section ///////////////////////////////////////////////////////

		// generate camera matrix
		Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

		// generate rotation matrix around y axis
		//Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

		// rotate the local coords of the object
		//Transform_OBJECT4DV2(&obj_terrain, &mrot, TRANSFORM_LOCAL_TO_TRANS,1);


		// insert terrain object into render list
		Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, &obj_terrain, 0);

#if 1
		//MAT_IDENTITY_4X4(&mrot);

		// generate rotation matrix around y axis
		Build_XYZ_Rotation_MATRIX4X4(-cam.dir.x * 1.5, cam.dir.y + turn_ang, jetski_yaw, &mrot);

		// rotate the local coords of the object
		Transform_OBJECT4DV2(&obj_player, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

		// perform world transform
		Model_To_World_OBJECT4DV2(&obj_player, TRANSFORM_TRANS_ONLY);

		// don't show model unless in play state
		if (enable_model_view) {
			// insert the object into render list
			Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, &obj_player, 0);
		} // end if

#endif

		// reset number of polys rendered
		debug_polys_rendered_per_frame = 0;
		debug_polys_lit_per_frame = 0;

		// remove backfaces
		if (backface_mode == 1)
			Remove_Backfaces_RENDERLIST4DV2(&rend_list, &cam);

		// apply world to camera transform
		World_To_Camera_RENDERLIST4DV2(&rend_list, &cam);

		// clip the polygons themselves now
		Clip_Polys_RENDERLIST4DV2(&rend_list, &cam, ((x_clip_mode == 1) ? CLIP_POLY_X_PLANE : 0) |
			((y_clip_mode == 1) ? CLIP_POLY_Y_PLANE : 0) |
			((z_clip_mode == 1) ? CLIP_POLY_Z_PLANE : 0));

		// light scene all at once 
		if (lighting_mode == 1) {
			Transform_LIGHTSV2(lights2, 3, &cam.mcam, TRANSFORM_LOCAL_TO_TRANS);
			Light_RENDERLIST4DV2_World2_16(&rend_list, &cam, lights2, 3);
		} // end if

		// sort the polygon list (hurry up!)
		if (zsort_mode == 1)
			Sort_RENDERLIST4DV2(&rend_list, SORT_POLYLIST_AVGZ);

		// apply camera to perspective transformation
		Camera_To_Perspective_RENDERLIST4DV2(&rend_list, &cam);

		// apply screen transform
		Perspective_To_Screen_RENDERLIST4DV2(&rend_list, &cam);

		// draw background
//		Draw_Bitmap16(&background_bmp, framebuffer, framebuffer_pitch, 0);
		RECT src_rect = { 0, 0, background_bmp.width, background_bmp.height }, // the source rectangle
			dest_rect = { 0, 0, screen_width, screen_height }; // the destination rectangle

		Blit_Rect16(src_rect, background_bmp.buffer, background_bmp.width * 2, dest_rect, framebuffer, framebuffer_pitch);

		// render the object

		if (wireframe_mode == 0) {
			Clear_Zbuffer(&zbuffer, (32000 << FIXP16_SHIFT));
			Draw_RENDERLIST4DV2_SolidZB16(&rend_list, framebuffer, framebuffer_pitch, (UCHAR *)zbuffer.zbuffer, screen_width * 4);

			Draw_RENDERLIST4DV2_Wire16(&rend_list, framebuffer, framebuffer_pitch);

		} else
			if (wireframe_mode == 1) {
				if (z_buffer_mode == 1) {
					// initialize zbuffer to 32000 fixed point
					//Mem_Set_QUAD((void *)zbuffer, (32000 << FIXP16_SHIFT), (screen_width*screen_height) ); 
					Clear_Zbuffer(&zbuffer, (32000 << FIXP16_SHIFT));
					Draw_RENDERLIST4DV2_SolidZB16(&rend_list, framebuffer, framebuffer_pitch, (UCHAR *)zbuffer.zbuffer, screen_width * 4);
				} else {
					// initialize zbuffer to 32000 fixed point
					Draw_RENDERLIST4DV2_Solid16(&rend_list, framebuffer, framebuffer_pitch);
				}

			} // end if

		// dislay z buffer graphically (sorta)
		if (display_mode == -1) {
			// use z buffer visualization mode
			// copy each line of the z buffer into the back buffer and translate each z pixel
			// into a color
			USHORT *screen_ptr = (USHORT *)framebuffer;
			UINT *zb_ptr = (UINT *)zbuffer.zbuffer;

			for (int y = 0; y < screen_height; y++) {
				for (int x = 0; x < screen_width; x++) {
					// z buffer is 32 bit, so be carefull
					UINT zpixel = zb_ptr[x + y * screen_width];
					zpixel = (zpixel / 4096 & 0xffff);
					screen_ptr[x + y * (framebuffer_pitch >> 1)] = (USHORT)zpixel;
				} // end for
			} // end for y

		} // end if

		// draw overlay text
		if (game_state == GAME_STATE_INTRO && game_state_count1 < INTRO_STATE_COUNT1) {
			Draw_BOB16(&intro_image, framebuffer, framebuffer_pitch);
		} // end if
		else
			if (game_state == GAME_STATE_INTRO && game_state_count1 > INTRO_STATE_COUNT2) {
				Draw_BOB16(&ready_image, framebuffer, framebuffer_pitch);
			} // end if

		// check for nice one display
		if (nice_one_on == 1 && game_state == GAME_STATE_PLAY) {
			// are we done displaying?
			if (--nice_count1 <= 0)
				nice_one_on = 0;

			Draw_BOB16(&nice_one_image, framebuffer, framebuffer_pitch);
		} // end if draw nice one

		// draw statistics 
		sprintf(work_string, "Lighting [%s]: Ambient=%d, Infinite=%d, Point=%d, BckFceRM [%s], Zsort [%s], Zbuffer [%s]",
			((lighting_mode == 1) ? "ON" : "OFF"),
			lights[AMBIENT_LIGHT_INDEX].state,
			lights[INFINITE_LIGHT_INDEX].state,
			lights[POINT_LIGHT_INDEX].state,
			((backface_mode == 1) ? "ON" : "OFF"),
			((zsort_mode == 1) ? "ON" : "OFF"),
			((z_buffer_mode == 1) ? "ON" : "OFF"));

		Draw_Text_GDI16(work_string, 0, screen_height - 34 - 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

		// draw instructions
		Draw_Text_GDI16("Press ESC to exit. Press <H> for Help.", 0, 0, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

		// should we display help
		int text_y = 16;
		if (help_mode == 1) {
			// draw help menu
			Draw_Text_GDI16("<A>..............Toggle ambient light source.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<I>..............Toggle infinite light source.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<P>..............Toggle point light source.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<N>..............Next object.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<W>..............Toggle wire frame/solid mode.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<B>..............Toggle backface removal.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<S>..............Toggle Z sorting.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<Z>..............Toggle Z buffering.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<D>..............Toggle Normal 3D display / Z buffer visualization mode.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<H>..............Toggle Help.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
			Draw_Text_GDI16("<ESC>............Exit demo.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

		} // end help

		sprintf(work_string, "Polys Rendered: %d, Polys lit: %d", debug_polys_rendered_per_frame, debug_polys_lit_per_frame);
		Draw_Text_GDI16(work_string, 0, screen_height - 34 - 16 - 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

		sprintf(work_string, "CAM [%5.2f, %5.2f, %5.2f @ %5.2f]", cam.pos.x, cam.pos.y, cam.pos.z, cam.dir.y);
		Draw_Text_GDI16(work_string, 0, screen_height - 34 - 16 - 16 - 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

		// sync to 30ish fps
		Wait_Clock(30);

		// end main simulation loop
	} break;

	case GAME_STATE_EXIT:
	{
		// do any cleanup here, and exit

	} break;

	default: break;

	} // end switch game_state
}

///////////////////////////////////////////////////////////

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init16(true);

	// seed random number generator
	srand(Start_Clock());

	Open_Error_File("error.txt");

	// initialize math engine
	Build_Sin_Cos_Tables();

	// initialize the camera with 90 FOV, normalized coordinates
	Init_CAM4DV1(&cam,      // the camera object
		CAM_MODEL_EULER, // the euler model
		&cam_pos,  // initial camera position
		&cam_dir,  // initial camera angles
		&cam_target,      // no target
		10.0,        // near and far clipping planes
		12000.0,
		90.0,      // field of view in degrees
		screen_width,   // size of final screen viewport
		screen_height);

	// position the terrain at 0,0,0
	VECTOR4D terrain_pos = { 0,0,0,0 };

	// generate terrain
	Generate_Terrain_OBJECT4DV2(&obj_terrain,            // pointer to object
		TERRAIN_WIDTH,           // width in world coords on x-axis
		TERRAIN_HEIGHT,          // height (length) in world coords on z-axis
		TERRAIN_SCALE,           // vertical scale of terrain
		"assets/water_track_height_04.bmp",  // filename of height bitmap encoded in 256 colors
		"assets/water_track_color_03.bmp",   // filename of texture map
		RGB16Bit(255, 255, 255),  // color of terrain if no texture        
		&terrain_pos,           // initial position
		NULL,                   // initial rotations
		POLY4DV2_ATTR_RGB16 |
		POLY4DV2_ATTR_SHADE_MODE_FLAT | /*POLY4DV2_ATTR_SHADE_MODE_GOURAUD */
		POLY4DV2_ATTR_SHADE_MODE_TEXTURE);

	// we are getting divide by zero errors due to degenerate triangles
	// after projection, this is a problem with the rasterizer that we will
	// fix later, but if we add a random bias to all y components of each vertice
	// is fixes the problem, its a hack, but hey no one is perfect :)
	for (int v = 0; v < obj_terrain.num_vertices; v++)
		obj_terrain.vlist_local[v].y += ((float)RAND_RANGE(-5, 5)) / 10;

	// load the jetski model for player
	VECTOR4D_INITXYZ(&vscale, 17.5, 17.50, 17.50);

	Load_OBJECT4DV2_COB(&obj_player, "assets/jetski05.cob",
		&vscale, &vpos, &vrot, VERTEX_FLAGS_SWAP_YZ |
		VERTEX_FLAGS_TRANSFORM_LOCAL | VERTEX_FLAGS_INVERT_TEXTURE_V
	/* VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD*/);

	// set up lights
	Reset_Lights_LIGHTV2(lights2, MAX_LIGHTS);

	// create some working colors
	white.rgba = _RGBA32BIT(255, 255, 255, 0);
	lightgray.rgba = _RGBA32BIT(200, 200, 200, 0);
	gray.rgba = _RGBA32BIT(100, 100, 100, 0);
	black.rgba = _RGBA32BIT(0, 0, 0, 0);
	red.rgba = _RGBA32BIT(255, 0, 0, 0);
	green.rgba = _RGBA32BIT(0, 255, 0, 0);
	blue.rgba = _RGBA32BIT(0, 0, 255, 0);

	// ambient light
	Init_Light_LIGHTV2(lights2,               // array of lights to work with
		AMBIENT_LIGHT_INDEX,
		LIGHTV2_STATE_ON,      // turn the light on
		LIGHTV2_ATTR_AMBIENT,  // ambient light type
		gray, black, black,    // color for ambient term only
		NULL, NULL,            // no need for pos or dir
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D dlight_dir = { -1,1,0,1 };

	// directional light
	Init_Light_LIGHTV2(lights2,               // array of lights to work with
		INFINITE_LIGHT_INDEX,
		LIGHTV2_STATE_ON,      // turn the light on
		LIGHTV2_ATTR_INFINITE, // infinite light type
		black, lightgray, black,    // color for diffuse term only
		NULL, &dlight_dir,     // need direction only
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D plight_pos = { 0,200,0,1 };

	// point light
	Init_Light_LIGHTV2(lights2,               // array of lights to work with
		POINT_LIGHT_INDEX,
		LIGHTV2_STATE_ON,      // turn the light on
		LIGHTV2_ATTR_POINT,    // pointlight type
		black, gray, black,    // color for diffuse term only
		&plight_pos, NULL,     // need pos only
		0, .002, 0,              // linear attenuation only
		0, 0, 1);                // spotlight info NA

	// create lookup for lighting engine
	RGB_16_8_IndexedRGB_Table_Builder(DD_PIXEL_FORMAT565,  // format we want to build table for
		palette,             // source palette
		rgblookup);          // lookup table

	// load background image
	Load_Bitmap_File(&bitmap16bit, "assets/cloud03.bmp");
	Create_Bitmap(&background_bmp, 0, 0, 800, 600, 16);
	Load_Image_Bitmap16(&background_bmp, &bitmap16bit, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);

	// load in all the text images

	// intro
	Load_Bitmap_File(&bitmap16bit, "assets/zbufferwr_intro01.bmp");
	Create_BOB(&intro_image, screen_width / 2 - 560 / 2, 40, 560, 160, 1, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);
	Load_Frame_BOB16(&intro_image, &bitmap16bit, 0, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);

	// get ready
	Load_Bitmap_File(&bitmap16bit, "assets/zbufferwr_ready01.bmp");
	Create_BOB(&ready_image, screen_width / 2 - 596 / 2, 40, 596, 244, 1, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);
	Load_Frame_BOB16(&ready_image, &bitmap16bit, 0, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);

	// nice one
	Load_Bitmap_File(&bitmap16bit, "assets/zbufferwr_nice01.bmp");
	Create_BOB(&nice_one_image, screen_width / 2 - 588 / 2, 40, 588, 92, 1, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);
	Load_Frame_BOB16(&nice_one_image, &bitmap16bit, 0, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);

	// allocate memory for zbuffer
	Create_Zbuffer(&zbuffer,
		screen_width,
		screen_height,
		ZBUFFER_ATTR_32BIT);
}

///////////////////////////////////////////////////////////

void DEMO_Deinitialize(void)
{
	// this function is where you shutdown your game and
	// release all resources that you allocated

	Close_Error_File();

	Delete_Zbuffer(&zbuffer);
}

//////////////////////////////////////////////////////////