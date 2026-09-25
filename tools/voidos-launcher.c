#include <windows.h>
#include <stdio.h>
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
(void)hInstance;(void)hPrevInstance;(void)lpCmdLine;(void)nCmdShow;
char base[MAX_PATH];DWORD n=GetModuleFileNameA(NULL,base,MAX_PATH);
if(!n||n>=MAX_PATH){MessageBoxA(NULL,"Could not locate VoidOS launcher.","VoidOS",MB_ICONERROR);return 1;}
for(DWORD i=n;i>0;--i)if(base[i-1]=='\\'||base[i-1]=='/'){base[i-1]=0;break;}
char iso[MAX_PATH*2],cmd[MAX_PATH*3];snprintf(iso,sizeof(iso),"%s\\build\\voidos.iso",base);
snprintf(cmd,sizeof(cmd),"qemu-system-i386.exe -m 256 -cdrom \"%s\" -boot d",iso);
STARTUPINFOA si;PROCESS_INFORMATION pi;ZeroMemory(&si,sizeof(si));ZeroMemory(&pi,sizeof(pi));si.cb=sizeof(si);
if(!CreateProcessA(NULL,cmd,NULL,NULL,FALSE,0,NULL,base,&si,&pi)){MessageBoxA(NULL,"QEMU was not found in PATH.\nInstall QEMU for Windows, then run VoidOS Launcher again.","VoidOS Launcher",MB_ICONERROR);return 2;}
CloseHandle(pi.hThread);CloseHandle(pi.hProcess);return 0;
}
