#pragma once


// CShowGear 对话框

class CShowGear : public CDialogEx
{
	DECLARE_DYNAMIC(CShowGear)

public:
	CShowGear(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowGear();

// 对话框数据
	enum { IDD = IDD_SHOWGEAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_imgFullPath;
	void ShowImage(void);
	afx_msg void OnPaint();
};
