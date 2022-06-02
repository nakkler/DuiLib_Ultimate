#include "stdafx.h"
#include "UIImageCore.h"
#include "Render/win/UIImageWin.h"

namespace DuiLib
{
#ifdef USE_XIMAGE_EFFECT
CxImage* CImageCore::LoadGifImageX(STRINGorID bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/)
{
#if defined(_WIN32)
    CImageWin image;
    return image.LoadGifImageX(bitmap, type, mask);
#elif defined(__linux__)
    return nullptr;
#endif
}
#endif

DuiLib::TImageInfo* CImageCore::LoadImage(STRINGorID bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, HINSTANCE instance /*= NULL*/)
{
#if defined(_WIN32)
    CImageWin image;
    return image.LoadImage(bitmap, type, mask, instance);
#elif defined(__linux__)
    return nullptr;
#endif
}

DuiLib::TImageInfo* CImageCore::LoadImage(UINT nID, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, HINSTANCE instance /*= NULL*/)
{
#if defined(_WIN32)
    CImageWin image;
    return image.LoadImage(nID, type, mask, instance);
#elif defined(__linux__)
    return nullptr;
#endif
}

DuiLib::TImageInfo* CImageCore::LoadImage(LPCTSTR pStrImage, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, HINSTANCE instance /*= NULL*/)
{
#if defined(_WIN32)
    CImageWin image;
    return image.LoadImage(pStrImage, type, mask, instance);
#elif defined(__linux__)
    return nullptr;
#endif
}

void CImageCore::FreeImage(TImageInfo* pImageInfo, bool bDelete /*= true*/)
{
#if defined(_WIN32)
    CImageWin image;
    return image.FreeImage(pImageInfo, bDelete);
#elif defined(__linux__)
    return nullptr;
#endif
}

TImageInfo* CImageCore::GdiplusLoadImage(STRINGorID bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, HINSTANCE instance /*= NULL*/)
{
#if defined(_WIN32)
    CImageWin image;
    return image.GdiplusLoadImage(bitmap, type, mask, instance);
#elif defined(__linux__)
    return nullptr;
#endif
}

}
