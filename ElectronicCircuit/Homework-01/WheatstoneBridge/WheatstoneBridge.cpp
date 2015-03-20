
// WheatstoneBridge.cpp : 定義應用程式的類別行為。
//

#include "stdafx.h"
#include "WheatstoneBridge.h"
#include "WheatstoneBridgeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWheatstoneBridgeApp

BEGIN_MESSAGE_MAP(CWheatstoneBridgeApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWheatstoneBridgeApp 建構

CWheatstoneBridgeApp::CWheatstoneBridgeApp()
{
	// 將所有重要的初始設定加入 InitInstance 中
}


// 僅有的一個 CWheatstoneBridgeApp 物件

CWheatstoneBridgeApp theApp;


// CWheatstoneBridgeApp 初始設定
typedef VOID (CALLBACK* VALIDATION_FUNC)(int R1, int R2, int R3, int R4, int Vin,
										 double* V1, double* V2, double* V3, double* V4, double* Vo,
										 double* I1, double* I2, double* I3, double* I4);
VALIDATION_FUNC ValidationFunction;
double V1, V2, V3, V4, Vo, I1, I2, I3, I4;
double V1_, V2_, V3_, V4_, Vo_, I1_, I2_, I3_, I4_;
BOOL CWheatstoneBridgeApp::InitInstance()
{
	// 假如應用程式資訊清單指定使用 ComCtl32.dll 6 (含) 以後版本，
	// 來啟動視覺化樣式，在 Windows XP 上，則需要 InitCommonControls()。
	// 否則任何視窗的建立都將失敗。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 設定要包含所有您想要用於應用程式中的
	// 通用控制項類別。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 標準初始設定
	// 如果您不使用這些功能並且想減少
	// 最後完成的可執行檔大小，您可以
	// 從下列程式碼移除不需要的初始化常式，
	// 變更儲存設定值的登錄機碼
	// TODO: 您應該適度修改此字串
	// (例如，公司名稱或組織名稱)
	SetRegistryKey(_T("本機 AppWizard 所產生的應用程式"));

	HMODULE Validator = LoadLibrary(L"BridgeValidator.exp");

	if (Validator && (ValidationFunction = (VALIDATION_FUNC) GetProcAddress(Validator, "?Validation@@YAXHHHHHPAN00000000@Z")))
	{
		ValidationFunction(1800, 1200, 150, 100, 100, &V1, &V2, &V3, &V4, &Vo, &I1, &I2, &I3, &I4);
		Calculate(1800, 1200, 150, 100, 100, V1_, V2_, V3_, V4_, Vo_, I1_, I2_, I3_, I4_);

		if ((V1 != V1_) || (V2 != V2_) || (V3 != V3_) || (V4 != V4_) || (Vo != Vo_) || (I1 != I1_) || (I2 != I2_) || (I3 != I3_) || (I4 != I4_))
		{
			AfxMessageBox(L"Failed");
			exit(0);
		}
		else
		{
			AfxMessageBox(L"Pass");
			exit(0);
		}
	}

	CWheatstoneBridgeDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// 在此放置於使用 [確定] 來停止使用對話方塊時
		// 處理的程式碼
	}
	else if (nResponse == IDCANCEL)
	{
		// 在此放置於使用 [取消] 來停止使用對話方塊時
		// 處理的程式碼
	}

	// 因為已經關閉對話方塊，傳回 FALSE，所以我們會結束應用程式，
	// 而非提示開始應用程式的訊息。
	return FALSE;
}
