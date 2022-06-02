#include "stdafx.h"
#include "UIImageWin.h"

#define STB_IMAGE_IMPLEMENTATION
#include "..\..\..\Utils\stb_image.h"

#ifdef USE_XIMAGE_EFFECT
#	include "../../../3rd/CxImage/ximage.h"
#	include "../../../3rd/CxImage/ximage.cpp"
#	include "../../../3rd/CxImage/ximaenc.cpp"
#	include "../../../3rd/CxImage/ximagif.cpp"
#	include "../../../3rd/CxImage/ximainfo.cpp"
#	include "../../../3rd/CxImage/ximalpha.cpp"
#	include "../../../3rd/CxImage/ximapal.cpp"
#	include "../../../3rd/CxImage/ximatran.cpp"
#	include "../../../3rd/CxImage/ximawnd.cpp"
#	include "../../../3rd/CxImage/xmemfile.cpp"
#endif

namespace DuiLib
{


#ifdef USE_XIMAGE_EFFECT
static DWORD LoadImage2Memory(const STRINGorID& bitmap, LPCTSTR type, LPBYTE& pData)
{
    assert(pData == NULL);
    pData = NULL;
    DWORD dwSize(0U);
    do
    {
        if (type == NULL)
        {
            CDuiString sFile = CPaintManagerUI::GetResourcePath();
            if (CPaintManagerUI::GetResourceZip().IsEmpty())
            {
                sFile += bitmap.m_lpstr;
                HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
                                            FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile == INVALID_HANDLE_VALUE) break;
                dwSize = ::GetFileSize(hFile, NULL);
                if (dwSize == 0) break;

                DWORD dwRead = 0;
                pData = new BYTE[dwSize + 1];
                memset(pData, 0, dwSize + 1);
                ::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
                ::CloseHandle(hFile);

                if (dwRead != dwSize)
                {
                    delete[] pData;
                    pData = NULL;
                    dwSize = 0U;
                    break;
                }
            }
            else
            {
                sFile += CPaintManagerUI::GetResourceZip();
                HZIP hz = NULL;
                if (CPaintManagerUI::IsCachedResourceZip())
                    hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
                else
                {
                    CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
#ifdef UNICODE
                    char* pwd = w2a((wchar_t*)sFilePwd.GetData());
                    hz = OpenZip((void*)sFile.GetData(), pwd);
                    if (pwd) delete[] pwd;
#else
                    hz = OpenZip((void*)sFile.GetData(), sFilePwd.GetData());
#endif
                }
                if (hz == NULL) break;
                ZIPENTRY ze;
                int i = 0;
                CDuiString key = bitmap.m_lpstr;
                key.Replace(_T("\\"), _T("/"));
                if (FindZipItem(hz, key, true, &i, &ze) != 0) break;
                dwSize = ze.unc_size;
                if (dwSize == 0) break;
                pData = new BYTE[dwSize];
                int res = UnzipItem(hz, i, pData, dwSize, 3);
                if (res != 0x00000000 && res != 0x00000600)
                {
                    delete[] pData;
                    pData = NULL;
                    dwSize = 0U;
                    if (!CPaintManagerUI::IsCachedResourceZip())
                        CloseZip(hz);
                    break;
                }
                if (!CPaintManagerUI::IsCachedResourceZip())
                    CloseZip(hz);
            }
        }
        else
        {
            HINSTANCE hDll = CPaintManagerUI::GetResourceDll();
            HRSRC hResource = ::FindResource(hDll, bitmap.m_lpstr, type);
            if (hResource == NULL) break;
            HGLOBAL hGlobal = ::LoadResource(hDll, hResource);
            if (hGlobal == NULL)
            {
                FreeResource(hResource);
                break;
            }

            dwSize = ::SizeofResource(hDll, hResource);
            if (dwSize == 0) break;
            pData = new BYTE[dwSize];
            ::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
            ::FreeResource(hGlobal);
        }
    } while (0);

    while (!pData)
    {
        //读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
        HANDLE hFile = ::CreateFile(bitmap.m_lpstr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
                                    FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) break;
        dwSize = ::GetFileSize(hFile, NULL);
        if (dwSize == 0) break;

        DWORD dwRead = 0;
        pData = new BYTE[dwSize];
        ::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
        ::CloseHandle(hFile);

        if (dwRead != dwSize)
        {
            delete[] pData;
            pData = NULL;
            dwSize = 0U;
        }
        break;
    }
    return dwSize;
}
CxImage* CImageWin::LoadGifImageX(STRINGorID bitmap, LPCTSTR type, DWORD mask)
{
    //write by wangji
    LPBYTE pData = NULL;
    DWORD dwSize = LoadImage2Memory(bitmap, type, pData);
    if (dwSize == 0U || !pData)
        return NULL;
    CxImage* pImg = NULL;
    if (pImg = new CxImage())
    {
        pImg->SetRetreiveAllFrames(TRUE);
        if (!pImg->Decode(pData, dwSize, CXIMAGE_FORMAT_GIF))
        {
            delete pImg;
            pImg = nullptr;
        }
    }
    delete[] pData;
    pData = NULL;
    return pImg;
}
#endif//USE_XIMAGE_EFFECT

TImageInfo* CImageWin::LoadImage(STRINGorID bitmap, LPCTSTR type, DWORD mask, HINSTANCE instance)
{
    LPBYTE pData = NULL;
    DWORD dwSize = 0;
    do
    {
        if (type == NULL)
        {
            CDuiString sFile = CPaintManagerUI::GetResourcePath();
            if (CPaintManagerUI::GetResourceZip().IsEmpty())
            {
                sFile += bitmap.m_lpstr;
                HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
                                            FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile == INVALID_HANDLE_VALUE) break;
                dwSize = ::GetFileSize(hFile, NULL);
                if (dwSize == 0) break;

                DWORD dwRead = 0;
                pData = new BYTE[dwSize];
                ::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
                ::CloseHandle(hFile);

                if (dwRead != dwSize)
                {
                    delete[] pData;
                    pData = NULL;
                    break;
                }
            }
            else
            {
                sFile += CPaintManagerUI::GetResourceZip();
                CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
                HZIP hz = NULL;
                if (CPaintManagerUI::IsCachedResourceZip()) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
                else
                {
#ifdef UNICODE
                    char* pwd = w2a((wchar_t*)sFilePwd.GetData());
                    hz = OpenZip(sFile.GetData(), pwd);
                    if (pwd) delete[] pwd;
#else
                    hz = OpenZip(sFile.GetData(), sFilePwd.GetData());
#endif
                }
                if (hz == NULL) break;
                ZIPENTRY ze;
                int i = 0;
                CDuiString key = bitmap.m_lpstr;
                key.Replace(_T("\\"), _T("/"));
                if (FindZipItem(hz, key, true, &i, &ze) != 0) break;
                dwSize = ze.unc_size;
                if (dwSize == 0) break;
                pData = new BYTE[dwSize];
                int res = UnzipItem(hz, i, pData, dwSize);
                if (res != 0x00000000 && res != 0x00000600)
                {
                    delete[] pData;
                    pData = NULL;
                    if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
                    break;
                }
                if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
            }
        }
        else
        {
            HINSTANCE dllinstance = NULL;
            if (instance)
            {
                dllinstance = instance;
            }
            else
            {
                dllinstance = CPaintManagerUI::GetResourceDll();
            }
            HRSRC hResource = ::FindResource(dllinstance, bitmap.m_lpstr, type);
            if (hResource == NULL) break;
            HGLOBAL hGlobal = ::LoadResource(dllinstance, hResource);
            if (hGlobal == NULL)
            {
                FreeResource(hResource);
                break;
            }

            dwSize = ::SizeofResource(dllinstance, hResource);
            if (dwSize == 0) break;
            pData = new BYTE[dwSize];
            ::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
            ::FreeResource(hGlobal);
        }
    } while (0);

    while (!pData)
    {
        //读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
        HANDLE hFile = ::CreateFile(bitmap.m_lpstr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
                                    FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) break;
        dwSize = ::GetFileSize(hFile, NULL);
        if (dwSize == 0) break;

        DWORD dwRead = 0;
        pData = new BYTE[dwSize];
        ::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
        ::CloseHandle(hFile);

        if (dwRead != dwSize)
        {
            delete[] pData;
            pData = NULL;
        }
        break;
    }
    if (!pData)
    {
        return NULL;
    }

    LPBYTE pImage = NULL;
    int x, y, n;
    pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
    delete[] pData;
    if (!pImage)
    {
        return NULL;
    }

    BITMAPINFO bmi;
    ::ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = x;
    bmi.bmiHeader.biHeight = -y;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = x * y * 4;

    bool bAlphaChannel = false;
    LPBYTE pDest = NULL;
    HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
    if (!hBitmap)
    {
        return NULL;
    }

    for (int i = 0; i < x * y; i++)
    {
        pDest[i * 4 + 3] = pImage[i * 4 + 3];
        if (pDest[i * 4 + 3] < 255)
        {
            pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2]) * pImage[i * 4 + 3] / 255);
            pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1]) * pImage[i * 4 + 3] / 255);
            pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4]) * pImage[i * 4 + 3] / 255);
            bAlphaChannel = true;
        }
        else
        {
            pDest[i * 4] = pImage[i * 4 + 2];
            pDest[i * 4 + 1] = pImage[i * 4 + 1];
            pDest[i * 4 + 2] = pImage[i * 4];
        }

        if (*(DWORD*)(&pDest[i * 4]) == mask)
        {
            pDest[i * 4] = (BYTE)0;
            pDest[i * 4 + 1] = (BYTE)0;
            pDest[i * 4 + 2] = (BYTE)0;
            pDest[i * 4 + 3] = (BYTE)0;
            bAlphaChannel = true;
        }
    }

    stbi_image_free(pImage);

    TImageInfo* data = new TImageInfo;
    data->pBits = NULL;
    data->pSrcBits = NULL;
    data->hBitmap = hBitmap;
    data->nX = x;
    data->nY = y;
    data->bAlpha = bAlphaChannel;
    return data;
}

void CImageWin::FreeImage(TImageInfo* pImageInfo, bool bDelete)
{
    if (pImageInfo == NULL) return;

    if (pImageInfo->pImage)
    {
        delete pImageInfo->pImage;
    }
    pImageInfo->pImage = NULL;

    if (pImageInfo->hBitmap)
    {
        ::DeleteObject(pImageInfo->hBitmap);
    }
    pImageInfo->hBitmap = NULL;

    if (pImageInfo->pBits)
    {
        delete[] pImageInfo->pBits;
    }
    pImageInfo->pBits = NULL;

    if (pImageInfo->pSrcBits)
    {
        delete[] pImageInfo->pSrcBits;
    }
    pImageInfo->pSrcBits = NULL;

    if (bDelete)
    {
        delete pImageInfo;
        pImageInfo = NULL;
    }
}

TImageInfo* CImageWin::LoadImage(LPCTSTR pStrImage, LPCTSTR type, DWORD mask, HINSTANCE instance)
{
    if (pStrImage == NULL) return NULL;

    CDuiString sStrPath = pStrImage;
    if (type == NULL)
    {
        sStrPath = CResourceManager::GetInstance()->GetImagePath(pStrImage);
        if (sStrPath.IsEmpty()) sStrPath = pStrImage;
    }
    return LoadImage(STRINGorID(sStrPath.GetData()), type, mask, instance);
}

TImageInfo* CImageWin::LoadImage(UINT nID, LPCTSTR type, DWORD mask, HINSTANCE instance)
{
    return LoadImage(STRINGorID(nID), type, mask, instance);
}

TImageInfo* CImageWin::GdiplusLoadImage(STRINGorID bitmap, LPCTSTR type /*= NULL*/, DWORD mask /*= 0*/, HINSTANCE instance /*= NULL*/)
{
    LPBYTE pData = NULL;
    DWORD dwSize = 0;
    do
    {
        if (type == NULL)
        {
            CDuiString sFile = CPaintManagerUI::GetResourcePath();
            if (CPaintManagerUI::GetResourceZip().IsEmpty())
            {
                sFile += bitmap.m_lpstr;
                HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
                                            FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile == INVALID_HANDLE_VALUE) break;
                dwSize = ::GetFileSize(hFile, NULL);
                if (dwSize == 0) break;

                DWORD dwRead = 0;
                pData = new BYTE[dwSize];
                ::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
                ::CloseHandle(hFile);

                if (dwRead != dwSize)
                {
                    delete[] pData;
                    pData = NULL;
                    break;
                }
            }
            else
            {
                sFile += CPaintManagerUI::GetResourceZip();
                CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
                HZIP hz = NULL;
                if (CPaintManagerUI::IsCachedResourceZip()) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
                else
                {
#ifdef UNICODE
                    char* pwd = w2a((wchar_t*)sFilePwd.GetData());
                    hz = OpenZip(sFile.GetData(), pwd);
                    if (pwd) delete[] pwd;
#else
                    hz = OpenZip(sFile.GetData(), sFilePwd.GetData());
#endif
                }
                if (hz == NULL) break;
                ZIPENTRY ze;
                int i = 0;
                CDuiString key = bitmap.m_lpstr;
                key.Replace(_T("\\"), _T("/"));
                if (FindZipItem(hz, key, true, &i, &ze) != 0) break;
                dwSize = ze.unc_size;
                if (dwSize == 0) break;
                pData = new BYTE[dwSize];
                int res = UnzipItem(hz, i, pData, dwSize);
                if (res != 0x00000000 && res != 0x00000600)
                {
                    delete[] pData;
                    pData = NULL;
                    if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
                    break;
                }
                if (!CPaintManagerUI::IsCachedResourceZip()) CloseZip(hz);
            }
        }
        else
        {
            HINSTANCE dllinstance = NULL;
            if (instance)
            {
                dllinstance = instance;
            }
            else
            {
                dllinstance = CPaintManagerUI::GetResourceDll();
            }
            HRSRC hResource = ::FindResource(dllinstance, bitmap.m_lpstr, type);
            if (hResource == NULL) break;
            HGLOBAL hGlobal = ::LoadResource(dllinstance, hResource);
            if (hGlobal == NULL)
            {
                FreeResource(hResource);
                break;
            }

            dwSize = ::SizeofResource(dllinstance, hResource);
            if (dwSize == 0) break;
            pData = new BYTE[dwSize];
            ::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
            ::FreeResource(hGlobal);
        }
    } while (0);

    while (!pData)
    {
        //读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
        HANDLE hFile = ::CreateFile(bitmap.m_lpstr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
                                    FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) break;
        dwSize = ::GetFileSize(hFile, NULL);
        if (dwSize == 0) break;

        DWORD dwRead = 0;
        pData = new BYTE[dwSize];
        ::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
        ::CloseHandle(hFile);

        if (dwRead != dwSize)
        {
            delete[] pData;
            pData = NULL;
        }
        break;
    }

    if (pData == NULL)
    {
        return NULL;
    }

    // GDI
    bool bAlphaChannel = false;
    HBITMAP hBitmap = NULL;
    int x, y, n;
    LPBYTE pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &n, 4);
    if (pImage != NULL)
    {
        BITMAPINFO bmi;
        ::ZeroMemory(&bmi, sizeof(BITMAPINFO));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = x;
        bmi.bmiHeader.biHeight = -y;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = x * y * 4;

        LPBYTE pDest = NULL;
        hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
        if (hBitmap != NULL)
        {
            for (int i = 0; i < x * y; i++)
            {
                pDest[i * 4 + 3] = pImage[i * 4 + 3];
                if (pDest[i * 4 + 3] < 255)
                {
                    pDest[i * 4] = (BYTE)(DWORD(pImage[i * 4 + 2]) * pImage[i * 4 + 3] / 255);
                    pDest[i * 4 + 1] = (BYTE)(DWORD(pImage[i * 4 + 1]) * pImage[i * 4 + 3] / 255);
                    pDest[i * 4 + 2] = (BYTE)(DWORD(pImage[i * 4]) * pImage[i * 4 + 3] / 255);
                    bAlphaChannel = true;
                }
                else
                {
                    pDest[i * 4] = pImage[i * 4 + 2];
                    pDest[i * 4 + 1] = pImage[i * 4 + 1];
                    pDest[i * 4 + 2] = pImage[i * 4];
                }

                if (*(DWORD*)(&pDest[i * 4]) == mask)
                {
                    pDest[i * 4] = (BYTE)0;
                    pDest[i * 4 + 1] = (BYTE)0;
                    pDest[i * 4 + 2] = (BYTE)0;
                    pDest[i * 4 + 3] = (BYTE)0;
                    bAlphaChannel = true;
                }
            }
            stbi_image_free(pImage);
        }
    }

    // GdiPlus
    HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
    BYTE* pMem = (BYTE*)::GlobalLock(hMem);
    memcpy(pMem, pData, dwSize);
    IStream* pStm = NULL;
    ::CreateStreamOnHGlobal(hMem, TRUE, &pStm);

    Gdiplus::Image* pGdiplusImage = Gdiplus::Image::FromStream(pStm);
    if (!pGdiplusImage || pGdiplusImage->GetLastStatus() != Gdiplus::Ok)
    {
        pGdiplusImage = NULL;

        pStm->Release();
        ::GlobalUnlock(hMem);
    }

    delete[] pData;
    pData = NULL;

    if (hBitmap == NULL && pGdiplusImage == NULL)
    {
        return NULL;
    }

    TImageInfo* data = new TImageInfo;
    data->pBits = NULL;
    data->pSrcBits = NULL;
    data->pImage = pGdiplusImage;
    data->hBitmap = hBitmap;
    data->nX = x;
    data->nY = y;
    data->bAlpha = bAlphaChannel;
    return data;
}

}


