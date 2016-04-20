
// MFC_GearDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ProcessThread.h"

// CMFC_GearDlg �Ի���
class CMFC_GearDlg : public CDialogEx
{
// ����
public:
	CMFC_GearDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_GEAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
		

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
