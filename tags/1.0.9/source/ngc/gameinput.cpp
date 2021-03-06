/****************************************************************************
 * Visual Boy Advance GX
 *
 * Carl Kenner Febuary 2009
 *
 * gameinput.cpp
 *
 * Wii/Gamecube controls for individual games
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdarg.h>
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
#include "gameinput.h"
#include "vbasupport.h"
#include "wiiusbsupport.h"
#include "gba/GBA.h"
#include "gba/bios.h"
#include "gba/GBAinline.h"

char DebugStr[50] = "";

/*void DebugPrintf(const char *format, ...) {
	va_list args;
    va_start( args, format );
    vsprintf( DebugStr, format, args );
    va_end( args );
}*/


u32 MK1Input(unsigned short pad) {
	u32 J = StandardMovement(pad);
	u8 Health = gbReadMemory(0xc695);
	static u8 OldHealth = 0;

	// Rumble when they lose health!
	if (Health < OldHealth)
		systemGameRumble(5);
	OldHealth = Health;

#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// Punch
	if (wp->btns_h & WPAD_BUTTON_UP || wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_B;
	// Kick
	if (wp->btns_h & WPAD_BUTTON_DOWN || wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_A;
	// Block
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_BUTTON_A | VBA_BUTTON_B; // different from MK2
	// Throw
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_B | VBA_RIGHT; // CAKTODO check which way we are facing
	// Pause
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;
	// Start
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Special move
	if (wp->btns_h & WPAD_BUTTON_B)
	{
		// CAKTODO
	}

#endif
	return J;
}

u32 MK4Input(unsigned short pad)
{
	u32 J = StandardMovement(pad);
	u8 Health = 0;
	static u8 OldHealth = 0;

	// Rumble when they lose health!
	if (Health < OldHealth)
		systemGameRumble(20);
	OldHealth = Health;

#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// Punch
	if (wp->btns_h & WPAD_BUTTON_UP || wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_B;
	// Kick
	if (wp->btns_h & WPAD_BUTTON_DOWN || wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_A;
	// Block
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_BUTTON_START;
	// Throw
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_B | VBA_RIGHT; // CAKTODO check which way we are facing
	// Pause
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;
	// Start
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Special move
	if (wp->btns_h & WPAD_BUTTON_B)
	{
		// CAKTODO
	}
#endif

	return J;
}

u32 MKAInput(unsigned short pad)
{
	u32 J = StandardMovement(pad);
	u8 Health = 0;
	static u8 OldHealth = 0;

	// Rumble when they lose health!
	if (Health < OldHealth)
		systemGameRumble(20);
	OldHealth = Health;

#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// Punch
	if (wp->btns_h & WPAD_BUTTON_UP || wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_B;
	// Kick
	if (wp->btns_h & WPAD_BUTTON_DOWN || wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_A;
	// Block
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_BUTTON_R;
	// Run (supposed to be change styles)
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_C))
		J |= VBA_BUTTON_L;
	// Throw
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_RIGHT; // CAKTODO check which way we are facing
	// Pause
	if (wp->btns_h & WPAD_BUTTON_PLUS || wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;
	// Special move
	if (wp->btns_h & WPAD_BUTTON_B)
	{
		// CAKTODO
	}
#endif

	return J;
}

u32 MKTEInput(unsigned short pad)
{
	static u32 prevJ = 0, prevPrevJ = 0;
	u32 J = StandardMovement(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);
	u8 Health;
	u8 Side;
	if (RomIdCode & 0xFFFFFF == MKDA)
	{
		Health = CPUReadByte(0x3000760); // 731 or 760
		Side = CPUReadByte(0x3000747);
	}
	else
	{
		Health = CPUReadByte(0x3000761); // or 790
		Side = CPUReadByte(0x3000777);
	}
	static u8 OldHealth = 0;

	// Rumble when they lose health!
	if (Health < OldHealth)
		systemGameRumble(20);
	OldHealth = Health;

	u32 Forwards, Back;
	if (Side == 0)
	{
		Forwards = VBA_RIGHT;
		Back = VBA_LEFT;
	}
	else
	{
		Forwards = VBA_LEFT;
		Back = VBA_RIGHT;
	}

	// Punch
	if (wp->btns_h & WPAD_BUTTON_UP || wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_B;
	// Kick
	if (wp->btns_h & WPAD_BUTTON_DOWN || wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_A;
	// Block
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_BUTTON_R;
	// Change styles
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_C))
		J |= VBA_BUTTON_L;
	// Throw
	if (wp->btns_h & WPAD_BUTTON_A)
	{
		if ((prevJ & Forwards && prevJ & VBA_BUTTON_A && prevJ & VBA_BUTTON_B) || ((prevPrevJ & Forwards) && !(prevJ & Forwards)))
			J |= Forwards | VBA_BUTTON_A | VBA_BUTTON_B; // R, R+1+2 = throw

		else if (prevJ & Forwards)
		{
			J &= ~Forwards;
			J &= ~VBA_BUTTON_A;
			J &= ~VBA_BUTTON_B;
		}
		else
			J |= Forwards;

	}
	// Pause
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;
	// Start
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Special move
	if (wp->btns_h & WPAD_BUTTON_B) {
		// CAKTODO
	}
	// Speed
	if (wp->btns_h & WPAD_BUTTON_1 || wp->btns_h & WPAD_BUTTON_2) {
		J |= VBA_SPEED;
	}
#endif

	if ((J & 48) == 48)
		J &= ~16;
	if ((J & 192) == 192)
		J &= ~128;
	prevPrevJ = prevJ;
	prevJ = J;

	return J;
}

u32 LegoStarWars1Input(unsigned short pad) {
	u32 J = StandardMovement(pad) | DecodeGamecube(pad) | DecodeKeyboard(pad);
	// Rumble when they lose health!
	u8 Health = 0;
	static u8 OldHealth = 0;
	if (Health < OldHealth)
		systemGameRumble(20);
	OldHealth = Health;

#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// Start/Select
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;

	if (wp->exp.type == WPAD_EXP_NONE) {
		// CAKTODO
		J |= DecodeWiimote(pad);
	} else if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		// build, use force
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z)
			J |= VBA_BUTTON_L;
		// change characters
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_C)
			J |= VBA_BUTTON_SELECT;
		// Jump
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A;
		// Shoot
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_BUTTON_B;
		// Light saber
		if (fabs(wp->gforce.x)> 1.5 )
			J |= VBA_BUTTON_B;
		// Force power, special ability
		if (wp->btns_h & WPAD_BUTTON_MINUS)
			J |= VBA_BUTTON_R;
		if (wp->btns_h & WPAD_BUTTON_1 || wp->btns_h & WPAD_BUTTON_2)
			J |= VBA_SPEED;
	} else if (wp->exp.type == WPAD_EXP_CLASSIC) {
		J |= DecodeClassic(pad);
	} else J |= DecodeWiimote(pad);
#endif
	return J;
}

u32 LegoStarWars2Input(unsigned short pad) {
	u32 J = StandardMovement(pad) | DecodeGamecube(pad) | DecodeKeyboard(pad);
	// Rumble when they lose health!
	u8 Health = 0;
	static u8 OldHealth = 0;
	if (Health < OldHealth)
		systemGameRumble(20);
	OldHealth = Health;

#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// Start/Select
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;

	if (wp->exp.type == WPAD_EXP_NONE) {
		J |= DecodeWiimote(pad);
	} else if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		// build, force transform, pull lever
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z)
			J |= VBA_BUTTON_R;
		// change characters
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_C || wp->btns_h & WPAD_BUTTON_1 || wp->btns_h & WPAD_BUTTON_2)
			J |= VBA_BUTTON_SELECT;
		// grapple
		if (fabs(wp->gforce.y)> 1.6 )
			J |= VBA_BUTTON_R;
		// Jump
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A;
		// Shoot
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_BUTTON_B;
		// Light saber
		if (fabs(wp->gforce.x)> 1.5 )
			J |= VBA_BUTTON_B;
		// Force power, vehicle special ability
		if (wp->btns_h & WPAD_BUTTON_MINUS)
			J |= VBA_BUTTON_L;
		if (wp->btns_h & WPAD_BUTTON_1 || wp->btns_h & WPAD_BUTTON_2)
			J |= VBA_SPEED;

	} else if (wp->exp.type == WPAD_EXP_CLASSIC) {
		J |= DecodeClassic(pad);
	}
#endif
	return J;
}

u32 TMNTInput(unsigned short pad) {
	u32 J = StandardMovement(pad) | StandardDPad(pad) | DecodeClassic(pad) | DecodeKeyboard(pad) | DecodeGamecube(pad);
	static u32 LastDir = VBA_RIGHT;
	static bool wait = false;
	static int holdcount = 0;
	bool Jump=0, Attack=0, SpinKick=0, Roll=0, Pause=0, Select=0;

#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);
	if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		Jump = (wp->btns_h & WPAD_BUTTON_A);
		Attack = (fabs(wp->gforce.x)> 1.5);
		SpinKick = (fabs(wp->exp.nunchuk.gforce.x)> 0.5);
		Roll = (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z || wp->btns_h & WPAD_NUNCHUK_BUTTON_C);
		Pause = (wp->btns_h & WPAD_BUTTON_PLUS);
		Select = (wp->btns_h & WPAD_BUTTON_MINUS);
		// Swap Turtles or super turtle summon
		if (wp->btns_h & WPAD_BUTTON_B) {
			if (wp->exp.nunchuk.orient.pitch < -35 && wp->orient.pitch < -35)
				J |= VBA_BUTTON_L | VBA_BUTTON_R;
			else J |= VBA_BUTTON_R;
		}
	} else if (wp->exp.type == WPAD_EXP_CLASSIC) {
		Jump = (wp->btns_h & WPAD_CLASSIC_BUTTON_B);
		Attack = (wp->btns_h & WPAD_CLASSIC_BUTTON_A);
		SpinKick = (wp->btns_h & WPAD_CLASSIC_BUTTON_X);
		Pause = (wp->btns_h & WPAD_CLASSIC_BUTTON_PLUS);
		Select = (wp->btns_h & WPAD_CLASSIC_BUTTON_MINUS);
		Roll = (wp->btns_h & (WPAD_CLASSIC_BUTTON_FULL_L | WPAD_CLASSIC_BUTTON_FULL_R | WPAD_CLASSIC_BUTTON_ZL | WPAD_CLASSIC_BUTTON_ZR));
		// Swap Turtles or super turtle summon
		if (wp->btns_h & WPAD_CLASSIC_BUTTON_Y) {
			holdcount++;
			if (holdcount > 20)
				J |= VBA_BUTTON_L | VBA_BUTTON_R;
		}
		if (wp->btns_u & WPAD_CLASSIC_BUTTON_Y) {
			if (holdcount <= 20)
				J |= VBA_BUTTON_R;
			holdcount = 0;
		}
	} else {
		Jump = (wp->btns_h & WPAD_BUTTON_A);
		Attack = (fabs(wp->gforce.x)> 1.5);
		Pause = (wp->btns_h & WPAD_BUTTON_PLUS);
		Select = (wp->btns_h & WPAD_BUTTON_MINUS);
		// Swap Turtles or super turtle summon
		if (wp->btns_h & WPAD_BUTTON_B) {
			if (wp->orient.pitch < -40)
				J |= VBA_BUTTON_L | VBA_BUTTON_R;
			else J |= VBA_BUTTON_R;
		}
		SpinKick = (wp->btns_h & WPAD_BUTTON_1);
		Roll = (wp->btns_h & WPAD_BUTTON_2);
	}

#endif
	u32 gc = PAD_ButtonsHeld(pad);
	u32 released = PAD_ButtonsUp(pad);
	// DPad moves
	if (gc & PAD_BUTTON_UP)
		J |= VBA_UP;
	if (gc & PAD_BUTTON_DOWN)
		J |= VBA_DOWN;
	if (gc & PAD_BUTTON_LEFT)
		J |= VBA_LEFT;
	if (gc & PAD_BUTTON_RIGHT)
		J |= VBA_RIGHT;
	// Jump
	if (gc & PAD_BUTTON_A) J |= VBA_BUTTON_A;
	// Swap turtles, hold for super family move
	if (gc & PAD_BUTTON_B) {
		holdcount++;
		if (holdcount > 20)
			J |= VBA_BUTTON_L | VBA_BUTTON_R;
	}
	if (released & PAD_BUTTON_B) {
		if (holdcount <= 20)
			J |= VBA_BUTTON_R;
		holdcount = 0;
	}
	// Attack
	if (gc & PAD_BUTTON_X) Attack = true;
	// Spin kick
	if (gc & PAD_BUTTON_Y) SpinKick = true;
	// Pause
	if (gc & PAD_BUTTON_START) Pause = true;
	// Select
	if (gc & PAD_TRIGGER_Z) Select = true;
	// Roll
	if (gc & PAD_TRIGGER_L || gc & PAD_TRIGGER_R) Roll = true;

	if (Jump) J |= VBA_BUTTON_A;
	if (Attack) J |= VBA_BUTTON_B;
	if (SpinKick) J |= VBA_BUTTON_B | VBA_BUTTON_A;
	if (Pause) J |= VBA_BUTTON_START;
	if (Select) J |= VBA_BUTTON_SELECT;
	if (Roll) {
		if (!wait) {
			J |= LastDir; // Double tap D-Pad to roll CAKTODO
			wait = true;
		} else wait = false;
	}
	
		
	if (J & VBA_RIGHT) LastDir = VBA_RIGHT;
	else if (J & VBA_LEFT) LastDir = VBA_LEFT;
	return J;
}

u32 HarryPotter1GBCInput(unsigned short pad) {
	u32 J = StandardMovement(pad) | StandardDPad(pad) | DPadWASD(pad) | DPadArrowKeys(pad) 
			| DecodeGamecube(pad);
	//u8 ScreenMode = gbReadMemory(0xFFCF);
	//u8 CursorItem = gbReadMemory(0xFFD5);

#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// Pause Menu
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Map (well, it tells you where you are)
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;

	// talk or interact
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_A;
	// cancel
	if (wp->btns_h & WPAD_BUTTON_B)
		J |= VBA_BUTTON_B;
	// spells
	if (fabs(wp->gforce.x)> 1.5)
		J |= VBA_BUTTON_A;

	if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_BUTTON_R;

	// Run (uses emulator speed button)
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_SPEED;
	// Camera, just tells you what room you are in. CAKTODO make press and release trigger it
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_C))
		J |= VBA_BUTTON_SELECT;
#endif

	// CAKTODO spell gestures

	return J;
}

u32 HarryPotter2GBCInput(unsigned short pad) {
	u32 J = StandardMovement(pad) | StandardDPad(pad) | DPadWASD(pad) | DPadArrowKeys(pad) 
			| DecodeGamecube(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// Pause Menu
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Map (well, it tells you where you are)
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;

	// talk or interact
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_A;
	// cancel
	if (wp->btns_h & WPAD_BUTTON_B)
		J |= VBA_BUTTON_B;
	// spells
	if (fabs(wp->gforce.x)> 1.5)
		J |= VBA_BUTTON_A;

	if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_BUTTON_R;

	// Run (uses emulator speed button)
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_SPEED;
	// Camera, just tells you what room you are in. CAKTODO make press and release trigger it
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_C))
		J |= VBA_BUTTON_SELECT;

	// CAKTODO spell gestures
#endif

	return J;
}

u32 HarryPotter1Input(unsigned short pad) {
	u32 J = StandardMovement(pad) | DPadWASD(pad) | DPadArrowKeys(pad) 
			| DecodeGamecube(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// DPad works in the map
	if (wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_R;
	if (wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_UP)
		J |= VBA_UP;
	if (wp->btns_h & WPAD_BUTTON_DOWN)
		J |= VBA_DOWN;

	// Pause
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Map
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;

	// talk or interact
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_A;
	// spells
	if (wp->btns_h & WPAD_BUTTON_B)
		J |= VBA_BUTTON_B;
	if (fabs(wp->gforce.x)> 1.5)
		J |= VBA_BUTTON_B;

	if (wp->btns_h & WPAD_BUTTON_1 || wp->btns_h & WPAD_BUTTON_2)
		J |= VBA_BUTTON_R;

	// Run (uses emulator speed button)
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_SPEED;
	// Flute
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_C))
		J |= VBA_BUTTON_L;

	// CAKTODO spell gestures
#endif

	return J;
}

u32 HarryPotter2Input(unsigned short pad) {
	u32 J = StandardMovement(pad) | DPadWASD(pad) | DPadArrowKeys(pad) 
			| DecodeGamecube(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// DPad works in the map
	if (wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_R;
	if (wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_UP)
		J |= VBA_UP;
	if (wp->btns_h & WPAD_BUTTON_DOWN)
		J |= VBA_DOWN;

	// Pause
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Map
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;

	// talk or interact or sneak
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_B;
	// spells
	if (wp->btns_h & WPAD_BUTTON_B)
		J |= VBA_BUTTON_A;
	if (fabs(wp->gforce.x)> 1.5)
		J |= VBA_BUTTON_A;

	if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_2)
		J |= VBA_BUTTON_L;

	// Sneak instead of run
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_BUTTON_B;
	// Jump with C button
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_C))
		J |= VBA_BUTTON_R;

	// CAKTODO spell gestures
#endif

	return J;
}

u32 HarryPotter3Input(unsigned short pad) {
	u32 J = StandardMovement(pad) | DPadWASD(pad) | DPadArrowKeys(pad) 
			| DecodeGamecube(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	// DPad works in the map
	if (wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_R;
	if (wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_UP)
		J |= VBA_UP;
	if (wp->btns_h & WPAD_BUTTON_DOWN)
		J |= VBA_DOWN;

	// Pause
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Map doesn't exist. Options instead
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;

	// talk or interact
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_A;
	// spells
	if (wp->btns_h & WPAD_BUTTON_B)
		J |= VBA_BUTTON_B;
	if (fabs(wp->gforce.x)> 1.5)
		J |= VBA_BUTTON_B;

	// Change spells
	if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_2)
		J |= VBA_BUTTON_R;

	// Run (uses emulator speed button)
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_SPEED;

	// CAKTODO spell gestures
	// swing sideways for Flipendo
	// point at ceiling for Lumos
#endif

	return J;
}

u32 HarryPotter4Input(unsigned short pad) {
	u32 J = StandardMovement(pad) | DPadArrowKeys(pad) 
			| DecodeGamecube(pad);
#ifdef HW_RVL
	if (DownUsbKeys[KB_A]) J |= VBA_BUTTON_L;
	if (DownUsbKeys[KB_D]) J |= VBA_BUTTON_R;
	if (DownUsbKeys[KB_ENTER] || DownUsbKeys[KB_X]) J |= VBA_BUTTON_A;
	if (DownUsbKeys[KB_C]) J |= VBA_BUTTON_B;
	if (DownUsbKeys[KB_TAB]) J |= VBA_BUTTON_SELECT;
	if (DownUsbKeys[KB_SPACE]) J |= VBA_BUTTON_START;
	if (DownUsbKeys[KB_LSHIFT] || DownUsbKeys[KB_RSHIFT]) J |= VBA_SPEED;

	WPADData * wp = WPAD_Data(pad);

	// DPad works in the map
	if (wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_R;
	if (wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_UP)
		J |= VBA_UP;
	if (wp->btns_h & WPAD_BUTTON_DOWN)
		J |= VBA_DOWN;

	// Pause
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Map doesn't exist. Select Needed for starting game.
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;

	// talk or interact or jinx
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_A;
	if (fabs(wp->gforce.x)> 1.5)
		J |= VBA_BUTTON_A;
	// Charms
	if (wp->btns_h & WPAD_BUTTON_B)
		J |= VBA_BUTTON_B;

	// L and R
	if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_2)
		J |= VBA_BUTTON_R;

	// Run (uses emulator speed button)
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_SPEED;

	// CAKTODO spell gestures
	// swing sideways for Flipendo
	// point at ceiling for Lumos
#endif

	return J;
}

u32 HarryPotter5Input(unsigned short pad) {
	u32 J = StandardMovement(pad) | DecodeGamecube(pad) | DPadWASD(pad);
#ifdef HW_RVL
	// Keyboard controls based on Harry Potter 5 for PC 
	if (DownUsbKeys[KB_LEFT]) J |= VBA_BUTTON_L;
	if (DownUsbKeys[KB_RIGHT]) J |= VBA_BUTTON_R;
	if (DownUsbKeys[KB_UP] || DownUsbKeys[KB_DOWN]) J |= VBA_BUTTON_B;
	if (DownUsbKeys[KB_ENTER]) J |= VBA_BUTTON_A;
	if (DownUsbKeys[KB_TAB]) J |= VBA_BUTTON_SELECT;
	if (DownUsbKeys[KB_SPACE]) J |= VBA_BUTTON_START;
	if (DownUsbKeys[KB_LSHIFT] || DownUsbKeys[KB_RSHIFT]) J |= VBA_SPEED;

	WPADData * wp = WPAD_Data(pad);

	// Wand cursor, normally controlled by DPad, now controlled by Wiimote!
	int cx = 0;
	int cy = 0;
	static int oldcx = 0;
	static int oldcy = 0;
	u8 WandOut = CPUReadByte(0x200e0dd);
	if (WandOut && CursorValid) {
		cx = (CursorX * 268)/640;
		cy = (CursorY * 187)/480;
		if (cx<0x14) cx=0x14;
		else if (cx>0xf8) cx=0xf8;
		if (cy<0x13) cy=0x13;
		else if (cy>0xa8) cy=0xa8;
		CPUWriteByte(0x200e0fe, cx);
		CPUWriteByte(0x200e102, cy);
	}
	oldcx = cx;
	oldcy = cy;

	// DPad works in the map
	if (wp->btns_h & WPAD_BUTTON_RIGHT)
		J |= VBA_BUTTON_R;
	if (wp->btns_h & WPAD_BUTTON_LEFT)
		J |= VBA_BUTTON_L;
	if (wp->btns_h & WPAD_BUTTON_UP)
		J |= VBA_UP;
	if (wp->btns_h & WPAD_BUTTON_DOWN)
		J |= VBA_DOWN;

	// Pause
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Map (actually objectives)
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;

	// talk or interact
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_A;
	// spells
	if (wp->btns_h & WPAD_BUTTON_B)
		J |= VBA_BUTTON_B;

	// Run (uses emulator speed button)
	if ((wp->exp.type == WPAD_EXP_NUNCHUK) && (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z))
		J |= VBA_SPEED;

	// CAKTODO spell gestures
#endif

	return J;
}

u32 TwistedInput(unsigned short pad) {
	// Change this to true if you want to see the screen tilt.
	TiltScreen = false;
	u32 J = StandardMovement(pad) | DecodeKeyboard(pad) | DecodeGamecube(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);

	if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		TiltSideways = false;
		J |= StandardDPad(pad);
		// Pause
		if (wp->btns_h & WPAD_BUTTON_PLUS)
			J |= VBA_BUTTON_START;
		// Select and L do nothing!
		if (wp->btns_h & WPAD_BUTTON_MINUS)
			J |= VBA_BUTTON_SELECT;
		if (wp->btns_h & WPAD_BUTTON_1)
			J |= VBA_BUTTON_L | VBA_SPEED;
		if (wp->btns_h & WPAD_BUTTON_2)
			J |= VBA_BUTTON_L | VBA_SPEED;

		// A Button
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A;
		// B Button
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_BUTTON_B;
		// Grab an icon and prevent menu from spinning
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z)
			J |= VBA_BUTTON_R;
		// Speed
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_C)
			J |= VBA_SPEED;
	} else if (wp->exp.type == WPAD_EXP_CLASSIC) {
		TiltSideways = false;
		J |= StandardDPad(pad) | StandardClassic(pad);

	} else {
		TiltSideways = true;
		J |= StandardSideways(pad);
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_SPEED;
	}
#endif
	return J;
}

u32 KirbyTntInput(unsigned short pad) {
	TiltScreen = false;
	u32 J = StandardMovement(pad) | DecodeGamecube(pad) | DecodeKeyboard(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);
	if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		TiltSideways = false;
		J |= StandardDPad(pad);
		// Pause
		if (wp->btns_h & WPAD_BUTTON_PLUS)
			J |= VBA_BUTTON_START;
		// Select and L do nothing!
		if (wp->btns_h & WPAD_BUTTON_MINUS)
			J |= VBA_BUTTON_SELECT;
		if (wp->btns_h & WPAD_BUTTON_1)
			J |= VBA_BUTTON_B;
		if (wp->btns_h & WPAD_BUTTON_2)
			J |= VBA_BUTTON_A;

		// A Button
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A;
		// B Button
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_BUTTON_B;
		// Speed
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_C)
			J |= VBA_SPEED;
	} else if (wp->exp.type == WPAD_EXP_CLASSIC) {
		TiltSideways = false;
		J |= StandardDPad(pad) | DecodeClassic(pad);
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A;
	} else {
		TiltSideways = true;
		J |= StandardSideways(pad);
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A;
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_SPEED;
	}
#endif
	return J;
}

u32 MohInfiltratorInput(unsigned short pad) {
	u32 J = StandardMovement(pad) | DecodeGamecube(pad) | DecodeKeyboard(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);
	if (wp->exp.type == WPAD_EXP_NONE)
		J |= DecodeWiimote(pad);
	else if (wp->exp.type == WPAD_EXP_CLASSIC) {
		J |= DecodeClassic(pad);
	} else if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		// Pause, objectives, menu
		if (wp->btns_h & WPAD_BUTTON_PLUS)
			J |= VBA_BUTTON_START;
		// Action button, use
		if (wp->btns_h & WPAD_BUTTON_MINUS)
			J |= VBA_BUTTON_L;
		// Use sights/scope, not needed in this game
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A | VBA_BUTTON_L;
		// Shoot
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_BUTTON_A;
		// Reload
		if (fabs(wp->gforce.y)> 1.6 || wp->btns_h & WPAD_BUTTON_UP || wp->btns_h & WPAD_BUTTON_2)
			J |= VBA_BUTTON_L;
		// Strafe
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_C)
			J |= VBA_BUTTON_R;
		// Change weapon
		if (wp->btns_h & WPAD_BUTTON_LEFT || wp->btns_h & WPAD_BUTTON_RIGHT)
			J |= VBA_BUTTON_B;
		// Speed
		if (wp->btns_h & WPAD_BUTTON_1)
			J |= VBA_SPEED;
	} else
		J |= DecodeWiimote(pad);
#endif
	return J;
}

u32 MohUndergroundInput(unsigned short pad) {
	u32 J = StandardMovement(pad) | DecodeClassic(pad) | DecodeWiimote(pad) | DecodeGamecube(pad) | DecodeKeyboard(pad);
	static bool crouched = false;
#ifdef HW_RVL	
	WPADData * wp = WPAD_Data(pad);

	if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		// Joystick controls L and R, not left and right
		if (J & VBA_LEFT)
		J |= VBA_BUTTON_L;
		if (J & VBA_RIGHT)
		J |= VBA_BUTTON_R;
		J &= ~(VBA_LEFT | VBA_RIGHT);
		// Cursor controls left and right for turning
		CursorVisible = true;
		if (CursorValid) {
			if (CursorX < 320 - 40)
			J |= VBA_LEFT;
			else if (CursorX> 320 + 40)
			J |= VBA_RIGHT;
		}
		// Crouch
		if (wp->btns_h & WPAD_BUTTON_DOWN)
		crouched = !crouched;

		// Pause, objectives, menu
		if (wp->btns_h & WPAD_BUTTON_PLUS)
			J |= VBA_BUTTON_START;
		// Action button, not needed in this game
		if (wp->btns_h & WPAD_BUTTON_MINUS)
			J |= 0;
		// Use sights/scope, not needed in this game
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A;
		// Shoot
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_BUTTON_A;
		// Reload
		if (fabs(wp->gforce.y)> 1.6 || wp->btns_h & WPAD_BUTTON_UP || wp->btns_h & WPAD_BUTTON_2)
			J |= VBA_BUTTON_SELECT;
		// Change weapon
		if (wp->btns_h & WPAD_BUTTON_LEFT || wp->btns_h & WPAD_BUTTON_RIGHT)
			J |= VBA_BUTTON_B;
		// Speed
		if (wp->btns_h & WPAD_BUTTON_1)
			J |= VBA_SPEED;
	} else {
		CursorVisible = false;
	}
#endif

	if (crouched && (!(J & VBA_BUTTON_L)) && (!(J & VBA_BUTTON_R)))
		J |= VBA_BUTTON_L | VBA_BUTTON_R;

	return J;
}

u32 BoktaiInput(unsigned short pad) {
	u32 J = StandardMovement(pad) | StandardDPad(pad)
			| DecodeKeyboard(pad) | DecodeGamecube(pad) | DecodeClassic(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);
	static bool GunRaised = false;
	// Action
	if (wp->btns_h & WPAD_BUTTON_A)
		J |= VBA_BUTTON_A;
	// Hold Gun Del Sol up to the light to recharge
	if ((-wp->orient.pitch)> 45) {
		GunRaised = true;
	} else if ((-wp->orient.pitch) < 40) {
		GunRaised = false;
	}
	if (GunRaised)
		J |= VBA_BUTTON_A;
	// Fire Gun Del Sol
	if (wp->btns_h & WPAD_BUTTON_B)
		J |= VBA_BUTTON_B;
	// Look around or change subscreen
	if (wp->btns_h & WPAD_BUTTON_2)
		J |= VBA_BUTTON_R;
	// Start
	if (wp->btns_h & WPAD_BUTTON_PLUS)
		J |= VBA_BUTTON_START;
	// Select
	if (wp->btns_h & WPAD_BUTTON_MINUS)
		J |= VBA_BUTTON_SELECT;

	if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		// Look around
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_C)
			J |= VBA_BUTTON_R;
		// Change element or change subscreen
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z)
			J |= VBA_BUTTON_L;

		if (wp->btns_h & WPAD_BUTTON_1)
			J |= VBA_SPEED;
	} else {
		// Change element or change subscreen
		if (wp->btns_h & WPAD_BUTTON_1)
			J |= VBA_BUTTON_L;
	}
#endif
	return J;
}

u32 Boktai2Input(unsigned short pad) {
	u32 J = StandardMovement(pad) | StandardDPad(pad)
			| DecodeKeyboard(pad) | DecodeGamecube(pad) | DecodeClassic(pad);
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);
	static bool GunRaised = false;
		// Action
		if (wp->btns_h & WPAD_BUTTON_A)
			J |= VBA_BUTTON_A;
		// Hold gun or hand up to the light to recharge
		if ((-wp->orient.pitch)> 45) {
			GunRaised = true;
		} else if ((-wp->orient.pitch) < 40) {
			GunRaised = false;
		}
		if (GunRaised)
			J |= VBA_BUTTON_A;
		// Fire Gun Del Sol
		if (wp->btns_h & WPAD_BUTTON_B)
			J |= VBA_BUTTON_B;
		// Swing sword or hammer or stab spear
		if (fabs(wp->gforce.x)> 1.8)
			J |= VBA_BUTTON_B;
		// Look around or change subscreen
		if (wp->btns_h & WPAD_BUTTON_2)
			J |= VBA_BUTTON_R;
		// Start
		if (wp->btns_h & WPAD_BUTTON_PLUS)
			J |= VBA_BUTTON_START;
		// Select
		if (wp->btns_h & WPAD_BUTTON_MINUS)
			J |= VBA_BUTTON_SELECT;

	if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		// Look around
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_C)
			J |= VBA_BUTTON_R;
		// Change element or change subscreen
		if (wp->btns_h & WPAD_NUNCHUK_BUTTON_Z)
			J |= VBA_BUTTON_L;

		if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_SPEED;
	} else {
		// Change element or change subscreen
		if (wp->btns_h & WPAD_BUTTON_1)
		J |= VBA_BUTTON_L;
	}
#endif

	return J;
}

u32 OnePieceInput(unsigned short pad) {
	// Only Nunchuk and Gamecube controls available
	// Keyboard, Wiimote and Classic controls depend on user configuration
	u32 J = StandardMovement(pad) 
		    | DecodeKeyboard(pad) | DecodeWiimote(pad) | DecodeClassic(pad);
	static u32 LastDir = VBA_RIGHT;
	bool JumpButton=0, AttackButton=0, ViewButton=0, CharacterButton=0, PauseButton=0,
	DashButton=0, GrabButton=0, SpeedButton=0, AttackUpButton = 0;
#ifdef HW_RVL
	WPADData * wp = WPAD_Data(pad);
	// Nunchuk controls are based on One Piece: Unlimited Adventure for the Wii
	if (wp->exp.type == WPAD_EXP_NUNCHUK) {
		J |= StandardDPad(pad);
		JumpButton = wp->btns_h & WPAD_BUTTON_B;
		AttackButton = wp->btns_h & WPAD_BUTTON_A;
		CharacterButton = wp->btns_h & WPAD_BUTTON_MINUS;
		PauseButton = wp->btns_h & WPAD_BUTTON_PLUS;
		DashButton = wp->btns_h & WPAD_NUNCHUK_BUTTON_C;
		GrabButton = wp->btns_h & WPAD_NUNCHUK_BUTTON_Z;
		ViewButton = wp->btns_h & WPAD_BUTTON_1; // doesn't do anything?
		SpeedButton = wp->btns_h & WPAD_BUTTON_2;
	}
#endif
	// Gamecube controls are based on One Piece Grand Adventure
	{
		u32 gc = PAD_ButtonsHeld(pad);
		signed char gc_px = PAD_SubStickX(pad);
		if (gc_px > 70) J |= VBA_SPEED;
		JumpButton = JumpButton || gc & PAD_BUTTON_Y;
		AttackButton = AttackButton || gc & PAD_BUTTON_A;
		GrabButton = GrabButton || gc & PAD_BUTTON_B;
		AttackUpButton = AttackUpButton || gc & PAD_BUTTON_X;
		DashButton = DashButton || gc & PAD_TRIGGER_L;
		PauseButton = PauseButton || gc & PAD_BUTTON_START;
		CharacterButton = CharacterButton || gc & PAD_TRIGGER_R; // supposed to be block
	}
	
	if (JumpButton) J |= VBA_BUTTON_A;
	if (AttackButton) J |= VBA_BUTTON_B;
	if (AttackUpButton) J |= VBA_UP | VBA_BUTTON_B;
	if (CharacterButton) J |= VBA_BUTTON_L;
	if (DashButton) J |= LastDir;
	if (PauseButton) J |= VBA_BUTTON_START;
	if (GrabButton) J |= VBA_BUTTON_R;
	if (SpeedButton) J |= VBA_SPEED;
	if (ViewButton) J |= VBA_BUTTON_SELECT; // doesn't do anything?

	if (J & VBA_RIGHT) LastDir = VBA_RIGHT;
	else if (J & VBA_LEFT) LastDir = VBA_LEFT;
	
	return J;
}

