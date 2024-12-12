#pragma comment(lib, "strmiids.lib")
#include "layerExMovie.hpp"

/**
 * �R���X�g���N�^
 */
layerExMovie::layerExMovie(DispatchT obj) : _pType(obj, TJS_W("type")), layerExBase(obj)
{
	pAMStream         = NULL;
	pPrimaryVidStream = NULL;
	pDDStream         = NULL;
	pSample           = NULL; 
	pSurface          = NULL;
	loop = false;
	alpha = false;
	movieWidth = 0;
	movieHeight = 0;
}

/**
 * �f�X�g���N�^
 */
layerExMovie::~layerExMovie()
{
	stopMovie();
}

/**
 * ���[�r�[�t�@�C�����J���ď�������
 * @param filename �t�@�C����
 * @param alpha �A���t�@�w��i�����̃T�C�Y�Ń���������j
 */
void
layerExMovie::openMovie(const tjs_char* filename, bool alpha)
{ 
	this->alpha = alpha;
	movieWidth = 0;
	movieHeight = 0;

	// �p�X����
	ttstr path = TVPGetPlacedPath(ttstr(filename));
	// ���[�J�����ɕϊ�
	TVPGetLocalName(path);

	if (SUCCEEDED(CoCreateInstance(CLSID_AMMultiMediaStream,0,1,IID_IAMMultiMediaStream,(void**)&pAMStream))) {
		if (SUCCEEDED(pAMStream->Initialize(STREAMTYPE_READ, 0, NULL)) &&
			SUCCEEDED(pAMStream->AddMediaStream(0, &MSPID_PrimaryVideo, 0, NULL))) {
			if (SUCCEEDED(pAMStream->OpenFile(path.c_str(), AMMSF_NOCLOCK))) {
				if (SUCCEEDED(pAMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream))) {
					if (SUCCEEDED(pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream,(void**)&pDDStream))) {

						// �t�H�[�}�b�g�̎w�� ARGB32
						DDSURFACEDESC desc;
						ZeroMemory(&desc, sizeof DDSURFACEDESC);
						desc.dwSize = sizeof(DDSURFACEDESC);
						desc.dwFlags = DDSD_CAPS | DDSD_PIXELFORMAT;
						desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
						desc.ddpfPixelFormat.dwSize  = sizeof(DDPIXELFORMAT);
						desc.ddpfPixelFormat.dwFlags = DDPF_RGB; 
						desc.ddpfPixelFormat.dwRGBBitCount = 32;
						desc.ddpfPixelFormat.dwRBitMask = 0x00FF0000;
						desc.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
						desc.ddpfPixelFormat.dwBBitMask = 0x000000FF;
//						desc.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;
						HRESULT hr;
						if (SUCCEEDED(hr = pDDStream->SetFormat(&desc, NULL))) {
							if (SUCCEEDED(pDDStream->CreateSample(0,0,0,&pSample))) {
								RECT rect; 
								if (SUCCEEDED(pSample->GetSurface(&pSurface,&rect))) {
									movieWidth  = rect.right - rect.left;
									movieHeight = rect.bottom - rect.top;
									if (alpha) {
										movieWidth /= 2;
									}
									// �摜�T�C�Y�����[�r�[�̃T�C�Y�ɂ��킹��
									_pWidth.SetValue(movieWidth);
									_pHeight.SetValue(movieHeight);
									_pType.SetValue(alpha ? ltAlpha : ltOpaque);
								} else {
									// �T�[�t�F�[�X�擾���s
									pSample->Release();
									pSample = NULL;
									pDDStream->Release();
									pDDStream = NULL;
									pPrimaryVidStream->Release();
									pPrimaryVidStream = NULL;
									pAMStream->Release();
									pAMStream = NULL;
								}
							}
						} else {
							// �T���v���쐻���s
							pDDStream->Release();
							pDDStream = NULL;
							pPrimaryVidStream->Release();
							pPrimaryVidStream = NULL;
							pAMStream->Release();
							pAMStream = NULL;
						}
					} else {
						// DirectDraw �X�g���[���擾���s
						pPrimaryVidStream->Release();
						pPrimaryVidStream = NULL;
						pAMStream->Release();
						pAMStream = NULL;
					}
				} else {
					// �r�f�I�X�g���[���擾���s
					pAMStream->Release();
					pAMStream = NULL;
				}
			} else {
				// �t�@�C���I�[�v���Ɏ��s
				pAMStream->Release();
				pAMStream = NULL;
			}
		} else {
			pAMStream->Release();
			pAMStream = NULL;
		}
	}
} 

/**
 * ���[�r�[�̊J�n
 */
void
layerExMovie::startMovie(bool loop)
{
	if (pSample) {
		// �Đ��J�n
		this->loop = loop;
		pAMStream->SetState(STREAMSTATE_RUN);
		pSample->Update(SSUPDATE_ASYNC, NULL, NULL, 0);
		start();
	}
}

/**
 * ���[�r�[�̒�~
 */
void
layerExMovie::stopMovie()
{
	stop();
	if (pAMStream) {
		pAMStream->SetState(STREAMSTATE_STOP);
		if (pSurface) {
			pSurface->Release();
			pSurface = NULL;
		}
		if (pSample) {
			pSample->Release();
			pSample = NULL;
		}
		if (pDDStream) {
			pDDStream->Release();
			pDDStream = NULL;
		}
		if (pPrimaryVidStream) {
			pPrimaryVidStream->Release();
			pPrimaryVidStream = NULL;
		}
		if (pAMStream) {
			pAMStream->Release();
			pAMStream = NULL;
		}
	}
}

void
layerExMovie::start()
{
	stop();
	TVPAddContinuousEventHook(this);
}

/**
 * Irrlicht �Ăяo��������~
 */
void
layerExMovie::stop()
{
	TVPRemoveContinuousEventHook(this);
}

void TJS_INTF_METHOD
layerExMovie::OnContinuousCallback(tjs_uint64 tick)
{
	if (pSample) {
		// �X�V
		HRESULT hr = pSample->CompletionStatus(0,0);
		if (hr == MS_S_PENDING) {
//			TVPAddLog("�X�V�҂�");
		} else if (hr == S_OK) {
			// �X�V����
			// �T�[�t�F�[�X���烌�C���ɉ�ʃR�s�[
			reset();
			DDSURFACEDESC  ddsd; 
			ddsd.dwSize=sizeof(DDSURFACEDESC); 
			if (SUCCEEDED(pSurface->Lock( NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT , NULL))) {
				if (alpha) {
					int w = movieWidth*4;
					for (int y=0; y<ddsd.dwHeight && y<_height; y++) {
						BYTE *dst  = _buffer+(y*_pitch);
						BYTE *src1 = (BYTE*)ddsd.lpSurface+y*ddsd.lPitch;
						BYTE *src2 = src1 + w;
						for (int x=0; x<_width;x++) {
							*dst++ = *src1++;
							*dst++ = *src1++;
							*dst++ = *src1++;
							*dst++ = *src2; src2+=4; src1++;
						}
					}
				} else {
					int w = _width < movieWidth ? _width * 4 : movieWidth * 4;
					for (int y=0; y<ddsd.dwHeight && y<_height; y++) {
						memcpy(_buffer+(y*_pitch), (BYTE*)ddsd.lpSurface+y*ddsd.lPitch, w);
					}
				}
				pSurface->Unlock(NULL); 
			}
			redraw();
			pSample->Update(SSUPDATE_ASYNC, NULL, NULL, 0);
		} else if (hr == MS_S_ENDOFSTREAM) {
			// �X�V�I��
			if (loop) {
				pAMStream->Seek(0);
				pSample->Update(SSUPDATE_ASYNC, NULL, NULL, 0);
			} else {
				stopMovie();
			}
		} else {
			stopMovie();
		}
	} else {
		stop();
	}
}
