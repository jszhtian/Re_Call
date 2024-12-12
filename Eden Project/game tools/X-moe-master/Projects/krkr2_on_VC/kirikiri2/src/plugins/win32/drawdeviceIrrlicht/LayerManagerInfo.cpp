#include <windows.h>
#include "IrrlichtDrawDevice.h"
#include "LayerManagerInfo.h"

/**
 * �R���X�g���N�^
 */
LayerManagerInfo::LayerManagerInfo(irr::video::ITexture *texture) : texture(texture)
{
	destBuffer = NULL;
};

/**
 * �f�X�g���N�^
 */
LayerManagerInfo::~LayerManagerInfo()
{
}


/**
 * �e�N�X�`�������b�N���ĕ`��̈�����擾����
 */
void
LayerManagerInfo::lock()
{
	if (texture) {
		destBuffer = (unsigned char *)texture->lock();
		irr::core::dimension2d<irr::s32> size = texture->getSize();
		destWidth  = size.Width;
		destHeight = size.Height;
		destPitch  = texture->getPitch();
	} else {
		destBuffer = NULL;
	}
}

/**
 * ���b�N���ꂽ�e�N�X�`���Ƀr�b�g�}�b�v�`����s��
 */
void
LayerManagerInfo::copy(tjs_int x, tjs_int y, const void * bits, const BITMAPINFO * bitmapinfo,
					   const tTVPRect &cliprect, tTVPLayerType type, tjs_int opacity)
{
	// bits, bitmapinfo �ŕ\�����r�b�g�}�b�v�� cliprect �̗̈���Ax, y �ɕ`�悷��B

	if (destBuffer) {
		int srcPitch = -bitmapinfo->bmiHeader.biWidth * 4; // XXX ���߂���
		unsigned char *srcBuffer = (unsigned char *)bits - srcPitch * (bitmapinfo->bmiHeader.biHeight - 1);
		int srcx   = cliprect.left;
		int srcy   = cliprect.top;
		int width  = cliprect.get_width();
		int height = cliprect.get_height();
		// �N���b�s���O
		if (x < 0) {
			srcx  += x;
			width += x;
			x = 0;
		}
		if (x + width > destWidth) {
			width -= ((x + width) - destWidth);
		}
		if (y < 0) {
			srcy += y;
			height += y;
			y = 0;
		}
		if (y + height > destHeight) {
			height -= ((y + height) - destHeight);
		}
		unsigned char *src  = srcBuffer  + srcy * srcPitch  + srcx * 4;
		unsigned char *dest = destBuffer +    y * destPitch +    x * 4;
		for (int i=0;i<height;i++) {
			memcpy(dest, src, width * 4);
			src  += srcPitch;
			dest += destPitch;
		}
	}
}

/**
 * �e�N�X�`���̃��b�N�̉���
 */
void
LayerManagerInfo::unlock()
{
	if (texture) {
		texture->unlock();
		destBuffer = NULL;
	}
}
