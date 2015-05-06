
// RLC_CircuitDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "RLC_Circuit.h"
#include "RLC_CircuitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

// CRLC_CircuitDlg 對話方塊


CRLC_CircuitDlg::CRLC_CircuitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRLC_CircuitDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRLC_CircuitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_R_Bar);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_L_Bar);
	DDX_Control(pDX, IDC_SCROLLBAR3, m_C_Bar);
	DDX_Control(pDX, IDC_SCROLLBAR4, m_f_Bar);
	DDX_Control(pDX, IDC_EDIT1, m_I_Value);
	DDX_Control(pDX, IDC_EDIT2, m_Vo_Value);
	DDX_Control(pDX, IDC_EDIT3, m_Z_Value);
	DDX_Control(pDX, IDC_R_VALUE, m_R_Value);
	DDX_Control(pDX, IDC_L_VALUE, m_L_Value);
	DDX_Control(pDX, IDC_C_VALUE, m_C_Value);
	DDX_Control(pDX, IDC_F_VALUE, m_f_Value);
}

BEGIN_MESSAGE_MAP(CRLC_CircuitDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CRLC_CircuitDlg::CalcAndDraw)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CRLC_CircuitDlg 訊息處理常式

BOOL CRLC_CircuitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	m_R_Bar.SetScrollRange(100, 1000);
	m_L_Bar.SetScrollRange(10, 100);
	m_C_Bar.SetScrollRange(100, 1000);
	m_f_Bar.SetScrollRange(1, 100);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CRLC_CircuitDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CRLC_CircuitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRLC_CircuitDlg::CalcAndDraw()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	int R, L, C, f;
	double I, Vo, Z;
	CString szI, szVo, szZ;

	R = m_R_Bar.GetScrollPos();
	L = m_L_Bar.GetScrollPos();
	C = m_C_Bar.GetScrollPos();
	f = m_f_Bar.GetScrollPos();

	Calculate(R, L, C, f, I, Vo, Z);

	szI.Format(L"%.2lf", I);
	szVo.Format(L"%.2lf", Vo);
	szZ.Format(L"%.2lf", Z);

	m_I_Value.SetWindowText(szI);
	m_Vo_Value.SetWindowText(szVo);
	m_Z_Value.SetWindowText(szZ);

	Draw(m_hWnd, R, L, C);
}

void CRLC_CircuitDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	int TempPos = pScrollBar->GetScrollPos();

	switch(nSBCode)
	{
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		pScrollBar->SetScrollPos(nPos);
		break;
	case SB_LINELEFT:
		TempPos--;
		pScrollBar->SetScrollPos(TempPos);
		break;
	case SB_LINERIGHT:
		TempPos++;
		pScrollBar->SetScrollPos(TempPos);
		break;
	}

	CString szR_Value, szL_Value, szC_Value, szf_Value;
	szR_Value.Format(L"%d", m_R_Bar.GetScrollPos());
	szL_Value.Format(L"%d", m_L_Bar.GetScrollPos());
	szC_Value.Format(L"%d", m_C_Bar.GetScrollPos());
	szf_Value.Format(L"%d", m_f_Bar.GetScrollPos());
	m_R_Value.SetWindowText(szR_Value);
	m_L_Value.SetWindowText(szL_Value);
	m_C_Value.SetWindowText(szC_Value);
	m_f_Value.SetWindowText(szf_Value);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
