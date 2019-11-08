#pragma once
#include <WinSock2.h>  
#include <stdio.h>  
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib")  

//接发数据的最长长度
#define TCP_STR_LEN 100

class TCPClient
{
public:
	TCPClient();
	~TCPClient();
	//连接服务器，str：接受的字符串，flag：true->AfxMessageBox/false->TRACE
	bool Connect(std::string &str, bool flag = true);
	//返回获取字符串线程的std::thread类
	std::thread GetThreadRecv(SOCKET sock, std::string & str);
	//发送字符串
	void SendMsg(std::string str);
	//发送数据
	void SendData(const BYTE* data, int len);
	//发送数据
	void SendData(const char* data, int len);
	//断开连接
	void Close();
	//服务器地址
	std::string m_address;
	//服务器端口
	int m_port;
	//连接标志位
	bool m_connect_flag;
	//接受到的数据
	BYTE m_data[TCP_STR_LEN];
private:
	//获取字符串线程
	void ThreadRecvData(SOCKET sock, std::string & str);
	//接受到的数据
	char recvBuff[TCP_STR_LEN];
	//服务器SOCKET
	SOCKET m_socketServer;
};

