// ContactAngleDoc.cpp : CContactAngleDoc 类的实现
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


// CContactAngleDoc 构造/析构

CContactAngleDoc::CContactAngleDoc()
{
	// TODO: 在此添加一次性构造代码
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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CContactAngleDoc 序列化

void CContactAngleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CContactAngleDoc 诊断

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


// CContactAngleDoc 命令

void CContactAngleDoc::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	
	//以下为图片载入程序
	CString strFilter;
	HRESULT hResult;
	CSimpleArray<GUID> aguidFileTypes;
	
	hResult=pImg[0]->GetExporterFilterString( strFilter, aguidFileTypes, _T("All Image Files"));//产生滤镜
	
	if(FAILED(hResult))
	{       
		MessageBox(NULL,"GetExporterFilter调用失败！","ERROR",NULL);
		return;
	}
	
	CFileDialog opendlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,strFilter);
	opendlg.m_ofn.lpstrTitle="打开图形文件";
    if(hResult=(IDOK==opendlg.DoModal()))
	{
		/*if(!m_image.IsNull())
		{
			MessageBox("请保存或清除图像");		
			return;
		}*/
		if(FAILED(hResult))
		{
			MessageBox(NULL,"opendlg失败","ERROR",NULL);
			return;
		}
	}
	m_nFilterLoad = opendlg.m_ofn.nFilterIndex;
	m_FileName = opendlg.GetFileName();
	pImg[0]->Destroy();
	hResult=pImg[0]->Load(opendlg.GetPathName());//载入图片
	if(FAILED(hResult))
	{
		MessageBox(NULL,"m_image.Load失败","ERROR",NULL);
	}
	 if (ImageType(pImg[0])<2) 
 		LowTypeToIndex(pImg[1],pImg[0]);
 	else
		ImageCopy(pImg[1],pImg[0]);

	//设置主程序标题栏
	//CString str;
    //str.LoadString(AFX_IDS_APP_TITLE);
	//AfxGetMainWnd()->SetWindowText(str+_T("-")+opendlg.GetFileName());
	
	SetPathName(m_FileName,1);
	UpdateAllViews(NULL);

}
