
// xpgwinDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "xpgwin.h"
#include "xpgwinDlg.h"
#include "afxdialogex.h"

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <errno.h>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <deque>

//-----
// 科大讯飞语音合成
#include "qtts.h"
#include "msp_cmn.h"
#include "msp_errors.h"
//----------


#include "json.hpp"
#include "hiveon.h"
#include "format.h"
#include "date.h"
#include "utils.h"


#include <iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib") 

// windows自带音频播放
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//using namespace nlohmann;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CxpgwinDlg 对话框



CxpgwinDlg::CxpgwinDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XPGWIN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_isStart = false;
	m_hLoopThread = NULL;
}

void CxpgwinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CxpgwinDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSTART, &CxpgwinDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_LOGIN, &CxpgwinDlg::OnBnClickedLogin)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CxpgwinDlg 消息处理程序

BOOL CxpgwinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	GetClientRect(&m_wndRect);//获取窗口尺寸

	// 设置标题
	SetWindowText(_T("小钢炮专用监控程序v1.1"));

	// 设置输出的字体
	CFont outputFont;
	outputFont.CreatePointFont(150, _T("微软雅黑"));
	GetDlgItem(IDC_OUTPUT)->SetFont(&outputFont);

	
	// 设置参数
	GetDlgItem(IDC_OFFTIME)->SetWindowText(_T("15")); // 离线时间阈值，默认15分钟
	GetDlgItem(IDC_GAPTIME)->SetWindowText(_T("10"));  // 每次报警间隔时间，默认10分钟

	// 设置监控钱包地址
	CString cstrAddress;
	cstrAddress += _T("0xc8eb99d5db1ec8ed483bf36cf548d096c063b4b2\r\n"); // w
	cstrAddress += _T("0xa71f66a1faa36ae54ef8c3141bbdfc0aae3791ee\r\n"); // d
	cstrAddress += _T("0x20f72f9bad243ac4d49101a29aa1cb180b933930\r\n"); // y
	cstrAddress += _T("0xeb4e7964ee29122253c0e10c07ed6bcbfac19236\r\n"); // heihei 
	cstrAddress += _T("0xa1647b564b3c1e9617d431100fff7ea8740fb62b\r\n"); // c
	cstrAddress += _T("0x6b41d273ebe0cfe3c1c54253aa251a0b5c57e06d\r\n"); // z
	GetDlgItem(IDC_ADDRESS)->SetWindowText(cstrAddress);


	// 最大化窗口
	ShowWindow(SW_MAXIMIZE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CxpgwinDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CxpgwinDlg::OnPaint()
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
HCURSOR CxpgwinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





/* wav音频头部格式 */
typedef struct _wave_pcm_hdr
{
	char            riff[4];                // = "RIFF"
	int				size_8;                 // = FileSize - 8
	char            wave[4];                // = "WAVE"
	char            fmt[4];                 // = "fmt "
	int				fmt_size;				// = 下一个结构体的大小 : 16

	short int       format_tag;             // = PCM : 1
	short int       channels;               // = 通道数 : 1
	int				samples_per_sec;        // = 采样率 : 8000 | 6000 | 11025 | 16000
	int				avg_bytes_per_sec;      // = 每秒字节数 : samples_per_sec * bits_per_sample / 8
	short int       block_align;            // = 每采样点字节数 : wBitsPerSample / 8
	short int       bits_per_sample;        // = 量化比特数: 8 | 16

	char            data[4];                // = "data";
	int				data_size;              // = 纯数据长度 : FileSize - 44 
} wave_pcm_hdr;

/* 默认wav音频头部数据 */
wave_pcm_hdr default_wav_hdr =
{
	{ 'R', 'I', 'F', 'F' },
	0,
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	16,
	1,
	1,
	16000,
	32000,
	2,
	16,
	{'d', 'a', 't', 'a'},
	0
};
/* 文本合成 */
int text_to_speech(const char* src_text, const char* des_path, const char* params)
{
	int          ret = -1;
	FILE* fp = NULL;
	const char* sessionID = NULL;
	unsigned int audio_len = 0;
	wave_pcm_hdr wav_hdr = default_wav_hdr;
	int          synth_status = MSP_TTS_FLAG_STILL_HAVE_DATA;

	if (NULL == src_text || NULL == des_path)
	{
		printf("params is error!\n");
		return ret;
	}
	fp = fopen(des_path, "wb");
	if (NULL == fp)
	{
		printf("open %s error.\n", des_path);
		return ret;
	}
	/* 开始合成 */
	sessionID = QTTSSessionBegin(params, &ret);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionBegin failed, error code: %d.\n", ret);
		fclose(fp);
		return ret;
	}
	ret = QTTSTextPut(sessionID, src_text, (unsigned int)strlen(src_text), NULL);
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSTextPut failed, error code: %d.\n", ret);
		QTTSSessionEnd(sessionID, "TextPutError");
		fclose(fp);
		return ret;
	}
	printf("正在合成 ...\n");
	fwrite(&wav_hdr, sizeof(wav_hdr), 1, fp); //添加wav音频头，使用采样率为16000
	while (1)
	{
		/* 获取合成音频 */
		const void* data = QTTSAudioGet(sessionID, &audio_len, &synth_status, &ret);
		if (MSP_SUCCESS != ret)
			break;
		if (NULL != data)
		{
			fwrite(data, audio_len, 1, fp);
			wav_hdr.data_size += audio_len; //计算data_size大小
		}
		if (MSP_TTS_FLAG_DATA_END == synth_status)
			break;
	}
	printf("\n");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSAudioGet failed, error code: %d.\n", ret);
		QTTSSessionEnd(sessionID, "AudioGetError");
		fclose(fp);
		return ret;
	}
	/* 修正wav文件头数据的大小 */
	wav_hdr.size_8 += wav_hdr.data_size + (sizeof(wav_hdr) - 8);

	/* 将修正过的数据写回文件头部,音频文件为wav格式 */
	fseek(fp, 4, 0);
	fwrite(&wav_hdr.size_8, sizeof(wav_hdr.size_8), 1, fp); //写入size_8的值
	fseek(fp, 40, 0); //将文件指针偏移到存储data_size值的位置
	fwrite(&wav_hdr.data_size, sizeof(wav_hdr.data_size), 1, fp); //写入data_size的值
	fclose(fp);
	fp = NULL;
	/* 合成完毕 */
	ret = QTTSSessionEnd(sessionID, "Normal");
	if (MSP_SUCCESS != ret)
	{
		printf("QTTSSessionEnd failed, error code: %d.\n", ret);
	}

	return ret;
}

#if 0
int generate()
{
	int         ret = MSP_SUCCESS;
	const char* login_params = "appid = 01e9f2eb, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动
	/*
	* rdn:           合成音频数字发音方式
	* volume:        合成音频的音量
	* pitch:         合成音频的音调
	* speed:         合成音频对应的语速
	* voice_name:    合成发音人
	* sample_rate:   合成音频采样率
	* text_encoding: 合成文本编码格式
	*
	*/
	const char* session_begin_params = "engine_type = local, voice_name = xiaoyan, text_encoding = GB2312, tts_res_path = fo|res\\tts\\xiaoyan.jet;fo|res\\tts\\common.jet, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
	const char* filename = "tts_sample.wav"; //合成的语音文件名称
	const char* text = "科大讯飞作为智能语音技术提供商，在智能语音技术领域有着长期的研究积累，并在中文语音合成、语音识别、口语评测等多项技术上拥有技术成果。科大讯飞是我国以语音技术为产业化方向的国家863计划产业化基地"; //合成文本
	/* 用户登录 */
	ret = MSPLogin(NULL, NULL, login_params); //第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
	if (MSP_SUCCESS != ret)
	{
		printf("MSPLogin failed, error code: %d.\n", ret);
		goto exit;//登录失败，退出登录
	}

	printf("\n###########################################################################\n");
	printf("## 语音合成（Text To Speech，TTS）技术能够自动将任意文字实时转换为连续的 ##\n");
	printf("## 自然语音，是一种能够在任何时间、任何地点，向任何人提供语音信息服务的  ##\n");
	printf("## 高效便捷手段，非常符合信息时代海量数据、动态更新和个性化查询的需求。  ##\n");
	printf("###########################################################################\n\n");

	/* 文本合成 */
	printf("开始合成 ...\n");
	ret = text_to_speech(text, filename, session_begin_params);
	if (MSP_SUCCESS != ret)
	{
		printf("text_to_speech failed, error code: %d.\n", ret);
	}
	printf("合成完毕\n");

exit:
	printf("按任意键退出 ...\n");
	//_getch();
	MSPLogout(); //退出登录

	return 0;
}

#endif

int generate(string convText, int n)
{
	int         ret = MSP_SUCCESS;
	const char* login_params = "appid = 01e9f2eb, work_dir = .";//登录参数,appid与msc库绑定,请勿随意改动
	/*
	* rdn:           合成音频数字发音方式
	* volume:        合成音频的音量
	* pitch:         合成音频的音调
	* speed:         合成音频对应的语速
	* voice_name:    合成发音人
	* sample_rate:   合成音频采样率
	* text_encoding: 合成文本编码格式
	*
	*/
	const char* session_begin_params = "engine_type = local, voice_name = xiaoyan, text_encoding = GB2312, tts_res_path = fo|res\\tts\\xiaoyan.jet;fo|res\\tts\\common.jet, sample_rate = 16000, speed = 35, volume = 50, pitch = 50, rdn = 2";
	//const char* filename = "tts_sample.wav"; //合成的语音文件名称
	string filename = fmt::format("myaudio_{}.wav", n);
	//const char* text = convText.c_str();
	/* 用户登录 */
	ret = MSPLogin(NULL, NULL, login_params); //第一个参数是用户名，第二个参数是密码，第三个参数是登录参数，用户名和密码可在http://www.xfyun.cn注册获取
	if (MSP_SUCCESS != ret)
	{
		printf("MSPLogin failed, error code: %d.\n", ret);
		goto exit;//登录失败，退出登录
	}


	/* 文本合成 */
	printf("开始合成 ...\n");
	ret = text_to_speech(convText.c_str(), filename.c_str(), session_begin_params);
	if (MSP_SUCCESS != ret)
	{
		printf("text_to_speech failed, error code: %d.\n", ret);
	}
	printf("合成完毕\n");

exit:
	printf("按任意键退出 ...\n");
	//_getch();
	MSPLogout(); //退出登录

	return 0;

}





DWORD  WINAPI  LoopThreadProc(LPVOID  lpParam);


// 开始监控掉线的矿机
void CxpgwinDlg::OnBnClickedStart()
{
	if(!m_isStart)
	{
		// 创建监控线程
		m_hLoopThread = ::CreateThread(NULL, 0, LoopThreadProc, this, 0, NULL);
		m_isStart = true;
		GetDlgItem(IDSTART)->SetWindowText(_T("监控中..."));
		GetDlgItem(IDSTART)->EnableWindow(0);
	}
	//else
	//{
	//	//m_hLoopThread
	//	if (::CloseHandle(m_hLoopThread))
	//	{
	//		GetDlgItem(IDSTART)->SetWindowText(_T("开始监控"));
	//		m_isStart = false;
	//	}
	//	else
	//	{
	//		AfxMessageBox(_T("停止失败,请重试!"));
	//	}
	//}
}


BOOL CheckIPReachable(const char* pszIpAddr)
{
	IPAddr ip = inet_addr(pszIpAddr);
	ULONG ulHopCount, ulRTT;

	BOOL  bRet = (BOOL)GetRTTAndHopCount(ip, &ulHopCount, 30/*最大hop数, 可自行设置*/, &ulRTT); //相当于  ping 
	return bRet;
}



DWORD  WINAPI  LoopThreadProc(LPVOID  lpParam)
{
	using namespace nlohmann;
	CxpgwinDlg  *pDlg = static_cast<CxpgwinDlg*>(lpParam);
	httplib::Client f2poolCli("https://api.f2pool.com");

	while (1)
	{
		// 获取参数设置
		int nOfflineTime = 10; // 离线报警阈值， 默认10分钟
		int nGapTime = 2; // 每次报警间隔时间, 默认一分钟
		{
			CString cstrOffTime;
			pDlg->GetDlgItem(IDC_OFFTIME)->GetWindowText(cstrOffTime);
			cstrOffTime.Trim();
			if (!cstrOffTime.IsEmpty())
			{
				nOfflineTime = _ttoi(cstrOffTime);
			}

			CString cstrGapTime;
			cstrGapTime.Trim();
			pDlg->GetDlgItem(IDC_GAPTIME)->GetWindowText(cstrGapTime);
			if (!cstrGapTime.IsEmpty())
			{
				nGapTime = _ttoi(cstrGapTime);
			}
		}

		// 获取监控钱包地址
		vector<string> vctWalletAddress;
		{
			CString cstrAddress;
			pDlg->GetDlgItem(IDC_ADDRESS)->GetWindowText(cstrAddress);
			cstrAddress.Trim();
			string strAddress = WCharToMByte(cstrAddress.GetBuffer());
			split(strAddress, vctWalletAddress, "\r\n");
		}

		// 要排除的机子编号
		set<string> setExceptWorker;
		{
			CString cstrExceptWorker;
			pDlg->GetDlgItem(IDC_EXCEPT)->GetWindowText(cstrExceptWorker);
			cstrExceptWorker.Trim();

			string strExceptWorker = WCharToMByte(cstrExceptWorker.GetBuffer());
			vector<string> vctExceptWorkers;
			split(strExceptWorker, vctExceptWorkers, ',');
			setExceptWorker.insert(vctExceptWorkers.begin(), vctExceptWorkers.end());
		}

		auto now = chrono::time_point_cast<std::chrono::seconds>(chrono::system_clock::now());
		string ts = date::format("%F-%T", now + chrono::hours(8));
		CWnd* pOutput = pDlg->GetDlgItem(IDC_OUTPUT);


		try
		{
			vector<pair<int, string>>  vctOfflineWorkers;
			int nOffline3060TiCount = 0; // 掉线的3060Ti
			int nOfflineXgpCount = 0; // 掉线的小钢炮数量
			const int IDX_NAME = 0; // 矿工名
			const int IDX_TIME = 6; // 最后提交时间

			int nIndex = 0;
			for (; nIndex < vctWalletAddress.size(); )
			{
				string address = vctWalletAddress[nIndex];
				auto res = f2poolCli.Get(fmt::format("/eth/{}", address).c_str());
				if (!res || 200 != res->status)
				{
					// 检查网关是否通
					string strGatewayIp = "192.168.1.1";
					if (!CheckIPReachable(strGatewayIp.c_str()))
					{
						// 网关不通
						string tipText = fmt::format("请注意, 网络异常:与网关{}的连接不通, 请检查本机网线是否插好!", strGatewayIp);
						pOutput->SetWindowText(StringToLPCWSTR(tipText));
						string strAudioText = "请注意, 网络异常: 拼不通网关,请检查本机网线是否插好!";

						// 播放音频
						generate(strAudioText, 0); // 语音合成
						PlaySound(_T("myaudio_0"), NULL, SND_FILENAME | SND_SYNC);
						::Sleep(60 * 1000);

						throw std::runtime_error(fmt::format("网络异常: 拼不通网关,{}, 请检查网线是否插好!", strGatewayIp));
					}

					// 检查网络连接是否正常
					if (!CheckIPReachable("114.114.114.114") && !CheckIPReachable("114.114.114.114"))
					{
						// 网络问题
						pOutput->SetWindowText(_T( "请注意，网络异常: 连接外网失败，请联系管理员检查网络！"));
						string strAudioText = "请注意，网络异常: 连接外网失败, 请联系管理员检查网络！";

						// 播放音频
						generate(strAudioText, 0); // 语音合成
						PlaySound(_T("myaudio_0"), NULL, SND_FILENAME | SND_SYNC);
						::Sleep(60 * 1000);

						throw std::runtime_error("网络异常: 连接外网失败, 请联系管理员检查网络！");
					}


					::Sleep(2 * 1000);
					continue;
				}
					

				// 转换为json
				json d = json::parse(res->body);
				auto workers = d["workers"];

				for (int i = 0; i < workers.size(); i++)
				{
					string workerName = workers[i][IDX_NAME];
					if (setExceptWorker.find(workerName) != setExceptWorker.end())
					{
						// 要排除的机子,直接跳过
						continue;
					}

					string datetime = workers[i][IDX_TIME];
					string trimDatetime = datetime.substr(0, datetime.find('.'));
					std::istringstream input(trimDatetime);
					date::sys_seconds tp;
					input >> date::parse("%FT%T", tp);
					auto last = chrono::time_point_cast<std::chrono::seconds>(tp);
					auto du = now - last;

					// 离线超过分钟
					if (int64_t(nOfflineTime) * 60 < du.count() && du.count() < 24 * 3600)
					{
						// 统计3060Ti 和 小钢炮 离线数量
						if (workerName.find("xgp") != string::npos) {
							nOfflineXgpCount++;
						}
						else {
							nOffline3060TiCount++;
						}
						if (du.count() < 60 * 60)
						{
							vctOfflineWorkers.push_back(make_pair<int, string>(du.count(), "[" + ts + "] - " + workerName + " " + fmt::format(", 离线{}分钟！", int(du.count() / 60))));
						}
						else if (3600 <= du.count())
						{
							char buf[100] = { 0 };
							memset(buf, 0, sizeof(buf));
							sprintf(buf, "%.1f", du.count() / 3600.0);
							vctOfflineWorkers.push_back(make_pair<int, string>(du.count(), "[" + ts + "] - " + workerName + "，离线" + string(buf) + "小时！"));
						}
					}
					// 离线超过24小时的,不进行报警
				}
			
				nIndex++;
			}

			// 最近掉线的排在前面
			sort(vctOfflineWorkers.begin(), vctOfflineWorkers.end(), [](pair<int, string>& a, pair<int, string>& b) {
				return a.first < b.first;
				});

			CString cstrOutput;
			if (vctOfflineWorkers.empty())
			{
				CString msg = _T("[") + CString(StringToLPCWSTR(ts)) + _T("] - ") + _T("恭喜！暂时没有小钢炮掉线。");
				pOutput->SetWindowText(msg);
			}
			else
			{
				for (auto name : vctOfflineWorkers)
				{
					cstrOutput += CString(name.second.c_str()) + _T("\r\n");
				}

				pOutput->SetWindowText(cstrOutput);

				if (nOffline3060TiCount > 0 || nOfflineXgpCount > 0)
				{
					string strAudioText = "请注意！";
					if (nOffline3060TiCount > 0) {

						string strChineseCount = convertInt2Chinese(nOffline3060TiCount);
						strAudioText += fmt::format("有{}台3060钛，有{}台3060钛,",
							strChineseCount, strChineseCount);
					}
					if (nOfflineXgpCount > 0) {
						string strChineseCount = convertInt2Chinese(nOfflineXgpCount);
						strAudioText += fmt::format("有{}台小钢炮，有{}台小钢炮，",
							strChineseCount, strChineseCount);
					}
					strAudioText += fmt::format("离线超过{}分钟，请及时处理！", convertInt2Chinese(nOfflineTime));

					// 播放音频
					generate(strAudioText, 0); // 语音合成
					PlaySound(_T("myaudio_0"), NULL, SND_FILENAME | SND_SYNC);
				}
			}

			::Sleep(1000 * 60 * nGapTime);
		}
		catch (exception& e)
		{
			CString cstrOld;
			cstrOld += CString(fmt::format("[{}] - {}, {}\r\n", ts.c_str(), "异常错误", e.what()).c_str());
			pOutput->SetWindowText(cstrOld);
		}
		catch (...)
		{
			CString cstrOld;
			cstrOld += CString(fmt::format("[{}] - {} \r\n", ts.c_str(), "未知错误" ).c_str());
			pOutput->SetWindowText(cstrOld);
		}
	}

	return 0;
}





void CxpgwinDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CHiveOn hive;

	// 获取输入的密码和谷歌验证码
	CString cstrEmail;
	CString cstrPasswd;
	CString cstrTwoFA;
	GetDlgItem(IDC_EMAIL)->GetWindowText(cstrEmail);
	GetDlgItem(IDC_PASSWD)->GetWindowText(cstrPasswd);
	GetDlgItem(IDC_TWOFA)->GetWindowText(cstrTwoFA);


	string login = WCharToMByte( cstrEmail.GetBuffer());
	string password = WCharToMByte(cstrPasswd.GetBuffer());
	string twofa_code = WCharToMByte(cstrTwoFA.GetBuffer());
	bool remember = true;

	try
	{
		string tmp = hive.login(login, password, twofa_code, remember);
		m_cstrAccessToken = tmp.c_str();
		OutputDebugString(m_cstrAccessToken);

		// 写入文件保存
		CFile file(_T("token.data"), CFile::modeCreate|CFile::modeWrite);
		file.Write(m_cstrAccessToken.GetBuffer(), m_cstrAccessToken.GetLength());
		file.Close();
	}
	catch (exception& e)
	{
		OutputDebugStringA(e.what());
		CString s(e.what());
		AfxMessageBox(s);
	}

}



void CxpgwinDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);


	// TODO: 在此处添加消息处理程序代码
	//我这里只是演示，所以写的控件少。
	vector<int> dlgitem = { IDC_OUTPUT, IDSTART, IDC_ADDRESS, 
		IDC_EXCEPT,IDC_TWOFA,IDC_PASSWD,IDC_EMAIL,IDC_LOGIN,IDC_OFFTIME,IDC_GAPTIME,
		IDC_STATIC2,
		IDC_STATIC3,
		IDC_STATIC5,
		IDC_STATIC6,
		IDC_STATIC7,
		IDC_STATIC8,
		IDC_STATIC9,
		IDC_STATICHIVEOS,
		IDC_STATICARGS,
	};
	for (int i = 0; i < dlgitem.size(); i++)//因为是多个控件，所以这里用了循环
	{
		CWnd* pWnd = GetDlgItem(dlgitem[i]);
		//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
		if (pWnd && nType != 1 && m_wndRect.Width() && m_wndRect.Height())
		{
			CRect rect;   //获取控件变化前的大小 
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
			rect.left = rect.left * cx / m_wndRect.Width();//调整控件大小
			rect.right = rect.right * cx / m_wndRect.Width();
			rect.top = rect.top * cy / m_wndRect.Height();
			rect.bottom = rect.bottom * cy / m_wndRect.Height();
			pWnd->MoveWindow(rect);//设置控件大小 
		}
	}
	//重新获得窗口尺寸
	GetClientRect(&m_wndRect);
}


void CxpgwinDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IDOK == MessageBox(_T("退出监控程序，将收不到离线语音提醒，是否继续退出？"), _T("提示"), MB_OKCANCEL))
	{
		CDialogEx::OnClose();
	}
}



