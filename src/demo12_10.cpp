//
// DEMOII12_10.CPP - texture mipmap generation demo
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

#define TEXTSIZE           128 // size of texture mxm
#define NUM_TEXT           12  // number of textures

// GLOBALS ////////////////////////////////////////////////

char buffer[80];                          // used to print text

BITMAP_IMAGE textures[12], // holds our texture library 
temp_text;    // temporary working texture

BITMAP_IMAGE_PTR gmipmaps;

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static int   curr_texture = 0;   // currently active texture
	static float gamma = 1.0; // mipmap gamma factor

	// start the timing clock
	Start_Clock();

	// copy texture into temp display texture for rendering and scaling
	Copy_Bitmap(&temp_text, 0, 0, &textures[curr_texture], 0, 0, TEXTSIZE, TEXTSIZE);

#if 0

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
#endif

	////////////////////////////////////////

	// generate the mipmaps on the fly
	Generate_Mipmaps(&temp_text, (BITMAP_IMAGE_PTR *)&gmipmaps, gamma);

	// draw texture
	Draw_Bitmap16(&temp_text, framebuffer, framebuffer_pitch, 0);

	// alias/cast an array of pointers to the storage which is single ptr
	BITMAP_IMAGE_PTR *tmipmaps = (BITMAP_IMAGE_PTR *)gmipmaps;

	// compute number of mip levels total
	int num_mip_levels = logbase2ofx[tmipmaps[0]->width] + 1;

	// x position to display
	int xpos = (screen_width / 2) - (TEXTSIZE / 2);

	// iterate thru and draw each bitmap
	for (int mip_level = 1; mip_level < num_mip_levels; mip_level++) {
		// position bitmap
		tmipmaps[mip_level]->x = xpos;
		tmipmaps[mip_level]->y = (screen_height / 2) + (TEXTSIZE);

		// draw bitmap
		Draw_Bitmap16(tmipmaps[mip_level], framebuffer, framebuffer_pitch, 0);
		xpos += tmipmaps[mip_level]->width + 8;

	} // end for mip_level

	// delete the mipmap chain now that we are done this frame
	Delete_Mipmaps((BITMAP_IMAGE_PTR *)&gmipmaps, 1);

	// test if user wants to change texture
	if (RETRO_KeyPressed(SDL_SCANCODE_RIGHT)) {
		if (++curr_texture > (NUM_TEXT - 1))
			curr_texture = (NUM_TEXT - 1);
	} // end if

	if (RETRO_KeyPressed(SDL_SCANCODE_LEFT)) {
		if (--curr_texture < 0)
			curr_texture = 0;
	} // end if

	// is user changing gamma factor
	if (RETRO_KeyState(SDL_SCANCODE_UP)) {
		gamma += .01;
		if (gamma > 10)
			gamma = 10;
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_DOWN)) {
		gamma -= .01;
		if (gamma < 0)
			gamma = 0;
	} // end if

	int ty = 10;

	// draw title
	Draw_Text_GDI16("Press <RIGHT>/<LEFT> arrows to change texture.", 10, ty, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("Press <UP>/<DOWN> arrows to change mipmap gamma factor.", 10, ty += 16, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("<ESC> to Exit. ", 10, ty += 16, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	// print stats
	sprintf(buffer, "Texture #%d, Gamma Factor: %f", curr_texture, gamma);
	Draw_Text_GDI16(buffer, 10, screen_height - 20, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	// sync to 30ish fps 
	Wait_Clock(30);
}

///////////////////////////////////////////////////////////////////////////////

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init16();

	// load in the textures
	Load_Bitmap_File(&bitmap16bit, "assets/omptxt128_24.bmp");

	// now extract each 64x64x16 texture from the image
	for (int itext = 0; itext < NUM_TEXT; itext++) {
		// create the bitmap
		Create_Bitmap(&textures[itext], (screen_width / 2) - (TEXTSIZE / 2), (screen_height / 2) - (TEXTSIZE / 2) - 64, TEXTSIZE, TEXTSIZE, 16);
		Load_Image_Bitmap16(&textures[itext], &bitmap16bit, itext % 4, itext / 4, BITMAP_EXTRACT_MODE_CELL);
	} // end for

	// create temporary working texture (load with first texture to set loaded flags)
	Create_Bitmap(&temp_text, (screen_width / 2) - (TEXTSIZE / 2), (screen_height / 2) - (TEXTSIZE / 2) - 40, TEXTSIZE, TEXTSIZE, 16);
	Load_Image_Bitmap16(&temp_text, &bitmap16bit, 0, 0, BITMAP_EXTRACT_MODE_CELL);

	// done, so unload the bitmap
	Unload_Bitmap_File(&bitmap16bit);

	// seed random number generate
	srand(Start_Clock());
}

//////////////////////////////////////////////////////////
