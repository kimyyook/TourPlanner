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

using namespace std;	// vector�� std:: �ȿ� �ֽ��ϴ�

/*
recv�� �޾ƿ��� �Լ� send�� ������ �Լ�
*/

vector<SOCKET> clnt_list;    //vector����

int recvn2(SOCKET s, char *buf, int len, int flags)		// �޼����� �޾ƿ��� ���� �� ������ ���� ���� ���ϴ� �Լ�...
{
	int received;		// �ѹ��� ���� ��..
	char *ptr = buf;
	int left = len;		//������..

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)		// ���������� ���� ����..
			return SOCKET_ERROR;
		else if(received == 0)				// �� �о ������ ������ �׳� break;
			break;
		left -= received;					// �ѹ� ������ŭ�� ���� �翡�� ��..
		ptr += received;					// �ѹ� ������ŭ ������ ���۸� �ű�..
											//    ==> �Ȱ��� ptr���� ������ �����...
											//          ==> ptr�� �ּ� ������ ������ �����ּ�
	}

	return (len - left);					// ���̿��� �������� ������ ���� (������� ���̸� �����ϴ°�)
}

void __cdecl RecvThread2 (void * p)   //�����带 �̿� Ŭ���̾�Ʈ �κ��� �޼������� �޾ƿ��°�
{
	SOCKET sock = (SOCKET)p;
	char buf[256];
	int count=0;
	
	int size;
	while(1)
	{
		
		
		//-----------Ŭ���̾�Ʈ�κ��� ����------------
		int recvsize = recvn2(sock, (char*)&size, sizeof(int),0);		// ����� ���� �а�
		recvsize = recvn2(sock,buf,size,0);					// �� �����ŭ�� ������ �б�
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
		//----------Ŭ���̾�Ʈ���� ����------------------
		for(int i = 0 ; i < clnt_list.size(); i++)
		{
			if(clnt_list[i] != sock)
			{
				int sendsize = send(clnt_list[i],(char*)&size,sizeof(int),0);		// ����� ���� ������
				sendsize = send(clnt_list[i],buf,strlen(buf),0);					// �� �����ŭ ������ ����..
			}
		}
		//-----------------------------------------------
		
	}
	gotoxy2(cur_numleft,cur_num);
	printf("���� ����\n");  //�ݺ�������

	//------------vector�� �ִ� ������ �����-----------
	vector<SOCKET>::iterator iter = clnt_list.begin();
	for(int i = 0 ; i < clnt_list.size() ; i++)
	{
		if(clnt_list[i] == sock)
		{
			clnt_list.erase(iter);   //vector�� erase����ؼ� ����
			break;
		}
		iter++;
	}
	//---------------------------------------------------

	//------------���� ����---------------------
	closesocket(sock);
	//----------------------------------------
}

int main_server()
{
	//-------���� ���̺귯�� �ҷ�����(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2,2),&wsaData);
	
	if(retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}
	//-------------------------------------------

	//----------���� ����--------------
	SOCKET serv_sock;
	serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(serv_sock == SOCKET_ERROR)
	{
		printf("socket() Error\n");  //���ϻ����� ����� �ȵǸ� error���
		return 0;
	}
	//-----------------------------------

	//--------����(�ڽ�)�� ���� ���� �Է�------------
	SOCKADDR_IN serv_addr = {0};					// �ʱ�ȭ
	serv_addr.sin_family = AF_INET;					// IP ���
	serv_addr.sin_port = htons(4000);				// ��Ʈ 4000��
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// �ּҴ� �˾Ƽ� ã��
	//------------------------------------------------

	//-----------�ڱ��ڽ� ���ͳ�(?)�� ����---------------------
	retval = bind(serv_sock,(SOCKADDR*)&serv_addr,sizeof(SOCKADDR));
	if(retval == SOCKET_ERROR)
	{
		printf("bind() Error\n");   //bind����� �ȵǸ� error���
		return 0;
	}
	//--------------------------------------------

	//-----------����ο� ����-----------------
	listen(serv_sock,5);		// 5������� ����� �� �ְ� ��...
	//-------------------------------------------
	SOCKADDR_IN clnt_addr = {0};
	int size = sizeof(SOCKADDR_IN);
	
	while(1)
	{
		
		//-------------Ŭ���̾�Ʈ ���� ���, connect�� �ϸ� ������-------------
		SOCKET clnt_sock = accept(serv_sock,(SOCKADDR*)&clnt_addr,&size);
		if(clnt_sock == SOCKET_ERROR)
		{
			printf("accept() Error\n");
			continue;
		}
		//----------------------------------------------------------------------

		//----------------vector �� ������ �ֱ�------------
		clnt_list.push_back(clnt_sock);
		//--------------------------------------------------
		gotoxy2(cur_numleft,cur_num);
		cur_num++;
		printf("Ŭ���̾�Ʈ ����\n");
		gotoxy2(cur_numleft,cur_num);
		cur_num++;
		printf("IP : %s, Port : %d\n",inet_ntoa(clnt_addr.sin_addr),clnt_addr.sin_port);
		//-----------���� ������ ����-------------
		_beginthread(RecvThread2,NULL,(void*)clnt_sock);
		//-----------------------------------------
	}
	//----------���� ����---------------
	closesocket(serv_sock);
	//-----------------------------------

	//-------���̺귯�� ����(?)---------
	WSACleanup();
	//----------------------------------
}