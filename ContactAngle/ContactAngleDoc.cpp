// ContactAngleDoc.cpp : CContactAngleDoc ���ʵ��
//

#include "stdafx.h"
#include "ContactAngle.h"

#include "ContactAngleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CImage	SaveCanvus,WorkCanvus,img2,img3,img4,img5,img6,img7,img8,WindowCanvus;
CImage*	pImg[10];
CString	m_FileName;
double	DoubleTab[256];

// CContactAngleDoc

IMPLEMENT_DYNCREATE(CContactAngleDoc, CDocument)

BEGIN_MESSAGE_MAP(CContactAngleDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CContactAngleDoc::OnFileOpen)
END_MESSAGE_MAP()


// CContactAngleDoc ����/����

CContactAngleDoc::CContactAngleDoc()
{
	// TODO: �ڴ����һ���Թ������
	pImg[0]=&SaveCanvus;
	pImg[1]=&WorkCanvus;
	pImg[2]=&img2;
	pImg[3]=&img3;
	pImg[4]=&img4;
	pImg[5]=&img5;
	pImg[6]=&img6;
	pImg[7]=&img7;
	pImg[8]=&img8;
	pImg[9]=&WindowCanvus;

}

CContactAngleDoc::~CContactAngleDoc()
{
		for (int i=0;i<10;i++) 
		pImg[i]->Destroy();
}

BOOL CContactAngleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CContactAngleDoc ���л�

void CContactAngleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CContactAngleDoc ���

#ifdef _DEBUG
void CContactAngleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CContactAngleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CContactAngleDoc ����

void CContactAngleDoc::OnFileOpen()
{
	// TODO: �ڴ���������������
	
	//����ΪͼƬ�������
	CString strFilter;
	HRESULT hResult;
	CSimpleArray<GUID> aguidFileTypes;
	
	hResult=pImg[0]->GetExporterFilterString( strFilter, aguidFileTypes, _T("All Image Files"));//�����˾�
	
	if(FAILED(hResult))
	{       
		MessageBox(NULL,"GetExporterFilter����ʧ�ܣ�","ERROR",NULL);
		return;
	}
	
	CFileDialog opendlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,strFilter);
	opendlg.m_ofn.lpstrTitle="��ͼ���ļ�";
    if(hResult=(IDOK==opendlg.DoModal()))
	{
		/*if(!m_image.IsNull())
		{
			MessageBox("�뱣������ͼ��");		
			return;
		}*/
		if(FAILED(hResult))
		{
			MessageBox(NULL,"opendlgʧ��","ERROR",NULL);
			return;
		}
	}
	m_nFilterLoad = opendlg.m_ofn.nFilterIndex;
	m_FileName = opendlg.GetFileName();
	pImg[0]->Destroy();
	hResult=pImg[0]->Load(opendlg.GetPathName());//����ͼƬ
	if(FAILED(hResult))
	{
		MessageBox(NULL,"m_image.Loadʧ��","ERROR",NULL);
	}
	 if (ImageType(pImg[0])<2) 
 		LowTypeToIndex(pImg[1],pImg[0]);
 	else
		ImageCopy(pImg[1],pImg[0]);

	//���������������
	//CString str;
    //str.LoadString(AFX_IDS_APP_TITLE);
	//AfxGetMainWnd()->SetWindowText(str+_T("-")+opendlg.GetFileName());
	
	SetPathName(m_FileName,1);
	UpdateAllViews(NULL);

}
