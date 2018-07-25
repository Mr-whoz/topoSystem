#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#define buf_size 1024

class Client
{
public:
	Client();
	~Client();
	void run(const char*host,const char*sendFile,const char*recvFile);
private:
	int recvall(SOCKET s, char *buf, int len) ;
	int sendall(SOCKET s, char *buf, int len) ;
	int sendsize8(SOCKET s, int size) ;
	int recvsize8(SOCKET s);
	int sendfile(SOCKET s, const char *file) ;
	int recvfile(SOCKET s, const char *file) ;

	WSADATA wd;
	SOCKET  s ;
	struct  sockaddr_in sin; 
	int     res;


};

#endif