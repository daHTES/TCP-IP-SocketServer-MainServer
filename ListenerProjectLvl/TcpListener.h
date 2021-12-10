#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#define MAX_BUFFER_SIZE (50000)
class CTcpListener;

typedef void (*MessageRecievedHandler)(CTcpListener* listener, int socketId, std::string msg);

class CTcpListener 
{
public:
	CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler);
	~CTcpListener();
	// Отправка смс клиенту
	void Send(int clientSocket, std::string msg);
		// инициа. сокета
	bool Init();

	//Главный процесс-цикл запуска
	void Run();


	void Cleanup();
private:
	// Создаем сокет
	SOCKET CreateSocket();

	// Ждем подключения
	SOCKET WaitForConnection(SOCKET listening);

	std::string  m_ipAddress;
	int  m_port;
	MessageRecievedHandler MessageReceived;
};

class TcpListener
{
};
