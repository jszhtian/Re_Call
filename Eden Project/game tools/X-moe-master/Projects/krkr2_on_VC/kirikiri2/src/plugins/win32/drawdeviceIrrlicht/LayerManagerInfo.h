#ifndef LAYERMANAGERINFO_H
#define LAYERMANAGERINFO_H

/**
 * ���C���}�l�[�W���p�t�����
 */
class LayerManagerInfo {
	
public:
	LayerManagerInfo(irr::video::ITexture *texture);
	virtual ~LayerManagerInfo();
	
private:
	// �R�s�[�����p�ꎞ�ϐ�
	unsigned char *destBuffer;
	int destWidth;
	int destHeight;
	int destPitch;
	
public:
	// ���蓖�ăe�N�X�`��
	irr::video::ITexture *texture;

	// �e�N�X�`���`�摀��p
	void lock();
	void copy(tjs_int x, tjs_int y, const void * bits, const BITMAPINFO * bitmapinfo,
			  const tTVPRect &cliprect, tTVPLayerType type, tjs_int opacity);
	void unlock();
};

#endif
