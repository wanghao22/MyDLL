#pragma once
#include <WinSock2.h>  
#include <stdio.h>  
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib")  

//�ӷ����ݵ������
#define TCP_STR_LEN 100

class TCPClient
{
public:
	TCPClient();
	~TCPClient();
	//���ӷ�������str�����ܵ��ַ�����flag��true->AfxMessageBox/false->TRACE
	bool Connect(std::string &str, bool flag = true);
	//���ػ�ȡ�ַ����̵߳�std::thread��
	std::thread GetThreadRecv(SOCKET sock, std::string & str);
	//�����ַ���
	void SendMsg(std::string str);
	//��������
	void SendData(const BYTE* data, int len);
	//��������
	void SendData(const char* data, int len);
	//�Ͽ�����
	void Close();
	//��������ַ
	std::string m_address;
	//�������˿�
	int m_port;
	//���ӱ�־λ
	bool m_connect_flag;
	//���ܵ�������
	BYTE m_data[TCP_STR_LEN];
private:
	//��ȡ�ַ����߳�
	void ThreadRecvData(SOCKET sock, std::string & str);
	//���ܵ�������
	char recvBuff[TCP_STR_LEN];
	//������SOCKET
	SOCKET m_socketServer;
};

