#include "stdafx.h"
#include "TCPClient.h"

using namespace std;

TCPClient::TCPClient()
{
	m_connect_flag = false;
	memset(m_data, 0x00, TCP_STR_LEN);
}


TCPClient::~TCPClient()
{
	Close();
}


bool TCPClient::Connect(std::string &str, bool flag)
{
	//加载套接字  
	WSADATA wsaData;
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return false;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_port);
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_address.c_str());

	//创建套接字  
	m_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == m_socketServer)
	{
		return false;
	}

	//向服务器发出连接请求  
	if (connect(m_socketServer, (struct  sockaddr*)&addrSrv, sizeof(addrSrv)) == SOCKET_ERROR)
	{
		return false;
	}

	m_connect_flag = true;
	std::thread task = GetThreadRecv(m_socketServer, str);
	task.detach();

	return true;
}

void TCPClient::ThreadRecvData(SOCKET sock, std::string &str)
{
	int  ret;
	while (m_connect_flag)
	{
		memset(recvBuff, 0, TCP_STR_LEN);
		ret = recv(sock, recvBuff, TCP_STR_LEN, 0);
		if (SOCKET_ERROR == ret || INVALID_SOCKET == ret || 0 == recvBuff[0] || strlen(recvBuff) == 0)
		{
			m_connect_flag = false;
			break;
		}
		m_str_len = ret;
		str = string(recvBuff);
		memcpy(m_data, recvBuff, TCP_STR_LEN);
	}
	return;
}

std::thread TCPClient::GetThreadRecv(SOCKET  sock, std::string &str)
{//return是因为运行时要获取线程句柄
	return std::thread(&TCPClient::ThreadRecvData, this, sock, std::ref(str));
}

void TCPClient::SendMsg(std::string str)
{
	if (m_connect_flag)
		send(m_socketServer, str.c_str(), str.length(), 0);
}

void TCPClient::SendData(const BYTE * data, int len)
{
	if (m_connect_flag)
		send(m_socketServer, (char*)data, len, 0);
}

void TCPClient::SendData(const char * data, int len)
{
	if (m_connect_flag)
		send(m_socketServer, data, len, 0);
}

void TCPClient::Close()
{
	closesocket(m_socketServer);
	//WSACleanup();
	m_connect_flag = false;
}