

#ifndef __ALPHA_MOVIE_ENCODER_DLL_HEADER_H__
#define __ALPHA_MOVIE_ENCODER_DLL_HEADER_H__

#include <windows.h>

extern "C" {

#define AME_NORMAL_ALPHA	0
#define AME_ADD_ALPHA		1

#define AME_ALPHA_ZLIB		0
#define AME_ALPHA_JPEG		1

#pragma pack(1)

struct AlphaMovieEnc_EncodeOption
{
	int			is_avi_file;	// AVI file or PNG file ?
	char*		input_file_path;
	char* 		output_file_path;

	int			input_alpha_type;		// 0 : normal, 1 : add alpha
	int			alpha_compress_type;	// 0 : zlib, 1 : jpeg

	int			fps_rate;				// > 0
	int			fps_scale;				// > 0

	int			keep_transparent_block;	// 0 : false, 1 : true
	int			quality;				// 0 - 100

	int			num_of_frame;
	int			currnt_frame;

	int			is_finished;
};

#pragma pack()


enum ALPHA_MOVIE_ENC_ERROR_CODE
{
	AME_SUCCESS = 0,		//!< ¬Œ÷
	AME_DLL_NOT_FOUND,
	AME_NULL_HANDLE,		//!< ƒnƒ“ƒhƒ‹‚ªNULL
	AME_AVI_FILE_IS_NOT_32BIT,
	AME_AVI_FILE_OPEN_ERROR,
	AME_PNG_FILE_NOT_FOUND,
	AME_INPUT_PARAMETER_ERROR,
	AME_ENCODING_ERROR,
	AME_ENCODE_FINISH,
	AME_ENCODE_CONTINUE,
	AME_EOT,
};

int __stdcall AlphaMovieEnc_CheckInputAVIFile( const char* filename, int* width, int* height, int* rate, int* scale, int* frames );
int __stdcall AlphaMovieEnc_CheckInputPNGFile( const char* filename, int* width, int* height, int* frames );
int __stdcall AlphaMovieEnc_StartEncode( void** handle, AlphaMovieEnc_EncodeOption* param );
int __stdcall AlphaMovieEnc_EncodeNext( void* handle, AlphaMovieEnc_EncodeOption* param );
int __stdcall AlphaMovieEnc_Close( void* handle );

};  // end of extern C

#endif // __ALPHA_MOVIE_ENCODER_DLL_HEADER_H__


