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
	//��ʼ��WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		//str = "��ʼ��WSAʧ�ܣ�TCP����ʧ�ܣ�";
		return -1;
	}
	//�����׽���
	m_server = socket(AF_INET, SOCK_STREAM, 0);
	if (m_server == INVALID_SOCKET)
	{
		m_connects = 0;
		//str = "�����׽���ʧ�ܣ�TCP����ʧ�ܣ�";
		return -2;
	}

	//��IP�Ͷ˿�
	u_short port = u_short(m_port);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(m_server, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		m_connects = 0;
		//str = "��IP�Ͷ˿�ʧ�ܣ�TCP����ʧ�ܣ�";
		return -3;
	}

	//��ʼ����
	if (listen(m_server, 5) == SOCKET_ERROR)
	{
		m_connects = 0;
		//str = "��������ʧ�ܣ�TCP����ʧ�ܣ�";
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
		//str = "��Ϣ����ʧ��";
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
			//str = "��������ʧ�ܣ�";
			m_connects = 0;
			break;
		}
		else
		{
			m_client_ip.push_back(remoteAddr);
			m_clients.push_back(socketClient);
			m_connects++;
			m_connect_falg = true;
			//����һ����Ϣ��ʾ�ͻ�������
			//str = "�ͻ���" + string(inet_ntoa(remoteAddr.sin_addr)) + "�ѽ���";
			//�������������߳�
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
		//������Ϣ��ʾ���յ�����Ϣ
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
	//����һ����Ϣ��ʾ�ͻ��˶Ͽ�
	//str = "�ͻ���" + string(inet_ntoa(remoteAddr.sin_addr)) + "�ѶϿ�";
	return;
}
