/**
 * @brief image资源管理类
 *
 * 详细描述：
 */

#ifndef __UIImageCore_H
#define __UIImageCore_H

#ifdef USE_XIMAGE_EFFECT
class CxImage;
#endif

namespace DuiLib
{

class CImageCore
{
public:
    // 图片加载
#ifdef USE_XIMAGE_EFFECT
    static CxImage* LoadGifImageX(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);
#endif
    static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL);
    static void FreeImage(TImageInfo* pImageInfo, bool bDelete = true);
    static TImageInfo* LoadImage(LPCTSTR pStrImage, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL);
    static TImageInfo* LoadImage(UINT nID, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL);
    static TImageInfo* GdiplusLoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, HINSTANCE instance = NULL);

};

}


#endif //__UIImageCore_H