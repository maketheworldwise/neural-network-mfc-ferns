
// FernsNNView.h : CFernsNNView 클래스의 인터페이스
//

#pragma once
#include "FernsNNDoc.h"
#include "FernsNNView.h"


#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include "TCHAR.h"
#include "math.h" 

#define _USE_MATH_DEFINES

using namespace cv;

#define BYTE unsigned char
#define FERN_SIZE 70			// Fern 개수
#define FERN_BI_SIZE 4 			// Fern 크기
#define RAND_NUM FERN_SIZE*16	// 좌표 240쌍(480개 숫자)
#define PATCH_WIDTH 40			// Patch 크기 40 x 40
#define PATCH_SIZE 1			// Patch 생성 개수

class CFernsNNView : public CView
{
protected: // serialization에서만 만들어집니다.
	CFernsNNView();
	DECLARE_DYNCREATE(CFernsNNView)

// 특성입니다.
public:
	CFernsNNDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CFernsNNView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:

	IplImage* Image;		//Original1
	IplImage* Display;		//Original2
	IplImage* RotImg;		//Rotate image

	IplImage* eigImage;		//Harris Coner Buff
	IplImage* tempImage;	//Harris Coner Buff

	IplImage* img[PATCH_SIZE];

	int nVw;				//Image Height
	int nHw;				//Image Width

	int pick_count;
	int count;

	double iDegree;

	int histogram[PATCH_SIZE][16];
	int*** newHist;


	UINT x, y, z;
	UINT nCoord[RAND_NUM];

	int LearnData[FERN_SIZE];

	BYTE **grayImg, **pbRes;
	BYTE gray[PATCH_SIZE][RAND_NUM / 2], result_value[PATCH_SIZE][RAND_NUM / 4];


	/*파일 관련 변수*/
	FILE* fp;
	char file_name[250];
	char filecount;
	char name;

	/*Ferns 함수*/
	CvPoint2D32f* pick;		// Harris corner coordinate
	CvPoint2D32f* rotCoord;	// Rotated harris coner coordinate

	BYTE** CreateArray(UINT anY, UINT anX);
	int*** CreateArray(UINT anZ, UINT anY, UINT anX);
	void DestroyArray(BYTE** p, UINT anY, UINT anX);
	void DestroyArray(int*** p, UINT anZ, UINT anY, UINT anX);

	void CreateRandCoord(FILE* afFp, UINT* anCoord);
	void LoadCoord(FILE* afFp, UINT* anCoord);

	void StoreHistData(FILE* afFp, int iHistogram[][16]);
	void NewStoreHistData(FILE* afFp, int*** iHistogram);

	void mtxAB(double* a, double* b, int m, int p, int n, double* x);
	void ImageRot(BYTE** apbDst, BYTE** apbSrc, int anHw, int anVw, double aoDegree);
	void CoordRot(CvPoint2D32f* rotCoord, CvPoint2D32f* pick, int anHw, int anVw, double oDegree);

	void CreatePatch(IplImage** Dst, IplImage* Src, IplImage* Display, CvPoint2D32f* Coord);

	void IplImage2BYTE(BYTE** apbDst, IplImage* apiSrc, int anHw, int anVw);
	void BYTE2IplImage(IplImage* apiDst, BYTE** apbSrc, int anHw, int anVw);
	void ExtractPixValue(BYTE apbDst[][RAND_NUM / 2], IplImage** apiSrc, UINT* nCoord, int anHw, int anVw);
	void GetBiFeature(BYTE apbDst[][RAND_NUM / 4], BYTE apbSrc[][RAND_NUM / 2]);
	void CreateFern(BYTE apbDst[][RAND_NUM / 4], int  iHistogram[][16]);
	void NewCreateFern(BYTE apbDst[][RAND_NUM / 4], int*** iHistogram);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnImage1();
	afx_msg void OnImage2();
	afx_msg void OnLearn();
	afx_msg void OnRecall();

	void InputA(int filecount);
	void InputB(int filecount);
	void NormalizeInput();
};

#ifndef _DEBUG  // FernsNNView.cpp의 디버그 버전
inline CFernsNNDoc* CFernsNNView::GetDocument() const
   { return reinterpret_cast<CFernsNNDoc*>(m_pDocument); }
#endif

