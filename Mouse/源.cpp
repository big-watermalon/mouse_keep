#include<iostream>
#include<windows.h>
#include<fstream>
#include<time.h>
#include "Դ.h"
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;

typedef struct act {//����洢��������Ϣ�Ľṹ�����
	long x;
	long y;
	long t;
}action;

action aa[500]; //����������飻
int a = 0;		//���������� 
int k = 1;		//ѭ������
float cost = 0.0;//�ű��ܺ�ʱ
int delay = 3
; //���սű���ʱ
int num = 0; //���սű�����

void store() {//����洢�����ķ�����

	POINT ptB = { 0,0 };	//��ȡ������ꣻ
	LPPOINT xy = &ptB;
	GetCursorPos(xy);
	long x = xy->x;
	long y = xy->y;

	SYSTEMTIME sys;//��ȡ���վ�����������
	GetLocalTime(&sys);
	long h = sys.wHour * 60 * 60 * 1000;
	long m = sys.wMinute * 60 * 1000;
	long s = sys.wSecond * 1000;
	long ms = sys.wMilliseconds;
	long t = 0;
	t = h + m + s + ms;

	struct act temp;//������������
	temp.x = x;		//�洢������꣬������ʱ��
	temp.y = y;
	temp.t = t;
	if (a == 0) {
		aa[a] = temp;
		printf("x=%d,y=%d,t=%d,count=%d ��һ�β�������ɹ���\n", aa[a].x, aa[a].y, aa[a].t, a);
		a++;
	}
	if ((temp.x == aa[a - 1].x) && (temp.t - aa[a - 1].t < 120)) {//����ͬ��ʱ�������̵ĵ��
	}
	else {
		aa[a] = temp;
		printf("x=%d,y=%d,t=%d,count=%d ��������ɹ���\n", aa[a].x, aa[a].y, aa[a].t, a);
		a++;//��¼���һ�β���ʱҲ������count++�������ܲ�����Ϊcount��
	}
}

void out() {//������������ļ�����
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
	cout << "������ű���Ҫѭ���Ĵ�����(����0Ϊ����ѭ��)\n";
	cin >> num;
	cout << "������ű�ÿ��ѭ������Ϣʱ�䣬(���س�ȷ��,���鲻Ҫ����Ϊ0)\n";
	cin >> delay;
	printf("���óɹ����ű���ѭ��%d�Σ�ÿ��ѭ������Ϣ%d��:\n", num, delay);
	Sleep(1000);
	cout << "----------------���濪ʼִ�нű�------------------\n";
}

void in() {//������ļ���ȡ���ݷ���
	ifstream infile;
	infile.open("data.dat");
	int i;
	infile >> a;
	for (i = 0; i < a; i++) {
		infile >> aa[i].x;
		infile >> aa[i].y;
		infile >> aa[i].t;
	}
	//��ÿ���ṹ���t�������ʱ�䣻
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

int fun_1() {//¼�Ʋ���
	while (true) {//ѭ�������갴�����
		if (KEY_DOWN(VK_LBUTTON)) {
			//���ô洢���������β������д浵��
			store();
		}
		if (KEY_DOWN(VK_ESCAPE)) {
			//���������������ȫ��������Ϊ�ļ���
			out();
			cout << "-------------------------------------------------\n";
			printf("¼�ƽ�������¼����%d������,��ʱ%d���룡\n", a, (aa[a - 1].t - aa[0].t));
			printf("��Ҫִ�иո�¼�ƽű������������б����߲�ѡ��ģʽ2��\n");
			return 0;
		}
		Sleep(20);//ѭ��ʱ��������ֹ̫ռ�ڴ�
	}
}

int fun_2() {//ִ�в���
	int i;
	for (i = 0; i < a; i++) {
		SetCursorPos(aa[i].x, aa[i].y);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		printf("ִ�е�%d����ϣ�\n", i + 1);
		Sleep(aa[i].t);
		if (KEY_DOWN(VK_ESCAPE)) {
			cost = cost * (k - 1) + delay * (k - 1);
			while (i != -1) {
				cost = cost + aa[i].t;
				i--;
			}
			cout << "-------------------------------------------------\n";
			printf("�ű���ǰ��������ִ����%d�νű�,��ʱ%.3f��\n", k, cost / 1000.0);
			k = 0;
			return 0;
		}
	}
	return 0;
}

void choose() {
	cout << "��ѡ������ģʽ��" << "������1��2����ѡ�񣬰�ENTER�س�ȷ��ѡ��\n" << "1.¼�ƽű�\n" << "2.ִ�нű�\n";
	char ch;
	switch (ch = getchar()) {
	case '1': {
		system("cls");
		cout << "��ѡ��¼�ƽű������濪ʼ¼����������(��ESC����¼��)\n";
		cout << "-------------------------------------------------\n";
		fun_1();
		break;
	}
	case '2': {
		system("cls");
		cout << "��ѡ��ִ�нű����������Ƚ���һЩ����.\n";
		in();
		initial();
		while (k) {
			fun_2();
			if (k == 0 || k == num) break;
			printf("��%d��ѭ��ִ����ϣ�%d��������һѭ����(��ǰ�����볤��ESC��رձ�����)\n", k, delay);
			k++;
			Sleep(1000 * delay);
		}
		if (num != 0 && k == num) {
			cout << "-------------------------------------------------\n";
			cost = cost * num + delay * 1000 * (num - 1);
			printf("��%d��ѭ��ִ����ϣ��ܺ�ʱ%.3f�룬�ű�ִֹͣ�С�\n", num, cost / 1000.0);
		}
	}
	}
	system("pause");
}

int main() {
	cout << "��ӭʹ�ã����ű����߿�ʼ���У�\n";
	cout << "-------------------------------------------------\n";
	choose();
}
