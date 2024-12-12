/* -*- C++ -*-
 * 
 *  onscripter_main.cpp -- main function of ONScripter
 *
 *  Copyright (c) 2001-2011 Ogapee. All rights reserved.
 *
 *  ogapee@aqua.dti2.ne.jp
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ONScripter.h"
#include "version.h"

#if defined(PSP)
#include <pspkernel.h>
#include <psputility.h>
#include <psppower.h>
#include <ctype.h>

PSP_HEAP_SIZE_KB(-1);

int psp_power_resume_number = 0;
ONScripter *g_ons;

int exit_callback(int arg1, int arg2, void *common)
{
    g_ons->endCommand();
    sceKernelExitGame();
    return 0;
}

int power_callback(int unknown, int pwrflags, void *common)
{
    if (pwrflags & PSP_POWER_CB_RESUMING) psp_power_resume_number++;
    return 0;
}

int CallbackThread(SceSize args, void *argp)
{
    int cbid;
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    cbid = sceKernelCreateCallback("Power Callback", power_callback, NULL);
    scePowerRegisterCallback(0, cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int SetupCallbacks(void)
{
    int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0) sceKernelStartThread(thid, 0, 0);
    return thid;
}
#endif

void optionHelp()
{
    printf( "Usage: onscripter [option ...]\n" );
    printf( "      --cdaudio\t\tuse CD audio if available\n");
    printf( "      --cdnumber no\tspecify CD-ROM drive number\n");
    printf( "  -f, --font file\tspecify a TTF font file\n");
    printf( "      --registry file\tspecify a registry file\n");
    printf( "      --dll file\tspecify a dll file\n");
    printf( "  -r, --root path\tspecify the root path to the archives\n");
    printf( "      --fullscreen\tstart in fullscreen mode\n");
    printf( "      --window\t\tstart in windowed mode\n");
    printf( "      --wide\t\tstart in wide screen (16:9) mode\n");
    printf( "      --force-button-shortcut\tignore useescspc and getenter command\n");
    printf( "      --enable-wheeldown-advance\tadvance the text on mouse wheel down\n");
    printf( "      --disable-rescale\tdo not rescale the images in the archives\n");
    printf( "      --edit\t\tenable online modification of the volume and variables when 'z' is pressed\n");
    printf( "      --key-exe file\tspecify a file (*.EXE) that includes a key table\n");
    printf( "  -h, --help\t\tshow this help and exit\n");
    printf( "  -v, --version\t\tshow the version number and exit\n");
    exit(0);
}

void optionVersion()
{
    printf("Written by Ogapee <ogapee@aqua.dti2.ne.jp>\n\n");
    printf("Copyright (c) 2001-2011 Ogapee.\n");
    printf("This is free software; see the source for copying conditions.\n");
    exit(0);
}
#if defined(QWS) || defined(ANDROID)
int SDL_main( int argc, char **argv )
#elif defined(PSP)
extern "C" int main( int argc, char **argv )
#else
int main( int argc, char **argv )
#endif
{
    printf("ONScripter version %s(%d.%02d)\n", ONS_VERSION, NSC_VERSION/100, NSC_VERSION%100 );

    ONScripter ons;

#if defined(PSP)
    ons.disableRescale();
    ons.enableButtonShortCut();
    g_ons = &ons;
    SetupCallbacks();
#elif defined(WINCE)
    char currentDir[256];
    strcpy(currentDir, argv[0]);
    char* cptr = currentDir;
    int i, len = strlen(currentDir);
    for(i=len-1; i>0; i--){
        if(cptr[i] == '\\' || cptr[i] == '/')
            break;
    }
    cptr[i] = '\0';
    ons.setArchivePath(currentDir);
    ons.disableRescale();
    ons.enableButtonShortCut();
#elif defined(ANDROID) 
    ons.enableButtonShortCut();
#endif

    // ----------------------------------------
    // Parse options
    argv++;
    while( argc > 1 ){
        if ( argv[0][0] == '-' ){
            if ( !strcmp( argv[0]+1, "h" ) || !strcmp( argv[0]+1, "-help" ) ){
                optionHelp();
            }
            else if ( !strcmp( argv[0]+1, "v" ) || !strcmp( argv[0]+1, "-version" ) ){
                optionVersion();
            }
            else if ( !strcmp( argv[0]+1, "-cdaudio" ) ){
                ons.enableCDAudio();
            }
            else if ( !strcmp( argv[0]+1, "-cdnumber" ) ){
                argc--;
                argv++;
                ons.setCDNumber(atoi(argv[0]));
            }
            else if ( !strcmp( argv[0]+1, "f" ) || !strcmp( argv[0]+1, "-font" ) ){
                argc--;
                argv++;
                ons.setFontFile(argv[0]);
            }
            else if ( !strcmp( argv[0]+1, "-registry" ) ){
                argc--;
                argv++;
                ons.setRegistryFile(argv[0]);
            }
            else if ( !strcmp( argv[0]+1, "-dll" ) ){
                argc--;
                argv++;
                ons.setDLLFile(argv[0]);
            }
            else if ( !strcmp( argv[0]+1, "r" ) || !strcmp( argv[0]+1, "-root" ) ){
                argc--;
                argv++;
                ons.setArchivePath(argv[0]);
            }
            else if ( !strcmp( argv[0]+1, "-fullscreen" ) ){
                ons.setFullscreenMode();
            }
            else if ( !strcmp( argv[0]+1, "-window" ) ){
                ons.setWindowMode();
            }
            else if ( !strcmp( argv[0]+1, "-wide" ) ){
                ons.setWideScreenMode();
            }
            else if ( !strcmp( argv[0]+1, "-force-button-shortcut" ) ){
                ons.enableButtonShortCut();
            }
            else if ( !strcmp( argv[0]+1, "-enable-wheeldown-advance" ) ){
                ons.enableWheelDownAdvance();
            }
            else if ( !strcmp( argv[0]+1, "-disable-rescale" ) ){
                ons.disableRescale();
            }
            else if ( !strcmp( argv[0]+1, "-edit" ) ){
                ons.enableEdit();
            }
            else if ( !strcmp( argv[0]+1, "-key-exe" ) ){
                argc--;
                argv++;
                ons.setKeyEXE(argv[0]);
            }
            else{
                printf(" unknown option %s\n", argv[0] );
            }
        }
        else{
            optionHelp();
        }
        argc--;
        argv++;
    }
    
    // ----------------------------------------
    // Run ONScripter

    if (ons.init()) exit(-1);
    ons.executeLabel();
    
    exit(0);
}
