# TCP/IP

_Used [WinSock2](<https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2?redirectedfrom=MSDN>)_
***
> ## Client:
>
> * Connect to Server and Receive Message:
>
> ```c++
> TCPClient m_client;//Init client
> m_client.m_address = "127.0.0.1";//input your Server Address
> m_client.m_port = 3000;//input your Server Port
> std::string m_revMsg;
> m_client.Connect(m_revMsg);//Connect to Server and Receive Message
> ```
>
>  * Send Message To Server:
>
> ```c++
> std::string m_sendMsg = "input your Send Message";
> m_client.SendMsg(m_sendMsg);//Send Message To Server
> ```
>
> * Close  The Connect:
>
> ```c++
> m_client.Close();
> ```

---

> ## Server:
>
> * Start Server and Listening:
>
> ```c++
> TCPServer m_server;//Init server
> m_server.m_port = 3000;//Open Server Port
> m_server.Init(m_revMsg);//Start Server and Listening
> ```
>
> * Send Message To Client:
>
> ```c++
> SOCKET client;//the client who get message
> std::string m_sendMsg = "input your Send Message";
> m_server.sendMsg(client,m_sendMsg);
> ```
>
> * Close The Server:
>
> ```c++
> m_server.Close();
> ```

---

