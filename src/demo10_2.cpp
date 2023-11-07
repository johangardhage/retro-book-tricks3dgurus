//
// DEMOII10_2.CPP - Terrain simualtion with clipping 
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

// DEFINES ////////////////////////////////////////////////

// create some constants for ease of access
#define AMBIENT_LIGHT_INDEX   0 // ambient light index
#define INFINITE_LIGHT_INDEX  1 // infinite light index
#define POINT_LIGHT_INDEX     2 // point light index
#define SPOT_LIGHT1_INDEX     4 // point light index
#define SPOT_LIGHT2_INDEX     3 // spot light index

// terrain defines
#define TERRAIN_WIDTH         4000
#define TERRAIN_HEIGHT        4000
#define TERRAIN_SCALE         700
#define MAX_SPEED             20

#define PITCH_RETURN_RATE (.33) // how fast the pitch straightens out
#define PITCH_CHANGE_RATE (.02) // the rate that pitch changes relative to height
#define CAM_HEIGHT_SCALER (.3)  // percentage cam height changes relative to height
#define VELOCITY_SCALER (.025)  // rate velocity changes with height
#define CAM_DECEL       (.25)   // deceleration/friction

// GLOBALS ////////////////////////////////////////////////

char buffer[2048];                        // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,500,-200,1 };
POINT4D  cam_target = { 0,0,0,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

// all your initialization code goes here...
VECTOR4D vscale = { 1.0,1.0,1.0,1 },
vpos = { 0,0,150,1 },
vrot = { 0,0,0,1 };

CAM4DV1         cam;            // the single camera
OBJECT4DV2      obj_terrain;    // the terrain object
RENDERLIST4DV2  rend_list;      // the render list
RGBAV1 white, gray, black, red, green, blue; // general colors

// physical model defines play with these to change the feel of the vehicle
float gravity = -.40;    // general gravity
float vel_y = 0;       // the y velocity of camera/jeep
float cam_speed = 0;       // speed of the camera/jeep
float sea_level = 50;      // sea level of the simulation
float gclearance = 75;      // clearance from the camera to the ground
float neutral_pitch = 10;   // the neutral pitch of the camera

// sounds
int wind_sound_id = -1, car_sound_id = -1;

// game imagery assets
BOB cockpit;               // the cockpit image

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static MATRIX4X4 mrot;   // general rotation matrix

	static float plight_ang = 0,
		slight_ang = 0; // angles for light motion

	// state variables for different rendering modes and help
	static int wireframe_mode = 1;
	static int backface_mode = 1;
	static int lighting_mode = 1;
	static int help_mode = 1;
	static int zsort_mode = 1;
	static int x_clip_mode = 1;
	static int y_clip_mode = 1;
	static int z_clip_mode = 1;

	char work_string[256]; // temp string

	// start the timing clock
	Start_Clock();

	// draw the sky
	Draw_Rectangle16(0, 0, screen_width, screen_height * .38, RGB16Bit(50, 100, 255), framebuffer, framebuffer_pitch);

	// draw the ground
	Draw_Rectangle16(0, screen_height * .38, screen_width, screen_height, RGB16Bit(25, 50, 110), framebuffer, framebuffer_pitch);

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

	// toggle spot light
	if (RETRO_KeyPressed(SDL_SCANCODE_S)) {
		// toggle spot light
		if (lights2[SPOT_LIGHT2_INDEX].state == LIGHTV2_STATE_ON)
			lights2[SPOT_LIGHT2_INDEX].state = LIGHTV2_STATE_OFF;
		else
			lights2[SPOT_LIGHT2_INDEX].state = LIGHTV2_STATE_ON;
	} // end if

	// help menu
	if (RETRO_KeyPressed(SDL_SCANCODE_H)) {
		// toggle help menu 
		help_mode = -help_mode;
	} // end if

	// z-sorting
	if (RETRO_KeyPressed(SDL_SCANCODE_Z)) {
		// toggle z sorting
		zsort_mode = -zsort_mode;
	} // end if

	// clipping
	if (RETRO_KeyPressed(SDL_SCANCODE_X)) {
		// toggle x clipping
		x_clip_mode = -x_clip_mode;
	} // end if

	if (RETRO_KeyPressed(SDL_SCANCODE_Y)) {
		// toggle y clipping
		y_clip_mode = -y_clip_mode;
	} // end if

	if (RETRO_KeyPressed(SDL_SCANCODE_Z)) {
		// toggle z clipping
		z_clip_mode = -z_clip_mode;
	} // end if

	// forward/backward
	if (RETRO_KeyState(SDL_SCANCODE_UP)) {
		// move forward
		if ((cam_speed += 1) > MAX_SPEED) cam_speed = MAX_SPEED;
	} // end if
	else
		if (RETRO_KeyState(SDL_SCANCODE_DOWN)) {
			// move backward
			if ((cam_speed -= 1) < -MAX_SPEED) cam_speed = -MAX_SPEED;
		} // end if

	// rotate around y axis or yaw
	if (RETRO_KeyState(SDL_SCANCODE_RIGHT)) {
		cam.dir.y += 5;
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_LEFT)) {
		cam.dir.y -= 5;
	} // end if

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

	// move point light source in ellipse around game world
	lights2[POINT_LIGHT_INDEX].pos.x = 1000 * Fast_Cos(plight_ang);
	lights2[POINT_LIGHT_INDEX].pos.y = 100;
	lights2[POINT_LIGHT_INDEX].pos.z = 1000 * Fast_Sin(plight_ang);

	if ((plight_ang += 3) > 360)
		plight_ang = 0;

	// move spot light source in ellipse around game world
	lights2[SPOT_LIGHT2_INDEX].pos.x = 1000 * Fast_Cos(slight_ang);
	lights2[SPOT_LIGHT2_INDEX].pos.y = 200;
	lights2[SPOT_LIGHT2_INDEX].pos.z = 1000 * Fast_Sin(slight_ang);

	if ((slight_ang -= 5) < 0)
		slight_ang = 360;

	// generate camera matrix
	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

	//////////////////////////////////////////////////////////////////////////
	// flat shaded textured cube

	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV2(&obj_terrain);

	// generate rotation matrix around y axis
	//Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

	MAT_IDENTITY_4X4(&mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV2(&obj_terrain, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

	// perform world transform
	Model_To_World_OBJECT4DV2(&obj_terrain, TRANSFORM_TRANS_ONLY);

	// insert the object into render list
	Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, &obj_terrain, 0);

	// reset number of polys rendered
	debug_polys_rendered_per_frame = 0;
	debug_polys_lit_per_frame = 0;

	// update rotation angles
	//if ((x_ang+=.2) > 360) x_ang = 0; 
	//if ((y_ang+=.4) > 360) y_ang = 0;
	//if ((z_ang+=.8) > 360) z_ang = 0;

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
		Transform_LIGHTSV2(lights2, 4, &cam.mcam, TRANSFORM_LOCAL_TO_TRANS);
		Light_RENDERLIST4DV2_World2_16(&rend_list, &cam, lights2, 4);
	} // end if

	// sort the polygon list (hurry up!)
	if (zsort_mode == 1)
		Sort_RENDERLIST4DV2(&rend_list, SORT_POLYLIST_AVGZ);

	// apply camera to perspective transformation
	Camera_To_Perspective_RENDERLIST4DV2(&rend_list, &cam);

	// apply screen transform
	Perspective_To_Screen_RENDERLIST4DV2(&rend_list, &cam);

	// reset number of polys rendered
	debug_polys_rendered_per_frame = 0;

	// render the object

	if (wireframe_mode == 0)
		Draw_RENDERLIST4DV2_Wire16(&rend_list, framebuffer, framebuffer_pitch);
	else
		if (wireframe_mode == 1)
			Draw_RENDERLIST4DV2_Solid16(&rend_list, framebuffer, framebuffer_pitch);

	// draw cockpit
//	Draw_BOB16(&cockpit, framebuffer, framebuffer_pitch);
	RECT src_rect = { 0, 0, cockpit.width, cockpit.height }, // the source rectangle
		dest_rect = { 0, 0, screen_width, screen_height }; // the destination rectangle

	Blit_Rect16(src_rect, cockpit.images[0], cockpit.width * 2, dest_rect, framebuffer, framebuffer_pitch, 0);

	sprintf(work_string, "Lighting [%s]: Ambient=%d, Infinite=%d, Point=%d, Spot=%d, BckFceRM [%s]",
		((lighting_mode == 1) ? "ON" : "OFF"),
		lights2[AMBIENT_LIGHT_INDEX].state,
		lights2[INFINITE_LIGHT_INDEX].state,
		lights2[POINT_LIGHT_INDEX].state,
		lights2[SPOT_LIGHT2_INDEX].state,
		((backface_mode == 1) ? "ON" : "OFF"));

	Draw_Text_GDI16(work_string, 0, screen_height - 34, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	sprintf(work_string, "X Clipping [%s], Y Clipping [%s], Z Clipping [%s],  ", ((x_clip_mode == 1) ? "ON" : "OFF"),
		((y_clip_mode == 1) ? "ON" : "OFF"),
		((z_clip_mode == 1) ? "ON" : "OFF"));

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
		Draw_Text_GDI16("<S>..............Toggle spot light source.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<W>..............Toggle wire frame/solid mode.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<B>..............Toggle backface removal.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<X>..............Toggle X axis clipping.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<Y>..............Toggle Y axis clipping.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<Z>..............Toggle Z axis clipping.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<H>..............Toggle Help.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<ESC>............Exit demo.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	} // end help

	sprintf(work_string, "Polys Rendered: %d, Polys lit: %d", debug_polys_rendered_per_frame, debug_polys_lit_per_frame);
	Draw_Text_GDI16(work_string, 0, screen_height - 34 - 16 - 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	sprintf(work_string, "CAM [%5.2f, %5.2f, %5.2f], CELL [%d, %d]", cam.pos.x, cam.pos.y, cam.pos.z, cell_x, cell_y);

	Draw_Text_GDI16(work_string, 0, screen_height - 34 - 16 - 16 - 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	// sync to 30ish fps
	Wait_Clock(30);
}

///////////////////////////////////////////////////////////

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init16();

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

	VECTOR4D terrain_pos = { 0,0,0,0 };

	Generate_Terrain_OBJECT4DV2(&obj_terrain,            // pointer to object
		TERRAIN_WIDTH,           // width in world coords on x-axis
		TERRAIN_HEIGHT,          // height (length) in world coords on z-axis
		TERRAIN_SCALE,           // vertical scale of terrain
		"assets/earthheightmap02.bmp",  // filename of height bitmap encoded in 256 colors
		"assets/earthcolormap01.bmp",   // filename of texture map
		RGB16Bit(255, 255, 255),  // color of terrain if no texture
		&terrain_pos,           // initial position
		NULL,                   // initial rotations
		POLY4DV2_ATTR_RGB16 |
		POLY4DV2_ATTR_SHADE_MODE_FLAT | /* POLY4DV2_ATTR_SHADE_MODE_GOURAUD */
		POLY4DV2_ATTR_SHADE_MODE_TEXTURE);

	// set up lights
	Reset_Lights_LIGHTV2(lights2, MAX_LIGHTS);

	// create some working colors
	white.rgba = _RGBA32BIT(255, 255, 255, 0);
	gray.rgba = _RGBA32BIT(100, 100, 100, 0);
	black.rgba = _RGBA32BIT(0, 0, 0, 0);
	red.rgba = _RGBA32BIT(255, 0, 0, 0);
	green.rgba = _RGBA32BIT(0, 255, 0, 0);
	blue.rgba = _RGBA32BIT(0, 0, 255, 0);

	// ambient light
	Init_Light_LIGHTV2(lights2,
		AMBIENT_LIGHT_INDEX,
		LIGHTV2_STATE_ON,      // turn the light on
		LIGHTV2_ATTR_AMBIENT,  // ambient light type
		gray, black, black,    // color for ambient term only
		NULL, NULL,            // no need for pos or dir
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D dlight_dir = { -1,0,-1,1 };

	// directional light
	Init_Light_LIGHTV2(lights2,
		INFINITE_LIGHT_INDEX,
		LIGHTV2_STATE_ON,      // turn the light on
		LIGHTV2_ATTR_INFINITE, // infinite light type
		black, gray, black,    // color for diffuse term only
		NULL, &dlight_dir,     // need direction only
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D plight_pos = { 0,200,0,1 };

	// point light
	Init_Light_LIGHTV2(lights2,
		POINT_LIGHT_INDEX,
		LIGHTV2_STATE_ON,      // turn the light on
		LIGHTV2_ATTR_POINT,    // pointlight type
		black, green, black,   // color for diffuse term only
		&plight_pos, NULL,     // need pos only
		0, .001, 0,              // linear attenuation only
		0, 0, 1);                // spotlight info NA

	VECTOR4D slight2_pos = { 0,200,0,1 };
	VECTOR4D slight2_dir = { -1,0,-1,1 };

	// spot light2
	Init_Light_LIGHTV2(lights2,
		SPOT_LIGHT2_INDEX,
		LIGHTV2_STATE_ON,         // turn the light on
		LIGHTV2_ATTR_SPOTLIGHT2,  // spot light type 2
		black, red, black,      // color for diffuse term only
		&slight2_pos, &slight2_dir, // need pos only
		0, .001, 0,                 // linear attenuation only
		0, 0, 1);

	// create lookup for lighting engine
	RGB_16_8_IndexedRGB_Table_Builder(DD_PIXEL_FORMAT565,  // format we want to build table for
		palette,             // source palette
		rgblookup);          // lookup table

	// load in the cockpit image
	Load_Bitmap_File(&bitmap16bit, "assets/lego01.bmp");
	Create_BOB(&cockpit, 0, 0, 800, 600, 1, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);
	Load_Frame_BOB16(&cockpit, &bitmap16bit, 0, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);
}

///////////////////////////////////////////////////////////

void DEMO_Deinitialize(void)
{
	// this function is where you shutdown your game and
	// release all resources that you allocated

	Close_Error_File();
}

//////////////////////////////////////////////////////////