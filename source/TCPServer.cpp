#include "stdafx.h"
#include "TCPServer.h"

using namespace std;

TCPServer::TCPServer()
{
	m_bThread_flag = true;
	m_connect_falg = false;
	m_connects = 0;
	memset(m_recData, 0, TCP_STR_LEN);
}


TCPServer::~TCPServer()
{
	Close();
}

int TCPServer::Init(string &str)
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		//str = "初始化WSA失败！TCP连接失败！";
		return -1;
	}
	//创建套接字
	m_server = socket(AF_INET, SOCK_STREAM, 0);
	if (m_server == INVALID_SOCKET)
	{
		m_connects = 0;
		//str = "创建套接字失败！TCP连接失败！";
		return -2;
	}

	//绑定IP和端口
	u_short port = u_short(m_port);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(m_server, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		m_connects = 0;
		//str = "绑定IP和端口失败！TCP连接失败！";
		return -3;
	}

	//开始监听
	if (listen(m_server, 5) == SOCKET_ERROR)
	{
		m_connects = 0;
		//str = "开启监听失败！TCP连接失败！";
		return -4;
	}

	m_bThread_flag = true;
	thread task(&TCPServer::WaitConnected, this, std::ref(str));
	task.detach();

	return 0;
}

void TCPServer::Close()
{
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		shutdown(m_clients[i], 2);
		closesocket(m_clients[i]);
	}
	closesocket(m_server);
	m_clients.clear();
	m_client_ip.clear();
	m_bThread_flag = false;
	m_connects = 0;
	//WSACleanup();
}

int TCPServer::sendMsg(SOCKET sock, std::string str)
{
	if (m_connects <= 0)
		return -1;
	int rst = send(sock, str.c_str(), str.length(), 0);
	if (rst == SOCKET_ERROR)
	{
		//str = "消息发送失败";
		return -1;
	}
	return 0;
}

void TCPServer::SendData(SOCKET sock, BYTE * str, int len)
{
	if (m_connects > 0)
		send(sock, (char*)str, len, 0);
}

void TCPServer::SendData(SOCKET sock, const char * str, int len)
{
	if (m_connects > 0)
		send(sock, str, len, 0);
}

void TCPServer::WaitConnected(std::string &str)
{
	sockaddr_in remoteAddr;
	HANDLE hThread;
	while (m_bThread_flag)
	{
		int nAddrlen = sizeof(remoteAddr);
		SOCKET socketClient = accept(m_server, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (socketClient == INVALID_SOCKET)
		{
			//str = "接受数据失败！";
			m_connects = 0;
			break;
		}
		else
		{
			m_client_ip.push_back(remoteAddr);
			m_clients.push_back(socketClient);
			m_connects++;
			m_connect_falg = true;
			//发送一个消息显示客户端连接
			//str = "客户端" + string(inet_ntoa(remoteAddr.sin_addr)) + "已接入";
			//创建接收数据线程
			thread task(&TCPServer::ThreadRecvData, this, socketClient, std::ref(str));
			hThread = task.native_handle();
			task.detach();
		}
	}
}

void TCPServer::ThreadRecvData(SOCKET sock, std::string &str)
{
	int  ret;
	while (m_connects > 0)
	{
		memset(recvBuff, 0, TCP_STR_LEN);
		ret = recv(sock, recvBuff, TCP_STR_LEN, 0);
		if (SOCKET_ERROR == ret || INVALID_SOCKET == ret || 0 == recvBuff[0] || strlen(recvBuff) == 0)
		{
			break;
		}
		//发送消息显示接收到的消息
		m_str_len = ret;
		str = string(recvBuff);
		memcpy(m_recData, recvBuff, TCP_STR_LEN);
		for (int i = 0; i < m_clients.size(); i++)
		{
			if (m_clients[i] == sock)
				m_recv_index = i;
		}
	}
	m_connects--;
	m_connect_falg = true;
	int index;
	for (int i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i] == sock)
			index = i;
	}
	if(m_clients.size()>0)
		m_clients.erase(m_clients.begin() + index);
	if(m_client_ip.size()>0)
		m_client_ip.erase(m_client_ip.begin() + index);
	//发送一个消息显示客户端断开
	//str = "客户端" + string(inet_ntoa(remoteAddr.sin_addr)) + "已断开";
	return;
}
