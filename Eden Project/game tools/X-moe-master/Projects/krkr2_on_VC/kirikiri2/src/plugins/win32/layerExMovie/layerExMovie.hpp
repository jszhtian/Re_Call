#ifndef _layExMovie_hpp_
#define _layExMovie_hpp_

#include <windows.h>
#include <dshow.h> 
#include <mmstream.h> 
#include <amstream.h> 
#include <ddstream.h> 

#include "layerExBase.hpp"

/*
 * Movie �`��p���C��
 */
struct layerExMovie : public layerExBase, public tTVPContinuousEventCallbackIntf
{
protected:
	IAMMultiMediaStream*     pAMStream; 
	IMediaStream*            pPrimaryVidStream; 
	IDirectDrawMediaStream*  pDDStream; 
	IDirectDrawStreamSample* pSample; 
	IDirectDrawSurface*      pSurface; 

	ObjectT _pType;

	int movieWidth;
	int movieHeight;
	
	bool loop;
	bool alpha;
	
public:
	layerExMovie(DispatchT obj);
	~layerExMovie();
	
public:
	
	// ���[�r�[�̃��[�h
	void openMovie(const tjs_char* filename, bool alpha);

	void startMovie(bool loop);
	void stopMovie();
	
	void start();
	void stop();
	
	/**
	 * Continuous �R�[���o�b�N
	 * �g���g�����ɂȂƂ��ɏ�ɌĂ΂��
	 * �h�蒼������
	 */
	virtual void TJS_INTF_METHOD OnContinuousCallback(tjs_uint64 tick);
};

#endif
