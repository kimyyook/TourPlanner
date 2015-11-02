
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <windows.h> 
#include <mmsystem.h> 
#include <tchar.h> 
#pragma comment(lib,"winmm")

int play_1()
{ 
   TCHAR strCmd[MAX_PATH], strRet[MAX_PATH]; 
   TCHAR strID[80]; 
   BOOL  bPlay=TRUE; 
   wsprintf(strID, TEXT("mymusic%d"), 1); 
   wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("test.mp3"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   wsprintf(strCmd, TEXT("play %s from 0"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   while (1) 
   { 
      if(GetAsyncKeyState(VK_SPACE)&0x8001==0x8001) 
      { 
         if (bPlay) 
         { 
            wsprintf(strCmd, TEXT("pause %s"), strID); 
            mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
            bPlay=FALSE; 
            }else{ 
            wsprintf(strCmd, TEXT("resume %s"), strID); 
            mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
            bPlay=TRUE; 
         } 
      }else if(GetAsyncKeyState(VK_ESCAPE)&0x8001==0x8001) 
      { 
         break; 
      } 
      Sleep(20); 
   } 
   wsprintf(strCmd, TEXT("stop %s"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   wsprintf(strCmd, TEXT("close %s"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 



   return 0; 
}
int play_2()
{ 
   TCHAR strCmd[MAX_PATH], strRet[MAX_PATH]; 
   TCHAR strID[80]; 
   BOOL  bPlay=TRUE; 
   wsprintf(strID, TEXT("mymusic%d"), 1); 
   wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("test2.mp3"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   wsprintf(strCmd, TEXT("play %s from 0"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   while (1) 
   { 
      if(GetAsyncKeyState(VK_SPACE)&0x8001==0x8001) 
      { 
         if (bPlay) 
         { 
            wsprintf(strCmd, TEXT("pause %s"), strID); 
            mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
            bPlay=FALSE; 
            }else{ 
            wsprintf(strCmd, TEXT("resume %s"), strID); 
            mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
            bPlay=TRUE; 
         } 
      }else if(GetAsyncKeyState(VK_ESCAPE)&0x8001==0x8001) 
      { 
         break; 
      } 
      Sleep(20); 
   } 
   wsprintf(strCmd, TEXT("stop %s"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   wsprintf(strCmd, TEXT("close %s"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 



   return 0; 
}
int play_3()
{ 
   TCHAR strCmd[MAX_PATH], strRet[MAX_PATH]; 
   TCHAR strID[80]; 
   BOOL  bPlay=TRUE; 
   wsprintf(strID, TEXT("mymusic%d"), 1); 
   wsprintf(strCmd, TEXT("open %s alias %s"), TEXT("test3.mp3"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   wsprintf(strCmd, TEXT("play %s from 0"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   while (1) 
   { 
      if(GetAsyncKeyState(VK_SPACE)&0x8001==0x8001) 
      { 
         if (bPlay) 
         { 
            wsprintf(strCmd, TEXT("pause %s"), strID); 
            mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
            bPlay=FALSE; 
            }else{ 
            wsprintf(strCmd, TEXT("resume %s"), strID); 
            mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
            bPlay=TRUE; 
         } 
      }else if(GetAsyncKeyState(VK_ESCAPE)&0x8001==0x8001) 
      { 
         break; 
      } 
      Sleep(20); 
   } 
   wsprintf(strCmd, TEXT("stop %s"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 
   wsprintf(strCmd, TEXT("close %s"), strID); 
   mciSendString(strCmd, strRet, sizeof(strRet), NULL); 



   return 0; 
}

