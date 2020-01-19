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
	//��ʼ
	int Init(std::string &str);
	//�ر�
	void Close();
	//����Ϣ
	int sendMsg(SOCKET sock, std::string str);
	//������
	void SendData(SOCKET sock, BYTE* str, int len);
	//������
	void SendData(SOCKET sock, const char* str, int len);
	std::vector<sockaddr_in> m_client_ip;//�ͻ���ip
	std::vector<SOCKET> m_clients;//�ͻ���
	int m_connects;//���Ӹ���
	int m_port;//�˿�
	bool m_connect_falg;//����״̬(������ʾip)
	int m_recv_index;//����Ϣ�ͻ��˵�index
	BYTE m_recData[TCP_STR_LEN];//���ܵ�����
	//���ܵ������ݳ���
	int m_str_len;
private:
	//�ȴ��ͻ��������߳�
	void WaitConnected(std::string &str);
	//������Ϣ�߳�
	void ThreadRecvData(SOCKET sock, std::string &str);
	SOCKET m_server;//�����
	char recvBuff[TCP_STR_LEN];//���ܵ�����
	bool m_bThread_flag;//�ȴ��ͻ��������̱߳�־λ
};

