// ShowGear.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Gear.h"
#include "ShowGear.h"
#include "afxdialogex.h"
#include "CvvImage.h"


// CShowGear �Ի���

IMPLEMENT_DYNAMIC(CShowGear, CDialogEx)

CShowGear::CShowGear(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowGear::IDD, pParent)
	, m_imgFullPath(_T(""))
{

}

CShowGear::~CShowGear()
{
}

void CShowGear::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowGear, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CShowGear ��Ϣ�������


BOOL CShowGear::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd* pWnd=GetDlgItem(IDC_IMAGESHOW);
	pWnd->SetWindowPos(NULL,0,0,640,480,SWP_NOMOVE );
	
	ShowImage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CShowGear::ShowImage(void)
{
	CDC* pDC = GetDlgItem(IDC_IMAGESHOW)->GetDC();     // �����ʾ�ؼ��� DC
	HDC hDC = pDC ->GetSafeHdc();                // ��ȡ HDC(�豸���) �����л�ͼ����
	CRect rect;
	GetDlgItem(IDC_IMAGESHOW)->GetClientRect( &rect );

	CvvImage cimg;
	int len = WideCharToMultiByte(CP_ACP,0,m_imgFullPath,-1,NULL,0,NULL,NULL);
	char *ptxtTemp = new char[len + 1];
	WideCharToMultiByte(CP_ACP,0,m_imgFullPath,-1,ptxtTemp,len,NULL,NULL);
	cimg.Load(ptxtTemp,1);
	cimg.DrawToHDC(hDC,&rect);
	ReleaseDC( pDC );
}


void CShowGear::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
		CDialog::OnPaint();                    // �ػ�Ի���
		CDialog::UpdateWindow();                // ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
		ShowImage();    // �ػ�ͼƬ����
	
}

