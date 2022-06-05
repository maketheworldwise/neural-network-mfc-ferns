
// FernsNNView.cpp : CFernsNNView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "FernsNN.h"
#endif

#include "FernsNNDoc.h"
#include "FernsNNView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFernsNNView

IMPLEMENT_DYNCREATE(CFernsNNView, CView)

BEGIN_MESSAGE_MAP(CFernsNNView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFernsNNView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_IMAGE1, &CFernsNNView::OnImage1)
	ON_COMMAND(ID_IMAGE2, &CFernsNNView::OnImage2)
	ON_COMMAND(ID_LEARN, &CFernsNNView::OnLearn)
	ON_COMMAND(ID_RECALL, &CFernsNNView::OnRecall)
END_MESSAGE_MAP()

// CFernsNNView 생성/소멸

CFernsNNView::CFernsNNView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	filecount = 1;
}

CFernsNNView::~CFernsNNView()
{
}

BOOL CFernsNNView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CFernsNNView 그리기

void CFernsNNView::OnDraw(CDC* pDC)
{
	CFernsNNDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	for (int y = 0; y < nVw; y++) {
		for (int x = 0; x < nHw; x++) {
			pDC->SetPixel(x, y, RGB(cvGet2D(Display, y, x).val[0], cvGet2D(Display, y, x).val[0], cvGet2D(Display, y, x).val[0]));
		}
	}
}


// CFernsNNView 인쇄


void CFernsNNView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CFernsNNView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CFernsNNView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CFernsNNView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CFernsNNView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CFernsNNView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CFernsNNView 진단

#ifdef _DEBUG
void CFernsNNView::AssertValid() const
{
	CView::AssertValid();
}

void CFernsNNView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFernsNNDoc* CFernsNNView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFernsNNDoc)));
	return (CFernsNNDoc*)m_pDocument;
}
#endif //_DEBUG


// CFernsNNView 메시지 처리기
/*추출한 부분의 변수 동적할당 및 메모리 비우기*/
BYTE** CFernsNNView::CreateArray(UINT anY, UINT anX)
{
	UINT y;
	BYTE **p;

	p = (BYTE**)malloc(sizeof(BYTE*) * anY);

	for (y = 0; y < anY; y++) {
		p[y] = (BYTE*)malloc(sizeof(BYTE) * anX);
	}

	return	p;
}


int*** CFernsNNView::CreateArray(UINT anZ, UINT anY, UINT anX)
{
	UINT y, z;
	int ***p;

	p = (int***)malloc(sizeof(int**) * anZ);

	for (z = 0; z < anZ; z++) {
		p[z] = (int**)malloc(sizeof(int*) * anY);
		for (y = 0; y < anY; y++) {
			p[z][y] = (int*)malloc(sizeof(int) * anX);
		}
	}

	return	p;
}


void CFernsNNView::DestroyArray(BYTE** p, UINT anY, UINT anX)
{
	UINT y;

	for (y = 0; y < anY; y++) {
		free(p[y]);
	}
	free(p);
}


void CFernsNNView::DestroyArray(int*** p, UINT anZ, UINT anY, UINT anX)
{
	UINT y, z;
	for (z = 0; z < anZ; z++) {
		for (y = 0; y < anY; y++) {
			free(p[z][y]);
		}
		free(p[z]);
	}
	free(p);
}


/*데이터 뽑아 text file로 저장*/
/*patch영역에서 70개의 점을 무작위로 찍어 저장*/
void CFernsNNView::CreateRandCoord(FILE* afFp, UINT* anCoord)
{
	afFp = fopen("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/RandomCoordDataFile/coordinate.txt", "w");
	for (int n = 0; n < RAND_NUM; n++) {
		anCoord[n] = rand() % PATCH_WIDTH;
		fprintf(afFp, "%d\n", anCoord[n]);
	}
	fclose(afFp);
}



/*무작위로 찍은 70개의 점의 위치를 사용하기 위해 로드를 하여 똑같은 위치의 점 선택*/
void CFernsNNView::LoadCoord(FILE* afFp, UINT *anCoord)
{
	afFp = fopen("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/RandomCoordDataFile/coordinate.txt", "r");
	for (int n = 0; n < RAND_NUM; n++) {
		anCoord[n] = rand() % PATCH_WIDTH;
		fprintf(afFp, "%d\n", anCoord[n]);
	}
	fclose(afFp);
}



/*Histogram 데이터 text file로 저장*/
void CFernsNNView::StoreHistData(FILE* afFp, int iHistogram[][16])
{
	afFp = fopen("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/2DHistFile/2DHistogram.txt", "w");

	for (int i = 0; i < PATCH_SIZE; i++) {
		for (int j = 0; j < pow(2.0, FERN_BI_SIZE); j++) {
			fprintf(afFp, "%d\n", iHistogram[i][j]);
			printf("histogram[%d][%d] : %d\n", i, j, iHistogram[i][j]);
		}
		//fprintf(afFp, "\n");
		printf("\n");
	}
	fclose(afFp);
}



/*3차원 데이터 저장(프로그램에선 사용 안함)*/
void CFernsNNView::NewStoreHistData(FILE* afFp, int*** iHistogram)
{
	afFp = fopen("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/3DHistFile/3DHistogram.txt", "w");
	int i, j, k;
	for (i = 0; i < PATCH_SIZE; i++) {
		for (j = 0; j < FERN_SIZE; j++) {
			for (k = 0; k < pow(2.0, FERN_BI_SIZE); k++) {
				fprintf(afFp, "%d\n", iHistogram[i][j][k]);
				printf("histogram[%d][%d][%d] : %d\n", i, j, k, iHistogram[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
	fclose(afFp);
}


/*이미지 회전(프로그램에서 사용안함)*/
void CFernsNNView::mtxAB(double* a, double* b, int m, int p, int n, double* x)
{
	int i, j, k;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			x[i*n + j] = 0.;
			for (k = 0; k < p; k++)
				x[i*n + j] += a[i*p + k] * b[k*n + j];
		}
	}
}


void CFernsNNView::ImageRot(BYTE** apbDst, BYTE** apbSrc, int anHw, int anVw, double aoDegree)
{
	double dTheta = aoDegree * 3.14 / 180.0;
	double  dRotMtx[4], dRotCoord[2], dCoord[2], dNw, dNe, dSe, dSw, dN, dS, result;
	int i, j;

	dRotMtx[0] = cos(dTheta);	dRotMtx[1] = sin(dTheta);
	dRotMtx[2] = -sin(dTheta);	dRotMtx[3] = cos(dTheta);

	for (j = 0; j < anVw; j++) {
		for (i = 0; i < anHw; i++) {
			dCoord[0] = i - anHw / 2;	dCoord[1] = j - anVw / 2;		// Change starting point to center

			mtxAB(dRotMtx, dCoord, 2, 2, 1, dRotCoord);					// Multiply rotate matrix X nCoord

			dRotCoord[0] += anHw / 2;	dRotCoord[1] += anVw / 2;		// Change starting point back to 0, 0

			if (!(dRotCoord[0] > 1 && dRotCoord[0] < anHw - 1) || !(dRotCoord[1] > 1 && dRotCoord[1] < anVw - 1))
				continue;

			// Bilinear interpolation
			dNw = apbSrc[(int)floor(dRotCoord[1])][(int)floor(dRotCoord[0])];
			dNe = apbSrc[(int)floor(dRotCoord[1])][(int)ceil(dRotCoord[0])];
			dSw = apbSrc[(int)ceil(dRotCoord[1])][(int)floor(dRotCoord[0])];
			dSe = apbSrc[(int)ceil(dRotCoord[1])][(int)ceil(dRotCoord[0])];

			dN = (dNw*((int)ceil(dRotCoord[0]) - dRotCoord[0])) + (dNe*(1 - (int)ceil(dRotCoord[0]) + dRotCoord[0]));
			dS = (dSw*((int)ceil(dRotCoord[0]) - dRotCoord[0])) + (dSe*(1 - (int)ceil(dRotCoord[0]) + dRotCoord[0]));

			result = (dN*((int)ceil(dRotCoord[1]) - dRotCoord[1])) + (dS*(1 - (int)ceil(dRotCoord[1]) + dRotCoord[1]));

			apbDst[j][i] = (BYTE)result;
		}
	}
}


/*추출한 데이터 회전(프로그램에서 사용안함)*/
void CFernsNNView::CoordRot(CvPoint2D32f* rotCoord, CvPoint2D32f* pick, int anHw, int anVw, double oDegree)
{
	double dTheta = -oDegree * 3.14 / 180.0;
	double  dRotMtx[4], dRotCoord[2], dCoord[2];
	int i;

	dRotMtx[0] = cos(dTheta);	dRotMtx[1] = sin(dTheta);
	dRotMtx[2] = -sin(dTheta);	dRotMtx[3] = cos(dTheta);

	for (i = 0; i < PATCH_SIZE; i++) {
		dCoord[0] = pick[i].x - anHw / 2;	dCoord[1] = pick[i].y - anVw / 2;	// Change starting point to center

		mtxAB(dRotMtx, dCoord, 2, 2, 1, dRotCoord);								// Multiply rotate matrix X nCoord

		rotCoord[i].x = dRotCoord[0] + anHw / 2;								// Change starting point back to 0, 0
		rotCoord[i].y = dRotCoord[1] + anVw / 2;
	}
}


/*추출할 데이터 영역 그리기*/
void CFernsNNView::CreatePatch(IplImage** Dst, IplImage* Src, IplImage* Display, CvPoint2D32f* Coord)
{
	int count;

	for (count = 0; count < PATCH_SIZE; count++) {
		cvSetImageROI(Src, cvRect(Coord[count].x - 20, Coord[count].y - 20, 40, 40));
		cvCopyImage(Src, Dst[count]);
		cvResetImageROI(Src);
	}
	for (count = 0; count < PATCH_SIZE; count++) {
		cvCircle(Display, cvPoint(Coord[count].x, Coord[count].y), 2, CV_RGB(0, 0, 0), -1, 8);
		cvRectangle(Display, cvPoint(Coord[count].x - 21, Coord[count].y - 21), cvPoint(Coord[count].x + 21, Coord[count].y + 21), CV_RGB(0, 0, 0));
	}
}


/*펀스 학습에 해당하는 부분이라 프로그램에 필요없음(IpIImage를 BYTE로 변환하는 부분)*/
void CFernsNNView::IplImage2BYTE(BYTE** apbDst, IplImage* apiSrc, int anHw, int anVw)
{
	int i, j;

	for (i = 0; i < anVw; i++) {
		for (j = 0; j < anHw; j++) {
			apbDst[i][j] = cvGet2D(apiSrc, i, j).val[0];
		}
	}
}


/*펀스 학습에 해당하는 부분이라 프로그램에 필요없음(BYTE를 IpIImage로 변환하는 부분)*/
void CFernsNNView::BYTE2IplImage(IplImage* apiDst, BYTE** apbSrc, int anHw, int anVw)
{
	int i, j;

	for (i = 0; i < anVw; i++) {
		for (j = 0; j < anHw; j++) {
			cvSet2D(apiDst, i, j, cvScalar(apbSrc[i][j]));
		}
	}
}


void CFernsNNView::ExtractPixValue(BYTE apbDst[][RAND_NUM / 2], IplImage** apiSrc, UINT* nCoord, int anHw, int anVw)
{
	int i, j;

	for (i = 0; i < PATCH_SIZE; i++) {
		for (j = 0; j < RAND_NUM / 2; j++) {
			apbDst[i][j] = cvGet2D(apiSrc[i], nCoord[j * 2 + 1], nCoord[j * 2]).val[0];
		}
	}
}


void CFernsNNView::GetBiFeature(BYTE apbDst[][RAND_NUM / 4], BYTE apbSrc[][RAND_NUM / 2])
{
	int i, j;

	for (i = 0; i < PATCH_SIZE; i++) {
		for (j = 0; j < RAND_NUM / 4; j++) {
			apbDst[i][j] = (apbSrc[i][j * 2] > apbSrc[i][j * 2 + 1]) ? 1 : 0;
		}
	}
}


void CFernsNNView::CreateFern(BYTE apbDst[][RAND_NUM / 4], int iHistogram[][16])
{
	int i, j, hist;

	for (i = 0; i < PATCH_SIZE; i++) {
		for (j = 0; j < RAND_NUM / 16; j++) {
			hist = (apbDst[i][j * 4] * 1) + (apbDst[i][j * 4 + 1] * 2) + (apbDst[i][j * 4 + 2] * 4) + (apbDst[i][j * 4 + 3] * 8);
			iHistogram[i][hist]++;
		}
	}
}


void CFernsNNView::NewCreateFern(BYTE apbDst[][RAND_NUM / 4], int*** iHistogram)
{
	int i, j, hist;
	count = 0;

	for (i = 0; i < PATCH_SIZE; i++) {
		for (j = 0; j < RAND_NUM / 16; j++) {
			hist = (apbDst[i][j * 4] * 1) + (apbDst[i][j * 4 + 1] * 2) + (apbDst[i][j * 4 + 2] * 4) + (apbDst[i][j * 4 + 3] * 8);
			iHistogram[i][j][hist]++;
			LearnData[count++] = hist;
		}
	}
}


void CFernsNNView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nChar)
	{
	case VK_UP:
		break;

	case 'Z':
		CreateRandCoord(fp, nCoord);
		break;

	case 'X':
		LoadCoord(fp, nCoord);
		break;
	}
	Invalidate(FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CFernsNNView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	rotCoord->x = point.x;
	rotCoord->y = point.y;

	CreatePatch(img, Image, Display, rotCoord);			//Draw Patch Area 

	ExtractPixValue(gray, img, nCoord, nHw, nVw);		// Extract pixel value from patch by random coordinate 

	GetBiFeature(result_value, gray);					// Get Binary feature from Extracted pixel value 
														// Construct Fern by Binary feature
	NewCreateFern(result_value, newHist);

	sprintf(file_name, "%s%c%d%s", "C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/Learntxt/", name, filecount++,".txt");
	fp = fopen(file_name, "w");
	for (int i = 0; i < FERN_SIZE; i++) {
		fprintf(fp, "%d\n", LearnData[i]);
	}
	fclose(fp);

	Invalidate(FALSE);

	CView::OnLButtonDown(nFlags, point);
}

void CFernsNNView::OnImage1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	filecount = 0;
	name = 'A';

	Image = cvLoadImage("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/image/하트.png", CV_LOAD_IMAGE_GRAYSCALE);		//original
	Display = cvLoadImage("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/image/하트.png", CV_LOAD_IMAGE_GRAYSCALE);		//original
	RotImg = cvLoadImage("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/image/하트.png", CV_LOAD_IMAGE_GRAYSCALE);		//Rot Image

																																						//계산
	eigImage = cvCreateImage(cvGetSize(Image), IPL_DEPTH_32F, 1);										// harris corner buff
	tempImage = cvCreateImage(cvGetSize(Image), IPL_DEPTH_32F, 1);										// harris corner buff

	nVw = Image->height, nHw = Image->width;															//Extract Image Height, Width

	pick_count = PATCH_SIZE;

	pick = new CvPoint2D32f[PATCH_SIZE];																// Harris corner coordinate
	rotCoord = new CvPoint2D32f[PATCH_SIZE];															// Rotated harris coner coordinate

	grayImg = CreateArray(nVw, nHw);																	//Dynamic Array
	pbRes = CreateArray(nVw, nHw);																		//Dynamic Array

	newHist = CreateArray(PATCH_SIZE, FERN_SIZE, pow(2.0, FERN_BI_SIZE));								//Dynamic Array2

	for (x = 0; x < PATCH_SIZE; x++) {
		for (y = 0; y < RAND_NUM / 4; y++)
			result_value[x][y] = 0;
		for (y = 0; y < pow(2.0, FERN_BI_SIZE); y++)
			histogram[x][y] = 0;
		for (y = 0; y < FERN_SIZE; y++)
			for (z = 0; z < pow(2.0, FERN_BI_SIZE); z++)
				newHist[x][y][z] = 0;
		img[x] = cvCreateImage(cvSize(40, 40), Image->depth, 1);
	}

	Invalidate(FALSE);
}


void CFernsNNView::OnImage2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	filecount = 0;
	name = 'B';	

	Image = cvLoadImage("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/image/호박.png", CV_LOAD_IMAGE_GRAYSCALE);		//original
	Display = cvLoadImage("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/image/호박.png", CV_LOAD_IMAGE_GRAYSCALE);		//original
	RotImg = cvLoadImage("C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/image/호박.png", CV_LOAD_IMAGE_GRAYSCALE);		//Rot Image

																																			//계산
	eigImage = cvCreateImage(cvGetSize(Image), IPL_DEPTH_32F, 1);										// harris corner buff
	tempImage = cvCreateImage(cvGetSize(Image), IPL_DEPTH_32F, 1);										// harris corner buff

	nVw = Image->height, nHw = Image->width;															//Extract Image Height, Width

	pick_count = PATCH_SIZE;

	pick = new CvPoint2D32f[PATCH_SIZE];																// Harris corner coordinate
	rotCoord = new CvPoint2D32f[PATCH_SIZE];															// Rotated harris coner coordinate

	grayImg = CreateArray(nVw, nHw);																	//Dynamic Array
	pbRes = CreateArray(nVw, nHw);																		//Dynamic Array

	newHist = CreateArray(PATCH_SIZE, FERN_SIZE, pow(2.0, FERN_BI_SIZE));								//Dynamic Array2

	for (x = 0; x < PATCH_SIZE; x++) {
		for (y = 0; y < RAND_NUM / 4; y++)
			result_value[x][y] = 0;
		for (y = 0; y < pow(2.0, FERN_BI_SIZE); y++)
			histogram[x][y] = 0;
		for (y = 0; y < FERN_SIZE; y++)
			for (z = 0; z < pow(2.0, FERN_BI_SIZE); z++)
				newHist[x][y][z] = 0;
		img[x] = cvCreateImage(cvSize(40, 40), Image->depth, 1);
	}

	Invalidate(FALSE);
}


void CFernsNNView::OnLearn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFernsNNDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	filecount = 50;
	int num = 0;

	do {

		for (x = 0; x < filecount; x++) {
			InputA(x);
			//NormalizeInput();
			pDoc->TargetOutput = 1;
			pDoc->Propagation(pDoc->InputPattern);
		}
		for (x = 0; x < filecount; x++) {
			InputB(x);
			//NormalizeInput();
			pDoc->TargetOutput = 0;
			pDoc->Propagation(pDoc->InputPattern);
		}
		printf("numbers: %d\n", num);
		num++;
	} while (pDoc->E > 0.000001 && num < 300000);
}


void CFernsNNView::OnRecall()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFernsNNDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	InputA(7);
	//NormalizeInput();
	pDoc->Recall();
	
	InputB(5);
	//NormalizeInput();
	pDoc->Recall();
}


void CFernsNNView::InputA(int filecount)
{
	CFernsNNDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	FILE *Rf;
	char file[250];

	sprintf(file, "%s%d%s", "C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/Learntxt/A", filecount, ".txt");
	Rf = fopen(file, "r");
	for (y = 0; y < MAXIM; y++) {
		fscanf(Rf, "%lf\n", &pDoc->InputPattern[y]);
		pDoc->InputPattern[y] = pDoc->InputPattern[y] / 15;
	}
	fclose(Rf);
}


void CFernsNNView::InputB(int filecount)
{
	CFernsNNDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	FILE *Rf;
	char file[250];

	sprintf(file, "%s%d%s", "C:/Users/AhnSeungSeop/Documents/Visual Studio 2015/Projects/FernsNN/Learntxt/B", filecount, ".txt");
	Rf = fopen(file, "r");
	for (y = 0; y < MAXIM; y++) {
		fscanf(Rf, "%lf\n", &pDoc->InputPattern[y]);
		pDoc->InputPattern[y] = pDoc->InputPattern[y] / 15;
	}
	fclose(Rf);

}


void CFernsNNView::NormalizeInput()
{
	CFernsNNDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	/*
	int min = pDoc->InputPattern[0], max = pDoc->InputPattern[0];
	
	for (y = 0; y < MAXIM; y++) {
		if (min > pDoc->InputPattern[y])
			min = pDoc->InputPattern[y];
		if (max < pDoc->InputPattern[y])
			max = pDoc->InputPattern[y];
	}
	for (y = 0; y < MAXIM; y++) {
		//pDoc->InputPattern[y] = (pDoc->InputPattern[y] - min) / (max - min); //정규화
		//pDoc->InputPattern[y] = (2 * (pDoc->InputPattern[y] - min) / (max - min))-1; //조정정규화
		//pDoc->InputPattern[y] = (pDoc->InputPattern[y] / max); //정규화
	}
	*/
}
/*
• 정규화.최소값을 뺀 후 범위로 나눕니다(x−min) / (max−min).정규화된 값은 0과 1 사이의 값입니다.
• 조정 정규화.최소값을 뺀 후 범위로 나누는 계산의 조정된 버전입니다.즉, [2 * (x−min) / (max−min)]−1.조정 정규화된 값은 - 1과 1 사이의 값입니다.
*/