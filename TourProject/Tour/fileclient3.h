#pragma comment (lib, "ws2_32.lib") 
#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <process.h>
#include <fcntl.h>

/*
recv�� �޾ƿ��� �Լ� send�� ������ �Լ�
*/

int filerecvn(SOCKET s, char *buf, int len, int flags)		// �޼����� �޾ƿ��� ���� �� ������ ���� ���� ���ϴ� �Լ�...
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

void __cdecl fileRecvThread (void * p)  //�����带 �̿� �����κ��� �޼������� �޾ƿ��°�
{
	SOCKET sock = (SOCKET)p; //���ϼ���
	char buf[2048];
	int size;
	while(1)
	{
		//-----------�����κ��� ����------------
		int recvsize = recv(sock,(char*)&size,sizeof(buf),0);
		recvsize = recv(sock,buf,size,0);
		if(recvsize <= 0)
		{
			printf("��������\n");
			break;
		}
		//------------------------------------------------
		buf[recvsize] = '\0';
		printf("%s\n",buf);
		printf("���ۼ���!!!!\n");
	}
}

int main_fileclient(char *name)
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
	char nick[20] ={'\0'};
	strcpy(nick,name);
	
    printf("�̸� : %s\n", nick);
	printf("���͸� ������ ����\n");
	_beginthread(fileRecvThread,0,(void*)serv_sock);
	//---------------------------------------
	while(1) //���ѹݺ�
	{
		int ch;
		FILE* fStream; // ������ �б� ���� ���� ����
		char trans[2048] = {0};
		char buf2[2048] = {0};
		int filesize = 0;
		ch=getch();
		//---------������ �޽��� ����---------------

		//------------------------------------------
		if(ch==13)
		{
		//������ �������� ���÷� ������� trans������ ���� ���Ƿ� �����	
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

	//----------���� ����------------------
	closesocket(serv_sock);
	WSACleanup();
	//-------------------------------------
}