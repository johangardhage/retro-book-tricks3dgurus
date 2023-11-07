//
// DEMOII12_1.CPP - gouraud texturing demo
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

#define NUM_OBJECTS           3 // number of objects per class

// GLOBALS ////////////////////////////////////////////////

char buffer[2048];                        // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,0,0,1 };
POINT4D  cam_target = { 0,0,0,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

// all your initialization code goes here...
VECTOR4D vscale = { 1.0,1.0,1.0,1 },
vpos = { 0,0,0,1 },
vrot = { 0,0,0,1 };

CAM4DV1        cam;                    // the single camera

OBJECT4DV2_PTR  obj_constant_work,     // pointer to active working objects
obj_flat_work,
obj_gouraud_work;

OBJECT4DV2      obj_constant_array[NUM_OBJECTS], // array of objects
obj_flat_array[NUM_OBJECTS],
obj_gouraud_array[NUM_OBJECTS];

int curr_constant_object = 0,  // the current object of each class
curr_flat_object = 0,
curr_gouraud_object = 0;

// filenames of objects to load
const char *object_constant_filenames[NUM_OBJECTS] = {
										"assets/cube_constant_textured_02.cob",
										"assets/sphere_constant_textured_02.cob",
										"assets/rec_constant_textured_01.cob",
};

// filenames of objects to load
const char *object_flat_filenames[NUM_OBJECTS] = {
										"assets/cube_flat_textured_02.cob",
										"assets/sphere_flat_textured_02.cob",
										"assets/rec_flat_textured_01.cob",
};

// filenames of objects to load
const char *object_gouraud_filenames[NUM_OBJECTS] = {
										"assets/cube_gouraud_textured_02.cob",
										"assets/sphere_gouraud_textured_02.cob",
										"assets/rec_gouraud_textured_01.cob",
};

RENDERLIST4DV2 rend_list; // the render list
ZBUFFERV1 zbuffer;          // out little z buffer!
RGBAV1 white, gray, black, red, green, blue;

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
	static int help_mode = 1;
	static int zsort_mode = 1;

	char work_string[256]; // temp string

	// start the timing clock
	Start_Clock();

	// draw the sky
	Draw_Rectangle16(0, 0, screen_width, screen_height / 2, RGB16Bit(0, 35, 50), framebuffer, framebuffer_pitch);

	// draw the ground
	Draw_Rectangle16(0, screen_height / 2 - 1, screen_width, screen_height, RGB16Bit(20, 12, 0), framebuffer, framebuffer_pitch);

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
		if (lights2[AMBIENT_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights2[AMBIENT_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights2[AMBIENT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	} // end if

	// toggle infinite light
	if (RETRO_KeyPressed(SDL_SCANCODE_I)) {
		// toggle ambient light
		if (lights2[INFINITE_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights2[INFINITE_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights2[INFINITE_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	} // end if

	// toggle point light
	if (RETRO_KeyPressed(SDL_SCANCODE_P)) {
		// toggle point light
		if (lights2[POINT_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights2[POINT_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights2[POINT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;
	} // end if

	// toggle spot light
	if (RETRO_KeyPressed(SDL_SCANCODE_S)) {
		// toggle spot light
		if (lights2[SPOT_LIGHT2_INDEX].state == LIGHTV1_STATE_ON)
			lights2[SPOT_LIGHT2_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights2[SPOT_LIGHT2_INDEX].state = LIGHTV1_STATE_ON;
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

	// move to next object
	if (RETRO_KeyPressed(SDL_SCANCODE_1)) {
		if (++curr_constant_object >= NUM_OBJECTS)
			curr_constant_object = 0;

		// update pointer
		obj_constant_work = &obj_constant_array[curr_constant_object];
	} // end if

	// move to next object
	if (RETRO_KeyPressed(SDL_SCANCODE_2)) {
		if (++curr_flat_object >= NUM_OBJECTS)
			curr_flat_object = 0;

		// update pointer
		obj_flat_work = &obj_flat_array[curr_flat_object];
	} // end if

	// move to next object
	if (RETRO_KeyPressed(SDL_SCANCODE_3)) {
		if (++curr_gouraud_object >= NUM_OBJECTS)
			curr_gouraud_object = 0;

		// update pointer
		obj_gouraud_work = &obj_gouraud_array[curr_gouraud_object];
	} // end if

	static float plight_ang = 0, slight_ang = 0; // angles for light motion

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

	// use these to rotate objects
	static float x_ang = 0, y_ang = 0, z_ang = 0;

	//////////////////////////////////////////////////////////////////////////
	// constant shaded 

	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV2(obj_constant_work);

	// set position of constant shaded water
	obj_constant_work->world_pos.x = -50;
	obj_constant_work->world_pos.y = 0;
	obj_constant_work->world_pos.z = 150;

	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV2(obj_constant_work, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

	// perform world transform
	Model_To_World_OBJECT4DV2(obj_constant_work, TRANSFORM_TRANS_ONLY);

	// insert the object into render list
	Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, obj_constant_work, 0);

	//////////////////////////////////////////////////////////////////////////
	// flat shaded

	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV2(obj_flat_work);

	// set position of flat shaded water
	obj_flat_work->world_pos.x = 0;
	obj_flat_work->world_pos.y = 0;
	obj_flat_work->world_pos.z = 150;

	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV2(obj_flat_work, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

	// perform world transform
	Model_To_World_OBJECT4DV2(obj_flat_work, TRANSFORM_TRANS_ONLY);

	// insert the object into render list
	Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, obj_flat_work, 0);

	//////////////////////////////////////////////////////////////////////////
	// gouraud shaded 

	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV2(obj_gouraud_work);

	// set position of gouraud shaded 
	obj_gouraud_work->world_pos.x = 50;
	obj_gouraud_work->world_pos.y = 0;
	obj_gouraud_work->world_pos.z = 150;

	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV2(obj_gouraud_work, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

	// perform world transform
	Model_To_World_OBJECT4DV2(obj_gouraud_work, TRANSFORM_TRANS_ONLY);

	// insert the object into render list
	Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, obj_gouraud_work, 0);

	///////////////////////////////////////////////////////////////////////////

	// update rotation angles
	//if ((x_ang+=1) > 360) x_ang = 0;
	if ((y_ang += 1) > 360) y_ang = 0;
	if ((z_ang += 2) > 360) z_ang = 0;

	// remove backfaces
	if (backface_mode == 1)
		Remove_Backfaces_RENDERLIST4DV2(&rend_list, &cam);

	// apply world to camera transform
	World_To_Camera_RENDERLIST4DV2(&rend_list, &cam);

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

	sprintf(work_string, "Lighting [%s]: Ambient=%d, Infinite=%d, Point=%d, Spot=%d | Zsort [%s], BckFceRM [%s]",
		((lighting_mode == 1) ? "ON" : "OFF"),
		lights2[AMBIENT_LIGHT_INDEX].state,
		lights2[INFINITE_LIGHT_INDEX].state,
		lights2[POINT_LIGHT_INDEX].state,
		lights2[SPOT_LIGHT2_INDEX].state,
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
		Draw_Text_GDI16("<1>..............Next constant shaded object.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<2>..............Next flat shaded object.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<3>..............Next gouraud shaded object.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<H>..............Toggle Help.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
		Draw_Text_GDI16("<ESC>............Exit demo.", 0, text_y += 12, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	} // end help

	// reset number of polys rendered
	debug_polys_rendered_per_frame = 0;

	// render the object
	if (wireframe_mode == 0)
		Draw_RENDERLIST4DV2_Wire16(&rend_list, framebuffer, framebuffer_pitch);
	else
		if (wireframe_mode == 1) {
			Clear_Zbuffer(&zbuffer, (32000 << FIXP16_SHIFT));
			Draw_RENDERLIST4DV2_SolidZB2_16(&rend_list, framebuffer, framebuffer_pitch, (UCHAR *)zbuffer.zbuffer, screen_width * 4);
		} // end if

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
	Init_CAM4DV1(&cam,             // the camera object
		CAM_MODEL_EULER,  // the euler model
		&cam_pos,         // initial camera position
		&cam_dir,         // initial camera angles
		&cam_target,      // no target
		20.0,             // near and far clipping planes
		12000.0,
		120.0,            // field of view in degrees
		screen_width,     // size of final screen viewport
		screen_height);

	// set a scaling vector
	VECTOR4D_INITXYZ(&vscale, 18, 18, 18);

	// load all the constant shaded objects in
	for (int index_obj = 0; index_obj < NUM_OBJECTS; index_obj++) {
		Load_OBJECT4DV2_COB2(&obj_constant_array[index_obj], object_constant_filenames[index_obj],
			&vscale, &vpos, &vrot, VERTEX_FLAGS_INVERT_WINDING_ORDER
			| VERTEX_FLAGS_TRANSFORM_LOCAL
			| VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD
			, 0);

	} // end for index_obj

	// set current object
	curr_constant_object = 0;
	obj_constant_work = &obj_constant_array[curr_constant_object];

	// load all the flat shaded objects in
	for (int index_obj = 0; index_obj < NUM_OBJECTS; index_obj++) {
		Load_OBJECT4DV2_COB2(&obj_flat_array[index_obj], object_flat_filenames[index_obj],
			&vscale, &vpos, &vrot, VERTEX_FLAGS_INVERT_WINDING_ORDER
			| VERTEX_FLAGS_TRANSFORM_LOCAL
			| VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD
			, 0);

	} // end for index_obj

	// set current object
	curr_flat_object = 0;
	obj_flat_work = &obj_flat_array[curr_flat_object];

	// load all the gouraud shaded objects in
	for (int index_obj = 0; index_obj < NUM_OBJECTS; index_obj++) {
		Load_OBJECT4DV2_COB2(&obj_gouraud_array[index_obj], object_gouraud_filenames[index_obj],
			&vscale, &vpos, &vrot, VERTEX_FLAGS_INVERT_WINDING_ORDER
			| VERTEX_FLAGS_TRANSFORM_LOCAL
			| VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD
			, 0);

	} // end for index_obj

	// set current object
	curr_gouraud_object = 0;
	obj_gouraud_work = &obj_gouraud_array[curr_gouraud_object];

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
	Init_Light_LIGHTV2(lights2, AMBIENT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_AMBIENT,  // ambient light type
		gray, black, black,    // color for ambient term only
		NULL, NULL,            // no need for pos or dir
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D dlight_dir = { -1,0,-1,0 };

	// directional light
	Init_Light_LIGHTV2(lights2, INFINITE_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_INFINITE, // infinite light type
		black, gray, black,    // color for diffuse term only
		NULL, &dlight_dir,     // need direction only
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D plight_pos = { 0,200,0,0 };

	// point light
	Init_Light_LIGHTV2(lights2, POINT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_POINT,    // pointlight type
		black, green, black,   // color for diffuse term only
		&plight_pos, NULL,     // need pos only
		0, .001, 0,              // linear attenuation only
		0, 0, 1);                // spotlight info NA

	VECTOR4D slight2_pos = { 0,200,0,0 };
	VECTOR4D slight2_dir = { -1,0,-1,0 };

	// spot light2
	Init_Light_LIGHTV2(lights2, SPOT_LIGHT2_INDEX,
		LIGHTV1_STATE_ON,         // turn the light on
		LIGHTV1_ATTR_SPOTLIGHT2,  // spot light type 2
		black, red, black,      // color for diffuse term only
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
}

///////////////////////////////////////////////////////////

void DEMO_Deinitialize(void)
{
	// this function is where you shutdown your game and
	// release all resources that you allocated

	Close_Error_File();
}

//////////////////////////////////////////////////////////