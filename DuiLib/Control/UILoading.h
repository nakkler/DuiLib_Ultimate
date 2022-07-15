#ifndef __UILoadingCircle_H
#define __UILoadingCircle_H

namespace DuiLib
{

enum StylePresets
{
	Custom,
	MacOSX,
	Firefox,
	IE7
};

class UILIB_API CLoadingUI : public CControlUI
{
    DECLARE_DUICONTROL(CControlUI)

	enum TIMEID
	{
		kTimerLoadingId = 100,
	};
public:
	CLoadingUI();
	virtual ~CLoadingUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void Start();
	void Stop();
protected:
    virtual void PaintBkImage(HDC hDC);
    virtual void DoEvent(TEventUI& event);
	virtual void Init();
	Gdiplus::Color* GenerateColorsPallet(Gdiplus::Color _objColor, bool _blnShadeColor, int _intNbSpoke);

	static DWORD WINAPI _ThreadFun(LPVOID p);
protected:
	int                 m_nNumber;
	int                 m_nTime;
	bool				m_bStop;
<<<<<<< HEAD
	bool				m_bExit;
	HANDLE				m_pTrdAni;
=======
>>>>>>> master

	int m_NumberOfSpoke;//��������
	int m_SpokeThickness;//������ϸ
	int m_ProgressValue;//------------------------
	int m_OuterCircleRadius;//��Ȧ
	int m_InnerCircleRadius;//��Ȧ
	Gdiplus::PointF m_CenterPoint;//��άƽ��ĵ�
	Gdiplus::Color m_Color;
	Gdiplus::Color* m_Colors;
	double* m_Angles;
<<<<<<< HEAD

	HANDLE m_condQueue;//�¼�֪ͨ
=======
>>>>>>> master
};

CControlUI* CreateLoadingControl(LPCTSTR pstrType);

}


#endif //__UILoadingCircle_H
