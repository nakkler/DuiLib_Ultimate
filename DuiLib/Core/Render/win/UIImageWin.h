#ifndef __UIImageWin_H
#define __UIImageWin_H

#include "../UIImageBase.h"

namespace DuiLib
{

class CImageWin: public IImageBase
{
public:
#ifdef USE_XIMAGE_EFFECT
    virtual CxImage* LoadGifImageX(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0) override;
#endif
    virtual TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL) override;
    virtual void FreeImage(TImageInfo* pImageInfo, bool bDelete = true) override;
    virtual TImageInfo* LoadImage(LPCTSTR pStrImage, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL) override;
    virtual TImageInfo* LoadImage(UINT nID, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL) override;
    virtual TImageInfo* GdiplusLoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL) override;
};

}


#endif //__UIImageWin_H