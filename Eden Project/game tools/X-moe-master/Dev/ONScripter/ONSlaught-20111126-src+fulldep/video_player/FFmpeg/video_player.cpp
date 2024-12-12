/*
* Copyright (c) 2009, 2010, Helios (helios.vmg@gmail.com)
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

#include "../../video_player.h"
#include "../../src/Thread.h"
#include "../common.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <set>
#include <memory>
#include <climits>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
}
#include <SDL/SDL_gfxPrimitives.h>

#if NONS_SYS_WINDOWS
#include <windows.h>
#elif NONS_SYS_UNIX
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#endif

//#define USE_OVERLAY
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const int rmask=0xFF000000;
const int gmask=0x00FF0000;
const int bmask=0x0000FF00;
const int amask=0x000000FF;
const int rshift=24;
const int gshift=16;
const int bshift=8;
const int ashift=0;
#else
const int rmask=0x000000FF;
const int gmask=0x0000FF00;
const int bmask=0x00FF0000;
const int amask=0xFF000000;
const int rshift=0;
const int gshift=8;
const int bshift=16;
const int ashift=24;
#endif

#define ENABLE_VIRTUAL_CONSOLE 1
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
#include <cassert>

class VirtualConsole{
	HANDLE near_end,
		far_end,
		process;
public:
	bool good;
	VirtualConsole(const std::string &name,ulong color);
	~VirtualConsole();
	void put(const char *str,size_t size=0);
	void put(const std::string &str){
		this->put(str.c_str(),str.size());
	}
};

VirtualConsole::VirtualConsole(const std::string &name,ulong color){
	this->good=0;
	SECURITY_ATTRIBUTES sa;
	sa.nLength=sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle=1;
	sa.lpSecurityDescriptor=0;
	if (!CreatePipe(&this->far_end,&this->near_end,&sa,0)){
		assert(this->near_end==INVALID_HANDLE_VALUE);
		return;
	}
	SetHandleInformation(this->near_end,HANDLE_FLAG_INHERIT,0);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&pi,sizeof(pi));
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(STARTUPINFO);
	si.hStdInput=this->far_end;
	si.dwFlags|=STARTF_USESTDHANDLES;
	TCHAR program[]=TEXT("console.exe");
	TCHAR arguments[100];
#ifndef UNICODE
	sprintf(arguments,"%d",color);
#else
	swprintf(arguments,L"0 %d",color);
#endif
	this->process=INVALID_HANDLE_VALUE;
	if (!CreateProcess(program,arguments,0,0,1,CREATE_NEW_CONSOLE|CREATE_UNICODE_ENVIRONMENT,0,0,&si,&pi))
		return;
	this->process=pi.hProcess;
	CloseHandle(pi.hThread);
	this->good=1;

	this->put(name);
	this->put("\n",1);
}

VirtualConsole::~VirtualConsole(){
	if (this->near_end!=INVALID_HANDLE_VALUE){
		if (this->process!=INVALID_HANDLE_VALUE){
			TerminateProcess(this->process,0);
			CloseHandle(this->process);
		}
		CloseHandle(this->near_end);
		CloseHandle(this->far_end);
	}
}
	
void VirtualConsole::put(const char *str,size_t size){
	if (!this->good)
		return;
	if (!size)
		size=strlen(str);
	DWORD l;
	WriteFile(this->near_end,str,size,&l,0);
}

#define VC_AUDIO_DECODE 0
#define VC_VIDEO_DECODE 1
#define VC_AUDIO_RENDER 2
#define VC_VIDEO_RENDER 3
#endif

template <typename T>
class thread_safe_queue{
	std::queue<T> queue;
	NONS_Mutex mutex;
public:
	ulong max_size;
	thread_safe_queue(){
		this->max_size=ULONG_MAX;
	}
	thread_safe_queue(const thread_safe_queue &b){
		NONS_MutexLocker ml(b.mutex);
		this->queue=b.queue;
	}
	const thread_safe_queue &operator=(const thread_safe_queue &b){
		NONS_MutexLocker ml[]={
			b.mutex,
			this->mutex
		};
		this->queue=b.queue;
	}
	void lock(){
		this->mutex.lock();
	}
	bool try_lock(){
		return this->mutex.try_lock();
	}
	void unlock(){
		this->mutex.unlock();
	}
	void push(const T &e){
		while (1){
			this->mutex.lock();
			ulong size=this->queue.size();
			if (size<this->max_size)
				break;
			this->mutex.unlock();
			SDL_Delay(10);
		}
		this->queue.push(e);
		this->mutex.unlock();
	}
	bool is_empty(){
		NONS_MutexLocker ml(this->mutex);
		return this->queue.empty();
	}
	ulong size(){
		NONS_MutexLocker ml(this->mutex);
		return this->queue.size();
	}
	T &peek(){
		NONS_MutexLocker ml(this->mutex);
		return this->queue.front();
	}
	T pop(){
		NONS_MutexLocker ml(this->mutex);
		T ret=this->queue.front();
		this->queue.pop();
		return ret;
	}
	void pop_without_copy(){
		NONS_MutexLocker ml(this->mutex);
		this->queue.pop();
	}
};

template <typename T,typename T2>
std::basic_string<T> itoa(T2 n,unsigned w=0){
	std::basic_stringstream<T> stream;
	if (w){
		stream.fill('0');
		stream.width(w);
	}
	stream <<n;
	return stream.str();
}

#define AUDIO_QUEUE_MAX_SIZE 5
#define FRAME_QUEUE_MAX_SIZE 5
#define AUDIO_QUEUE_REFILL_WAIT 25
#define FRAME_QUEUE_REFILL_WAIT 25

#ifdef AV_NOPTS_VALUE
#undef AV_NOPTS_VALUE
#endif
static const int64_t AV_NOPTS_VALUE=0x8000000000000000LL;

struct Packet{
	bool free_packet;
	AVPacket packet;
    int64_t dts,
		pts;
	Packet(AVPacket *packet);
	~Packet();
	double compute_time(AVStream *stream,AVFrame *frame);
	double compute_time(AVStream *stream);
private:
	Packet(const Packet &){}
	Packet &operator=(const Packet &){ return *this; }
};

struct audioBuffer{
	int16_t *buffer;
	size_t size,
		start_reading;
	bool quit,
		for_copy;
	ulong time_offset;
	audioBuffer(const int16_t *src,size_t size,bool for_copy,double t){
		this->quit=0;
		this->size=size;
		this->start_reading=0;
		this->buffer=new int16_t[size];
		memcpy(this->buffer,src,size*sizeof(int16_t));
		this->for_copy=for_copy;
		this->time_offset=ulong(t*1000.0);
	}
	audioBuffer(bool q){
		this->quit=q;
		this->buffer=0;
	}
	audioBuffer(const audioBuffer &b){
		this->buffer=b.buffer;
		this->size=b.size;
		this->start_reading=b.start_reading;
		this->quit=b.quit;
		this->for_copy=0;
		this->time_offset=b.time_offset;
	}
	~audioBuffer(){
		if (this->buffer && !this->for_copy)
			delete[] this->buffer;
	}
	//1 if this buffer is done
	bool read_into_buffer(int16_t *dst,size_t &size){
		if (size>this->size-this->start_reading)
			size=this->size-this->start_reading;
		memcpy(dst,this->buffer+this->start_reading,size*sizeof(int16_t));
		this->start_reading+=size;
		if (this->start_reading>=this->size){
			delete[] this->buffer;
			this->buffer=0;
			return 1;
		}
		return 0;
	}
};

struct video_player;

class AudioOutput{
	NONS_Thread thread;
	ulong frequency,
		channels;
	int16_t *buffer;
	size_t buffers_size;
	thread_safe_queue<audioBuffer> *incoming_queue;
	volatile bool stop_thread;
	void running_thread(video_player *);
	static bool fill(int16_t *buffer,size_t size,thread_safe_queue<audioBuffer> *queue,ulong &time);
public:
	bool good,
		expect_buffers;
	audio_f audio_output;
	AudioOutput(ulong channels,ulong frequency);
	~AudioOutput();
	thread_safe_queue<audioBuffer> *startThread(video_player *vp);
	void stopThread(bool join);
	void wait_until_stop(video_player *vp,bool immediate);
};

class CompleteVideoFrame{
	SDL_Overlay *overlay;
	SDL_Surface *old_screen;
	SDL_Rect frameRect;
	CompleteVideoFrame(const CompleteVideoFrame &):mutex(*new NONS_Mutex){}
	const CompleteVideoFrame &operator=(const CompleteVideoFrame &){ return *this; }
	NONS_Mutex &mutex;
public:
	uint64_t repeat;
	double pts;
	CompleteVideoFrame(volatile SDL_Surface *screen,AVStream *videoStream,AVFrame *videoFrame,double pts,NONS_Mutex &mutex);
	~CompleteVideoFrame();
	void blit(volatile SDL_Surface *currentScreen,video_player *);
	const SDL_Rect &frame(){ return this->frameRect; }
};

struct cmp_pCompleteVideoFrame{
	bool operator()(CompleteVideoFrame * const &A,CompleteVideoFrame * const &B){
		return A->pts>B->pts;
	}
};

#include "../C_play_video.cpp"

namespace protocol{
	int read(void *p,uint8_t *dst,int count){
		file_protocol *fp=(file_protocol *)p;
		return fp->read(fp->data,dst,count);
	}
	int64_t seek(void *p,int64_t pos,int whence){
		file_protocol *fp=(file_protocol *)p;
		if (!fp)
			return 0;
		if (whence==AVSEEK_SIZE)
			whence=-1;
		return fp->seek(fp->data,pos,whence);
	}
};

struct auto_stream{
	AVFormatContext *&avfc;
	auto_stream(AVFormatContext *&avfc):avfc(avfc){}
	~auto_stream(){
		av_close_input_stream(this->avfc);
	}
};

struct auto_codec_context{
	AVCodecContext *&cc;
	bool close;
	auto_codec_context(AVCodecContext *&cc,bool close):cc(cc),close(close){}
	~auto_codec_context(){
		if (close)
			avcodec_close(this->cc);
	}
};

struct video_player{
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
	VirtualConsole *vc[3];
#endif
	ulong global_time,
		start_time,
		real_global_time,
		real_start_time;
	bool debug_messages;
	volatile bool stop_playback;
	volatile SDL_Surface *global_screen;
	uint64_t global_pts;
	void *user_data;
	typedef thread_safe_queue<Packet *> packet_queue;

	video_player():global_pts(AV_NOPTS_VALUE){
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
		this->vc[0]=new VirtualConsole("audio_decode",7);
		this->vc[1]=new VirtualConsole("video_decode",7);
		this->vc[2]=new VirtualConsole("audio_render",7);
#endif
	}
	~video_player(){
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
		for (size_t a=0;a<3;a++)
			delete this->vc[a];
#endif
	}
	void video_display_thread(thread_safe_queue<CompleteVideoFrame *> *queue,AudioOutput *aoutput,void *user_data,cb_vector *callback_pairs);
	struct get_buffer_struct{
		int64_t pts;
		video_player *vp;
	};
	static int get_buffer(struct AVCodecContext *c,AVFrame *pic);
	static void release_buffer(struct AVCodecContext *c, AVFrame *pic);
	void decode_audio(AVCodecContext *audioCC,AVStream *audioS,packet_queue *packet_queue,AudioOutput *output);
	void decode_video(AVCodecContext *videoCC,AVStream *videoS,packet_queue *packet_queue,AudioOutput *aoutput,void *user_data,cb_vector *callback_pairs);
	bool play_video(
		SDL_Surface *screen,
		const char *input,
		volatile int *stop,
		void *user_data,
		audio_f audio_output,
		int print_debug,
		std::string &exception_string,
		cb_vector &callback_pairs,
		file_protocol fp);
};

AudioOutput::AudioOutput(ulong channels,ulong frequency){
	this->stop_thread=0;
	this->buffers_size=frequency/10*channels;
	this->buffer=new int16_t[this->buffers_size];
	memset(this->buffer,0,this->buffers_size*sizeof(int16_t));
	this->good=0;
	this->frequency=frequency;
	this->channels=channels;
	this->good=1;
}

AudioOutput::~AudioOutput(){
	this->stop_thread=1;
	this->thread.join();
	delete[] this->buffer;
}

thread_safe_queue<audioBuffer> *AudioOutput::startThread(video_player *vp){
	this->incoming_queue=new thread_safe_queue<audioBuffer>;
	this->incoming_queue->max_size=30;
	this->thread.call(member_bind(&AudioOutput::running_thread,this,vp),1);
	return this->incoming_queue;
}

void AudioOutput::stopThread(bool join){
	this->stop_thread=1;
	if (join)
		this->thread.join();
}

void AudioOutput::running_thread(video_player *vp){
	while (this->incoming_queue->is_empty());
	vp->real_start_time=vp->start_time=SDL_GetTicks();
	while (!this->stop_thread){
		SDL_Delay(10);
		ulong now=SDL_GetTicks();
		vp->global_time=now-vp->start_time;
		vp->real_global_time=now-vp->real_start_time;
		if (!this->expect_buffers)
			continue;
		this->incoming_queue->lock();
		while (this->incoming_queue->size()){
			audioBuffer *buffer=&this->incoming_queue->peek();
			if (this->audio_output.write(buffer->buffer,buffer->size/this->channels,this->channels,this->frequency,vp->user_data)){
				this->incoming_queue->pop_without_copy();
				continue;
			}
			break;
		}
		this->incoming_queue->unlock();
		vp->start_time=now-(ulong)this->audio_output.get_time_offset(vp->user_data);
	}
	delete this->incoming_queue;
	this->incoming_queue=0;
}

bool AudioOutput::fill(int16_t *buffer,size_t size,thread_safe_queue<audioBuffer> *queue,ulong &time){
	bool ret=1,
		time_unset=1;
	queue->lock();
	if (queue->is_empty()){
		memset(buffer,0,size*sizeof(int16_t));
		ret=0;
	}else{
		audioBuffer *buffer2=&queue->peek();
		if (buffer2->quit){
			memset(buffer,0,size*sizeof(int16_t));
		}else{
			size_t write_at=0,
				read_size;
			while (write_at<size){
				read_size=size-write_at;
				if (buffer2->read_into_buffer(buffer+write_at,read_size)){
					if (time_unset){
						time=buffer2->time_offset;
						time_unset=0;
					}
					queue->pop();
					if (queue->is_empty())
						break;
					buffer2=&queue->peek();
				}
				write_at+=read_size;
			}
			if (write_at<size){
				memset(buffer+write_at,0,(size-write_at)*sizeof(int16_t));
				ret=0;
			}
		}
	}
	queue->unlock();
	return ret;
}

void AudioOutput::wait_until_stop(video_player *vp,bool immediate){
	this->audio_output.wait(vp->user_data,immediate);
}

CompleteVideoFrame::CompleteVideoFrame(volatile SDL_Surface *screen,AVStream *videoStream,AVFrame *videoFrame,double pts,NONS_Mutex &mutex)
		:mutex(mutex){
	this->overlay=0;
	ulong width,height;
	float screenRatio=float(screen->w)/float(screen->h),
		videoRatio;
	if (videoStream->sample_aspect_ratio.num)
		videoRatio=
			float(videoStream->codec->width*videoStream->sample_aspect_ratio.num)/
			float(videoStream->codec->height*videoStream->sample_aspect_ratio.den);
	else if (videoStream->codec->sample_aspect_ratio.num)
		videoRatio=
			float(videoStream->codec->width*videoStream->codec->sample_aspect_ratio.num)/
			float(videoStream->codec->height*videoStream->codec->sample_aspect_ratio.den);
	else
		videoRatio=float(videoStream->codec->width)/float(videoStream->codec->height);
	if (screenRatio<videoRatio){ //widescreen
		width=screen->w;
		height=ulong(float(screen->w)/videoRatio);
	}else if (screenRatio>videoRatio){ //"narrowscreen"
		width=ulong(float(screen->h)*videoRatio);
		height=screen->h;
	}else{
		width=screen->w;
		height=screen->h;
	}
	this->mutex.lock();
	this->overlay=SDL_CreateYUVOverlay(width,height,SDL_YV12_OVERLAY,(SDL_Surface *)screen);
	this->mutex.unlock();
	this->frameRect.x=Sint16((screen->w-width)/2);
	this->frameRect.y=Sint16((screen->h-height)/2);
	this->frameRect.w=(Uint16)width;
	this->frameRect.h=(Uint16)height;
	SDL_LockYUVOverlay(this->overlay);
	AVPicture pict;
	pict.data[0]=this->overlay->pixels[0];
	pict.data[1]=this->overlay->pixels[2];
	pict.data[2]=this->overlay->pixels[1];
	pict.linesize[0]=this->overlay->pitches[0];
	pict.linesize[1]=this->overlay->pitches[2];
	pict.linesize[2]=this->overlay->pitches[1];
	SwsContext *sc;
#if 1
	sc=sws_alloc_context();
	av_set_int(sc,"srcw",videoStream->codec->width);
	av_set_int(sc,"srch",videoStream->codec->height);
	av_set_int(sc,"src_format",videoStream->codec->pix_fmt);
	av_set_int(sc,"dstw",this->overlay->w);
	av_set_int(sc,"dsth",this->overlay->h);
	av_set_int(sc,"dst_format",PIX_FMT_YUV420P);
	av_set_int(sc,"sws_flags",SWS_FAST_BILINEAR); 
	sws_init_context(sc,0,0);
#else
	sc=sws_getContext(
		videoStream->codec->width,videoStream->codec->height,videoStream->codec->pix_fmt,
		this->overlay->w,this->overlay->h,PIX_FMT_YUV420P,
		SWS_FAST_BILINEAR,
		0,0,0
	);
#endif
	sws_scale(sc,videoFrame->data,videoFrame->linesize,0,videoStream->codec->height,pict.data,pict.linesize);
	sws_freeContext(sc);
//The UNIX build of FFmpeg already performs color correction.
#if !NONS_SYS_UNIX
	{
		//apply color correction
		const int min=16,
			max=235;
#if 0
		Uint8 *row=this->overlay->pixels[0],
			*end=row+this->overlay->w*this->overlay->h;
		ulong w=this->overlay->w,
			h=this->overlay->h;
		for (ulong y=0;y<h;y++){
			Uint8 *pixel=row;
			for (ulong x=w-y*w/h;x<w;x++){
				int a=pixel[x];
				if (a<min)
					a=0;
				else if (a>max)
					a=255;
				else
					a=(a-min)*255/(max-min);
				pixel[x]=(Uint8)a;
			}
			row+=w;
		}
#else
		Uint8 *pixels=this->overlay->pixels[0],
			*end=pixels+this->overlay->w*this->overlay->h;
		for (;pixels!=end;pixels++){
			int a=*pixels;
			if (a<min)
				a=0;
			else if (a>max)
				a=255;
			else
				a=(a-min)*255/(max-min);
			*pixels=(Uint8)a;
		}
#endif
	}
#endif
	SDL_UnlockYUVOverlay(this->overlay);
	this->old_screen=(SDL_Surface *)screen;
	this->pts=pts;
	this->repeat=videoFrame->repeat_pict;
}

CompleteVideoFrame::~CompleteVideoFrame(){
	if (!!this->overlay){
		this->mutex.lock();
		SDL_FreeYUVOverlay(this->overlay);
		this->mutex.unlock();
	}
}

void CompleteVideoFrame::blit(volatile SDL_Surface *currentScreen,video_player *vp){
	if (this->old_screen==currentScreen){
		if (vp->debug_messages){
			std::string s=seconds_to_time_format(this->pts);
			//s.push_back('\n');
			//s.append(seconds_to_time_format(double(real_global_time)/1000.0));
			//s.append(" "+itoa<char>(real_global_time-this->pts*1000.0,4));
			blit_font(this->overlay,s.c_str(),0,0);
		}
		this->mutex.lock();
		SDL_DisplayYUVOverlay(this->overlay,&this->frameRect);
		this->mutex.unlock();
	}
}

Packet::Packet(AVPacket *packet){
	this->free_packet=!!packet;
	if (this->free_packet){
		this->packet=*packet;
		this->dts=this->packet.dts;
		this->pts=this->packet.pts;
	}else
		memset(&this->packet,0,sizeof(this->packet));
}

Packet::~Packet(){
	if (this->free_packet)
		av_free_packet(&this->packet);

}

double Packet::compute_time(AVStream *stream,AVFrame *frame){
	double ret;
	if (this->dts!=AV_NOPTS_VALUE)
		ret=(double)this->dts;
	else{
		ret=0;
		if (frame->opaque){
			int64_t pts=((video_player::get_buffer_struct *)frame->opaque)->pts;
			if (pts!=AV_NOPTS_VALUE)
				ret=(double)pts;
		}
	}
	return ret*av_q2d(stream->time_base);
}

double Packet::compute_time(AVStream *stream){
	double ret;
	if ((this->dts!=AV_NOPTS_VALUE))
		ret=(double)this->dts;
	else
		ret=0;
	return ret*av_q2d(stream->time_base);
}

void video_player::video_display_thread(thread_safe_queue<CompleteVideoFrame *> *queue,AudioOutput *aoutput,void *user_data,cb_vector *callback_pairs){
	while (!stop_playback){
		SDL_Delay(10);
		double current_time=double(this->global_time)/1000.0;
		if (queue->is_empty())
			continue;
		for (ulong a=0;a<callback_pairs->size();a++){
			if (*(*callback_pairs)[a].trigger){
				*(*callback_pairs)[a].trigger=0;
				global_screen=(*callback_pairs)[a].callback(global_screen,user_data);
			}
		}

		CompleteVideoFrame *frame=(CompleteVideoFrame *)queue->peek();

		/*
		if (debug_messages){
			std::stringstream stream;
			stream <<frame->pts<<'\t'<<current_time<<" ("<<SDL_GetTicks()<<'-'<<start_time<<'='<<global_time<<')'<<std::endl;
			threadsafe_print(stream.str());
		}
		*/

		while (frame->pts<=current_time){
			frame->blit(global_screen,this);
			delete queue->pop();
			if (queue->is_empty())
				break;
			frame=(CompleteVideoFrame *)queue->peek();
		}
	}
	while (!queue->is_empty()){
		CompleteVideoFrame *frame=queue->pop();
		if (frame)
			delete frame;
	}
}

int video_player::get_buffer(struct AVCodecContext *c,AVFrame *pic){
	int ret=avcodec_default_get_buffer(c,pic);
	get_buffer_struct *gbs=(get_buffer_struct *)c->opaque;
	gbs->pts=gbs->vp->global_pts;
	return ret;
}

void video_player::release_buffer(struct AVCodecContext *c, AVFrame *pic) {
	if (pic)
		delete (get_buffer_struct *)pic->opaque;
	avcodec_default_release_buffer(c, pic);
}

void video_player::decode_audio(AVCodecContext *audioCC,AVStream *audioS,packet_queue *packet_queue,AudioOutput *output){
	const size_t output_s=AVCODEC_MAX_AUDIO_FRAME_SIZE*2;
	std::vector<int16_t> audioOutputBuffer_vector(output_s);
	int16_t *audioOutputBuffer=&audioOutputBuffer_vector[0];

	thread_safe_queue<audioBuffer> *queue=output->startThread(this);

	if (!audioCC)
		return;

	{
		get_buffer_struct *gbs=new get_buffer_struct;
		gbs->vp=this;
		audioCC->opaque=gbs;
	}
	audioCC->get_buffer=get_buffer;
	audioCC->release_buffer=release_buffer;
	while (1){
		Packet *packet=0;
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
		vc[VC_AUDIO_DECODE]->put("["+itoa<char>(real_global_time,8)+"] decode_audio(): Waiting for packet.\n");
#endif
		while (packet_queue->is_empty() && !stop_playback)
			SDL_Delay(10);
		if (stop_playback)
			break;
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
		vc[VC_AUDIO_DECODE]->put("["+itoa<char>(real_global_time,8)+"] decode_audio(): Popping queue.\n");
#endif
		packet=packet_queue->pop();

		uint8_t *packet_data=packet->packet.data;
		size_t packet_data_s=packet->packet.size,
			write_at=0,
			buffer_size=0;
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
		vc[VC_AUDIO_DECODE]->put("["+itoa<char>(real_global_time,8)+"] decode_audio(): Decoding packet.\n");
#endif
		while (packet_data_s){
			int bytes_decoded=output_s,
				bytes_extracted=-1;
			bytes_extracted=avcodec_decode_audio3(audioCC,audioOutputBuffer+write_at,&bytes_decoded,&packet->packet);
			if (bytes_extracted<0)
				break;
			packet_data+=bytes_extracted;
			if (packet_data_s>=(size_t)bytes_extracted)
				packet_data_s-=bytes_extracted;
			else
				packet_data_s=0;
			buffer_size+=bytes_decoded/sizeof(int16_t);
		}
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
		vc[VC_AUDIO_DECODE]->put("["+itoa<char>(real_global_time,8)+"] decode_audio(): Pushing frame.\n");
#endif
		queue->push(audioBuffer(audioOutputBuffer,buffer_size,1,packet->compute_time(audioS)));
		delete packet;
	}
	output->stopThread(0);

	while (!packet_queue->is_empty()){
		Packet *packet=packet_queue->pop();
		if (packet)
			delete packet;
	}
}

void video_player::decode_video(AVCodecContext *videoCC,AVStream *videoS,packet_queue *packet_queue,AudioOutput *aoutput,void *user_data,cb_vector *callback_pairs){
	AVFrame *videoFrame=avcodec_alloc_frame();
	std::set<CompleteVideoFrame *,cmp_pCompleteVideoFrame> preQueue;
	thread_safe_queue<CompleteVideoFrame *> frameQueue;
	frameQueue.max_size=FRAME_QUEUE_MAX_SIZE;
	NONS_Mutex overlay_mutex;
	NONS_Thread display(member_bind(&video_player::video_display_thread,this,&frameQueue,aoutput,user_data,callback_pairs));

	{
		get_buffer_struct *gbs=new get_buffer_struct;
		gbs->vp=this;
		videoCC->opaque=gbs;
	}
	videoCC->get_buffer=get_buffer;
	videoCC->release_buffer=release_buffer;
	double max_pts=-9999;
	bool msg=0;
	while (1){
		Packet *packet;
		if (stop_playback)
			break;
		if (packet_queue->is_empty()){
			if (!msg){
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
				vc[VC_VIDEO_DECODE]->put("["+itoa<char>(real_global_time,8)+"] decode_video(): Queue is empty. Nothing to do.\n");
#endif
				msg=1;
			}
			SDL_Delay(10);
			continue;
		}
		msg=0;
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
		vc[VC_VIDEO_DECODE]->put("["+itoa<char>(real_global_time,8)+"] decode_video(): Popping queue.\n");
#endif
		packet=packet_queue->pop();
		int frameFinished;
		global_pts=packet->pts;
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
		vc[VC_VIDEO_DECODE]->put("["+itoa<char>(real_global_time,8)+"] decode_video(): Decoding packet.\n");
#endif
		//avcodec_decode_video(videoCC,videoFrame,&frameFinished,packet->data,packet->size);
		avcodec_decode_video2(videoCC,videoFrame,&frameFinished,&packet->packet);
		double pts=packet->compute_time(videoS,videoFrame);
		if (frameFinished){
			CompleteVideoFrame *new_frame=new CompleteVideoFrame(global_screen,videoS,videoFrame,pts,overlay_mutex);
			if (new_frame->pts>max_pts){
				max_pts=new_frame->pts;
				while (!preQueue.empty()){
					std::set<CompleteVideoFrame *,cmp_pCompleteVideoFrame>::iterator first=preQueue.begin();
#if NONS_SYS_WINDOWS && ENABLE_VIRTUAL_CONSOLE
					vc[VC_AUDIO_DECODE]->put("["+itoa<char>(real_global_time,8)+"] decode_audio(): Pushing frame.\n");
#endif
					frameQueue.push(*first);
					preQueue.erase(first);
				}
			}
			preQueue.insert(new_frame);
		}
		delete packet;
	}
	av_free(videoFrame);
	display.join();
	while (!frameQueue.is_empty()){
		CompleteVideoFrame *frame=frameQueue.pop();
		if (frame)
			delete frame;
	}
	while (!preQueue.empty()){
		std::set<CompleteVideoFrame *,cmp_pCompleteVideoFrame>::iterator first=preQueue.begin();
		delete *first;
		preQueue.erase(first);
	}
	while (!packet_queue->is_empty()){
		Packet *packet=packet_queue->pop();
		if (packet)
			delete packet;
	}
}

bool video_player::play_video(
		SDL_Surface *screen,
		const char *input,
		volatile int *stop,
		void *user_data,
		audio_f audio_output,
		int print_debug,
		std::string &exception_string,
		cb_vector &callback_pairs,
		file_protocol fp){
	stop_playback=0;
	debug_messages=!!print_debug;
	global_screen=screen;
	this->global_time=0;
	this->user_data=user_data;
	av_register_all();
	AVFormatContext *avfc;
	SDL_FillRect(screen,0,0);
	SDL_UpdateRect(screen,0,0,0,0);

	std::vector<uchar> io_buffer((1<<12)+FF_INPUT_BUFFER_PADDING_SIZE);
	AVIOContext *avioc=avio_alloc_context(&io_buffer[0],io_buffer.size(),0,&fp,protocol::read,0,protocol::seek);

	AVInputFormat *aif;
	{
		AVProbeData pd;
		pd.filename=input;
		std::vector<uchar> temp((1<<12)+AVPROBE_PADDING_SIZE);
		pd.buf=&temp[0];
		pd.buf_size=fp.read(fp.data,pd.buf,1<<12);
		aif=av_probe_input_format(&pd,1);
	}

	fp.seek(fp.data,0,0);
	if (!aif || av_open_input_stream(&avfc,avioc,input,aif,0)!=0){
		exception_string="Unrecognized file format.";
		return 0;
	}
	auto_stream as(avfc);
	if (av_find_stream_info(avfc)<0){
		exception_string="Stream info not found.";
		return 0;
	}
	if (debug_messages)
		av_dump_format(avfc,0,input,0);

	AVCodecContext *videoCC,
		*audioCC=0;
	long videoStream=-1,
		audioStream=-1;
	for (ulong a=0;a<avfc->nb_streams && (videoStream<0 || audioStream<0);a++){
		if (avfc->streams[a]->codec->codec_type==CODEC_TYPE_VIDEO)
			videoStream=a;
		else if (avfc->streams[a]->codec->codec_type==CODEC_TYPE_AUDIO)
			audioStream=a;
	}
	bool useAudio=(audioStream!=-1);
	if (videoStream==-1){
		if (videoStream==-1)
			exception_string="No video stream. ";
		if (audioStream==-1)
			exception_string.append("No audio stream.");
		return 0;
	}
	videoCC=avfc->streams[videoStream]->codec;
	if (useAudio)
		audioCC=avfc->streams[audioStream]->codec;

	AVCodec *videoCodec=avcodec_find_decoder(videoCC->codec_id),
		*audioCodec=0;
	if (useAudio)
		audioCodec=avcodec_find_decoder(audioCC->codec_id);
	if (!videoCodec || useAudio && !audioCodec){
		if (!videoCodec)
			exception_string="Unsupported video codec. ";
		if (!audioCodec)
			exception_string.append("Unsupported audio codec.");
		return 0;
	}
	int video_codec=avcodec_open(videoCC,videoCodec),
		audio_codec=0;
	if (useAudio)
		audio_codec=avcodec_open(audioCC,audioCodec);
	auto_codec_context acc_video(videoCC,video_codec>=0);
	auto_codec_context acc_audio(audioCC,useAudio && audio_codec>=0);
	if (video_codec<0 || useAudio && audio_codec<0){
		if (video_codec<0)
			exception_string="Open video codec failed. ";
		if (useAudio && audio_codec<0)
			exception_string.append("Open audio codec failed.");
		return 0;
	}
	ulong channels,sample_rate;
	if (useAudio){
		channels=audioCC->channels;
		sample_rate=audioCC->sample_rate;
	}else{
		channels=1;
		sample_rate=11025;
	}
	{
		AudioOutput output(channels,sample_rate);
		if (!output.good){
			exception_string="Open audio output failed.";
			return 0;
		}
		output.audio_output=audio_output;
		output.expect_buffers=useAudio;

		AVPacket packet;
		packet_queue audio_packets,
			video_packets;
		video_packets.max_size=25;
		audio_packets.max_size=200;
		NONS_Thread audio_decoder(member_bind(&video_player::decode_audio,this,audioCC,avfc->streams[audioStream],&audio_packets,&output),1);
		NONS_Thread video_decoder(member_bind(&video_player::decode_video,this,videoCC,avfc->streams[videoStream],&video_packets,&output,user_data,&callback_pairs));
		for (ulong a=0;av_read_frame(avfc,&packet)>=0;a++){
			if (*stop){
				av_free_packet(&packet);
				break;
			}
			if (packet.stream_index==videoStream)
				video_packets.push(new Packet(&packet));
			else if (packet.stream_index==audioStream)
				audio_packets.push(new Packet(&packet));
			else
				av_free_packet(&packet);
		}
		stop_playback=1;
		audio_decoder.join();
		video_decoder.join();
		if (!useAudio)
			output.stopThread(0);
		output.wait_until_stop(this,!!*stop);
	}
	return 1;
}

VIDEO_CONSTRUCTOR_SIGNATURE{
	return new video_player;
}

VIDEO_DESTRUCTOR_SIGNATURE{
	delete (video_player *)player;
}

play_video_SIGNATURE{
	return ((video_player *)player)->play_video(
		screen,
		input,
		stop,
		user_data,
		audio_output,
		print_debug,
		exception_string,
		callback_pairs,
		fp
	);
}

PLAYER_TYPE_FUNCTION_SIGNATURE{
	return "FFmpeg";
}

PLAYER_VERSION_FUNCTION_SIGNATURE{
	return C_PLAY_VIDEO_PARAMS_VERSION;
}
