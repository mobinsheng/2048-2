#ifndef _UTIL_H_
#define _UTIL_H_

#include "common.h"
#include <mutex>
/************************************************************************
*   工具类                                                                
************************************************************************/
class Util
{
public:

	// 获取区间[a,b)内的一个随机数
	static int				GetRandomNum(int a,int b);

	// 获取一个随机概率
	static float			GetProbability();

	// 把int转换成string
	static string						ConvertToStr(long long value)
	{
		char strBuffer[16] = {0};
		sprintf(strBuffer,"%d",value);
		return strBuffer;
	}

	// 把int转换成string
	static string						ConvertToStr(int value)
	{
		char strBuffer[16] = {0};
		sprintf(strBuffer,"%d",value);
		return strBuffer;
	}

	// 把double转换成string
	static string						ConvertToStr(double value)
	{
		char strBuffer[16] = {0};
		sprintf(strBuffer,"%lf",value);
		return strBuffer;
	}

	// 从sockaddr_in中获取IP地址
	static string GetIPString(sockaddr_in& addr)
	{
		return inet_ntoa(addr.sin_addr);
	}

	// 从sockaddr_in获取端口
	static u_short GetPort(sockaddr_in& addr)
	{
		return ntohs(addr.sin_port);
	}

	// 根据spilit_char切割字符串，结果放在vector中返回
	static vector<string>			Spilit(const char* str,int len,char spilit_char)
	{
		vector<string>					stringList;
		const int						nstrBufferLen = 32;
		int								nstrBufferIndex = 0;
		char							strBuffer[nstrBufferLen] = {0};

		for (int i = 0; i < len; ++i)
		{
			if (str[i] != spilit_char)
			{
				strBuffer[nstrBufferIndex++] = str[i];
			}
			else
			{
				//++i;
				stringList.push_back(strBuffer);
				memset(strBuffer,0,nstrBufferLen);
				nstrBufferIndex = 0;
			}
		}

		stringList.push_back(strBuffer);

		return stringList;
	}

	// 日志记录器
	static void Log(const char* format,...)
	{
		// 简单的日志类
		class CreateLogFile
		{
		public:
			CreateLogFile()
			{
				// 创建一个可写文件
				m_pFile = 0;
				m_strFileName = "";

				time_t ntime;
				tm* ptimeinfo;

				time(&ntime);
				ptimeinfo = localtime(&ntime);

				char szBuffer[1024] = {0};

				sprintf(szBuffer,"%4d-%02d-%02d",1900+ptimeinfo->tm_year, 1+ptimeinfo->tm_mon,
					ptimeinfo->tm_mday);

				m_strFileName = szBuffer;
				int index = GetRandomNum(0,100);
				string strIndex = ConvertToStr(index);
				//m_strFileName += "_";
				//m_strFileName += strIndex;
				m_strFileName += ".log";

				m_pFile = fopen(m_strFileName.c_str(),"w+");

			}
			void	Record(const string& str)
			{
				// 将信息写入到文件中
				if (m_pFile == 0)
				{
					return;
				}

				fwrite(str.c_str(),str.size(),1,m_pFile);
				fflush(m_pFile);
			}

			~CreateLogFile()
			{
				// 关闭文件
				if (m_pFile != 0)
				{
					fflush(m_pFile);
					fclose(m_pFile);
				}
			}

		private:
			string m_strFileName;		// 日志文件名字
			FILE* m_pFile;					// 文件指针

		};

		static CreateLogFile logfile; // static保证只初始化一次

		time_t ntime;
		tm* ptimeinfo;

		time(&ntime);								// 获取时间
		ptimeinfo = localtime(&ntime);		// 转换为本地时间

		char szBuffer[SZ_1024_LENGTH] = {0};

		// 在记录信息前面打上时间标记
		sprintf(szBuffer,"%4d-%02d-%02d %02d:%02d:%02d",1900+ptimeinfo->tm_year, 1+ptimeinfo->tm_mon,
			ptimeinfo->tm_mday,ptimeinfo->tm_hour,ptimeinfo->tm_min,ptimeinfo->tm_sec);

		string temp = "";
		temp = "[";
		temp += szBuffer;


		va_list info;

		va_start(info,format);
		vsnprintf(szBuffer,SZ_1024_LENGTH,format,info);
		//printf("%s",szBuffer);
#if (defined(WIN32) || defined(WIN64))
		strcat(szBuffer,"\r\n");
#else
		strcat(szBuffer,"\n");
#endif 
		temp += "]   ";
		temp += szBuffer;
		
		logfile.Record(temp);
	}

	static string	GetRandomString(int len)
	{
		if (len <= 0)
		{
			return "";
		}
		string strRet = "";
		for (int i = 0; i < len; ++i)
		{
			int x =  rand()%128;
			//int x = GetRandomNum(0,128);
			if ((x >= '0' && x <= '9') || (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
			{
				strRet += (char)(x);
			}
		}

		return strRet;
	}
};

//Util::Log("[%s] [%s] [Line:%d]\t",__TIME__,__FILE__,__LINE__);

#ifndef LOG_DEBUG
#define LOG_DEBUG \
	Util::Log
#endif

#endif //_UTIL_H_