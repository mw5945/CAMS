// ContactAngleDoc.h : CContactAngleDoc ��Ľӿ�
//


#pragma once


class CContactAngleDoc : public CDocument
{
protected: // �������л�����
	CContactAngleDoc();
	DECLARE_DYNCREATE(CContactAngleDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CContactAngleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
private:
	int m_nFilterLoad;
};


