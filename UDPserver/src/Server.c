#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#include "header.h"
#include "ServerLib.h"

void clearwinsock(){
#if defined WIN32
	WSACleanup();
#endif
}

int main(){
#if defined WIN32
	WSADATA wsadata;
	int iResult = WSAStartup(MAKEWORD(2,2),&wsadata);
	if(iResult !=0){
		printf("error at WSAStartup\n");
		return EXIT_FAILURE;
	}
#endif

	int sock;
	struct sockaddr_in s_Addr, c_Addr;
	int c_AddrLen;
	char buffer[BUFFSIZE];
	struct hostent *host;
	int recvMsgSize;

	//HOSTNAME RESOLUTION
	if((host=gethostbyname(HOSTNAME))==NULL){
		errorhandler("Name resolution error\n");
		return EXIT_FAILURE;
	}

	//SOCKET CREATION
	if ((sock= socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<0){
		errorhandler("socket() failed");
		clearwinsock(sock);
		return EXIT_FAILURE;
	}

	//SERVER ADDRESS CREATION
	memset(&s_Addr,0,sizeof(s_Addr));
	s_Addr.sin_family=AF_INET;
	s_Addr.sin_port=htons(PROTOPORT);
	memcpy(&s_Addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);  // hostname resolved in IP address


	//ADDRESS BIND
	if((bind(sock,(struct sockaddr*)&s_Addr,sizeof(s_Addr)))<0){
		errorhandler("bind() error");
		clearwinsock(sock);
		closesocket(sock);
		return EXIT_FAILURE;
	}
	printf("Server listening on %s, port %d...\n", HOSTNAME, PROTOPORT);

	//INFINITE LOOP OF READING AND WRITING
	while(1){
		c_AddrLen= sizeof(c_Addr);
		if((recvMsgSize= recvfrom(sock,buffer,BUFFSIZE,0,(struct sockaddr*)&c_Addr,&c_AddrLen))<0){
			errorhandler("client error or disconnection\n");
			continue;
		}
		printf("New Request from:%s:%d\n",inet_ntoa(s_Addr.sin_addr),PROTOPORT);

		// Message decomposition and choice of password generation function
	    char *del = " \0"; // Delimiters
		char *tipo = strtok(buffer, del);
		char *len = strtok(NULL, del);

		int LenNum = atoi(len); // Converts the second token to a number
		if (LenNum <= 0) {
			errorhandler("Invalid password length.\n");
			continue;
		}

		char password[LenNum + 1];

		// Password generation based on type
		switch (*tipo) {
			case 'a':
				generate_alpha(password, LenNum);
				break;
			case 'n':
				generate_numeric(password, LenNum);
				break;
			case 'm':
				generate_mixed(password, LenNum);
				break;
			case 's':
				generate_secure(password, LenNum);
				break;
			case 'u':
				generate_unambiguous(password, LenNum);
				break;
			default:
				errorhandler("Invalid password type.\n");
				continue;
		}

		if ((sendto(sock,password,LenNum,0,(struct sockaddr*)&c_Addr,sizeof(c_Addr)))!= LenNum){
			 errorhandler("sendto() sent different number of bytes than expected");
			 continue;
		}
		else{
			printf("sent %s successifull...\n",password);
			buffer[recvMsgSize] = '\0';
		}
	}
	//AFTER THE LOOP CLOSE THE SERVER SOCKET
    closesocket(sock);
	clearwinsock();
	return 0;
}
