#pragma once


// m_fileFinder

class m_fileFinder : public CFileDialog
{
	DECLARE_DYNAMIC(m_fileFinder)

public:
	m_fileFinder(BOOL bOpenFileDialog, // FileOpen은 TRUE, FileSaveAs는 FALSE입니다.
		LPCTSTR lpszDefExt = nullptr,
		LPCTSTR lpszFileName = nullptr,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = nullptr,
		CWnd* pParentWnd = nullptr);
	virtual ~m_fileFinder();

protected:
	DECLARE_MESSAGE_MAP()
};


