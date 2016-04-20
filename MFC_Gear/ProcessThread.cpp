// ProcessThread.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Gear.h"
#include "ProcessThread.h"
#include "JHCap.h"
#include "MFC_GearDlg.h"
#include "resource.h"
#include "opencv2/opencv.hpp"
 using namespace cv;
 using namespace HalconCpp;

using namespace cv;
using namespace HalconCpp;

extern CEvent g_event;
// CProcessThread

IMPLEMENT_DYNCREATE(CProcessThread, CWinThread)

void HObjectToMat(HObject *object, Mat *mat)
{
	HTuple hv_PointGray,hv_PointRed, hv_PointGreen, hv_PointBlue;
	HTuple hv_Type, hv_Width, hv_Height;
	HTuple hv_Request,hv_nChannels;
	int w, h;

	CountChannels(*object,&hv_nChannels);
	if (hv_nChannels.L() == 1)
	{
		GetImagePointer1(*object,&hv_PointGray,&hv_Type,&hv_Width,&hv_Height);
		w = *hv_Width.LArr();
		h = *hv_Height.LArr();

		memccpy(mat->data, (uchar*)hv_PointGray[0].L(), w * h, sizeof(uchar));
	}
}

CProcessThread::CProcessThread()
: r(0)
, c(0)
, m_IsDisplay(FALSE)
, r1(0)
, c1(0)
, m_min(3)
, m_max(10)
{
}

CProcessThread::~CProcessThread()
{
	ClearShapeModel(ModelID);
	ClearShapeModel(ModelID1);
}

BOOL CProcessThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	CWnd *hwnd=(CMFC_GearDlg* )AfxGetApp()->GetMainWnd();
	pShowWnd= hwnd->GetDlgItem(IDC_IMAGE)->GetSafeHwnd();
	
	//ModelID.ReadShapeModel("Matching 01.shm");
	//ReadModel();

	return TRUE;
}

int CProcessThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProcessThread, CWinThread)
END_MESSAGE_MAP()


// CProcessThread 消息处理程序


int CProcessThread::Run()
{
	// TODO: 在此添加专用代码和/或调用基类
	while (1)
	{
		if (m_IsDisplay)
		{
			int width=0, height=0, len=0;
			CameraGetImageSize(0,&width, &height);
			CameraGetImageBufferSize(0,&len, CAMERA_IMAGE_GRAY8);
			if(CameraQueryImage(0,m_inBuf, &len, CAMERA_IMAGE_GRAY8)==API_OK)
			{
				static int imgIndex = 0;
				char imgName[8] = {0};
				sprintf(imgName,"%d.bmp",imgIndex);
				Mat img(width,height,CV_8UC1,m_inBuf);
				imwrite(imgName,img);
				if (imgIndex > 100)
					Display(FALSE);
				//action();
			}
			else
				Sleep(1000);
		}
		/*else
			::WaitForSingleObject(g_event,INFINITE);*/
	}
	return 1;
}





void CProcessThread::xld2vector(Mat &mat, HObject &xld, Scalar color)
{
	vector<vector<Point>> contours;
	HObject ho_xld;
	HTuple hv_Number,hv_Row,hv_Col;
	CountObj(xld, &hv_Number);
	int numObj = hv_Number.L();
	if (numObj > 0)
	{
		for (int i=1; i<=numObj; i++)
		{
			vector<Point> tempVec;
			SelectObj(xld, &ho_xld, i);
			GetContourXld(ho_xld, &hv_Row, &hv_Col);

			int numPoint =  hv_Col.Length();
			for (int j = 0; j < numPoint; j++)
			{
				tempVec.push_back(Point(hv_Col.DArr()[j],hv_Row.DArr()[j]));
			}
			contours.push_back(tempVec);
		}
		for (int i = 0; i < numObj; i ++)
		{
			drawContours(mat,contours,i-1,color);
		}
	}else
		return;
	
}



void CProcessThread::ReadModel(CString modelName)
{
	CString strName[2] = {modelName+ _T(".shm"),modelName  + "1"+ _T(".shm") };
	char **ptxtTemp = new char *[2];
	for (int i = 0; i < 2; i++)
	{
		//MessageBox(NULL,strName[i],NULL,NULL);
		int len = WideCharToMultiByte(CP_ACP,0,strName[i],-1,NULL,0,NULL,NULL);
		ptxtTemp[i] = new char[len + 1];
		WideCharToMultiByte(CP_ACP,0,strName[i],-1,ptxtTemp[i],len,NULL,NULL);
	}
	try
	{
		printf("%s__%s",ptxtTemp[0],ptxtTemp[1]);
		ModelID.ReadShapeModel(ptxtTemp[0]);
		ModelID1.ReadShapeModel(ptxtTemp[1]);
	}
	catch(HException &except)
	{
		if (except.ErrorCode() == H_ERR_FNF)
		{
			MessageBox(NULL,_T("在.../res/model 目录下未发现模板文件，请检查并重试!"),NULL,NULL);
			return;

		}
		//else
			//MessageBox(NULL,_T("成功"),NULL,NULL);
	}
	for (int i = 0; i < 2; i++)
	{
		delete[] ptxtTemp[i];
	}
	delete ptxtTemp;
	GetShapeModelContours(&m_ModelContours, ModelID, 1);
	GetShapeModelContours(&m_ModelContours1, ModelID1, 1);
	HTuple hv_DistanceTransformID,hv_DistanceTransformID1;
	CreateDistanceTransformXld(m_ModelContours, "point_to_segment", m_max, &m_DistanceTransformID);
	CreateDistanceTransformXld(m_ModelContours1, "point_to_segment", m_max, &m_DistanceTransformID1);
	/*CreateDistanceTransformXld(m_ModelContours, "point_to_segment", 10, &hv_DistanceTransformID);
	CreateDistanceTransformXld(m_ModelContours1, "point_to_segment", 10, &hv_DistanceTransformID1);*/
	//m_DistanceTransformID = hv_DistanceTransformID.Clone();
	//m_DistanceTransformID1 = hv_DistanceTransformID1.Clone();
}


void CProcessThread::Display(BOOL display)
{
	m_IsDisplay = display;
}

void CProcessThread::action()
{
  // Local iconic variables
  HObject  ho_GrayImage, ho_ImageSmooth;
  HObject  ho_Edges, ho_TransContours, ho_SelectedXLD, ho__TransContours;
  HObject  ho_ContourOut, ho_ContourPart, ho__ContourPart;
  HObject  ho_TransContours1, ho_SelectedXLD1, ho__TransContours1;
  HObject  ho_ContourOut1, ho_ContourPart1, ho__ContourPart1;
  HObject  ho_UnionEdges;
  // Local control variables
  HTuple  hv_ModelRow, hv_ModelColumn;
  HTuple  hv_ModelAngle, hv_ModelScale, hv_ModelScore;
  HTuple  hv_HomMat, hv_HomMat2DInvert, hv_ModelRow1, hv_ModelColumn1;
  HTuple  hv_ModelAngle1, hv_ModelScale1, hv_ModelScore1;
  HTuple  hv_HomMat1, hv_HomMat2DInvert1;
  HTuple  hv_Edges,hv_BigNum,hv_SmallNum;

  Mat srcimg(480,640,CV_8UC1,m_inBuf),colorimg;
  int length = 20;
  cvtColor(srcimg, colorimg, CV_GRAY2RGB );

  HImage src("byte",640,480,m_inBuf);
  SmoothImage(src, &ho_ImageSmooth, "gauss", 0.5);
  EdgesSubPix(ho_ImageSmooth, &ho_Edges, "canny", 1, 20, 40);
  UnionAdjacentContoursXld(ho_Edges,&ho_UnionEdges,10,1,"attr_keep");
  xld2vector(colorimg,ho_UnionEdges,CV_RGB(0,0,255));

  FindScaledShapeModel(ho_ImageSmooth, ModelID, HTuple(0).TupleRad(), HTuple(60).TupleRad(), 
      0.85, 1.15, 0.5, 1, 0.5, "least_squares", (HTuple(6).Append(1)), 0.75, &hv_ModelRow, 
      &hv_ModelColumn, &hv_ModelAngle, &hv_ModelScale, &hv_ModelScore);
  //Matching 01: transform the model contours into the detected positions
  if(hv_ModelScore.TupleLength() > 0 )
  {
	  r = hv_ModelRow[0].D();
	  c = hv_ModelColumn[0].D();

	  HomMat2dIdentity(&hv_HomMat);
	  HomMat2dScale(hv_HomMat, HTuple(hv_ModelScale[0]), HTuple(hv_ModelScale[0]), 
		  0, 0, &hv_HomMat);
	  HomMat2dRotate(hv_HomMat, HTuple(hv_ModelAngle[0]), 0, 0, &hv_HomMat);
	  HomMat2dTranslate(hv_HomMat, HTuple(hv_ModelRow[0]), HTuple(hv_ModelColumn[0]), 
		  &hv_HomMat);
	  //show match result
	  AffineTransContourXld(m_ModelContours, &ho_TransContours, hv_HomMat);
	  xld2vector(colorimg,ho_TransContours,CV_RGB(0,255,0));

	  //transate to model and distance transition
	  HomMat2dInvert(hv_HomMat, &hv_HomMat2DInvert);
	  AffineTransContourXld(ho_UnionEdges, &ho__TransContours, hv_HomMat2DInvert);
	  ApplyDistanceTransformXld(ho__TransContours, &ho_ContourOut, m_DistanceTransformID);
	  SegmentContourAttribXld(ho_ContourOut, &ho_ContourPart, "distance", "and", m_min, m_max-1);
	  //show dismatch parts
	   AffineTransContourXld(ho_ContourPart, &ho__ContourPart, hv_HomMat);
	  CountObj(ho__ContourPart,&hv_BigNum);
	  if ((int)(hv_BigNum.L()) > 0)
		  xld2vector(colorimg,ho__ContourPart,CV_RGB(255,0,0));
  }
  else
  {
	  r = 100;
	  c = 100;
  }

  FindScaledShapeModel(ho_ImageSmooth, ModelID1, HTuple(0).TupleRad(), HTuple(60).TupleRad(), 
      0.85, 1.15, 0.5, 1, 0.5, "least_squares", (HTuple(6).Append(1)), 0.75, &hv_ModelRow1, 
      &hv_ModelColumn1, &hv_ModelAngle1, &hv_ModelScale1, &hv_ModelScore1);
  //Matching 0: transform the model contours into the detected positions
 if(hv_ModelScore1.TupleLength()>0)
 {
	 r1 = hv_ModelRow1[0].D();
	 c1 = hv_ModelColumn1[0].D();

	 HomMat2dIdentity(&hv_HomMat1);
	 HomMat2dScale(hv_HomMat1, HTuple(hv_ModelScale1[0]), HTuple(hv_ModelScale1[0]), 
		 0, 0, &hv_HomMat1);
	 HomMat2dRotate(hv_HomMat1, HTuple(hv_ModelAngle1[0]), 0, 0, 
		 &hv_HomMat1);
	 HomMat2dTranslate(hv_HomMat1, HTuple(hv_ModelRow1[0]), HTuple(hv_ModelColumn1[0]), 
		 &hv_HomMat1);
	 //show match result
	 AffineTransContourXld(m_ModelContours1, &ho_TransContours1, hv_HomMat1);
	  xld2vector(colorimg,ho_TransContours1,CV_RGB(0,255,0));
	 //transate to model
	 HomMat2dInvert(hv_HomMat1, &hv_HomMat2DInvert1);
	 AffineTransContourXld(ho_UnionEdges, &ho__TransContours1, hv_HomMat2DInvert1);
	 //distance transition
	 ApplyDistanceTransformXld(ho__TransContours1, &ho_ContourOut1, m_DistanceTransformID1);
	 SegmentContourAttribXld(ho_ContourOut1, &ho_ContourPart1, "distance", "and", m_min,m_max-1);
	 //show dismatch parts
	 AffineTransContourXld(ho_ContourPart1, &ho__ContourPart1, hv_HomMat1);
	 CountObj(ho__ContourPart1,&hv_SmallNum);
	 if ((int)(hv_SmallNum.L()) > 0)
		 xld2vector(colorimg,ho__ContourPart1,CV_RGB(255,0,0));
 }
 else
 {
	 r1 = 100;
	 c1 = 100;
 }

 // show ceter of the match parts
 line(colorimg,Point(c- length,r),Point(c + length, r),Scalar(0,0,255),2);
 line(colorimg,Point(c , r- length),Point(c, r + length),Scalar(0,0,255),2);
 circle(colorimg,Point(c1,r1),10,Scalar(0,255,0),3);
 CameraShowBufferImage(pShowWnd,colorimg.data, 640, 480, true, true);
}