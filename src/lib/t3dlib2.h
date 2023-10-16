// T3DLIB2.H - Header file for T3DLIB2.CPP game engine library

#ifndef T3DLIB2
#define T3DLIB2

// DEFINES ////////////////////////////////////////////////

// TYPES //////////////////////////////////////////////////

typedef struct DIMOUSESTATE {
	LONG lX;
	LONG lY;
	LONG lZ;
	BYTE rgbButtons[4];
} DIMOUSESTATE, *LPDIMOUSESTATE;

// GLOBALS ////////////////////////////////////////////////

DIMOUSESTATE mouse_state;  // contains state of mouse

// FUNCTIONS //////////////////////////////////////////////

int DInput_Read_Mouse(void)
{
	// this function reads the mouse state

	RETRO_MouseState mouse = RETRO_GetMouseState2();

	if (mouse.isrelative) {
		mouse_state.lX = mouse.xrel;
		mouse_state.lY = mouse.yrel;
	} else {
		mouse_state.lX = mouse.x;
		mouse_state.lY = mouse.y;
	}
	mouse_state.rgbButtons[0] = (mouse.leftbutton && mouse.leftcount == 1 ? 1 : 0);
	mouse_state.rgbButtons[2] = mouse.rightbutton;

	// return sucess
	return(1);

} // end DInput_Read_Mouse

/////////////////////////////////////////////////////////////////

#endif
