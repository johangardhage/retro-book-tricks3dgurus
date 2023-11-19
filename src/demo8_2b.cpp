//
// DEMOII8_2b.CPP - texture color modulation with base texture demo
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

#define NUM_LMAP           4   // number of light map textures

// GLOBALS ////////////////////////////////////////////////

char buffer[80];                          // used to print text

BITMAP_IMAGE textures[NUM_TEXT],   // holds our texture library 
lightmaps[NUM_LMAP],  // holds our light map textures
temp_text;            // temporary working texture

// FUNCTIONS //////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static int   curr_texture = 7;   // currently active texture
	static int   curr_lightmap = 1;   // current light map
	static float scalef = .25; // texture ambient scale factor

	// start the timing clock
	Start_Clock();

	///////////////////////////////////////////
	// our little image processing algorithm :)

	// Pixel_dest[x,y]rgb = pixel_source[x,y]rgb * ambient + 
	//                      pixel_source[x,y]rgb * light_map[x,y]rgb

	USHORT *sbuffer = (USHORT *)textures[curr_texture].buffer;
	USHORT *lbuffer = (USHORT *)lightmaps[curr_lightmap].buffer;
	USHORT *tbuffer = (USHORT *)temp_text.buffer;

	// perform RGB transformation on bitmap
	for (int iy = 0; iy < temp_text.height; iy++)
		for (int ix = 0; ix < temp_text.width; ix++) {
			int rs, gs, bs;   // used to extract the source rgb values
			int rl, gl, bl; // light map rgb values
			int rf, gf, bf;   // the final rgb terms

			// extract pixel from source bitmap
			USHORT spixel = sbuffer[iy * temp_text.width + ix];

			// extract RGB values
			_RGB565FROM16BIT(spixel, &rs, &gs, &bs);

			// extract pixel from lightmap bitmap
			USHORT lpixel = lbuffer[iy * temp_text.width + ix];

			// extract RGB values
			_RGB565FROM16BIT(lpixel, &rl, &gl, &bl);

			// simple formula base + scale * lightmap
			rf = (scalef * (float)rl) + ((float)rs);
			gf = (scalef * (float)gl) + ((float)gs);
			bf = (scalef * (float)bl) + ((float)bs);

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
	Draw_Bitmap16(&textures[curr_texture], framebuffer, framebuffer_pitch, 0);
	Draw_Bitmap16(&lightmaps[curr_lightmap], framebuffer, framebuffer_pitch, 0);

	// test if user wants to change texture
	if (RETRO_KeyPressed(SDL_SCANCODE_RIGHT)) {
		if (++curr_texture > (NUM_TEXT - 1))
			curr_texture = (NUM_TEXT - 1);
	} // end if

	if (RETRO_KeyPressed(SDL_SCANCODE_LEFT)) {
		if (--curr_texture < 0)
			curr_texture = 0;
	} // end if

	// test if user wants to change ligthmap texture
	if (RETRO_KeyPressed(SDL_SCANCODE_UP)) {
		if (++curr_lightmap > (NUM_LMAP - 1))
			curr_lightmap = (NUM_LMAP - 1);
	} // end if

	if (RETRO_KeyPressed(SDL_SCANCODE_DOWN)) {
		if (--curr_lightmap < 0)
			curr_lightmap = 0;
	} // end if

	// is user changing scaling factor
	if (RETRO_KeyState(SDL_SCANCODE_PAGEUP)) {
		scalef += .01;
		if (scalef > 10)
			scalef = 10;
	} // end if

	if (RETRO_KeyState(SDL_SCANCODE_PAGEDOWN)) {
		scalef -= .01;
		if (scalef < 0)
			scalef = 0;
	} // end if

	// draw title
	Draw_Text_GDI16("Use <RIGHT>/<LEFT> arrows to change texture.", 10, 4, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("Use <UP>/<DOWN> arrows to change the light map texture.", 10, 20, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("Use <PAGE UP>/<PAGE DOWN> arrows to change ambient scale factor.", 10, 36, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);
	Draw_Text_GDI16("Press <ESC> to Exit. ", 10, 56, RGB16Bit(255, 255, 255), framebuffer, framebuffer_pitch);

	// print stats
	sprintf(buffer, "Texture #%d, Ligtmap #%d, Ambient Scaling Factor: %f", curr_texture, curr_lightmap, scalef);
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

	// now extract each 128x128x16 texture from the image
	for (int itext = 0; itext < NUM_TEXT; itext++) {
		// create the bitmap
		Create_Bitmap(&textures[itext], (screen_width / 2) - 4 * (TEXTSIZE / 2), (screen_height / 2) - 2 * (TEXTSIZE / 2), TEXTSIZE, TEXTSIZE, 16);
		Load_Image_Bitmap16(&textures[itext], &bitmap16bit, itext % 4, itext / 4, BITMAP_EXTRACT_MODE_CELL);
	} // end for

	// create temporary working texture (load with first texture to set loaded flags)
	Create_Bitmap(&temp_text, (screen_width / 2) - (TEXTSIZE / 2), (screen_height / 2) + (TEXTSIZE / 2), TEXTSIZE, TEXTSIZE, 16);
	Load_Image_Bitmap16(&temp_text, &bitmap16bit, 0, 0, BITMAP_EXTRACT_MODE_CELL);

	// done, so unload the bitmap
	Unload_Bitmap_File(&bitmap16bit);

	// now load the lightmaps

	// load in the textures
	Load_Bitmap_File(&bitmap16bit, "assets/lightmaps128_24.bmp");

	// now extract each 128x128x16 texture from the image
	for (int itext = 0; itext < NUM_LMAP; itext++) {
		// create the bitmap
		Create_Bitmap(&lightmaps[itext], (screen_width / 2) + 2 * (TEXTSIZE / 2), (screen_height / 2) - 2 * (TEXTSIZE / 2), TEXTSIZE, TEXTSIZE, 16);
		Load_Image_Bitmap16(&lightmaps[itext], &bitmap16bit, itext % 4, itext / 4, BITMAP_EXTRACT_MODE_CELL);
	} // end for

	// done, so unload the bitmap
	Unload_Bitmap_File(&bitmap16bit);

	// seed random number generate
	srand(Start_Clock());
}

///////////////////////////////////////////////////////////
