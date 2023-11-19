//
// DEMOII3_2.CPP - full screen 256 color demo
//

#define RETRO_WIDTH 640
#define RETRO_HEIGHT 480

#include "lib/retro.h"
#include "lib/retromain.h"
#include "lib/retrofont.h"
#include "lib/t3dlib1.h"

// DEFINES ////////////////////////////////////////////////

// physics demo defines
#define NUM_BALLS       10   // number of pool balls
#define BALL_RADIUS     12   // radius of ball

// extents of table
#define TABLE_MIN_X     100
#define TABLE_MAX_X     500
#define TABLE_MIN_Y     50
#define TABLE_MAX_Y     450

// variable lookup indices
#define INDEX_X               0 
#define INDEX_Y               1  
#define INDEX_XV              2 
#define INDEX_YV              3  
#define INDEX_MASS            4

// MACROS ///////////////////////////////////////////////

#define RAND_RANGE(x,y) ( (x) + (rand()%((y)-(x)+1)))
#define DOT_PRODUCT(ux,uy,vx,vy) ((ux)*(vx) + (uy)*(vy))

// GLOBALS ////////////////////////////////////////////////

char buffer[256];                          // used to print text

BITMAP_IMAGE background_bmp;      // holds the background
BOB          balls[NUM_BALLS];    // the balls

float cof_E = 1.0;  // coefficient of restitution

// FUNCTIONS //////////////////////////////////////////////

void Collision_Response(void)
{
	// this function does all the "real" physics to determine if there has
	// been a collision between any ball and any other ball, if there is a collision
	// the function uses the mass of each ball along with the intial velocities to 
	// compute the resulting velocities

	// from the book we know that in general 
	// va2 = (e+1)*mb*vb1+va1(ma - e*mb)/(ma+mb)
	// vb2 = (e+1)*ma*va1+vb1(ma - e*mb)/(ma+mb)

	// and the objects will have direction vectors co-linear to the normal
	// of the point of collision, but since we are using spheres here as the objects
	// we know that the normal to the point of collision is just the vector from the 
	// center's of each object, thus the resulting velocity vector of each ball will
	// be along this normal vector direction

	// step 1: test each object against each other object and test for a collision
	// there are better ways to do this other than a double nested loop, but since
	// there are a small number of objects this is fine, also we want to somewhat model
	// if two or more balls hit simulataneously

	for (int ball_a = 0; ball_a < NUM_BALLS; ball_a++) {
		for (int ball_b = ball_a + 1; ball_b < NUM_BALLS; ball_b++) {
			if (ball_a == ball_b)
				continue;

			// compute the normal vector from a->b
			float nabx = (balls[ball_b].varsF[INDEX_X] - balls[ball_a].varsF[INDEX_X]);
			float naby = (balls[ball_b].varsF[INDEX_Y] - balls[ball_a].varsF[INDEX_Y]);
			float length = sqrt(nabx * nabx + naby * naby);

			// is there a collision?
			if (length <= 2.0 * (BALL_RADIUS * .75)) {
				// the balls have made contact, compute response

				// compute the response coordinate system axes
				// normalize normal vector
				nabx /= length;
				naby /= length;

				// compute the tangential vector perpendicular to normal, simply rotate vector 90
				float tabx = -naby;
				float taby = nabx;

				// blue is normal
				Draw_Clip_Line(balls[ball_a].varsF[INDEX_X] + 0.5,
					balls[ball_a].varsF[INDEX_Y] + 0.5,
					balls[ball_a].varsF[INDEX_X] + 20 * nabx + 0.5,
					balls[ball_a].varsF[INDEX_Y] + 20 * naby + 0.5,
					252, framebuffer, framebuffer_pitch);

				// yellow is tangential
				Draw_Clip_Line(balls[ball_a].varsF[INDEX_X] + 0.5,
					balls[ball_a].varsF[INDEX_Y] + 0.5,
					balls[ball_a].varsF[INDEX_X] + 20 * tabx + 0.5,
					balls[ball_a].varsF[INDEX_Y] + 20 * taby + 0.5,
					251, framebuffer, framebuffer_pitch);

				// tangential is also normalized since it's just a rotated normal vector

				// step 2: compute all the initial velocities
				// notation ball: (a,b) initial: i, final: f, n: normal direction, t: tangential direction

				float vait = DOT_PRODUCT(balls[ball_a].varsF[INDEX_XV],
					balls[ball_a].varsF[INDEX_YV],
					tabx, taby);

				float vain = DOT_PRODUCT(balls[ball_a].varsF[INDEX_XV],
					balls[ball_a].varsF[INDEX_YV],
					nabx, naby);

				float vbit = DOT_PRODUCT(balls[ball_b].varsF[INDEX_XV],
					balls[ball_b].varsF[INDEX_YV],
					tabx, taby);

				float vbin = DOT_PRODUCT(balls[ball_b].varsF[INDEX_XV],
					balls[ball_b].varsF[INDEX_YV],
					nabx, naby);


				// now we have all the initial velocities in terms of the n and t axes
				// step 3: compute final velocities after collision, from book we have
				// note: all this code can be optimized, but I want you to see what's happening :)

				float ma = balls[ball_a].varsF[INDEX_MASS];
				float mb = balls[ball_b].varsF[INDEX_MASS];

				float vafn = (mb * vbin * (cof_E + 1) + vain * (ma - cof_E * mb)) / (ma + mb);
				float vbfn = (ma * vain * (cof_E + 1) - vbin * (ma - cof_E * mb)) / (ma + mb);

				// now luckily the tangential components are the same before and after, so
				float vaft = vait;
				float vbft = vbit;

				// and that's that baby!
				// the velocity vectors are:
				// object a (vafn, vaft)
				// object b (vbfn, vbft)    

				// the only problem is that we are in the wrong coordinate system! we need to 
				// translate back to the original x,y coordinate system, basically we need to 
				// compute the sum of the x components relative to the n,t axes and the sum of
				// the y components relative to the n,t axis, since n,t may both have x,y
				// components in the original x,y coordinate system

				float xfa = vafn * nabx + vaft * tabx;
				float yfa = vafn * naby + vaft * taby;

				float xfb = vbfn * nabx + vbft * tabx;
				float yfb = vbfn * naby + vbft * taby;

				// store results
				balls[ball_a].varsF[INDEX_XV] = xfa;
				balls[ball_a].varsF[INDEX_YV] = yfa;

				balls[ball_b].varsF[INDEX_XV] = xfb;
				balls[ball_b].varsF[INDEX_YV] = yfb;

				// update position
				balls[ball_a].varsF[INDEX_X] += balls[ball_a].varsF[INDEX_XV];
				balls[ball_a].varsF[INDEX_Y] += balls[ball_a].varsF[INDEX_YV];

				balls[ball_b].varsF[INDEX_X] += balls[ball_b].varsF[INDEX_XV];
				balls[ball_b].varsF[INDEX_Y] += balls[ball_b].varsF[INDEX_YV];

			} // end if

		} // end for ball2

	} // end for ball1

} // end Collision_Response

//////////////////////////////////////////////////////////

void DEMO_Render(double deltatime)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	// start the timing clock
	Start_Clock();

	// draw background
	Draw_Bitmap(&background_bmp, framebuffer, framebuffer_pitch, 0);

	// draw table
	HLine(TABLE_MIN_X, TABLE_MAX_X, TABLE_MIN_Y, 250, framebuffer, framebuffer_pitch);
	HLine(TABLE_MIN_X, TABLE_MAX_X, TABLE_MAX_Y, 250, framebuffer, framebuffer_pitch);
	VLine(TABLE_MIN_Y, TABLE_MAX_Y, TABLE_MIN_X, 250, framebuffer, framebuffer_pitch);
	VLine(TABLE_MIN_Y, TABLE_MAX_Y, TABLE_MAX_X, 250, framebuffer, framebuffer_pitch);

	// check for change of e
	if (RETRO_KeyState(SDL_SCANCODE_RIGHT)) {
		cof_E += .01;
	} else if (RETRO_KeyState(SDL_SCANCODE_LEFT)) {
		cof_E -= .01;
	}

	float total_ke_x = 0, total_ke_y = 0;

	// move all the balls and compute system momentum
	for (int index = 0; index < NUM_BALLS; index++) {
		// move the ball
		balls[index].varsF[INDEX_X] += balls[index].varsF[INDEX_XV];
		balls[index].varsF[INDEX_Y] += balls[index].varsF[INDEX_YV];

		// add x,y contributions to kinetic energy
		total_ke_x += (balls[index].varsF[INDEX_XV] * balls[index].varsF[INDEX_XV] * balls[index].varsF[INDEX_MASS]);
		total_ke_y += (balls[index].varsF[INDEX_YV] * balls[index].varsF[INDEX_YV] * balls[index].varsF[INDEX_MASS]);

	} // end fof

	// test for boundary collision with virtual table edge, no need for collision
	// response here, I know what's going to happen :)
	for (int index = 0; index < NUM_BALLS; index++) {
		if ((balls[index].varsF[INDEX_X] >= TABLE_MAX_X - BALL_RADIUS) ||
			(balls[index].varsF[INDEX_X] <= TABLE_MIN_X + BALL_RADIUS)) {
			// invert velocity
			balls[index].varsF[INDEX_XV] = -balls[index].varsF[INDEX_XV];

			balls[index].varsF[INDEX_X] += balls[index].varsF[INDEX_XV];
			balls[index].varsF[INDEX_Y] += balls[index].varsF[INDEX_YV];
		} // end if

		if ((balls[index].varsF[INDEX_Y] >= TABLE_MAX_Y - BALL_RADIUS) ||
			(balls[index].varsF[INDEX_Y] <= TABLE_MIN_Y + BALL_RADIUS)) {
			// invert velocity
			balls[index].varsF[INDEX_YV] = -balls[index].varsF[INDEX_YV];

			balls[index].varsF[INDEX_X] += balls[index].varsF[INDEX_XV];
			balls[index].varsF[INDEX_Y] += balls[index].varsF[INDEX_YV];
		} // end if
	} // end for index

	// draw the balls
	for (int index = 0; index < NUM_BALLS; index++) {
		balls[index].x = balls[index].varsF[INDEX_X] + 0.5 - BALL_RADIUS;
		balls[index].y = balls[index].varsF[INDEX_Y] + 0.5 - BALL_RADIUS;
		Draw_BOB(&balls[index], framebuffer, framebuffer_pitch);
	} // end for

	// draw the velocity vectors
	for (int index = 0; index < NUM_BALLS; index++) {
		Draw_Clip_Line(balls[index].varsF[INDEX_X] + 0.5,
			balls[index].varsF[INDEX_Y] + 0.5,
			balls[index].varsF[INDEX_X] + 2 * balls[index].varsF[INDEX_XV] + 0.5,
			balls[index].varsF[INDEX_Y] + 2 * balls[index].varsF[INDEX_YV] + 0.5,
			246, framebuffer, framebuffer_pitch);
	} // end for

	// draw the title
	Draw_Text_GDI("ELASTIC Object-Object Collision Response DEMO, Press <ESC> to Exit.", 10, 10, 255, framebuffer, framebuffer_pitch);

	// draw the title
	sprintf(buffer, "Coefficient of Restitution e=%f, use <RIGHT>, <LEFT> arrow to change.", cof_E);
	Draw_Text_GDI(buffer, 10, 30, 255, framebuffer, framebuffer_pitch);

	sprintf(buffer, "Total System Kinetic Energy Sum(1/2MiVi^2)=%f ", 0.5 * sqrt(total_ke_x * total_ke_x + total_ke_y * total_ke_y));
	Draw_Text_GDI(buffer, 10, 465, 255, framebuffer, framebuffer_pitch);

	// run collision response algorithm here
	Collision_Response();

	// sync to 30 fps = 1/30sec = 33 ms
	Wait_Clock(33);
}

void DEMO_Initialize(void)
{
	// Initialize T3DLIB
	T3DLIB_Init(true);

	// seed random number generate
	srand(Start_Clock());

	// load background image
	Load_Bitmap_File(&bitmap8bit, "assets/greengrid.bmp");
	Create_Bitmap(&background_bmp, 0, 0, 640, 480);
	Load_Image_Bitmap(&background_bmp, &bitmap8bit, 0, 0, BITMAP_EXTRACT_MODE_ABS);
	Set_Palette(bitmap8bit.palette);
	Unload_Bitmap_File(&bitmap8bit);

	// load the bitmaps
	Load_Bitmap_File(&bitmap8bit, "assets/balls8.bmp");

	// create master ball
	Create_BOB(&balls[0], 0, 0, 24, 24, 6, BOB_ATTR_MULTI_FRAME | BOB_ATTR_VISIBLE);

	// load the imagery in
	for (int index = 0; index < 6; index++) {
		Load_Frame_BOB(&balls[0], &bitmap8bit, index, index, 0, BITMAP_EXTRACT_MODE_CELL);
	}

	// create all the clones
	for (int index = 1; index < NUM_BALLS; index++) {
		Clone_BOB(&balls[0], &balls[index]);
	}

	// now set the initial conditions of all the balls
	for (int index = 0; index < NUM_BALLS; index++) {
		// set position randomly
		balls[index].varsF[INDEX_X] = RAND_RANGE(TABLE_MIN_X + 20, TABLE_MAX_X - 20);
		balls[index].varsF[INDEX_Y] = RAND_RANGE(TABLE_MIN_Y + 20, TABLE_MAX_Y - 20);

		// set initial velocity
		balls[index].varsF[INDEX_XV] = RAND_RANGE(-100, 100) / 15;
		balls[index].varsF[INDEX_YV] = RAND_RANGE(-100, 100) / 15;

		// set mass of ball in virtual kgs :)
		balls[index].varsF[INDEX_MASS] = 1; // 1 for now

		// set ball color
		balls[index].curr_frame = rand() % 6;
	} // end for index

	// unload bitmap image
	Unload_Bitmap_File(&bitmap8bit);

	// set clipping region
	min_clip_x = TABLE_MIN_X;
	min_clip_y = TABLE_MIN_Y;
	max_clip_x = TABLE_MAX_X;
	max_clip_y = TABLE_MAX_Y;
}

void DEMO_Deinitialize(void)
{
	for (int index = 0; index < NUM_BALLS; index++) {
		Destroy_BOB(&balls[index]);
	}
}

//////////////////////////////////////////////////////////
