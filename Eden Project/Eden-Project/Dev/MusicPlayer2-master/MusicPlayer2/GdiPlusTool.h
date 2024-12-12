﻿#pragma once
#include <gdiplus.h>
#include "CommonData.h"

class CGdiPlusTool
{
public:
    CGdiPlusTool();
    ~CGdiPlusTool();

    static Gdiplus::Color COLORREFToGdiplusColor(COLORREF color, BYTE alpha = 255);
    static int ToGDIPluseFontStyle(const FontStyle& style);
};

