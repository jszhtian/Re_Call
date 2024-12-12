/*
 * wavfile.c  WAV file check
 *
 *  Copyright: wavfile.c (c) Erik de Castro Lopo  erikd@zip.com.au
 *
 *  Modified : 1997-1998 Masaki Chikama (Wren) <chikama@kasumi.ipl.mech.nagoya-u.ac.jp>
 *             1998-                           <masaki-c@is.aist-nara.ac.jp>
 *             2000-     Kazunori Ueno(JAGARL) <jagarl@createor.club.ne.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
*/

#include        <stdarg.h>
#include  	<stdio.h>
#include  	<stdlib.h>
#include	<errno.h>
#include	<sys/types.h>
#include	<unistd.h>
#include  	<string.h>
#include        "wavfile.h"
#include        "LittleEndian.h"

#define		BUFFERSIZE   		1024
#define		PCM_WAVE_FORMAT   	1

/* modified by jagarl. */
#ifndef TRUE
#define		TRUE			1
#endif
#define		FALSE			0

typedef  struct
{	u_long     dwSize ;
	u_short    wFormatTag ;
	u_short    wChannels ;
	u_long     dwSamplesPerSec ;
	u_long     dwAvgBytesPerSec ;
	u_short    wBlockAlign ;
	u_short    wBitsPerSample ;
} WAVEFORMAT ;

typedef  struct
{	char    	RiffID [4] ;
	u_long    	RiffSize ;
	char    	WaveID [4] ;
	char    	FmtID  [4] ;
	u_long    	FmtSize ;
	u_short   	wFormatTag ;
	u_short   	nChannels ;
	u_long		nSamplesPerSec ;
	u_long		nAvgBytesPerSec ;
	u_short		nBlockAlign ;
	u_short		wBitsPerSample ;
	char		DataID [4] ;
	u_long		nDataBytes ;
} WAVE_HEADER ;


static void waveFormatCopy( WAVEFORMAT* wav, char *ptr );

/*=================================================================================================*/

static char*  findchunk (char* s1, char* s2, size_t n) ;

/*=================================================================================================*/


static int  WaveHeaderCheck  (char *wave_buf,int* channels, u_long* samplerate, int* samplebits, u_long* samples,u_long* datastart)
{	
	static  WAVEFORMAT  waveformat ;
	char*   ptr ;
	u_long  databytes ;

	if (findchunk (wave_buf, "RIFF", BUFFERSIZE) != wave_buf) {
		fprintf(stderr, "Bad format: Cannot find RIFF file marker");
		return  WR_BADRIFF ;
	}

	if (! findchunk (wave_buf, "WAVE", BUFFERSIZE)) {
		fprintf(stderr, "Bad format: Cannot find WAVE file marker");
		return  WR_BADWAVE ;
	}

	ptr = findchunk (wave_buf, "fmt ", BUFFERSIZE) ;

	if (! ptr) {
		fprintf(stderr, "Bad format: Cannot find 'fmt' file marker");
		return  WR_BADFORMAT ;
	}

	ptr += 4 ;	/* Move past "fmt ".*/
	waveFormatCopy( &waveformat, ptr );
	
	if (waveformat.dwSize != (sizeof (WAVEFORMAT) - sizeof (u_long))) {
		fprintf(stderr, "Bad format: Bad fmt size");
		/* return  WR_BADFORMATSIZE ; */
	}

	if (waveformat.wFormatTag != PCM_WAVE_FORMAT) {
		fprintf(stderr, "Only supports PCM wave format");
		return  WR_NOTPCMFORMAT ;
	}

	ptr = findchunk (wave_buf, "data", BUFFERSIZE) ;

	if (! ptr) {
		fprintf(stderr,"Bad format: unable to find 'data' file marker");
		return  WR_NODATACHUNK ;
	}

	ptr += 4 ;	/* Move past "data".*/
	databytes = LittleEndian_getDW(ptr, 0);
	
	/* Everything is now cool, so fill in output data.*/

	*channels   = waveformat.wChannels;
	*samplerate = waveformat.dwSamplesPerSec ;
	*samplebits = waveformat.wBitsPerSample ;
	*samples    = databytes / waveformat.wBlockAlign ;
	
	*datastart  = (u_long)(ptr) + 4;

	if (waveformat.dwSamplesPerSec != waveformat.dwAvgBytesPerSec / waveformat.wBlockAlign) {
		fprintf(stderr, "Bad file format");
		return  WR_BADFORMATDATA ;
	}

	if (waveformat.dwSamplesPerSec != waveformat.dwAvgBytesPerSec / waveformat.wChannels / ((waveformat.wBitsPerSample == 16) ? 2 : 1)) {
		fprintf(stderr, "Bad file format");
		return  WR_BADFORMATDATA ;
	}

	return  0 ;
} ; /* WaveHeaderCheck*/

/*===========================================================================================*/

static char* findchunk  (char* pstart, char* fourcc, size_t n)
{	char	*pend ;
	int		k, test ;

	pend = pstart + n ;

	while (pstart < pend)
	{ 
		if (*pstart == *fourcc)       /* found match for first char*/
		{	test = TRUE ;
			for (k = 1 ; fourcc [k] != 0 ; k++)
				test = (test ? ( pstart [k] == fourcc [k] ) : FALSE) ;
			if (test)
				return  pstart ;
			} ; /* if*/
		pstart ++ ;
		} ; /* while lpstart*/

	return  NULL ;
} ; /* findchuck*/

static void waveFormatCopy( WAVEFORMAT* wav, char *ptr ) {
	wav->dwSize           = LittleEndian_getDW( ptr,  0 );
	wav->wFormatTag       = LittleEndian_getW(  ptr,  4 );
	wav->wChannels        = LittleEndian_getW(  ptr,  6 );
	wav->dwSamplesPerSec  = LittleEndian_getDW( ptr,  8 );
	wav->dwAvgBytesPerSec = LittleEndian_getDW( ptr, 12 );
	wav->wBlockAlign      = LittleEndian_getW(  ptr, 16 );
	wav->wBitsPerSample   = LittleEndian_getW(  ptr, 18 );
}

WAVFILE * WavGetInfo(char *data) {
	int e;					/* Saved errno value */
	int channels;				/* Channels recorded in this wav file */
	u_long samplerate;			/* Sampling rate */
	int sample_bits;			/* data bit size (8/12/16) */
	u_long samples;				/* The number of samples in this file */
	u_long datastart;			/* The offset to the wav data */
	WAVFILE *wfile = (WAVFILE *)malloc(sizeof (WAVFILE));
	memset(wfile->other_data, 0, sizeof(wfile->other_data));

	if ( wfile == NULL ) {
		fprintf(stderr, "WavGetInfo(): Allocating WAVFILE structure\n");
		return NULL;
	}
	
	if ( (e = WaveHeaderCheck(data,
				  &channels,&samplerate,
				  &sample_bits,&samples,&datastart) != 0 )) {
		fprintf(stderr,"WavGetInfo(): Reading WAV header\n");
		goto errxit;
	}
	
	/*
	 * Copy WAV data over to WAVFILE struct:
	 */
	if ( channels == 2 )
		wfile->wavinfo.Channels = Stereo;
	else	wfile->wavinfo.Channels = Mono;


	wfile->wavinfo.SamplingRate = (UInt32) samplerate;
	wfile->wavinfo.Samples = (UInt32) samples;
	wfile->wavinfo.DataBits = (UInt16) sample_bits;
	wfile->wavinfo.DataStart = (UInt32) datastart;

	wfile->wavinfo.DataBytes_o = wfile->wavinfo.DataBytes = LittleEndian_getDW((char *)(datastart - 4),0);
	wfile->data = (char *) datastart;
	
	/*
	 * Open succeeded:
	 */
	return wfile;				/* Return open descriptor */

	/*
	 * Return error after failed open:
	 */
errxit:	e = errno;				/* Save errno */
	free(wfile);				/* Dispose of WAVFILE struct */
	errno = e;				/* Restore error number */
	return NULL;				/* Return error indication */
}

int WavRemoveInfo(WAVFILE *wfile) {
	if ( wfile == NULL ) {
		fprintf(stderr, "WavRemoveInfo(): WAVFILE pointer is NULL!\n");
		return -1;
	}

	free(wfile);
	return 0;
}
