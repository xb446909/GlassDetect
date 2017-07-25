
// GlassDetectDoc.h : CGlassDetectDoc 类的接口
//


#pragma once

#include "GLPrimitive.h"

class CGlassDetectDoc : public CDocument
{
protected: // 仅从序列化创建
	CGlassDetectDoc();
	DECLARE_DYNCREATE(CGlassDetectDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CGlassDetectDoc();

	map<int, CGLPrimitive*> m_mapPrimitives;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()


#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
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
