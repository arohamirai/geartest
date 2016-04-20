
// MFC_GearDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Gear.h"
#include "MFC_GearDlg.h"
#include "afxdialogex.h"
#include "JHCap.h"
#include "ShowGear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_GearDlg 对话框




CMFC_GearDlg::CMFC_GearDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_GearDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_GearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboModelSel);
}

BEGIN_MESSAGE_MAP(CMFC_GearDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFC_GearDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC_GearDlg::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMFC_GearDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CMFC_GearDlg 消息处理程序

BOOL CMFC_GearDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CWnd* pWnd=GetDlgItem(IDC_IMAGE);
	pWnd->SetWindowPos(NULL,0,0,640,480,SWP_NOMOVE );

	CFileFind fileFind;
	CString strDir = _T("res/image/*.bmp");
	BOOL res = fileFind.FindFile(strDir);
	while(res)
	{
		res = fileFind.FindNextFile();
		if (!fileFind.IsDirectory() && !fileFind.IsDots())
		{
			CString fileName = fileFind.GetFileTitle();
			m_ComboModelSel.AddString(fileName);
		}
	}
	m_ComboModelSel.SetCurSel(-1);
	fileFind.Close();

	MyCameraInit();
	OnBnClickedButton1();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_GearDlg::OnPaint()
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
HCURSOR CMFC_GearDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_GearDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pProcessThread = new CProcessThread();
	m_pProcessThread->CreateThread(CREATE_SUSPENDED);
	m_pProcessThread->SetThreadPriority(THREAD_PRIORITY_NORMAL);
	//OnCbnSelchangeCombo1();
	m_pProcessThread->Display(FALSE);
	m_pProcessThread->ResumeThread();
	
}


void CMFC_GearDlg::MyCameraInit(void)
{
	int CamAllNum=0;
	CameraGetCount(&CamAllNum);
	if(CamAllNum == 0) 
		MessageBox(_T("未发现相机，请检查相机连接，并尝试重新连接！"));
	else
	{
		for(int i=0; i<CamAllNum; i++)
		{
			CameraInit(i);
		}
		int width1=752,height1=480;
		API_STATUS flag;
		for (int i=0; i<CamAllNum; i++)
		{
			flag = CameraSetMirrorX(i,true);
			flag = CameraSetMirrorY(i,true);
			flag = CameraSetAGC(i,false);
			flag = CameraSetAEC(i,false);
			flag = CameraSetResolution(i,0,&width1, &height1);	    
			
			flag = CameraSetExposure(i,120);	
			flag = CameraSetGain(i,0);
			flag = CameraSetBlackLevel(i,70);
			flag = CameraSetROI(i,56,0,640,480);
		}	
	}
}


void CMFC_GearDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	MyCameraInit();
}


void CMFC_GearDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pProcessThread->Display(FALSE);
	int index = m_ComboModelSel.GetCurSel();
	if(index == -1) return;
	CString modelName;
	m_ComboModelSel.GetLBText(index,modelName);

	/*int len = WideCharToMultiByte(CP_ACP,0,modelName,-1,NULL,0,NULL,NULL);
	char *ptxtTemp = new char[len + 1];
	WideCharToMultiByte(CP_ACP,0,modelName,-1,ptxtTemp,len,NULL,NULL);*/

	CString imgFullPath = _T("res/image/") + modelName +_T(".bmp");
	CShowGear dlg;
	dlg.m_imgFullPath = imgFullPath;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		CString modelFullPath;
		modelFullPath = _T("res/model/") + modelName;
		//modelFullPath += modelName;
		//modelFullPath += ".shm";
		m_pProcessThread->ReadModel(modelFullPath);
		m_pProcessThread->Display(TRUE);
	}
}
