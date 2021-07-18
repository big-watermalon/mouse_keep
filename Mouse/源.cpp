#include<iostream>
#include<windows.h>
#include<fstream>
#include<time.h>
#include "源.h"
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;

typedef struct act {//定义存储鼠标操作信息的结构体对象；
	long x;
	long y;
	long t;
}action;

action aa[500]; //定义操作数组；
int a = 0;		//操作计数； 
int k = 1;		//循环计数
float cost = 0.0;//脚本总耗时
int delay = 3
; //接收脚本延时
int num = 0; //接收脚本次数

void store() {//定义存储操作的方法；

	POINT ptB = { 0,0 };	//获取鼠标坐标；
	LPPOINT xy = &ptB;
	GetCursorPos(xy);
	long x = xy->x;
	long y = xy->y;

	SYSTEMTIME sys;//获取当日经过毫秒数；
	GetLocalTime(&sys);
	long h = sys.wHour * 60 * 60 * 1000;
	long m = sys.wMinute * 60 * 1000;
	long s = sys.wSecond * 1000;
	long ms = sys.wMilliseconds;
	long t = 0;
	t = h + m + s + ms;

	struct act temp;//创建操作对象；
	temp.x = x;		//存储鼠标坐标，当日用时；
	temp.y = y;
	temp.t = t;
	if (a == 0) {
		aa[a] = temp;
		printf("x=%d,y=%d,t=%d,count=%d 第一次操作存入成功！\n", aa[a].x, aa[a].y, aa[a].t, a);
		a++;
	}
	if ((temp.x == aa[a - 1].x) && (temp.t - aa[a - 1].t < 120)) {//过滤同点时间间隔过短的点击
	}
	else {
		aa[a] = temp;
		printf("x=%d,y=%d,t=%d,count=%d 操作存入成功！\n", aa[a].x, aa[a].y, aa[a].t, a);
		a++;//记录最后一次操作时也进行了count++，所以总操作数为count；
	}
}

void out() {//定义数据输出文件方法
	ofstream outfile;
	outfile.open("data.dat", ios::trunc);
	int i;
	outfile << a << endl;
	for (i = 0; i < a; i++) {
		outfile << aa[i].x << " " << aa[i].y << " " << aa[i].t << endl;
	}
	outfile.close();
}
void initial() {
	cout << "-------------------------------------------------\n";
	cout << "请输入脚本需要循环的次数：(输入0为无限循环)\n";
	cin >> num;
	cout << "请输入脚本每次循环后休息时间，(按回车确认,建议不要设置为0)\n";
	cin >> delay;
	printf("配置成功！脚本将循环%d次，每次循环后休息%d秒:\n", num, delay);
	Sleep(1000);
	cout << "----------------下面开始执行脚本------------------\n";
}

void in() {//定义从文件读取数据方法
	ifstream infile;
	infile.open("data.dat");
	int i;
	infile >> a;
	for (i = 0; i < a; i++) {
		infile >> aa[i].x;
		infile >> aa[i].y;
		infile >> aa[i].t;
	}
	//将每个结构体的t存放休眠时间；
	for (i = 0; i < a; i++) {
		if (i == (a - 1)) {
			aa[i].t = 0;
		}
		else {
			aa[i].t = aa[i + 1].t - aa[i].t;
			cost = cost + aa[i].t;
		}
	}
	infile.close();
}

int fun_1() {//录制操作
	while (true) {//循环监控鼠标按键情况
		if (KEY_DOWN(VK_LBUTTON)) {
			//调用存储方法将本次操作进行存档；
			store();
		}
		if (KEY_DOWN(VK_ESCAPE)) {
			//调用输出方法进行全部操作存为文件；
			out();
			cout << "-------------------------------------------------\n";
			printf("录制结束！共录制了%d个操作,耗时%d毫秒！\n", a, (aa[a - 1].t - aa[0].t));
			printf("如要执行刚刚录制脚本，请重新运行本工具并选择模式2！\n");
			return 0;
		}
		Sleep(20);//循环时间间隔，防止太占内存
	}
}

int fun_2() {//执行操作
	int i;
	for (i = 0; i < a; i++) {
		SetCursorPos(aa[i].x, aa[i].y);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		printf("执行第%d步完毕！\n", i + 1);
		Sleep(aa[i].t);
		if (KEY_DOWN(VK_ESCAPE)) {
			cost = cost * (k - 1) + delay * (k - 1);
			while (i != -1) {
				cost = cost + aa[i].t;
				i--;
			}
			cout << "-------------------------------------------------\n";
			printf("脚本提前结束！共执行了%d次脚本,耗时%.3f秒\n", k, cost / 1000.0);
			k = 0;
			return 0;
		}
	}
	return 0;
}

void choose() {
	cout << "请选择运行模式：" << "（输入1或2进行选择，按ENTER回车确认选择）\n" << "1.录制脚本\n" << "2.执行脚本\n";
	char ch;
	switch (ch = getchar()) {
	case '1': {
		system("cls");
		cout << "您选择录制脚本，下面开始录制鼠标操作：(按ESC结束录制)\n";
		cout << "-------------------------------------------------\n";
		fun_1();
		break;
	}
	case '2': {
		system("cls");
		cout << "您选择执行脚本，下面请先进行一些配置.\n";
		in();
		initial();
		while (k) {
			fun_2();
			if (k == 0 || k == num) break;
			printf("第%d次循环执行完毕，%d秒后进入下一循环！(提前结束请长按ESC或关闭本窗口)\n", k, delay);
			k++;
			Sleep(1000 * delay);
		}
		if (num != 0 && k == num) {
			cout << "-------------------------------------------------\n";
			cost = cost * num + delay * 1000 * (num - 1);
			printf("第%d次循环执行完毕，总耗时%.3f秒，脚本停止执行。\n", num, cost / 1000.0);
		}
	}
	}
	system("pause");
}

int main() {
	cout << "欢迎使用，鼠标脚本工具开始运行！\n";
	cout << "-------------------------------------------------\n";
	choose();
}
