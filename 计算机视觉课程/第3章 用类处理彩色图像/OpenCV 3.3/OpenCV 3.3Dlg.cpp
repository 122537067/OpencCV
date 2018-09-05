
// OpenCV 3.3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenCV 3.3.h"
#include "OpenCV 3.3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCV33Dlg 对话框



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


// COpenCV33Dlg 消息处理程序

BOOL COpenCV33Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenCV33Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenCV33Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COpenCV33Dlg::OnBnClickedBtnOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	// 选择bmp或jpg类型文件的MFC对话框
	CFileDialog dlg(TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("图像文件(*.bmp; *.jpg)|*.bmp;*.jpg|所有文件(*.*)|*.*||"),
		NULL);

	dlg.m_ofn.lpstrTitle = _T("打开图像");

	// 如果选中了一个文件名
	if (dlg.DoModal() == IDOK) {
		// 取得选定文件名的完整路径
		std::string filename = dlg.GetPathName();

		// 设置并显示输入的图像
		colordetect.setInputImage(filename);
		cv::imshow("输入图像", colordetect.getInputImage());
	}
}


void COpenCV33Dlg::OnBnClickedBtnProcess()
{
	// TODO: 在此添加控件通知处理程序代码

	// 这里目标颜色采用了硬编码
	colordetect.setTargetColor(191, 209, 221);
	// 处理输入图像并显示结果
	colordetect.process();
	cv::imshow("输出图像", colordetect.getLastResult());
}
