#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/audio.h>
#include <stropts.h>

#include "audioIO.h"
#include "config.h"

#define DEFAULT_GAIN AUDIO_MAX_GAIN
#define BLOCK_SIZE 16*1024

static int sol_fd = -1;
static uint_t samples = 0;
static uint_t precision = 0;

int audioOpen( DSPFILE *dfile, WAVFILE *wfile, char *dev_dsp ){
  audio_info_t ainfo;

  if( (dfile->fd = open( dev_dsp, O_WRONLY )) < 0 ){
    fprintf( stderr, "audioOpen(): open failed: %s\n", dev_dsp );
    return -1;
  }
  sol_fd = dfile->fd;

  /*
   * Set the data bit size:
   */
  AUDIO_INITINFO( &ainfo );
  switch( wfile->wavinfo.DataBits ){
    case 16:
      ainfo.play.encoding = AUDIO_ENCODING_LINEAR;
      ainfo.play.precision = AUDIO_PRECISION_16;
      precision = 2;
      break;
#if 0
    case 8:
      ainfo.play.encoding = AUDIO_ENCODING_ULAW;
      ainfo.play.precision = AUDIO_PRECISION_8;
      precision = 1;
      break;
#endif
    default:
      fprintf( stderr,"audioOpen(): not supported\n" );
      return -1;
  }

  /*
   * Set the mode to be Stereo or Mono:
   */
  if( wfile->wavinfo.Channels == Stereo ){
    ainfo.play.channels = 2;
    precision *= 2;
  }else
    ainfo.play.channels = 1;

  /*
   * Set the sampling rate:
   */
  ainfo.play.sample_rate = wfile->wavinfo.SamplingRate;

  /*
   * Determine the audio device's block size:
   */
  ainfo.play.buffer_size = dfile->dspblksiz = BLOCK_SIZE;
  if( ioctl( dfile->fd, AUDIO_SETINFO, &ainfo ) < 0 ){
    fprintf( stderr, "audioOpen(): set audio info\n" );
    return -1;
  }

  /*
   * get number of samples
   */
  if( ioctl( dfile->fd, AUDIO_GETINFO, &ainfo ) < 0 ){
    fprintf( stderr, "audioOpen(): get audio info\n" );
    return -1;
  }
  samples = ainfo.play.samples;

  /*
   * Allocate a buffer to do the I/O through:
   */
  if( (dfile->dspbuf = (char *) malloc( BLOCK_SIZE )) == NULL ){
    fprintf( stderr, "audioOpen(): For DSP I/O buffer\n" );
    return -1;
  }

  /*
   * Return successfully opened device:
   */
  return 0;
}

int audioClose(){
  int ret = close( sol_fd );
  sol_fd = -1;
  return ret;
}

int audioRest( DSPFILE *dfile ){
  audio_info_t ainfo;

  if( ioctl( dfile->fd, AUDIO_GETINFO, &ainfo ) < 0 ){
    fprintf(stderr, "audioRest(): get audio info\n");
    return -1;
  }
  return( samples - ainfo.play.samples * precision );
}

void audioFlush( DSPFILE *dfile ){
  audio_info_t ainfo;

  if( ioctl( dfile->fd, I_FLUSH, FLUSHRW ) < 0 )
    fprintf( stderr, "audioFlush(): flush\n" );
  if( ioctl( dfile->fd, AUDIO_GETINFO, &ainfo ) < 0 ){
    fprintf( stderr, "audioRest(): get audio info\n" );
    return;
  }
  samples = ainfo.play.samples;
}

int audioWrite( DSPFILE *dfile, int cnt ){
  samples += cnt;
  return write( dfile->fd, dfile->dspbuf, cnt );
}

int audioCheck( DSPINFO *info, char *dev_dsp ){
  audio_info_t ainfo;
  int fd;

  if( (fd = open( dev_dsp, O_WRONLY )) < 0 ){
    fprintf( stderr, "audioCheck(): open failed: %s\n", dev_dsp );
    goto eexit;
  }

  AUDIO_INITINFO( &ainfo );
  ainfo.play.port = AUDIO_LINE_OUT;
  if( ioctl( fd, AUDIO_SETINFO, &ainfo ) < 0 ){
    fprintf( stderr, "audioCheck(): set audio info/port\n" );
    goto eexit;
  }
  switch( info->DataBits ){
    case 16:
      ainfo.play.encoding = AUDIO_ENCODING_LINEAR;
      ainfo.play.precision = AUDIO_PRECISION_16;
      break;
#if 1
    case 8:
      ainfo.play.encoding = AUDIO_ENCODING_ULAW;
      ainfo.play.precision = AUDIO_PRECISION_8;
      break;
#endif
    default:
      fprintf( stderr, "audioCheck(): not supported DataBits\n" );
      goto eexit;
  }
  if( ioctl( fd, AUDIO_SETINFO, &ainfo ) < 0 ){
    fprintf( stderr, "audioCheck(): set audio info/encoding\n" );
    goto eexit;
  }
  ainfo.play.channels = info->Channels == Stereo ? 2 : 1;
  if( ioctl( fd, AUDIO_SETINFO, &ainfo ) < 0 ){
    fprintf( stderr, "audioCheck(): set audio info/channels\n" );
    goto eexit;
  }
  ainfo.play.sample_rate = info->SamplingRate;
  if( ioctl( fd, AUDIO_SETINFO, &ainfo ) < 0 ){
    fprintf( stderr, "audioCheck(): set audio info/sample_rate\n" );
    goto eexit;
  }
  if( ioctl( fd, AUDIO_GETINFO, &ainfo ) < 0 ) {
    fprintf( stderr, "audioCheck(): get audio info\n" );
    goto eexit;
  }
  if( (double) abs( ainfo.play.sample_rate-info->SamplingRate )
      / info->SamplingRate > 0.2 ){
    fprintf( stderr, "CheckDSP(): Too differnt from desierd sampling rate\n" );
    goto eexit;
  }
  close( fd );
  info->Available = TRUE;
  return OK;
 eexit:
  info->Available = FALSE;
  if( fd >= 0 ) close( fd );
  return NG;
}

void audioStop(){
  audio_info_t ainfo;

  AUDIO_INITINFO( &ainfo );
  ainfo.play.pause = 1;
  if( ioctl( sol_fd, AUDIO_SETINFO, &ainfo ) < 0 )
    fprintf( stderr, "audioStop(): set audio info\n" );
}

#include "audioMix_Solaris.c"
