﻿//这个类用于定义用于绘图的函数
#pragma once

enum class Alignment	//对齐方式
{
    LEFT,
    RIGHT,
    CENTER
};

class CDrawCommon
{
public:

    //拉伸模式
    enum class StretchMode
    {
        STRETCH,		//拉伸，会改变比例
        FILL,			//填充，不改变比例，会裁剪长边
        FIT			//适应，不会改变比例，不裁剪
    };

    //用于在DrawScrollText函数调用时使用的一些需要在函数调用完毕后继续存在的变量
    struct ScrollInfo
    {
        int shift_cnt{};		//移动的次数
        bool shift_dir{};		//移动的方向，右移为false，左移为true
        int freez{};			//当该变量大于0时，文本不滚动，直到小于等于0为止
        bool dir_changed{ false };	//如果方向发生了变化，则为true
        CString last_string;        //上一次绘制的文本
    };

    CDrawCommon();
    ~CDrawCommon();

    virtual void Create(CDC* pDC, CWnd* pMainWnd = nullptr);
    //void SetBackColor(COLORREF back_color);		//设置绘制文本时填充的背景颜色
    //COLORREF GetBackColor() const { return m_backColor; }
    void SetFont(CFont* pfont);		//设置绘制文本的字体
	CFont* GetFont() { return m_pfont; }
    void SetDC(CDC* pDC);		//设置绘图的DC
    CDC* GetDC()
    {
        return m_pDC;
    }

    //在指定的矩形区域内绘制有颜色的文本
    //rect: 文本的矩形区域
    //lpszString: 要绘制的文本
    //color: 文本的颜色
    //align: 文本的对齐方式（默认值：左对齐）
    //no_clip_area: 如果为true，则不在输出文字时限制绘图区域
    //multi_line: 是否多行显示
    //default_right_align: 如果文本的宽度大于矩形区域，是否右对齐
    void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color, Alignment align = Alignment::LEFT, bool no_clip_area = false, bool multi_line = false, bool default_right_align = false);

    //在指定的矩形区域内绘制分割颜色的文本
    //rect: 文本的矩形区域
    //lpszString: 要绘制的文本
    //color1: 分割位置左边的文本颜色
    //color2: 分割位置右边的文本颜色
    //split: 颜色分割位置，取值为0~1000（用于歌词动态显示）
    //center: 文本是否居中
    //no_clip_area: 如果为true，则不在输出文字时限制绘图区域
    void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color1, COLORREF color2, int split, Alignment align = Alignment::LEFT, bool no_clip_area = false);

    //在控件上绘制滚动的文本（当长度不够时），pixel指定此函数调用一次移动的像素值，如果reset为true，则滚动到初始位置
    //rect: 文本的矩形区域
    //lpszString: 要绘制的文本
    //color: 文本的颜色
    //pixel: 此函数调用一次滚动的像素值
    //center: 文本是否居中
    //scroll_info: 用来保存一些当前文本滚动的状态信息
    //reset: 如果reset为true，则重置scroll_info，并滚动到初始位置
    void DrawScrollText(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset = false);

    //函数功能和DrawScrollText一样，只是这个函数只会从左到右滚动，不会更换方向
    void DrawScrollText2(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset = false);

    static void SetDrawArea(CDC* pDC, CRect rect);
    void SetDrawArea(CRect rect);

    //绘制一个位图
    void DrawBitmap(CBitmap& bitmap, CPoint start_point, CSize size, StretchMode stretch_mode);
    void DrawBitmap(UINT bitmap_id, CPoint start_point, CSize size, StretchMode stretch_mode);
    void DrawBitmap(HBITMAP hbitmap, CPoint start_point, CSize size, StretchMode stretch_mode);

    void DrawIcon(HICON hIcon, CPoint start_point, CSize size);

    void FillRect(CRect rect, COLORREF color, bool no_clip_area = false);
    void FillAlphaRect(CRect rect, COLORREF color, BYTE alpha, bool no_clip_area = false);		//填充一个半透明的矩形（参照http://blog.csdn.net/lee353086/article/details/38311421）

    void DrawRectTopFrame(CRect rect, COLORREF color, int pilex = 1);
    void DrawRectOutLine(CRect rect, COLORREF color, int width, bool dot_line);
	void DrawLine(CPoint point1, CPoint point2, COLORREF color, int width, bool dot_line);

    CSize GetTextExtent(LPCTSTR str);

    //将图片拉伸到指定尺寸(https://blog.csdn.net/sichuanpb/article/details/22986877)
    static bool BitmapStretch(CImage *pImage, CImage *ResultImage, CSize size);

    static HICON LoadIconResource(UINT id, int width, int height);

protected:
    CDC* m_pDC{};		//用于绘图的CDC类的指针
    CWnd* m_pMainWnd{};	//绘图窗口的句柄
    //COLORREF m_backColor{ RGB(255,255,255) };
    CFont* m_pfont{};
};


//用于双缓冲绘图的类
class CDrawDoubleBuffer
{
public:
    CDrawDoubleBuffer(CDC* pDC, CRect rect)
        : m_pDC(pDC), m_rect(rect)
    {
        m_memDC.CreateCompatibleDC(NULL);
        m_memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
        m_pOldBit = m_memDC.SelectObject(&m_memBitmap);
    }

    ~CDrawDoubleBuffer()
    {
        m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &m_memDC, 0, 0, SRCCOPY);
        m_memDC.SelectObject(m_pOldBit);
        m_memBitmap.DeleteObject();
        m_memDC.DeleteDC();
    }

    CDC* GetMemDC()
    {
        return &m_memDC;
    }

private:
    CDC* m_pDC;
    CDC m_memDC;
    CBitmap m_memBitmap;
    CBitmap* m_pOldBit;
    CRect m_rect;
};