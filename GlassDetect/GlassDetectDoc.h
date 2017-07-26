
// GlassDetectDoc.h : CGlassDetectDoc ��Ľӿ�
//


#pragma once

#include "GLPrimitive.h"

class CGlassDetectDoc : public CDocument
{
protected: // �������л�����
	CGlassDetectDoc();
	DECLARE_DYNCREATE(CGlassDetectDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CGlassDetectDoc();

	vector<CGLPrimitive*> m_vecPrimitives;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()


#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual void OnCloseDocument();
	bool IsValid() { return m_isValid; }
	void SetValid(bool isValid) { m_isValid = isValid; }
private:
	void UpdateView();
	void ReadFile(const char* szPath);
	bool m_isValid;
};
