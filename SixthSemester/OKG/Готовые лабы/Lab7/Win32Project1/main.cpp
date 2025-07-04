#include "CMatrix.h"
#include "CPyramid.h"
#include <afxwin.h>
#include <windows.h>
#include <fstream>

#define ID_Dr1 2002
#define ID_Dr 2003
#define CAM 2004
#define FROMFILE 2005

class CMainWin : public CFrameWnd
{
public:
	CMainWin()
	{
		Create(NULL, L"Lab07");
		Index = 0;
		PView.RedimMatrix(3);
	}
	CMenu *menu;
	CRect WinRect;
	CPyramid PIR;
	CMatrix PView;
	int Index;
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Dr1();
	void Dr();
	void Cam();
	void FromFile();
	DECLARE_MESSAGE_MAP();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
class CMyApp : public CWinApp
{
public:
	CMyApp() {};
	virtual BOOL InitInstance()
	{
		m_pMainWnd = new CMainWin();
		m_pMainWnd->ShowWindow(SW_SHOW);
		return TRUE;
	}
};
BEGIN_MESSAGE_MAP(CMainWin, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_Dr1, Dr1)
	ON_COMMAND(ID_Dr, Dr)
	ON_COMMAND(CAM, Cam)
	ON_COMMAND(FROMFILE, FromFile)
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

afx_msg void CMainWin::OnPaint()
{
	CPaintDC dc(this);
	WinRect = CRect(200, 100, 900, 800);
	if (Index == 1) PIR.Draw(dc, PView, WinRect);
	if (Index == 2) PIR.Draw1(dc, PView, WinRect);

}

afx_msg int CMainWin::OnCreate(LPCREATESTRUCT)
{
	menu = new CMenu();
	menu->CreateMenu();
	CMenu* submenu1 = new CMenu();
	submenu1->CreatePopupMenu();
	submenu1->AppendMenu(MF_STRING, ID_Dr, _T("Только видимые"));
	submenu1->AppendMenu(MF_STRING, ID_Dr1, _T("Все грани"));
	submenu1->AppendMenu(MF_STRING, CAM, _T("Положение камеры по умолчанию"));
	submenu1->AppendMenu(MF_STRING, FROMFILE, _T("Из файла"));
	TrackPopupMenu((HMENU)submenu1, TPM_LEFTALIGN | TPM_RIGHTALIGN, 0, 0, NULL, m_hWnd, NULL);
	menu->AppendMenu(MF_POPUP | MF_STRING, (UINT)submenu1->m_hMenu, _T("Пирамида"));
	MoveWindow(GetSystemMetrics(SM_CXMAXTRACK) / 2 - 500, GetSystemMetrics(SM_CYMAXTRACK) / 2 - 550, 1000, 1000);
	SetMenu(menu);
	return 0;
}

void CMainWin::Dr1()
{

	PView(0) = 10;
	PView(1) = 315;
	PView(2) = 45;
	Index = 1;
	Invalidate();
}

void CMainWin::Dr()
{

	PView(0) = 10;
	PView(1) = 315;
	PView(2) = 45;
	Index = 2;
	Invalidate();
}

void CMainWin::Cam()
{

	PView(0) = 10;
	PView(1) = 315;
	PView(2) = 45;
	Invalidate();
}

void CMainWin::FromFile() 
{
	std::ifstream fin("coords.txt");
	char* str = new char[80];
	fin >> str;
	PView(1) = atoi(str);
	fin >> str;
	PView(2) = atoi(str);
	Invalidate();
}

afx_msg void CMainWin::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (Index == 1 || Index == 2)
	{
		switch (nChar)
		{
			case VK_UP:
			{
				double d = PView(2) - 1;
				if (d >= 0)
				{
					PView(2) = d;
				} 
				break;
			}
			case VK_DOWN:
			{
				double d = PView(2) + 1;
				if (d <= 180)
				{
					PView(2) = d;
				}
				break;
			}
			case VK_LEFT:		// вращение вокруг оси z
			{
				double d = PView(1) - 1;
				if (d >= 0)
				{
					PView(1) = d;
				}
				else
				{
					PView(1) = 360 + d;
				}
				break;
			}
			case VK_RIGHT:		// вращение вокруг оси z
			{
				double d = PView(1) + 1;
				if (d <= 360)
				{
					PView(1) = d;
				}
				else
				{
					PView(1) = d - 360;
				}
				break;
			}
		}
		Invalidate();
	}
}

afx_msg void CMainWin::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
    WinRect.SetRect(cx * 0.5, -cy * 0.1, cx, cy);
}

CMyApp theApp;

void CMainWin::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 1000;
	lpMMI->ptMaxTrackSize.x = 1000;
	lpMMI->ptMinTrackSize.y = 1000;
	lpMMI->ptMaxTrackSize.y = 1000;
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}
