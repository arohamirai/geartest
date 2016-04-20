// ShowGear.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Gear.h"
#include "ShowGear.h"
#include "afxdialogex.h"
#include "CvvImage.h"


// CShowGear 对话框

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


// CShowGear 消息处理程序


BOOL CShowGear::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CWnd* pWnd=GetDlgItem(IDC_IMAGESHOW);
	pWnd->SetWindowPos(NULL,0,0,640,480,SWP_NOMOVE );
	
	ShowImage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CShowGear::ShowImage(void)
{
	CDC* pDC = GetDlgItem(IDC_IMAGESHOW)->GetDC();     // 获得显示控件的 DC
	HDC hDC = pDC ->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作
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
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
		CDialog::OnPaint();                    // 重绘对话框
		CDialog::UpdateWindow();                // 更新windows窗口，如果无这步调用，图片显示还会出现问题
		ShowImage();    // 重绘图片函数
	
}

