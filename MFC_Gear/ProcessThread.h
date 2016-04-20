
#include "HalconCpp.h"
#include "hshapemodel.h"
#include "htuple.h"
#include "cv.h"
#include<vector>
using namespace HalconCpp;
#pragma once

// CProcessThread

class CProcessThread : public CWinThread
{
	DECLARE_DYNCREATE(CProcessThread)

protected:
	           // 动态创建所使用的受保护的构造函数
	virtual ~CProcessThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CProcessThread();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();

public:
	HWND pShowWnd;
	HShapeModel ModelID;
	
	void action(void);
	void xld2vector(cv::Mat &mat, HObject &xld, cv::Scalar color);
	void ReadModel(CString modelName);
	void Display(BOOL display = true);
	BOOL m_IsDisplay;
protected:
	HShapeModel ModelID1;
public:
	HObject m_ModelContours;
	HObject m_ModelContours1;
	HTuple m_DistanceTransformID;
	HTuple m_DistanceTransformID1;
	unsigned char m_inBuf[640*480];
private:
	int r;
	int c;
	int r1;
	int c1;
	int m_min;
	int m_max;
};


