#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib , "ws2_32.lib")
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#include <vector>
#include <windows.h>
int cur_numleft=16;
int cur_num=7;
void gotoxy2(int x, int y){
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}

using namespace std;	// vector는 std:: 안에 있습니다

/*
recv는 받아오는 함수 send는 보내는 함수
*/

vector<SOCKET> clnt_list;    //vector만듬

int recvn2(SOCKET s, char *buf, int len, int flags)		// 메세지등 받아오는 것을 다 받을때 까지 리턴 안하는 함수...
{
	int received;		// 한번에 읽은 양..
	char *ptr = buf;
	int left = len;		//남은양..

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)		// 에러났으면 에러 리턴..
			return SOCKET_ERROR;
		else if(received == 0)				// 다 읽어서 읽은거 없으면 그냥 break;
			break;
		left -= received;					// 한번 읽은만큼을 남은 양에서 뺌..
		ptr += received;					// 한번 읽은만큼 저장할 버퍼를 옮김..
											//    ==> 똑같이 ptr에서 돌리면 덮어씌움...
											//          ==> ptr은 주소 저장할 공간의 시작주소
	}

	return (len - left);					// 길이에서 남은양을 뺀값을 리턴 (읽으라는 길이를 리턴하는거)
}

void __cdecl RecvThread2 (void * p)   //쓰레드를 이용 클라이언트 로부터 메세지등을 받아오는것
{
	SOCKET sock = (SOCKET)p;
	char buf[256];
	int count=0;
	
	int size;
	while(1)
	{
		
		
		//-----------클라이언트로부터 수신------------
		int recvsize = recvn2(sock, (char*)&size, sizeof(int),0);		// 사이즈를 먼저 읽고
		recvsize = recvn2(sock,buf,size,0);					// 그 사이즈만큼의 데이터 읽기
		if(recvsize <= 0)		break;
		//------------------------------------------------
		buf[recvsize] = '\0';
		gotoxy2(cur_numleft,cur_num);
		cur_num++;
		if(cur_numleft==16 && cur_num==25)
		{
			cur_numleft=50;
			cur_num=7;
			gotoxy2(cur_numleft,cur_num);
			for(count=0;count<18;count++){
				gotoxy2(cur_numleft,cur_num);
				printf("                            \n");
				cur_num++;
			}
			cur_numleft=50;
			cur_num=7;
		}
		else if(cur_numleft==50 && cur_num==25)
		{
			cur_numleft=16;
			cur_num=7;
			gotoxy2(cur_numleft,cur_num);
			for(count=0;count<18;count++){
				gotoxy2(cur_numleft,cur_num);
				printf("                            \n");
				cur_num++;
			}
			cur_numleft=16;
			cur_num=7;
		}
		gotoxy2(cur_numleft,cur_num);
		if(recvsize<=128)
			printf("%s\n",buf);
		//----------클라이언트에게 전송------------------
		for(int i = 0 ; i < clnt_list.size(); i++)
		{
			if(clnt_list[i] != sock)
			{
				int sendsize = send(clnt_list[i],(char*)&size,sizeof(int),0);		// 사이즈를 먼저 보내고
				sendsize = send(clnt_list[i],buf,strlen(buf),0);					// 그 사이즈만큼 데이터 보냄..
			}
		}
		//-----------------------------------------------
		
	}
	gotoxy2(cur_numleft,cur_num);
	printf("접속 종료\n");  //반복끝나면

	//------------vector에 있는 데이터 지우기-----------
	vector<SOCKET>::iterator iter = clnt_list.begin();
	for(int i = 0 ; i < clnt_list.size() ; i++)
	{
		if(clnt_list[i] == sock)
		{
			clnt_list.erase(iter);   //vector의 erase사용해서 지움
			break;
		}
		iter++;
	}
	//---------------------------------------------------

	//------------소켓 해제---------------------
	closesocket(sock);
	//----------------------------------------
}

int main_server()
{
	//-------소켓 라이브러리 불러오기(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2,2),&wsaData);
	
	if(retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}
	//-------------------------------------------

	//----------소켓 생성--------------
	SOCKET serv_sock;
	serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(serv_sock == SOCKET_ERROR)
	{
		printf("socket() Error\n");  //소켓생성이 제대로 안되면 error출력
		return 0;
	}
	//-----------------------------------

	//--------서버(자신)의 소켓 정보 입력------------
	SOCKADDR_IN serv_addr = {0};					// 초기화
	serv_addr.sin_family = AF_INET;					// IP 사용
	serv_addr.sin_port = htons(4000);				// 포트 4000번
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 주소는 알아서 찾게
	//------------------------------------------------

	//-----------자기자신 인터넷(?)에 연결---------------------
	retval = bind(serv_sock,(SOCKADDR*)&serv_addr,sizeof(SOCKADDR));
	if(retval == SOCKET_ERROR)
	{
		printf("bind() Error\n");   //bind제대로 안되면 error출력
		return 0;
	}
	//--------------------------------------------

	//-----------대기인원 설정-----------------
	listen(serv_sock,5);		// 5명까지만 대기할 수 있게 함...
	//-------------------------------------------
	SOCKADDR_IN clnt_addr = {0};
	int size = sizeof(SOCKADDR_IN);
	
	while(1)
	{
		
		//-------------클라이언트 접속 대기, connect를 하면 리턴함-------------
		SOCKET clnt_sock = accept(serv_sock,(SOCKADDR*)&clnt_addr,&size);
		if(clnt_sock == SOCKET_ERROR)
		{
			printf("accept() Error\n");
			continue;
		}
		//----------------------------------------------------------------------

		//----------------vector 에 데이터 넣기------------
		clnt_list.push_back(clnt_sock);
		//--------------------------------------------------
		gotoxy2(cur_numleft,cur_num);
		cur_num++;
		printf("클라이언트 접속\n");
		gotoxy2(cur_numleft,cur_num);
		cur_num++;
		printf("IP : %s, Port : %d\n",inet_ntoa(clnt_addr.sin_addr),clnt_addr.sin_port);
		//-----------수신 스레드 생성-------------
		_beginthread(RecvThread2,NULL,(void*)clnt_sock);
		//-----------------------------------------
	}
	//----------소켓 닫음---------------
	closesocket(serv_sock);
	//-----------------------------------

	//-------라이브러리 해제(?)---------
	WSACleanup();
	//----------------------------------
}