#pragma comment (lib, "ws2_32.lib") 
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#include <fcntl.h>
int count=0;
int cur_numle=16;
int cur_numri=7;
void gotoxy3(int x, int y){
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}

/*
recv�� �޾ƿ��� �Լ� send�� ������ �Լ�
*/

int recvn(SOCKET s, char *buf, int len, int flags)		// �޼����� �޾ƿ��� ���� �� ������ ���� ���� ���ϴ� �Լ�...
{
	
	int received;		// �ѹ��� ���� ��
	char *ptr = buf;
	int left = len;		//������

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)		// ���������� ���� ����..
			return SOCKET_ERROR;
		else if(received == 0)				// �� �о ������ ������ �׳� break;
			break;
		left -= received;					// �ѹ� ������ŭ�� ���� �翡�� ��
		ptr += received;					// �ѹ� ������ŭ ������ ���۸� �ű�
											//    ==> �Ȱ��� ptr���� ������ �����
											//          ==> ptr�� �ּ� ������ ������ �����ּ�
	}

	return (len - left);					// ���̿��� �������� ������ ���� (������� ���̸� �����ϴ°�)
}

void __cdecl RecvThread (void * p)  //�����带 �̿� �����κ��� �޼������� �޾ƿ��°�
{
	SOCKET sock = (SOCKET)p; //���ϼ���
	char buf[256];
	int size;
	while(1)
	{
		//-----------�����κ��� ����------------
		int recvsize = recv(sock,(char*)&size,sizeof(int),0);
		recvsize = recv(sock,buf,size,0);
		if(recvsize <= 0)
		{
			gotoxy3(cur_numle, cur_numri);
			printf("��������\n");
			cur_numri++;
			break;
		}
		//------------------------------------------------
		buf[recvsize] = '\0';
		
		cur_numri++;
		gotoxy3(cur_numle,cur_numri);
		printf("%s>>",buf);
		cur_numri++;
		gotoxy3(cur_numle,cur_numri);
		
		
	}
}

int main_client(char *name)
{
	//-------���� ���̺귯�� �ҷ�����(?)--------

	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}
	//------------------------------------------

	//---------���ϻ���-------- 
	SOCKET serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	//TCP�� �̿��� ����
	//-------------------------

	//---------���� ���� �Է�--------------------
	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;						// IP�ּҸ� �̿��ϰ�
	serv_addr.sin_port = htons(4000);					// ������ 4000����
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// ������ ip �ּҴ� 127.0.0.1 (�ڱ� ��ǻ��)
	//--------------------------------------------

	//---------���� ����------------
	retval = connect(serv_sock,(SOCKADDR*)&serv_addr,sizeof(SOCKADDR));
	if(retval == SOCKET_ERROR)
	{
		printf("connect() Error\n");
		return 0;
	}
	//-------------------------------

	FILE* fStream; // ������ �б� ���� ���� ����
	//-------������ �̸��� ������ ���÷� ������� name������ ���� ���� ���Ϳ� �ִ°� ---------
	char nick[20] = {'\0'};
	strcpy(nick,name);
	gotoxy3(cur_numle, cur_numri);
         printf("�̸� : %s\n", nick);
		 cur_numri++;
	_beginthread(RecvThread,0,(void*)serv_sock);
	//---------------------------------------

	while(1) //���ѹݺ�
	{
		char buf[256] = {0};
		char str[256];
		int size = 0;


		gotoxy3(cur_numle,cur_numri);
		printf(">> ");
		gets(str);

		if(strcmp(str,"exit")==0)
			break;
		cur_numri++;
		if(cur_numle==16 && cur_numri>=25)
		{
			cur_numle=50;
			cur_numri=7;
			gotoxy3(cur_numle,cur_numri);
			for(count=0;count<18;count++){
				gotoxy3(cur_numle,cur_numri);
				printf("                            \n");
				cur_numri++;
			}
			cur_numle=50;
			cur_numri=7;
		}
		else if(cur_numle==50 && cur_numri>=25)
		{
			cur_numle=16;
			cur_numri=7;
			gotoxy3(cur_numle,cur_numri);
			for(count=0;count<18;count++){
				gotoxy3(cur_numle,cur_numri);
				printf("                            \n");
				cur_numri++;
			}
			cur_numle=16;
			cur_numri=7;
		}
		sprintf(buf,"[%s] %s",nick,str);
		
		size = strlen(buf);
		//---------������ �޽��� ����---------------
		int sendsize = send(serv_sock,(char*)&size,sizeof(int),0);
		sendsize = send(serv_sock,buf,size,0);
		if(sendsize <= 0)
			break;
		//------------------------------------------
	}

	//----------���� ����------------------
	closesocket(serv_sock);
	WSACleanup();
	//-------------------------------------

}