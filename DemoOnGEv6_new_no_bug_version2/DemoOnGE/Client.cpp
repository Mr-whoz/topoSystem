#include "Client.h"
#include <winsock2.h>
#include <Ws2tcpip.h>

#define buf_size 1024

Client::Client()
{
	s = INVALID_SOCKET;
}
Client::~Client()
{

}
void Client::run(const char*host,const char*sendFile,const char*recvFile)
{
	res = WSAStartup(MAKEWORD(2,2), &wd);
	if (res != NO_ERROR) {
		//QMessageBox::information(this, QObject::tr("提示"),QString(tr("WSAStartup failed")));
		return ;
	}
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		//QMessageBox::information(this, QObject::tr("提示"),QString(tr("Error at socket()")));
		WSACleanup();
		return ;
	}
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(host);
	sin.sin_port = htons(27015);

	res = connect(s, (SOCKADDR *)&sin, sizeof(sin));
	if (res == SOCKET_ERROR) {
		closesocket(s);
		WSACleanup();
		return ;
	}
	sendfile(s, sendFile);
	recvfile(s, recvFile);
	closesocket(s);
	WSACleanup();
}
int Client::recvall(SOCKET s, char *buf, int len) {
	int res, recvd;

	for (recvd = 0; recvd < len; recvd += res) {
		if ((res = recv(s, buf + recvd, min(len - recvd, buf_size), 0)) <= 0) {
			break;
		}
	}
	return recvd;
}

int Client::sendall(SOCKET s, char *buf, int len) {
	int res, sent;

	for (sent = 0; sent < len; sent += res) {
		if ((res = send(s, buf + sent, min(len - sent, buf_size), 0)) <= 0) {
			break;
		}
	}
	return sent;
}

int Client::sendsize8(SOCKET s, int size) {
	char buf[16];

	sprintf(buf, "%8u", size);
	return sendall(s, buf, 8);
}

int Client::recvsize8(SOCKET s) {
	char buf[16];

	if (8 != recvall(s, buf, 8)) {
		return 0;
	}
	return atoi(buf);
}

int Client::sendfile(SOCKET s, const char *file) {
	unsigned int numread, filesize, sendsize;
	char     buf[buf_size];
	FILE    *f;

	f = fopen(file, "rb");
	if (f == NULL) {
		return 0;
	}
	if (fseek(f, 0, SEEK_END)) {
		fclose(f);
		return 0;
	}
	filesize = ftell(f);
	if (filesize == -1) {
		fclose(f);
		return 0;
	}
	if (fseek(f, 0, SEEK_SET)) {
		fclose(f);
		return 0;
	}
	if (8 != sendsize8(s, filesize)) {
		return 0;
	}
	for (sendsize = 0; sendsize < filesize; sendsize += numread) {
		numread = min(filesize - sendsize, sizeof(buf));
		if (numread != fread(buf, 1, numread, f)) {
			break;
		}
		if (8 != sendsize8(s, numread)) {
			break;
		}
		if (numread != sendall(s, buf, numread)) {
			break;
		}
	}

	fclose(f);
	return filesize == sendsize;
}

int Client::recvfile(SOCKET s, const char *file) {
	unsigned int n, len, filesize, recvsize, numwritten;
	char     buf[buf_size];
	FILE    *f;

	f = fopen(file, "wb");
	if (f == NULL) {
		return 0;
	}
	if ((filesize = recvsize8(s)) == 0) {
		return 0;
	}
	for (recvsize = 0; recvsize < filesize; recvsize += numwritten) {
		if ((len = recvsize8(s)) == 0) {
			break;
		}
		for (numwritten = 0; numwritten < len; numwritten += n) {
			n = min(len - numwritten, sizeof(buf));
			if (n != recvall(s, buf, n)) {
				break;
			}
			if (n != fwrite(buf, 1, n, f)) {
				break;
			}
		}
		if (len != numwritten) break;
	}

	fclose(f);
	return filesize == recvsize;
}