/*
* Copyright (c) 2009-2010, Helios (helios.vmg@gmail.com)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of the author may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY HELIOS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
* EVENT SHALL HELIOS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define play_video_SIGNATURE bool play_video( \
		void *player,                         \
		SDL_Surface *screen,                  \
		const char *input,                    \
		volatile int *stop,                   \
		void *user_data,                      \
		audio_f audio_output,                 \
		int print_debug,                      \
		std::string &exception_string,        \
		cb_vector &callback_pairs,            \
		file_protocol fp                      \
	)
typedef std::vector<C_play_video_params::trigger_callback_pair> cb_vector;

play_video_SIGNATURE;

typedef C_play_video_params C_play_video_params_1;
typedef C_play_video_params C_play_video_params_latest;
typedef bool (*versioned_play_video_f)(void *,void *);

bool play_video_1(void *player,void *void_params){
	C_play_video_params_1 *params=(C_play_video_params_1 *)void_params;
	if (params->exception_string)
		*params->exception_string=0;
	std::string temp;
	std::vector<C_play_video_params::trigger_callback_pair> callback_pairs(
		params->pairs,
		params->pairs+params->trigger_callback_pairs_n
	);
	if (play_video(
			player,
			params->screen,
			params->input,
			params->stop,
			params->user_data,
			params->audio_output,
			params->print_debug,
			temp,
			callback_pairs,
			params->protocol
		))
		return 1;
	if (params->exception_string){
		strncpy(params->exception_string,temp.c_str(),params->exception_string_size);
		params->exception_string[params->exception_string_size-1]=0;
	}
	return 0;
}

PLAYBACK_FUNCTION_SIGNATURE{
	ulong version=*(ulong *)parameters;
	versioned_play_video_f functions[]={
		play_video_1
	};
	version--;
	if (version>=sizeof(functions)/sizeof(*functions))
		return 0;
	return functions[version](player,parameters);
}
