#include <sys/cdio.h>
#include "mixer.h"
#include "cdrom.h"
#include "portab.h"

static char *mixer_devicename = MIXERDEV;
static const char *cdrom_devicename;
static struct cdrom_volctrl volctrl;

void mixer_set_level( int device, int level ){
  audio_info_t ainfo;
  int fd;

  switch( device ){
    case MIX_CD:
      if( (fd = open( cdrom_devicename, O_WRONLY )) < 0 )
	fprintf( stderr, "mixer_initilize(): cd volume control failed\n" );
      else{
	volctrl.channel0 = volctrl.channel1
	  = volctrl.channel2 = volctrl.channel3 = DEFAULT_GAIN;
	if( ioctl( fd, CDROMVOLCTRL, &volctrl ) < 0 )
	  fprintf( stderr, "mixer_set_level(): cd volume control failed\n" );
      }
      break;
    case MIX_PCM:
      AUDIO_INITINFO( &ainfo );
      ainfo.play.gain = level;
      if( (fd = open( mixer_devicename, O_WRONLY )) < 0 )
	fprintf( stderr, "mixer_set_level(): open audio mixer\n" );
      else{
	if( ioctl( fd, AUDIO_SETINFO, &ainfo ) < 0 )
	  fprintf( stderr, "mixer_set_level(): set audio info\n" );
	close( fd );
      }
      break;
    case MIX_MASTER:
      AUDIO_INITINFO( &ainfo );
      ainfo.monitor_gain = level;
      if( (fd = open( mixer_devicename, O_WRONLY )) < 0 )
	fprintf( stderr, "mixer_set_level(): open audio mixer\n" );
      else{
	if( ioctl( fd, AUDIO_SETINFO, &ainfo ) < 0 )
	  fprintf( stderr, "mixer_set_level(): set audio info\n" );
	close( fd );
      }
  }
}

int mixer_get_level( int device ){
  audio_info_t ainfo;
  int fd;

  switch( device ){
    case MIX_CD:
      /* fixme: can't read volume level. use last value */
      return volctrl.channel0;
    case MIX_PCM:
      if( (fd = open( mixer_devicename, O_WRONLY )) < 0 ){
	fprintf( stderr, "mixer_get_level(): open audio mixer\n" );
	break;
      }
      if( ioctl( fd, AUDIO_GETINFO, &ainfo ) < 0 ){
	fprintf( stderr, "mixer_get_level(): get audio info\n" );
	close( fd );
	break;
      }
      close( fd );
      return ainfo.play.gain;
    case MIX_MASTER:
      if( (fd = open( mixer_devicename, O_WRONLY )) < 0 ){
	fprintf( stderr, "mixer_get_level(): open audio mixer\n" );
	break;
      }
      if( ioctl( fd, AUDIO_GETINFO, &ainfo ) < 0 ){
	fprintf( stderr, "mixer_get_level(): get audio info\n" );
	close( fd );
	break;
      }
      close( fd );
      return ainfo.monitor_gain;
  }
  return -1;
}

int mixer_initilize(){
  audio_info_t ainfo;
  int fd;

  cdrom_devicename = cd_get_devicename();
  if( (fd = open( cdrom_devicename, O_WRONLY )) >= 0 ){
    volctrl.channel0 = volctrl.channel1
      = volctrl.channel2 = volctrl.channel3 = DEFAULT_GAIN;
    if( ioctl( fd, CDROMVOLCTRL, &volctrl ) < 0 )
      fprintf( stderr, "mixer_initilize(): cd volume control failed\n" );
  }else
    fprintf( stderr, "mixer_initilize(): open cd device\n" );

  AUDIO_INITINFO( &ainfo );
  ainfo.play.gain = DEFAULT_GAIN;
  if( (fd = open( mixer_devicename, O_WRONLY )) >= 0 )
    if( ioctl( fd, AUDIO_SETINFO, &ainfo ) >= 0 ){
      close( fd );
      return OK;
    }else
      fprintf( stderr, "mixer_initilize(): set audio info\n" );
  else
    fprintf( stderr, "mixer_initilize(): open audio mixer\n" );
  return NG;
}

void mixer_setDeviceName( char *name ){
  mixer_devicename = name;
}
