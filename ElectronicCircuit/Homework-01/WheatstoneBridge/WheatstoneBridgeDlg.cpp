
// WheatstoneBridgeDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "WheatstoneBridge.h"
#include "WheatstoneBridgeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWheatstoneBridgeDlg 對話方塊

#define MIN_SCROLL_POS 1
#define MAX_SCROLL_POS 2000


CWheatstoneBridgeDlg::CWheatstoneBridgeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWheatstoneBridgeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWheatstoneBridgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_R1_Bar);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_R2_Bar);
	DDX_Control(pDX, IDC_SCROLLBAR3, m_R3_Bar);
	DDX_Control(pDX, IDC_SCROLLBAR4, m_R4_Bar);
	DDX_Control(pDX, IDC_SCROLLBAR5, m_Vin_Bar);
	DDX_Control(pDX, IDC_EDIT1, m_I1_Value);
	DDX_Control(pDX, IDC_EDIT2, m_I2_Value);
	DDX_Control(pDX, IDC_EDIT3, m_I3_Value);
	DDX_Control(pDX, IDC_EDIT4, m_I4_Value);
	DDX_Control(pDX, IDC_EDIT5, m_V1_Value);
	DDX_Control(pDX, IDC_EDIT6, m_V2_Value);
	DDX_Control(pDX, IDC_EDIT7, m_V3_Value);
	DDX_Control(pDX, IDC_EDIT8, m_V4_Value);
	DDX_Control(pDX, IDC_R1_VALUE, m_R1_Value);
	DDX_Control(pDX, IDC_R2_VALUE, m_R2_Value);
	DDX_Control(pDX, IDC_R3_VALUE, m_R3_Value);
	DDX_Control(pDX, IDC_R4_VALUE, m_R4_Value);
	DDX_Control(pDX, IDC_VIN_VALUE, m_Vin_Value);
	DDX_Control(pDX, IDC_EDIT9, m_Vo_Value);
}

BEGIN_MESSAGE_MAP(CWheatstoneBridgeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CALCULATE, &CWheatstoneBridgeDlg::OnBnClickedCalculate)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CWheatstoneBridgeDlg 訊息處理常式

BOOL CWheatstoneBridgeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	m_R1_Bar.SetScrollRange(MIN_SCROLL_POS, MAX_SCROLL_POS);
	m_R2_Bar.SetScrollRange(MIN_SCROLL_POS, MAX_SCROLL_POS);
	m_R3_Bar.SetScrollRange(MIN_SCROLL_POS, MAX_SCROLL_POS);
	m_R4_Bar.SetScrollRange(MIN_SCROLL_POS, MAX_SCROLL_POS);
	m_Vin_Bar.SetScrollRange(MIN_SCROLL_POS, MAX_SCROLL_POS);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CWheatstoneBridgeDlg::OnPaint()
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
HCURSOR CWheatstoneBridgeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWheatstoneBridgeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	int TempPos = pScrollBar->GetScrollPos();

	switch(nSBCode)
	{
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		pScrollBar->SetScrollPos(nPos);
		break;
	case SB_LINELEFT:
		if (TempPos > MIN_SCROLL_POS)
		{
			TempPos--;
		}
		pScrollBar->SetScrollPos(TempPos);
		break;

	case SB_LINERIGHT:
		if (TempPos < MAX_SCROLL_POS)
		{
			TempPos++;
		}
		pScrollBar->SetScrollPos(TempPos);
		break;
	}

	CString szR1_Value, szR2_Value, szR3_Value, szR4_Value, szVin_Value;
	szR1_Value.Format(L"%d", m_R1_Bar.GetScrollPos());
	szR2_Value.Format(L"%d", m_R2_Bar.GetScrollPos());
	szR3_Value.Format(L"%d", m_R3_Bar.GetScrollPos());
	szR4_Value.Format(L"%d", m_R4_Bar.GetScrollPos());
	szVin_Value.Format(L"%d", m_Vin_Bar.GetScrollPos());
	m_R1_Value.SetWindowText(szR1_Value);
	m_R2_Value.SetWindowText(szR2_Value);
	m_R3_Value.SetWindowText(szR3_Value);
	m_R4_Value.SetWindowText(szR4_Value);
	m_Vin_Value.SetWindowText(szVin_Value);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CWheatstoneBridgeDlg::OnBnClickedCalculate()
{
	// 在此加入控制項告知處理常式程式碼
	int R1, R2, R3, R4, Vin;
	double I1, I2, I3, I4, V1, V2, V3, V4, Vo;
	CString szI1, szI2, szI3, szI4, szV1, szV2, szV3, szV4, szVo;

	R1 = m_R1_Bar.GetScrollPos();
	R2 = m_R2_Bar.GetScrollPos();
	R3 = m_R3_Bar.GetScrollPos();
	R4 = m_R4_Bar.GetScrollPos();
	Vin = m_Vin_Bar.GetScrollPos();

	Calculate(R1, R2, R3, R4, Vin, V1, V2, V3, V4, Vo, I1, I2, I3, I4);
	szI1.Format(L"%lf", I1);
	szI2.Format(L"%lf", I2);
	szI3.Format(L"%lf", I3);
	szI4.Format(L"%lf", I4);
	szV1.Format(L"%lf", V1);
	szV2.Format(L"%lf", V2);
	szV3.Format(L"%lf", V3);
	szV4.Format(L"%lf", V4);
	szVo.Format(L"%lf", Vo);

	m_I1_Value.SetWindowText(szI1);
	m_I2_Value.SetWindowText(szI2);
	m_I3_Value.SetWindowText(szI3);
	m_I4_Value.SetWindowText(szI4);
	m_V1_Value.SetWindowText(szV1);
	m_V2_Value.SetWindowText(szV2);
	m_V3_Value.SetWindowText(szV3);
	m_V4_Value.SetWindowText(szV4);
	m_Vo_Value.SetWindowText(szVo);
}