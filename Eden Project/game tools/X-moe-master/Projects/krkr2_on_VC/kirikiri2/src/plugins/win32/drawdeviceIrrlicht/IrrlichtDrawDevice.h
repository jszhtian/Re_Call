#ifndef IRRLICHTDRAWDEVICE_H
#define IRRLICHTDRAWDEVICE_H

#include <windows.h>
#include "tp_stub.h"
#include <irrlicht.h>

#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_types.h"
#include "BasicDrawDevice.h"

class SWFMovie;

/**
 * Irrlicht �x�[�X�� DrawDevice
 */
class tTVPIrrlichtDrawDevice : public tTVPDrawDevice, public tTVPContinuousEventCallbackIntf,
							   public irr::IEventReceiver,
							   public gameswf::render_handler
{
	typedef tTVPDrawDevice inherited;

protected:
	/// �f�o�C�X
	irr::IrrlichtDevice *device;
	// �h���C�o
	irr::video::IVideoDriver *driver;
	
public:
	tTVPIrrlichtDrawDevice(); //!< �R���X�g���N�^

private:
	virtual ~tTVPIrrlichtDrawDevice(); //!< �f�X�g���N�^

	// �e�N�X�`���̊��蓖��
	void allocInfo(iTVPLayerManager * manager);
	// �e�N�X�`���̉��
	void freeInfo(iTVPLayerManager * manager);
	
	void attach(HWND hwnd);
	void detach();

	// Irrlicht �ɃC�x���g�𑗂�
	bool postEvent(irr::SEvent &ev);
	
public:
	//---- LayerManager �̊Ǘ��֘A
	virtual void TJS_INTF_METHOD AddLayerManager(iTVPLayerManager * manager);
	virtual void TJS_INTF_METHOD RemoveLayerManager(iTVPLayerManager * manager);

	//---- �`��ʒu�E�T�C�Y�֘A
	virtual void TJS_INTF_METHOD SetTargetWindow(HWND wnd);

	//---- ���[�U�[�C���^�[�t�F�[�X�֘A
	// window �� drawdevice
	virtual void TJS_INTF_METHOD OnMouseDown(tjs_int x, tjs_int y, tTVPMouseButton mb, tjs_uint32 flags);
	virtual void TJS_INTF_METHOD OnMouseUp(tjs_int x, tjs_int y, tTVPMouseButton mb, tjs_uint32 flags);
	virtual void TJS_INTF_METHOD OnMouseMove(tjs_int x, tjs_int y, tjs_uint32 flags);
	virtual void TJS_INTF_METHOD OnKeyDown(tjs_uint key, tjs_uint32 shift);
	virtual void TJS_INTF_METHOD OnKeyUp(tjs_uint key, tjs_uint32 shift);
	virtual void TJS_INTF_METHOD OnKeyPress(tjs_char key);
	virtual void TJS_INTF_METHOD OnMouseWheel(tjs_uint32 shift, tjs_int delta, tjs_int x, tjs_int y);
	
	//---- LayerManager ����̉摜�󂯓n���֘A
	virtual void TJS_INTF_METHOD StartBitmapCompletion(iTVPLayerManager * manager);
	virtual void TJS_INTF_METHOD NotifyBitmapCompleted(iTVPLayerManager * manager,
		tjs_int x, tjs_int y, const void * bits, const BITMAPINFO * bitmapinfo,
		const tTVPRect &cliprect, tTVPLayerType type, tjs_int opacity);
	virtual void TJS_INTF_METHOD EndBitmapCompletion(iTVPLayerManager * manager);

	//---------------------------------------------------------------------------
public:
	// �e�X�g�p����
	void init();

	// ------------------------------------------------------------
	// SWF �n UI
	// ------------------------------------------------------------

protected:
	/// UI �p SWF
	SWFMovie *uiSWF;

	void initSWF();
	void deinitSWF();

	bool first;
	tjs_uint64 prevTick;
	void drawSWF(tjs_uint64 tick, int x, int y, int width, int height);

	void notifyKeySWF(int code, bool down);

	int swfMouseX;
	int swfMouseY;
	int swfMouseButton;
	
public:
	// �ǂݍ���
	void loadSWF(const char *name);
	
	// ------------------------------------------------------------
	// SWF�����p
	// ------------------------------------------------------------

protected:
	// Enable/disable antialiasing.
	bool	m_enable_antialias;

	// Output size.
	float	m_display_width;
	float	m_display_height;
	
	irr::core::matrix4 m_current_matrix;
	gameswf::cxform	m_current_cxform;

	irr::core::matrix4 m_projection_bak;
	irr::core::matrix4 m_view_bak;
	irr::core::matrix4 m_world_bak;

	irr::video::SColor m_line_color;
	irr::video::SColor m_fill_color;
	
public:
	virtual gameswf::bitmap_info* create_bitmap_info_empty();
	virtual gameswf::bitmap_info *create_bitmap_info_alpha(int w, int h, unsigned char* data);
	virtual gameswf::bitmap_info *create_bitmap_info_rgb(image::rgb* im);
	virtual gameswf::bitmap_info *create_bitmap_info_rgba(image::rgba* im);
	virtual void delete_bitmap_info(gameswf::bitmap_info* bi);

	gameswf::YUV_video*	create_YUV_video(int w, int h);
	void delete_YUV_video(gameswf::YUV_video* yuv);

	//cairo_matrix_t viewport;
	
	// Bracket the displaying of a frame from a movie.
	// Fill the background color, and set up default
	// transforms, etc.
	virtual void	begin_display(gameswf::rgba bc,
								  int viewport_x0, int viewport_y0,
								  int viewport_width, int viewport_height,
								  float x0, float x1, float y0, float y1);
	virtual void	end_display();
	
	// Geometric and color transforms for mesh and line_strip rendering.
	virtual void	set_matrix(const gameswf::matrix& m);
	
	virtual void	set_cxform(const gameswf::cxform& cx);
	
	// Draw triangles using the current fill-style 0.
	// Clears the style list after rendering.
	//
	// coords is a list of (x,y) coordinate pairs, in
	// triangle-strip order.  The type of the array should
	// be Sint16[vertex_count*2]
	virtual void	draw_mesh_strip(const void* coords, int vertex_count);

	// As above, but coords is in triangle list order.
	virtual void	draw_triangle_list(const void *coords, int vertex_count);
	
	// Draw a line-strip using the current line style.
	// Clear the style list after rendering.
	//
	// Coords is a list of (x,y) coordinate pairs, in
	// sequence.  Each coord is a 16-bit signed integer.
	virtual void	draw_line_strip(const void* coords, int vertex_count);
		
	// Set line and fill styles for mesh & line_strip
	// rendering.
	enum bitmap_wrap_mode {
		WRAP_REPEAT,
		WRAP_CLAMP
	};

	virtual void	fill_style_disable(int fill_side);
	virtual void	fill_style_color(int fill_side, gameswf::rgba c);
	virtual void	fill_style_bitmap(int fill_side, const gameswf::bitmap_info* bi, const gameswf::matrix& m, gameswf::render_handler::bitmap_wrap_mode wm);
	
	virtual void	line_style_disable();
	virtual void	line_style_color(gameswf::rgba c);
	virtual void	line_style_width(float width);

	// Special function to draw a rectangular bitmap;
	// intended for textured glyph rendering.  Ignores
	// current transforms.
	virtual void draw_bitmap(
		const gameswf::matrix&		m,
		const gameswf::bitmap_info*	bi,
		const gameswf::rect&		coords,
		const gameswf::rect&		uv_coords,
		gameswf::rgba			color);

	virtual void set_antialiased(bool enable);
	virtual void begin_submit_mask();
	virtual void end_submit_mask();
	virtual void disable_mask();

	// ------------------------------------------------------------
	// �g���g������̌Ăяo������p
	// ------------------------------------------------------------
public:
	/**
	 * Irrlicht �Ăяo�������J�n
	 */
	void start();

	/**
	 * Irrlicht �Ăяo���������f
	 */
	void stop();

	/**
	 * Continuous �R�[���o�b�N
	 * �g���g�����ɂȂƂ��ɏ�ɌĂ΂��
	 * �h�蒼������
	 */
	virtual void TJS_INTF_METHOD OnContinuousCallback(tjs_uint64 tick);

	// ------------------------------------------------------------
	// Irrlicht �C�x���g�����p
	// ------------------------------------------------------------

	virtual bool OnEvent(irr::SEvent event);
};

#endif
