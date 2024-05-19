#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
// quadtree_compressionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "quadtree_compression.h"
#include "quadtree_compressionDlg.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include "quadtree.h"
#include<iostream>
#include<stdio.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CquadtreecompressionDlg 대화 상자



CquadtreecompressionDlg::CquadtreecompressionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QUADTREE_COMPRESSION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CquadtreecompressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit_path);
	DDX_Control(pDX, IDC_EDIT3, m_edit_deviation);
	DDX_Control(pDX, IDC_EDIT4, m_edit_minimum_size);
	DDX_Control(pDX, IDC_EDIT2, m_edit_describe);
	DDX_Control(pDX, IDC_STATIC2, m_original_pic);
	DDX_Control(pDX, IDC_STATIC3, m_compressed_pic);
	DDX_Control(pDX, IDC_EDIT5, m_edit_just);
}

BEGIN_MESSAGE_MAP(CquadtreecompressionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_EN_CHANGE(IDC_EDIT1, &CquadtreecompressionDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CquadtreecompressionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CquadtreecompressionDlg 메시지 처리기

BOOL CquadtreecompressionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CquadtreecompressionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);


		

	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CquadtreecompressionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CquadtreecompressionDlg::OnEnChangeEdit1()
//{
//	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
//	// CDialogEx::OnInitDialog() 함수를 재지정 
//	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
//	// 이 알림 메시지를 보내지 않습니다.
//
//	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


void CquadtreecompressionDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())

	{
		path = dlg.GetPathName();
		m_edit_path.SetWindowText(path);//SetDlgItemText(IDC_EDIT1,_T(path));
		MessageBox(path);
		/// <summary>
		/// ///
		/// </summary>
		//m_matImage = imread((string)CT2CA(path), IMREAD_UNCHANGED);
		//CreateBitmapInfo(m_matImage.cols, m_matImage.rows, m_matImage.channels() * 8);

		//DrawImage();
		
		FILE* fp = fopen("compressed.txt", "wb");
		quadtree q;
		Image* img = new Image;
		img->ImageRead(img);

		CRect rect;
		GetDlgItem(IDC_STATIC2)->GetClientRect(&rect);
		resize(img->M_img, img->M_img, Size(512, 512));
		CreateBitmapInfo(img->M_img.cols, img->M_img.rows, img->M_img.channels() * 8);
		DrawImage(img->M_img,IDC_STATIC2);
		int w = img->M_img.cols;
		int h = img->M_img.rows;
		q.total_sigma = 1;
		q.img_init(img);//쿼드트리에 이미지 입력
		q.quadrant(q.head_r.root);///분할
		q.BFSearch(q.head_r.root);//너비우선탐색하며 인코딩
		q.saveAsFile( w, h, fp);//파일로 저장

		fp = fopen("compressed.txt", "rb");
		fseek(fp, 0, SEEK_END);
		int compressed_size = ftell(fp);
		unsigned char* data = readFile(fp);//파일 읽기

		Mat* compressed = q.decode(data);//디코딩

		int orignal_size = w * h * 24;
		double compress_ratio = ((double)(orignal_size - compressed_size) / orignal_size) * 100;
		//printf("\n compression ratio = %lf%%\n", compress_ratio);
		//printf("minimum block size = %d", minimum_block_size);
		

		Image t;
		t.M_img = compressed->clone();
		CreateBitmapInfo(t.M_img.cols,t.M_img.rows, t.M_img.channels() * 8);
		DrawImage(t.M_img, IDC_STATIC3);

		float comp_deviation=t.calcImageStd();
		CString deviation_;
		deviation_.Format(_T("%f"), comp_deviation);
		m_edit_deviation.SetWindowText(deviation_);

		CString minimun_;
		minimun_.Format(_T("%d"), minimum_block_size);
		m_edit_minimum_size.SetWindowText(minimun_);
		
		m_edit_just.SetWindowText(_T("압축&복원"));

		float PSNR_= PSNR(img->M_img,*compressed);

		CString text;
		text.Format("해상도: %d*%d    원본크기:%d bytes 압축크기:%d bytes    압축률:((%d-%d))*100/%d=%lf   화질(PSNR dB)\n%lf dB",w,h, orignal_size, compressed_size, orignal_size, compressed_size, orignal_size,compress_ratio,PSNR_);
		m_edit_describe.SetWindowText(text);
	}
}

void CquadtreecompressionDlg::CreateBitmapInfo(int w, int h, int bpp)
{
	if (m_pBitmapInfo != NULL)
	{
		delete m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;
}
void CquadtreecompressionDlg::DrawImage(Mat m_matImage, int ID)
{
	CClientDC dc(GetDlgItem(ID));

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	//resize(m_matImage, m_matImage, Size(rect.Width(), rect.Height()));

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_matImage.cols, m_matImage.rows, m_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}
void CquadtreecompressionDlg::DrawImage(int ID)
{
	CClientDC dc(GetDlgItem(ID));

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_matImage.cols, m_matImage.rows, m_matImage.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}