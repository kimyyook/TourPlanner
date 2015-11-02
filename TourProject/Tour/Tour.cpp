#include "client.h"
#include "fileclient3.h"
#include "server.h"
#include "member.h"
#include "location.h"
#include "tourplanner.h"
#include "music.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define ENTER 13
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_UP 72
#define ESC 27
#define COLSIZE 110
#define LINESIZE 40
#define SETDEFAULTCURSOR 34

int window_num=0;
int number=12;
int ch=0;
char user[20];
char password[20];
char location[20];

void gotoxy(int x, int y){
	COORD Cur;
	Cur.X=x;
	Cur.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}
void SetSize(){
	
	char col_size[10]={'\0',};
	char line_size[10] = {'\0',};
	char full[40]={"mode con:cols="};
	
	int a = LINESIZE;
	int b = COLSIZE;

	itoa(a, line_size, 10);
	itoa(b, col_size, 10);
	strcat(full,col_size);
	strcat(full," lines=");
	strcat(full,line_size);
	
	system("title TourPlanner");
	system(full);
}
void clear_console(){
	system("cls");
}
void cursor_default(){
	gotoxy(0,SETDEFAULTCURSOR);
}
void main_text(int curs){
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 10 );
	gotoxy(14,curs);
	printf("������ ��������  �� ����         ������ ��    ������ ��   ����   ������ ����  ");
	gotoxy(14,curs+1);
	printf("  ��   ��  ����  �� ����         ��  �� ��    ��  �� ���� ������ ������ ����");
	gotoxy(14,curs+2);
	printf("  ��   ��  ����  �� ������       ������ ��    ������ �� �᦭�� �᦭��   ������");
	gotoxy(14,curs+3);
	printf("  ��   ������������ ��  ��       ��     ����  ��  �� ��   ����   ������ ��  ��");
	cursor_default();
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 15 );
}
void help_text(int kick){
	int mask=kick;
	if(mask==1){
		gotoxy(5,LINESIZE-5);
		printf("������ �̵����� �ϸ�, EnterŰ�� �����մϴ�.");
		gotoxy(5,LINESIZE-6);
		printf("ESCŰ�� ���α׷��� �����մϴ�.");
	}
	else if(mask==2){
		gotoxy(5,LINESIZE-5);
		printf("EnterŰ�� ������ ���� ȭ������ �Ѿ�ϴ�.");
		gotoxy(5,LINESIZE-6);
		printf("ESCŰ�� ���α׷��� �����մϴ�.");
	}
	else if(mask==3){
		gotoxy(5,LINESIZE-5);
		printf("SPACEŰ�� ������ ������ �Ͻ����� �˴ϴ�.");
		gotoxy(5,LINESIZE-6);
		printf("ESCŰ�� ������ �����մϴ�.");
	}
}

void log_text(){
	gotoxy(38,15);
	printf("����������������������������\n");
	gotoxy(38,16);
	printf("��                        ��\n");
	gotoxy(38,17);
	printf("����������������������������\n");
	gotoxy(38,18);
	printf("��                        ��\n");
	gotoxy(38,19);
	printf("��                        ��\n");
	gotoxy(38,20);
	printf("����������������������������\n");
	
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
	gotoxy(48,16);
	printf("Login...");
	gotoxy(40,18);
	printf("   ID    :");
	gotoxy(40,19);
	printf("password :");
	
	
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 15 );
	cursor_default();
}
void print_bar(){
	int i=0;
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 5 );
	printf("��������������������������������������������������������������������������������������������������������\n");
	for(i=0;i<=LINESIZE-10;i++){
		if(i==25)
			printf("��������������������������������������������������������������������������������������������������������\n");
		else
			printf("����                                                                                                ����\n");
	}
	printf("��������������������������������������������������������������������������������������������������������\n");
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 15 );
	cursor_default();
}
void say(int num){
	int num1 = num;
	gotoxy(4,28);
	if(num1==1)
		printf("�ȳ��ϼ��� ���� �α��� ������ �Է����ֽñ� �ٶ��ϴ�.");
	else if(num1==2){
		printf("�����մϴ�, %s ����!",user);
		gotoxy(4,29);
		printf("TourPlanner�� %s������ ��Ȱ�� ���񽺸� ���Ͽ� ��� ������ ����ϴ�.\n",user);
		gotoxy(4,30);
		printf("���� ��ġ�� ��� �ǽó���?");
	}
	else if(num1==3){
		printf("�����մϴ�, %s ����!",user);
		gotoxy(4,29);
		printf("���� %s ������ ��ġ�� %s �Դϴ�.",user,location);
		gotoxy(4,30);
		printf("���Ͻô� ���񽺸� �������ּ���.");
	}
	else if(num1==11){
		printf("�ȳ��ϼ��� �����ڴ�!");
		gotoxy(4,29);
		printf("�������α׷��� �����մϴ�.");
	}
	else if(num1==4){
		printf("�ȳ��ϼ��� %s ����!",user);
		gotoxy(4,29);
		printf("���ǰ��� �Ͻð� ������~!");
		gotoxy(4,30);
		printf("���ϴ� ���� ��ȣ�� �������ֽðھ��?");
	}
	cursor_default();
	
}
void getuser_info(){
	gotoxy(51,18);
	gets(user);
	gotoxy(51,19);
	gets(password);
	cursor_default();
}
void location_info(){
	gotoxy(35,14);
	printf("����������������������������\n");
	gotoxy(35,15);
	printf("��                        ��\n");
	gotoxy(35,16);
	printf("��                        ��\n");
	gotoxy(35,17);
	printf("��                        ��\n");
	gotoxy(35,18);
	printf("����������������������������\n");

	gotoxy(40,16);
	printf("Location : ");
	gets(location);
	cursor_default();
}
void choice_info(){
	int i=1;
	int curnum=12;
	
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
	gotoxy(33,curnum);
	printf("��1. ��õ������");
	gotoxy(33,curnum+2);
	printf("��2. ģ������ �޽���������");
	gotoxy(33,curnum+4);
	printf("��3. ģ������ ��õ������ �����ϱ�");
	gotoxy(33,curnum+6);
	printf("��4. �ΰ����");
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 15 );
	cursor_default();
}
int curs_num(){
	number=12;
	ch=getch();
	gotoxy(35,number);
	while(ch!=ENTER){
		ch=getch();
		

		if(ch==KEY_UP){
			number=number-2;
			gotoxy(33,number);
			printf("��");

		}
		else if(ch==KEY_DOWN){
			number=number+2;
			gotoxy(33,number);
			printf("��");
		}
		else if(ch==KEY_RIGHT){
			number=number+2;
			gotoxy(33,number);
			printf("��");
		}	
		else if(ch==KEY_LEFT){
			number=number-2;
			gotoxy(33,number);
			printf("��");
		}

		if(number<12){
			number=12;
			gotoxy(33,number);
		}
		else if(number>18){
			number=18;
			gotoxy(33,number);
			
		}

		if(number==12){
			gotoxy(33,10);
			printf(" ");
			gotoxy(33,20);
			printf(" ");
			gotoxy(33,14);
			printf("��");
			gotoxy(33,16);
			printf("��");
			gotoxy(33,18);
			printf("��");
		}
		else if(number==14){
			gotoxy(33,10);
			printf(" ");
			gotoxy(33,20);
			printf(" ");
			gotoxy(33,12);
			printf("��");
			gotoxy(33,16);
			printf("��");
			gotoxy(33,18);
			printf("��");
		}
		else if(number==16){
			gotoxy(33,10);
			printf(" ");
			gotoxy(33,20);
			printf(" ");
			gotoxy(33,12);
			printf("��");
			gotoxy(33,14);
			printf("��");
			gotoxy(33,18);
			printf("��");
		}
		else if(number==18){
			gotoxy(33,10);
			printf(" ");
			gotoxy(33,20);
			printf(" ");
			gotoxy(33,14);
			printf("��");
			gotoxy(33,16);
			printf("��");
			gotoxy(33,12);
			printf("��");
		}
		gotoxy(33,number);
	}
	
	
	return number;
	cursor_default();
}

int print_musiclist(){
	int a;
	gotoxy(28,12);
	printf("1. ������ ���丮 bgm 1");
	gotoxy(28,14);
	printf("2. ������ ���丮 bgm 2");
	gotoxy(28,16);
	printf("3. ������ ���丮 bgm 3");

	cursor_default();
	scanf("%d",&a);

	return a;

}
void excute(){
	int excute_num=0;
	int numbertoplay=0;
	
	if(number==12){
		last(user,location);
	}
	else if(number==14){
		main_client(user);
	}
	else if(number==16){
		
		clear_console();
		main_fileclient(user);
		//main_client2(user);
		
	}
	else if(number==18){
		say(4);
		help_text(3);
		
		numbertoplay = print_musiclist();
		if(numbertoplay ==1)
			play_1();
		else if(numbertoplay==2)
			play_2();
		else if(numbertoplay==3)
			play_3();
	}
	else{
		printf("�ٽ� �Է����ֽñ� �ٶ��ϴ�.");
	}
	window_num=3;
}
void start_load(){
	int i=0;
	int nume=2;
	main_text(6);
	gotoxy(30,18);
	printf("Now Loading....");
	gotoxy(27,19);
	printf("������������������������������������������������������\n");
	gotoxy(27,20);
	printf("������������������������������������������������������\n");
	gotoxy(27,21);
	printf("������������������������������������������������������\n");

	for(i=1;i<=50;i++){
		gotoxy(i+28,20);
		Sleep(50);
		printf("*");
	}
	clear_console();
}
int choice_exit(){
	int isTrue=0;
	char choice[10]={'\0',};
	
	ch=getch();
	
	if(ch==ESC){
		clear_console();
		
		log_text();
		gotoxy(43,16);
		printf("�����Ͻðڽ��ϱ�?");
		gotoxy(40,18);
		printf("        Y / N");
		gotoxy(40,19);
		printf("           ");
		gets(choice);
		if((strcmp(choice,"y")==0 || strcmp(choice,"Y")==0)){
			isTrue=1;
		}
		else if((strcmp(choice,"n")==0 || strcmp(choice,"N")==0)){
			isTrue=2;
		}
	}

	if(isTrue==1){
		clear_console();
		gotoxy(40,16);
		printf("���α׷��� �����մϴ�\n.");
		cursor_default();
		exit(1);
		return 1;
	}
	else if(isTrue==2){
		clear_console();
		gotoxy(40,16);
		printf("����մϴ�.");
		Sleep(1000);
	}
	
		
}

int main(){
	
	SetSize();
	print_bar();
	start_load();
	window_num=1;
	while(TRUE){
		if(window_num==1){
				print_bar();
				say(window_num);
				main_text(6);
				log_text();
				getuser_info();
				help_text(2);
				
				if(choice_exit()==1)
					break;
				
							
				if(strcmp(user,"admin")==0 && strcmp(password,"7777")==0){
					window_num=11;
				}
				else if((login_infomation(user,password))==1){
					clear_console();
					window_num=2;
				}
				else if((login_infomation(user,password))==0){
					clear_console();
					gotoxy(30,15);
					printf("������������������������������������������������\n");
					gotoxy(30,16);
					printf("��                                            ��\n");
					gotoxy(30,17);
					printf("��                                            ��\n");
					gotoxy(30,18);
					printf("��                                            ��\n");
					gotoxy(30,19);
					printf("��                                            ��\n");
					gotoxy(30,20);
					printf("������������������������������������������������\n");
					gotoxy(35,17);
					printf("���� �����Դϴ�. �ٽ� �Է����ּ���\n");
					gotoxy(35,18);
					printf("�ǵ��ư��÷��� �ƹ�Ű�� �����ּ���");
					getch();
					clear_console();
					window_num=1;
				}
				else{
					break;
				}
				
		}
		if(window_num==2){
			while(TRUE){
				clear_console();

				if(window_num==2){
					print_bar();
					say(window_num);
					main_text(2);
					location_info();
					help_text(2);					
					if(choice_exit()==1)
					break;
					if(locating(location)==1){
						clear_console();
						window_num=3;
					}
					else if(locating(location)==0){
						clear_console();
						gotoxy(30,15);
						printf("������������������������������������������������\n");
						gotoxy(30,16);
						printf("��                                            ��\n");
						gotoxy(30,17);
						printf("��                                            ��\n");
						gotoxy(30,18);
						printf("��                                            ��\n");
						gotoxy(30,19);
						printf("��                                            ��\n");
						gotoxy(30,20);
						printf("������������������������������������������������\n");
						gotoxy(35,17);
						printf("���� �����Դϴ�. �ٽ� �Է����ּ���\n");
						gotoxy(35,18);
						printf("�ǵ��ư��÷��� �ƹ�Ű�� �����ּ���");
						getch();
						clear_console();
						window_num=2;
					}
				}
				else if(window_num==3){
					print_bar();
					say(window_num);
					main_text(2);
					help_text(1);
					choice_info();
					curs_num();
					window_num=4;
				}
				else if(window_num==4){
					
					print_bar();
					main_text(2);
					excute();
					window_num=3;
					if(choice_exit()==1)
						break;
				}
			}
		}

		else if(window_num==11){
			while(TRUE){
				clear_console();
				if(window_num==11){
					clear_console();
					print_bar();
					main_text(2);
					say(window_num);
					choice_exit();
					gotoxy(15,6);
					main_server();
				}
			}
		}
		else if(window_num==0){
			choice_exit();
			
		}
	}
	gotoxy(30,25);
}

