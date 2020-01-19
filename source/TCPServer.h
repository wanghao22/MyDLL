#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")

#define TCP_STR_LEN	100

class __declspec(dllexport) TCPServer
{
public:
	TCPServer();
	~TCPServer();
	//开始
	int Init(std::string &str);
	//关闭
	void Close();
	//发消息
	int sendMsg(SOCKET sock, std::string str);
	//发数据
	void SendData(SOCKET sock, BYTE* str, int len);
	//发数据
	void SendData(SOCKET sock, const char* str, int len);
	std::vector<sockaddr_in> m_client_ip;//客户端ip
	std::vector<SOCKET> m_clients;//客户端
	int m_connects;//连接个数
	int m_port;//端口
	bool m_connect_falg;//连接状态(用于显示ip)
	int m_recv_index;//发消息客户端的index
	BYTE m_recData[TCP_STR_LEN];//接受的数据
	//接受到的数据长度
	int m_str_len;
private:
	//等待客户端连接线程
	void WaitConnected(std::string &str);
	//接收消息线程
	void ThreadRecvData(SOCKET sock, std::string &str);
	SOCKET m_server;//服务端
	char recvBuff[TCP_STR_LEN];//接受的数据
	bool m_bThread_flag;//等待客户端连接线程标志位
};

