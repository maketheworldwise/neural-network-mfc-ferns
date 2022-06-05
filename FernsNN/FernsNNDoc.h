
// FernsNNDoc.h : CFernsNNDoc Ŭ������ �������̽�
//


#pragma once
#define MAXIM 70
#define HMAXIM 10
#include <math.h>
#include <malloc.h>

class CFernsNNDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CFernsNNDoc();
	DECLARE_DYNCREATE(CFernsNNDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CFernsNNDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

	double* InputPattern;
	double* HiddenI;
	double* HiddenO;
	double OutputI;
	double OutputO;
	double TargetOutput;

	double** w1;
	double* w2;
	double Bias1;
	double Bias2;

	double E;
	double EO;
	double EH;

	double* Woh;
	double* Whi;

	int i, j;

	double Sigmoid(double p);
	void Initialization();
	void Propagation(double *input);
	void Recall();
	void Print(double** w1, double* w2, double OutputO, double E);
};
