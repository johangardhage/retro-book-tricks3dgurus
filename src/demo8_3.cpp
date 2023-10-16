//
// DEMOII8_3.CPP - Alpha blending demo 
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

// GLOBALS ////////////////////////////////////////////////

char buffer[80];                          // used to print text

BITMAP_IMAGE textures1[NUM_TEXT],  // holds source texture library  1
textures2[NUM_TEXT],  // holds source texture library  2
temp_text;            // temporary working texture

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static int   curr_texture1 = 0;  // source texture 1
	static int   curr_texture2 = 5;  // source texture 2
	static float alphaf = .5;  // alpha blending factor

	// start the timing clock
	Start_Clock();

	///////////////////////////////////////////
	// our little image processing algorithm :)

	//Pixel_dest[x][y]rgb = alpha    * pixel_source1[x][y]rgb + 
	//                      (1-alpha)* pixel_source2[x][y]rgb

	USHORT *s1buffer = (USHORT *)textures1[curr_texture1].buffer;
	USHORT *s2buffer = (USHORT *)textures2[curr_texture2].buffer;
	USHORT *tbuffer = (USHORT *)temp_text.buffer;

	// perform RGB transformation on bitmap
	for (int iy = 0; iy < temp_text.height; iy++)
		for (int ix = 0; ix < temp_text.width; ix++) {
			int rs1, gs1, bs1;   // used to extract the source rgb values
			int rs2, gs2, bs2; // light map rgb values
			int rf, gf, bf;   // the final rgb terms

			// extract pixel from source bitmap
			USHORT s1pixel = s1buffer[iy * temp_text.width + ix];

			// extract RGB values
			_RGB565FROM16BIT(s1pixel, &rs1, &gs1, &bs1);

			// extract pixel from lightmap bitmap
			USHORT s2pixel = s2buffer[iy * temp_text.width + ix];

			// extract RGB values
			_RGB565FROM16BIT(s2pixel, &rs2, &gs2, &bs2);

			// alpha blend them
			rf = (alphaf * (float)rs1) + ((1 - alphaf) * (float)rs2);
			gf = (alphaf * (float)gs1) + ((1 - alphaf) * (float)gs2);
			bf = (alphaf * (float)bs1) + ((1 - alphaf) * (float)bs2);

			// test for overflow
			if (rf > 255) rf = 255;
			if (gf > 255) gf = 255;
			if (bf > 255) bf = 255;

			// rebuild RGB and test for overflow
			// and write back to buffer
			tbuffer[iy * temp_text.width + ix] = _RGB16BIT565(rf, gf, bf);

		} // end for ix     

	////////////////////////////////////////

	// draw textures
	Draw_Bitmap16(&temp_text, framebuffer, framebuffer_pitch, 0);
	Draw_Bitmap16(&textures1[curr_texture1], framebuffer, framebuffer_pitch, 0);
	Draw_Bitmap16(&textures2[curr_texture2], framebuffer, framebuffer_pitch, 0);

	// test if user wants to change texture
	if (RETRO_KeyPressed(SDL_SCANCODE_RIGHT)) {
		if (++curr_texture1 > (NUM_TEXT - 1))
			curr_texture1 = (NUM_TEXT - 1);
	} // end if

	if (RETRO_KeyPressed(SDL_SCANCODE_LEFT)) {
		if (--curr_texture1 < 0)
			curr_texture1 = 0;
	} // end if

	// test if user wants to change ligthmap texture
	if (RETRO_KeyPressed(SDL_SCANCODE_UP)) {
		if (++curr_texture2 > (NUM_TEXT - 1))
			curr_texture2 = (NUM_TEXT - 1);
	} // end if

	if (RETRO_KeyPressed(SDL_SCANCODE_DOWN)) {
		if (--curr_texture2 < 0)
			curr_texture2 = 0;
	} // end if

	// is user changing scaling factor
	if (RETRO_KeyState(SDL_SCANCODE_PAGEUP)) {
		alphaf += .01;
		if (alphaf > 1)
			alphaf = 1;
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_PAGEDOWN)) {
		alphaf -= .01;
		if (alphaf < 0)
			alphaf = 0;
	} // end if

	// draw title
	Draw_Text_GDI16("Use <RIGHT>/<LEFT> arrows to change texture 1.", 10, 4, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("Use <UP>/<DOWN> arrows to change the texture 2.", 10, 20, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("Use <PAGE UP>/<PAGE DOWN> arrows to change blending factor alpha.", 10, 36, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("Press <ESC> to Exit. ", 10, 56, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	// print stats
	sprintf(buffer, "Texture 1: %d, Texture 2: %d, Blending factor: %f", curr_texture1, curr_texture2, alphaf);
	Draw_Text_GDI16(buffer, 10, screen_height - 20, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	// sync to 30ish fps
	Wait_Clock(30);
}

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init16();

	// load in the textures
	Load_Bitmap_File(&bitmap16bit, "assets/omptxt128_24.bmp");

	// now extract each 128x128x16 texture from the image
	for (int itext = 0; itext < NUM_TEXT; itext++) {
		// create the bitmap
		Create_Bitmap(&textures1[itext], (screen_width / 2) - 4 * (TEXTSIZE / 2), (screen_height / 2) - 2 * (TEXTSIZE / 2), TEXTSIZE, TEXTSIZE, 16);
		Load_Image_Bitmap16(&textures1[itext], &bitmap16bit, itext % 4, itext / 4, BITMAP_EXTRACT_MODE_CELL);
	} // end for

	// create temporary working texture (load with first texture to set loaded flags)
	Create_Bitmap(&temp_text, (screen_width / 2) - (TEXTSIZE / 2), (screen_height / 2) + (TEXTSIZE / 2), TEXTSIZE, TEXTSIZE, 16);
	Load_Image_Bitmap16(&temp_text, &bitmap16bit, 0, 0, BITMAP_EXTRACT_MODE_CELL);

	// done, so unload the bitmap
	Unload_Bitmap_File(&bitmap16bit);

	// load in the textures
	Load_Bitmap_File(&bitmap16bit, "assets/sextxt128_24.bmp");

	// now extract each 128x128x16 texture from the image
	for (int itext = 0; itext < NUM_TEXT; itext++) {
		// create the bitmap
		Create_Bitmap(&textures2[itext], (screen_width / 2) + 2 * (TEXTSIZE / 2), (screen_height / 2) - 2 * (TEXTSIZE / 2), TEXTSIZE, TEXTSIZE, 16);
		Load_Image_Bitmap16(&textures2[itext], &bitmap16bit, itext % 4, itext / 4, BITMAP_EXTRACT_MODE_CELL);
	} // end for

	// done, so unload the bitmap
	Unload_Bitmap_File(&bitmap16bit);

	// seed random number generate
	srand(Start_Clock());
}

//////////////////////////////////////////////////////////
