/****************************************************************************
 * Visual Boy Advance GX
 *
 * Tantric September 2008
 *
 * vba.cpp
 *
 * This file controls overall program flow. Most things start and end here!
 ***************************************************************************/

#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogcsys.h>
#include <unistd.h>
#include <wiiuse/wpad.h>

#ifdef HW_RVL
extern "C" {
#include <di/di.h>
}
#endif

#include "gba/Sound.h"

#include "vba.h"
#include "vbasupport.h"
#include "preferences.h"
#include "audio.h"
#include "dvd.h"
#include "networkop.h"
#include "fileop.h"
#include "menu.h"
#include "menudraw.h"
#include "input.h"
#include "video.h"
#include "vbaconfig.h"

extern bool ROMLoaded;
extern int emulating;
int ConfigRequested = 0;
int ShutdownRequested = 0;
int ResetRequested = 0;
char appPath[1024];

/****************************************************************************
 * Shutdown / Reboot / Exit
 ***************************************************************************/

static void ExitCleanup()
{
	LWP_SuspendThread (devicethread);
	UnmountAllFAT();
	CloseShare();

#ifdef HW_RVL
	DI_Close();
#endif
}

#ifdef HW_DOL
	#define PSOSDLOADID 0x7c6000a6
	int *psoid = (int *) 0x80001800;
	void (*PSOReload) () = (void (*)()) 0x80001800;
#endif

void Reboot()
{
	ExitCleanup();
#ifdef HW_RVL
    SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
#else
	#define SOFTRESET_ADR ((volatile u32*)0xCC003024)
	*SOFTRESET_ADR = 0x00000000;
#endif
}

void ExitToLoader()
{
	ExitCleanup();
	// Exit to Loader
	#ifdef HW_RVL
		exit(0);
	#else	// gamecube
		if (psoid[0] == PSOSDLOADID)
			PSOReload();
	#endif
}

#ifdef HW_RVL
void ShutdownCB()
{
	ConfigRequested = 1;
	ShutdownRequested = 1;
}
void ResetCB()
{
	ResetRequested = 1;
}
void ShutdownWii()
{
	ExitCleanup();
	SYS_ResetSystem(SYS_POWEROFF, 0, 0);
}
#endif

#ifdef HW_DOL
/****************************************************************************
 * ipl_set_config
 * lowlevel Qoob Modchip disable
 ***************************************************************************/

static void ipl_set_config(unsigned char c)
{
	volatile unsigned long* exi = (volatile unsigned long*)0xCC006800;
	unsigned long val,addr;
	addr=0xc0000000;
	val = c << 24;
	exi[0] = ((((exi[0]) & 0x405) | 256) | 48);	//select IPL
	//write addr of IPL
	exi[0 * 5 + 4] = addr;
	exi[0 * 5 + 3] = ((4 - 1) << 4) | (1 << 2) | 1;
	while (exi[0 * 5 + 3] & 1);
	//write the ipl we want to send
	exi[0 * 5 + 4] = val;
	exi[0 * 5 + 3] = ((4 - 1) << 4) | (1 << 2) | 1;
	while (exi[0 * 5 + 3] & 1);

	exi[0] &= 0x405;	//deselect IPL
}
#endif

static void CreateAppPath(char origpath[])
{
#ifdef HW_DOL
	snprintf(appPath, 1024, GCSettings.SaveFolder);
#else
	char path[1024];
	strcpy(path, origpath); // make a copy so we don't mess up original

	char * loc;
	int pos = -1;

	loc = strrchr(path,'/');
	if (loc != NULL)
		*loc = 0; // strip file name

	loc = strchr(path,'/'); // looking for / from fat:/
	if (loc != NULL)
		pos = loc - path + 1;

	if(pos >= 0 && pos < 1024)
		sprintf(appPath, &(path[pos]));
#endif
}

/****************************************************************************
* main
*
* Program entry
****************************************************************************/
int main(int argc, char *argv[])
{
	#ifdef HW_DOL
	ipl_set_config(6); // disable Qoob modchip
	#endif

	#ifdef HW_RVL
	DI_Init();	// first
	#endif

	InitDeviceThread();

	// Initialise controllers
	PAD_Init ();

	#ifdef HW_RVL
	WPAD_Init();
	// read wiimote accelerometer and IR data
	WPAD_SetDataFormat(WPAD_CHAN_ALL,WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_ALL,640,480);

	// Wii Power/Reset buttons
	WPAD_SetPowerButtonCallback((WPADShutdownCallback)ShutdownCB);
	SYS_SetPowerCallback(ShutdownCB);
	SYS_SetResetCallback(ResetCB);
	#endif

	int selectedMenu = -1;

	InitialiseVideo();
	ResetVideo_Menu (); // change to menu video mode

	// Initialise freetype font system
	if (FT_Init ())
	{
		printf ("Cannot initialise font subsystem!\n");
		while (1);
	}

	// Initialize libFAT for SD and USB
	MountAllFAT();

	// Initialize DVD subsystem (GameCube only)
	#ifdef HW_DOL
	DVD_Init ();
	#endif

	// Check if DVD drive belongs to a Wii
	SetDVDDriveType();

	InitialiseSound();

	InitialisePalette();

	// Set defaults
	DefaultSettings ();

	// store path app was loaded from
	sprintf(appPath, "vbagx");
	if(argc > 0 && argv[0] != NULL)
		CreateAppPath(argv[0]);

	// Load preferences
	if(!LoadPrefs())
	{
		WaitPrompt("Preferences reset - check settings!");
		selectedMenu = 2; // change to preferences menu
	}

	while(1) // main loop
	{
		#ifdef HW_RVL
		if(ShutdownRequested)
			ShutdownWii();
		#endif

		// go back to checking if devices were inserted/removed
		// since we're entering the menu
		LWP_ResumeThread (devicethread);

		MainMenu(selectedMenu);
		selectedMenu = 3; // return to game menu from now on

		// stop checking if devices were removed/inserted
		// since we're starting emulation again
		LWP_SuspendThread (devicethread);

		ResetVideo_Emu();

		while (emulating) // emulation loop
		{
			emulator.emuMain(emulator.emuCount);

			if(ResetRequested)
			{
				emulator.emuReset(); // reset game
				ResetRequested = 0;
			}

			if(ConfigRequested)
			{
				StopAudio();
				ResetVideo_Menu (); // change to menu video mode

				if (GCSettings.AutoSave == 1)
				{
					SaveBatteryOrState(GCSettings.SaveMethod, FILE_SRAM, SILENT); // save battery
				}
				else if (GCSettings.AutoSave == 2)
				{
					SaveBatteryOrState(GCSettings.SaveMethod, FILE_SNAPSHOT, SILENT); // save state
				}
				else if(GCSettings.AutoSave == 3)
				{
					SaveBatteryOrState(GCSettings.SaveMethod, FILE_SRAM, SILENT); // save battery
					SaveBatteryOrState(GCSettings.SaveMethod, FILE_SNAPSHOT, SILENT); // save state
				}

				// save zoom level
				SavePrefs(SILENT);
				ConfigRequested = 0;
				break;
			}
		}
	}
}
