﻿
// ScreenshotDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Screenshot.h"
#include "ScreenshotDlg.h"
#include "afxdialogex.h"
#include <atlimage.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CScreenshotDlg 对话框



CScreenshotDlg::CScreenshotDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCREENSHOT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenshotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CScreenshotDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CScreenshotDlg 消息处理程序

BOOL CScreenshotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CScreenshotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScreenshotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CScreenshotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScreenshotDlg::OnBnClickedButton1()
{
	//获取桌面窗口句柄
	HWND hDesktopWnd = ::GetDesktopWindow();
	//获取桌面窗口DC
	HDC hdc = ::GetDC(hDesktopWnd);
	//创建兼容DC
	HDC mdc = CreateCompatibleDC(hdc);

	//获取屏幕宽高
	DWORD dwScrenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	DWORD dwScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	//创建兼容位图
	HBITMAP bmp = CreateCompatibleBitmap(hdc, dwScrenWidth, dwScreenHeight);

	//选中位图
	HBITMAP holdBmp = (HBITMAP)::SelectObject(mdc, bmp);
	::BitBlt(mdc, 0, 0, dwScrenWidth, dwScreenHeight, hdc, 0, 0, SRCCOPY);


#pragma region 绘制鼠标
	CURSORINFO cursorInfo{};
	ICONINFO iconInfo{};
	HBITMAP bmpOldMask = NULL;
	HDC cdc = NULL;

	cdc = ::CreateCompatibleDC(mdc);

	cursorInfo.cbSize = sizeof(cursorInfo);
	GetCursorInfo(&cursorInfo);
	GetIconInfo(cursorInfo.hCursor, &iconInfo);
	bmpOldMask = (HBITMAP)::SelectObject(cdc, iconInfo.hbmMask);
	::BitBlt(mdc, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y, 50, 50, cdc, 0, 0, SRCAND);
	::SelectObject(cdc, iconInfo.hbmColor);
	::BitBlt(mdc, cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y, 50, 50, cdc, 0, 0, SRCPAINT);

	::SelectObject(cdc, bmpOldMask);
	::DeleteObject(iconInfo.hbmColor);
	::DeleteObject(iconInfo.hbmMask);
	::DeleteDC(cdc);
#pragma endregion


	CImage image;
	image.Attach(bmp);

	//保存
	image.Save(L"D:\\a.jpg");

	ShellExecute(NULL, L"open", L"D:\\a.jpg", NULL, NULL, SW_SHOWNORMAL);

	//::SetWindowPos(GetSafeHwnd(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	::ReleaseDC(hDesktopWnd,hdc);
	DeleteDC(mdc);
	DeleteObject(holdBmp);
	DeleteObject(bmp);
}
