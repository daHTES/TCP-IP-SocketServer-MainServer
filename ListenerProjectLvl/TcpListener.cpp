#include "TcpListener.h"
#include <iostream>

CTcpListener::CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler)
	: m_ipAddress(ipAddress), m_port(m_port), MessageReceived(handler)
{

}
CTcpListener::~CTcpListener()
{
	Cleanup();
}
	// Отправка смс клиенту
void CTcpListener::Send(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}
	// инициа. сокета
bool CTcpListener::Init()
{
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsInit = WSAStartup(ver, &data);
	return wsInit == 0;
}

	//Главный процесс-цикл запуска
void CTcpListener::Run()
{

	char buf[MAX_BUFFER_SIZE];
	while (true) 
	{
		// Создаем сокет
		SOCKET listening = CreateSocket();
		if (listening == INVALID_SOCKET) 
		{
			std::cout << "Error Invalid";
			break;
		}
		SOCKET client = WaitForConnection(listening);
		if (client != INVALID_SOCKET)
		{
			closesocket(listening);
			int bytesReceived = 0;
			do 
			{
				ZeroMemory(buf, MAX_BUFFER_SIZE);

				bytesReceived = recv(client, buf, MAX_BUFFER_SIZE, 0);
				if (bytesReceived > 0) 
				{
					if (MessageReceived != NULL)
					{
						MessageReceived(this, client, std::string(buf, 0, bytesReceived));
					}
				}
			} while (bytesReceived > 0);
			closesocket(client);
		}
	}
}

void CTcpListener::Cleanup()
{
	WSACleanup();
}

	// Создаем сокет
SOCKET CTcpListener::CreateSocket()
{
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening != INVALID_SOCKET)
	{
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

		int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
		if (bindOk != SOCKET_ERROR) 
		{
			int listenOk = listen(listening, SOMAXCONN);

			if (listenOk == SOCKET_ERROR) 
			{
				std::cout << "Error";
				return -1;
			}
		}
		else
		{
			std::cout << "Error";
			return -1;
		}
	}
	return listening;
}

	// Ждем подключения
SOCKET CTcpListener::WaitForConnection(SOCKET listening)
{
	SOCKET client = accept(listening, NULL, NULL);
	return client;
}


