
// MFC_GearDlg.cpp : ʵ���ļ�
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


// CMFC_GearDlg �Ի���




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


// CMFC_GearDlg ��Ϣ�������

BOOL CMFC_GearDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_GearDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_GearDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_GearDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(_T("δ�������������������ӣ��������������ӣ�"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MyCameraInit();
}


void CMFC_GearDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
