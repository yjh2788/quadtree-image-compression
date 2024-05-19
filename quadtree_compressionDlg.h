
// quadtree_compressionDlg.h: 헤더 파일
//

#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;
// CquadtreecompressionDlg 대화 상자
class CquadtreecompressionDlg : public CDialogEx
{
// 생성입니다.
public:
	CquadtreecompressionDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QUADTREE_COMPRESSION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	CEdit m_edit_path;
	CEdit m_edit_deviation;
	CEdit m_edit_minimum_size;
	CEdit m_edit_describe;

	Mat m_matImage; // 이미지 정보를 담고 있는 객체.
	BITMAPINFO* m_pBitmapInfo; // Bitmap 정보를 담고 있는 구조체.

	void CreateBitmapInfo(int w, int h, int bpp); // Bitmap 정보를 생성하는 함수.
	void DrawImage(Mat m_matImage,int ID); // 그리는 작업을 수행하는 함수.
	void DrawImage(int ID);
	CStatic m_original_pic;
	CStatic m_compressed_pic;
	CEdit m_edit_just;
};
