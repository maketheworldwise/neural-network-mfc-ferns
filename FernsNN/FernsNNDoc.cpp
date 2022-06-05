
// FernsNNDoc.cpp : CFernsNNDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "FernsNN.h"
#endif

#include "FernsNNDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFernsNNDoc

IMPLEMENT_DYNCREATE(CFernsNNDoc, CDocument)

BEGIN_MESSAGE_MAP(CFernsNNDoc, CDocument)
END_MESSAGE_MAP()


// CFernsNNDoc ����/�Ҹ�

CFernsNNDoc::CFernsNNDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	//���� �Ҵ�
	InputPattern = (double*)malloc(sizeof(double) * MAXIM);
	HiddenI = (double*)malloc(sizeof(double) * HMAXIM);
	HiddenO = (double*)malloc(sizeof(double) * HMAXIM);

	w1 = (double**)malloc(sizeof(double*) * MAXIM);

	for (int y = 0; y < MAXIM; y++) {
		w1[y] = (double*)malloc(sizeof(double) * HMAXIM);
	}
	w2 = (double*)malloc(sizeof(double) * HMAXIM);

	Woh = (double*)malloc(sizeof(double) * HMAXIM);
	Whi = (double*)malloc(sizeof(double) * MAXIM);

	//Random ����ġ
	for (i = 0; i < MAXIM; i++) {
		for (j = 0; j < HMAXIM; j++) {
			w1[i][j] = (rand() % 49 + 1) / 100.0;
		}
	}
	for (i = 0; i < HMAXIM; i++) {
		w2[i] = (rand() % 49 + 1) / 100.0;
	}

	Bias1 = (rand() % 49 + 1) / 100.0;
	Bias2 = (rand() % 49 + 1) / 100.0;

	//E = 100.0;
}

CFernsNNDoc::~CFernsNNDoc()
{
	free(InputPattern);
	free(HiddenI);
	free(HiddenO);

	for (int y = 0; y < MAXIM; y++) {
		free(w1[y]);
	}
	free(w1);
	free(w2);

	free(Woh);
	free(Whi);
}

BOOL CFernsNNDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CFernsNNDoc serialization

void CFernsNNDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CFernsNNDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CFernsNNDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CFernsNNDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CFernsNNDoc ����

#ifdef _DEBUG
void CFernsNNDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFernsNNDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFernsNNDoc ����
double CFernsNNDoc::Sigmoid(double p)
{
	return 1 / (1 + exp((-1)*p));
}


void CFernsNNDoc::Initialization()
{
	for (i = 0; i < HMAXIM; i++)
		HiddenI[i] = 0.0;
	OutputI = 0.0;
	EH = 0.0;
}


void CFernsNNDoc::Propagation(double *input)
{
	Initialization();

	//Forward Propagation
	for (i = 0; i < HMAXIM; i++) {
		for (j = 0; j < MAXIM; j++) {
			HiddenI[i] += InputPattern[j] * w1[j][i];
		}
		HiddenI[i] = HiddenI[i] + Bias1;
	}
	for (i = 0; i < HMAXIM; i++)
		HiddenO[i] = Sigmoid(HiddenI[i]);

	for (i = 0; i < HMAXIM; i++)
		OutputI += HiddenO[i] * w2[i];

	OutputI = OutputI + Bias2;
	OutputO = Sigmoid(OutputI);

	//Back Propagation
	E = 0.5 * (TargetOutput - OutputO) * (TargetOutput - OutputO);

	EO = (TargetOutput - OutputO)*OutputO*(1 - OutputO);		//����� ������
																

	for (i = 0; i < HMAXIM; i++)
		EH += EO * w2[i] * HiddenO[i] * (1 - HiddenO[i]);		//������ ������

	for (i = 0; i < HMAXIM; i++)
		Woh[i] = 1 * EO * HiddenO[i];

	for (i = 0; i < MAXIM; i++)
		Whi[i] = 1 * EH * InputPattern[i];

	for (i = 0; i < MAXIM; i++) {
		for (j = 0; j < HMAXIM; j++) {
			w1[i][j] = w1[i][j] + Whi[i];
		}
	}

	for (i = 0; i < HMAXIM; i++)
		w2[i] = w2[i] + Woh[i];

	Bias1 = Bias1 + 1 * EH;
	Bias2 = Bias2 + 1 * EO;

	Print(w1, w2, OutputO, E);

}


void CFernsNNDoc::Recall()
{
	Initialization();

	//Forward Propagation
	for (i = 0; i < HMAXIM; i++) {
		for (j = 0; j < MAXIM; j++) {
			HiddenI[i] += InputPattern[j] * w1[j][i];
		}
		HiddenI[i] = HiddenI[i] + Bias1;
	}
	for (i = 0; i < HMAXIM; i++)
		HiddenO[i] = Sigmoid(HiddenI[i]);

	for (i = 0; i < HMAXIM; i++)
		OutputI += HiddenO[i] * w2[i];

	OutputI = OutputI + Bias2;
	OutputO = Sigmoid(OutputI);

	E = 0.5 * pow(TargetOutput - OutputO, 2);

	printf("\n\n------------��� �����------------\n");
	printf("���� ������: %lf\n", E);
	printf("���� ��°�: %lf\n", OutputO);

	if (OutputO > 0.5)
		AfxMessageBox(_T("(A) �Դϴ�."));
	else
		AfxMessageBox(_T("(B) �Դϴ�."));

}


void CFernsNNDoc::Print(double** w1, double* w2, double OutputO, double E)
{
	for (i = 0; i < MAXIM; i++) {
		for (j = 0; j < HMAXIM; j++) {
			printf("w1[%d][%d] = %.16lf\n", i, j, w1[i][j]);
		}
	}
	for (i = 0; i < HMAXIM; i++)
		printf("w2[%d] = %.16lf\n", i, w2[i]);
	printf("��°�: %lf\n", OutputO);
	printf("������: %lf\n", E);
}