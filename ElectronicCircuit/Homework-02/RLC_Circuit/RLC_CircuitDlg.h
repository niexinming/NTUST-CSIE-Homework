
// RLC_CircuitDlg.h : 標頭檔
//

#pragma once
#include "afxwin.h"


// CRLC_CircuitDlg 對話方塊
class CRLC_CircuitDlg : public CDialog
{
// 建構
public:
	CRLC_CircuitDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_RLC_CIRCUIT_DIALOG };

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
	CScrollBar m_R_Bar;
	CScrollBar m_L_Bar;
	CScrollBar m_C_Bar;
	CScrollBar m_f_Bar;
	CEdit m_I_Value;
	CEdit m_Vo_Value;
	CEdit m_Z_Value;
	CStatic m_R_Value;
	CStatic m_L_Value;
	CStatic m_C_Value;
	CStatic m_f_Value;
	afx_msg void CalcAndDraw();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
