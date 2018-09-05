
// OpenCV 3.3Dlg.h : ͷ�ļ�
//

#pragma once
#include "ColorDetectController.h"

// COpenCV33Dlg �Ի���
class COpenCV33Dlg : public CDialogEx
{
// ����
public:
	COpenCV33Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCV33_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	ColorDetectController colordetect;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnProcess();
};
