//
// RAIDERS3D_2.CPP - 3D star raiders game version 2.0, 16-bit
//

#define RETRO_WIDTH 800
#define RETRO_HEIGHT 600

#include "lib/retro.h"
#include "lib/retromain.h"
#include "lib/retrofont.h"
#include "lib/retromouse.h"
#include "lib/t3dlib1.h"
#include "lib/t3dlib2.h"
#include "lib/t3dlib4.h"
#include "lib/t3dlib5.h"
#include "lib/t3dlib6.h"
#include "lib/t3dlib7.h"

// DEFINES ////////////////////////////////////////////////

#undef DEBUG_ON

// 3D engine constants for overlay star field
#define NEAR_Z            10    // the near clipping plane
#define FAR_Z             2000  // the far clipping plane    
#define VIEW_DISTANCE      400  // viewing distance from viewpoint 
								// this gives a field of view of 90 degrees
								// when projected on a window of 800 wide

// create some constants for ease of access
#define AMBIENT_LIGHT_INDEX   0 // ambient light index
#define INFINITE_LIGHT_INDEX  1 // infinite light index
#define POINT_LIGHT_INDEX     2 // point light index
#define POINT_LIGHT2_INDEX    3 // point light index
#define SPOT_LIGHT2_INDEX     4 // spot light index

// alien defines
#define NUM_ALIENS            16 // total number of ALIEN fighters 

// states for aliens
#define ALIEN_STATE_DEAD      0  // alien is dead
#define ALIEN_STATE_ALIVE     1  // alien is alive
#define ALIEN_STATE_DYING     2  // alien is in process of dying

// star defines
#define NUM_STARS           256  // number of stars in sim

// game state defines
#define GAME_STATE_INIT       0  // game initializing for first time
#define GAME_STATE_RESTART    1  // game restarting after game over
#define GAME_STATE_RUN        2  // game running normally
#define GAME_STATE_DEMO       3  // game in demo mode
#define GAME_STATE_OVER       4  // game over
#define GAME_STATE_EXIT       5  // game state exit

#define MAX_MISSES_GAME_OVER  25 // miss this many and it's game over man

// interface text positions
#define TPOS_SCORE_X          346  // players score
#define TPOS_SCORE_Y          4

#define TPOS_HITS_X           250  // total kills
#define TPOS_HITS_Y           36

#define TPOS_ESCAPED_X        224  // aliens that have escaped
#define TPOS_ESCAPED_Y        58

#define TPOS_GLEVEL_X         430 // diff level of game
#define TPOS_GLEVEL_Y         58

#define TPOS_SPEED_X          404 // diff level of game
#define TPOS_SPEED_Y          36

#define TPOS_GINFO_X          400  // general information
#define TPOS_GINFO_Y          240

#define TPOS_ENERGY_X         158 // weapon energy level
#define TPOS_ENERGY_Y         6

// player defines
#define CROSS_START_X         0
#define CROSS_START_Y         0
#define CROSS_WIDTH           56
#define CROSS_HEIGHT          56

// font display stuff
#define FONT_HPITCH           12  // space between chars
#define FONT_VPITCH           14  // space between lines

// difficulty stuff
#define DIFF_RATE             (float)(.001f)  // rate to advance  difficulty per frame
#define DIFF_PMAX             75

// explosion stuff
#define NUM_EXPLOSIONS        16

// TYPES ///////////////////////////////////////////////////

// this a 3D star
typedef struct STAR3D_TYP
{
	USHORT color;     // color of point 16-bit
	float x, y, z;      // coordinates of point in 3D
} STAR3D, *STAR3D_PTR;

// ALIEN fighter object
typedef struct ALIEN_TYP
{
	int      state;  // state of this ALIEN fighter
	int      count;  // generic counter
	int      aux;    // generic auxialiary var
	POINT3D  pos;    // position of ALIEN fighter
	VECTOR3D vel;    // velocity of ALIEN fighter
	VECTOR3D rot;    // rotational velocities
	VECTOR3D ang;    // current orientation
} ALIEN, *ALIEN_PTR;

// GLOBALS /////////////////////////////////////////////////

char buffer[2048];                        // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,0,0,1 };
POINT4D  cam_target = { 0,0,0,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

// all your initialization code goes here...
VECTOR4D vscale = { 1.0,1.0,1.0,1 },
vpos = { 0,0,0,1 },
vrot = { 0,0,0,1 };

CAM4DV1        cam;       // the single camera
RENDERLIST4DV2 rend_list; // the render list

// color constants for line drawing
USHORT rgb_green,
rgb_white,
rgb_red,
rgb_blue;

// colors for lights
RGBAV1 white, light_gray, gray, black, red, green, blue;

// music and sound stuff

// star globals
STAR3D stars[NUM_STARS]; // the starfield

// player and game globals
int player_z_vel = 4; // virtual speed of viewpoint/ship

float cross_x = 0, // cross hairs
cross_y = 0;

int cross_x_screen = screen_width / 2,   // used for cross hair
cross_y_screen = screen_height / 2,
target_x_screen = screen_width / 2,   // used for targeter
target_y_screen = screen_height / 2;

int escaped = 0;   // tracks number of missed ships
int hits = 0;   // tracks number of hits
int score = 0;   // take a guess :)
int weapon_energy = 100; // weapon energy
int weapon_active = 0;   // tracks if weapons are energized

int game_state = GAME_STATE_INIT; // state of game
int game_state_count1 = 0;               // general counters
int game_state_count2 = 0;
int restart_state = 0;               // state when game is restarting
int restart_state_count1 = 0;               // general counter

// diffiuculty
float  diff_level = 1;                   // difficulty level used in velocity and probability calcs

// alien globals
ALIEN      aliens[NUM_ALIENS];   // the ALIEN fighters
OBJECT4DV2 obj_alien;            // the master object

// explosions
OBJECT4DV2 explobj[NUM_EXPLOSIONS]; // the explosions

// game imagery assets
BOB cockpit;               // the cockpit image
BOB starscape;             // the background starscape
BOB tech_font;             // the bitmap font for the engine
BOB crosshair;             // the player's cross hair

// these are the positions of the energy binding on the main lower control panel
POINT2D energy_bindings[6] = { {342, 527}, {459, 527},
							   {343, 534}, {458, 534},
							   {343, 540}, {458, 540} };

// these hold the positions of the weapon burst which use lighting too
// the starting points are known, but the end points are computed on the fly
// based on the cross hair
POINT2D weapon_bursts[4] = { {78, 500}, {0,0},     // left energy weapon
							 {720, 500}, {0,0} };  // right energy weapon

int px = 0, py = 0, pz = 500; // debug stuff for object tracking

// FUNCTIONS //////////////////////////////////////////////

void Render_Energy_Bindings(POINT2D_PTR bindings, // array containing binding positions in the form
	// start point, end point, start point, end point...
	int num_bindings,     // number of energy bindings to render 1-3
	int num_segments,     // number of segments to randomize bindings into
	int amplitude,        // amplitude of energy binding
	int color,            // color of bindings
	UCHAR *video_buffer,  // video buffer to render
	int lpitch)           // memory pitch of buffer
{
	// this functions renders the energy bindings across the main exterior energy
	// transmission emitters :) Basically, the function takes two points in 2d then
	// it anchors a line at the two ends and randomly walks from end point to the
	// other by breaking the line into segments and then randomly modulating the y position
	// and amount amplitude +-, maximum number of segments 16
	POINT2D segments[17]; // to hold constructed segments after construction

	// render each binding
	for (int index = 0; index < num_bindings; index++) {
		// store starting and ending positions

		// starting position
		segments[0] = bindings[index * 2];

		// ending position
		segments[num_segments] = bindings[index * 2 + 1];

		// compute vertical gradient, so if y positions of endpoints are
		// greatly different bindings will be modulated using the straight line
		// as a basis
		float dyds = (segments[num_segments].y - segments[0].y) / (float)num_segments;
		float dxds = (segments[num_segments].x - segments[0].x) / (float)num_segments;

		// now build up segments
		for (int sindex = 1; sindex < num_segments; sindex++) {
			segments[sindex].x = segments[sindex - 1].x + dxds;
			segments[sindex].y = segments[0].y + sindex * dyds + RAND_RANGE(-amplitude, amplitude);
		} // end for segment            

		// draw binding
		for (int sindex = 0; sindex < num_segments; sindex++)
			Draw_Line16(segments[sindex].x, segments[sindex].y,
				segments[sindex + 1].x, segments[sindex + 1].y,
				color, video_buffer, lpitch);

	} // end for index

} // end Render_Energy_Binding

////////////////////////////////////////////////////////////////////

void Render_Weapon_Bursts(POINT2D_PTR burstpoints, // array containing energy burst positions in the form
	// start point, end point, start point, end point...
	int num_bursts,
	int num_segments,     // number of segments to randomize bindings into
	int amplitude,        // amplitude of energy binding
	int color,            // color of bindings
	UCHAR *video_buffer,  // video buffer to render
	int lpitch)           // memory pitch of buffer
{
	// this functions renders the weapon energy bursts from the weapon emitter or wherever
	// function derived from Render_Energy_Binding, but generalized 
	// Basically, the function takes two points in 2d then
	// it anchors a line at the two ends and randomly walks from end point to the
	// other by breaking the line into segments and then randomly modulating the x,y position
	// and amount amplitude +-, maximum number of segments 16

	POINT2D segments[17]; // to hold constructed segments after construction

	// render each energy burst
	for (int index = 0; index < num_bursts; index++) {
		// store starting and ending positions

		// starting position
		segments[0] = burstpoints[index * 2];

		// ending position
		segments[num_segments] = burstpoints[index * 2 + 1];

		// compute horizontal/vertical gradients, so we can modulate the lines 
		// on the proper trajectory
		float dyds = (segments[num_segments].y - segments[0].y) / (float)num_segments;
		float dxds = (segments[num_segments].x - segments[0].x) / (float)num_segments;

		// now build up segments
		for (int sindex = 1; sindex < num_segments; sindex++) {
			segments[sindex].x = segments[0].x + sindex * dxds + RAND_RANGE(-amplitude, amplitude);
			segments[sindex].y = segments[0].y + sindex * dyds + RAND_RANGE(-amplitude, amplitude);
		} // end for segment            

		// draw binding
		for (int sindex = 0; sindex < num_segments; sindex++)
			Draw_Line16(segments[sindex].x, segments[sindex].y,
				segments[sindex + 1].x, segments[sindex + 1].y,
				color, video_buffer, lpitch);

	} // end for index

} // end Render_Weapons_Bursts

///////////////////////////////////////////////////////////////

int Load_Bitmap_Font(const char *fontfile, BOB_PTR font)
{
	// this is a semi generic font loader...
	// expects the file name of a font in a template that is 
	// 4 rows of 16 cells, each character is 16x14, and cell 0 is the space character
	// characters from 32 to 95, " " to "-", suffice for 90% of text work

	// load the font bitmap template
	if (!Load_Bitmap_File(&bitmap16bit, fontfile))
		return(0);

	// create the bob that will hold the font, use a bob for speed, we can use the 
	// hardware blitter
	Create_BOB(font, 0, 0, 16, 14, 64, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);

	// load all the frames
	for (int index = 0; index < 64; index++)
		Load_Frame_BOB16(font, &bitmap16bit, index, index % 16, index / 16, BITMAP_EXTRACT_MODE_CELL);

	// unload the bitmap file
	Unload_Bitmap_File(&bitmap16bit);

	// return success
	return(1);

} // end Load_Bitmap_Font

///////////////////////////////////////////////////////////////

int Draw_Bitmap_Font_String(BOB_PTR font,  // pointer to bob containing font
	int x, int y,  // screen position to render
	char *string,  // string to render
	int hpitch, int vpitch, // horizontal and vertical pitch
	UCHAR *dest, int mempitch) // destination surface
{
	// this function draws a string based on a 64 character font sent in as a bob
	// the string will be drawn at the given x,y position with intercharacter spacing
	// if hpitch and a interline spacing of vpitch

	// are things semi valid?
	if (!string || !dest)
		return(0);

	// loop and render
	for (unsigned int index = 0; index < strlen(string); index++) {
		// set the position and character
		font->x = x;
		font->y = y;
		font->curr_frame = string[index] - 32;
		// test for overflow set to space
		if (font->curr_frame > 63 || font->curr_frame < 0) font->curr_frame = 0;

		// render character (i hate making a function call!)
		Draw_BOB16(font, dest, mempitch);

		// move position
		x += hpitch;

	} // end for index

	// return success
	return(1);

} // end Draw_Bitmap_Font_String

//////////////////////////////////////////////////////////

void Draw_Mesh_Explosions(void)
{
	// this function draws the mesh explosions
	MATRIX4X4 mrot; // used to transform objects

	// draw the explosions, note we do NOT cull them
	for (int eindex = 0; eindex < NUM_EXPLOSIONS; eindex++) {
		// is the mesh explosions active
		if ((explobj[eindex].state & OBJECT4DV2_STATE_ACTIVE)) {
			// reset the object
			Reset_OBJECT4DV2(&explobj[eindex]);

			// generate rotation matrix 
			Build_XYZ_Rotation_MATRIX4X4(0, 0, 0, &mrot);

			// rotate the local coords of the object
			Transform_OBJECT4DV2(&explobj[eindex], &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

			// perform world transform
			Model_To_World_OBJECT4DV2(&explobj[eindex], TRANSFORM_TRANS_ONLY);

			// insert the object into render list
			Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, &explobj[eindex], 0);

			// now animate the mesh
			VECTOR4D_PTR trajectory = (VECTOR4D_PTR)explobj[eindex].ivar1;

			for (int pindex = 0; pindex < explobj[eindex].num_polys; pindex++) {
				// vertex 0
				VECTOR4D_Add(&explobj[eindex].vlist_local[pindex * 3 + 0].v,
					&trajectory[pindex * 2 + 0],
					&explobj[eindex].vlist_local[pindex * 3 + 0].v);

				// vertex 1
				VECTOR4D_Add(&explobj[eindex].vlist_local[pindex * 3 + 1].v,
					&trajectory[pindex * 2 + 0],
					&explobj[eindex].vlist_local[pindex * 3 + 1].v);

				// vertex 2
				VECTOR4D_Add(&explobj[eindex].vlist_local[pindex * 3 + 2].v,
					&trajectory[pindex * 2 + 0],
					&explobj[eindex].vlist_local[pindex * 3 + 2].v);

				// modulate color
				//explobj[eindex].plist[pindex].color = RGB16Bit(rand()%255,0,0);

			} // end for pindex

			// update counter, test for terminate
			if (--explobj[eindex].ivar2 < 0) {
				// reset this object
				explobj[eindex].state = OBJECT4DV2_STATE_NULL;

				// release memory of object, but only data that isn't linked to master object
				// local vertex list
				if (explobj[eindex].head_vlist_local)
					free(explobj[eindex].head_vlist_local);

				// transformed vertex list
				if (explobj[eindex].head_vlist_trans)
					free(explobj[eindex].head_vlist_trans);

				// polygon list
				if (explobj[eindex].plist)
					free(explobj[eindex].plist);

				// trajectory list
				if ((VECTOR4D_PTR)(explobj[eindex].ivar1))
					free((VECTOR4D_PTR)explobj[eindex].ivar1);

				// now clear out object completely
				memset((void *)&explobj[eindex], 0, sizeof(OBJECT4DV2));

			} // end if 

		} // end if  

	} // end for eindex

} // end Draw_Mesh_Explosions

//////////////////////////////////////////////////////////

int Start_Mesh_Explosion(OBJECT4DV2_PTR obj,  // object to destroy
	MATRIX4X4_PTR mrot, // initial orientation of object
	int detail,          // the detail level,1 highest detail
	float rate,          // velocity of explosion shrapnel
	int lifetime)        // total lifetime of explosion
{
	// this function "blows up" an object, it takes a pointer to the object to
	// be destroyed, the detail level of the polyogonal explosion, 1 is high detail, 
	// numbers greater than 1 are lower detail, the detail selects the polygon extraction
	// stepping, hence a detail of 5 would mean every 5th polygon in the original mesh
	// should be part of the explosion, on average no more than 10-50% of the polygons in 
	// the original mesh should be used for the explosion; some would be vaporized and
	// in a more practical sense, when an object is far, there's no need to have detail
	// the next parameter is the rate which is used as a general scale for the explosion
	// velocity, and finally lifetime which is the number of cycles to display the explosion

	// the function works by taking the original object then copying the core information
	// except for the polygon and vertex lists, the function operates by selecting polygon 
	// by polygon to be destroyed and then makes a copy of the polygon AND all of it's vertices,
	// thus the vertex coherence is completely lost, this is a necessity since each polygon must
	// be animated separately by the engine, thus they can't share vertices, additionally at the
	// end if the vertex list will be the velocity and rotation information for each polygon
	// this is hidden to the rest of the engine, now for the explosion, the center of the object
	// is used as the point of origin then a ray is drawn thru each polygon, then each polygon
	// is thrown at some velocity with a small rotation rate
	// finally the system can only have so many explosions at one time

	// step: 1 find an available explosion
	for (int eindex = 0; eindex < NUM_EXPLOSIONS; eindex++) {
		// is this explosion available?
		if (explobj[eindex].state == OBJECT4DV2_STATE_NULL) {
			// copy the object, including the pointers which we will unlink shortly...
			explobj[eindex] = *obj;

			explobj[eindex].state = OBJECT4DV2_STATE_ACTIVE | OBJECT4DV2_STATE_VISIBLE;

			// recompute a few things
			explobj[eindex].num_polys = obj->num_polys / detail;
			explobj[eindex].num_vertices = 3 * obj->num_polys;
			explobj[eindex].total_vertices = 3 * obj->num_polys; // one frame only

			// unlink/re-allocate all the pointers except the texture coordinates, we can use those, they don't
			// depend on vertex coherence
			// allocate memory for vertex lists
			if (!(explobj[eindex].vlist_local = (VERTEX4DTV1_PTR)malloc(sizeof(VERTEX4DTV1) * explobj[eindex].num_vertices)))
				return(0);

			// clear data
			memset((void *)explobj[eindex].vlist_local, 0, sizeof(VERTEX4DTV1) * explobj[eindex].num_vertices);

			if (!(explobj[eindex].vlist_trans = (VERTEX4DTV1_PTR)malloc(sizeof(VERTEX4DTV1) * explobj[eindex].num_vertices)))
				return(0);

			// clear data
			memset((void *)explobj[eindex].vlist_trans, 0, sizeof(VERTEX4DTV1) * explobj[eindex].num_vertices);

			// allocate memory for polygon list
			if (!(explobj[eindex].plist = (POLY4DV2_PTR)malloc(sizeof(POLY4DV2) * explobj[eindex].num_polys)))
				return(0);

			// clear data
			memset((void *)explobj[eindex].plist, 0, sizeof(POLY4DV2) * explobj[eindex].num_polys);

			// now, we need somewhere to store the vector trajectories of the polygons and their rotational rates
			// so let's allocate an array of VECTOR4D elements to hold this information and then store it
			// in ivar1, therby using ivar as a pointer, this is perfectly fine :)
			// each record will consist of a velocity and a rotational rate in x,y,z, so V0,R0, V1,R1,...Vn-1, Rn-1
			// allocate memory for polygon list
			if (!(explobj[eindex].ivar1 = (long)malloc(sizeof(VECTOR4D) * 2 * explobj[eindex].num_polys)))
				return(0);

			// clear data
			memset((void *)explobj[eindex].ivar1, 0, sizeof(VECTOR4D) * 2 * explobj[eindex].num_polys);

			// alias working pointer
			VECTOR4D_PTR trajectory = (VECTOR4D_PTR)explobj[eindex].ivar1;

			// these are needed to track the "head" of the vertex list for multi-frame objects
			explobj[eindex].head_vlist_local = explobj[eindex].vlist_local;
			explobj[eindex].head_vlist_trans = explobj[eindex].vlist_trans;

			// set the lifetime in ivar2
			explobj[eindex].ivar2 = lifetime;

			// now comes the tricky part, loop and copy each polygon, but as we copy the polygons, we have to copy
			// the vertices, and insert them, and fix up the vertice indices etc.
			for (int pindex = 0; pindex < explobj[eindex].num_polys; pindex++) {
				// alright, we want to copy the (pindex*detail)th poly from the master to the (pindex)th
				// polygon in explosion
				explobj[eindex].plist[pindex] = obj->plist[pindex * detail];

				// we need to modify, the vertex list pointer and the vertex indices themselves
				explobj[eindex].plist[pindex].vlist = explobj[eindex].vlist_local;

				// now comes the hard part, we need to first copy the vertices from the original mesh
				// into the new vertex storage, but at the same time keep the same vertex ordering

				// vertex 0
				explobj[eindex].plist[pindex].vert[0] = pindex * 3 + 0;
				explobj[eindex].vlist_local[pindex * 3 + 0] = obj->vlist_local[obj->plist[pindex * detail].vert[0]];

				// vertex 1
				explobj[eindex].plist[pindex].vert[1] = pindex * 3 + 1;
				explobj[eindex].vlist_local[pindex * 3 + 1] = obj->vlist_local[obj->plist[pindex * detail].vert[1]];

				// vertex 2
				explobj[eindex].plist[pindex].vert[2] = pindex * 3 + 2;
				explobj[eindex].vlist_local[pindex * 3 + 2] = obj->vlist_local[obj->plist[pindex * detail].vert[2]];

				// reset shading flag to constant for some of the shrapnel since they are giving off light 
				if ((rand() % 5) == 1) {
					SET_BIT(explobj[eindex].plist[pindex].attr, POLY4DV2_ATTR_SHADE_MODE_PURE);
					RESET_BIT(explobj[eindex].plist[pindex].attr, POLY4DV2_ATTR_SHADE_MODE_GOURAUD);
					RESET_BIT(explobj[eindex].plist[pindex].attr, POLY4DV2_ATTR_SHADE_MODE_FLAT);

					// set color
					explobj[eindex].plist[pindex].color = RGB16Bit(128 + rand() % 128, 0, 0);

				} // end if

				// add some random noise to trajectory
				static VECTOR4D mv;

				// first compute trajectory as vector from center to vertex piercing polygon
				VECTOR4D_INIT(&trajectory[pindex * 2 + 0], &obj->vlist_local[obj->plist[pindex * detail].vert[0]].v);
				VECTOR4D_Scale(rate, &trajectory[pindex * 2 + 0], &trajectory[pindex * 2 + 0]);

				mv.x = RAND_RANGE(-10, 10);
				mv.y = RAND_RANGE(-10, 10);
				mv.z = RAND_RANGE(-10, 10);
				mv.w = 1;

				VECTOR4D_Add(&mv, &trajectory[pindex * 2 + 0], &trajectory[pindex * 2 + 0]);

				// now rotation rate, this is difficult to do since we don't have the center of the polygon
				// maybe later...
				// trajectory[pindex*2+1] = 

			} // end for pindex

			// now transform final mesh into position destructively
			Transform_OBJECT4DV2(&explobj[eindex], mrot, TRANSFORM_LOCAL_ONLY, 1);

			return(1);

		} // end if found a free explosion

	} // end for eindex

	// return failure
	return(0);

} // end Start_Mesh_Explosion

/////////////////////////////////////////////////////////

int Start_Alien(void)
{
	// this function hunts in the alien list, finds an available alien and 
	// starts it up

	for (int index = 0; index < NUM_ALIENS; index++) {
		// is this alien available?
		if (aliens[index].state == ALIEN_STATE_DEAD) {
			// clear any residual data
			memset((void *)&aliens[index], 0, sizeof(ALIEN));

			// start alien up
			aliens[index].state = ALIEN_STATE_ALIVE;

			// select random position in bounding volume
			aliens[index].pos.x = RAND_RANGE(-1000, 1000);
			aliens[index].pos.y = RAND_RANGE(-1000, 1000);
			aliens[index].pos.z = 20000;

			// select velocity based on difficulty level
			aliens[index].vel.x = RAND_RANGE(-10, 10);
			aliens[index].vel.y = RAND_RANGE(-10, 10);
			aliens[index].vel.z = -(10 * diff_level + rand() % 200);

			// set rotation rate for z axis only
			aliens[index].rot.z = RAND_RANGE(-5, 5);

			// return the index
			return(index);
		} // end if   

	} // end for index

	// failure
	return(-1);

} // end Start_Alien

/////////////////////////////////////////////////////////

void Init_Aliens(void)
{
	// initializes all the ALIEN fighters to a known state
	for (int index = 0; index < NUM_ALIENS; index++) {
		// zero ALIEN fighter out
		memset((void *)&aliens[index], 0, sizeof(ALIEN));

		// set any other specific info now...
		aliens[index].state = ALIEN_STATE_DEAD;

	} // end for

} // end Init_Aliens

/////////////////////////////////////////////////////////

void Draw_Aliens(void)
{
	// this function draws all the active ALIEN fighters

	MATRIX4X4 mrot; // used to transform objects

	for (int index = 0; index < NUM_ALIENS; index++) {
		// which state is alien in
		switch (aliens[index].state) {
			// is the alien dead? if so move on
		case ALIEN_STATE_DEAD: break;

			// is the alien alive?
		case ALIEN_STATE_ALIVE:
		case ALIEN_STATE_DYING:
		{
			// reset the object (this only matters for backface and object removal)
			Reset_OBJECT4DV2(&obj_alien);

			// generate rotation matrix around y axis
			Build_XYZ_Rotation_MATRIX4X4(aliens[index].ang.x, aliens[index].ang.y, aliens[index].ang.z, &mrot);

			// rotate the local coords of the object
			Transform_OBJECT4DV2(&obj_alien, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

			// set position of constant shaded cube
			obj_alien.world_pos.x = aliens[index].pos.x;
			obj_alien.world_pos.y = aliens[index].pos.y;
			obj_alien.world_pos.z = aliens[index].pos.z;

			// attempt to cull object
			if (!Cull_OBJECT4DV2(&obj_alien, &cam, CULL_OBJECT_XYZ_PLANES)) {
				// perform world transform
				Model_To_World_OBJECT4DV2(&obj_alien, TRANSFORM_TRANS_ONLY);

				// insert the object into render list
				Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, &obj_alien, 0);
			} // end if 

			// ok, now test for collision with energy bursts, strategy is as follows
			// we will project the bounding box of the object into screen space to coincide with
			// the energy burst on the screen, if there is an overlap then the target is hit
			// simply need to do a few transforms, this kinda sucks that we are going to do this work
			// 2x, but the problem is that the graphics pipeline knows nothing about collision etc.,
			// so we can't "get to" the information me want since we are ripping the objects apart
			// and passing them down the pipeline for a later world to camera transform, thus you
			// may want to think about this problem when making your own game, how tight to couple
			// collision and the engine, HOWEVER, the only reason we are having a problem at all is that
			// we want to use the screen coords of the energy burst this is fine, but a more effective method
			// would be to compute the 3D world coords of where the energy burst is firing and then project
			// that parallelpiped in 3D space to see where the player is trying to fire, this is a classic
			// problem with screen picking, hence, in the engine, when an object is rendered sometimes its 
			// a good idea to track somekind of collision boundary in screen coords that can be used later
			// for "object picking" and collision, anyway, let's do it the easy way, but the long way....

			// first is the player firing weapon?
			if (weapon_active) {
				// we need 4 transforms total, first we need all our points in world coords,
				// then we need camera coords, then perspective coords, then screen coords

				// we need to transform 2 points: the center and a point lying on the surface of the  
				// bounding sphere, as long as the 

				POINT4D pbox[4], // bounding box coordinates, center points, surrounding object
					// denoted by X's, we need to project these to screen coords
					// ....X.... 
					// . |   | .
					// X |-O-| X
					// . |   | . 
					// ....X....
					// we will use the average radius as the distance to each X from the center

					presult; // used to hold temp results

				// world to camera transform

				// transform center point only
				Mat_Mul_VECTOR4D_4X4(&obj_alien.world_pos, &cam.mcam, &presult);

				// result holds center of object in camera coords now
				// now we are in camera coords, aligned to z-axis, compute radial point axis aligned
				// bounding box points

				// x+r, y, z
				pbox[0].x = presult.x + obj_alien.avg_radius[obj_alien.curr_frame];
				pbox[0].y = presult.y;
				pbox[0].z = presult.z;
				pbox[0].w = 1;

				// x-r, y, z
				pbox[1].x = presult.x - obj_alien.avg_radius[obj_alien.curr_frame];
				pbox[1].y = presult.y;
				pbox[1].z = presult.z;
				pbox[1].w = 1;

				// x, y+r, z
				pbox[2].x = presult.x;
				pbox[2].y = presult.y + obj_alien.avg_radius[obj_alien.curr_frame];
				pbox[2].z = presult.z;
				pbox[2].w = 1;

				// x, y-r, z
				pbox[3].x = presult.x;
				pbox[3].y = presult.y - obj_alien.avg_radius[obj_alien.curr_frame];
				pbox[3].z = presult.z;
				pbox[3].w = 1;

				// now we are ready to project the points to screen space
				float alpha = (0.5 * cam.viewport_width - 0.5);
				float beta = (0.5 * cam.viewport_height - 0.5);

				// loop and process each point
				for (int bindex = 0; bindex < 4; bindex++) {
					float z = pbox[bindex].z;

					// perspective transform first
					pbox[bindex].x = cam.view_dist * pbox[bindex].x / z;
					pbox[bindex].y = cam.view_dist * pbox[bindex].y * cam.aspect_ratio / z;
					// z = z, so no change

					// screen projection
					pbox[bindex].x = alpha * pbox[bindex].x + alpha;
					pbox[bindex].y = -beta * pbox[bindex].y + beta;

				} // end for bindex

#ifdef DEBUG_ON
				// now we have the 4 points is screen coords and we can test them!!! ya!!!!
				Draw_Clip_Line16(pbox[0].x, pbox[2].y, pbox[1].x, pbox[2].y, rgb_red, framebuffer, framebuffer_pitch);
				Draw_Clip_Line16(pbox[0].x, pbox[3].y, pbox[1].x, pbox[3].y, rgb_red, framebuffer, framebuffer_pitch);

				Draw_Clip_Line16(pbox[0].x, pbox[2].y, pbox[0].x, pbox[3].y, rgb_red, framebuffer, framebuffer_pitch);
				Draw_Clip_Line16(pbox[1].x, pbox[2].y, pbox[1].x, pbox[3].y, rgb_red, framebuffer, framebuffer_pitch);
#endif

				// test for collision
				if ((cross_x_screen > pbox[1].x) && (cross_x_screen < pbox[0].x) &&
					(cross_y_screen > pbox[2].y) && (cross_y_screen < pbox[3].y)) {
					Start_Mesh_Explosion(&obj_alien, // object to blow
						&mrot,      // initial orientation
						3,          // the detail level,1 highest detail
						.5,         // velocity of explosion shrapnel
						100);       // total lifetime of explosion         

					// remove from simulation
					aliens[index].state = ALIEN_STATE_DEAD;

					// increment hits
					hits++;

					// take into consideration the z, the speed, the level, blah blah
					score += (int)((diff_level * 10 + obj_alien.world_pos.z / 10));

				} // end if

			} // end if weapon active

		} break;

		default: break;

		} // end switch

	} // end for index

// debug code

#ifdef DEBUG_ON
	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV2(&obj_alien);
	static int ang_y = 0;
	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(0, ang_y++, 0, &mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV2(&obj_alien, &mrot, TRANSFORM_LOCAL_TO_TRANS, 1);

	// set position of constant shaded cube
	obj_alien.world_pos.x = px;
	obj_alien.world_pos.y = py;
	obj_alien.world_pos.z = pz;

	// perform world transform
	Model_To_World_OBJECT4DV2(&obj_alien, TRANSFORM_TRANS_ONLY);

	// insert the object into render list
	Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, &obj_alien, 0);
#endif

} // end Draw_Aliens

///////////////////////////////////////////////////////////

void Process_Aliens(void)
{
	// this function moves the aliens and performs AI

	for (int index = 0; index < NUM_ALIENS; index++) {
		// which state is alien in
		switch (aliens[index].state) {
			// is the alien dead? if so move on
		case ALIEN_STATE_DEAD: break;

			// is the alien alive?
		case ALIEN_STATE_ALIVE:
		case ALIEN_STATE_DYING:
		{
			// move the alien
			aliens[index].pos.x += aliens[index].vel.y;
			aliens[index].pos.y += aliens[index].vel.y;
			aliens[index].pos.z += aliens[index].vel.z;

			// rotate the alien
			if ((aliens[index].ang.x += aliens[index].rot.x) > 360)
				aliens[index].ang.x = 0;

			if ((aliens[index].ang.y += aliens[index].rot.y) > 360)
				aliens[index].ang.y = 0;

			if ((aliens[index].ang.z += aliens[index].rot.z) > 360)
				aliens[index].ang.z = 0;

			// test if alien has past players near clipping plane
			// if so remove from simulation
			if (aliens[index].pos.z < cam.near_clip_z) {
				// remove from simulation
				aliens[index].state = ALIEN_STATE_DEAD;

				// another one gets away!
				escaped++;
			} // end if 

		} break;

		default: break;

		} // end switch

	} // end for index

} // end Process_Aliens

//////////////////////////////////////////////////////////

void Init_Starfield(void)
{
	// create the starfield
	for (int index = 0; index < NUM_STARS; index++) {
		// randomly position stars in an elongated cylinder stretching from
		// the viewpoint 0,0,-d to the yon clipping plane 0,0,far_z
		stars[index].x = -screen_width / 2 + rand() % screen_width;
		stars[index].y = -screen_height / 2 + rand() % screen_height;
		stars[index].z = NEAR_Z + rand() % (FAR_Z - NEAR_Z);

		// set color of stars
		stars[index].color = rgb_white;
	} // end for index

} // end Init_Starfield

//////////////////////////////////////////////////////////

void Move_Starfield(void)
{
	// move the stars

	int index; // looping var

	// the stars are technically stationary,but we are going
	// to move them to simulate motion of the viewpoint
	for (index = 0; index < NUM_STARS; index++) {
		// move the next star
		stars[index].z -= (player_z_vel + 3 * diff_level);

		// test for past near clipping plane
		if (stars[index].z <= NEAR_Z)
			stars[index].z = FAR_Z;

	} // end for index

} // end Move_Starfield

/////////////////////////////////////////////////////////

void Draw_Starfield(UCHAR *video_buffer, int lpitch)
{
	// draw the stars in 3D using perspective transform

	int index; // looping var

	// convert pitch to words
	lpitch >>= 1;

	// draw each star
	for (index = 0; index < NUM_STARS; index++) {
		// draw the next star
		// step 1: perspective transform
		float x_per = VIEW_DISTANCE * stars[index].x / stars[index].z;
		float y_per = VIEW_DISTANCE * stars[index].y / stars[index].z;

		// step 2: compute screen coords
		int x_screen = screen_width / 2 + x_per;
		int y_screen = screen_height / 2 - y_per;

		// clip to screen coords
		if (x_screen >= screen_width || x_screen < 0 ||
			y_screen >= screen_height || y_screen < 0) {
			// continue to next star
			continue;
		} // end if
		else {
			// else render to buffer

			int i = ((float)(10000 * NEAR_Z) / stars[index].z);
			if (i > 255) i = 255;

			((USHORT *)video_buffer)[x_screen + y_screen * lpitch] = RGB16Bit(i, i, i);
		} // end else

	} // end for index

}  // Draw_Starfield

//////////////////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	// state variables for different rendering modes and help
	static int wireframe_mode = 0;
	static int backface_mode = 1;
	static int lighting_mode = 1;
	static int zsort_mode = 1;

	char work_string[256]; // temp string

	// what state is game in?
	switch (game_state) {
	case GAME_STATE_INIT:
	{
		// load the font
		Load_Bitmap_Font("assets/tech_char_set_01.bmp", &tech_font);

		// transition to un state
		game_state = GAME_STATE_RESTART;
	} break;

	case GAME_STATE_RESTART:
	{
		// initialize the stars
		Init_Starfield();

		// initialize all the aliens
		Init_Aliens();

		// reset all vars
		player_z_vel = 4; // virtual speed of viewpoint/ship

		cross_x = CROSS_START_X; // cross hairs
		cross_y = CROSS_START_Y;

		cross_x_screen = CROSS_START_X;   // used for cross hair
		cross_y_screen = CROSS_START_Y;
		target_x_screen = CROSS_START_X;   // used for targeter
		target_y_screen = CROSS_START_Y;

		escaped = 0;   // tracks number of missed ships
		hits = 0;   // tracks number of hits
		score = 0;   // take a guess :)
		weapon_energy = 100; // weapon energy
		weapon_active = 0;   // weapons are off

		game_state_count1 = 0;    // general counters
		game_state_count2 = 0;
		restart_state = 0;    // state when game is restarting
		restart_state_count1 = 0;    // general counter

		// difficulty
		diff_level = 1;

		// transition to run state
		game_state = GAME_STATE_RUN;

	} break;

	case GAME_STATE_RUN:
	case GAME_STATE_OVER: // keep running sim, but kill diff_level, and player input
	{
		// start the clock
		Start_Clock();

		// draw background
		RECT src_rect = { 0, 0, starscape.width, starscape.height }, // the source rectangle
			dest_rect = { 0, 0, screen_width, screen_height }; // the destination rectangle
		Blit_Rect16(src_rect, starscape.images[starscape.curr_frame], starscape.width * 2, dest_rect, framebuffer, framebuffer_pitch);

		// reset the render list
		Reset_RENDERLIST4DV2(&rend_list);

		// read keyboard and other devices here
		DInput_Read_Mouse();

		// game logic begins here...

#ifdef DEBUG_ON      
		if (RETRO_KeyState(SDL_SCANCODE_UP)) {
			py += 10;

		} // end if

		if (RETRO_KeyState(SDL_SCANCODE_DOWN)) {
			py -= 10;
		} // end if

		if (RETRO_KeyState(SDL_SCANCODE_RIGHT)) {
			px += 10;
		} // end if

		if (RETRO_KeyState(SDL_SCANCODE_LEFT)) {
			px -= 10;
		} // end if

		if (RETRO_KeyState(SDL_SCANCODE_1)) {
			pz += 20;

		} // end if

		if (RETRO_KeyState(SDL_SCANCODE_2)) {
			pz -= 20;

		} // end if
#endif

		// modes and lights

		// wireframe mode
		if (RETRO_KeyPressed(SDL_SCANCODE_W)) {
			// toggle wireframe mode
			if (++wireframe_mode > 1)
				wireframe_mode = 0;
		} // end if

		// change nebula
		if (RETRO_KeyPressed(SDL_SCANCODE_N)) {
			if (++starscape.curr_frame >= starscape.num_frames)
				starscape.curr_frame = 0;
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
			if (lights[SPOT_LIGHT2_INDEX].state == LIGHTV1_STATE_ON)
				lights[SPOT_LIGHT2_INDEX].state = LIGHTV1_STATE_OFF;
			else
				lights[SPOT_LIGHT2_INDEX].state = LIGHTV1_STATE_ON;
		} // end if

		// z-sorting
		if (RETRO_KeyPressed(SDL_SCANCODE_Z)) {
			// toggle z sorting
			zsort_mode = -zsort_mode;
		} // end if

		// track cross hair
		cross_x += mouse_state.lX;
		cross_y += mouse_state.lY;

		// check bounds (x,y) are in 2D space coords, not screen coords
		if (cross_x >= (screen_width / 2 - CROSS_WIDTH / 2))
			cross_x = (screen_width / 2 - CROSS_WIDTH / 2) - 1;
		else
			if (cross_x <= -(screen_width / 2 - CROSS_WIDTH / 2))
				cross_x = -(screen_width / 2 - CROSS_WIDTH / 2) + 1;

		if (cross_y >= (screen_height / 2 - CROSS_HEIGHT / 2))
			cross_y = (screen_height / 2 - CROSS_HEIGHT / 2) - 1;
		else
			if (cross_y <= -(screen_height / 2 - CROSS_HEIGHT / 2))
				cross_y = -(screen_height / 2 - CROSS_HEIGHT / 2) + 1;

		// player is done moving create camera matrix //////////////////////////
		Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

		// perform all non-player game AI and motion here /////////////////////

		// move starfield
		Move_Starfield();

		// move aliens
		Process_Aliens();

		// update difficulty of game
		if ((diff_level += DIFF_RATE) > DIFF_PMAX)
			diff_level = DIFF_PMAX;

		// start a random alien as a function of game difficulty
		if ((rand() % (DIFF_PMAX - (int)diff_level + 2)) == 1)
			Start_Alien();

		// perform animation and transforms on lights //////////////////////////

		// draw all 3D entities
		Draw_Aliens();

		// draw mesh explosions
		Draw_Mesh_Explosions();

		// entire into final 3D pipeline /////////////////////////////////////// 

		// remove backfaces
		if (backface_mode == 1)
			Remove_Backfaces_RENDERLIST4DV2(&rend_list, &cam);

		// light scene all at once 
		if (lighting_mode == 1)
			Light_RENDERLIST4DV2_World16(&rend_list, &cam, lights, 4);

		// apply world to camera transform
		World_To_Camera_RENDERLIST4DV2(&rend_list, &cam);

		// sort the polygon list (hurry up!)
		if (zsort_mode == 1)
			Sort_RENDERLIST4DV2(&rend_list, SORT_POLYLIST_AVGZ);

		// apply camera to perspective transformation
		Camera_To_Perspective_RENDERLIST4DV2(&rend_list, &cam);

		// apply screen transform
		Perspective_To_Screen_RENDERLIST4DV2(&rend_list, &cam);

		// draw the starfield now
		Draw_Starfield(framebuffer, framebuffer_pitch);

		// reset number of polys rendered
		debug_polys_rendered_per_frame = 0;

		// render the list
		if (wireframe_mode == 0) {
			Draw_RENDERLIST4DV2_Solid16(&rend_list, framebuffer, framebuffer_pitch);
		} else
			if (wireframe_mode == 1) {
				Draw_RENDERLIST4DV2_Wire16(&rend_list, framebuffer, framebuffer_pitch);
			} // end if

		 // draw energy bindings
		Render_Energy_Bindings(energy_bindings, 1, 8, 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

		// fire the weapon
		if (game_state == GAME_STATE_RUN && (mouse_state.rgbButtons[0] || mouse_state.rgbButtons[2]) && weapon_energy > 20) {
			// set endpoints of energy bolts
			weapon_bursts[1].x = cross_x_screen + RAND_RANGE(-4, 4);
			weapon_bursts[1].y = cross_y_screen + RAND_RANGE(-4, 4);

			weapon_bursts[3].x = cross_x_screen + RAND_RANGE(-4, 4);
			weapon_bursts[3].y = cross_y_screen + RAND_RANGE(-4, 4);

			// draw the weapons firing
			Render_Weapon_Bursts(weapon_bursts, 2, 8, 16, RGB16Bit(0, 255, 0), framebuffer, framebuffer_pitch);

			// decrease weapon energy
			if ((weapon_energy -= 5) < 0)
				weapon_energy = 0;

			// energize weapons
			weapon_active = 1;

			// turn the lights on!
			lights[POINT_LIGHT2_INDEX].state = LIGHTV1_STATE_ON;
			cockpit.curr_frame = 1;

		} // end if
		else {
			weapon_active = 0;

			// turn the lights off!
			lights[POINT_LIGHT2_INDEX].state = LIGHTV1_STATE_OFF;
			cockpit.curr_frame = 0;
		} // end else

		if ((weapon_energy += 1) > 100)
			weapon_energy = 100;

		// draw any overlays ///////////////////////////////////////////////////

		// draw cockpit
//		Draw_BOB16(&cockpit, framebuffer, framebuffer_pitch);
		src_rect = { 0, 0, cockpit.width, cockpit.height }; // the source rectangle
		dest_rect = { 0, 0, screen_width, screen_height }; // the destination rectangle
		Blit_Rect16(src_rect, cockpit.images[0], cockpit.width * 2, dest_rect, framebuffer, framebuffer_pitch, 0);

		// draw information
		sprintf(work_string, "SCORE:%d", score);
		Draw_Bitmap_Font_String(&tech_font, TPOS_SCORE_X, TPOS_SCORE_Y, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

		sprintf(work_string, "HITS:%d", hits);
		Draw_Bitmap_Font_String(&tech_font, TPOS_HITS_X, TPOS_HITS_Y, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

		sprintf(work_string, "ESCAPED:%d", escaped);
		Draw_Bitmap_Font_String(&tech_font, TPOS_ESCAPED_X, TPOS_ESCAPED_Y, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

		sprintf(work_string, "LEVEL:%2.4f", diff_level);
		Draw_Bitmap_Font_String(&tech_font, TPOS_GLEVEL_X, TPOS_GLEVEL_Y, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

		sprintf(work_string, "VEL:%3.2fK/S", player_z_vel + diff_level);
		Draw_Bitmap_Font_String(&tech_font, TPOS_SPEED_X, TPOS_SPEED_Y, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

		sprintf(work_string, "NRG:%d", weapon_energy);
		Draw_Bitmap_Font_String(&tech_font, TPOS_ENERGY_X, TPOS_ENERGY_Y, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

		// process game over stuff
		if (game_state == GAME_STATE_OVER) {
			// do rendering
			sprintf(work_string, "GAME OVER");
			Draw_Bitmap_Font_String(&tech_font, TPOS_GINFO_X - (FONT_HPITCH / 2) * strlen(work_string), TPOS_GINFO_Y, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

			sprintf(work_string, "PRESS ENTER TO PLAY AGAIN");
			Draw_Bitmap_Font_String(&tech_font, TPOS_GINFO_X - (FONT_HPITCH / 2) * strlen(work_string), TPOS_GINFO_Y + 2 * FONT_VPITCH, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

			// logic...
			if (RETRO_KeyPressed(SDL_SCANCODE_RETURN)) {
				game_state = GAME_STATE_RESTART;
			} // end if

		} // end if

#ifdef DEBUG_ON
	   // display diagnostics
		sprintf(work_string, "LE[%s]:AMB=%d,INFL=%d,PNTL=%d,SPTL=%d,ZS[%s],BFRM[%s], WF=%s",
			((lighting_mode == 1) ? "ON" : "OFF"),
			lights[AMBIENT_LIGHT_INDEX].state,
			lights[INFINITE_LIGHT_INDEX].state,
			lights[POINT_LIGHT_INDEX].state,
			lights[SPOT_LIGHT2_INDEX].state,
			((zsort_mode == 1) ? "ON" : "OFF"),
			((backface_mode == 1) ? "ON" : "OFF"),
			((wireframe_mode == 1) ? "ON" : "OFF"));

		Draw_Bitmap_Font_String(&tech_font, 4, screen_height - 16, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);
#endif

		// draw startup information for first few seconds of restart
		if (restart_state == 0) {
			sprintf(work_string, "GET READY!");
			Draw_Bitmap_Font_String(&tech_font, TPOS_GINFO_X - (FONT_HPITCH / 2) * strlen(work_string), TPOS_GINFO_Y, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);

			// update counter
			if (++restart_state_count1 > 100)
				restart_state = 1;
		} // end if

#ifdef DEBUG_ON
		sprintf(work_string, "p=[%d, %d, %d]", px, py, pz);
		Draw_Bitmap_Font_String(&tech_font, TPOS_GINFO_X - (FONT_HPITCH / 2) * strlen(work_string), TPOS_GINFO_Y + 24, work_string, FONT_HPITCH, FONT_VPITCH, framebuffer, framebuffer_pitch);
#endif

		if (game_state == GAME_STATE_RUN) {
			// draw cross on top of everything, it's holographic :)
			cross_x_screen = screen_width / 2 + cross_x;
			cross_y_screen = screen_height / 2 + cross_y;

			crosshair.x = cross_x_screen - CROSS_WIDTH / 2 + 1;
			crosshair.y = cross_y_screen - CROSS_HEIGHT / 2;

			Draw_BOB16(&crosshair, framebuffer, framebuffer_pitch);
		} // end if

		// sync to 30ish fps
		Wait_Clock(30);

		// check if player has lost?
		if (escaped == MAX_MISSES_GAME_OVER) {
			game_state = GAME_STATE_OVER;
			escaped++;
		} // end if

	} break;

	case GAME_STATE_DEMO:
	{
	} break;

	case GAME_STATE_EXIT:
	{
	} break;

	default: break;

	} // end switch game state
}

//////////////////////////////////////////////////////////

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init16();
	RETRO_SetMouseMode(true);

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
		&cam_target,    // no target
		150.0,          // near and far clipping planes
		20000.0,
		120.0,      // field of view in degrees
		screen_width,   // size of final screen viewport
		screen_height);

	// load flat shaded cube
	VECTOR4D_INITXYZ(&vscale, 18.00, 18.00, 18.00);
	Load_OBJECT4DV2_COB(&obj_alien, "assets/tie04.cob",
		&vscale, &vpos, &vrot, VERTEX_FLAGS_SWAP_YZ | VERTEX_FLAGS_SWAP_XZ |
		VERTEX_FLAGS_INVERT_WINDING_ORDER |
		VERTEX_FLAGS_TRANSFORM_LOCAL |
		VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD);

	// create system colors
	rgb_green = RGB16Bit(0, 255, 0);
	rgb_white = RGB16Bit(255, 255, 255);
	rgb_blue = RGB16Bit(0, 0, 255);
	rgb_red = RGB16Bit(255, 0, 0);

	// set up lights
	Reset_Lights_LIGHTV1();

	// create some working colors
	white.rgba = _RGBA32BIT(255, 255, 255, 0);
	light_gray.rgba = _RGBA32BIT(100, 100, 100, 0);
	gray.rgba = _RGBA32BIT(50, 50, 50, 0);
	black.rgba = _RGBA32BIT(0, 0, 0, 0);
	red.rgba = _RGBA32BIT(255, 0, 0, 0);
	green.rgba = _RGBA32BIT(0, 255, 0, 0);
	blue.rgba = _RGBA32BIT(0, 0, 255, 0);

	// ambient light
	Init_Light_LIGHTV1(AMBIENT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_AMBIENT,  // ambient light type
		gray, black, black,    // color for ambient term only
		NULL, NULL,            // no need for pos or dir
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	VECTOR4D dlight_dir = { -1,-1,1,0 };

	// directional sun light
	Init_Light_LIGHTV1(INFINITE_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_INFINITE, // infinite light type
		black, light_gray, black,    // color for diffuse term only
		NULL, &dlight_dir,     // need direction only
		0, 0, 0,                 // no need for attenuation
		0, 0, 0);                // spotlight info NA

	// the red giant
	VECTOR4D plight_pos = { -2700,-1600,8000,0 };

	// point light
	Init_Light_LIGHTV1(POINT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_POINT,    // pointlight type
		black, red, black,     // color for diffuse term only
		&plight_pos, NULL,     // need pos only
		0, .001, 0,              // linear attenuation only
		0, 0, 1);                // spotlight info NA

	// the weapons blast green glow point light from front of ship
	VECTOR4D plight2_pos = { 0,0,200,0 };

	// point light2
	Init_Light_LIGHTV1(POINT_LIGHT2_INDEX,
		LIGHTV1_STATE_OFF,       // turn the light on
		LIGHTV1_ATTR_POINT,     //  pointlight
		black, green, black,      // color for diffuse term only
		&plight2_pos, NULL, // need pos only
		0, .0005, 0,                 // linear attenuation only
		0, 0, 1);

	// create lookup for lighting engine
	RGB_16_8_IndexedRGB_Table_Builder(DD_PIXEL_FORMAT565,  // format we want to build table for
		palette,             // source palette
		rgblookup);          // lookup table

	// load in the cockpit image
	Load_Bitmap_File(&bitmap16bit, "assets/cockpit03.bmp");
	Create_BOB(&cockpit, 0, 0, 800, 600, 2, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);
	Load_Frame_BOB16(&cockpit, &bitmap16bit, 0, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);

	Load_Bitmap_File(&bitmap16bit, "assets/cockpit03b.bmp");
	Load_Frame_BOB16(&cockpit, &bitmap16bit, 1, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);

	// load in the background starscape(s)

	Create_BOB(&starscape, 0, 0, 800, 600, 3, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);

	Load_Bitmap_File(&bitmap16bit, "assets/nebblue01.bmp");
	Load_Frame_BOB16(&starscape, &bitmap16bit, 0, 0, 0, BITMAP_EXTRACT_MODE_ABS);

	Load_Bitmap_File(&bitmap16bit, "assets/nebred01.bmp");
	Load_Frame_BOB16(&starscape, &bitmap16bit, 1, 0, 0, BITMAP_EXTRACT_MODE_ABS);

	Load_Bitmap_File(&bitmap16bit, "assets/nebgreen03.bmp");
	Load_Frame_BOB16(&starscape, &bitmap16bit, 2, 0, 0, BITMAP_EXTRACT_MODE_ABS);

	Unload_Bitmap_File(&bitmap16bit);

	// load the crosshair image
	Load_Bitmap_File(&bitmap16bit, "assets/crosshair01.bmp");
	Create_BOB(&crosshair, 0, 0, CROSS_WIDTH, CROSS_HEIGHT, 1, BOB_ATTR_VISIBLE | BOB_ATTR_SINGLE_FRAME, 0, 0, 16);
	Load_Frame_BOB16(&crosshair, &bitmap16bit, 0, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Unload_Bitmap_File(&bitmap16bit);
}

///////////////////////////////////////////////////////////

void DEMO_Deinitialize(void)
{
	Close_Error_File();
}

//////////////////////////////////////////////////////////