#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#define size 20

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13

void gotoxy(int x, int y);							//Ŀ���� ��ǥ�� �ű�� �Լ�
void clearStatusBar(void);							//������ ���� ����â�� ����� �Լ�
void ShowCursorPos(int x, int y, char ch, int trig);//Ŀ���� ����Ű�� ��ġ�� ǥ���ϴ� �Լ�
void TurnChange(char *ptr);							//���� ���� ǥ�õǴ� ���� �ٲٴ� �Լ�
void ShowPlate(char(*plate)[20]);					//�������� �����ִ� �Լ�
int Win(char c);									//�̰��� �� ����ϴ� �Լ�
int WinOrNot(char plate[][20], int y, int x);		//�������� �ƴ��� �Ǵ��ϴ� �Լ�
int move(char plate[][20]);							//�̵��� �� ���⸦ ������ �Լ�

char plate[20][20];					//������ �迭
int x, y;
int trig = 0;

int main(void)
{
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);

	for(x=1; x<size; x++)				//1���� 19���� 
		plate[0][x] = x;				//���� ���ٿ� 1~19 ������� ����
	for(y=1; y<size; y++)				//1���� 19����
		plate[y][0] = y;				//���� ���ʿ� 1~19 ������� ����
	for(x=1; x<size; x++)
		for(y=1; y<size; y++)
			plate[y][x] = '.';			//�������� �� .���� ä��

	plate[0][0] = 'O';					//���� O ���� ����
	ShowPlate(plate);					//�������� ������
	gotoxy(10 * 3 + 1, 10);				//(10, 10) �������� Ŀ�� �̵�
	while(1)
	{
		trig = move(plate);				//Ʈ���ſ� move()�� ��ȯ�� �� ����
		clearStatusBar();

		switch (trig)
		{
		case 2:							//���� �޼��� ��� Ʈ���Ű� Ȱ��ȭ��������
			gotoxy(10 * 3 + 1, 10);		//������ġ
			trig = 0;					//Ʈ���� ��Ȱ��ȭ
			break;	
		case 1:							//���� �޼��� ��� Ʈ���Ű� Ȱ��ȭ��������
			gotoxy(0, 21);				//����� ��ҷ� �̵��� ��
			SetConsoleTextAttribute(hnd, 0x04);//���, ���ڻ� ����(����, ���� ����)
			printf("�߸��� ��ǥ�Դϴ�! \n");//���� �޼��� ���
			SetConsoleTextAttribute(hnd, 0x0F);//���, ���ڻ� ����(����, �Ͼ�)
			trig = 0;					//Ʈ���� ��Ȱ��ȭ
			break;
		case -1:						//-1�� ��ȯ�Ǿ�����
			return 0;					//����
		case 0:
			TurnChange(&plate[0][0]);		//�� �ٲ� �� CASE �ȿ� �־ �����ذ�.		
		}
		
	}
}
void clear() //�ʱ�ȭ �Լ� �߰�
{
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	//char plate[20][20];					//������ �迭
	//int x, y;
	//int trig = 0;

	for(x=1; x<size; x++)				//1���� 19���� 
		plate[0][x] = x;				//���� ���ٿ� 1~19 ������� ����
	for(y=1; y<size; y++)				//1���� 19����
		plate[y][0] = y;				//���� ���ʿ� 1~19 ������� ����
	for(x=1; x<size; x++)
		for(y=1; y<size; y++)
			plate[y][x] = '.';			//�������� �� .���� ä��

	plate[0][0] = 'O';					//���� O ���� ����
	ShowPlate(plate);					//�������� ������
	gotoxy(10 * 3 + 1, 10);				//(10, 10) �������� Ŀ�� �̵�
}
void gotoxy(int x, int y)
{
	/*
	Ŀ���� ��ǥ�� �ű�� �Լ�

	�ű� x������ y��ǥ�� ���ڷ� ����
	*/
	COORD pos = { x, y };				//x�� y��ǥ�� ����ü�� �Ҵ�
	static HANDLE hnd;
	hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hnd, pos);	//Ŀ�� ��ǥ�� �ű�
}
void clearStatusBar(void)
{
	/*
	������ ���� ����â�� ����� �Լ�
	*/
	gotoxy(0, 21);						//"�߸��� ��ǥ�Դϴ�!" �κ� ����
	printf("                              ");

	gotoxy(0, 22);						//��ǥ �κ� ����
	printf("                              \n");
}
void ShowCursorPos(int x, int y, char ch, int trig)
{
	/*
	Ŀ���� ����Ű�� ��ġ�� ǥ���ϴ� �Լ�

	x, y��ǥ�� �� ��ǥ�� ��µǴ� ����, ǥ�� ����/���� ���� Ʈ���Ÿ� ���ڷ� ����
	*/
	static HANDLE hnd;
	hnd = GetStdHandle(STD_OUTPUT_HANDLE);

	if (trig)									//Ʈ���Ű� 1�̸� ����� ����� �ٲ�
	{
		gotoxy(x + 2, y);						//��µ� �� ������ �̵�
		putchar('\b');							//�� ĭ ����
		SetConsoleTextAttribute(hnd, 0xE0);		//���, ���ڻ� ����(���, ����)
		putchar(ch);							//���� ���
		gotoxy(x + 1, y);						//��µ� ������ ��ǥ�� �̵�
		SetConsoleTextAttribute(hnd, 0x0F);		//���, ���ڻ� ����(����, �Ͼ�)
	}
	else										//Ʈ���Ű� 0�̸� ���󺹱�
	{
		gotoxy(x + 2, y);						//��µ� �� ������ �̵�
		putchar('\b');							//�� ĭ ����
		switch (ch)
		{
		case '.':								//���ڰ� '.'�̸�
			SetConsoleTextAttribute(hnd, 0x0F); //���, ���ڻ�����(����, �Ͼ�)
			break;
		case 'O':								//���ڰ� 'O'��
			SetConsoleTextAttribute(hnd, 0x09); //���, ���ڻ� ����(����, �Ķ�)
			break;
		case 'X':								//���ڰ� 'X'��
			SetConsoleTextAttribute(hnd, 0x0C); //���, ���ڻ� ����(����, ����)
			break;
		}
		putchar(ch);							//���� ���
		gotoxy(x + 1, y);						//��µ� ������ ��ǥ�� �̵�
		SetConsoleTextAttribute(hnd, 0x0F);		//���, ���ڻ� ����(����, ���)
	}
}
void TurnChange(char *ptr)
{
	/*
	���� ���� ǥ�õǴ� ���� �ٲٴ� �Լ�

	������ ���� ���� ��ǥ�� ���ڷ� ����
	*/
	static HANDLE hnd;
	hnd = GetStdHandle(STD_OUTPUT_HANDLE);

	*ptr = (*ptr == 'O') ? 'X' : 'O';

	gotoxy(2, 0);							//���� ǥ�õǴ� ��ǥ���� �� ĭ �ڷ� �̵�
	putchar('\b');							//�� ĭ�� ���� ��
	if (*ptr == 'O')						//'O'�̸�
		SetConsoleTextAttribute(hnd, 0x71);	//���� ���ڻ� �ٲ�(���� ȸ��, £�� �Ķ�)
	else									//'X'�̸�
		SetConsoleTextAttribute(hnd, 0x74);	//���� ���ڻ� �ٲ�(���� ȸ��, £�� ����)
	putchar(*ptr);							//���

	SetConsoleTextAttribute(hnd, 0x0F);		//���� ���ڻ� ����(������, ���)
}
void ShowPlate(char(*plate)[20])
{
	/*
	�������� �����ִ� �Լ�

	������ �迭�� ���ڷ� ����
	*/
	int x, y;
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");						//ȭ�� Ŭ����

	if (plate[0][0] == 'O')				//'O'�� ���̶��
		SetConsoleTextAttribute(hnd, 0x71); //���� ���ڻ� �ٲ�(���� ȸ��, £�� �Ķ�)
	else								//'X'�� ���̶��
		SetConsoleTextAttribute(hnd, 0x74); //���� ���ڻ� �ٲ�(���� ȸ��, £�� ����)
	printf("%2c ", plate[0][0]);		//�� ���

	SetConsoleTextAttribute(hnd, 0x8F);	//���� ���ڻ� �ٲ�(£�� ȸ��, ����)
	for (x = 1; x < size; x++)
		printf("%2d ", x);				//1���� size(20) ������ ���ڷ� ���
	putchar('\n');

	for (y = 1; y < size; y++)
	{
		SetConsoleTextAttribute(hnd, 0x8F);//���� ���ڻ� �ٲ�(£�� ȸ��, ����)
		printf("%2d ", plate[y][0]);    //���� �տ��� ���ڷ� ���
		for (x = 1; x < size; x++)
		{
			SetConsoleTextAttribute(hnd, 0x0F);//���� ���ڻ� �ٲ�(����, ���)
			printf("%2c ", plate[y][x]);//�ٵ��� ���
		}
		putchar('\n');
	}
}
int Win(char c)
{
	/*
	�̰��� �� ����ϴ� �Լ�

	�̱� �÷��̾�(O �Ǵ� X)�� ���ڷ� ����
	*/
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hnd, 0x02);	//������ ���ڻ� �ٲ�(����, �ʷ�)
	printf("=======");

	if (c == 'O')						//���ڰ� 'O'���
		SetConsoleTextAttribute(hnd, 0x09);//������ ���ڻ� �ٲ�(����, �Ķ�)
	else								//���ڰ� 'X'���
		SetConsoleTextAttribute(hnd, 0x0C);//������ ���ڻ� �ٲ�(����, ����)
	printf("%c ��!", c);					//���

	SetConsoleTextAttribute(hnd, 0x02);	//������ ���ڻ� �ٲ�(����, �ʷ�)
	printf("=======");

	SetConsoleTextAttribute(hnd, 0x0F); //������ ���ڻ� ����(����, �Ͼ�)
	putchar('\n');

	return 1;
}
int WinOrNot(char plate[][20], int y, int x)
{
	/*
	�������� �ƴ��� �Ǵ��ϴ� �Լ�

	�����ǰ� y, x��ǥ�� ���ڷ� ����
	������ ��� 1, �ƴϸ� 0�� ��ȯ
	*/
	int i, j;
	int trig = 0;

	for (i = x - 4; i <= x + 4; i++)  //���� �˻� (�ڿ��� 4ĭ~������ 4ĭ)
	{
		if (plate[y][x] == plate[y][i]) //ó�� ���۰� ���� ��ġ�� ���ٸ�
			trig++;						//Ʈ����++
		else							//�ٸ��ٸ�
			trig = 0;					//Ʈ���Ÿ� 0���� �ʱ�ȭ

		if (trig >= 5)					//�˻����� �� Ʈ���Ű� 5���� ũ�ٸ�
			return 1;
	}
	trig = 0;							//Ʈ���� 0���� �ʱ�ȭ

	for (j = y - 4; j <= y + 4; j++) //���� �˻� (�ڿ��� 4ĭ~������ 4ĭ)
	{
		if (plate[y][x] == plate[j][x])	//ó�� ���۰� ���� ��ġ�� ���ٸ�
			trig++;						//Ʈ����++
		else							//�ٸ��ٸ�
			trig = 0;					//Ʈ���Ÿ� 0���� �ʱ�ȭ

		if (trig >= 5)					//�˻����� �� Ʈ���Ű� 5���� ũ�ٸ�
			return 1;
	}
	trig = 0;							//Ʈ���� 0���� �ʱ�ȭ

	j = y - 4;							//�밢������ ���� ������, j�� �̸� �ʱ�ȭ
	for (i = x - 4; i <= x + 4; )  //������ �Ʒ� �밢��
	{

		if (plate[y][x] == plate[j][i])	//ó�� ���۰� ���� ��ġ�� ���ٸ�
			trig++;						//Ʈ����++
		else							//�ٸ��ٸ�
			trig = 0;					//Ʈ���� 0���� �ʱ�ȭ

		if ((i == 0) && (j == 0))		//��ǥ�� ���� ���� ǥ���ϴ� ��ǥ(������ ��)�� ���
			trig = 0;					//Ʈ���� �ʱ�ȭ

		if (trig >= 5)					//�˻����� �� Ʈ���Ű� 5���� ũ�ٸ�
			return 1;
		j++;
		i++;
	}
	trig = 0;							//Ʈ���� 0���� �ʱ�ȭ

	j = y - 4;							//�밢������ ���� ������, j�� �̸� �ʱ�ȭ
	for (i = x + 4; i >= x - 4; i--) //���� �Ʒ� �밢��
	{
		if (plate[y][x] == plate[j][i])	//ó�� ���۰� ���� ��ġ�� ���ٸ�
			trig++;						//Ʈ����++
		else							//�ٸ��ٸ�
			trig = 0;					//Ʈ���� 0���� �ʱ�ȭ

		if (trig >= 5)					//�˻����� �� Ʈ���Ű� 5���� ũ�ٸ�
			return 1;
		j++;
	}
	return 0;
}
int move(char plate[][20])
{
	/*
	�̵��� �� ���⸦ ������ �Լ�

	������ �迭�� ���ڷ� ����
	������ �������� -1, �߸��� ��ǥ�� �����ϸ� 1, �������̸� 0 ��ȯ
	*/

	char clt;
	static int x = 10 * 3;							//�� ĭ�� ���ΰ� 3�̱� ������, 3�� ����
	static int y = 10;
	static HANDLE hnd;
	hnd = GetStdHandle(STD_OUTPUT_HANDLE);

	while (1)
	{
		gotoxy(0, 22);							//(0, 22)�������� �̵��ؼ�
		printf("��ǥ : %-2d %-2d", x / 3, y);	//���

		ShowCursorPos(x, y, plate[y][x / 3], 1);

		clt = _getch();					//Ű����� �Է��� ����
		if (clt == -32 || clt == 0)		//�Է¹��� Ű���尡 Ư�� Ű�ڵ��̸�
			clt = _getch();				//�ѹ� �� �о�� ����� �� �� ����

		ShowCursorPos(x, y, plate[y][x / 3], 0);

		switch (clt)
		{
		case LEFT:						//����Ű�̸�
			x -= 3;						//�������� �� ĭ(�� ��)�̵�
			break;
		case RIGHT:						//������ Ű�̸�
			x += 3;						//���������� �� ĭ(�� ��)�̵�
			break;
		case UP:						//���� Ű�̸�
			y -= 1;						//���� �� ĭ �̵�
			break;
		case DOWN:						//�Ʒ��� Ű�̸�
			y += 1;						//�Ʒ������� �� ĭ �̵�
			break;
		case ENTER:						//����Ű�̸�
			if (plate[y][x / 3] != '.')			//��ǥ�� ������� ������
				return 1;						//1�� ��ȯ�� ���� �޼��� ��� Ʈ���� Ȱ��ȭ
			else								//���������
			{
				plate[y][x / 3] = plate[0][0];  //���� ����
				ShowCursorPos(x, y, plate[y][x / 3], 0);
			}

			if (WinOrNot(plate, y, x / 3))		//���� ������ ��������
			{
				gotoxy(0, 20);
				Win(plate[0][0]);				//Win() ����

				gotoxy(0, 22);
				printf("Enter�� ���� �����, �����Ϸ��� �ƹ�Ű�� ��������.");
				clt = _getch();
				if(clt == ENTER)
				{
					clear();
				}		//�����
				else return -1;						//-1 ��ȯ
			}
			else								//������ �ʾ�����
			{
				return 0;						//0 ��ȯ
			}
		}

		if (x < 1 * 3)							//Ŀ���� ���� ��踦 �Ѿ��
			x = 19 * 3;							//������ ������ �̵�
		else if (x > 19 * 3)					//Ŀ���� ������ ��踦 �Ѿ��
			x = 1 * 3;							//���� ������ �̵�
		if (y < 1)								//Ŀ���� ���� ��踦 �Ѿ��
			y = 19;								//�Ʒ��� ������ �̵�
		else if (y > 19)						//Ŀ���� �Ʒ��� ��踦 �Ѿ��
			y = 1;								//���� ������ �̵�
	}
}