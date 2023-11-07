//
// DEMOII12_3.CPP - Alpha Blending demo with polygon level control
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
#include "lib/t3dlib10.h"

// DEFINES ////////////////////////////////////////////////

// create some constants for ease of access
#define AMBIENT_LIGHT_INDEX   0 // ambient light index
#define INFINITE_LIGHT_INDEX  1 // infinite light index
#define POINT_LIGHT_INDEX     2 // point light index
#define SPOT_LIGHT1_INDEX     4 // point light index
#define SPOT_LIGHT2_INDEX     3 // spot light index

#define CAM_DECEL            (.25)  // deceleration/friction
#define MAX_SPEED             20
#define NUM_OBJECTS           5     // number of objects system loads
#define NUM_SCENE_OBJECTS     100    // number of scenery objects 
#define UNIVERSE_RADIUS       2000  // size of universe
#define MAX_VEL               20   // maxium velocity of objects

// GLOBALS ////////////////////////////////////////////////

char buffer[2048];                        // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,0,0,1 };
POINT4D  cam_target = { 0,0,0,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

// all your initialization code goes here...
VECTOR4D vscale = { 1.0,1.0,1.0,1 },
vpos = { 0,0,150,1 },
vrot = { 0,0,0,1 };

CAM4DV1         cam;                    // the single camera
OBJECT4DV2_PTR  obj_work;               // pointer to active working object
OBJECT4DV2      obj_array[NUM_OBJECTS], // array of objects 
obj_scene;              // general scenery object

// filenames of objects to load
const char *object_filenames[NUM_OBJECTS] = { "assets/sphere_bw01.cob",
										"assets/sphere_red_alpha01.cob",
										"assets/sphere_green_alpha01.cob",
										"assets/sphere_blue_alpha01.cob",
										"assets/sphere_white_alpha01.cob",
};

POINT4D         scene_objects[NUM_SCENE_OBJECTS];     // positions of scenery objects
VECTOR4D        scene_objects_vel[NUM_SCENE_OBJECTS]; // velocities of scenery objects

RENDERLIST4DV2  rend_list;      // the render list
RGBAV1 white, gray, black, red, green, blue; // general colors

// physical model defines
float cam_speed = 0;       // speed of the camera/jeep

ZBUFFERV1 zbuffer;          // out little z buffer!
RENDERCONTEXTV1 rc;         // the rendering context
BOB background;             // the background image

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static MATRIX4X4 mrot;   // general rotation matrix

	static float plight_ang = 0,
		slight_ang = 0; // angles for light motion

	// use these to rotate objects
	static float x_ang = 0, y_ang = 0, z_ang = 0;

	// state variables for different rendering modes and help
	static int wireframe_mode = 1;
	static int backface_mode = 1;
	static int lighting_mode = 1;
	static int help_mode = 1;
	static int zsort_mode = 1;
	static int x_clip_mode = 1;
	static int y_clip_mode = 1;
	static int z_clip_mode = 1;
	static float turning = 0;

	char work_string[256]; // temp string

	int index; // looping var

	// start the timing clock
	Start_Clock();

	// draw the sky
//	Draw_Rectangle16(0,0, screen_width, screen_height, RGB16Bit(0,0,0), framebuffer, framebuffer_pitch);

	// draw the ground
//	Draw_Rectangle16(0,screen_height*.5, screen_width, screen_height, RGB16Bit(190,190,230), framebuffer, framebuffer_pitch);

	// draw background
	RECT src_rect = { 0, 0, background.width, background.height }, // the source rectangle
		dest_rect = { 0, 0, screen_width, screen_height }; // the destination rectangle

	Blit_Rect16(src_rect, background.images[0], background.width * 2, dest_rect, framebuffer, framebuffer_pitch);

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
	if (RETRO_KeyPressed(SDL_SCANCODE_S)) {
		// toggle z sorting
		zsort_mode = -zsort_mode;
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

	 // rotate
	if (RETRO_KeyState(SDL_SCANCODE_RIGHT)) {
		cam.dir.y += 3;

		// add a little turn to object
		if ((turning += 2) > 25)
			turning = 25;
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_LEFT)) {
		cam.dir.y -= 3;

		// add a little turn to object
		if ((turning -= 2) < -25)
			turning = -25;

	} // end if
	else // center heading again
	{
		if (turning > 0)
			turning -= 1;
		else
			if (turning < 0)
				turning += 1;

	} // end else

	// decelerate camera
	if (cam_speed > (CAM_DECEL)) cam_speed -= CAM_DECEL;
	else
		if (cam_speed < (-CAM_DECEL)) cam_speed += CAM_DECEL;
		else
			cam_speed = 0;

	// move camera
	cam.pos.x += cam_speed * Fast_Sin(cam.dir.y);
	cam.pos.z += cam_speed * Fast_Cos(cam.dir.y);

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

	////////////////////////////////////////////////////////
	// insert the scenery into universe
	for (index = 0; index < NUM_SCENE_OBJECTS; index++) {
		// select proper object first
		obj_work = &obj_array[(int)scene_objects[index].w];

		// reset the object (this only matters for backface and object removal)
		Reset_OBJECT4DV2(obj_work);

		// set position of tower
		obj_work->world_pos.x = scene_objects[index].x;
		obj_work->world_pos.y = scene_objects[index].y;
		obj_work->world_pos.z = scene_objects[index].z;

		// move objects
		scene_objects[index].x += scene_objects_vel[index].x;
		scene_objects[index].y += scene_objects_vel[index].y;
		scene_objects[index].z += scene_objects_vel[index].z;

		// test for out of bounds
		if (scene_objects[index].x >= UNIVERSE_RADIUS || scene_objects[index].x <= -UNIVERSE_RADIUS) {
			scene_objects_vel[index].x = -scene_objects_vel[index].x;
			scene_objects[index].x += scene_objects_vel[index].x;
		} // end if

		if (scene_objects[index].y >= (UNIVERSE_RADIUS / 2) || scene_objects[index].y <= -(UNIVERSE_RADIUS / 2)) {
			scene_objects_vel[index].y = -scene_objects_vel[index].y;
			scene_objects[index].y += scene_objects_vel[index].y;
		} // end if

		if (scene_objects[index].z >= UNIVERSE_RADIUS || scene_objects[index].z <= -UNIVERSE_RADIUS) {
			scene_objects_vel[index].z = -scene_objects_vel[index].z;
			scene_objects[index].z += scene_objects_vel[index].z;
		} // end if

		// attempt to cull object   
		if (!Cull_OBJECT4DV2(obj_work, &cam, CULL_OBJECT_XYZ_PLANES)) {
			MAT_IDENTITY_4X4(&mrot);

			// rotate the local coords of the object
			Transform_OBJECT4DV2(obj_work, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

			// perform world transform
			Model_To_World_OBJECT4DV2(obj_work, TRANSFORM_TRANS_ONLY);

			// insert the object into render list
			Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, obj_work, 0);

		} // end if

	} // end for

	// update rotation angles
	if ((x_ang += .2) > 360) x_ang = 0;
	if ((y_ang += .4) > 360) y_ang = 0;
	if ((z_ang += .8) > 360) z_ang = 0;

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

	// render the renderinglist
	if (wireframe_mode == 0)
		Draw_RENDERLIST4DV2_Wire16(&rend_list, framebuffer, framebuffer_pitch);
	else
		if (wireframe_mode == 1) {
			// initialize zbuffer to 16000 fixed point
			Clear_Zbuffer(&zbuffer, (16000 << FIXP16_SHIFT));

#if 0
			RENDERCONTEXTV1 rc;

			// no z buffering, polygons will be rendered as are in list
#define RENDER_ATTR_NOBUFFER                     0x00000001  

// use z buffering rasterization
#define RENDER_ATTR_ZBUFFER                      0x00000002  

// use 1/z buffering rasterization
#define RENDER_ATTR_INVZBUFFER                   0x00000004  

// use mipmapping
#define RENDER_ATTR_MIPMAP                       0x00000010  

// enable alpha blending and override
#define RENDER_ATTR_ALPHA                        0x00000020  

// use affine texturing for all polys
#define RENDER_ATTR_TEXTURE_PERSPECTIVE_AFFINE   0x00000100  

// use perfect perspective texturing
#define RENDER_ATTR_TEXTURE_PERSPECTIVE_CORRECT  0x00000200  

// use linear piecewise perspective texturing
#define RENDER_ATTR_TEXTURE_PERSPECTIVE_LINEAR   0x00000400  

// use a hybrid of affine and linear piecewise based on distance
#define RENDER_ATTR_TEXTURE_PERSPECTIVE_HYBRID1  0x00000800  

// not implemented yet
#define RENDER_ATTR_TEXTURE_PERSPECTIVE_HYBRID2  0x00001000  

#endif

			// set up rendering context
			rc.attr = RENDER_ATTR_ZBUFFER
				| RENDER_ATTR_ALPHA
				//| RENDER_ATTR_MIPMAP  
				//| RENDER_ATTR_BILERP
				| RENDER_ATTR_TEXTURE_PERSPECTIVE_AFFINE;

			rc.video_buffer = framebuffer;
			rc.lpitch = framebuffer_pitch;
			rc.mip_dist = 3500;
			rc.zbuffer = (UCHAR *)zbuffer.zbuffer;
			rc.zpitch = screen_width * 4;
			rc.rend_list = &rend_list;
			rc.texture_dist = 0;
			rc.alpha_override = -1;

			// render scene
			Draw_RENDERLIST4DV2_RENDERCONTEXTV1_16(&rc);
		}

	sprintf(work_string, "Lighting [%s]: Ambient=%d, Infinite=%d, Point=%d, Spot=%d, BckFceRM [%s], Zsort [%s]",
		((lighting_mode == 1) ? "ON" : "OFF"),
		lights[AMBIENT_LIGHT_INDEX].state,
		lights[INFINITE_LIGHT_INDEX].state,
		lights[POINT_LIGHT_INDEX].state,
		lights[SPOT_LIGHT2_INDEX].state,
		((backface_mode == 1) ? "ON" : "OFF"),
		((zsort_mode == 1) ? "ON" : "OFF"));


	Draw_Text_GDI16(work_string, 0, screen_height - 34 - 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	// draw instructions
	Draw_Text_GDI16("Press ESC to exit. Press <H> for Help.", 0, 0, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	// should we display help
	int text_y = 16;
	if (help_mode == 1) {
		// draw help menu
		Draw_Text_GDI16("<A>..............Toggle ambient light source.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<I>..............Toggle infinite light source.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<P>..............Toggle point light source.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<S>..............Toggle spot light source.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<N>..............Next object.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<W>..............Toggle wire frame/solid mode.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<B>..............Toggle backface removal.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<S>..............Toggle Z sorting.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<H>..............Toggle Help.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<ESC>............Exit demo.", 0, text_y += 12, RGB16Bit(0, 0, 0), framebuffer, framebuffer_pitch);

	} // end help

	sprintf(work_string, "Polys Rendered: %d, Polys lit: %d", debug_polys_rendered_per_frame, debug_polys_lit_per_frame);
	Draw_Text_GDI16(work_string, 0, screen_height - 34 - 16 - 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	sprintf(work_string, "CAM [%5.2f, %5.2f, %5.2f]", cam.pos.x, cam.pos.y, cam.pos.z);

	Draw_Text_GDI16(work_string, 0, screen_height - 34 - 16 - 16 - 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

	// sync to 30ish fps
	Wait_Clock(30);
}

//////////////////////////////////////////////////////////

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
		120.0,      // field of view in degrees
		screen_width,   // size of final screen viewport
		screen_height);

	// set a scaling vector
	VECTOR4D_INITXYZ(&vscale, 50.00, 50.00, 50.00);

	// load all the objects in
	for (int index_obj = 0; index_obj < NUM_OBJECTS; index_obj++) {
		Load_OBJECT4DV2_COB2(&obj_array[index_obj], object_filenames[index_obj],
			&vscale, &vpos, &vrot, VERTEX_FLAGS_SWAP_YZ |
			VERTEX_FLAGS_TRANSFORM_LOCAL
			/* VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD*/, 0);

	} // end for index_obj

	// position the scenery objects randomly
	for (int index = 0; index < NUM_SCENE_OBJECTS; index++) {
		// randomly position object
		scene_objects[index].x = RAND_RANGE(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);
		scene_objects[index].y = RAND_RANGE(-(UNIVERSE_RADIUS / 2), (UNIVERSE_RADIUS / 2));
		scene_objects[index].z = RAND_RANGE(-UNIVERSE_RADIUS, UNIVERSE_RADIUS);

		// select random object, use w to store value
		scene_objects[index].w = RAND_RANGE(0, NUM_OBJECTS - 1);
	} // end for


	// select random velocities
	for (int index = 0; index < NUM_SCENE_OBJECTS; index++) {
		// randomly position object
		scene_objects_vel[index].x = RAND_RANGE(-MAX_VEL, MAX_VEL);
		scene_objects_vel[index].y = RAND_RANGE(-MAX_VEL, MAX_VEL);
		scene_objects_vel[index].z = RAND_RANGE(-MAX_VEL, MAX_VEL);
	} // end for

	// set up lights
	Reset_Lights_LIGHTV2(lights2, MAX_LIGHTS);

	// create some working colors
	white.rgba = _RGBA32BIT(255, 255, 255, 0);
	gray.rgba = _RGBA32BIT(150, 150, 150, 0);
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

	VECTOR4D dlight_dir = { -1,0,-1,1 };

	// directional light
	Init_Light_LIGHTV2(lights2,               // array of lights to work with
		INFINITE_LIGHT_INDEX,
		LIGHTV2_STATE_ON,      // turn the light on
		LIGHTV2_ATTR_INFINITE, // infinite light type
		black, gray, black,    // color for diffuse term only
		NULL, &dlight_dir,     // need direction only
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D plight_pos = { 0,200,0,1 };

	// point light
	Init_Light_LIGHTV2(lights2,               // array of lights to work with
		POINT_LIGHT_INDEX,
		LIGHTV2_STATE_ON,      // turn the light on
		LIGHTV2_ATTR_POINT,    // pointlight type
		black, green, black,    // color for diffuse term only
		&plight_pos, NULL,     // need pos only
		0, .002, 0,              // linear attenuation only
		0, 0, 1);                // spotlight info NA

	VECTOR4D slight2_pos = { 0,1000,0,1 };
	VECTOR4D slight2_dir = { -1,0,-1,1 };

	// spot light2
	Init_Light_LIGHTV2(lights2,                  // array of lights to work with
		SPOT_LIGHT2_INDEX,
		LIGHTV2_STATE_ON,         // turn the light on
		LIGHTV2_ATTR_SPOTLIGHT2,  // spot light type 2
		black, red, black,        // color for diffuse term only
		&slight2_pos, &slight2_dir, // need pos only
		0, .001, 0,                 // linear attenuation only
		0, 0, 1);

	// create lookup for lighting engine
	RGB_16_8_IndexedRGB_Table_Builder(DD_PIXEL_FORMAT565,  // format we want to build table for
		palette,             // source palette
		rgblookup);          // lookup table

	// create the z buffer
	Create_Zbuffer(&zbuffer,
		screen_width,
		screen_height,
		ZBUFFER_ATTR_32BIT);

	// build alpha lookup table
	RGB_Alpha_Table_Builder(NUM_ALPHA_LEVELS, rgb_alpha_table);

	// load in the background
	Create_BOB(&background, 0, 0, 800, 600, 1, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);
	Load_Bitmap_File(&bitmap16bit, "assets/checkerboard800.bmp");
	Load_Frame_BOB16(&background, &bitmap16bit, 0, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);
}

///////////////////////////////////////////////////////////

void DEMO_Deinitialize(void)
{
	Close_Error_File();

	Delete_Zbuffer(&zbuffer);
}

//////////////////////////////////////////////////////////