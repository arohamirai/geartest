#pragma once


// CShowGear �Ի���

class CShowGear : public CDialogEx
{
	DECLARE_DYNAMIC(CShowGear)

public:
	CShowGear(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowGear();

// �Ի�������
	enum { IDD = IDD_SHOWGEAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_imgFullPath;
	void ShowImage(void);
	afx_msg void OnPaint();
};
