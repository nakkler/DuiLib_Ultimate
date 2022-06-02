#ifndef __UIImageBase_H
#define __UIImageBase_H

namespace DuiLib
{

class IImageBase
{
public:
    // Õº∆¨º”‘ÿ
#ifdef USE_XIMAGE_EFFECT
    virtual CxImage* LoadGifImageX(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0) = 0;
#endif
    virtual TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL) = 0;
    virtual void FreeImage(TImageInfo* pImageInfo, bool bDelete = true) = 0;
    virtual TImageInfo* LoadImage(LPCTSTR pStrImage, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL) = 0;
    virtual TImageInfo* LoadImage(UINT nID, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL) = 0;

    virtual TImageInfo* GdiplusLoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL) = 0;

};

}

#endif //__UIImageBase_H