//
// DEMOII9_1_8b.CPP - Gouraud triangle rendering demo 8-bit version
//
#include "lib/retro.h"
#include "lib/retromain.h"
#include "lib/retrofont.h"
#include "lib/t3dlib1.h"
#include "lib/t3dlib4.h"
#include "lib/t3dlib5.h"
#include "lib/t3dlib6.h"
#include "lib/t3dlib7.h"

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render2(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	// start the timing clock
	Start_Clock();

	// draw a randomly positioned gouraud triangle with 3 random vertex colors
	POLYF4DV2 face;

	// set the vertices
	face.tvlist[0].x = (int)RAND_RANGE(0, screen_width - 1);
	face.tvlist[0].y = (int)RAND_RANGE(0, screen_height - 1);
	face.lit_color[0] = RGB16Bit(RAND_RANGE(0, 255), RAND_RANGE(0, 255), RAND_RANGE(0, 255));

	face.tvlist[1].x = (int)RAND_RANGE(0, screen_width - 1);
	face.tvlist[1].y = (int)RAND_RANGE(0, screen_height - 1);
	face.lit_color[1] = RGB16Bit(RAND_RANGE(0, 255), RAND_RANGE(0, 255), RAND_RANGE(0, 255));

	face.tvlist[2].x = (int)(int)RAND_RANGE(0, screen_width - 1);
	face.tvlist[2].y = (int)(int)RAND_RANGE(0, screen_height - 1);
	face.lit_color[2] = RGB16Bit(RAND_RANGE(0, 255), RAND_RANGE(0, 255), RAND_RANGE(0, 255));

	// draw the gouraud shaded triangle
	Draw_Gouraud_Triangle(&face, framebuffer, framebuffer_pitch);

	// draw instructions
	Draw_Text_GDI("Press ESC to exit.", 0, 0, RGB(0, 255, 0), framebuffer, framebuffer_pitch);

	// Flip framebuffer
	RETRO_Flip();

	// wait a sec to see pretty triangle
	Wait_Clock(30);
}

///////////////////////////////////////////////////////////

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init(true);

	// seed random number generator
	srand(Start_Clock());

	Open_Error_File("error.txt");

	// initialize math engine
	Build_Sin_Cos_Tables();

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