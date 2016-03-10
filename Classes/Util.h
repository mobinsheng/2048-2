#ifndef _UTIL_H_
#define _UTIL_H_

#include "common.h"
#include <mutex>
/************************************************************************
*   ������                                                                
************************************************************************/
class Util
{
public:

	// ��ȡ����[a,b)�ڵ�һ�������
	static int				GetRandomNum(int a,int b);

	// ��ȡһ���������
	static float			GetProbability();

	// ��intת����string
	static string						ConvertToStr(long long value)
	{
		char strBuffer[16] = {0};
		sprintf(strBuffer,"%d",value);
		return strBuffer;
	}

	// ��intת����string
	static string						ConvertToStr(int value)
	{
		char strBuffer[16] = {0};
		sprintf(strBuffer,"%d",value);
		return strBuffer;
	}

	// ��doubleת����string
	static string						ConvertToStr(double value)
	{
		char strBuffer[16] = {0};
		sprintf(strBuffer,"%lf",value);
		return strBuffer;
	}

	// ��sockaddr_in�л�ȡIP��ַ
	static string GetIPString(sockaddr_in& addr)
	{
		return inet_ntoa(addr.sin_addr);
	}

	// ��sockaddr_in��ȡ�˿�
	static u_short GetPort(sockaddr_in& addr)
	{
		return ntohs(addr.sin_port);
	}

	// ����spilit_char�и��ַ������������vector�з���
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

	// ��־��¼��
	static void Log(const char* format,...)
	{
		// �򵥵���־��
		class CreateLogFile
		{
		public:
			CreateLogFile()
			{
				// ����һ����д�ļ�
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
				// ����Ϣд�뵽�ļ���
				if (m_pFile == 0)
				{
					return;
				}

				fwrite(str.c_str(),str.size(),1,m_pFile);
				fflush(m_pFile);
			}

			~CreateLogFile()
			{
				// �ر��ļ�
				if (m_pFile != 0)
				{
					fflush(m_pFile);
					fclose(m_pFile);
				}
			}

		private:
			string m_strFileName;		// ��־�ļ�����
			FILE* m_pFile;					// �ļ�ָ��

		};

		static CreateLogFile logfile; // static��ֻ֤��ʼ��һ��

		time_t ntime;
		tm* ptimeinfo;

		time(&ntime);								// ��ȡʱ��
		ptimeinfo = localtime(&ntime);		// ת��Ϊ����ʱ��

		char szBuffer[SZ_1024_LENGTH] = {0};

		// �ڼ�¼��Ϣǰ�����ʱ����
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