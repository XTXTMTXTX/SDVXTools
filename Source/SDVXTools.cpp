#include<cstdio>
#include<cstdlib>
#include<windows.h>
#include<winuser.h>
#define BIT "32"
double interval0=1;
int VolLL=118,VolLR=119,VolRL=120,VolRR=121;
double CPUclock(){
	LARGE_INTEGER nFreq;
	LARGE_INTEGER t1;
	double dt;
 	QueryPerformanceFrequency(&nFreq);
 	QueryPerformanceCounter(&t1);
  	dt=(t1.QuadPart)/(double)nFreq.QuadPart;
  	return(dt*1000);
}
int read(FILE *fp){
	int x=0,f=1;char ch=fgetc(fp);
	while(ch<'0'||ch>'9')ch=fgetc(fp);
	while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=fgetc(fp);}
	return x*f;
}
void init(){
	FILE *fp;
	if(!(fp=fopen("config.ini","r"))){
		fp=fopen("config.ini","w");
		fprintf(fp,"[KeyConfig]\n");
		fprintf(fp,"VolL_L=%d\n",VolLL);
		fprintf(fp,"VolL_R=%d\n",VolLR);
		fprintf(fp,"VolR_L=%d\n",VolRL);
		fprintf(fp,"VolR_R=%d\n",VolRR);
		fclose(fp);
	}else{
		VolLL=read(fp);
		VolLR=read(fp);
		VolRL=read(fp);
		VolRR=read(fp);
		fclose(fp);
	}
}
int main(){
    HINSTANCE hinst;
    typedef int(__stdcall*lpfun_DD_todc)(int);
    typedef int(__stdcall*lpfun_DD_btn)(int btn);
    typedef int(__stdcall*lpfun_DD_mov)(int x,int y);
    typedef int(__stdcall*lpfun_DD_movR)(int dx,int dy);
    typedef int(__stdcall*lpfun_DD_whl)(int whl);
    typedef int(__stdcall*lpfun_DD_key)(int code,int flag);
    typedef int(__stdcall*lpfun_DD_str)(char* str);
    typedef int(__stdcall*lpfun_DD_MouseMove)(HWND,int x,int y);
    typedef int(__stdcall*lpfun_DD_SnapPic)(HWND,int x,int y,int w,int h);
    typedef COLORREF(__stdcall*lpfun_DD_PickColor)(HWND,int x,int y,int mode);
    typedef HWND(__stdcall*lpfun_DD_GetActiveWindow)();
    lpfun_DD_todc dd_todc;
    lpfun_DD_mov dd_movR;
    lpfun_DD_btn dd_btn;
    lpfun_DD_mov dd_mov;
    lpfun_DD_whl dd_whl;
    lpfun_DD_key dd_key;
    lpfun_DD_str dd_str;
    lpfun_DD_MouseMove dd_MouseMove;
    lpfun_DD_SnapPic dd_SnapPic;
    lpfun_DD_PickColor dd_PickColor;
    lpfun_DD_GetActiveWindow dd_GetActiveWindow;
	hinst = LoadLibrary("DD"BIT".dll");
	if(!hinst){
		printf("Error while loading DD"BIT".dll\n");
	    return 0;
	}
	dd_todc = (lpfun_DD_todc)GetProcAddress(hinst, "DD_todc");
	dd_btn = (lpfun_DD_btn)GetProcAddress(hinst, "DD_btn");
	dd_mov = (lpfun_DD_mov)GetProcAddress(hinst, "DD_mov");
	dd_movR = (lpfun_DD_mov)GetProcAddress(hinst, "DD_movR");
	dd_whl = (lpfun_DD_whl)GetProcAddress(hinst, "DD_whl");
	dd_key = (lpfun_DD_key)GetProcAddress(hinst, "DD_key");
	dd_str = (lpfun_DD_str)GetProcAddress(hinst, "DD_str");
	dd_MouseMove = (lpfun_DD_MouseMove)GetProcAddress(hinst,"DD_MouseMove");
	dd_SnapPic = (lpfun_DD_SnapPic)GetProcAddress(hinst, "DD_SnapPic");
	dd_PickColor = (lpfun_DD_PickColor)GetProcAddress(hinst,"DD_PickColor");
	dd_GetActiveWindow = (lpfun_DD_GetActiveWindow)GetProcAddress(hinst,"DD_GetActiveWindow");
	init();
	if(dd_mov&&dd_movR&&dd_MouseMove)printf("Done!\n");
	else{printf("Error\n");return 0;}
	double T0=CPUclock();
	while(1){
		if(CPUclock()-T0<interval0)continue;
		T0=CPUclock();
		if(GetAsyncKeyState(VolLL)&0x8000f)dd_movR(-1,0);
		if(GetAsyncKeyState(VolLR)&0x8000f)dd_movR(1,0);
		if(GetAsyncKeyState(VolRL)&0x8000f)dd_movR(0,-1);
		if(GetAsyncKeyState(VolRR)&0x8000f)dd_movR(0,1);
	}
}

