
// RenderCenterDlg.cpp : 实现文件
//

#include "stdafx.h"

#include "RenderCenter.h"
#include "RenderCenterDlg.h"
#include "CvvImage.h"
#include "FeatureMatch.h"
#include "LaplacianBlending.h"
#include "RateBlending.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926
string dir_path = "D:\\test\\imageA\\"; 
string dir_path1= "D:\\test\\imageB\\";
string dir_path2="D:\\test\\fusionimage\\";
Directory dir;  
CRenderCenterDlg * _Global_Obj_Ptr=NULL;
int i=0,j=0,k=0,l=0,m=0,h=0;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{	
	
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	if(i<fileNames.size())
	{
		string fileName = fileNames[i];  
		string fileFullName = dir_path + fileName;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->OnBnClickedEnhance();		
		i++;

	}
	else
	{
		i=0;
	}
	

}
VOID CALLBACK TimerProc1(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	if(j<fileNames.size())
	{
		string fileName = fileNames[j];
		string fileName1=fileNames1[j];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		_Global_Obj_Ptr->OnBnClickedRegistry();					
		j++;

	}
	else
	{
		j=0;
	}
}
VOID CALLBACK TimerProc2(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	if(k<fileNames.size())
	{
		string fileName = fileNames[k];  
		string fileName1=fileNames1[k];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		_Global_Obj_Ptr->OnBnClickedImagefusion();
		string fileFullName2=dir_path2+fileName;
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		k++;

	}
	else
	{
		k=0;
	}
}
VOID CALLBACK TimerProc3(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	if(l<fileNames.size())
	{
		string fileName = fileNames[l]; 
		string fileName1=fileNames1[l];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		_Global_Obj_Ptr->OnBnClickedLaplace();
		string fileFullName2=dir_path2+fileName;
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		l++;

	}
	else
	{
		l=0;
	}
}
VOID CALLBACK TimerProc4(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	if(m<fileNames.size())
	{
		string fileName = fileNames[m];
		string fileName1=fileNames1[m];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		_Global_Obj_Ptr->OnBnClickedHisfusion();
		string fileFullName2=dir_path2+fileName;
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		m++;

	}
	else
	{
		m=0;
	}
}
VOID CALLBACK TimerProc5(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	vector<string> fileNames = dir.GetListFiles(dir_path, "*.*", false);
	vector<string> fileNames1 = dir.GetListFiles(dir_path1, "*.*", false);
	if(h<fileNames.size())
	{
		string fileName = fileNames[h]; 
		string fileName1=fileNames1[h];
		string fileFullName = dir_path + fileName;
		string fileFullName1=dir_path1+fileName1;
		_Global_Obj_Ptr->img1=imread(fileFullName);
		_Global_Obj_Ptr->img2=imread(fileFullName1);
		_Global_Obj_Ptr->OnBnClickedRate();	
		string fileFullName2=dir_path2+fileName;
		imwrite(fileFullName2,_Global_Obj_Ptr->imgfusion);
		h++;

	}
	else
	{
		h=0;
	}
}

// CRenderCenterDlg 对话框




CRenderCenterDlg::CRenderCenterDlg(CWnd* pParent /*=NULL*/)
	: CCommonDlg(CRenderCenterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRenderCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboL);
	DDX_Control(pDX, IDC_IMGA, m_imageA);
	DDX_Control(pDX, IDC_IMGB, m_imageB);
	DDX_Control(pDX, IDC_IMGFUSION, m_imagefusion);
	DDX_Control(pDX, IDC_IMGTURN, m_imageturn);
}

BEGIN_MESSAGE_MAP(CRenderCenterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INPUTA, &CRenderCenterDlg::OnBnClickedInputa)
	ON_BN_CLICKED(IDC_INPUTB, &CRenderCenterDlg::OnBnClickedInputb)
	ON_BN_CLICKED(IDC_INPUTMUBAN, &CRenderCenterDlg::OnBnClickedInputmuban)
	ON_BN_CLICKED(IDC_ENHANCE, &CRenderCenterDlg::OnBnClickedEnhance)
	ON_BN_CLICKED(IDC_REGISTRY, &CRenderCenterDlg::OnBnClickedRegistry)
	ON_BN_CLICKED(IDC_IMAGEFUSION, &CRenderCenterDlg::OnBnClickedImagefusion)
	ON_BN_CLICKED(IDC_LAPLACE, &CRenderCenterDlg::OnBnClickedLaplace)
	ON_BN_CLICKED(IDC_RATE, &CRenderCenterDlg::OnBnClickedRate)
	ON_BN_CLICKED(IDC_HISFUSION, &CRenderCenterDlg::OnBnClickedHisfusion)
	ON_BN_CLICKED(IDC_TARGET, &CRenderCenterDlg::OnBnClickedTarget)
	ON_BN_CLICKED(IDC_START, &CRenderCenterDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CRenderCenterDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_SAVE, &CRenderCenterDlg::OnBnClickedSave)
END_MESSAGE_MAP()


// CRenderCenterDlg 消息处理程序

BOOL CRenderCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ComboL.AddString(_T("图像增强"));
	m_ComboL.AddString(_T("图像配准"));
	m_ComboL.AddString(_T("图像简单加权融合"));
	m_ComboL.AddString(_T("图像拉普拉斯融合"));
	m_ComboL.AddString(_T("图像HIS融合"));
	m_ComboL.AddString(_T("图像比率低通融合"));
	m_ComboL.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRenderCenterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRenderCenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CRenderCenterDlg::hsi2rgb(Mat &hsi,Mat &image)
{
	if(!hsi.data)
	{  
		cout<<"Miss Data"<<endl;  
		return -1;  
	}  
	int nl = hsi.rows;  
	int nc = hsi.cols;  
	if(hsi.isContinuous())
	{  
		nc = nc*nl;  
		nl = 1;  
	}  
	for(int i = 0;i < nl;i++)
	{
		uchar *src = hsi.ptr<uchar>(i);  
		uchar *dst = image.ptr<uchar>(i);  
		for(int j = 0;j < nc;j++)
		{
			float H = src[j*3]/255.0;  
			float S = src[j*3+1]/255.0;  
			float I = src[j*3+2]/255.0;  
			float b,g,r;
			if(I==0)
			{
				b=g=r=0;
			}
			else
			{
				if(S==0)
				{
					r=g=b=1;
				}
				float temp1,temp2,temp3;
				temp1=(1-S)/3;
				temp2=S*cos(H);
				if(H>=0&&H<(PI*2/3))
				{
					b=temp1;temp3=cos(PI/3-H);
					r=(1+temp2/temp3)/3;
					g=1-r-b;
					r=3*I*r;
					g=3*g*I;
					b=3*I*b;
				}
				else if(H>=(PI*2/3)&&H<(PI*4/3))
				{
					r=temp1;
					temp3=cos(PI-H);
					g=(1+temp2/temp3)/3;
					b=1-r-b;
					r=3*I*r;
					g=3*g*I;
					b=3*I*b;
				}
				else if(H>=(PI*4/3)&&H<(PI*2))
				{
					g=temp1;
					temp3=cos(PI*5/3-H);
					b=(1+temp2/temp3)/3;
					r=1-r-b;
					r=3*I*r;
					g=3*g*I;
					b=3*I*b;
				}
			}
			dst[3*j] = b*255;  
			dst[3*j+1] = g*255;  
			dst[3*j+2] = r*255;  

		}

	}
	return 0;
}
int CRenderCenterDlg::rgb2hsi(Mat &image,Mat &hsi)
{
	if(!image.data)
	{  
		cout<<"Miss Data"<<endl;  
		return -1;  
	}  
	int nl = image.rows;  
	int nc = image.cols;  
	if(image.isContinuous())
	{  
		nc = nc*nl;  
		nl = 1;  
	}  
	for(int i = 0;i < nl;i++)
	{  
		uchar *src = image.ptr<uchar>(i);  
		uchar *dst = hsi.ptr<uchar>(i);  
		for(int j = 0;j < nc;j++)
		{  
			float b = src[j*3]/255.0;  
			float g = src[j*3+1]/255.0;  
			float r = src[j*3+2]/255.0;  

			float num = (float)(0.5*((r-g)+(r-b)));  
			float den = (float)sqrt((r-g)*(r-g)+(r-b)*(g-b));  
			float H,S,I;  
			if(den == 0)
			{   //分母不能为0  
				H = 0;  
			}  
			else
			{  
				double theta = acos(num/den);  
				if(b <= g)  
					H = theta/(PI*2);  
				else  
					H = (2*PI - theta)/(2*PI);  
			}  
			double minRGB = min(min(r,g),b);  
			den = r+g+b;  
			if(den == 0)    //分母不能为0  
				S = 0;  
			else  
				S = 1 - 3*minRGB/den;  
			I = den/3.0;  
			//将S分量和H分量都扩充到[0,255]区间以便于显示;  
			//一般H分量在[0,2pi]之间，S在[0,1]之间  
			dst[3*j] = H*255;  
			dst[3*j+1] = S*255;  
			dst[3*j+2] = I*255;  
		}  
	}  
	return 0;  
}

Mat CRenderCenterDlg::GetImage(double &p,double &q)
{
	//接受俯仰和方位，返回一张图片
	Mat getimage;
	return getimage;
}
void CRenderCenterDlg::OnBnClickedInputa()
{
	// TODO: 在此添加控件通知处理程序代码
	string tstring;
	CString tFileName;
	CFileDialog tDlg(TRUE);
	if(tDlg.DoModal()==IDOK)
	{
		tFileName=tDlg.GetPathName();
		tstring=tFileName.GetBuffer(0);
	}
	img1=imread(tstring,1);
	if(!img1.data)
	{
		MessageBox("error","no image loaded!",MB_OK);
		return;
	}


	CWnd *pWnd=GetDlgItem(IDC_IMGA);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=img1;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGA)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedInputb()
{
	// TODO: 在此添加控件通知处理程序代码
	string tstring;
	CString tFileName;
	CFileDialog tDlg(TRUE);
	if(tDlg.DoModal()==IDOK)
	{
		tFileName=tDlg.GetPathName();
		tstring=tFileName.GetBuffer(0);
	}
	img2=imread(tstring,1);
	if(!img2.data)
	{
		MessageBox("error","no image loaded!",MB_OK);
		return;
	}
	CWnd *pWnd=GetDlgItem(IDC_IMGB);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=img2;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGB)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedInputmuban()
{
	// TODO: 在此添加控件通知处理程序代码
	string tstring;
	CString tFileName;
	CFileDialog tDlg(TRUE);
	if(tDlg.DoModal()==IDOK)
	{
		tFileName=tDlg.GetPathName();
		tstring=tFileName.GetBuffer(0);
	}
	imgtarget=imread(tstring,1);
	if(!imgtarget.data)
	{
		MessageBox("error","no image loaded!",MB_OK);
		return;
	}
	CWnd *pWnd=GetDlgItem(IDC_MUBAN);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgtarget;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_MUBAN)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedEnhance()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!img1.data)
	{
		MessageBox("no image loaded!","error",MB_OK);
		return;
	}
	vector<Mat> splitBGR(img1.channels());
	split(img1,splitBGR);
	for(int i=0;i<img1.channels();i++)
		equalizeHist(splitBGR[i],splitBGR[i]);
	merge(splitBGR,img1);	
	CWnd *pWnd=GetDlgItem(IDC_IMGA);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=img1;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGA)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedRegistry()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!img1.data||!img2.data)
	{
		MessageBox("no image loaded!","error",MB_OK);
		return;
	}
	cv::initModule_nonfree();
	FeatureMatch *sp=NULL;
	bool isWarpPerspective = 0;
	double ransacReprojThreshold = 3;
	Ptr<FeatureDetector> detector = FeatureDetector::create( "SIFT");
	Ptr<DescriptorExtractor> descriptorExtractor = DescriptorExtractor::create( "SIFT");
	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create( "FlannBased");//"BruteForce");
	int mactherFilterType = sp->getMatcherFilterType( "CrossCheckFilter");
	bool eval = false;
	vector<KeyPoint> keypoints1;
	detector->detect( img1, keypoints1 );
	Mat descriptors1;
	descriptorExtractor->compute( img1, keypoints1, descriptors1 );
	RNG rng = theRNG();
	img3=sp->doIteration( img1, img2, isWarpPerspective, keypoints1, descriptors1,
		detector, descriptorExtractor, descriptorMatcher, mactherFilterType, eval,
		ransacReprojThreshold, rng );
	img1=img3;
	CWnd *pWnd=GetDlgItem(IDC_IMGTURN);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=img1;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGTURN)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedImagefusion()
{
	// TODO: 在此添加控件通知处理程序代码
	if(img1.size()!=img2.size())
	{
		MessageBox("Two images are not the same size!","error",MB_OK);
		return;
	}
	double alpha=0.6;
	double beta=(1.0-alpha);

	addWeighted(img1,alpha,img2,beta,0.0,imgfusion);
	

	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedLaplace()
{
	// TODO: 在此添加控件通知处理程序代码
	if(img1.size()!=img2.size())
	{
		MessageBox("Two images are not the same size!","error",MB_OK);
		return;
	}
	LaplacianBlending lp;
	Mat_<Vec3f> l; img1.convertTo(l,CV_32F,1.0/255.0);//Vec3f表示有三个通道，即 l[row][column][depth]  
	Mat_<Vec3f> r; img2.convertTo(r,CV_32F,1.0/255.0);
	Mat_<Vec3f> blend =lp.LaplacianBlend(l, r);
	blend.convertTo(imgfusion,CV_8UC3,255);

	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedRate()
{
	// TODO: 在此添加控件通知处理程序代码
	if(img1.size()!=img2.size())
	{
		MessageBox("Two images are not the same size!","error",MB_OK);
		return;
	}
	RateBlending lp;
	Mat_<Vec3f> l; img1.convertTo(l,CV_32F,1.0/255.0);//Vec3f表示有三个通道，即 l[row][column][depth]  
	Mat_<Vec3f> r; img2.convertTo(r,CV_32F,1.0/255.0);
	Mat_<float> m(l.rows,l.cols,0.0);
	m(Range::all(),Range(0,m.cols/2)) = 1.0; 
	Mat_<Vec3f> blend =lp.RateBlend(l, r, m);
	blend.convertTo(imgfusion,CV_8UC3,255);

	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedHisfusion()
{
	// TODO: 在此添加控件通知处理程序代码
	if(img1.size()!=img2.size())
	{
		MessageBox("Two images are not the same size!","error",MB_OK);
		return;
	}
	Mat hsi1,hsi2,hsi3,p1,p2;
	p1=img1;
	p2=img2;
	hsi1.create(p1.rows,img1.cols,CV_8UC3);
	hsi2.create(p2.rows,img2.cols,CV_8UC3);
	hsi3=hsi1;
	rgb2hsi(p1,hsi1);
	rgb2hsi(p2,hsi2);
	int n1=hsi1.rows;
	int nc=hsi1.cols;
	if(hsi1.isContinuous())
	{
		nc=nc*n1;
		n1=1;
	}
	for(int i=0;i<n1;i++)
	{
		uchar *pl=hsi1.ptr<uchar>(i);
		uchar *pr=hsi2.ptr<uchar>(i);		
		for(int j=0;j<nc;j++)
		{
			uchar t;
			t=pl[3*j+2];
			pl[3*j+2]=pr[3*j+2];
			pr[3*j+2]=t;

		}
	}
	hsi2rgb(hsi1,p1);
	hsi2rgb(hsi2,p2);

	double alpha=0.5;
	double beta=(1.0-alpha);	
	addWeighted(p1,alpha,p2,beta,0.0,imgfusion);

	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedTarget()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!imgtarget.data)
	{
		MessageBox("no target image loaded!","error",MB_OK);
		return;
	}
	SiftFeatureDetector siftdtc;
	vector<KeyPoint>kp1,kp2;
	siftdtc.detect(imgtarget,kp1);
	siftdtc.detect(imgfusion,kp2);

	SiftDescriptorExtractor extractor;
	Mat descriptor1,descriptor2;
	extractor.compute(imgtarget,kp1,descriptor1);
	extractor.compute(imgfusion,kp2,descriptor2);

	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch> matches;
	matcher.match(descriptor1,descriptor2,matches);

	int i,j;
	int pointcount=(int)matches.size();
	Mat point1(pointcount,2,CV_32F);
	Mat point2(pointcount,2,CV_32F);
	Point2f point;
	for(i=0;i<pointcount;i++)
	{
		point=kp1[matches[i].queryIdx].pt;
		point1.at<float>(i,0)=point.x;
		point1.at<float>(i,1)=point.y;

		point=kp2[matches[i].trainIdx].pt;
		point2.at<float>(i,0)=point.x;
		point2.at<float>(i,1)=point.y;
	}
	Mat m_fundamental;
	vector<uchar> m_ransacstatus;
	m_fundamental=findFundamentalMat(point1,point2,m_ransacstatus,FM_RANSAC);

	float hhh[9];
	for(i=0;i<9;i++)
		hhh[i]=0;

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			hhh[i*3+j]=m_fundamental.ptr<float>(i)[j];
		}
	}
	int outlinercount=0;
	for(i=0;i<pointcount;i++)
	{
		if(m_ransacstatus[i]==0)
		{
			outlinercount++;
		}
	}

	vector<Point2f> m_leftinliner;
	vector<Point2f> m_rightinliner;
	vector<DMatch> m_inlinermatches;
	int inlinercount=pointcount-outlinercount;
	m_inlinermatches.resize(inlinercount);
	m_leftinliner.resize(inlinercount);
	m_rightinliner.resize(inlinercount);
	inlinercount=0;
	for(i=0;i<pointcount;i++)
	{
		if(m_ransacstatus[i]!=0)
		{
			m_leftinliner[inlinercount].x=point1.at<float>(i,0);
			m_leftinliner[inlinercount].y=point1.at<float>(i,1);
			m_rightinliner[inlinercount].x=point2.at<float>(i,0);
			m_rightinliner[inlinercount].y=point2.at<float>(i,1);
			m_inlinermatches[inlinercount].queryIdx=inlinercount;
			m_inlinermatches[inlinercount].trainIdx=inlinercount;
			inlinercount++;
		}
	}
	vector<KeyPoint> key1(inlinercount);
	vector<KeyPoint> key2(inlinercount);
	KeyPoint::convert(m_leftinliner,key1);
	KeyPoint::convert(m_rightinliner,key2);

	Mat H=findHomography(m_leftinliner,m_rightinliner,CV_RANSAC);
	std::vector<Point2f> obj_corners(4);
    obj_corners[0]=cv::Point(0,0);obj_corners[1]=cv::Point(imgtarget.cols,0);
    obj_corners[2]=cv::Point(imgtarget.cols,imgtarget.rows);obj_corners[3]=cv::Point(0,imgtarget.rows);
	std::vector<Point2f>scene_corners(4);
	perspectiveTransform(obj_corners,scene_corners,H);
	rectangle(imgfusion,scene_corners[0],scene_corners[2],Scalar(0,0,255,0),1,8,0);


	CWnd *pWnd=GetDlgItem(IDC_IMGFUSION);
	CDC *pDC=pWnd->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	IplImage img=imgfusion;
	CvvImage cimg;
	cimg.CopyOf(&img);
	CRect rect;
	GetDlgItem(IDC_IMGFUSION)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect);
}


void CRenderCenterDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	
	switch(m_ComboL.GetCurSel())
	{
	case 0:SetTimer(1,1000,TimerProc);break;
	case 1:SetTimer(2,1000,TimerProc1);break;
	case 2:SetTimer(3,1000,TimerProc2);break;
	case 3:SetTimer(4,1000,TimerProc3);break;
	case 4:SetTimer(5,1000,TimerProc4);break;
	case 5:SetTimer(6,1000,TimerProc5);break;

	}

	_Global_Obj_Ptr = this;
}


void CRenderCenterDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_ComboL.GetCurSel())
	{
	case 0:KillTimer(1); break;
	case 1:KillTimer(2);break;
	case 2:KillTimer(3);break;
	case 3:KillTimer(4);break;
	case 4:KillTimer(5);break;
	case 5:KillTimer(6);break;
	}
}


void CRenderCenterDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	string tstring;
	CString tFileName;
	CFileDialog tDlg(FALSE,TEXT("jpg"),NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
		TEXT("图像文件格式(*.bmp)|*.bmp|JPG file format(*.jpg)|*.jpg|",NULL));
	if(tDlg.DoModal()==IDOK)
	{
		tFileName=tDlg.GetPathName();
		tstring=tFileName.GetBuffer(0);
	}
	if(!imgfusion.data)
	{
		MessageBox("error","imgfusion is empty!",MB_OK);
		return;
	}
	imwrite(tstring,imgfusion);
}
