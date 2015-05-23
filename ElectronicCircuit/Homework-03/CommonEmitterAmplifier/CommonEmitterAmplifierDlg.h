
// CommonEmitterAmplifierDlg.h : 標頭檔
//

#pragma once
#include "afxwin.h"


// CCommonEmitterAmplifierDlg 對話方塊
class CCommonEmitterAmplifierDlg : public CDialog
{
// 建構
public:
	CCommonEmitterAmplifierDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_COMMONEMITTERAMPLIFIER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	CScrollBar m_AmplitudeBar;
	CScrollBar m_FrequencyBar;
	CScrollBar m_PhaseBar;
	CScrollBar m_R1Bar;
	CScrollBar m_R2Bar;
	CScrollBar m_RcBar;
	CScrollBar m_ReBar;
	CScrollBar m_BetaBar;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CStatic m_AmplitudeValue;
	CStatic m_FrequencyValue;
	CStatic m_PhaseValue;
	CStatic m_R1Value;
	CStatic m_R2Value;
	CStatic m_RcValue;
	CStatic m_ReValue;
	CStatic m_BetaValue;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_Vi_Min;
	CStatic m_Vi_Max;
	CStatic m_Vo_Max;
	CStatic m_Vo_Min;
	CStatic m_CurrentTime;
};
