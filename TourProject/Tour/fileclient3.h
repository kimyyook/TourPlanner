#pragma comment (lib, "ws2_32.lib") 
#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <process.h>
#include <fcntl.h>

/*
recv는 받아오는 함수 send는 보내는 함수
*/

int filerecvn(SOCKET s, char *buf, int len, int flags)		// 메세지등 받아오는 것을 다 받을때 까지 리턴 안하는 함수...
{
	int received;		// 한번에 읽은 양
	char *ptr = buf;
	int left = len;		//남은양

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)		// 에러났으면 에러 리턴..
			return SOCKET_ERROR;
		else if(received == 0)				// 다 읽어서 읽은거 없으면 그냥 break;
			break;
		left -= received;					// 한번 읽은만큼을 남은 양에서 뺌
		ptr += received;					// 한번 읽은만큼 저장할 버퍼를 옮김
											//    ==> 똑같이 ptr에서 돌리면 덮어씌움
											//          ==> ptr은 주소 저장할 공간의 시작주소
	}

	return (len - left);					// 길이에서 남은양을 뺀값을 리턴 (읽으라는 길이를 리턴하는거)
}

void __cdecl fileRecvThread (void * p)  //쓰레드를 이용 서버로부터 메세지등을 받아오는것
{
	SOCKET sock = (SOCKET)p; //소켓설정
	char buf[2048];
	int size;
	while(1)
	{
		//-----------서버로부터 수신------------
		int recvsize = recv(sock,(char*)&size,sizeof(buf),0);
		recvsize = recv(sock,buf,size,0);
		if(recvsize <= 0)
		{
			printf("접속종료\n");
			break;
		}
		//------------------------------------------------
		buf[recvsize] = '\0';
		printf("%s\n",buf);
		printf("전송성공!!!!\n");
	}
}

int main_fileclient(char *name)
{
	//-------소켓 라이브러리 불러오기(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}
	//------------------------------------------

	//---------소켓생성-------- 
	SOCKET serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	//TCP를 이용한 소켓
	//-------------------------

	//---------서버 정보 입력--------------------
	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;						// IP주소를 이용하고
	serv_addr.sin_port = htons(4000);					// 소켓은 4000번에
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// 서버의 ip 주소는 127.0.0.1 (자기 컴퓨터)
	//--------------------------------------------

	//---------서버 연결------------
	retval = connect(serv_sock,(SOCKADDR*)&serv_addr,sizeof(SOCKADDR));
	if(retval == SOCKET_ERROR)
	{
		printf("connect() Error\n");
		return 0;
	}
	//-------------------------------

	FILE* fStream; // 파일을 읽기 쓰기 모드로 생성
	//-------서버로 이름을 보내기 예시로 만든거임 name파일은 내가 만들어서 컴터에 있는거 ---------
	char nick[20] ={'\0'};
	strcpy(nick,name);
	
    printf("이름 : %s\n", nick);
	printf("엔터를 누르면 전송\n");
	_beginthread(fileRecvThread,0,(void*)serv_sock);
	//---------------------------------------
	while(1) //무한반복
	{
		int ch;
		FILE* fStream; // 파일을 읽기 쓰기 모드로 생성
		char trans[2048] = {0};
		char buf2[2048] = {0};
		int filesize = 0;
		ch=getch();
		//---------서버에 메시지 전송---------------

		//------------------------------------------
		if(ch==13)
		{
		//서버에 파일전송 예시로 만든거임 trans파일은 내가 임의로 만든거	
		fStream=fopen("result.txt","r");
         fread(trans, 1024, 1, fStream);
		 sprintf(buf2,"%s",trans);
		 filesize = strlen(buf2);
		 int sendsize = send(serv_sock,(char*)&filesize,sizeof(int),0);
		 sendsize = send(serv_sock,buf2,filesize,0);
		 if(sendsize <= 0)
			break;

		}
		else
			break;

	}

	//----------소켓 닫음------------------
	closesocket(serv_sock);
	WSACleanup();
	//-------------------------------------
}