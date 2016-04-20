
// MFC_GearDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "ProcessThread.h"

// CMFC_GearDlg 对话框
class CMFC_GearDlg : public CDialogEx
{
// 构造
public:
	CMFC_GearDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_GEAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
		

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboModelSel;
	afx_msg void OnBnClickedButton1();

public:
	CProcessThread *m_pProcessThread;
	void MyCameraInit(void);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSelchangeCombo1();
};
