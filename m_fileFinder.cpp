// m_fileFinder.cpp: 구현 파일
//

#include "pch.h"
#include "quadtree_compression.h"
#include "m_fileFinder.h"


// m_fileFinder

IMPLEMENT_DYNAMIC(m_fileFinder, CFileDialog)

m_fileFinder::m_fileFinder(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{

}

m_fileFinder::~m_fileFinder()
{
}


BEGIN_MESSAGE_MAP(m_fileFinder, CFileDialog)
END_MESSAGE_MAP()



// m_fileFinder 메시지 처리기


