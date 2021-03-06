/****************************************************************************
 * Visual Boy Advance GX
 *
 * Tantric September 2008
 *
 * input.cpp
 *
 * Wii/Gamecube controller management
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ogcsys.h>
#include <unistd.h>
#include <wiiuse/wpad.h>

#include "vba.h"
#include "button_mapping.h"
#include "audio.h"
#include "video.h"
#include "input.h"

#define VBA_BUTTON_A		1
#define VBA_BUTTON_B		2
#define VBA_BUTTON_SELECT	4
#define VBA_BUTTON_START	8
#define VBA_RIGHT			16
#define VBA_LEFT			32
#define VBA_UP				64
#define VBA_DOWN			128
#define VBA_BUTTON_R		256
#define VBA_BUTTON_L		512
#define VBA_SPEED			1024
#define VBA_CAPTURE			2048


unsigned int vbapadmap[10]; // VBA controller buttons
unsigned int gcpadmap[10]; // Gamecube controller Padmap
unsigned int wmpadmap[10]; // Wiimote Padmap
unsigned int ccpadmap[10]; // Classic Controller Padmap
unsigned int ncpadmap[10]; // Nunchuk + wiimote Padmap

void ResetControls()
{
	int i = 0;

	// VBA controller buttons
	// All other pads are mapped to this
	i=0;
	vbapadmap[i++] = VBA_BUTTON_B;
	vbapadmap[i++] = VBA_BUTTON_A;
	vbapadmap[i++] = VBA_BUTTON_SELECT;
	vbapadmap[i++] = VBA_BUTTON_START;
	vbapadmap[i++] = VBA_UP;
	vbapadmap[i++] = VBA_DOWN;
	vbapadmap[i++] = VBA_LEFT;
	vbapadmap[i++] = VBA_RIGHT;
	vbapadmap[i++] = VBA_BUTTON_L;
	vbapadmap[i++] = VBA_BUTTON_R;

	/*** Gamecube controller Padmap ***/
	i=0;
	gcpadmap[i++] = PAD_BUTTON_B;
	gcpadmap[i++] = PAD_BUTTON_A;
	gcpadmap[i++] = PAD_TRIGGER_Z;
	gcpadmap[i++] = PAD_BUTTON_START;
	gcpadmap[i++] = PAD_BUTTON_UP;
	gcpadmap[i++] = PAD_BUTTON_DOWN;
	gcpadmap[i++] = PAD_BUTTON_LEFT;
	gcpadmap[i++] = PAD_BUTTON_RIGHT;
	gcpadmap[i++] = PAD_TRIGGER_L;
	gcpadmap[i++] = PAD_TRIGGER_R;

	/*** Wiimote Padmap ***/
	i=0;
	wmpadmap[i++] = WPAD_BUTTON_1;
	wmpadmap[i++] = WPAD_BUTTON_2;
	wmpadmap[i++] = WPAD_BUTTON_MINUS;
	wmpadmap[i++] = WPAD_BUTTON_PLUS;
	wmpadmap[i++] = WPAD_BUTTON_RIGHT;
	wmpadmap[i++] = WPAD_BUTTON_LEFT;
	wmpadmap[i++] = WPAD_BUTTON_UP;
	wmpadmap[i++] = WPAD_BUTTON_DOWN;
	wmpadmap[i++] = WPAD_BUTTON_B;
	wmpadmap[i++] = WPAD_BUTTON_A;

	/*** Classic Controller Padmap ***/
	i=0;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_Y;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_B;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_MINUS;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_PLUS;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_UP;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_DOWN;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_LEFT;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_RIGHT;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_FULL_L;
	ccpadmap[i++] = WPAD_CLASSIC_BUTTON_FULL_R;

	/*** Nunchuk + wiimote Padmap ***/
	i=0;
	ncpadmap[i++] = WPAD_NUNCHUK_BUTTON_C;
	ncpadmap[i++] = WPAD_NUNCHUK_BUTTON_Z;
	ncpadmap[i++] = WPAD_BUTTON_MINUS;
	ncpadmap[i++] = WPAD_BUTTON_PLUS;
	ncpadmap[i++] = WPAD_BUTTON_UP;
	ncpadmap[i++] = WPAD_BUTTON_DOWN;
	ncpadmap[i++] = WPAD_BUTTON_LEFT;
	ncpadmap[i++] = WPAD_BUTTON_RIGHT;
	ncpadmap[i++] = WPAD_BUTTON_2;
	ncpadmap[i++] = WPAD_BUTTON_1;
}

/****************************************************************************
 * WPAD_Stick
 *
 * Get X/Y value from Wii Joystick (classic, nunchuk) input
 ***************************************************************************/

s8 WPAD_Stick(u8 chan, u8 right, int axis)
{
	float mag = 0.0;
	float ang = 0.0;
	WPADData *data = WPAD_Data(chan);

	switch (data->exp.type)
	{
		case WPAD_EXP_NUNCHUK:
		case WPAD_EXP_GUITARHERO3:
			if (right == 0)
			{
				mag = data->exp.nunchuk.js.mag;
				ang = data->exp.nunchuk.js.ang;
			}
			break;

		case WPAD_EXP_CLASSIC:
			if (right == 0)
			{
				mag = data->exp.classic.ljs.mag;
				ang = data->exp.classic.ljs.ang;
			}
			else
			{
				mag = data->exp.classic.rjs.mag;
				ang = data->exp.classic.rjs.ang;
			}
			break;

		default:
			break;
	}

	/* calculate x/y value (angle need to be converted into radian) */
	if (mag > 1.0) mag = 1.0;
	else if (mag < -1.0) mag = -1.0;
	double val;

	if(axis == 0) // x-axis
		val = mag * sin((PI * ang)/180.0f);
	else // y-axis
		val = mag * cos((PI * ang)/180.0f);

	return (s8)(val * 128.0f);
}

/****************************************************************************
 * DecodeJoy
 *
 * Reads the changes (buttons pressed, etc) from a controller and reports
 * these changes to VBA
 ****************************************************************************/

u32 DecodeJoy(unsigned short pad)
{
	signed char pad_x = PAD_StickX (pad);
	signed char pad_y = PAD_StickY (pad);
	signed char gc_px = PAD_SubStickX (0);
	u32 jp = PAD_ButtonsHeld (pad);
	u32 J = 0;

	#ifdef HW_RVL
	signed char wm_ax = WPAD_Stick ((u8)pad, 0, 0);
	signed char wm_ay = WPAD_Stick ((u8)pad, 0, 1);
	u32 wp = WPAD_ButtonsHeld (pad);
	signed char wm_sx = WPAD_Stick (0,1,0); // CC right joystick

	u32 exp_type;
	if ( WPAD_Probe(pad, &exp_type) != 0 ) exp_type = WPAD_EXP_NONE;
	#endif

	/***
	Gamecube Joystick input
	***/
	// Is XY inside the "zone"?
	if (pad_x * pad_x + pad_y * pad_y > PADCAL * PADCAL)
	{
		if (pad_x > 0 && pad_y == 0) J |= VBA_RIGHT;
		if (pad_x < 0 && pad_y == 0) J |= VBA_LEFT;
		if (pad_x == 0 && pad_y > 0) J |= VBA_UP;
		if (pad_x == 0 && pad_y < 0) J |= VBA_DOWN;

		if (pad_x != 0 && pad_y != 0)
		{
			if ((float)pad_y / pad_x >= -2.41421356237 && (float)pad_y / pad_x < 2.41421356237)
			{
				if (pad_x >= 0)
					J |= VBA_RIGHT;
				else
					J |= VBA_LEFT;
			}

			if ((float)pad_x / pad_y >= -2.41421356237 && (float)pad_x / pad_y < 2.41421356237)
			{
				if (pad_y >= 0)
					J |= VBA_UP;
				else
					J |= VBA_DOWN;
			}
		}
	}
#ifdef HW_RVL
	/***
	Wii Joystick (classic, nunchuk) input
	***/
	// Is XY inside the "zone"?
	if (wm_ax * wm_ax + wm_ay * wm_ay > PADCAL * PADCAL)
	{
		/*** we don't want division by zero ***/
		if (wm_ax > 0 && wm_ay == 0)
			J |= VBA_RIGHT;
		if (wm_ax < 0 && wm_ay == 0)
			J |= VBA_LEFT;
		if (wm_ax == 0 && wm_ay > 0)
			J |= VBA_UP;
		if (wm_ax == 0 && wm_ay < 0)
			J |= VBA_DOWN;

		if (wm_ax != 0 && wm_ay != 0)
		{

			/*** Recalc left / right ***/
			float t;

			t = (float) wm_ay / wm_ax;
			if (t >= -2.41421356237 && t < 2.41421356237)
			{
				if (wm_ax >= 0)
					J |= VBA_RIGHT;
				else
					J |= VBA_LEFT;
			}

			/*** Recalc up / down ***/
			t = (float) wm_ax / wm_ay;
			if (t >= -2.41421356237 && t < 2.41421356237)
			{
				if (wm_ay >= 0)
					J |= VBA_UP;
				else
					J |= VBA_DOWN;
			}
		}
	}
#endif

	// Turbo feature
	if(
	(gc_px > 70)
	#ifdef HW_RVL
	|| (wm_sx > 70)
	|| ((wp & WPAD_BUTTON_A) && (wp & WPAD_BUTTON_B))
	#endif
	)
		J |= VBA_SPEED;

	/*** Report pressed buttons (gamepads) ***/
	int i;

	for (i = 0; i < MAXJP; i++)
	{
		if ( (jp & gcpadmap[i])											// gamecube controller
		#ifdef HW_RVL
		|| ( (exp_type == WPAD_EXP_NONE) && (wp & wmpadmap[i]) )	// wiimote
		|| ( (exp_type == WPAD_EXP_CLASSIC) && (wp & ccpadmap[i]) )	// classic controller
		|| ( (exp_type == WPAD_EXP_NUNCHUK) && (wp & ncpadmap[i]) )	// nunchuk + wiimote
		#endif
		)
			J |= vbapadmap[i];
	}

	if ((J & 48) == 48)
		J &= ~16;
	if ((J & 192) == 192)
		J &= ~128;

	return J;
}
u32 GetJoy(int pad)
{
    pad = 0;

    s8 gc_px = PAD_SubStickX (0);
    s8 gc_py = PAD_SubStickY (0);

    #ifdef HW_RVL
    s8 wm_sx = WPAD_Stick (0,1,0);
    s8 wm_sy = WPAD_Stick (0,1,1);
    u32 wm_pb = WPAD_ButtonsHeld (0); // wiimote / expansion button info
    #endif

    // Check for video zoom
	if (GCSettings.Zoom)
	{
		if (gc_py < -36 || gc_py > 36)
			zoom ((float) gc_py / -36);
		#ifdef HW_RVL
			if (wm_sy < -36 || wm_sy > 36)
				zoom ((float) wm_sy / -36);
		#endif
	}

    // request to go back to menu
    if ((gc_px < -70)
    #ifdef HW_RVL
    		 || (wm_pb & WPAD_BUTTON_HOME)
    		 || (wm_pb & WPAD_CLASSIC_BUTTON_HOME)
    		 || (wm_sx < -70)
    #endif
    )
	{
    	StopAudio();
    	ConfigRequested = 1;
		return 0;
	}
	else
	{
		return DecodeJoy(pad);
	}
}
