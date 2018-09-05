
// OpenCV 3.3Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenCV 3.3.h"
#include "OpenCV 3.3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCV33Dlg �Ի���



COpenCV33Dlg::COpenCV33Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENCV33_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenCV33Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenCV33Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &COpenCV33Dlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_PROCESS, &COpenCV33Dlg::OnBnClickedBtnProcess)
END_MESSAGE_MAP()


// COpenCV33Dlg ��Ϣ�������

BOOL COpenCV33Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COpenCV33Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COpenCV33Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COpenCV33Dlg::OnBnClickedBtnOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ѡ��bmp��jpg�����ļ���MFC�Ի���
	CFileDialog dlg(TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("ͼ���ļ�(*.bmp; *.jpg)|*.bmp;*.jpg|�����ļ�(*.*)|*.*||"),
		NULL);

	dlg.m_ofn.lpstrTitle = _T("��ͼ��");

	// ���ѡ����һ���ļ���
	if (dlg.DoModal() == IDOK) {
		// ȡ��ѡ���ļ���������·��
		std::string filename = dlg.GetPathName();

		// ���ò���ʾ�����ͼ��
		colordetect.setInputImage(filename);
		cv::imshow("����ͼ��", colordetect.getInputImage());
	}
}


void COpenCV33Dlg::OnBnClickedBtnProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// ����Ŀ����ɫ������Ӳ����
	colordetect.setTargetColor(191, 209, 221);
	// ��������ͼ����ʾ���
	colordetect.process();
	cv::imshow("���ͼ��", colordetect.getLastResult());
}
