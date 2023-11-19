//
// DEMOII8_8.CPP - .ASC file loading demo with the works!
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

#define POINT_SIZE        100
#define NUM_POINTS_X      (2*UNIVERSE_RADIUS/POINT_SIZE)
#define NUM_POINTS_Z      (2*UNIVERSE_RADIUS/POINT_SIZE)
#define NUM_POINTS        (NUM_POINTS_X*NUM_POINTS_Z)

// defines for objects
#define NUM_TOWERS        64 // 96
#define NUM_TANKS         32 // 24
#define TANK_SPEED        15

// create some constants for ease of access
#define AMBIENT_LIGHT_INDEX   0 // ambient light index
#define INFINITE_LIGHT_INDEX  1 // infinite light index
#define POINT_LIGHT_INDEX     2 // point light index
#define SPOT_LIGHT_INDEX      3 // spot light index

// GLOBALS ////////////////////////////////////////////////

char buffer[2048];                        // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,0,-250,1 };
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

// state variables for different rendering modes and help
int wireframe_mode = -1;
int backface_mode = 1;
int lighting_mode = 1;
int help_mode = 1;
int zsort_mode = 1;

// controlled during load dialog
int swapyz = 0,
iwinding = 0;

char ascfilename[256]; // holds file name when loader loads

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static MATRIX4X4 mrot;   // general rotation matrix

	// these are used to create a circling camera
	char work_string[256]; // temp string

	// start the timing clock
	Start_Clock();

	// game logic here...

	// reset the render list
	Reset_RENDERLIST4DV1(&rend_list);

	// allow user to move camera

	// forward/backward
	if (RETRO_KeyState(SDL_SCANCODE_UP)) {
		// move forward
		cam.pos.x += 5 * Fast_Sin(cam.dir.y);
		cam.pos.z += 5 * Fast_Cos(cam.dir.y);
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_DOWN)) {
		// move backward
		cam.pos.x -= 5 * Fast_Sin(cam.dir.y);
		cam.pos.z -= 5 * Fast_Cos(cam.dir.y);
	} // end if

	// rotate object
	if (RETRO_KeyState(SDL_SCANCODE_RIGHT)) {
		obj_player.dir.y += 2;
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_LEFT)) {
		obj_player.dir.y -= 2;
	} // end if

	if (obj_player.dir.y > 360)
		obj_player.dir.y -= 360;

	if (obj_player.dir.y < 0)
		obj_player.dir.y += 360;

	// ambient rotation
	obj_player.dir.y++;

	// scale object
	if (RETRO_KeyState(SDL_SCANCODE_PAGEUP)) {
		VECTOR4D_INITXYZ(&vscale, 1.1, 1.1, 1.1);
		Scale_OBJECT4DV1(&obj_player, &vscale);
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_PAGEDOWN)) {
		VECTOR4D_INITXYZ(&vscale, .9, .9, .9);
		Scale_OBJECT4DV1(&obj_player, &vscale);
	} // end if

	// modes and lights

	// wireframe mode
	if (RETRO_KeyPressed(SDL_SCANCODE_W)) {
		// toggle wireframe mode
		wireframe_mode = -wireframe_mode;
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
		if (lights[AMBIENT_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights[AMBIENT_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights[AMBIENT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	} // end if

	// toggle infinite light
	if (RETRO_KeyPressed(SDL_SCANCODE_I)) {
		// toggle ambient light
		if (lights[INFINITE_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights[INFINITE_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights[INFINITE_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	} // end if

	// toggle point light
	if (RETRO_KeyPressed(SDL_SCANCODE_P)) {
		// toggle point light
		if (lights[POINT_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights[POINT_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights[POINT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	} // end if

	// toggle spot light
	if (RETRO_KeyPressed(SDL_SCANCODE_S)) {
		// toggle spot light
		if (lights[SPOT_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights[SPOT_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights[SPOT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
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

	static float plight_ang = 0, slight_ang = 0; // angles for light motion

	// move point light source in ellipse around game world
	lights[POINT_LIGHT_INDEX].pos.x = 300 * Fast_Cos(plight_ang);
	lights[POINT_LIGHT_INDEX].pos.y = 200;
	lights[POINT_LIGHT_INDEX].pos.z = 300 * Fast_Sin(plight_ang);

	if ((plight_ang += 3) > 360)
		plight_ang = 0;

	// move spot light source in ellipse around game world
	lights[SPOT_LIGHT_INDEX].pos.x = 200 * Fast_Cos(slight_ang);
	lights[SPOT_LIGHT_INDEX].pos.y = 200;
	lights[SPOT_LIGHT_INDEX].pos.z = 200 * Fast_Sin(slight_ang);

	if ((slight_ang -= 5) < 0)
		slight_ang = 360;

	/////////////////////////////////////////

	// generate camera matrix
	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

	// insert the player into the world
	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV1(&obj_player);

	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(0, obj_player.dir.y, 0, &mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV1(&obj_player, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

	// perform world transform
	Model_To_World_OBJECT4DV1(&obj_player, TRANSFORM_TRANS_ONLY);

	// perform lighting
	if (lighting_mode == 1)
		Light_OBJECT4DV1_World16(&obj_player, &cam, lights, 4);

	// insert the object into render list
	Insert_OBJECT4DV1_RENDERLIST4DV12(&rend_list, &obj_player, 0, lighting_mode);

	// insert the object into render list
	//Insert_OBJECT4DV1_RENDERLIST4DV12(&rend_list, &obj_player,0, 0);

	//////////////////////////////////////

	// remove backfaces
	if (backface_mode == 1)
		Remove_Backfaces_RENDERLIST4DV1(&rend_list, &cam);

	// light scene all at once 
	//if (lighting_mode==1)
	//   Light_RENDERLIST4DV1_World16(&rend_list, &cam, lights, 4);

	// apply world to camera transform
	World_To_Camera_RENDERLIST4DV1(&rend_list, &cam);

	// sort the polygon list (hurry up!)
	if (zsort_mode == 1)
		Sort_RENDERLIST4DV1(&rend_list, SORT_POLYLIST_AVGZ);

	// apply camera to perspective transformation
	Camera_To_Perspective_RENDERLIST4DV1(&rend_list, &cam);

	// apply screen transform
	Perspective_To_Screen_RENDERLIST4DV1(&rend_list, &cam);

	sprintf(work_string, "pos:[%f, %f, %f] heading:[%f] elev:[%f], polys[%d]",
		cam.pos.x, cam.pos.y, cam.pos.z, cam.dir.y, cam.dir.x, debug_polys_rendered_per_frame);

	Draw_Text_GDI16(work_string, 0, screen_height - 20, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	sprintf(work_string, "Lighting [%s]: Ambient=%d, Infinite=%d, Point=%d, Spot=%d | Zsort [%s], BckFceRM [%s]",
		((lighting_mode == 1) ? "ON" : "OFF"),
		lights[AMBIENT_LIGHT_INDEX].state,
		lights[INFINITE_LIGHT_INDEX].state,
		lights[POINT_LIGHT_INDEX].state,
		lights[SPOT_LIGHT_INDEX].state,
		((zsort_mode == 1) ? "ON" : "OFF"),
		((backface_mode == 1) ? "ON" : "OFF"));

	Draw_Text_GDI16(work_string, 0, screen_height - 34, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

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
		Draw_Text_GDI16("<RIGHT ARROW>....Rotate object right.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<LEFT ARROW>.....Rotate object left.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<UP ARROW>.......Move camera forward.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<DOWN ARROW>.....Move camera backward.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<PG UP>..........Scale object down", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<PG DWN>.........Scale object down", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<SPACE BAR>......Turbo.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<H>..............Toggle Help.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<ESC>............Exit demo.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	} // end help

	// reset number of polys rendered
	debug_polys_rendered_per_frame = 0;

	// render the object
	if (wireframe_mode == 1)
		Draw_RENDERLIST4DV1_Wire16(&rend_list, framebuffer, framebuffer_pitch);
	else
		Draw_RENDERLIST4DV1_Solid16(&rend_list, framebuffer, framebuffer_pitch);

	// sync to 30ish fps
	Wait_Clock(30);
}

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
		200.0,      // near and far clipping planes
		12000.0,
		120.0,      // field of view in degrees
		screen_width,   // size of final screen viewport
		screen_height);

	// load in default object
	VECTOR4D_INITXYZ(&vscale, 5.00, 5.00, 5.00);

	Load_OBJECT4DV1_3DSASC(&obj_player, "assets/sphere01.asc",
		&vscale, &vpos, &vrot,
		VERTEX_FLAGS_INVERT_WINDING_ORDER | VERTEX_FLAGS_SWAP_YZ);

	// set up lights
	Reset_Lights_LIGHTV1();

	// create some working colors
	RGBAV1 gray, black, red, green;

	gray.rgba = _RGBA32BIT(100, 100, 100, 0);
	black.rgba = _RGBA32BIT(0, 0, 0, 0);
	red.rgba = _RGBA32BIT(255, 0, 0, 0);
	green.rgba = _RGBA32BIT(0, 255, 0, 0);

	// ambient light
	Init_Light_LIGHTV1(AMBIENT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_AMBIENT,  // ambient light type
		gray, black, black,    // color for ambient term only
		NULL, NULL,            // no need for pos or dir
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D dlight_dir = { -1,0,-1,0 };

	// directional light
	Init_Light_LIGHTV1(INFINITE_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_INFINITE, // infinite light type
		black, gray, black,    // color for diffuse term only
		NULL, &dlight_dir,     // need direction only
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D plight_pos = { 0,200,0,0 };

	// point light
	Init_Light_LIGHTV1(POINT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_POINT,    // pointlight type
		black, green, black,   // color for diffuse term only
		&plight_pos, NULL,     // need pos only
		0, .001, 0,              // linear attenuation only
		0, 0, 1);                // spotlight info NA

	VECTOR4D slight_pos = { 0,200,0,0 };
	VECTOR4D slight_dir = { -1,0,-1,0 };

	// spot light
	Init_Light_LIGHTV1(SPOT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,         // turn the light on
		LIGHTV1_ATTR_SPOTLIGHT2,  // spot light type 2
		black, red, black,      // color for diffuse term only
		&slight_pos, &slight_dir, // need pos only
		0, .001, 0,                 // linear attenuation only
		0, 0, 1);

	// create lookup for lighting engine
	RGB_16_8_IndexedRGB_Table_Builder(DD_PIXEL_FORMAT565,  // format we want to build table for
		palette,             // source palette
		rgblookup);          // lookup table
}

///////////////////////////////////////////////////////////

void DEMO_Deinitialize(void)
{
	// this function is where you shutdown your game and
	// release all resources that you allocated

	Close_Error_File();
}

//////////////////////////////////////////////////////////
