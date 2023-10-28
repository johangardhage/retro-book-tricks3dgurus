// T3DLIB1.H - Header file for T3DLIB1.CPP game engine library

#ifndef T3DLIB1
#define T3DLIB1

// DEFINES ////////////////////////////////////////////////

#define DEBUG_ON

#define MAX_COLORS_PALETTE  256

#define DEFAULT_PALETTE_FILE "assets/paldata2.pal"

// used for selecting full screen/windowed mode
#define SCREEN_FULLSCREEN    0
#define SCREEN_WINDOWED      1

// bitmap defines
#define BITMAP_ID            0x4D42 // universal id for a bitmap
#define BITMAP_STATE_DEAD    0
#define BITMAP_STATE_ALIVE   1
#define BITMAP_STATE_DYING   2 
#define BITMAP_ATTR_LOADED   128

#define BITMAP_EXTRACT_MODE_CELL  0
#define BITMAP_EXTRACT_MODE_ABS   1

// directdraw pixel format defines, used to help bitmap loader put data in proper format
#define DD_PIXEL_FORMAT8        8
#define DD_PIXEL_FORMAT555      15
#define DD_PIXEL_FORMAT565      16
#define DD_PIXEL_FORMAT888      24
#define DD_PIXEL_FORMATALPHA888 32 

// defines for BOBs
#define BOB_STATE_DEAD         0    // this is a dead bob
#define BOB_STATE_ALIVE        1    // this is a live bob
#define BOB_STATE_DYING        2    // this bob is dying
#define BOB_STATE_ANIM_DONE    1    // done animation state
#define MAX_BOB_FRAMES         64   // maximum number of bob frames
#define MAX_BOB_ANIMATIONS     16   // maximum number of animation sequeces

#define BOB_ATTR_SINGLE_FRAME   1   // bob has single frame
#define BOB_ATTR_MULTI_FRAME    2   // bob has multiple frames
#define BOB_ATTR_MULTI_ANIM     4   // bob has multiple animations
#define BOB_ATTR_ANIM_ONE_SHOT  8   // bob will perform the animation once
#define BOB_ATTR_VISIBLE        16  // bob is visible
#define BOB_ATTR_BOUNCE         32  // bob bounces off edges
#define BOB_ATTR_WRAPAROUND     64  // bob wraps around edges
#define BOB_ATTR_LOADED         128 // the bob has been loaded
#define BOB_ATTR_CLONE          256 // the bob is a clone

// screen transition commands
#define SCREEN_DARKNESS  0         // fade to black
#define SCREEN_WHITENESS 1         // fade to white
#define SCREEN_SWIPE_X   2         // do a horizontal swipe
#define SCREEN_SWIPE_Y   3         // do a vertical swipe
#define SCREEN_DISOLVE   4         // a pixel disolve
#define SCREEN_SCRUNCH   5         // a square compression
#define SCREEN_BLUENESS  6         // fade to blue
#define SCREEN_REDNESS   7         // fade to red
#define SCREEN_GREENNESS 8         // fade to green

// defines for Blink_Colors
#define BLINKER_ADD           0    // add a light to database  
#define BLINKER_DELETE        1    // delete a light from database
#define BLINKER_UPDATE        2    // update a light
#define BLINKER_RUN           3    // run normal

// pi defines
#define PI         ((float)3.141592654f)
#define PI2        ((float)6.283185307f)
#define PI_DIV_2   ((float)1.570796327f)
#define PI_DIV_4   ((float)0.785398163f) 
#define PI_INV     ((float)0.318309886f) 

// fixed point mathematics constants
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000

// MACROS /////////////////////////////////////////////////

// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// lookup 8 bit color value from 16 bit format
#define RGB(r,g,b) (rgblookup[RGB16Bit(r, g, b)])

// this builds a 16 bit color value in 5.5.5 format (1-bit alpha mode)
#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))

// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))

// this builds a 24 bit color value in 8.8.8 format 
#define _RGB24BIT(r,g,b) ((b) + ((g) << 8) + ((r) << 16) )

// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

// initializes a direct draw struct, basically zeros it and sets the dwSize field
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

// used to compute the min and max of two expresions
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (a) : (b)) 

// used for swapping algorithm
#define SWAP(a,b,t) {t=a; a=b; b=t;}

// some math macros
#define DEG_TO_RAD(ang) ((ang)*PI/180.0)
#define RAD_TO_DEG(rads) ((rads)*180.0/PI)

#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))

// TYPES //////////////////////////////////////////////////

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned int   QUAD;
typedef unsigned int   UINT;
typedef unsigned int   DWORD;
typedef long           LONG;
typedef char           CHAR;

// PaletteEntryFlag Enumeration - MS-WMF
typedef enum
{
	PC_RESERVED = 0x01,
	PC_EXPLICIT = 0x02,
	PC_NOCOLLAPSE = 0x04
} PaletteEntryFlag;

// OFSTRUCT structure (winbase.h)
typedef struct OFSTRUCT_TYP {
	BYTE cBytes;
	BYTE fFixedDisk;
	WORD nErrCode;
	WORD Reserved1;
	WORD Reserved2;
	CHAR szPathName[128];
} OFSTRUCT, *LPOFSTRUCT, *POFSTRUCT;

// RECT structure (windef.h)
typedef struct RECT_TYP {
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

// PALETTEENTRY structure (wingdi.h)
typedef struct PALETTEENTRY_TYP {
	BYTE peRed;
	BYTE peGreen;
	BYTE peBlue;
	BYTE peFlags;
} PALETTEENTRY, *PPALETTEENTRY, *LPPALETTEENTRY;

// BITMAPFILEHEADER structure (wingdi.h)
typedef struct BITMAPFILEHEADER_TYP {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} __attribute__((__packed__)) BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

// BITMAPINFOHEADER structure (wingdi.h)
typedef struct BITMAPINFOHEADER_TYP {
	DWORD biSize;
	DWORD biWidth;
	DWORD biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	DWORD biXPelsPerMeter;
	DWORD biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} __attribute__((__packed__)) BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

// container structure for bitmaps .BMP file
typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
	BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
	PALETTEENTRY     palette[256];      // we will store the palette here
	UCHAR *buffer;           // this is a pointer to the data
} BITMAP_FILE, *BITMAP_FILE_PTR;

// the blitter object structure BOB
typedef struct BOB_TYP
{
	int state;          // the state of the object (general)
	int anim_state;     // an animation state variable, up to you
	int attr;           // attributes pertaining to the object (general)
	float x, y;            // position bitmap will be displayed at
	float xv, yv;          // velocity of object
	int width, height;  // the width and height of the bob
	int width_fill;     // internal, used to force 8*x wide surfaces
	int bpp;            // bits per pixel
	int counter_1;      // general counters
	int counter_2;
	int max_count_1;    // general threshold values;
	int max_count_2;
	int varsI[16];      // stack of 16 integers
	float varsF[16];    // stack of 16 floats
	int curr_frame;     // current animation frame
	int num_frames;     // total number of animation frames
	int curr_animation; // index of current animation
	int anim_counter;   // used to time animation transitions
	int anim_index;     // animation element index
	int anim_count_max; // number of cycles before animation
	int *animations[MAX_BOB_ANIMATIONS]; // animation sequences
	UCHAR *images[MAX_BOB_FRAMES]; // the bitmap images DD surfaces
} BOB, *BOB_PTR;

// the simple bitmap image
typedef struct BITMAP_IMAGE_TYP
{
	int state;          // state of bitmap
	int attr;           // attributes of bitmap
	int x, y;            // position of bitmap
	int width, height;  // size of bitmap
	int num_bytes;      // total bytes of bitmap
	int bpp;            // bits per pixel
	UCHAR *buffer;      // pixels of bitmap
} BITMAP_IMAGE, *BITMAP_IMAGE_PTR;

// blinking light structure
typedef struct BLINKER_TYP
{
	// user sets these
	int color_index;         // index of color to blink
	PALETTEENTRY on_color;   // RGB value of "on" color
	PALETTEENTRY off_color;  // RGB value of "off" color
	int on_time;             // number of frames to keep "on" 
	int off_time;            // number of frames to keep "off"

	// internal member
	int counter;             // counter for state transitions
	int state;               // state of light, -1 off, 1 on, 0 dead
} BLINKER, *BLINKER_PTR;

// a 2D vertex
typedef struct VERTEX2DI_TYP
{
	int x, y; // the vertex
} VERTEX2DI, *VERTEX2DI_PTR;

// a 2D vertex
typedef struct VERTEX2DF_TYP
{
	float x, y; // the vertex
} VERTEX2DF, *VERTEX2DF_PTR;

// a 2D polygon
typedef struct POLYGON2D_TYP
{
	int state;        // state of polygon
	int num_verts;    // number of vertices
	int x0, y0;        // position of center of polygon  
	int xv, yv;        // initial velocity
	DWORD color;      // could be index or PALETTENTRY
	VERTEX2DF *vlist; // pointer to vertex list
} POLYGON2D, *POLYGON2D_PTR;

// matrix defines

// 3x3 matrix /////////////////////////////////////////////
typedef struct MATRIX3X3_TYP
{
	union
	{
		float M[3][3]; // array indexed data storage

		// storage in row major form with explicit names
		struct
		{
			float M00, M01, M02;
			float M10, M11, M12;
			float M20, M21, M22;
		}; // end explicit names

	}; // end union
} MATRIX3X3, *MATRIX3X3_PTR;

// 1x3 matrix /////////////////////////////////////////////
typedef struct MATRIX1X3_TYP
{
	union
	{
		float M[3]; // array indexed data storage

		// storage in row major form with explicit names
		struct
		{
			float M00, M01, M02;

		}; // end explicit names
	}; // end union
} MATRIX1X3, *MATRIX1X3_PTR;

// 3x2 matrix /////////////////////////////////////////////
typedef struct MATRIX3X2_TYP
{
	union
	{
		float M[3][2]; // array indexed data storage

		// storage in row major form with explicit names
		struct
		{
			float M00, M01;
			float M10, M11;
			float M20, M21;
		}; // end explicit names

	}; // end union
} MATRIX3X2, *MATRIX3X2_PTR;

// 1x2 matrix /////////////////////////////////////////////
typedef struct MATRIX1X2_TYP
{
	union
	{
		float M[2]; // array indexed data storage

		// storage in row major form with explicit names
		struct
		{
			float M00, M01;

		}; // end explicit names
	}; // end union
} MATRIX1X2, *MATRIX1X2_PTR;

// GLOBALS ////////////////////////////////////////////////

FILE *fp_error = NULL; // general error file
char error_filename[80];                  // error file name

PALETTEENTRY palette[MAX_COLORS_PALETTE];         // color palette
PALETTEENTRY save_palette[MAX_COLORS_PALETTE];    // used to save palettes

UCHAR *framebuffer = NULL; // primary video buffer
int framebuffer_pitch;    // memory line pitch for framebuffer

BITMAP_FILE          bitmap8bit;             // a 8 bit bitmap file
BITMAP_FILE          bitmap16bit;            // a 16 bit bitmap file
BITMAP_FILE          bitmap24bit;            // a 24 bit bitmap file

DWORD                start_clock_count = 0;     // used for timing

// these defined the general clipping rectangle
int min_clip_x,                             // clipping rectangle 
max_clip_x,
min_clip_y,
max_clip_y;

int screen_width,            // width of screen
screen_height,           // height of screen
screen_bpp;              // bits per pixel

int dd_pixel_format;  // pixel format

// storage for our lookup tables
float cos_look[361]; // 1 extra element so we can store 0-360 inclusive
float sin_look[361];

// function ptr to RGB16 builder
USHORT(*RGB16Bit)(int r, int g, int b) = NULL;

// conditionally compilation for engine stats
int debug_total_polys_per_frame = 0;
int debug_polys_lit_per_frame = 0;
int debug_polys_clipped_per_frame = 0;
int debug_polys_rendered_per_frame = 0;
int debug_backfaces_removed_per_frame = 0;
int debug_objects_removed_per_frame = 0;
int debug_total_transformations_per_frame = 0;

// FUNCTIONS //////////////////////////////////////////////

USHORT RGB16Bit565(int r, int g, int b)
{
	// this function simply builds a 5.6.5 format 16 bit pixel
	// assumes input is RGB 0-255 each channel
	r >>= 3; g >>= 2; b >>= 3;
	return(_RGB16BIT565((r), (g), (b)));
} // end RGB16Bit565

//////////////////////////////////////////////////////////

USHORT RGB16Bit555(int r, int g, int b)
{
	// this function simply builds a 5.5.5 format 16 bit pixel
	// assumes input is RGB 0-255 each channel
	r >>= 3; g >>= 3; b >>= 3;
	return(_RGB16BIT555((r), (g), (b)));
} // end RGB16Bit555

//////////////////////////////////////////////////////////

inline void Mem_Set_WORD(void *dest, USHORT data, int count)
{
	// this function fills or sets unsigned 16-bit aligned memory
	// count is number of words

	USHORT *dest_ptr = (USHORT *)dest;

	for (int index = 0; index < count; index++) {
		dest_ptr[index] = data;
	}
} // end Mem_Set_WORD

///////////////////////////////////////////////////////////

inline void Mem_Set_QUAD(void *dest, UINT data, int count)
{
	// this function fills or sets unsigned 32-bit aligned memory
	// count is number of quads

	UINT *dest_ptr = (UINT *)dest;

	for (int index = 0; index < count; index++) {
		dest_ptr[index] = data;
	}
} // end Mem_Set_QUAD

///////////////////////////////////////////////////////////

int Write_Error(const char *string, ...)
{
	// this function prints out the error string to the error file

	char buffer[1024]; // working buffer

	va_list arglist; // variable argument list

	// make sure both the error file and string are valid
	if (!string || !fp_error)
		return(0);

	// print out the string using the variable number of arguments on stack
	va_start(arglist, string);
	vsprintf(buffer, string, arglist);
	va_end(arglist);

	// write string to file
	fprintf(fp_error, buffer);

	// flush buffer incase the system bails
	fflush(fp_error);

	// return success
	return(1);
} // end Write_Error

///////////////////////////////////////////////////////////

int Open_Error_File(const char *filename, FILE *fp_override = NULL)
{
	// this function creates the output error file

	// is user requesting special file handle? stdout, stderr, etc.?
	if (fp_override) {
		fp_error = fp_override;
	} else {
		// test if this file is valid
		if ((fp_error = fopen(filename, "w")) == NULL)
			return(0);
	}

	// get the current time
	time_t timebuffer;
	char *timeline;
	char timestring[280];

	time(&timebuffer);
	timeline = ctime(&(timebuffer));

	sprintf(timestring, "%.19s.%lu, %s", timeline, timebuffer * 1000, &timeline[20]);

	// write out error header with time
	Write_Error("\nOpening Error Output File (%s) on %s\n", filename, timestring);

	// now the file is created, re-open with append mode

	if (!fp_override) {
		fclose(fp_error);
		if ((fp_error = fopen(filename, "a+")) == NULL)
			return(0);
	}

	// return success
	return(1);

} // end Open_Error_File

///////////////////////////////////////////////////////////

int Close_Error_File(void)
{
	// this function closes the error file

	if (fp_error) {
		// write close file string
		Write_Error("\nClosing Error Output File.");

		if (fp_error != stdout || fp_error != stderr) {
			// close the file handle
			fclose(fp_error);
		}

		fp_error = NULL;

		// return success
		return(1);
	} // end if
	else
		return(0);

} // end Close_Error_File

//////////////////////////////////////////////////////////

int Create_BOB(BOB_PTR bob,           // the bob to create
	int x, int y,          // initial posiiton
	int width, int height, // size of bob
	int num_frames,        // number of frames
	int attr,              // attrs
	int mem_flags = 0,         // memory flags in DD format
	USHORT color_key_value = 0, // default color key
	int bpp = 8)            // bits per pixel
{
	// Create the BOB object, note that all BOBs 
	// are created as offscreen surfaces in VRAM as the
	// default, if you want to use system memory then
	// set flags equal to:
	// DDSCAPS_SYSTEMMEMORY 
	// for video memory you can create either local VRAM surfaces or AGP 
	// surfaces via the second set of constants shown below in the regular expression
	// DDSCAPS_VIDEOMEMORY | (DDSCAPS_NONLOCALVIDMEM | DDSCAPS_LOCALVIDMEM ) 

	int index;           // looping var

	// set state and attributes of BOB
	bob->state = BOB_STATE_ALIVE;
	bob->attr = attr;
	bob->anim_state = 0;
	bob->counter_1 = 0;
	bob->counter_2 = 0;
	bob->max_count_1 = 0;
	bob->max_count_2 = 0;
	bob->curr_frame = 0;
	bob->num_frames = num_frames;
	bob->bpp = bpp;
	bob->curr_animation = 0;
	bob->anim_counter = 0;
	bob->anim_index = 0;
	bob->anim_count_max = 0;
	bob->x = x;
	bob->y = y;
	bob->xv = 0;
	bob->yv = 0;

	// set dimensions of the new bitmap surface
	bob->width = width;
	bob->height = height;

	// set all images to null
	for (index = 0; index < MAX_BOB_FRAMES; index++) {
		bob->images[index] = NULL;
	}

	// set all animations to null
	for (index = 0; index < MAX_BOB_ANIMATIONS; index++) {
		bob->animations[index] = NULL;
	}

#if 0
	// make sure surface width is a multiple of 8, some old version of dd like that
	// now, it's unneeded...
	bob->width_fill = ((width % 8 != 0) ? (8 - width % 8) : 0);
	Write_Error("\nCreate BOB: width_fill=%d", bob->width_fill);
#endif

	// return success
	return(1);
} // end Create_BOB

///////////////////////////////////////////////////////////

int Clone_BOB(BOB_PTR source, BOB_PTR dest)
{
	// this function clones a BOB and updates the attr var to reflect that
	// the BOB is a clone and not real, this is used later in the destroy
	// function so a clone doesn't destroy the memory of a real bob

	if ((source && dest) && (source != dest)) {
		// copy the bob data
		memcpy(dest, source, sizeof(BOB));

		// set the clone attribute
		dest->attr |= BOB_ATTR_CLONE;
	} // end if

	else {
		return(0);
	}

	// return success
	return(1);
} // end Clone_BOB

///////////////////////////////////////////////////////////

int Destroy_BOB(BOB_PTR bob)
{
	// destroy the BOB, tests if this is a real bob or a clone
	// if real then release all the memory, otherwise, just resets
	// the pointers to null

	int index; // looping var

	// is this bob valid
	if (!bob) {
		return(0);
	}

	// test if this is a clone
	if ((bob->attr & BOB_ATTR_CLONE)) {
		// null link all surfaces
		for (index = 0; index < MAX_BOB_FRAMES; index++) {
			if (bob->images[index]) {
				bob->images[index] = NULL;
			}
		}

		// release memory for animation sequences
		for (index = 0; index < MAX_BOB_ANIMATIONS; index++) {
			if (bob->animations[index]) {
				bob->animations[index] = NULL;
			}
		}
	} // end if
	else {
		// release memory for all surfaces
		for (index = 0; index < MAX_BOB_FRAMES; index++) {
			if (bob->images[index]) {
				free(bob->images[index]);
			}
		}

		// release memory for animation sequences
		for (index = 0; index < MAX_BOB_ANIMATIONS; index++) {
			if (bob->animations[index]) {
				free(bob->animations[index]);
			}
		}
	} // end else not clone

	// return success
	return(1);
} // end Destroy_BOB

///////////////////////////////////////////////////////////

void Blit_Rect(RECT src_rect, UCHAR *src_buf, int src_pitch, RECT dest_rect, UCHAR *dest_buf, int dest_pitch, int alpha = -1)
{
	UCHAR *src = (UCHAR *)src_buf;
	UCHAR *dest = (UCHAR *)dest_buf;

	float dest_xdiff = (dest_rect.right - dest_rect.left) != 0 ? dest_rect.right - dest_rect.left : 1;
	float dest_ydiff = (dest_rect.bottom - dest_rect.top) != 0 ? dest_rect.bottom - dest_rect.top : 1;
	float src_xdelta = (src_rect.right - src_rect.left) / dest_xdiff;
	float src_ydelta = (src_rect.bottom - src_rect.top) / dest_ydiff;

	for (int y = 0; y < dest_ydiff; y++) {
		for (int x = 0; x < dest_xdiff; x++) {
			int src_x = src_rect.left + src_xdelta * x;
			int src_y = src_rect.top + src_ydelta * y;
			int dest_x = dest_rect.left + x;
			int dest_y = dest_rect.top + y;
			if (dest_x >= 0 && dest_x < screen_width && dest_y >= 0 && dest_y < screen_height) {
				if (alpha == -1 || src[src_y * src_pitch + src_x] != alpha) {
					dest[dest_y * dest_pitch + dest_x] = src[src_y * src_pitch + src_x];
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////

void Blit_Rect16(RECT src_rect, UCHAR *src_buf, int src_pitch, RECT dest_rect, UCHAR *dest_buf, int dest_pitch, int alpha = -1)
{
	// recompute mempitch in 16-bit words
	src_pitch = src_pitch >> 1;
	dest_pitch = dest_pitch >> 1;

	USHORT *src = (USHORT *)src_buf;
	USHORT *dest = (USHORT *)dest_buf;

	float dest_xdiff = (dest_rect.right - dest_rect.left) != 0 ? dest_rect.right - dest_rect.left : 1;
	float dest_ydiff = (dest_rect.bottom - dest_rect.top) != 0 ? dest_rect.bottom - dest_rect.top : 1;
	float src_xdelta = (src_rect.right - src_rect.left) / dest_xdiff;
	float src_ydelta = (src_rect.bottom - src_rect.top) / dest_ydiff;

	for (int y = 0; y < dest_ydiff; y++) {
		for (int x = 0; x < dest_xdiff; x++) {
			int src_x = src_rect.left + src_xdelta * x;
			int src_y = src_rect.top + src_ydelta * y;
			int dest_x = dest_rect.left + x;
			int dest_y = dest_rect.top + y;
			if (dest_x >= 0 && dest_x < screen_width && dest_y >= 0 && dest_y < screen_height) {
				if (alpha == -1 || src[src_y * src_pitch + src_x] != alpha) {
					dest[dest_y * dest_pitch + dest_x] = src[src_y * src_pitch + src_x];
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////

int Draw_BOB(BOB_PTR bob,               // bob to draw
	UCHAR *dest, int lpitch) // surface to draw the bob on
{
	// draw a bob at the x,y defined in the BOB
	// on the destination surface defined in dest

	RECT dest_rect,   // the destination rectangle
		source_rect; // the source rectangle                             

	// is this a valid bob
	if (!bob)
		return(0);

	// is bob visible
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);

	// fill in the source rect
	source_rect.left = 0;
	source_rect.top = 0;
	source_rect.right = bob->width;
	source_rect.bottom = bob->height;

	// fill in the destination rect
	dest_rect.left = bob->x;
	dest_rect.top = bob->y;
	dest_rect.right = bob->x + bob->width;
	dest_rect.bottom = bob->y + bob->height;

	// blt to destination surface
	Blit_Rect(source_rect, bob->images[bob->curr_frame], bob->width, dest_rect, dest, lpitch, 0);

	// return success
	return(1);
} // end Draw_BOB

///////////////////////////////////////////////////////////

int Draw_Scaled_BOB(BOB_PTR bob, int swidth, int sheight,  // bob and new dimensions
	UCHAR *dest, int lpitch) // surface to draw the bob on)
{
	// this function draws a scaled bob to the size swidth, sheight

	RECT dest_rect,   // the destination rectangle
		source_rect; // the source rectangle                             

	// is this a valid bob
	if (!bob)
		return(0);

	// is bob visible
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);

	// fill in the source rect
	source_rect.left = 0;
	source_rect.top = 0;
	source_rect.right = bob->width;
	source_rect.bottom = bob->height;

	// fill in the destination rect
	dest_rect.left = bob->x;
	dest_rect.top = bob->y;
	dest_rect.right = bob->x + swidth;
	dest_rect.bottom = bob->y + sheight;

	// blt to destination surface
	Blit_Rect(source_rect, bob->images[bob->curr_frame], bob->width, dest_rect, dest, lpitch, 0);

	// return success
	return(1);
} // end Draw_Scaled_BOB

////////////////////////////////////////////////////

int Draw_BOB16(BOB_PTR bob,             // bob to draw
	UCHAR *dest, int lpitch) // surface to draw the bob on
{
	// draw a bob at the x,y defined in the BOB
	// on the destination surface defined in dest

	RECT dest_rect,   // the destination rectangle
		source_rect; // the source rectangle                             

	// is this a valid bob
	if (!bob)
		return(0);

	// is bob visible
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);

	// fill in the source rect
	source_rect.left = 0;
	source_rect.top = 0;
	source_rect.right = bob->width;
	source_rect.bottom = bob->height;

	// fill in the destination rect
	dest_rect.left = bob->x;
	dest_rect.top = bob->y;
	dest_rect.right = bob->x + bob->width;
	dest_rect.bottom = bob->y + bob->height;

	// blt to destination surface
	Blit_Rect16(source_rect, bob->images[bob->curr_frame], bob->width * 2, dest_rect, dest, lpitch, 0);

	// return success
	return(1);
} // end Draw_BOB16

///////////////////////////////////////////////////////////

int Draw_Scaled_BOB16(BOB_PTR bob, int swidth, int sheight,  // bob and new dimensions
	UCHAR *dest, int lpitch) // surface to draw the bob on)
{
	// this function draws a scaled bob to the size swidth, sheight

	RECT dest_rect,   // the destination rectangle
		source_rect; // the source rectangle                             

	// is this a valid bob
	if (!bob)
		return(0);

	// is bob visible
	if (!(bob->attr & BOB_ATTR_VISIBLE))
		return(1);

	// fill in the source rect
	source_rect.left = 0;
	source_rect.top = 0;
	source_rect.right = bob->width;
	source_rect.bottom = bob->height;

	// fill in the destination rect
	dest_rect.left = bob->x;
	dest_rect.top = bob->y;
	dest_rect.right = bob->x + swidth;
	dest_rect.bottom = bob->y + sheight;

	// blt to destination surface
	Blit_Rect16(source_rect, bob->images[bob->curr_frame], bob->width * 2, dest_rect, dest, lpitch, 0);

	// return success
	return(1);
} // end Draw_Scaled_BOB16

///////////////////////////////////////////////////////////

int Load_Frame_BOB(BOB_PTR bob, // bob to load with data
	BITMAP_FILE_PTR bitmap, // bitmap to scan image data from
	int frame,       // frame to load
	int cx, int cy,   // cell or absolute pos. to scan image from
	int mode)        // if 0 then cx,cy is cell position, else cx,cy are absolute coords
{
	// this function extracts a bitmap out of a bitmap file

	// is this a valid bob
	if (!bob)
		return(0);

	UCHAR *source_ptr,   // working pointers
		*dest_ptr;

	// test the mode of extraction, cell based or absolute
	if (mode == BITMAP_EXTRACT_MODE_CELL) {
		// re-compute x,y
		cx = cx * (bob->width + 1) + 1;
		cy = cy * (bob->height + 1) + 1;
	} // end if

	// extract bitmap data
	source_ptr = bitmap->buffer + cy * bitmap->bitmapinfoheader.biWidth + cx;

	// allocate memory for bob image
	if (!(bob->images[frame] = (UCHAR *)malloc(bob->width * bob->height)))
		return(0);

	// assign a pointer to the memory surface for manipulation
	dest_ptr = (UCHAR *)bob->images[frame];

	// iterate thru each scanline and copy bitmap
	for (int index_y = 0; index_y < bob->height; index_y++) {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr, bob->width);

		// advance pointers
		dest_ptr += bob->width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} // end for index_y

	// set state to loaded
	bob->attr |= BOB_ATTR_LOADED;

	// return success
	return(1);
} // end Load_Frame_BOB

///////////////////////////////////////////////////////////////

int Load_Frame_BOB16(BOB_PTR bob, // bob to load with data
	BITMAP_FILE_PTR bitmap, // bitmap to scan image data from
	int frame,       // frame to load
	int cx, int cy,   // cell or absolute pos. to scan image from
	int mode)        // if 0 then cx,cy is cell position, else 
	// cx,cy are absolute coords
{
	// this function extracts a 16-bit bitmap out of a 16-bit bitmap file

	// is this a valid bob
	if (!bob)
		return(0);

	USHORT *source_ptr,   // working pointers
		*dest_ptr;

	// test the mode of extraction, cell based or absolute
	if (mode == BITMAP_EXTRACT_MODE_CELL) {
		// re-compute x,y
		cx = cx * (bob->width + 1) + 1;
		cy = cy * (bob->height + 1) + 1;
	} // end if

	// extract bitmap data
	source_ptr = (USHORT *)bitmap->buffer + cy * bitmap->bitmapinfoheader.biWidth + cx;

	// allocate memory for bob image
	if (!(bob->images[frame] = (UCHAR *)malloc(bob->width * bob->height * 2)))
		return(0);

	// assign a pointer to the memory surface for manipulation
	dest_ptr = (USHORT *)bob->images[frame];

	int bytes_per_line = bob->width * 2;

	// iterate thru each scanline and copy bitmap
	for (int index_y = 0; index_y < bob->height; index_y++) {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr, bytes_per_line);

		// advance pointers
		dest_ptr += bob->width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} // end for index_y

	// set state to loaded
	bob->attr |= BOB_ATTR_LOADED;

	// return success
	return(1);

} // end Load_Frame_BOB16

///////////////////////////////////////////////////////////

int Animate_BOB(BOB_PTR bob)
{
	// this function animates a bob, basically it takes a look at
	// the attributes of the bob and determines if the bob is 
	// a single frame, multiframe, or multi animation, updates
	// the counters and frames appropriately

	// is this a valid bob
	if (!bob)
		return(0);

	// test the level of animation
	if (bob->attr & BOB_ATTR_SINGLE_FRAME) {
		// current frame always = 0
		bob->curr_frame = 0;
		return(1);
	} // end if
	else
		if (bob->attr & BOB_ATTR_MULTI_FRAME) {
			// update the counter and test if its time to increment frame
			if (++bob->anim_counter >= bob->anim_count_max) {
				// reset counter
				bob->anim_counter = 0;

				// move to next frame
				if (++bob->curr_frame >= bob->num_frames)
					bob->curr_frame = 0;

			} // end if

		} // end elseif
		else
			if (bob->attr & BOB_ATTR_MULTI_ANIM) {
				// this is the most complex of the animations it must look up the
				// next frame in the animation sequence

				// first test if its time to animate
				if (++bob->anim_counter >= bob->anim_count_max) {
					// reset counter
					bob->anim_counter = 0;

					// increment the animation frame index
					bob->anim_index++;

					// extract the next frame from animation list 
					bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];

					// is this and end sequence flag -1
					if (bob->curr_frame == -1) {
						// test if this is a single shot animation
						if (bob->attr & BOB_ATTR_ANIM_ONE_SHOT) {
							// set animation state message to done
							bob->anim_state = BOB_STATE_ANIM_DONE;

							// reset frame back one
							bob->anim_index--;

							// extract animation frame
							bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];

						} // end if
						else {
							// reset animation index
							bob->anim_index = 0;

							// extract first animation frame
							bob->curr_frame = bob->animations[bob->curr_animation][bob->anim_index];
						} // end else

					}  // end if

				} // end if

			} // end elseif

		 // return success
	return(1);

} // end Amimate_BOB

///////////////////////////////////////////////////////////

int Scroll_BOB(void)
{
	// this function scrolls a bob 
	// not implemented

	// return success
	return(1);
} // end Scroll_BOB

///////////////////////////////////////////////////////////

int Move_BOB(BOB_PTR bob)
{
	// this function moves the bob based on its current velocity
	// also, the function test for various motion attributes of the'
	// bob and takes the appropriate actions

	// is this a valid bob
	if (!bob)
		return(0);

	// translate the bob
	bob->x += bob->xv;
	bob->y += bob->yv;

	// test for wrap around
	if (bob->attr & BOB_ATTR_WRAPAROUND) {
		// test x extents first
		if (bob->x > max_clip_x)
			bob->x = min_clip_x - bob->width;
		else
			if (bob->x < min_clip_x - bob->width)
				bob->x = max_clip_x;

		// now y extents
		if (bob->x > max_clip_x)
			bob->x = min_clip_x - bob->width;
		else
			if (bob->x < min_clip_x - bob->width)
				bob->x = max_clip_x;

	} // end if
	else
		// test for bounce
		if (bob->attr & BOB_ATTR_BOUNCE) {
			// test x extents first
			if ((bob->x > max_clip_x - bob->width) || (bob->x < min_clip_x))
				bob->xv = -bob->xv;

			// now y extents 
			if ((bob->y > max_clip_y - bob->height) || (bob->y < min_clip_y))
				bob->yv = -bob->yv;

		} // end if

	 // return success
	return(1);
} // end Move_BOB

///////////////////////////////////////////////////////////

int Load_Animation_BOB(BOB_PTR bob,
	int anim_index,
	int num_frames,
	int *sequence)
{
	// this function load an animation sequence for a bob
	// the sequence consists of frame indices, the function
	// will append a -1 to the end of the list so the display
	// software knows when to restart the animation sequence

	// is this bob valid
	if (!bob)
		return(0);

	// allocate memory for bob animation
	if (!(bob->animations[anim_index] = (int *)malloc((num_frames + 1) * sizeof(int))))
		return(0);

	// load data into 
	for (int index = 0; index < num_frames; index++)
		bob->animations[anim_index][index] = sequence[index];

	// set the end of the list to a -1
	bob->animations[anim_index][num_frames] = -1;

	// return success
	return(1);

} // end Load_Animation_BOB

///////////////////////////////////////////////////////////

int Set_Pos_BOB(BOB_PTR bob, int x, int y)
{
	// this functions sets the postion of a bob

	// is this a valid bob
	if (!bob)
		return(0);

	// set positin
	bob->x = x;
	bob->y = y;

	// return success
	return(1);
} // end Set_Pos_BOB

///////////////////////////////////////////////////////////

int Set_Anim_Speed_BOB(BOB_PTR bob, int speed)
{
	// this function simply sets the animation speed of a bob

	// is this a valid bob
	if (!bob)
		return(0);

	// set speed
	bob->anim_count_max = speed;

	// return success
	return(1);

} // end Set_Anim_Speed

///////////////////////////////////////////////////////////

int Set_Animation_BOB(BOB_PTR bob, int anim_index)
{
	// this function sets the animation to play

	// is this a valid bob
	if (!bob)
		return(0);

	// set the animation index
	bob->curr_animation = anim_index;

	// reset animation 
	bob->anim_index = 0;

	// return success
	return(1);

} // end Set_Animation_BOB

///////////////////////////////////////////////////////////

int Set_Vel_BOB(BOB_PTR bob, int xv, int yv)
{
	// this function sets the velocity of a bob

	// is this a valid bob
	if (!bob)
		return(0);

	// set velocity
	bob->xv = xv;
	bob->yv = yv;

	// return success
	return(1);
} // end Set_Vel_BOB

///////////////////////////////////////////////////////////

int Hide_BOB(BOB_PTR bob)
{
	// this functions hides bob 

	// is this a valid bob
	if (!bob)
		return(0);

	// reset the visibility bit
	RESET_BIT(bob->attr, BOB_ATTR_VISIBLE);

	// return success
	return(1);
} // end Hide_BOB

///////////////////////////////////////////////////////////

int Show_BOB(BOB_PTR bob)
{
	// this function shows a bob

	// is this a valid bob
	if (!bob)
		return(0);

	// set the visibility bit
	SET_BIT(bob->attr, BOB_ATTR_VISIBLE);

	// return success
	return(1);
} // end Show_BOB

///////////////////////////////////////////////////////////

int Collision_BOBS(BOB_PTR bob1, BOB_PTR bob2)
{
	// are these a valid bobs
	if (!bob1 || !bob2)
		return(0);

	// get the radi of each rect
	int width1 = (bob1->width >> 1) - (bob1->width >> 3);
	int height1 = (bob1->height >> 1) - (bob1->height >> 3);

	int width2 = (bob2->width >> 1) - (bob2->width >> 3);
	int height2 = (bob2->height >> 1) - (bob2->height >> 3);

	// compute center of each rect
	int cx1 = bob1->x + width1;
	int cy1 = bob1->y + height1;

	int cx2 = bob2->x + width2;
	int cy2 = bob2->y + height2;

	// compute deltas
	int dx = abs(cx2 - cx1);
	int dy = abs(cy2 - cy1);

	// test if rects overlap
	if (dx < (width1 + width2) && dy < (height1 + height2))
		return(1);
	else
		// else no collision
		return(0);

} // end Collision_BOBS

///////////////////////////////////////////////////////////

DWORD Get_Clock(void)
{
	// this function returns the current tick count

	// return time
	return(SDL_GetTicks64());

} // end Get_Clock

///////////////////////////////////////////////////////////

DWORD Start_Clock(void)
{
	// this function starts the clock, that is, saves the current
	// count, use in conjunction with Wait_Clock()

	return(start_clock_count = Get_Clock());

} // end Start_Clock

////////////////////////////////////////////////////////////

DWORD Wait_Clock(DWORD count)
{
	// this function is used to wait for a specific number of clicks
	// since the call to Start_Clock

	while ((Get_Clock() - start_clock_count) < count);
	return(Get_Clock());

} // end Wait_Clock

///////////////////////////////////////////////////////////

int Draw_Line(int x0, int y0, // starting position 
	int x1, int y1, // ending position
	int color,     // color index
	UCHAR *vb_start, int lpitch) // video buffer and memory pitch
{
	// this function draws a line from xo,yo to x1,y1 using differential error
	// terms (based on Bresenahams work)

	int dx,             // difference in x's
		dy,             // difference in y's
		dx2,            // dx,dy * 2
		dy2,
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error,          // the discriminant i.e. error i.e. decision variable
		index;          // used for looping

	// pre-compute first pixel address in video buffer
	vb_start = vb_start + x0 + y0 * lpitch;

	// compute horizontal and vertical deltas
	dx = x1 - x0;
	dy = y1 - y0;

	// test which direction the line is going in i.e. slope angle
	if (dx >= 0) {
		x_inc = 1;

	} // end if line is moving right
	else {
		x_inc = -1;
		dx = -dx;  // need absolute value

	} // end else moving left

	// test y component of slope

	if (dy >= 0) {
		y_inc = lpitch;
	} // end if line is moving down
	else {
		y_inc = -lpitch;
		dy = -dy;  // need absolute value

	} // end else moving up

	// compute (dx,dy) * 2
	dx2 = dx << 1;
	dy2 = dy << 1;

	// now based on which delta is greater we can draw the line
	if (dx > dy) {
		// initialize error term
		error = dy2 - dx;

		// draw the line
		for (index = 0; index <= dx; index++) {
			// set the pixel
			*vb_start = color;

			// test if error has overflowed
			if (error >= 0) {
				error -= dx2;

				// move to next line
				vb_start += y_inc;

			} // end if error overflowed

			// adjust the error term
			error += dy2;

			// move to the next pixel
			vb_start += x_inc;

		} // end for

	} // end if |slope| <= 1
	else {
		// initialize error term
		error = dx2 - dy;

		// draw the line
		for (index = 0; index <= dy; index++) {
			// set the pixel
			*vb_start = color;

			// test if error overflowed
			if (error >= 0) {
				error -= dy2;

				// move to next line
				vb_start += x_inc;

			} // end if error overflowed

		 // adjust the error term
			error += dx2;

			// move to the next pixel
			vb_start += y_inc;

		} // end for

	} // end else |slope| > 1

	// return success
	return(1);

} // end Draw_Line

///////////////////////////////////////////////////////////

int Draw_Line16(int x0, int y0, // starting position 
	int x1, int y1, // ending position
	int color,     // color index
	UCHAR *vb_start, int lpitch) // video buffer and memory pitch
{
	// this function draws a line from xo,yo to x1,y1 using differential error
	// terms (based on Bresenahams work)

	int dx,             // difference in x's
		dy,             // difference in y's
		dx2,            // dx,dy * 2
		dy2,
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error,          // the discriminant i.e. error i.e. decision variable
		index;          // used for looping

	int lpitch_2 = lpitch >> 1; // USHORT strided lpitch

	// pre-compute first pixel address in video buffer based on 16bit data
	USHORT *vb_start2 = (USHORT *)vb_start + x0 + y0 * lpitch_2;

	// compute horizontal and vertical deltas
	dx = x1 - x0;
	dy = y1 - y0;

	// test which direction the line is going in i.e. slope angle
	if (dx >= 0) {
		x_inc = 1;

	} // end if line is moving right
	else {
		x_inc = -1;
		dx = -dx;  // need absolute value

	} // end else moving left

	// test y component of slope

	if (dy >= 0) {
		y_inc = lpitch_2;
	} // end if line is moving down
	else {
		y_inc = -lpitch_2;
		dy = -dy;  // need absolute value

	} // end else moving up

	// compute (dx,dy) * 2
	dx2 = dx << 1;
	dy2 = dy << 1;

	// now based on which delta is greater we can draw the line
	if (dx > dy) {
		// initialize error term
		error = dy2 - dx;

		// draw the line
		for (index = 0; index <= dx; index++) {
			// set the pixel
			*vb_start2 = (USHORT)color;

			// test if error has overflowed
			if (error >= 0) {
				error -= dx2;

				// move to next line
				vb_start2 += y_inc;

			} // end if error overflowed

			// adjust the error term
			error += dy2;

			// move to the next pixel
			vb_start2 += x_inc;

		} // end for

	} // end if |slope| <= 1
	else {
		// initialize error term
		error = dx2 - dy;

		// draw the line
		for (index = 0; index <= dy; index++) {
			// set the pixel
			*vb_start2 = (USHORT)color;

			// test if error overflowed
			if (error >= 0) {
				error -= dy2;

				// move to next line
				vb_start2 += x_inc;

			} // end if error overflowed

		 // adjust the error term
			error += dx2;

			// move to the next pixel
			vb_start2 += y_inc;

		} // end for

	} // end else |slope| > 1

	// return success
	return(1);

} // end Draw_Line16

///////////////////////////////////////////////////////////

int Clip_Line(int &x1, int &y1, int &x2, int &y2)
{
	// this function clips the sent line using the globally defined clipping
	// region

	// internal clipping codes
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001

#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009 
#define CLIP_CODE_SW 0x0005

	int xc1 = x1,
		yc1 = y1,
		xc2 = x2,
		yc2 = y2;

	int p1_code = 0,
		p2_code = 0;

	// determine codes for p1 and p2
	if (y1 < min_clip_y)
		p1_code |= CLIP_CODE_N;
	else
		if (y1 > max_clip_y)
			p1_code |= CLIP_CODE_S;

	if (x1 < min_clip_x)
		p1_code |= CLIP_CODE_W;
	else
		if (x1 > max_clip_x)
			p1_code |= CLIP_CODE_E;

	if (y2 < min_clip_y)
		p2_code |= CLIP_CODE_N;
	else
		if (y2 > max_clip_y)
			p2_code |= CLIP_CODE_S;

	if (x2 < min_clip_x)
		p2_code |= CLIP_CODE_W;
	else
		if (x2 > max_clip_x)
			p2_code |= CLIP_CODE_E;

	// try and trivially reject
	if ((p1_code & p2_code))
		return(0);

	// test for totally visible, if so leave points untouched
	if (p1_code == 0 && p2_code == 0)
		return(1);

	// determine end clip point for p1
	switch (p1_code) {
	case CLIP_CODE_C: break;

	case CLIP_CODE_N:
	{
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);
	} break;
	case CLIP_CODE_S:
	{
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);
	} break;

	case CLIP_CODE_W:
	{
		xc1 = min_clip_x;
		yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
	} break;

	case CLIP_CODE_E:
	{
		xc1 = max_clip_x;
		yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
	} break;

	// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
	{
		// north hline intersection
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x) {
			// east vline intersection
			xc1 = max_clip_x;
			yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
		} // end if

	} break;

	case CLIP_CODE_SE:
	{
		// south hline intersection
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x) {
			// east vline intersection
			xc1 = max_clip_x;
			yc1 = y1 + 0.5 + (max_clip_x - x1) * (y2 - y1) / (x2 - x1);
		} // end if

	} break;

	case CLIP_CODE_NW:
	{
		// north hline intersection
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x) {
			xc1 = min_clip_x;
			yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
		} // end if

	} break;

	case CLIP_CODE_SW:
	{
		// south hline intersection
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x2 - x1) / (y2 - y1);

		// test if intersection is valid, of so then done, else compute next
		if (xc1 < min_clip_x || xc1 > max_clip_x) {
			xc1 = min_clip_x;
			yc1 = y1 + 0.5 + (min_clip_x - x1) * (y2 - y1) / (x2 - x1);
		} // end if

	} break;

	default:break;

	} // end switch

	// determine clip point for p2
	switch (p2_code) {
	case CLIP_CODE_C: break;

	case CLIP_CODE_N:
	{
		yc2 = min_clip_y;
		xc2 = x2 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);
	} break;

	case CLIP_CODE_S:
	{
		yc2 = max_clip_y;
		xc2 = x2 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);
	} break;

	case CLIP_CODE_W:
	{
		xc2 = min_clip_x;
		yc2 = y2 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
	} break;

	case CLIP_CODE_E:
	{
		xc2 = max_clip_x;
		yc2 = y2 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
	} break;

	// these cases are more complex, must compute 2 intersections
	case CLIP_CODE_NE:
	{
		// north hline intersection
		yc2 = min_clip_y;
		xc2 = x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x) {
			// east vline intersection
			xc2 = max_clip_x;
			yc2 = y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
		} // end if

	} break;

	case CLIP_CODE_SE:
	{
		// south hline intersection
		yc2 = max_clip_y;
		xc2 = x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x) {
			// east vline intersection
			xc2 = max_clip_x;
			yc2 = y2 + 0.5 + (max_clip_x - x2) * (y1 - y2) / (x1 - x2);
		} // end if

	} break;

	case CLIP_CODE_NW:
	{
		// north hline intersection
		yc2 = min_clip_y;
		xc2 = x2 + 0.5 + (min_clip_y - y2) * (x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x) {
			xc2 = min_clip_x;
			yc2 = y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
		} // end if

	} break;

	case CLIP_CODE_SW:
	{
		// south hline intersection
		yc2 = max_clip_y;
		xc2 = x2 + 0.5 + (max_clip_y - y2) * (x1 - x2) / (y1 - y2);

		// test if intersection is valid, of so then done, else compute next
		if (xc2 < min_clip_x || xc2 > max_clip_x) {
			xc2 = min_clip_x;
			yc2 = y2 + 0.5 + (min_clip_x - x2) * (y1 - y2) / (x1 - x2);
		} // end if

	} break;

	default:break;

	} // end switch

	// do bounds check
	if ((xc1 < min_clip_x) || (xc1 > max_clip_x) ||
		(yc1 < min_clip_y) || (yc1 > max_clip_y) ||
		(xc2 < min_clip_x) || (xc2 > max_clip_x) ||
		(yc2 < min_clip_y) || (yc2 > max_clip_y)) {
		return(0);
	} // end if

	// store vars back
	x1 = xc1;
	y1 = yc1;
	x2 = xc2;
	y2 = yc2;

	return(1);
} // end Clip_Line

///////////////////////////////////////////////////////////

int Draw_Clip_Line(int x0, int y0, int x1, int y1, int color,
	UCHAR *dest_buffer, int lpitch)
{
	// this function draws a wireframe triangle

	int cxs, cys,
		cxe, cye;

	// clip and draw each line
	cxs = x0;
	cys = y0;
	cxe = x1;
	cye = y1;

	// clip the line
	if (Clip_Line(cxs, cys, cxe, cye))
		Draw_Line(cxs, cys, cxe, cye, color, dest_buffer, lpitch);

	// return success
	return(1);
} // end Draw_Clip_Line

///////////////////////////////////////////////////////////

int Draw_Clip_Line16(int x0, int y0, int x1, int y1, int color,
	UCHAR *dest_buffer, int lpitch)
{
	// this function draws a clipped line

	int cxs, cys,
		cxe, cye;

	// clip and draw each line
	cxs = x0;
	cys = y0;
	cxe = x1;
	cye = y1;

	// clip the line
	if (Clip_Line(cxs, cys, cxe, cye))
		Draw_Line16(cxs, cys, cxe, cye, color, dest_buffer, lpitch);

	// return success
	return(1);
} // end Draw_Clip_Line16

///////////////////////////////////////////////////////////

int Draw_Pixel(int x, int y, int color,
	UCHAR *video_buffer, int lpitch)
{
	// this function plots a single pixel at x,y with color

	video_buffer[x + y * lpitch] = color;

	// return success
	return(1);
} // end Draw_Pixel

///////////////////////////////////////////////////////////   

int Draw_Pixel16(int x, int y, int color,
	UCHAR *video_buffer, int lpitch)
{
	// this function plots a single pixel at x,y with color

	((USHORT *)video_buffer)[x + y * (lpitch >> 1)] = color;

	// return success
	return(1);
} // end Draw_Pixel16

///////////////////////////////////////////////////////////   

int Draw_Rectangle(int x1, int y1, int x2, int y2, int color, UCHAR *dest, int lpitch)
{
	for (int y = y1; y <= y2; y++) {
		for (int x = x1; x <= x2; x++) {
			dest[(y * lpitch) + x] = color;
		}
	}

	// return success
	return(1);
} // end Draw_Rectangle

///////////////////////////////////////////////////////////   

int Draw_Rectangle16(int x1, int y1, int x2, int y2, int color, UCHAR *dest, int lpitch)
{
	USHORT *dest_ptr = (USHORT *)dest;

	// recompute pitch in 16-bit words
	lpitch = (lpitch >> 1);

	for (int y = y1; y <= y2; y++) {
		for (int x = x1; x <= x2; x++) {
			dest_ptr[(y * lpitch) + x] = color;
		}
	}

	// return success
	return(1);
} // end Draw_Rectangle

///////////////////////////////////////////////////////////

int Set_Palette_Entry(int color_index, LPPALETTEENTRY color)
{
	// set data in shadow palette
	memcpy(&palette[color_index], color, sizeof(PALETTEENTRY));

	// return success
	return(1);
} // end Set_Palette_Entry

///////////////////////////////////////////////////////////   

int Get_Palette_Entry(int color_index, LPPALETTEENTRY color)
{
	// this function retrieves a palette entry from the color
	// palette

	// copy data out from shadow palette
	memcpy(color, &palette[color_index], sizeof(PALETTEENTRY));

	// return success
	return(1);
} // end Get_Palette_Entry

///////////////////////////////////////////////////////////

int Load_Palette_From_File(const char *filename, LPPALETTEENTRY palette)
{
	// this function loads a palette from disk into a palette
	// structure, but does not set the pallette

	FILE *fp_file; // working file

	int peRed, peGreen, peBlue, peFlags; // used for to store temporary palette colors

	// try and open file
	if ((fp_file = fopen(filename, "r")) == NULL)
		return(0);

	// read in all 256 colors RGBF
	for (int index = 0; index < MAX_COLORS_PALETTE; index++) {
		// read the next entry in
		fscanf(fp_file, "%d %d %d %d", &peRed, &peGreen, &peBlue, &peFlags);

		// store the color in next element of palette
		palette[index].peRed = peRed;
		palette[index].peGreen = peGreen;
		palette[index].peBlue = peBlue;
		palette[index].peFlags = peFlags;
	} // end for index

	// close the file
	fclose(fp_file);

	// return success
	return(1);
} // end Load_Palette_From_Disk

///////////////////////////////////////////////////////////   

int Save_Palette_To_File(const char *filename, LPPALETTEENTRY palette)
{
	// this function saves a palette to disk

	FILE *fp_file; // working file

	// try and open file
	if ((fp_file = fopen(filename, "w")) == NULL)
		return(0);

	// write in all 256 colors RGBF
	for (int index = 0; index < MAX_COLORS_PALETTE; index++) {
		// read the next entry in
		fprintf(fp_file, "\n%d %d %d %d", palette[index].peRed, palette[index].peGreen, palette[index].peBlue, palette[index].peFlags);
	} // end for index

	// close the file
	fclose(fp_file);

	// return success
	return(1);
} // end Save_Palette_To_Disk

///////////////////////////////////////////////////////////

int Save_Palette(LPPALETTEENTRY sav_palette)
{
	// this function saves the current palette 

	memcpy(sav_palette, palette, MAX_COLORS_PALETTE * sizeof(PALETTEENTRY));

	// return success
	return(1);
} // end Save_Palette

///////////////////////////////////////////////////////////   

int Set_Palette(LPPALETTEENTRY set_palette)
{
	// this function writes the sent palette

	// first save the new palette in shadow
	memcpy(palette, set_palette, MAX_COLORS_PALETTE * sizeof(PALETTEENTRY));

	// now set the new palette
	for (int i = 0; i < MAX_COLORS_PALETTE; i++) {
		RETRO_SetColor(i, set_palette[i].peRed, set_palette[i].peGreen, set_palette[i].peBlue);
	}

	// return success
	return(1);
} // end Set_Palette

///////////////////////////////////////////////////////////

int Rotate_Colors(int start_index, int end_index)
{
	// this function rotates the color between start and end

	PALETTEENTRY work_pal[MAX_COLORS_PALETTE]; // working palette

	// get the color palette
	for (int i = start_index; i <= end_index; i++) {
		RETRO_Palette color = RETRO_GetColor(i);
		work_pal[i].peRed = color.r;
		work_pal[i].peGreen = color.g;
		work_pal[i].peBlue = color.b;
		work_pal[i].peFlags = 0;
	}

	// shift the colors
	for (int i = start_index; i < end_index; i++) {
		RETRO_SetColor(i + 1, work_pal[i].peRed, work_pal[i].peGreen, work_pal[i].peBlue);
	}

	// fix up the last color
	RETRO_SetColor(start_index, work_pal[end_index].peRed, work_pal[end_index].peGreen, work_pal[end_index].peBlue);

	// update shadow palette
	for (int i = 0; i < MAX_COLORS_PALETTE; i++) {
		RETRO_Palette color = RETRO_GetColor(i);
		palette[i].peRed = color.r;
		palette[i].peGreen = color.g;
		palette[i].peBlue = color.b;
		palette[i].peFlags = 0;
	}

	// return success
	return(1);
} // end Rotate_Colors

///////////////////////////////////////////////////////////   

int Blink_Colors(int command, BLINKER_PTR new_light, int id)
{
	// this function blinks a set of lights

	static BLINKER lights[256]; // supports up to 256 blinking lights
	static int initialized = 0; // tracks if function has initialized

	// test if this is the first time function has ran
	if (!initialized) {
		// set initialized
		initialized = 1;

		// clear out all structures
		memset((void *)lights, 0, sizeof(lights));

	} // end if

	// now test what command user is sending
	switch (command) {
	case BLINKER_ADD: // add a light to the database
	{
		// run thru database and find an open light
		for (int index = 0; index < 256; index++) {
			// is this light available?
			if (lights[index].state == 0) {
				// set light up
				lights[index] = *new_light;

				// set internal fields up
				lights[index].counter = 0;
				lights[index].state = -1; // off

				// update palette entry
				RETRO_SetColor(lights[index].color_index, lights[index].off_color.peRed, lights[index].off_color.peGreen, lights[index].off_color.peBlue);

				// return id to caller
				return(index);

			} // end if

		} // end for index

	} break;

	case BLINKER_DELETE: // delete the light indicated by id
	{
		// delete the light sent in id 
		if (lights[id].state != 0) {
			// kill the light
			memset((void *)&lights[id], 0, sizeof(BLINKER));

			// return id
			return(id);

		} // end if
		else
			return(-1); // problem


	} break;

	case BLINKER_UPDATE: // update the light indicated by id
	{
		// make sure light is active
		if (lights[id].state != 0) {
			// update on/off parms only
			lights[id].on_color = new_light->on_color;
			lights[id].off_color = new_light->off_color;
			lights[id].on_time = new_light->on_time;
			lights[id].off_time = new_light->off_time;

			// update palette entry
			if (lights[id].state == -1)
				RETRO_SetColor(lights[id].color_index, lights[id].off_color.peRed, lights[id].off_color.peGreen, lights[id].off_color.peBlue);
			else
				RETRO_SetColor(lights[id].color_index, lights[id].on_color.peRed, lights[id].on_color.peGreen, lights[id].on_color.peBlue);

			// return id
			return(id);

		} // end if
		else
			return(-1); // problem

	} break;

	case BLINKER_RUN: // run the algorithm
	{
		// run thru database and process each light
		for (int index = 0; index < 256; index++) {
			// is this active?
			if (lights[index].state == -1) {
				// update counter
				if (++lights[index].counter >= lights[index].off_time) {
					// reset counter
					lights[index].counter = 0;

					// change states 
					lights[index].state = -lights[index].state;

					// update color
					RETRO_SetColor(lights[index].color_index, lights[id].on_color.peRed, lights[id].on_color.peGreen, lights[id].on_color.peBlue);

				} // end if

			} // end if
			else
				if (lights[index].state == 1) {
					// update counter
					if (++lights[index].counter >= lights[index].on_time) {
						// reset counter
						lights[index].counter = 0;

						// change states 
						lights[index].state = -lights[index].state;

						// update color
						RETRO_SetColor(lights[index].color_index, lights[id].off_color.peRed, lights[id].off_color.peGreen, lights[id].off_color.peBlue);

					} // end if
				} // end else if

		} // end for index

	} break;

	default: break;

	} // end switch

	// return success
	return(1);
} // end Blink_Colors

///////////////////////////////////////////////////////////

int Draw_Text_GDI(const char *text, int x, int y, UCHAR color, UCHAR *dest_buffer, int lpitch)
{
	// this function draws the sent text on the sent surface 
	// using color index as the color in the palette

	char c;
	while ((c = *text++) != 0) {
		unsigned char *glyph = RETRO_FontData[c - 32];

		for (int yy = 0; yy < RETRO_FONT_HEIGHT; yy++) {
			for (int xx = 0; xx < RETRO_FONT_WIDTH; xx++) {
				if (glyph[yy] & (1 << xx)) {
					dest_buffer[(y + yy) * lpitch + x + xx] = color;
				}
			}
		}
		x += RETRO_FONT_WIDTH;
	}

	// return success
	return(1);
} // end Draw_Text_GDI

///////////////////////////////////////////////////////////

int Draw_Text_GDI16(const char *text, int x, int y, USHORT color, UCHAR *dest_buffer, int lpitch)
{
	// this function draws the sent text on the sent surface 
	// using color index as the color in the palette

	USHORT *dest_buffer_ptr = (USHORT *)dest_buffer;

	// recompute pitch in 16-bit words
	lpitch = (lpitch >> 1);

	char c;
	while ((c = *text++) != 0) {
		unsigned char *glyph = RETRO_FontData[c - 32];

		for (int yy = 0; yy < RETRO_FONT_HEIGHT; yy++) {
			for (int xx = 0; xx < RETRO_FONT_WIDTH; xx++) {
				if (glyph[yy] & (1 << xx)) {
					dest_buffer_ptr[(y + yy) * lpitch + x + xx] = color;
				}
			}
		}
		x += RETRO_FONT_WIDTH;
	}

	// return success
	return(1);
} // end Draw_Text_GDI16

///////////////////////////////////////////////////////////////////////////////

int Create_Bitmap(BITMAP_IMAGE_PTR image, int x, int y, int width, int height, int bpp = 8)
{
	// this function is used to intialize a bitmap, 8 or 16 bit

	// allocate the memory
	if (!(image->buffer = (UCHAR *)malloc(width * height * (bpp >> 3))))
		return(0);

	// initialize variables
	image->state = BITMAP_STATE_ALIVE;
	image->attr = 0;
	image->width = width;
	image->height = height;
	image->bpp = bpp;
	image->x = x;
	image->y = y;
	image->num_bytes = width * height * (bpp >> 3);

	// clear memory out
	memset(image->buffer, 0, width * height * (bpp >> 3));

	// return success
	return(1);
} // end Create_Bitmap

///////////////////////////////////////////////////////////////////////////////

int Destroy_Bitmap(BITMAP_IMAGE_PTR image)
{
	// this function releases the memory associated with a bitmap

	if (image && image->buffer) {
		// free memory and reset vars
		free(image->buffer);

		// set all vars in structure to 0
		memset(image, 0, sizeof(BITMAP_IMAGE));

		// return success
		return(1);

	} // end if
	else  // invalid entry pointer of the object wasn't initialized
		return(0);
} // end Destroy_Bitmap

///////////////////////////////////////////////////////////

int Draw_Bitmap(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent)
{
	// this function draws the bitmap onto the destination memory surface
	// if transparent is 1 then color 0 (8bit) or 0.0.0 (16bit) will be transparent
	// note this function does NOT clip, so be carefull!!!

	// test if this bitmap is loaded
	if (!(source_bitmap->attr & BITMAP_ATTR_LOADED))
		return(0);

	UCHAR *dest_addr,   // starting address of bitmap in destination
		*source_addr; // starting adddress of bitmap data in source

	UCHAR pixel;        // used to hold pixel value

	int index,          // looping vars
		pixel_x;

	// compute starting destination address
	dest_addr = dest_buffer + source_bitmap->y * lpitch + source_bitmap->x;

	// compute the starting source address
	source_addr = source_bitmap->buffer;

	// is this bitmap transparent
	if (transparent) {
		// copy each line of bitmap into destination with transparency
		for (index = 0; index < source_bitmap->height; index++) {
			// copy the memory
			for (pixel_x = 0; pixel_x < source_bitmap->width; pixel_x++) {
				if (index < screen_height && pixel_x < screen_width) {
					if ((pixel = source_addr[pixel_x]) != 0)
						dest_addr[pixel_x] = pixel;
				}
			} // end if

			// advance all the pointers
			dest_addr += lpitch;
			source_addr += source_bitmap->width;

		} // end for index
	} // end if
	else {
		// non-transparent version
		// copy each line of bitmap into destination
		for (index = 0; index < source_bitmap->height; index++) {
			if (index < screen_height) {
				// copy the memory
				memcpy(dest_addr, source_addr, source_bitmap->width);

				// advance all the pointers
				dest_addr += lpitch;
				source_addr += source_bitmap->width;
			}
		} // end for index
	} // end else

	// return success
	return(1);
} // end Draw_Bitmap

///////////////////////////////////////////////////////////////

int Draw_Bitmap16(BITMAP_IMAGE_PTR source_bitmap, UCHAR *dest_buffer, int lpitch, int transparent)
{
	// this function draws the bitmap onto the destination memory surface
	// if transparent is 1 then color 0 (8bit) or 0.0.0 (16bit) will be transparent
	// note this function does NOT clip, so be carefull!!!

	// test if this bitmap is loaded
	if (!(source_bitmap->attr & BITMAP_ATTR_LOADED))
		return(0);

	USHORT *dest_addr,   // starting address of bitmap in destination
		*source_addr; // starting adddress of bitmap data in source

	USHORT pixel;        // used to hold pixel value

	int index,           // looping vars
		pixel_x,
		lpitch_2 = lpitch >> 1; // lpitch in USHORT terms

	// compute starting destination address
	dest_addr = ((USHORT *)dest_buffer) + source_bitmap->y * lpitch_2 + source_bitmap->x;

	// compute the starting source address
	source_addr = (USHORT *)source_bitmap->buffer;

	// is this bitmap transparent
	if (transparent) {
		// copy each line of bitmap into destination with transparency
		for (index = 0; index < source_bitmap->height; index++) {
			// copy the memory
			for (pixel_x = 0; pixel_x < source_bitmap->width; pixel_x++) {
				if (index < screen_height && pixel_x < screen_width) {
					if ((pixel = source_addr[pixel_x]) != 0)
						dest_addr[pixel_x] = pixel;
				}

			} // end if

			// advance all the pointers
			dest_addr += lpitch_2;
			source_addr += source_bitmap->width;

		} // end for index
	} // end if
	else {
		// non-transparent version
		// copy each line of bitmap into destination

		int source_bytes_per_line = source_bitmap->width * 2;

		for (index = 0; index < source_bitmap->height; index++) {
			if (index < screen_height) {
				// copy the memory
				memcpy(dest_addr, source_addr, source_bytes_per_line);

				// advance all the pointers
				dest_addr += lpitch_2;
				source_addr += source_bitmap->width;
			}
		} // end for index

	} // end else

	// return success
	return(1);
} // end Draw_Bitmap16

///////////////////////////////////////////////////////////////////////////////

int Load_Image_Bitmap(BITMAP_IMAGE_PTR image,  // bitmap image to load with data
	BITMAP_FILE_PTR bitmap,  // bitmap to scan image data from
	int cx, int cy,   // cell or absolute pos. to scan image from
	int mode)        // if 0 then cx,cy is cell position, else cx,cy are absolute coords
{
	// this function extracts a bitmap out of a bitmap file

	// is this a valid bitmap
	if (!image)
		return(0);

	UCHAR *source_ptr,   // working pointers
		*dest_ptr;

	// test the mode of extraction, cell based or absolute
	if (mode == BITMAP_EXTRACT_MODE_CELL) {
		// re-compute x,y
		cx = cx * (image->width + 1) + 1;
		cy = cy * (image->height + 1) + 1;
	} // end if

	// extract bitmap data
	source_ptr = bitmap->buffer + cy * bitmap->bitmapinfoheader.biWidth + cx;

	// assign a pointer to the bimap image
	dest_ptr = (UCHAR *)image->buffer;

	// iterate thru each scanline and copy bitmap
	for (int index_y = 0; index_y < image->height; index_y++) {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr, image->width);

		// advance pointers
		dest_ptr += image->width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} // end for index_y

	// set state to loaded
	image->attr |= BITMAP_ATTR_LOADED;

	// return success
	return(1);
} // end Load_Image_Bitmap

///////////////////////////////////////////////////////////

int Load_Image_Bitmap16(BITMAP_IMAGE_PTR image,  // bitmap image to load with data
	BITMAP_FILE_PTR bitmap,  // bitmap to scan image data from
	int cx, int cy,   // cell or absolute pos. to scan image from
	int mode)        // if 0 then cx,cy is cell position, else 
	// cx,cy are absolute coords
{
	// this function extracts a 16-bit bitmap out of a 16-bit bitmap file

	// is this a valid bitmap
	if (!image)
		return(0);

	// must be a 16bit bitmap
	USHORT *source_ptr,   // working pointers
		*dest_ptr;

	// test the mode of extraction, cell based or absolute
	if (mode == BITMAP_EXTRACT_MODE_CELL) {
		// re-compute x,y
		cx = cx * (image->width + 1) + 1;
		cy = cy * (image->height + 1) + 1;
	} // end if

	// extract bitmap data
	source_ptr = (USHORT *)bitmap->buffer + cy * bitmap->bitmapinfoheader.biWidth + cx;

	// assign a pointer to the bimap image
	dest_ptr = (USHORT *)image->buffer;

	int bytes_per_line = image->width * 2;

	// iterate thru each scanline and copy bitmap
	for (int index_y = 0; index_y < image->height; index_y++) {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr, bytes_per_line);

		// advance pointers
		dest_ptr += image->width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} // end for index_y

	// set state to loaded
	image->attr |= BITMAP_ATTR_LOADED;

	// return success
	return(1);
} // end Load_Image_Bitmap16

///////////////////////////////////////////////////////////

int Copy_Bitmap(BITMAP_IMAGE_PTR dest_bitmap, int dest_x, int dest_y,
	BITMAP_IMAGE_PTR source_bitmap, int source_x, int source_y,
	int width, int height)
{
	// this function copies a bitmap from one source to another

	// make sure the pointers are at least valid
	if (!dest_bitmap || !source_bitmap)
		return(0);

	// do some computations
	int bytes_per_pixel = (source_bitmap->bpp >> 3);

	// create some pointers
	UCHAR *source_ptr = source_bitmap->buffer + (source_x + source_y * source_bitmap->width) * bytes_per_pixel;
	UCHAR *dest_ptr = dest_bitmap->buffer + (dest_x + dest_y * dest_bitmap->width) * bytes_per_pixel;

	// now copying is easy :)
	for (int y = 0; y < height; y++) {
		// copy this line
		memcpy(dest_ptr, source_ptr, bytes_per_pixel * width);

		// advance the pointers
		source_ptr += (source_bitmap->width * bytes_per_pixel);
		dest_ptr += (dest_bitmap->width * bytes_per_pixel);
	} // end for

	// return success
	return(1);
} // end Copy_Bitmap

///////////////////////////////////////////////////////////

int Scroll_Bitmap(BITMAP_IMAGE_PTR image, int dx, int dy = 0)
{
	// this function scrolls a bitmap

	BITMAP_IMAGE temp_image; // temp image buffer

	// are the parms valid 
	if (!image || (dx == 0 && dy == 0))
		return(0);

	// scroll on x-axis first
	if (dx != 0) {
		// step 1: normalize scrolling amount
		dx %= image->width;

		// step 2: which way?
		if (dx > 0) {
			// scroll right
			// create bitmap to hold region that is scrolled around
			Create_Bitmap(&temp_image, 0, 0, dx, image->height, image->bpp);

			// copy region we are going to scroll and wrap around
			Copy_Bitmap(&temp_image, 0, 0, image, image->width - dx, 0, dx, image->height);

			// set some pointers up
			UCHAR *source_ptr = image->buffer;  // start of each line
			int shift = (image->bpp >> 3) * dx;

			// now scroll image to right "scroll" pixels
			for (int y = 0; y < image->height; y++) {
				// scroll the line over
				memmove(source_ptr + shift, source_ptr, (image->width - dx) * (image->bpp >> 3));

				// advance to the next line
				source_ptr += ((image->bpp >> 3) * image->width);
			} // end for

			// and now copy it back
			Copy_Bitmap(image, 0, 0, &temp_image, 0, 0, dx, image->height);

		} // end if
		else {
			// scroll left
			dx = -dx; // invert sign

			// create bitmap to hold region that is scrolled around
			Create_Bitmap(&temp_image, 0, 0, dx, image->height, image->bpp);

			// copy region we are going to scroll and wrap around
			Copy_Bitmap(&temp_image, 0, 0, image, 0, 0, dx, image->height);

			// set some pointers up
			UCHAR *source_ptr = image->buffer;  // start of each line
			int shift = (image->bpp >> 3) * dx;

			// now scroll image to left "scroll" pixels
			for (int y = 0; y < image->height; y++) {
				// scroll the line over
				memmove(source_ptr, source_ptr + shift, (image->width - dx) * (image->bpp >> 3));

				// advance to the next line
				source_ptr += ((image->bpp >> 3) * image->width);
			} // end for

			// and now copy it back
			Copy_Bitmap(image, image->width - dx, 0, &temp_image, 0, 0, dx, image->height);

		} // end else
	} // end scroll on x-axis

	// return success
	return(1);
} // end Scroll_Bitmap

///////////////////////////////////////////////////////////

int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)
{
	// this function is used to flip bottom-up .BMP images

	UCHAR *buffer; // used to perform the image processing
	int index;     // looping index

	// allocate the temporary buffer
	if (!(buffer = (UCHAR *)malloc(bytes_per_line * height)))
		return(0);

	// copy image to work area
	memcpy(buffer, image, bytes_per_line * height);

	// flip vertically
	for (index = 0; index < height; index++)
		memcpy(&image[((height - 1) - index) * bytes_per_line], &buffer[index * bytes_per_line], bytes_per_line);

	// release the memory
	free(buffer);

	// return success
	return(1);
} // end Flip_Bitmap

///////////////////////////////////////////////////////////

int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, const char *filename)
{
	// this function opens a bitmap file and loads the data into bitmap

	FILE *file_handle;  // the file handle
	int index;        // looping index

	UCHAR *temp_buffer = NULL; // used to convert 24 bit images to 16 bit
	OFSTRUCT file_data;          // the file data information

	// open the file if it exists
	if ((file_handle = fopen(filename, "rb")) == NULL)
		return(0);

	// now load the bitmap file header
	fread(&bitmap->bitmapfileheader, sizeof(BITMAPFILEHEADER), 1, file_handle);

	// test if this is a bitmap file
	if (bitmap->bitmapfileheader.bfType != BITMAP_ID) {
		// close the file
		fclose(file_handle);

		// return error
		return(0);
	} // end if

	// now we know this is a bitmap, so read in all the sections

	// first the bitmap infoheader

	// now load the bitmap file header
	fread(&bitmap->bitmapinfoheader, sizeof(BITMAPINFOHEADER), 1, file_handle);

	// now load the color palette if there is one
	if (bitmap->bitmapinfoheader.biBitCount == 8) {
		fread(&bitmap->palette, sizeof(PALETTEENTRY), MAX_COLORS_PALETTE, file_handle);

		// now set all the flags in the palette correctly and fix the reversed 
		// BGR RGBQUAD data format
		for (index = 0; index < MAX_COLORS_PALETTE; index++) {
			// reverse the red and green fields
			int temp_color = bitmap->palette[index].peRed;
			bitmap->palette[index].peRed = bitmap->palette[index].peBlue;
			bitmap->palette[index].peBlue = temp_color;

			// always set the flags word to this
			bitmap->palette[index].peFlags = PC_NOCOLLAPSE;
		} // end for index
	} // end if

	// finally the image data itself
	fseek(file_handle, -(int)(bitmap->bitmapinfoheader.biSizeImage), SEEK_END);

	// now read in the image
	if (bitmap->bitmapinfoheader.biBitCount == 8 || bitmap->bitmapinfoheader.biBitCount == 16) {
		// delete the last image if there was one
		if (bitmap->buffer)
			free(bitmap->buffer);

		// allocate the memory for the image
		if (!(bitmap->buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage))) {
			// close the file
			fclose(file_handle);

			// return error
			return(0);
		} // end if

		// now read it in
		fread(bitmap->buffer, bitmap->bitmapinfoheader.biSizeImage, 1, file_handle);
	} // end if
	else
		if (bitmap->bitmapinfoheader.biBitCount == 24) {
			// allocate temporary buffer to load 24 bit image
			if (!(temp_buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage))) {
				// close the file
				fclose(file_handle);

				// return error
				return(0);
			} // end if

			// allocate final 16 bit storage buffer
			if (!(bitmap->buffer = (UCHAR *)malloc(2 * bitmap->bitmapinfoheader.biWidth * bitmap->bitmapinfoheader.biHeight))) {
				// close the file
				fclose(file_handle);

				// release working buffer
				free(temp_buffer);

				// return error
				return(0);
			} // end if

			// now read the file in
			fread(temp_buffer, bitmap->bitmapinfoheader.biSizeImage, 1, file_handle);

			// now convert each 24 bit RGB value into a 16 bit value
			for (index = 0; index < bitmap->bitmapinfoheader.biWidth * bitmap->bitmapinfoheader.biHeight; index++) {
				// build up 16 bit color word
				USHORT color;

				// build pixel based on format of directdraw surface
				if (dd_pixel_format == DD_PIXEL_FORMAT555) {
					// extract RGB components (in BGR order), note the scaling
					UCHAR blue = (temp_buffer[index * 3 + 0] >> 3),
						green = (temp_buffer[index * 3 + 1] >> 3),
						red = (temp_buffer[index * 3 + 2] >> 3);
					// use the 555 macro
					color = _RGB16BIT555(red, green, blue);
				} // end if 555
				else
					if (dd_pixel_format == DD_PIXEL_FORMAT565) {
						// extract RGB components (in BGR order), note the scaling
						UCHAR blue = (temp_buffer[index * 3 + 0] >> 3),
							green = (temp_buffer[index * 3 + 1] >> 2),
							red = (temp_buffer[index * 3 + 2] >> 3);

						// use the 565 macro
						color = _RGB16BIT565(red, green, blue);

					} // end if 565

				 // write color to buffer
				((USHORT *)bitmap->buffer)[index] = color;
			} // end for index

			// finally write out the correct number of bits
			bitmap->bitmapinfoheader.biBitCount = 16;

			// release working buffer
			free(temp_buffer);
		} // end if 24 bit
		else {
			// serious problem
			return(0);
		} // end else

#if 0
	// write the file info out 
	printf("\nfilename:%s \nsize=%d \nwidth=%d \nheight=%d \nbitsperpixel=%d \ncolors=%d \nimpcolors=%d",
		filename,
		bitmap->bitmapinfoheader.biSizeImage,
		bitmap->bitmapinfoheader.biWidth,
		bitmap->bitmapinfoheader.biHeight,
		bitmap->bitmapinfoheader.biBitCount,
		bitmap->bitmapinfoheader.biClrUsed,
		bitmap->bitmapinfoheader.biClrImportant);
#endif

	// close the file
	fclose(file_handle);

	// flip the bitmap
	Flip_Bitmap(bitmap->buffer, bitmap->bitmapinfoheader.biWidth * (bitmap->bitmapinfoheader.biBitCount / 8), bitmap->bitmapinfoheader.biHeight);

	// return success
	return(1);
} // end Load_Bitmap_File

///////////////////////////////////////////////////////////

int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap)
{
	// this function releases all memory associated with "bitmap"
	if (bitmap->buffer) {
		// release memory
		free(bitmap->buffer);

		// reset pointer
		bitmap->buffer = NULL;

	} // end if

	// return success
	return(1);
} // end Unload_Bitmap_File

///////////////////////////////////////////////////////////

void HLine16(int x1, int x2, int y, int color, UCHAR *vbuffer, int lpitch)
{
	// draw a horizontal line using the memset function

	int temp; // used for temporary storage during endpoint swap

	USHORT *vbuffer2 = (USHORT *)vbuffer; // short pointer to buffer

	// convert pitch to words
	lpitch = lpitch >> 1;

	// perform trivial rejections
	if (y > max_clip_y || y < min_clip_y)
		return;

	// sort x1 and x2, so that x2 > x1
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
	} // end swap

	// perform trivial rejections
	if (x1 > max_clip_x || x2 < min_clip_x)
		return;

	// now clip
	x1 = ((x1 < min_clip_x) ? min_clip_x : x1);
	x2 = ((x2 > max_clip_x) ? max_clip_x : x2);

	// draw the row of pixels
	Mem_Set_WORD((vbuffer2 + (y * lpitch) + x1), color, x2 - x1 + 1);
} // end HLine16

//////////////////////////////////////////////////////////////////////////////

void VLine16(int y1, int y2, int x, int color, UCHAR *vbuffer, int lpitch)
{
	// draw a vertical line, note that a memset function can no longer be
	// used since the pixel addresses are no longer contiguous in memory
	// note that the end points of the line must be on the screen

	USHORT *start_offset; // starting memory offset of line

	int index, // loop index
		temp;  // used for temporary storage during swap

	// convert lpitch to number of words
	lpitch = lpitch >> 1;

	// perform trivial rejections
	if (x > max_clip_x || x < min_clip_x)
		return;

	// make sure y2 > y1
	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
	} // end swap

	// perform trivial rejections
	if (y1 > max_clip_y || y2 < min_clip_y)
		return;

	// now clip
	y1 = ((y1 < min_clip_y) ? min_clip_y : y1);
	y2 = ((y2 > max_clip_y) ? max_clip_y : y2);

	// compute starting position
	start_offset = (USHORT *)vbuffer + (y1 * lpitch) + x;

	// draw line one pixel at a time
	for (index = 0; index <= y2 - y1; index++) {
		// set the pixel
		*start_offset = color;

		// move downward to next line
		start_offset += lpitch;

	} // end for index

} // end VLine16

///////////////////////////////////////////////////////////

void HLine(int x1, int x2, int y, int color, UCHAR *vbuffer, int lpitch)
{
	// draw a horizontal line using the memset function

	int temp; // used for temporary storage during endpoint swap

	// perform trivial rejections
	if (y > max_clip_y || y < min_clip_y)
		return;

	// sort x1 and x2, so that x2 > x1
	if (x1 > x2) {
		temp = x1;
		x1 = x2;
		x2 = temp;
	} // end swap

	// perform trivial rejections
	if (x1 > max_clip_x || x2 < min_clip_x)
		return;

	// now clip
	x1 = ((x1 < min_clip_x) ? min_clip_x : x1);
	x2 = ((x2 > max_clip_x) ? max_clip_x : x2);

	// draw the row of pixels
	memset((UCHAR *)(vbuffer + (y * lpitch) + x1), (UCHAR)color, x2 - x1 + 1);

} // end HLine

//////////////////////////////////////////////////////////////////////////////

void VLine(int y1, int y2, int x, int color, UCHAR *vbuffer, int lpitch)
{
	// draw a vertical line, note that a memset function can no longer be
	// used since the pixel addresses are no longer contiguous in memory
	// note that the end points of the line must be on the screen

	UCHAR *start_offset; // starting memory offset of line

	int index, // loop index
		temp;  // used for temporary storage during swap


	// perform trivial rejections
	if (x > max_clip_x || x < min_clip_x)
		return;

	// make sure y2 > y1
	if (y1 > y2) {
		temp = y1;
		y1 = y2;
		y2 = temp;
	} // end swap

	// perform trivial rejections
	if (y1 > max_clip_y || y2 < min_clip_y)
		return;

	// now clip
	y1 = ((y1 < min_clip_y) ? min_clip_y : y1);
	y2 = ((y2 > max_clip_y) ? max_clip_y : y2);

	// compute starting position
	start_offset = vbuffer + (y1 * lpitch) + x;

	// draw line one pixel at a time
	for (index = 0; index <= y2 - y1; index++) {
		// set the pixel
		*start_offset = (UCHAR)color;

		// move downward to next line
		start_offset += lpitch;

	} // end for index

} // end VLine

///////////////////////////////////////////////////////////

void Screen_Transitions(int effect, UCHAR *vbuffer, int lpitch)
{
	// this function can be called to perform a myraid of screen transitions
	// to the destination buffer, make sure to save and restore the palette
	// when performing color transitions in 8-bit modes

	int pal_reg;         // used as loop counter
	int index;           // used as loop counter
	int red, green, blue;           // used in fad algorithm

	PALETTEENTRY color;              // temporary color
	PALETTEENTRY work_palette[MAX_COLORS_PALETTE];  // used as a working palette
	PALETTEENTRY work_color;         // used in color algorithms

	// test which screen effect is being selected
	switch (effect) {
	case SCREEN_DARKNESS:
	{
		// fade to black

		for (index = 0; index < 80; index++) {
			// get the palette 
			Save_Palette(work_palette);

			// process each color
			for (pal_reg = 1; pal_reg < MAX_COLORS_PALETTE; pal_reg++) {
				// get the entry data
				color = work_palette[pal_reg];

				// test if this color register is already black
				if (color.peRed > 4) color.peRed -= 3;
				else
					color.peRed = 0;

				if (color.peGreen > 4) color.peGreen -= 3;
				else
					color.peGreen = 0;

				if (color.peBlue > 4) color.peBlue -= 3;
				else
					color.peBlue = 0;

				// set the color to a diminished intensity
				work_palette[pal_reg] = color;

			} // end for pal_reg

			// write the palette back out
			Set_Palette(work_palette);

			// wait a bit
			Start_Clock(); Wait_Clock(12);

		} // end for index

	} break;

	case SCREEN_WHITENESS:
	{
		// fade to white
		for (index = 0; index < 64; index++) {
			// get the palette 
			Save_Palette(work_palette);

			// loop thru all palette registers
			for (pal_reg = 0; pal_reg < MAX_COLORS_PALETTE; pal_reg++) {
				// get the entry data
				color = work_palette[pal_reg];

				// make 32 bit copy of color
				red = color.peRed;
				green = color.peGreen;
				blue = color.peBlue;

				if ((red += 4) >= 255)
					red = 255;

				if ((green += 4) >= 255)
					green = 255;

				if ((blue += 4) >= 255)
					blue = 255;

				// store colors back
				color.peRed = red;
				color.peGreen = green;
				color.peBlue = blue;

				// set the color to a diminished intensity
				work_palette[pal_reg] = color;

			} // end for pal_reg

			// write the palette back out
			Set_Palette(work_palette);

			// wait a bit
			Start_Clock(); Wait_Clock(12);

		} // end for index

	} break;

	case SCREEN_REDNESS:
	{
		// fade to red

		for (index = 0; index < 64; index++) {
			// get the palette 
			Save_Palette(work_palette);

			// loop thru all palette registers
			for (pal_reg = 0; pal_reg < MAX_COLORS_PALETTE; pal_reg++) {
				// get the entry data
				color = work_palette[pal_reg];

				// make 32 bit copy of color
				red = color.peRed;
				green = color.peGreen;
				blue = color.peBlue;

				if ((red += 6) >= 255)
					red = 255;

				if ((green -= 4) < 0)
					green = 0;

				if ((blue -= 4) < 0)
					blue = 0;

				// store colors back
				color.peRed = red;
				color.peGreen = green;
				color.peBlue = blue;

				// set the color to a diminished intensity
				work_palette[pal_reg] = color;

			} // end for pal_reg

			// write the palette back out
			Set_Palette(work_palette);

			// wait a bit
			Start_Clock(); Wait_Clock(12);

		} // end for index

	} break;

	case SCREEN_BLUENESS:
	{
		// fade to blue

		for (index = 0; index < 64; index++) {
			// get the palette 
			Save_Palette(work_palette);

			// loop thru all palette registers
			for (pal_reg = 0; pal_reg < MAX_COLORS_PALETTE; pal_reg++) {
				// get the entry data
				color = work_palette[pal_reg];

				// make 32 bit copy of color
				red = color.peRed;
				green = color.peGreen;
				blue = color.peBlue;

				if ((red -= 4) < 0)
					red = 0;

				if ((green -= 4) < 0)
					green = 0;

				if ((blue += 6) >= 255)
					blue = 255;

				// store colors back
				color.peRed = red;
				color.peGreen = green;
				color.peBlue = blue;

				// set the color to a diminished intensity
				work_palette[pal_reg] = color;

			} // end for pal_reg

			// write the palette back out
			Set_Palette(work_palette);

			// wait a bit
			Start_Clock(); Wait_Clock(12);

		} // end for index

	} break;

	case SCREEN_GREENNESS:
	{
		// fade to green
		for (index = 0; index < 64; index++) {
			// get the palette 
			Save_Palette(work_palette);

			// loop thru all palette registers
			for (pal_reg = 0; pal_reg < MAX_COLORS_PALETTE; pal_reg++) {
				// get the entry data
				color = work_palette[pal_reg];

				// make 32 bit copy of color
				red = color.peRed;
				green = color.peGreen;
				blue = color.peBlue;

				if ((red -= 4) < 0)
					red = 0;

				if ((green += 6) >= 255)
					green = 255;

				if ((blue -= 4) < 0)
					blue = 0;

				// store colors back
				color.peRed = red;
				color.peGreen = green;
				color.peBlue = blue;

				// set the color to a diminished intensity
				work_palette[pal_reg] = color;

			} // end for pal_reg

			// write the palette back out
			Set_Palette(work_palette);

			// wait a bit
			Start_Clock(); Wait_Clock(12);
		} // end for index

	} break;

	case SCREEN_SWIPE_X:
	{
		// do a screen wipe from right to left, left to right
		for (index = 0; index < (screen_width / 2); index += 2) {
			// use this as a 1/70th of second time delay

			Start_Clock(); Wait_Clock(12);

			// test screen depth
			if (screen_bpp == 8) {
				// draw two vertical lines at opposite ends of the screen
				VLine(0, (screen_height - 1), (screen_width - 1) - index, 0, vbuffer, lpitch);
				VLine(0, (screen_height - 1), index, 0, vbuffer, lpitch);
				VLine(0, (screen_height - 1), (screen_width - 1) - (index + 1), 0, vbuffer, lpitch);
				VLine(0, (screen_height - 1), index + 1, 0, vbuffer, lpitch);
			} // end if 8-bit mode
			else
				if (screen_bpp == 16) {
					// 16-bit mode draw two vertical lines at opposite ends of the screen
					VLine16(0, (screen_height - 1), (screen_width - 1) - index, 0, vbuffer, lpitch);
					VLine16(0, (screen_height - 1), index, 0, vbuffer, lpitch);
					VLine16(0, (screen_height - 1), (screen_width - 1) - (index + 1), 0, vbuffer, lpitch);
					VLine16(0, (screen_height - 1), index + 1, 0, vbuffer, lpitch);
				} // end if 16-bit mode
		} // end for index

	} break;

	case SCREEN_SWIPE_Y:
	{
		// do a screen wipe from top to bottom, bottom to top
		for (index = 0; index < (screen_height / 2); index += 2) {
			// use this as a 1/70th of second time delay

			Start_Clock(); Wait_Clock(12);

			// test screen depth             
			if (screen_bpp == 8) {
				// draw two horizontal lines at opposite ends of the screen
				HLine(0, (screen_width - 1), (screen_height - 1) - index, 0, vbuffer, lpitch);
				HLine(0, (screen_width - 1), index, 0, vbuffer, lpitch);
				HLine(0, (screen_width - 1), (screen_height - 1) - (index + 1), 0, vbuffer, lpitch);
				HLine(0, (screen_width - 1), index + 1, 0, vbuffer, lpitch);
			} // end if 8-bit mode
			else
				if (screen_bpp == 16) {
					// draw two horizontal lines at opposite ends of the screen
					HLine16(0, (screen_width - 1), (screen_height - 1) - index, 0, vbuffer, lpitch);
					HLine16(0, (screen_width - 1), index, 0, vbuffer, lpitch);
					HLine16(0, (screen_width - 1), (screen_height - 1) - (index + 1), 0, vbuffer, lpitch);
					HLine16(0, (screen_width - 1), index + 1, 0, vbuffer, lpitch);
				} // end if 16-bit mode

		} // end for index


	} break;

	case SCREEN_SCRUNCH:
	{
		// do a screen wipe from top to bottom, bottom to top
		for (index = 0; index < (screen_width / 2); index += 2) {
			// use this as a 1/70th of second time delay

			Start_Clock(); Wait_Clock(12);

			// test screen depth             
			if (screen_bpp == 8) {
				// draw two horizontal lines at opposite ends of the screen
				HLine(0, (screen_width - 1), (screen_height - 1) - index % (screen_height / 2), 0, vbuffer, lpitch);
				HLine(0, (screen_width - 1), index % (screen_height / 2), 0, vbuffer, lpitch);
				HLine(0, (screen_width - 1), (screen_height - 1) - (index % (screen_height / 2) + 1), 0, vbuffer, lpitch);
				HLine(0, (screen_width - 1), index % (screen_height / 2) + 1, 0, vbuffer, lpitch);

				// draw two vertical lines at opposite ends of the screen
				VLine(0, (screen_height - 1), (screen_width - 1) - index, 0, vbuffer, lpitch);
				VLine(0, (screen_height - 1), index, 0, vbuffer, lpitch);
				VLine(0, (screen_height - 1), (screen_width - 1) - (index + 1), 0, vbuffer, lpitch);
				VLine(0, (screen_height - 1), index + 1, 0, vbuffer, lpitch);
			} // end if 8-bit mode
			else
				// test screen depth             
				if (screen_bpp == 16) {
					// draw two horizontal lines at opposite ends of the screen
					HLine16(0, (screen_width - 1), (screen_height - 1) - index % (screen_height / 2), 0, vbuffer, lpitch);
					HLine16(0, (screen_width - 1), index % (screen_height / 2), 0, vbuffer, lpitch);
					HLine16(0, (screen_width - 1), (screen_height - 1) - (index % (screen_height / 2) + 1), 0, vbuffer, lpitch);
					HLine16(0, (screen_width - 1), index % (screen_height / 2) + 1, 0, vbuffer, lpitch);

					// draw two vertical lines at opposite ends of the screen
					VLine16(0, (screen_height - 1), (screen_width - 1) - index, 0, vbuffer, lpitch);
					VLine16(0, (screen_height - 1), index, 0, vbuffer, lpitch);
					VLine16(0, (screen_height - 1), (screen_width - 1) - (index + 1), 0, vbuffer, lpitch);
					VLine16(0, (screen_height - 1), index + 1, 0, vbuffer, lpitch);
				} // end if 8-bit mode

		} // end for index

	} break;


	case SCREEN_DISOLVE:
	{
		// disolve the screen by plotting zillions of little black dots

		if (screen_bpp == 8)
			for (index = 0; index <= screen_width * screen_height * 4; index++)
				Draw_Pixel(rand() % screen_width, rand() % screen_height, 0, vbuffer, lpitch);
		else
			if (screen_bpp == 16)
				for (index = 0; index <= screen_width * screen_height * 4; index++)
					Draw_Pixel16(rand() % screen_width, rand() % screen_height, 0, vbuffer, lpitch);

	} break;

	default:break;

	} // end switch

} // end Screen_Transitions

//////////////////////////////////////////////////////////////////////////////

int Collision_Test(int x1, int y1, int w1, int h1,
	int x2, int y2, int w2, int h2)
{
	// this function tests if the two rects overlap

	// get the radi of each rect
	int width1 = (w1 >> 1) - (w1 >> 3);
	int height1 = (h1 >> 1) - (h1 >> 3);

	int width2 = (w2 >> 1) - (w2 >> 3);
	int height2 = (h2 >> 1) - (h2 >> 3);

	// compute center of each rect
	int cx1 = x1 + width1;
	int cy1 = y1 + height1;

	int cx2 = x2 + width2;
	int cy2 = y2 + height2;

	// compute deltas
	int dx = abs(cx2 - cx1);
	int dy = abs(cy2 - cy1);

	// test if rects overlap
	if (dx < (width1 + width2) && dy < (height1 + height2))
		return(1);
	else
		// else no collision
		return(0);

} // end Collision_Test

///////////////////////////////////////////////////////////

int Color_Scan(int x1, int y1, int x2, int y2,
	UCHAR scan_start, UCHAR scan_end,
	UCHAR *scan_buffer, int scan_lpitch)
{
	// this function implements a crude collision technique
	// based on scanning for a range of colors within a rectangle

	// clip rectangle

	// x coords first    
	if (x1 >= screen_width)
		x1 = screen_width - 1;
	else
		if (x1 < 0)
			x1 = 0;

	if (x2 >= screen_width)
		x2 = screen_width - 1;
	else
		if (x2 < 0)
			x2 = 0;

	// now y-coords
	if (y1 >= screen_height)
		y1 = screen_height - 1;
	else
		if (y1 < 0)
			y1 = 0;

	if (y2 >= screen_height)
		y2 = screen_height - 1;
	else
		if (y2 < 0)
			y2 = 0;

	// scan the region
	scan_buffer += y1 * scan_lpitch;

	for (int scan_y = y1; scan_y <= y2; scan_y++) {
		for (int scan_x = x1; scan_x <= x2; scan_x++) {
			if (scan_buffer[scan_x] >= scan_start && scan_buffer[scan_x] <= scan_end)
				return(1);
		} // end for x

	// move down a line
		scan_buffer += scan_lpitch;

	} // end for y

// return failure
	return(0);

} // end Color_Scan

////////////////////////////////////////////////////////////////

int Color_Scan16(int x1, int y1, int x2, int y2,
	USHORT scan_start, USHORT scan_end,
	UCHAR *scan_buffer, int scan_lpitch)
{
	// this function implements a crude collision technique
	// based on scanning for a range of colors within a rectangle
	// this is the 16-bit version, thus the interpretation of scan_start
	// and end are different, they are they EXACT RGB values you are looking
	// for, thus you can test for 2 values at most, else make them equal to
	// test for one value
	USHORT *scan_buffer2 = (USHORT *)scan_buffer;

	// convert number of bytes per line to number of 16-bit shorts
	scan_lpitch = (scan_lpitch >> 1);

	// clip rectangle

	// x coords first    
	if (x1 >= screen_width)
		x1 = screen_width - 1;
	else
		if (x1 < 0)
			x1 = 0;

	if (x2 >= screen_width)
		x2 = screen_width - 1;
	else
		if (x2 < 0)
			x2 = 0;

	// now y-coords
	if (y1 >= screen_height)
		y1 = screen_height - 1;
	else
		if (y1 < 0)
			y1 = 0;

	if (y2 >= screen_height)
		y2 = screen_height - 1;
	else
		if (y2 < 0)
			y2 = 0;

	// scan the region
	scan_buffer2 += y1 * scan_lpitch;

	for (int scan_y = y1; scan_y <= y2; scan_y++) {
		for (int scan_x = x1; scan_x <= x2; scan_x++) {
			if (scan_buffer2[scan_x] == scan_start || scan_buffer2[scan_x] == scan_end)
				return(1);
		} // end for x

	// move down a line
		scan_buffer2 += scan_lpitch;

	} // end for y

	// return failure
	return(0);

} // end Color_Scan16

////////////////////////////////////////////////////////////////

int Scan_Image_Bitmap(BITMAP_FILE_PTR bitmap,     // bitmap file to scan image data from
	UCHAR *video_buffer, int width, int height, // surface to hold data
	int cx, int cy)              // cell to scan image from
{
	// this function extracts a bitmap out of a bitmap file

	UCHAR *source_ptr,   // working pointers
		*dest_ptr;

	// get the addr to destination surface memory

	// compute position to start scanning bits from
	cx = cx * (width + 1) + 1;
	cy = cy * (height + 1) + 1;

	// extract bitmap data
	source_ptr = bitmap->buffer + cy * bitmap->bitmapinfoheader.biWidth + cx;

	// assign a pointer to the memory surface for manipulation
	dest_ptr = (UCHAR *)video_buffer;

	// iterate thru each scanline and copy bitmap
	for (int index_y = 0; index_y < height; index_y++) {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr, width);

		// advance pointers
		dest_ptr += width;
		source_ptr += bitmap->bitmapinfoheader.biWidth;
	} // end for index_y

	// return success
	return(1);

} // end Scan_Image_Bitmap

//////////////////////////////////////////////////////////////////////////////

void Draw_Top_Tri(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat top

	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs, xe,       // the starting and ending points of the edges
		height;      // the height of the triangle

	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;

	// destination address of next scanline
	UCHAR *dest_addr = NULL;

	// test order of x1 and x2
	if (x2 < x1) {
		temp_x = x2;
		x2 = x1;
		x1 = temp_x;
	} // end if swap

	// compute delta's
	height = y3 - y1;

	dx_left = (x3 - x1) / height;
	dx_right = (x3 - x2) / height;

	// set starting points
	xs = (float)x1;
	xe = (float)x2 + (float)0.5;

	// perform y clipping
	if (y1 < min_clip_y) {
		// compute new xs and ys
		xs = xs + dx_left * (float)(-y1 + min_clip_y);
		xe = xe + dx_right * (float)(-y1 + min_clip_y);

		// reset y1
		y1 = min_clip_y;

	} // end if top is off screen

	if (y3 > max_clip_y)
		y3 = max_clip_y;

	// compute starting address in video memory
	dest_addr = dest_buffer + y1 * mempitch;

	// test if x clipping is needed
	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			memset((UCHAR *)dest_addr + (unsigned int)xs, color, (unsigned int)((int)xe - (int)xs + 1));

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

		} // end for

	} // end if no x clipping needed
	else {
		// clip x axis with slower version

		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			// do x clip
			left = (int)xs;
			right = (int)xe;

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

			// clip line
			if (left < min_clip_x) {
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}

			memset((UCHAR *)dest_addr + (unsigned int)left, color, (unsigned int)(right - left + 1));

		} // end for

	} // end else x clipping needed

} // end Draw_Top_Tri

/////////////////////////////////////////////////////////////////////////////

void Draw_Bottom_Tri(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat bottom

	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs, xe,       // the starting and ending points of the edges
		height;      // the height of the triangle

	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;

	// destination address of next scanline
	UCHAR *dest_addr;

	// test order of x1 and x2
	if (x3 < x2) {
		temp_x = x2;
		x2 = x3;
		x3 = temp_x;
	} // end if swap

	// compute delta's
	height = y3 - y1;

	dx_left = (x2 - x1) / height;
	dx_right = (x3 - x1) / height;

	// set starting points
	xs = (float)x1;
	xe = (float)x1; // +(float)0.5;

	// perform y clipping
	if (y1 < min_clip_y) {
		// compute new xs and ys
		xs = xs + dx_left * (float)(-y1 + min_clip_y);
		xe = xe + dx_right * (float)(-y1 + min_clip_y);

		// reset y1
		y1 = min_clip_y;

	} // end if top is off screen

	if (y3 > max_clip_y)
		y3 = max_clip_y;

	// compute starting address in video memory
	dest_addr = dest_buffer + y1 * mempitch;

	// test if x clipping is needed
	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			memset((UCHAR *)dest_addr + (unsigned int)xs, color, (unsigned int)((int)xe - (int)xs + 1));

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

		} // end for

	} // end if no x clipping needed
	else {
		// clip x axis with slower version

		// draw the triangle

		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			// do x clip
			left = (int)xs;
			right = (int)xe;

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

			// clip line
			if (left < min_clip_x) {
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}

			memset((UCHAR *)dest_addr + (unsigned int)left, color, (unsigned int)(right - left + 1));

		} // end for

	} // end else x clipping needed

} // end Draw_Bottom_Tri

///////////////////////////////////////////////////////////////////////////////

void Draw_Top_Tri16(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *_dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat top

	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs, xe,       // the starting and ending points of the edges
		height;      // the height of the triangle

	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;

	// cast dest buffer to ushort
	USHORT *dest_buffer = (USHORT *)_dest_buffer;

	// destination address of next scanline
	USHORT *dest_addr = NULL;

	// recompute mempitch in 16-bit words
	mempitch = (mempitch >> 1);

	// test order of x1 and x2
	if (x2 < x1) {
		temp_x = x2;
		x2 = x1;
		x1 = temp_x;
	} // end if swap

	// compute delta's
	height = y3 - y1;

	dx_left = (x3 - x1) / height;
	dx_right = (x3 - x2) / height;

	// set starting points
	xs = (float)x1;
	xe = (float)x2; // +(float)0.5;

	// perform y clipping
	if (y1 < min_clip_y) {
		// compute new xs and ys
		xs = xs + dx_left * (float)(-y1 + min_clip_y);
		xe = xe + dx_right * (float)(-y1 + min_clip_y);

		// reset y1
		y1 = min_clip_y;

	} // end if top is off screen

	if (y3 > max_clip_y)
		y3 = max_clip_y;

	// compute starting address in video memory
	dest_addr = dest_buffer + y1 * mempitch;

	// test if x clipping is needed
	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			// draw the line
			Mem_Set_WORD(dest_addr + (unsigned int)(xs), color, (unsigned int)((int)xe - (int)xs + 1));

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

		} // end for

	} // end if no x clipping needed
	else {
		// clip x axis with slower version

		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			// do x clip
			left = (int)xs;
			right = (int)xe;

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

			// clip line
			if (left < min_clip_x) {
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}

			// draw the line
			Mem_Set_WORD(dest_addr + (unsigned int)(left), color, (unsigned int)(right - left + 1));

		} // end for

	} // end else x clipping needed

} // end Draw_Top_Tri16

/////////////////////////////////////////////////////////////////////////////

void Draw_Bottom_Tri16(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *_dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat bottom

	float dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs, xe,       // the starting and ending points of the edges
		height;      // the height of the triangle

	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;

	// cast dest buffer to ushort
	USHORT *dest_buffer = (USHORT *)_dest_buffer;

	// destination address of next scanline
	USHORT *dest_addr = NULL;

	// recompute mempitch in 16-bit words
	mempitch = (mempitch >> 1);

	// test order of x1 and x2
	if (x3 < x2) {
		temp_x = x2;
		x2 = x3;
		x3 = temp_x;
	} // end if swap

	// compute delta's
	height = y3 - y1;

	dx_left = (x2 - x1) / height;
	dx_right = (x3 - x1) / height;

	// set starting points
	xs = (float)x1;
	xe = (float)x1; // +(float)0.5;

	// perform y clipping
	if (y1 < min_clip_y) {
		// compute new xs and ys
		xs = xs + dx_left * (float)(-y1 + min_clip_y);
		xe = xe + dx_right * (float)(-y1 + min_clip_y);

		// reset y1
		y1 = min_clip_y;

	} // end if top is off screen

	if (y3 > max_clip_y)
		y3 = max_clip_y;

	// compute starting address in video memory
	dest_addr = dest_buffer + y1 * mempitch;

	// test if x clipping is needed
	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			// draw the line
			Mem_Set_WORD(dest_addr + (unsigned int)(xs), color, (unsigned int)((int)xe - (int)xs + 1));

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

		} // end for

	} // end if no x clipping needed
	else {
		// clip x axis with slower version

		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			// do x clip
			left = (int)xs;
			right = (int)xe;

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

			// clip line
			if (left < min_clip_x) {
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}
			// draw the line
			Mem_Set_WORD(dest_addr + (unsigned int)(left), color, (unsigned int)(right - left + 1));

		} // end for

	} // end else x clipping needed

} // end Draw_Bottom_Tri16

////////////////////////////////////////////////////////////////////////////////

void Draw_Top_TriFP(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle that has a flat top using fixed point math

	int dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs, xe,       // the starting and ending points of the edges
		height;      // the height of the triangle

	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;

	UCHAR *dest_addr;

	// test for degenerate
	if (y1 == y3 || y2 == y3)
		return;

	// test order of x1 and x2
	if (x2 < x1) {
		temp_x = x2;
		x2 = x1;
		x1 = temp_x;
	} // end if swap

	// compute delta's
	height = y3 - y1;

	dx_left = ((x3 - x1) << FIXP16_SHIFT) / height;
	dx_right = ((x3 - x2) << FIXP16_SHIFT) / height;

	// set starting points
	xs = (x1 << FIXP16_SHIFT);
	xe = (x2 << FIXP16_SHIFT);

	// perform y clipping
	if (y1 < min_clip_y) {
		// compute new xs and ys
		xs = xs + dx_left * (-y1 + min_clip_y);
		xe = xe + dx_right * (-y1 + min_clip_y);

		// reset y1
		y1 = min_clip_y;

	} // end if top is off screen

	if (y3 > max_clip_y)
		y3 = max_clip_y;

	// compute starting address in video memory
	dest_addr = dest_buffer + y1 * mempitch;

	// test if x clipping is needed
	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			memset((UCHAR *)dest_addr + ((xs + FIXP16_ROUND_UP) >> FIXP16_SHIFT),
				color, (((xe - xs + FIXP16_ROUND_UP) >> FIXP16_SHIFT) + 1));

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

		} // end for

	} // end if no x clipping needed
	else {
		// clip x axis with slower version

		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			// do x clip
			left = ((xs + FIXP16_ROUND_UP) >> 16);
			right = ((xe + FIXP16_ROUND_UP) >> 16);

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

			// clip line
			if (left < min_clip_x) {
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}

			memset((UCHAR *)dest_addr + (unsigned int)left,
				color, (unsigned int)(right - left + 1));

		} // end for

	} // end else x clipping needed

} // end Draw_Top_TriFP

/////////////////////////////////////////////////////////////////////////////

void Draw_Bottom_TriFP(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{

	// this function draws a triangle that has a flat bottom using fixed point math

	int dx_right,    // the dx/dy ratio of the right edge of line
		dx_left,     // the dx/dy ratio of the left edge of line
		xs, xe,       // the starting and ending points of the edges
		height;      // the height of the triangle

	int temp_x,        // used during sorting as temps
		temp_y,
		right,         // used by clipping
		left;

	UCHAR *dest_addr;

	if (y1 == y2 || y1 == y3)
		return;

	// test order of x1 and x2
	if (x3 < x2) {
		temp_x = x2;
		x2 = x3;
		x3 = temp_x;

	} // end if swap

	// compute delta's
	height = y3 - y1;

	dx_left = ((x2 - x1) << FIXP16_SHIFT) / height;
	dx_right = ((x3 - x1) << FIXP16_SHIFT) / height;

	// set starting points
	xs = (x1 << FIXP16_SHIFT);
	xe = (x1 << FIXP16_SHIFT);

	// perform y clipping
	if (y1 < min_clip_y) {
		// compute new xs and ys
		xs = xs + dx_left * (-y1 + min_clip_y);
		xe = xe + dx_right * (-y1 + min_clip_y);

		// reset y1
		y1 = min_clip_y;

	} // end if top is off screen

	if (y3 > max_clip_y)
		y3 = max_clip_y;

	// compute starting address in video memory
	dest_addr = dest_buffer + y1 * mempitch;

	// test if x clipping is needed
	if (x1 >= min_clip_x && x1 <= max_clip_x &&
		x2 >= min_clip_x && x2 <= max_clip_x &&
		x3 >= min_clip_x && x3 <= max_clip_x) {
		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			memset((UCHAR *)dest_addr + ((xs + FIXP16_ROUND_UP) >> FIXP16_SHIFT),
				color, (((xe - xs + FIXP16_ROUND_UP) >> FIXP16_SHIFT) + 1));

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

		} // end for

	} // end if no x clipping needed
	else {
		// clip x axis with slower version

		// draw the triangle
		for (temp_y = y1; temp_y <= y3; temp_y++, dest_addr += mempitch) {
			// do x clip
			left = ((xs + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
			right = ((xe + FIXP16_ROUND_UP) >> FIXP16_SHIFT);

			// adjust starting point and ending point
			xs += dx_left;
			xe += dx_right;

			// clip line
			if (left < min_clip_x) {
				left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}

			memset((UCHAR *)dest_addr + left,
				color, (right - left + 1));

		} // end for

	} // end else x clipping needed

} // end Draw_Bottom_TriFP

///////////////////////////////////////////////////////////////////////////////

void Draw_TriangleFP_2D(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle on the destination buffer using fixed point
	// it decomposes all triangles into a pair of flat top, flat bottom

	int temp_x, // used for sorting
		temp_y,
		new_x;

#ifdef DEBUG_ON
	// track rendering stats
	debug_polys_rendered_per_frame++;
#endif

	// test for h lines and v lines
	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
		return;

	// sort p1,p2,p3 in ascending y order
	if (y2 < y1) {
		temp_x = x2;
		temp_y = y2;
		x2 = x1;
		y2 = y1;
		x1 = temp_x;
		y1 = temp_y;
	} // end if

	// now we know that p1 and p2 are in order
	if (y3 < y1) {
		temp_x = x3;
		temp_y = y3;
		x3 = x1;
		y3 = y1;
		x1 = temp_x;
		y1 = temp_y;
	} // end if

	// finally test y3 against y2
	if (y3 < y2) {
		temp_x = x3;
		temp_y = y3;
		x3 = x2;
		y3 = y2;
		x2 = temp_x;
		y2 = temp_y;

	} // end if

	// do trivial rejection tests for clipping
	if (y3<min_clip_y || y1>max_clip_y ||
		(x1 < min_clip_x && x2 < min_clip_x && x3 < min_clip_x) ||
		(x1 > max_clip_x && x2 > max_clip_x && x3 > max_clip_x))
		return;

	// test if top of triangle is flat
	if (y1 == y2) {
		Draw_Top_TriFP(x1, y1, x2, y2, x3, y3, color, dest_buffer, mempitch);
	} // end if
	else
		if (y2 == y3) {
			Draw_Bottom_TriFP(x1, y1, x2, y2, x3, y3, color, dest_buffer, mempitch);
		} // end if bottom is flat
		else {
			// general triangle that's needs to be broken up along long edge
			new_x = x1 + (int)(0.5 + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));

			// draw each sub-triangle
			Draw_Bottom_TriFP(x1, y1, new_x, y2, x2, y2, color, dest_buffer, mempitch);
			Draw_Top_TriFP(x2, y2, new_x, y2, x3, y3, color, dest_buffer, mempitch);

		} // end else

} // end Draw_TriangleFP_2D

/////////////////////////////////////////////////////////////

void Draw_Triangle_2D(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle on the destination buffer
	// it decomposes all triangles into a pair of flat top, flat bottom

	int temp_x, // used for sorting
		temp_y,
		new_x;

#ifdef DEBUG_ON
	// track rendering stats
	debug_polys_rendered_per_frame++;
#endif

	// test for h lines and v lines
	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
		return;

	// sort p1,p2,p3 in ascending y order
	if (y2 < y1) {
		temp_x = x2;
		temp_y = y2;
		x2 = x1;
		y2 = y1;
		x1 = temp_x;
		y1 = temp_y;
	} // end if

	// now we know that p1 and p2 are in order
	if (y3 < y1) {
		temp_x = x3;
		temp_y = y3;
		x3 = x1;
		y3 = y1;
		x1 = temp_x;
		y1 = temp_y;
	} // end if

	// finally test y3 against y2
	if (y3 < y2) {
		temp_x = x3;
		temp_y = y3;
		x3 = x2;
		y3 = y2;
		x2 = temp_x;
		y2 = temp_y;

	} // end if

	// do trivial rejection tests for clipping
	if (y3<min_clip_y || y1>max_clip_y ||
		(x1 < min_clip_x && x2 < min_clip_x && x3 < min_clip_x) ||
		(x1 > max_clip_x && x2 > max_clip_x && x3 > max_clip_x))
		return;

	// test if top of triangle is flat
	if (y1 == y2) {
		Draw_Top_Tri(x1, y1, x2, y2, x3, y3, color, dest_buffer, mempitch);
	} // end if
	else
		if (y2 == y3) {
			Draw_Bottom_Tri(x1, y1, x2, y2, x3, y3, color, dest_buffer, mempitch);
		} // end if bottom is flat
		else {
			// general triangle that's needs to be broken up along long edge
			new_x = x1 + (int)(0.5 + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));

			// draw each sub-triangle
			Draw_Bottom_Tri(x1, y1, new_x, y2, x2, y2, color, dest_buffer, mempitch);
			Draw_Top_Tri(x2, y2, new_x, y2, x3, y3, color, dest_buffer, mempitch);

		} // end else

} // end Draw_Triangle_2D

///////////////////////////////////////////////////////////////////////////////

void Draw_Triangle_2D16(int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{
	// this function draws a triangle on the destination buffer
	// it decomposes all triangles into a pair of flat top, flat bottom

	int temp_x, // used for sorting
		temp_y,
		new_x;

#ifdef DEBUG_ON
	// track rendering stats
	debug_polys_rendered_per_frame++;
#endif

	// test for h lines and v lines
	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
		return;

	// sort p1,p2,p3 in ascending y order
	if (y2 < y1) {
		temp_x = x2;
		temp_y = y2;
		x2 = x1;
		y2 = y1;
		x1 = temp_x;
		y1 = temp_y;
	} // end if

	// now we know that p1 and p2 are in order
	if (y3 < y1) {
		temp_x = x3;
		temp_y = y3;
		x3 = x1;
		y3 = y1;
		x1 = temp_x;
		y1 = temp_y;
	} // end if

	// finally test y3 against y2
	if (y3 < y2) {
		temp_x = x3;
		temp_y = y3;
		x3 = x2;
		y3 = y2;
		x2 = temp_x;
		y2 = temp_y;

	} // end if

	// do trivial rejection tests for clipping
	if (y3<min_clip_y || y1>max_clip_y ||
		(x1 < min_clip_x && x2 < min_clip_x && x3 < min_clip_x) ||
		(x1 > max_clip_x && x2 > max_clip_x && x3 > max_clip_x))
		return;

	// test if top of triangle is flat
	if (y1 == y2) {
		Draw_Top_Tri16(x1, y1, x2, y2, x3, y3, color, dest_buffer, mempitch);
	} // end if
	else
		if (y2 == y3) {
			Draw_Bottom_Tri16(x1, y1, x2, y2, x3, y3, color, dest_buffer, mempitch);
		} // end if bottom is flat
		else {
			// general triangle that's needs to be broken up along long edge
			new_x = x1 + (int)(0.5 + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));

			// draw each sub-triangle
			Draw_Bottom_Tri16(x1, y1, new_x, y2, x2, y2, color, dest_buffer, mempitch);
			Draw_Top_Tri16(x2, y2, new_x, y2, x3, y3, color, dest_buffer, mempitch);

		} // end else

} // end Draw_Triangle_2D16

////////////////////////////////////////////////////////////////////////////////
#if 0
inline void Draw_QuadFP_2D(int x0, int y0,
	int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{
	// this function draws a 2D quadrilateral

	// simply call the triangle function 2x, let it do all the work
	Draw_TriangleFP_2D(x0, y0, x1, y1, x3, y3, color, dest_buffer, mempitch);
	Draw_TriangleFP_2D(x1, y1, x2, y2, x3, y3, color, dest_buffer, mempitch);

} // end Draw_QuadFP_2D
#endif

////////////////////////////////////////////////////////////////////////////

int Fast_Distance_2D(int x, int y)
{
	// this function computes the distance from 0,0 to x,y with 3.5% error

	// first compute the absolute value of x,y
	x = abs(x);
	y = abs(y);

	// compute the minimum of x,y
	int mn = MIN(x, y);

	// return the distance
	return(x + y - (mn >> 1) - (mn >> 2) + (mn >> 4));

} // end Fast_Distance_2D

///////////////////////////////////////////////////////////////////////////////

float Fast_Distance_3D(float fx, float fy, float fz)
{
	// this function computes the distance from the origin to x,y,z

	int temp;  // used for swaping
	int x, y, z; // used for algorithm

	// make sure values are all positive
	x = fabs(fx) * 1024;
	y = fabs(fy) * 1024;
	z = fabs(fz) * 1024;

	// sort values
	if (y < x) SWAP(x, y, temp)

		if (z < y) SWAP(y, z, temp)

			if (y < x) SWAP(x, y, temp)

				int dist = (z + 11 * (y >> 5) + (x >> 2));

	// compute distance with 8% error
	return((float)(dist >> 10));

} // end Fast_Distance_3D

///////////////////////////////////////////////////////////////////////////////

int Find_Bounding_Box_Poly2D(POLYGON2D_PTR poly,
	float &min_x, float &max_x,
	float &min_y, float &max_y)
{
	// this function finds the bounding box of a 2D polygon 
	// and returns the values in the sent vars

	// is this poly valid?
	if (poly->num_verts == 0)
		return(0);

	// initialize output vars (note they are pointers)
	// also note that the algorithm assumes local coordinates
	// that is, the poly verts are relative to 0,0
	max_x = max_y = min_x = min_y = 0;

	// process each vertex
	for (int index = 0; index < poly->num_verts; index++) {
		// update vars - run min/max seek
		if (poly->vlist[index].x > max_x)
			max_x = poly->vlist[index].x;

		if (poly->vlist[index].x < min_x)
			min_x = poly->vlist[index].x;

		if (poly->vlist[index].y > max_y)
			max_y = poly->vlist[index].y;

		if (poly->vlist[index].y < min_y)
			min_y = poly->vlist[index].y;

	} // end for index

	// return success
	return(1);

} // end Find_Bounding_Box_Poly2D

////////////////////////////////////////////////////////////////

void Draw_Filled_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int mempitch)
{
	// this function draws a general n sided polygon 

	int ydiff1, ydiff2,         // difference between starting x and ending x
		xdiff1, xdiff2,         // difference between starting y and ending y
		start,                  // starting offset of line between edges
		length,                 // distance from edge 1 to edge 2
		errorterm1, errorterm2, // error terms for edges 1 & 2
		offset1, offset2,       // offset of current pixel in edges 1 & 2
		count1, count2,         // increment count for edges 1 & 2
		xunit1, xunit2;         // unit to advance x offset for edges 1 & 2

#ifdef DEBUG_ON
	// track rendering stats
	debug_polys_rendered_per_frame++;
#endif

	// initialize count of number of edges drawn:
	int edgecount = poly->num_verts - 1;

	// determine which vertex is at top of polygon:

	int firstvert = 0;         // start by assuming vertex 0 is at top

	int min_y = poly->vlist[0].y; // find y coordinate of vertex 0

	for (int index = 1; index < poly->num_verts; index++) {
		// Search thru vertices
		if ((poly->vlist[index].y) < min_y) {
			// is another vertex higher?
			firstvert = index;
			min_y = poly->vlist[index].y;
		} // end if

	} // end for index

	// finding starting and ending vertices of first two edges:
	int startvert1 = firstvert;      // get starting vertex of edge 1
	int startvert2 = firstvert;      // get starting vertex of edge 2
	int xstart1 = poly->vlist[startvert1].x + poly->x0;
	int ystart1 = poly->vlist[startvert1].y + poly->y0;
	int xstart2 = poly->vlist[startvert2].x + poly->x0;
	int ystart2 = poly->vlist[startvert2].y + poly->y0;
	int endvert1 = startvert1 - 1;           // get ending vertex of edge 1

	if (endvert1 < 0)
		endvert1 = poly->num_verts - 1;    // check for wrap

	int xend1 = poly->vlist[endvert1].x + poly->x0;      // get x & y coordinates
	int yend1 = poly->vlist[endvert1].y + poly->y0;      // of ending vertices
	int endvert2 = startvert2 + 1;           // get ending vertex of edge 2

	if (endvert2 == (poly->num_verts))
		endvert2 = 0;  // Check for wrap

	int xend2 = poly->vlist[endvert2].x + poly->x0;      // get x & y coordinates
	int yend2 = poly->vlist[endvert2].y + poly->y0;      // of ending vertices

	// draw the polygon:

	while (edgecount > 0) {
		// continue drawing until all edges drawn
		offset1 = mempitch * ystart1 + xstart1;  // offset of edge 1
		offset2 = mempitch * ystart2 + xstart2;  // offset of edge 2

		// initialize error terms
		// for edges 1 & 2
		errorterm1 = 0;
		errorterm2 = 0;

		// get absolute value of
		if ((ydiff1 = yend1 - ystart1) < 0)
			ydiff1 = -ydiff1;

		// x & y lengths of edges
		if ((ydiff2 = yend2 - ystart2) < 0)
			ydiff2 = -ydiff2;

		if ((xdiff1 = xend1 - xstart1) < 0) {
			// get value of length
			xunit1 = -1;                    // calculate X increment
			xdiff1 = -xdiff1;
		} // end if
		else {
			xunit1 = 1;
		} // end else

		if ((xdiff2 = xend2 - xstart2) < 0) {
			// Get value of length
			xunit2 = -1;                   // calculate X increment
			xdiff2 = -xdiff2;
		} // end else
		else {
			xunit2 = 1;
		} // end else

		// choose which of four routines to use
		if (xdiff1 > ydiff1) {
			// if x length of edge 1 is greater than y length
			if (xdiff2 > ydiff2) {
				// if X length of edge 2 is greater than y length

				// increment edge 1 on X and edge 2 on X:
				count1 = xdiff1;    // count for x increment on edge 1
				count2 = xdiff2;    // count for x increment on edge 2

				while (count1 && count2) {
					// continue drawing until one edge is done
					// calculate edge 1:
					while ((errorterm1 < xdiff1) && (count1 > 0)) {
						// finished w/edge 1?
						if (count1--) {
							// count down on edge 1
							offset1 += xunit1;  // increment pixel offset
							xstart1 += xunit1;
						} // end if

						errorterm1 += ydiff1; // increment error term

						if (errorterm1 < xdiff1) {  // if not more than XDIFF
							vbuffer[offset1] = (UCHAR)poly->color; // ...plot a pixel
						} // end if

					} // end while

					errorterm1 -= xdiff1; // if time to increment X, restore error term

					// calculate edge 2:

					while ((errorterm2 < xdiff2) && (count2 > 0)) {
						// finished w/edge 2?
						if (count2--) {
							// count down on edge 2
							offset2 += xunit2;  // increment pixel offset
							xstart2 += xunit2;
						} // end if

						errorterm2 += ydiff2; // increment error term

						if (errorterm2 < xdiff2) {  // if not more than XDIFF
							vbuffer[offset2] = (UCHAR)poly->color;  // ...plot a pixel
						} // end if

					} // end while

					errorterm2 -= xdiff2; // if time to increment X, restore error term

					// draw line from edge 1 to edge 2:

					length = offset2 - offset1; // determine length of horizontal line

					if (length < 0) { // if negative...
						length = -length;       // make it positive
						start = offset2;        // and set START to edge 2
					} // end if
					else
						start = offset1;     // else set START to edge 1

					for (int index = start; index < start + length + 1; index++) {  // From edge to edge...
						vbuffer[index] = (UCHAR)poly->color;         // ...draw the line
					} // end for index

					offset1 += mempitch;           // advance edge 1 offset to next line
					ystart1++;
					offset2 += mempitch;           // advance edge 2 offset to next line
					ystart2++;

				} // end if

			} // end if
			else {
				// increment edge 1 on X and edge 2 on Y:
				count1 = xdiff1;    // count for X increment on edge 1
				count2 = ydiff2;    // count for Y increment on edge 2

				while (count1 && count2) {  // continue drawing until one edge is done
					// calculate edge 1:
					while ((errorterm1 < xdiff1) && (count1 > 0)) { // finished w/edge 1?
						if (count1--) {
							// count down on edge 1
							offset1 += xunit1;  // increment pixel offset
							xstart1 += xunit1;
						} // end if

						errorterm1 += ydiff1; // increment error term

						if (errorterm1 < xdiff1) {  // If not more than XDIFF
							vbuffer[offset1] = (UCHAR)poly->color; // ...plot a pixel
						} // end if

					} // end while

					errorterm1 -= xdiff1; // If time to increment X, restore error term

					// calculate edge 2:
					errorterm2 += xdiff2; // increment error term

					if (errorterm2 >= ydiff2) { // if time to increment Y...
						errorterm2 -= ydiff2;        // ...restore error term
						offset2 += xunit2;           // ...and advance offset to next pixel
						xstart2 += xunit2;
					} // end if

					count2--;

					// draw line from edge 1 to edge 2:

					length = offset2 - offset1; // determine length of horizontal line

					if (length < 0) { // if negative...
						length = -length;       // ...make it positive
						start = offset2;        // and set START to edge 2
					} // end if
					else
						start = offset1;        // else set START to edge 1

					for (int index = start; index < start + length + 1; index++)  // from edge to edge
					{
						vbuffer[index] = (UCHAR)poly->color;         // ...draw the line
					} // end for index

					offset1 += mempitch;           // advance edge 1 offset to next line
					ystart1++;
					offset2 += mempitch;           // advance edge 2 offset to next line
					ystart2++;

				} // end while
			} // end if
		} // end if
		else {
			if (xdiff2 > ydiff2) {
				// increment edge 1 on Y and edge 2 on X:

				count1 = ydiff1;  // count for Y increment on edge 1
				count2 = xdiff2;  // count for X increment on edge 2

				while (count1 && count2) {  // continue drawing until one edge is done
					// calculate edge 1:

					errorterm1 += xdiff1; // Increment error term

					if (errorterm1 >= ydiff1) {  // if time to increment Y...
						errorterm1 -= ydiff1;         // ...restore error term
						offset1 += xunit1;            // ...and advance offset to next pixel
						xstart1 += xunit1;
					} // end if

					count1--;

					// Calculate edge 2:

					while ((errorterm2 < xdiff2) && (count2 > 0)) { // finished w/edge 1?
						if (count2--) { // count down on edge 2
							offset2 += xunit2;  // increment pixel offset
							xstart2 += xunit2;
						} // end if

						errorterm2 += ydiff2; // increment error term

						if (errorterm2 < xdiff2) {  // if not more than XDIFF
							vbuffer[offset2] = (UCHAR)poly->color; // ...plot a pixel
						} // end if
					} // end while

					errorterm2 -= xdiff2;  // if time to increment X, restore error term

					// draw line from edge 1 to edge 2:

					length = offset2 - offset1; // determine length of horizontal line

					if (length < 0) {    // if negative...
						length = -length;  // ...make it positive
						start = offset2;   // and set START to edge 2
					} // end if
					else
						start = offset1;  // else set START to edge 1

					for (int index = start; index < start + length + 1; index++) // from edge to edge...
					{
						vbuffer[index] = (UCHAR)poly->color;      // ...draw the line
					} // end for index

					offset1 += mempitch;         // advance edge 1 offset to next line
					ystart1++;
					offset2 += mempitch;         // advance edge 2 offset to next line
					ystart2++;

				} // end if
			} // end if
			else {
				// increment edge 1 on Y and edge 2 on Y:
				count1 = ydiff1;  // count for Y increment on edge 1
				count2 = ydiff2;  // count for Y increment on edge 2

				while (count1 && count2) {
					// continue drawing until one edge is done
					// calculate edge 1:
					errorterm1 += xdiff1;  // increment error term

					if (errorterm1 >= ydiff1) {                           // if time to increment Y
						errorterm1 -= ydiff1;         // ...restore error term
						offset1 += xunit1;            // ...and advance offset to next pixel
						xstart1 += xunit1;
					} // end if

					count1--;

					// calculate edge 2:
					errorterm2 += xdiff2;            // increment error term

					if (errorterm2 >= ydiff2) {                           // if time to increment Y
						errorterm2 -= ydiff2;         // ...restore error term
						offset2 += xunit2;            // ...and advance offset to next pixel
						xstart2 += xunit2;
					} // end if

					--count2;

					// draw line from edge 1 to edge 2:

					length = offset2 - offset1;  // determine length of horizontal line

					if (length < 0) {
						// if negative...
						length = -length;        // ...make it positive
						start = offset2;         // and set START to edge 2
					} // end if
					else
						start = offset1;         // else set START to edge 1

					for (int index = start; index < start + length + 1; index++) { // from edge to edge
						vbuffer[index] = (UCHAR)poly->color;   // ...draw the linee
					} // end for index

					offset1 += mempitch;            // advance edge 1 offset to next line
					ystart1++;
					offset2 += mempitch;            // advance edge 2 offset to next line
					ystart2++;

				} // end while

			} // end else

		} // end if

		// another edge (at least) is complete. Start next edge, if any.
		if (!count1) {                      // if edge 1 is complete...
			--edgecount;           // decrement the edge count
			startvert1 = endvert1;   // make ending vertex into start vertex
			--endvert1;            // and get new ending vertex

			if (endvert1 < 0)
				endvert1 = poly->num_verts - 1; // check for wrap

			xend1 = poly->vlist[endvert1].x + poly->x0;  // get x & y of new end vertex
			yend1 = poly->vlist[endvert1].y + poly->y0;
		} // end if

		if (!count2) {                     // if edge 2 is complete...
			--edgecount;          // decrement the edge count
			startvert2 = endvert2;  // make ending vertex into start vertex
			endvert2++;           // and get new ending vertex

			if (endvert2 == (poly->num_verts))
				endvert2 = 0;                // check for wrap

			xend2 = poly->vlist[endvert2].x + poly->x0;  // get x & y of new end vertex
			yend2 = poly->vlist[endvert2].y + poly->y0;

		} // end if

	} // end while

} // end Draw_Filled_Polygon2D

///////////////////////////////////////////////////////////////

void Draw_Filled_Polygon2D16(POLYGON2D_PTR poly, UCHAR *_vbuffer, int mempitch)
{
	// this function draws a general n sided polygon 

	int ydiff1, ydiff2,         // difference between starting x and ending x
		xdiff1, xdiff2,         // difference between starting y and ending y
		start,                  // starting offset of line between edges
		length,                 // distance from edge 1 to edge 2
		errorterm1, errorterm2, // error terms for edges 1 & 2
		offset1, offset2,       // offset of current pixel in edges 1 & 2
		count1, count2,         // increment count for edges 1 & 2
		xunit1, xunit2;         // unit to advance x offset for edges 1 & 2

#ifdef DEBUG_ON
	// track rendering stats
	debug_polys_rendered_per_frame++;
#endif

	// recast vbuffer into short version since this is a 16 bit mode
	USHORT *vbuffer = (USHORT *)_vbuffer;

	// convert mempitch into WORD or 16bit stride
	mempitch = (mempitch >> 1);

	// initialize count of number of edges drawn:
	int edgecount = poly->num_verts - 1;

	// determine which vertex is at top of polygon:

	int firstvert = 0;         // start by assuming vertex 0 is at top

	int min_y = poly->vlist[0].y; // find y coordinate of vertex 0

	for (int index = 1; index < poly->num_verts; index++) {
		// Search thru vertices
		if ((poly->vlist[index].y) < min_y) {
			// is another vertex higher?
			firstvert = index;
			min_y = poly->vlist[index].y;
		} // end if

	} // end for index

	// finding starting and ending vertices of first two edges:
	int startvert1 = firstvert;      // get starting vertex of edge 1
	int startvert2 = firstvert;      // get starting vertex of edge 2
	int xstart1 = poly->vlist[startvert1].x + poly->x0;
	int ystart1 = poly->vlist[startvert1].y + poly->y0;
	int xstart2 = poly->vlist[startvert2].x + poly->x0;
	int ystart2 = poly->vlist[startvert2].y + poly->y0;
	int endvert1 = startvert1 - 1;           // get ending vertex of edge 1

	if (endvert1 < 0)
		endvert1 = poly->num_verts - 1;    // check for wrap

	int xend1 = poly->vlist[endvert1].x + poly->x0;      // get x & y coordinates
	int yend1 = poly->vlist[endvert1].y + poly->y0;      // of ending vertices
	int endvert2 = startvert2 + 1;           // get ending vertex of edge 2

	if (endvert2 == (poly->num_verts))
		endvert2 = 0;  // Check for wrap

	int xend2 = poly->vlist[endvert2].x + poly->x0;      // get x & y coordinates
	int yend2 = poly->vlist[endvert2].y + poly->y0;      // of ending vertices

	// draw the polygon:

	while (edgecount > 0) {
		// continue drawing until all edges drawn
		offset1 = mempitch * ystart1 + xstart1;  // offset of edge 1
		offset2 = mempitch * ystart2 + xstart2;  // offset of edge 2

		// initialize error terms
		// for edges 1 & 2
		errorterm1 = 0;
		errorterm2 = 0;

		// get absolute value of
		if ((ydiff1 = yend1 - ystart1) < 0)
			ydiff1 = -ydiff1;

		// x & y lengths of edges
		if ((ydiff2 = yend2 - ystart2) < 0)
			ydiff2 = -ydiff2;

		if ((xdiff1 = xend1 - xstart1) < 0) {
			// get value of length
			xunit1 = -1;                    // calculate X increment
			xdiff1 = -xdiff1;
		} // end if
		else {
			xunit1 = 1;
		} // end else

		if ((xdiff2 = xend2 - xstart2) < 0) {
			// Get value of length
			xunit2 = -1;                   // calculate X increment
			xdiff2 = -xdiff2;
		} // end else
		else {
			xunit2 = 1;
		} // end else

		// choose which of four routines to use
		if (xdiff1 > ydiff1) {
			// if x length of edge 1 is greater than y length
			if (xdiff2 > ydiff2) {
				// if X length of edge 2 is greater than y length

				// increment edge 1 on X and edge 2 on X:
				count1 = xdiff1;    // count for x increment on edge 1
				count2 = xdiff2;    // count for x increment on edge 2

				while (count1 && count2) {
					// continue drawing until one edge is done
					// calculate edge 1:
					while ((errorterm1 < xdiff1) && (count1 > 0)) {
						// finished w/edge 1?
						if (count1--) {
							// count down on edge 1
							offset1 += xunit1;  // increment pixel offset
							xstart1 += xunit1;
						} // end if

						errorterm1 += ydiff1; // increment error term

						if (errorterm1 < xdiff1) {  // if not more than XDIFF
							vbuffer[offset1] = (USHORT)poly->color; // ...plot a pixel
						} // end if

					} // end while

					errorterm1 -= xdiff1; // if time to increment X, restore error term

					// calculate edge 2:

					while ((errorterm2 < xdiff2) && (count2 > 0)) {
						// finished w/edge 2?
						if (count2--) {
							// count down on edge 2
							offset2 += xunit2;  // increment pixel offset
							xstart2 += xunit2;
						} // end if

						errorterm2 += ydiff2; // increment error term

						if (errorterm2 < xdiff2) {  // if not more than XDIFF
							vbuffer[offset2] = (USHORT)poly->color;  // ...plot a pixel
						} // end if

					} // end while

					errorterm2 -= xdiff2; // if time to increment X, restore error term

					// draw line from edge 1 to edge 2:

					length = offset2 - offset1; // determine length of horizontal line

					if (length < 0) { // if negative...
						length = -length;       // make it positive
						start = offset2;        // and set START to edge 2
					} // end if
					else
						start = offset1;     // else set START to edge 1

					for (int index = start; index < start + length + 1; index++) {  // From edge to edge...
						vbuffer[index] = (USHORT)poly->color;         // ...draw the line
					} // end for index

					offset1 += mempitch;           // advance edge 1 offset to next line
					ystart1++;
					offset2 += mempitch;           // advance edge 2 offset to next line
					ystart2++;

				} // end if

			} // end if
			else {
				// increment edge 1 on X and edge 2 on Y:
				count1 = xdiff1;    // count for X increment on edge 1
				count2 = ydiff2;    // count for Y increment on edge 2

				while (count1 && count2) {  // continue drawing until one edge is done
					// calculate edge 1:
					while ((errorterm1 < xdiff1) && (count1 > 0)) { // finished w/edge 1?
						if (count1--) {
							// count down on edge 1
							offset1 += xunit1;  // increment pixel offset
							xstart1 += xunit1;
						} // end if

						errorterm1 += ydiff1; // increment error term

						if (errorterm1 < xdiff1) {  // If not more than XDIFF
							vbuffer[offset1] = (USHORT)poly->color; // ...plot a pixel
						} // end if

					} // end while

					errorterm1 -= xdiff1; // If time to increment X, restore error term

					// calculate edge 2:
					errorterm2 += xdiff2; // increment error term

					if (errorterm2 >= ydiff2) { // if time to increment Y...
						errorterm2 -= ydiff2;        // ...restore error term
						offset2 += xunit2;           // ...and advance offset to next pixel
						xstart2 += xunit2;
					} // end if

					count2--;

					// draw line from edge 1 to edge 2:

					length = offset2 - offset1; // determine length of horizontal line

					if (length < 0) { // if negative...
						length = -length;       // ...make it positive
						start = offset2;        // and set START to edge 2
					} // end if
					else
						start = offset1;        // else set START to edge 1

					for (int index = start; index < start + length + 1; index++)  // from edge to edge
					{
						vbuffer[index] = (USHORT)poly->color;         // ...draw the line
					} // end for index

					offset1 += mempitch;           // advance edge 1 offset to next line
					ystart1++;
					offset2 += mempitch;           // advance edge 2 offset to next line
					ystart2++;

				} // end while
			} // end if
		} // end if
		else {
			if (xdiff2 > ydiff2) {
				// increment edge 1 on Y and edge 2 on X:

				count1 = ydiff1;  // count for Y increment on edge 1
				count2 = xdiff2;  // count for X increment on edge 2

				while (count1 && count2) {  // continue drawing until one edge is done
					// calculate edge 1:

					errorterm1 += xdiff1; // Increment error term

					if (errorterm1 >= ydiff1) {  // if time to increment Y...
						errorterm1 -= ydiff1;         // ...restore error term
						offset1 += xunit1;            // ...and advance offset to next pixel
						xstart1 += xunit1;
					} // end if

					count1--;

					// Calculate edge 2:

					while ((errorterm2 < xdiff2) && (count2 > 0)) { // finished w/edge 1?
						if (count2--) { // count down on edge 2
							offset2 += xunit2;  // increment pixel offset
							xstart2 += xunit2;
						} // end if

						errorterm2 += ydiff2; // increment error term

						if (errorterm2 < xdiff2) {  // if not more than XDIFF
							vbuffer[offset2] = (USHORT)poly->color; // ...plot a pixel
						} // end if
					} // end while

					errorterm2 -= xdiff2;  // if time to increment X, restore error term

					// draw line from edge 1 to edge 2:

					length = offset2 - offset1; // determine length of horizontal line

					if (length < 0) {    // if negative...
						length = -length;  // ...make it positive
						start = offset2;   // and set START to edge 2
					} // end if
					else
						start = offset1;  // else set START to edge 1

					for (int index = start; index < start + length + 1; index++) // from edge to edge...
					{
						vbuffer[index] = (USHORT)poly->color;      // ...draw the line
					} // end for index

					offset1 += mempitch;         // advance edge 1 offset to next line
					ystart1++;
					offset2 += mempitch;         // advance edge 2 offset to next line
					ystart2++;

				} // end if
			} // end if
			else {
				// increment edge 1 on Y and edge 2 on Y:
				count1 = ydiff1;  // count for Y increment on edge 1
				count2 = ydiff2;  // count for Y increment on edge 2

				while (count1 && count2) {
					// continue drawing until one edge is done
					// calculate edge 1:
					errorterm1 += xdiff1;  // increment error term

					if (errorterm1 >= ydiff1) {                           // if time to increment Y
						errorterm1 -= ydiff1;         // ...restore error term
						offset1 += xunit1;            // ...and advance offset to next pixel
						xstart1 += xunit1;
					} // end if

					count1--;

					// calculate edge 2:
					errorterm2 += xdiff2;            // increment error term

					if (errorterm2 >= ydiff2) {                           // if time to increment Y
						errorterm2 -= ydiff2;         // ...restore error term
						offset2 += xunit2;            // ...and advance offset to next pixel
						xstart2 += xunit2;
					} // end if

					--count2;

					// draw line from edge 1 to edge 2:

					length = offset2 - offset1;  // determine length of horizontal line

					if (length < 0) {
						// if negative...
						length = -length;        // ...make it positive
						start = offset2;         // and set START to edge 2
					} // end if
					else
						start = offset1;         // else set START to edge 1

					for (int index = start; index < start + length + 1; index++) { // from edge to edge
						vbuffer[index] = (USHORT)poly->color;   // ...draw the linee
					} // end for index

					offset1 += mempitch;            // advance edge 1 offset to next line
					ystart1++;
					offset2 += mempitch;            // advance edge 2 offset to next line
					ystart2++;

				} // end while

			} // end else

		} // end if

		// another edge (at least) is complete. Start next edge, if any.
		if (!count1) {                      // if edge 1 is complete...
			--edgecount;           // decrement the edge count
			startvert1 = endvert1;   // make ending vertex into start vertex
			--endvert1;            // and get new ending vertex

			if (endvert1 < 0)
				endvert1 = poly->num_verts - 1; // check for wrap

			xend1 = poly->vlist[endvert1].x + poly->x0;  // get x & y of new end vertex
			yend1 = poly->vlist[endvert1].y + poly->y0;
		} // end if

		if (!count2) {                     // if edge 2 is complete...
			--edgecount;          // decrement the edge count
			startvert2 = endvert2;  // make ending vertex into start vertex
			endvert2++;           // and get new ending vertex

			if (endvert2 == (poly->num_verts))
				endvert2 = 0;                // check for wrap

			xend2 = poly->vlist[endvert2].x + poly->x0;  // get x & y of new end vertex
			yend2 = poly->vlist[endvert2].y + poly->y0;

		} // end if

	} // end while

} // end Draw_Filled_Polygon2D16

///////////////////////////////////////////////////////////////

void Build_Sin_Cos_Tables(void)
{

	// create sin/cos lookup table
	// note the creation of one extra element; 360
	// this helps with logic in using the tables

	// generate the tables 0 - 360 inclusive
	for (int ang = 0; ang <= 360; ang++) {
		// convert ang to radians
		float theta = (float)ang * PI / (float)180;

		// insert next entry into table
		cos_look[ang] = cos(theta);
		sin_look[ang] = sin(theta);

	} // end for ang

} // end Build_Sin_Cos_Tables

//////////////////////////////////////////////////////////////

int Translate_Polygon2D(POLYGON2D_PTR poly, int dx, int dy)
{
	// this function translates the center of a polygon

	// test for valid pointer
	if (!poly)
		return(0);

	// translate
	poly->x0 += dx;
	poly->y0 += dy;

	// return success
	return(1);

} // end Translate_Polygon2D

///////////////////////////////////////////////////////////////

int Rotate_Polygon2D(POLYGON2D_PTR poly, int theta)
{
	// this function rotates the local coordinates of the polygon

	// test for valid pointer
	if (!poly)
		return(0);

	// test for negative rotation angle
	if (theta < 0)
		theta += 360;

	// loop and rotate each point, very crude, no lookup!!!
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++) {

		// perform rotation
		float xr = (float)poly->vlist[curr_vert].x * cos_look[theta] -
			(float)poly->vlist[curr_vert].y * sin_look[theta];

		float yr = (float)poly->vlist[curr_vert].x * sin_look[theta] +
			(float)poly->vlist[curr_vert].y * cos_look[theta];

		// store result back
		poly->vlist[curr_vert].x = xr;
		poly->vlist[curr_vert].y = yr;

	} // end for curr_vert

	// return success
	return(1);

} // end Rotate_Polygon2D

////////////////////////////////////////////////////////

int Scale_Polygon2D(POLYGON2D_PTR poly, float sx, float sy)
{
	// this function scalesthe local coordinates of the polygon

	// test for valid pointer
	if (!poly)
		return(0);

	// loop and scale each point
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++) {
		// scale and store result back
		poly->vlist[curr_vert].x *= sx;
		poly->vlist[curr_vert].y *= sy;

	} // end for curr_vert

	// return success
	return(1);

} // end Scale_Polygon2D

///////////////////////////////////////////////////////////

int Draw_Polygon2D16(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch)
{
	// this function draws a POLYGON2D based on 

	int index;           // looping var

	// test if the polygon is visible
	if (poly->state) {
		// loop thru and draw a line from vertices 1 to n
		for (index = 0; index < poly->num_verts - 1; index++) {
			// draw line from ith to ith+1 vertex
			Draw_Clip_Line16(poly->vlist[index].x + poly->x0,
				poly->vlist[index].y + poly->y0,
				poly->vlist[index + 1].x + poly->x0,
				poly->vlist[index + 1].y + poly->y0,
				poly->color,
				vbuffer, lpitch);

		} // end for

	   // now close up polygon
	   // draw line from last vertex to 0th
		Draw_Clip_Line16(poly->vlist[0].x + poly->x0,
			poly->vlist[0].y + poly->y0,
			poly->vlist[index].x + poly->x0,
			poly->vlist[index].y + poly->y0,
			poly->color,
			vbuffer, lpitch);

		// return success
		return(1);
	} // end if
	else
		return(0);

} // end Draw_Polygon2D16

///////////////////////////////////////////////////////////

int Draw_Polygon2D(POLYGON2D_PTR poly, UCHAR *vbuffer, int lpitch)
{
	// this function draws a POLYGON2D based on 

	int index;           // looping var

	// test if the polygon is visible
	if (poly->state) {
		// loop thru and draw a line from vertices 1 to n
		for (index = 0; index < poly->num_verts - 1; index++) {
			// draw line from ith to ith+1 vertex
			Draw_Clip_Line(poly->vlist[index].x + poly->x0,
				poly->vlist[index].y + poly->y0,
				poly->vlist[index + 1].x + poly->x0,
				poly->vlist[index + 1].y + poly->y0,
				poly->color,
				vbuffer, lpitch);

		} // end for

		// now close up polygon
		// draw line from last vertex to 0th
		Draw_Clip_Line(poly->vlist[0].x + poly->x0,
			poly->vlist[0].y + poly->y0,
			poly->vlist[index].x + poly->x0,
			poly->vlist[index].y + poly->y0,
			poly->color,
			vbuffer, lpitch);

		// return success
		return(1);
	} // end if
	else
		return(0);

} // end Draw_Polygon2D

///////////////////////////////////////////////////////////////

// these are the matrix versions, note they are more inefficient for
// single transforms, but their power comes into play when you concatenate
// multiple transformations, not to mention that all transforms are accomplished
// with the same code, just the matrix differs

////////////////////////////////////////////////////////////

inline int Mat_Init_3X2(MATRIX3X2_PTR ma,
	float m00, float m01,
	float m10, float m11,
	float m20, float m21)
{
	// this function fills a 3x2 matrix with the sent data in row major form
	ma->M[0][0] = m00; ma->M[0][1] = m01;
	ma->M[1][0] = m10; ma->M[1][1] = m11;
	ma->M[2][0] = m20; ma->M[2][1] = m21;

	// return success
	return(1);

} // end Mat_Init_3X2

//////////////////////////////////////////////////////////////

#if 0
inline int Mat_Init_3X2(MATRIX3X2_PTR ma,
	float m00, float m01,
	float m10, float m11,
	float m20, float m21)
{
	// this function fills a 3x2 matrix with the sent data in row major form
	ma->M[0][0] = m00; ma->M[0][1] = m01;
	ma->M[1][0] = m10; ma->M[1][1] = m11;
	ma->M[2][0] = m20; ma->M[2][1] = m21;

	// return success
	return(1);

} // end Mat_Init_3X2
#endif

////////////////////////////////////////////////////////////////

int Mat_Mul_1X2_3X2(MATRIX1X2_PTR ma,
	MATRIX3X2_PTR mb,
	MATRIX1X2_PTR mprod)
{
	// this function multiplies a 1x2 matrix against a 
	// 3x2 matrix - ma*mb and stores the result
	// using a dummy element for the 3rd element of the 1x2 
	// to make the matrix multiply valid i.e. 1x3 X 3x2

	int index;           // looping var

	for (int col = 0; col < 2; col++) {
		// compute dot product from row of ma 
		// and column of mb

		float sum = 0; // used to hold result

		for (index = 0; index < 2; index++) {
			// add in next product pair
			sum += (ma->M[index] * mb->M[index][col]);
		} // end for index

		// add in last element * 1 
		sum += mb->M[index][col];

		// insert resulting col element
		mprod->M[col] = sum;

	} // end for col

	return(1);

} // end Mat_Mul_1X2_3X2

////////////////////////////////////////////////////////////

int Mat_Mul_3X3(MATRIX3X3_PTR ma,
	MATRIX3X3_PTR mb,
	MATRIX3X3_PTR mprod)
{
	// this function multiplies two matrices together and 
	// and stores the result

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			// compute dot product from row of ma 
			// and column of mb

			float sum = 0; // used to hold result

			for (int index = 0; index < 3; index++) {
				// add in next product pair
				sum += (ma->M[row][index] * mb->M[index][col]);
			} // end for index

			// insert resulting row,col element
			mprod->M[row][col] = sum;

		} // end for col

	} // end for row

	return(1);

} // end Mat_Mul_3X3

////////////////////////////////////////////////////////////////

int Mat_Mul_1X3_3X3(MATRIX1X3_PTR ma,
	MATRIX3X3_PTR mb,
	MATRIX1X3_PTR mprod)
{
	// this function multiplies a 1x3 matrix against a 
	// 3x3 matrix - ma*mb and stores the result

	for (int col = 0; col < 3; col++) {
		// compute dot product from row of ma 
		// and column of mb

		float sum = 0; // used to hold result

		for (int index = 0; index < 3; index++) {
			// add in next product pair
			sum += (ma->M[index] * mb->M[index][col]);
		} // end for index

		// insert resulting col element
		mprod->M[col] = sum;

	} // end for col

	return(1);

} // end Mat_Mul_1X3_3X3

////////////////////////////////////////////////////////////

int Translate_Polygon2D_Mat(POLYGON2D_PTR poly, int dx, int dy)
{
	// this function translates the center of a polygon by using a matrix multiply
	// on the the center point, this is incredibly inefficient, but for educational purposes
	// if we had an object that wasn't in local coordinates then it would make more sense to
	// use a matrix, but since the origin of the object is at x0,y0 then 2 lines of code can
	// translate, but lets do it the hard way just to see :)

	// test for valid pointer
	if (!poly)
		return(0);

	MATRIX3X2 mt; // used to hold translation transform matrix

	// initialize the matrix with translation values dx dy
	Mat_Init_3X2(&mt, 1, 0, 0, 1, dx, dy);

	// create a 1x2 matrix to do the transform
	MATRIX1X2 p0 = { poly->x0, poly->y0 };
	MATRIX1X2 p1 = { 0,0 }; // this will hold result

	// now translate via a matrix multiply
	Mat_Mul_1X2_3X2(&p0, &mt, &p1);

	// now copy the result back into polygon
	poly->x0 = p1.M[0];
	poly->y0 = p1.M[1];

	// return success
	return(1);

} // end Translate_Polygon2D_Mat

///////////////////////////////////////////////////////////////

int Rotate_Polygon2D_Mat(POLYGON2D_PTR poly, int theta)
{
	// this function rotates the local coordinates of the polygon

	// test for valid pointer
	if (!poly)
		return(0);

	// test for negative rotation angle
	if (theta < 0)
		theta += 360;

	MATRIX3X2 mr; // used to hold rotation transform matrix

	// initialize the matrix with translation values dx dy
	Mat_Init_3X2(&mr, cos_look[theta], sin_look[theta],
		-sin_look[theta], cos_look[theta],
		0, 0);

	// loop and rotate each point, very crude, no lookup!!!
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++) {
		// create a 1x2 matrix to do the transform
		MATRIX1X2 p0 = { poly->vlist[curr_vert].x, poly->vlist[curr_vert].y };
		MATRIX1X2 p1 = { 0,0 }; // this will hold result

		// now rotate via a matrix multiply
		Mat_Mul_1X2_3X2(&p0, &mr, &p1);

		// now copy the result back into vertex
		poly->vlist[curr_vert].x = p1.M[0];
		poly->vlist[curr_vert].y = p1.M[1];

	} // end for curr_vert

	// return success
	return(1);

} // end Rotate_Polygon2D_Mat

////////////////////////////////////////////////////////

int Scale_Polygon2D_Mat(POLYGON2D_PTR poly, float sx, float sy)
{
	// this function scalesthe local coordinates of the polygon

	// test for valid pointer
	if (!poly)
		return(0);


	MATRIX3X2 ms; // used to hold scaling transform matrix

	// initialize the matrix with translation values dx dy
	Mat_Init_3X2(&ms, sx, 0,
		0, sy,
		0, 0);

	// loop and scale each point
	for (int curr_vert = 0; curr_vert < poly->num_verts; curr_vert++) {
		// scale and store result back

		// create a 1x2 matrix to do the transform
		MATRIX1X2 p0 = { poly->vlist[curr_vert].x, poly->vlist[curr_vert].y };
		MATRIX1X2 p1 = { 0,0 }; // this will hold result

		// now scale via a matrix multiply
		Mat_Mul_1X2_3X2(&p0, &ms, &p1);

		// now copy the result back into vertex
		poly->vlist[curr_vert].x = p1.M[0];
		poly->vlist[curr_vert].y = p1.M[1];

	} // end for curr_vert

	// return success
	return(1);

} // end Scale_Polygon2D_Mat

///////////////////////////////////////////////////////////

inline void Draw_QuadFP_2D(int x0, int y0,
	int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	int color,
	UCHAR *dest_buffer, int mempitch)
{
	// this function draws a 2D quadrilateral

	// simply call the triangle function 2x, let it do all the work
	Draw_TriangleFP_2D(x0, y0, x1, y1, x3, y3, color, dest_buffer, mempitch);
	Draw_TriangleFP_2D(x1, y1, x2, y2, x3, y3, color, dest_buffer, mempitch);

} // end Draw_QuadFP_2D

/////////////////////////////////////////////////////////////////

int T3DLIB_Init(int lookup_format = DD_PIXEL_FORMAT565)
{
	if (lookup_format == DD_PIXEL_FORMAT565) {
		RETRO_InitFrameBuffer(SDL_PIXELFORMAT_ARGB8888);
		RGB16Bit = RGB16Bit565;
	} else if (lookup_format == DD_PIXEL_FORMAT555) {
		RETRO_InitFrameBuffer(SDL_PIXELFORMAT_ARGB8888);
		RGB16Bit = RGB16Bit555;
	}
	dd_pixel_format = DD_PIXEL_FORMAT888;

	screen_width = RETRO_WIDTH;
	screen_height = RETRO_HEIGHT;
	screen_bpp = 8;

	framebuffer = RETRO.framebuffer;
	framebuffer_pitch = screen_width;

	min_clip_x = 0;
	max_clip_x = screen_width;
	min_clip_y = 0;
	max_clip_y = screen_height;

	memset(palette, 0, MAX_COLORS_PALETTE * sizeof(PALETTEENTRY));
	Load_Palette_From_File(DEFAULT_PALETTE_FILE, palette);
	Set_Palette(palette);

	// return success
	return(1);
}

/////////////////////////////////////////////////////////////////

int T3DLIB_Init16(int pixel_format = DD_PIXEL_FORMAT565)
{
	if (pixel_format == DD_PIXEL_FORMAT565) {
		RETRO_InitFrameBuffer(SDL_PIXELFORMAT_RGB565);
		RGB16Bit = RGB16Bit565;
	} else if (pixel_format == DD_PIXEL_FORMAT555) {
		RETRO_InitFrameBuffer(SDL_PIXELFORMAT_RGB555);
		RGB16Bit = RGB16Bit555;
	}
	dd_pixel_format = pixel_format;

	screen_width = RETRO_WIDTH;
	screen_height = RETRO_HEIGHT;
	screen_bpp = 16;

	framebuffer = RETRO.framebuffer;
	framebuffer_pitch = screen_width * 2;

	min_clip_x = 0;
	max_clip_x = screen_width;
	min_clip_y = 0;
	max_clip_y = screen_height;

	// return success
	return(1);
}

/////////////////////////////////////////////////////////////////

#endif
