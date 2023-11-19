//
// DEMOII8_1.CPP - texture multiplication demo
//
#include "lib/retro.h"
#include "lib/retromain.h"
#include "lib/retrofont.h"
#include "lib/t3dlib1.h"
#include "lib/t3dlib4.h"
#include "lib/t3dlib5.h"
#include "lib/t3dlib6.h"

// DEFINES ////////////////////////////////////////////////

#define TEXTSIZE           128 // size of texture mxm
#define NUM_TEXT           12  // number of textures

// PROTOTYPES /////////////////////////////////////////////

// GLOBALS ////////////////////////////////////////////////

char buffer[80];                          // used to print text

BITMAP_IMAGE textures[12], // holds our texture library 
temp_text;    // temporary working texture

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static int   curr_texture = 0;   // currently active texture
	static float scalef = 1.0; // texture scaling factor

	// start the timing clock
	Start_Clock();

	// copy texture into temp display texture for rendering and scaling
	Copy_Bitmap(&temp_text, 0, 0, &textures[curr_texture], 0, 0, TEXTSIZE, TEXTSIZE);

	///////////////////////////////////////////
	// our little image processing algorithm :)
	//Cmodulated = s*C1 = (s*r1, s*g1, s*b1)

	USHORT *pbuffer = (USHORT *)temp_text.buffer;

	// perform RGB transformation on bitmap
	for (int iy = 0; iy < temp_text.height; iy++)
		for (int ix = 0; ix < temp_text.width; ix++) {
			// extract pixel
			USHORT pixel = pbuffer[iy * temp_text.width + ix];

			int ri, gi, bi; // used to extract the rgb values

			// extract RGB values
			_RGB565FROM16BIT(pixel, &ri, &gi, &bi);

			// perform scaling operation and test for overflow
			if ((ri = (float)ri * scalef) > 255) ri = 255;
			if ((gi = (float)gi * scalef) > 255) gi = 255;
			if ((bi = (float)bi * scalef) > 255) bi = 255;

			// rebuild RGB and test for overflow
			// and write back to buffer
			pbuffer[iy * temp_text.width + ix] = _RGB16BIT565(ri, gi, bi);

		} // end for ix     

	////////////////////////////////////////

	// draw texture
	Draw_Bitmap16(&temp_text, framebuffer, framebuffer_pitch, 0);

	// test if user wants to change texture
	if (RETRO_KeyPressed(SDL_SCANCODE_RIGHT)) {
		if (++curr_texture > (NUM_TEXT - 1))
			curr_texture = (NUM_TEXT - 1);
	} // end if

	if (RETRO_KeyPressed(SDL_SCANCODE_LEFT)) {
		if (--curr_texture < 0)
			curr_texture = 0;
	} // end if

	// is user changing scaling factor
	if (RETRO_KeyState(SDL_SCANCODE_UP)) {
		scalef += .01;
		if (scalef > 10)
			scalef = 10;
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_DOWN)) {
		scalef -= .01;
		if (scalef < 0)
			scalef = 0;
	} // end if

	// draw title
	Draw_Text_GDI16("Use <RIGHT>/<LEFT> arrows to change texture, <UP>/<DOWN> arrows to change scale factor.", 10, 10, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("Press <ESC> to Exit. ", 10, 32, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	// print stats
	sprintf(buffer, "Texture #%d, Scaling Factor: %f", curr_texture, scalef);
	Draw_Text_GDI16(buffer, 10, screen_height - 20, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	// sync to 30ish fps
	Wait_Clock(30);
}

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init16(true);

	// load in the textures
	Load_Bitmap_File(&bitmap16bit, "assets/omptxt128_24.bmp");

	// now extract each 64x64x16 texture from the image
	for (int itext = 0; itext < NUM_TEXT; itext++) {
		// create the bitmap
		Create_Bitmap(&textures[itext], (screen_width / 2) - (TEXTSIZE / 2), (screen_height / 2) - (TEXTSIZE / 2), TEXTSIZE, TEXTSIZE, 16);
		Load_Image_Bitmap16(&textures[itext], &bitmap16bit, itext % 4, itext / 4, BITMAP_EXTRACT_MODE_CELL);
	} // end for

	// create temporary working texture (load with first texture to set loaded flags)
	Create_Bitmap(&temp_text, (screen_width / 2) - (TEXTSIZE / 2), (screen_height / 2) - (TEXTSIZE / 2), TEXTSIZE, TEXTSIZE, 16);
	Load_Image_Bitmap16(&temp_text, &bitmap16bit, 0, 0, BITMAP_EXTRACT_MODE_CELL);

	// done, so unload the bitmap
	Unload_Bitmap_File(&bitmap16bit);

	// seed random number generate
	srand(Start_Clock());
}

//////////////////////////////////////////////////////////
