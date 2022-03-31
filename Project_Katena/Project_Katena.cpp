//holicmiku 1인 게임제작 프로젝트
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h> //_getch,_kbhit

#include<memory.h> //memset
#include<windows.h>//system 관련 함수
#include<mmsystem.h> //playsound 보조
#include "resource.h"

#pragma comment(lib,"winmm.lib")

//실제 스테이지에서 출력하는 노트의 위치들 gotoxy의 좌표값.
#define z 13 //version 1.0.1 14->13
#define x 18 //version 1.0.1 20->18
#define c 23 //version 1.0.1 26->23
#define v 28 //version 1.0.1 32->28


#define X1 12
#define left 75
#define right 77
#define Enter 13


short stage = 1; //현재의 스테이지
short music = 0; //노래성적 반영을 위한 몇번째 곡인지 알기위한 전역변수
int music_score = 0; //노래 성적
bool draw_bitmap = true; //비트맵 그리기 중 게임이 일정시간 멈추는현상을 해결하기위한 전역변수

int save_score1 = 0; //첫번째 곡의 스코어(플레이마다 제일 높은 점수가 갱신됨)
int save_score2 = 0; //두번째 곡의 스코어
int save_score3 = 0; //세번째 곡의 스코어

short Fix_Note = 1; 
//version 1.0.0 - 1.0.1에서의 튜토리얼 노트 제외 본게임 노트가 실제판정보다 1칸 더 빨리 없어지는 것을 해결하기위한 변수




void gotoxy(int x1, int y1) //좌표 이동 함수
{
	COORD Cur;
	Cur.X = x1;
	Cur.Y = y1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
void delete_Cursor()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void DrawBitmap(HDC hdc, int x1, int y1, HBITMAP hBit)  //비트맵 출력 api함수
{

	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x1, y1, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);

}

void bmp_draw()  //비트맵 출력 api함수
{
	int count = 1;

	HBITMAP hBit;
	HDC hdc;
	MSG msg;
	HWND hWnd;
	HINSTANCE hInst;


	SetConsoleTitle(TEXT("EASY TO DJ"));
	hWnd = FindWindow(NULL, TEXT("EASY TO DJ"));
	hInst = GetModuleHandle(NULL);
	hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));



	if (count == 1)
	{

		while (1) {

			hdc = GetDC(hWnd);
			DrawBitmap(hdc, 500, 240, hBit);
			
			Sleep(1);
			break;




		}
		count = 0;
		ReleaseDC(hWnd, hdc);
	}


}
void SetColor(int color) //글꼴 색상 지정 함수
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void pattern(char note[][15], int num, int num1) // 노래선택화면에서 애니메이션 출력부분 보조.
{
	int i;
	for (i = 0; i < num; i++)
	{
		note[11 - i][num1] = 64;
	}

}
void delete_line(int x1, int y1) // 현재 고정된 화면 가로 110, 세로 30 기준으로 만든 deleteline함수 , 컴퓨터마다 110 30의 기준이 다르므로 주의
{
	int delete_lines;
	gotoxy(x1, y1);
	for (delete_lines = 0; delete_lines < 110 - x1; delete_lines++)
	{
		printf(" ");
	}

}
void delete_line_for_make_space(int x1, int y1) // 한줄이 아닌 일정부분만 지우기 위해 제작
{
	int delete_lines_for_make_space;
	gotoxy(x1, y1);
	for (delete_lines_for_make_space = 0; delete_lines_for_make_space < 40; delete_lines_for_make_space++)
	{
		printf(" ");
	}
}
void delete_line_for_make_space1(int x1, int y1)
{
	int delete_lines_for_make_space1;
	gotoxy(x1, y1);
	for (delete_lines_for_make_space1 = 0; delete_lines_for_make_space1 < 34; delete_lines_for_make_space1++)
	{
		printf(" ");
	}
}
void delete_line_for_note(int x1, int y1) //4칸을 차지하는 노트만 지우는 함수
{
	int delete_lines;
	gotoxy(x1, y1);
	for (delete_lines = 0; delete_lines < 4; delete_lines++)
	{
		printf(" ");
	}

}
void array_move(char* array) //array 관련 함수들은 튜토리얼중 위아래에서 움직이는 타이틀 처리 관련 함수들입니다.
{
	int m;
	int string_length = strlen(array);
	char temp = array[0];
	for (m = 0; m < string_length - 1; m++)
	{
		array[m] = array[m + 1];
	}
	array[m] = temp;
}
void array_remove(char* array)
{
	int k;
	int string_length_1 = strlen(array);
	char temp_1 = array[string_length_1 - 1];
	for (k = string_length_1 - 1; k > 0; k--)
	{
		array[k] = array[k - 1];
	}
	array[0] = temp_1;
}
void array_slide(char* array)
{

	gotoxy(0, 0);
	array_move(array);
	printf("%s", array);
	Sleep(50);

}
void array_backslide(char* array)
{

	gotoxy(0, 29);
	array_remove(array);
	printf("%s", array);
	Sleep(50);

}
void simultaneous_move_plus_extra(char* array, char* array_1) //튜토리얼 중 나오는 모든 처리 과정
{
	int wait = 0;
	int wait_1 = 0; //튜토리얼 시간은 사람마다 다르므로 wait를 다시 지정해줍니다.
	int next_step = 0; //다음으로 가기 위한 조건
	char SKIP; //스킵
//
	char tutorial_note[24][25]; //튜토리얼용 노트 배열입니다.
	char groove_gauge[15][2]; //그루브게이지(HP) 배열입니다,
	int i, j; //for문을 위해 사용합니다.
	int note_z = 0; //노트 Z키에 해당되는 노트 1입니다.
	int note_x = 0;
	int note_c = 0;
	int note_v = 0;
	int good = 0, perfect = 0; // 판정처리
	int hit = 0; //good이든 perfect든 판정범위 내에 인식한 노트 수 입니다.
	int recover_groove_good = 0; // 그루브게이지를 회복하기 위한 조건
	int recover_groove_perfect = 0; //그루브게이지를 회복하기 위한 조건
	int miss = 0; //게임오버를 좌지우지하는 변수
	int if_z_press = 0;
	int if_x_press = 0;
	int if_c_press = 0;
	int if_v_press = 0;//아무키도 누르지않았을경우 Miss처리


	memset(tutorial_note, 48, sizeof(char) * 24 * 25);
	memset(groove_gauge, 42, sizeof(char) * 15 * 2);
	//	
	while (1)
	{
		gotoxy(0, 0);
		array_move(array);
		printf("%s", array);
		Sleep(50);
		gotoxy(0, 29);
		array_remove(array_1);
		printf("%s", array_1);
		Sleep(50);

		wait++;
		if (_kbhit())
		{
			SKIP = _getch();
			if (SKIP == Enter)
			{

				system("cls");
				break;
			}
			else
			{

			}
		}
		if (next_step == 0)
		{
			gotoxy(2, 1); SetColor(12);
			printf("H O W   T O  P L A Y");
			gotoxy(2, 2); SetColor(15);
			printf("기본 플레이 방법");


			gotoxy(47, 7);
			printf("******");
			gotoxy(46, 8);
			printf("*      *");
			gotoxy(40, 9);
			printf("E A Z Y  T O  D J™");
			gotoxy(46, 10);
			printf("*      *");
			gotoxy(47, 11);
			printf("******");
		}
		if (wait == 15 && next_step == 0)
		{
			gotoxy(24, 15);
			printf("EASY TO DJ는 키보드를 이용하여 음악을 연주하는 게임입니다.");
		}
		else if (wait == 30 && next_step == 0)
		{
			gotoxy(28, 19);
			printf("아래로 떨어지는 노트가 판정선과 정확히 일치할 때");
		}
		else if (wait == 40 && next_step == 0)
		{
			gotoxy(30, 20);
			printf("해당되는 입력 장치를 누르면 음이 연주됩니다.");

		}
		else if (wait == 60 && next_step == 0)
		{
			gotoxy(30, 20);
			printf("해당되는 입력 장치를 누르면 음이 연주됩니다.");
			next_step++;
		}

		if (next_step == 1)
		{
			delete_line(47, 7);
			delete_line(46, 8);
			delete_line(40, 9);
			delete_line(46, 10);
			delete_line(47, 11);
			delete_line(24, 15);
			delete_line(28, 19);
			delete_line(30, 20);
			next_step++;
		}
		else if (next_step == 2 && wait == 65)
		{
			gotoxy(18, 7);  printf("┏━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
			gotoxy(18, 8);  printf("┃ＫＥＹＢＯＡＲＤ┃┃      ＫＥＹＢＯＡＲＤ     ┃");
			gotoxy(18, 9);  printf("┃┏━━┓┏━━┓┏━━┓┏━━┓┃┃ ┏━━┓ ┏━━━━━━━━━━┓ ┏━━┓┏━━┓┃");
			gotoxy(18, 10); printf("┃┃Ｚ┃┃Ｘ┃┃Ｃ┃┃Ｖ┃┃┃ ┃F1┃ ┃ＥＮＴＥＲ┃ ┃←┃┃→┃┃");
			gotoxy(18, 11); printf("┃┗━━┛┗━━┛┗━━┛┗━━┛┃┃ ┗━━┛ ┗━━━━━━━━━━┛ ┗━━┛┗━━┛┃");
			gotoxy(18, 12); printf("┗━━━━━━━━━━━━━━━━┛┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
		}
		else if (next_step == 2 && wait == 80)
		{
			gotoxy(28, 16);
			printf("EASY TO DJ에 사용하는 입력장치는 다음과 같습니다.");
		}
		else if (next_step == 2 && wait == 95)
		{
			gotoxy(38, 20);
			printf("주 입력부인 대문자 ＺＸＣＶ");
		}
		else if (next_step == 2 && wait == 105)
		{
			gotoxy(22, 21);
			printf("보조 입력부인 F1 ＥＮＴＥＲ 방향키 ← →로 구성 되어있습니다.");
		}
		else if (next_step == 2 && wait == 125)
		{
			gotoxy(22, 21);
			printf("보조 입력부인 F1 ＥＮＴＥＲ 방향키 ← →로 구성 되어있습니다.");
			delete_line(28, 16);
			delete_line(38, 20);
			delete_line(22, 21);

		}
		else if (next_step == 2 && wait == 135)
		{

			gotoxy(28, 16);
			printf("F1키는 게임을 하기 위해 코인을 넣는 역할을 합니다.");
		}
		else if (next_step == 2 && wait == 150)
		{
			gotoxy(38, 20);
			printf("방향키 ← →는 곡을 선곡하며");
		}
		else if (next_step == 2 && wait == 160)
		{
			gotoxy(34, 21);
			printf("ＥＮＴＥＲ를 누르면 곡을 선택합니다.");
		}
		else if (next_step == 2 && wait == 180)
		{
			gotoxy(34, 21);
			printf("ＥＮＴＥＲ를 누르면 곡을 선택합니다.");
			next_step++;

		}

		if (next_step == 3)
		{
			delete_line(18, 7);
			delete_line(18, 8);
			delete_line(18, 9);
			delete_line(18, 10);
			delete_line(18, 11);
			delete_line(18, 12);
			delete_line(28, 16);
			delete_line(38, 20);
			delete_line(34, 21);
			next_step++;

		}
		else if (next_step == 4 && wait == 185)
		{
			gotoxy(66, 2);  printf("┃@@@@┃@@@@┃@@@@┃@@@@┃");
			gotoxy(66, 3);  printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 4);  printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 5);  printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 6);  printf("┃    ┃    ┃    ┃    ┣━┓");
			gotoxy(66, 7);  printf("┃    ┃    ┃    ┃    ┃ ┗┓");
			gotoxy(66, 8);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 9);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 10); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 11); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 12); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 13); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 14); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 15); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 16); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 17); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 18); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 19); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 20); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 21); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 22); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 23); printf("┣━━━━╋━━━━╋━━━━╋━━━━╋━━┛");
			gotoxy(66, 24); printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 25); printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 26); printf("┃ Ｚ ┃ Ｘ ┃ Ｃ ┃ Ｖ ┃");
			gotoxy(66, 27); printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 28); printf("┃    ┃    ┃    ┃    ┃");

		}
		else if (next_step == 4 && wait == 200)
		{
			gotoxy(18, 6);
			printf("[EASY TO DJ의 일반 노트 처리 방법]");
		}
		else if (next_step == 4 && wait == 215)
		{
			gotoxy(10, 13);
			printf("각 키에 해당하는 노트가 판정선에 정확히 일치할 때");
		}
		else if (next_step == 4 && wait == 225)
		{
			gotoxy(22, 14);
			printf("그 키를 눌러주시면 됩니다.");
		}
		else if (next_step == 4 && wait == 245)
		{
			gotoxy(22, 14);
			printf("그 키를 눌러주시면 됩니다.");
			next_step++;
		}

		if (next_step == 5)
		{
			delete_line(66, 2);
			delete_line(66, 3);
			delete_line(66, 4);
			delete_line(66, 5);
			delete_line(66, 6);
			delete_line(66, 7);
			delete_line(66, 8);
			delete_line(66, 9);
			delete_line(66, 10);
			delete_line(66, 11);
			delete_line(66, 12);
			delete_line(66, 13);
			delete_line(66, 14);
			delete_line(66, 15);
			delete_line(66, 16);
			delete_line(66, 17);
			delete_line(66, 18);
			delete_line(66, 19);
			delete_line(66, 20);
			delete_line(66, 21);
			delete_line(66, 22);
			delete_line(66, 23);
			delete_line(66, 24);
			delete_line(66, 25);
			delete_line(66, 26);

			delete_line_for_make_space1(66, 27);
			delete_line_for_make_space1(66, 28);

			delete_line(18, 6);
			delete_line(10, 13);
			delete_line(22, 14);
			next_step++;

		}
		else if (next_step == 6 && wait == 250)
		{
			gotoxy(66, 2);  printf("┃@@@@┃@@@@┃@@@@┃@@@@┃");
			gotoxy(66, 3);  printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 4);  printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 5);  printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 6);  printf("┃    ┃    ┃    ┃    ┣━┓");
			gotoxy(66, 7);  printf("┃    ┃    ┃    ┃    ┃ ┗┓");
			gotoxy(66, 8);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 9);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 10); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 11); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 12); printf("┃    ┃    ┃    ┃    ┃**┃  그");
			gotoxy(66, 13); printf("┃    ┃    ┃    ┃    ┃**┃  루");
			gotoxy(66, 14); printf("┃    ┃    ┃    ┃    ┃**┃  브");
			gotoxy(66, 15); printf("┃    ┃    ┃    ┃    ┃**┃←");
			gotoxy(66, 16); printf("┃    ┃    ┃    ┃    ┃**┃  게");
			gotoxy(66, 17); printf("┃    ┃    ┃    ┃    ┃**┃  이");
			gotoxy(66, 18); printf("┃    ┃    ┃    ┃    ┃**┃  지");
			gotoxy(66, 19); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 20); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 21); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 22); printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(66, 23); printf("┣━━━━╋━━━━╋━━━━╋━━━━╋━━┛");
			gotoxy(66, 24); printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 25); printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 26); printf("┃ Ｚ ┃ Ｘ ┃ Ｃ ┃ Ｖ ┃");
			gotoxy(66, 27); printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(66, 28); printf("┃    ┃    ┃    ┃    ┃");


		}
		else if (next_step == 6 && wait == 265)
		{
			gotoxy(10, 13);
			printf("그루브 게이지를 전부 소진하지 않고 연주를 완주하면");
		}
		else if (next_step == 6 && wait == 275)
		{
			gotoxy(16, 14);
			printf("다음 스테이지를 플레이 할 수 있습니다.");
		}
		else if (next_step == 6 && wait == 295)
		{
			gotoxy(16, 14);
			printf("다음 스테이지를 플레이 할 수 있습니다.");
			next_step++;
		}

		if (next_step == 7)
		{
			delete_line(66, 2);
			delete_line(66, 3);
			delete_line(66, 4);
			delete_line(66, 5);
			delete_line(66, 6);
			delete_line(66, 7);
			delete_line(66, 8);
			delete_line(66, 9);
			delete_line(66, 10);
			delete_line(66, 11);
			delete_line(66, 12);
			delete_line(66, 13);
			delete_line(66, 14);
			delete_line(66, 15);
			delete_line(66, 16);
			delete_line(66, 17);
			delete_line(66, 18);
			delete_line(66, 19);
			delete_line(66, 20);
			delete_line(66, 21);
			delete_line(66, 22);
			delete_line(66, 23);
			delete_line(66, 24);
			delete_line(66, 25);
			delete_line(66, 26);

			delete_line_for_make_space1(66, 27);
			delete_line_for_make_space1(66, 28);


			delete_line(10, 13);
			delete_line(16, 14);
			next_step++;
		}
		else if (next_step == 8 && wait == 300)
		{
			gotoxy(40, 14); printf("튜토리얼을 시작합니다!\n");

		}
		else if (next_step == 8 && wait == 320)
		{
			delete_line(40, 14);
			gotoxy(26, 2);   printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(26, 3);   printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(26, 4);   printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(26, 5);   printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(26, 6);   printf("┃    ┃    ┃    ┃    ┣━┓");
			gotoxy(26, 7);   printf("┃    ┃    ┃    ┃    ┃ ┗┓");
			gotoxy(26, 8);   printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 9);   printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 10);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 11);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 12);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 13);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 14);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 15);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 16);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 17);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 18);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 19);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 20);  printf("┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(19, 21);  printf("GOOD →┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(16, 22);  printf("PERFECT →┃    ┃    ┃    ┃    ┃**┃");
			gotoxy(26, 23);  printf("┣━━━━╋━━━━╋━━━━╋━━━━╋━━┛");
			gotoxy(26, 24);  printf("┃    ┃    ┃    ┃    ┃");
			gotoxy(26, 25);  printf("┃ Ｚ ┃ Ｘ ┃ Ｃ ┃ Ｖ ┃");
			gotoxy(26, 26);  printf("┃    ┃    ┃    ┃    ┃");

		}
		else if (next_step == 8 && wait == 335)
		{
			gotoxy(60, 13); printf("화살표로 표시되어있는 것이 판정선입니다.");
			gotoxy(62, 14); printf("실제 게임에서는 표시가 되지않습니다.");

		}
		else if (next_step == 8 && wait == 360)
		{
			delete_line(60, 13);
			delete_line(62, 14);
			gotoxy(58, 13); printf("Z키부터 순서대로 튜토리얼을 진행하겠습니다.");
			gotoxy(57, 14); printf("판정범위에 주의하여 노트를 2회 연주해주십시오.");
		}
		else if (next_step == 8 && wait == 380)
		{
			delete_line(58, 13);
			delete_line(57, 14);
			while (1)
			{
				gotoxy(0, 0);
				array_move(array);
				printf("%s", array);

				gotoxy(0, 29);
				array_remove(array_1);
				printf("%s", array_1);


				if_z_press = 0;


				if (note_z == 21)
				{	//version 1.0.1 28->27
					delete_line_for_note(27, 22);
					note_z = 0;
				}


				tutorial_note[note_z][2] = 64;
				tutorial_note[note_z][3] = 64;
				tutorial_note[note_z][4] = 64;
				tutorial_note[note_z][5] = 64;


				//노트 출력부분 및 그루브 게이지 출력부분 시작

				for (i = 0; i < 15; i++)
				{
					for (j = 0; j < 2; j++)
					{	//version 1.0.1 52->47
						gotoxy(47 + j, i + 8);
						if (groove_gauge[i][j] != 48)
						{
							printf("%c", groove_gauge[i][j]);
						}
						else
						{
							printf(" ");
						}
					}
					printf("\n");
				}
				//
				for (i = 0; i < 24; i++)
				{

					for (j = 0; j < 25; j++)
					{
						//version 1.0.1 26->25
						gotoxy(25 + j, i + 2);
						if (tutorial_note[i][j] == 64)
						{
							printf("%c", tutorial_note[i][j]);
						}

					}
				}


				//노트 출력부분 및 그루브 게이지 출력부분 끝	


				//잔상처리 
				//version 1.0.1 28->27
				note_z++;
				tutorial_note[note_z - 1][2] = 48;
				tutorial_note[note_z - 1][3] = 48;
				tutorial_note[note_z - 1][4] = 48;
				tutorial_note[note_z - 1][5] = 48;
				delete_line_for_note(27, note_z);
				Sleep(100);



				//  노트 처리부분


				if ((GetAsyncKeyState(0x5A) & 0x8000) && note_z == 20)
				{	//version 1.0.1 28->27
					delete_line_for_note(27, 21);
					note_z = 0;
					good++;
					recover_groove_good++;
					if_z_press++;
					hit++;
				}

				if ((GetAsyncKeyState(0x5A) & 0x8000) && note_z == 21)
				{
					perfect++;
					recover_groove_perfect++;
					if_z_press++;
					hit++;
				}








				gotoxy(56, 21); printf("GOOD %d", good);
				gotoxy(56, 22); printf("Perfect %d", perfect);

				if (hit == 2)
				{	//version 1.0.1 28->27
					delete_line_for_note(27, 22);
					next_step++;
					break;
				}



			}
		}

		if (next_step == 9)
		{

			hit = 0;
			while (1)
			{
				gotoxy(0, 0);
				array_move(array);
				printf("%s", array);

				gotoxy(0, 29);
				array_remove(array_1);
				printf("%s", array_1);


				if_x_press = 0;


				if (note_x == 21)
				{
					//version 1.0.1 34->32
					delete_line_for_note(32, 22);
					note_x = 0;
				}


				tutorial_note[note_x][2] = 64;
				tutorial_note[note_x][3] = 64;
				tutorial_note[note_x][4] = 64;
				tutorial_note[note_x][5] = 64;


				//노트 출력부분 및 그루브 게이지 출력부분 시작

				for (i = 0; i < 15; i++)
				{
					for (j = 0; j < 2; j++)
					{
						//version 1.0.1 52->47
						gotoxy(47 + j, i + 8);
						if (groove_gauge[i][j] != 48)
						{
							printf("%c", groove_gauge[i][j]);
						}
						else
						{
							printf(" ");
						}
					}
					printf("\n");
				}
				//
				for (i = 0; i < 24; i++)
				{

					for (j = 0; j < 25; j++)
					{
						//version 1.0.1 32->30
						gotoxy(30 + j, i + 2);
						if (tutorial_note[i][j] == 64)
						{
							printf("%c", tutorial_note[i][j]);
						}

					}
				}


				//노트 출력부분 및 그루브 게이지 출력부분 끝	


				//잔상처리 

				note_x++;
				tutorial_note[note_x - 1][2] = 48;
				tutorial_note[note_x - 1][3] = 48;
				tutorial_note[note_x - 1][4] = 48;
				tutorial_note[note_x - 1][5] = 48;
				delete_line_for_note(32, note_x);
				//version 1.0.1 34->32
				Sleep(100);



				//  노트 처리부분


				if ((GetAsyncKeyState(0x58) & 0x8000) && note_x == 20)
				{	//version 1.0.1 34->32
					delete_line_for_note(32, 21);
					note_x = 0;
					good++;
					if_x_press++;
					hit++;
				}

				if ((GetAsyncKeyState(0x58) & 0x8000) && note_x == 21)
				{
					perfect++;
					if_x_press++;
					hit++;
				}








				gotoxy(56, 21); printf("GOOD %d", good);
				gotoxy(56, 22); printf("Perfect %d", perfect);

				if (hit == 2)
				{
					//version 1.0.1 34->32
					delete_line_for_note(32, 22);
					next_step++;
					break;
				}



			}


		}
		if (next_step == 10)
		{
			hit = 0;
			while (1)
			{
				gotoxy(0, 0);
				array_move(array);
				printf("%s", array);

				gotoxy(0, 29);
				array_remove(array_1);
				printf("%s", array_1);


				if_c_press = 0;


				if (note_c == 21)
				{
					//version 1.0.1 40->37
					delete_line_for_note(37, 22);
					note_c = 0;
				}


				tutorial_note[note_c][2] = 64;
				tutorial_note[note_c][3] = 64;
				tutorial_note[note_c][4] = 64;
				tutorial_note[note_c][5] = 64;


				//노트 출력부분 및 그루브 게이지 출력부분 시작

				for (i = 0; i < 15; i++)
				{
					for (j = 0; j < 2; j++)
					{
						//version 1.0.1 52->47
						gotoxy(47 + j, i + 8);
						if (groove_gauge[i][j] != 48)
						{
							printf("%c", groove_gauge[i][j]);
						}
						else
						{
							printf(" ");
						}
					}
					printf("\n");
				}
				//
				for (i = 0; i < 24; i++)
				{

					for (j = 0; j < 25; j++)
					{
						//version 1.0.1 38->35
						gotoxy(35 + j, i + 2);
						if (tutorial_note[i][j] == 64)
						{
							printf("%c", tutorial_note[i][j]);
						}

					}
				}


				//노트 출력부분 및 그루브 게이지 출력부분 끝	


				//잔상처리 

				note_c++;
				tutorial_note[note_c - 1][2] = 48;
				tutorial_note[note_c - 1][3] = 48;
				tutorial_note[note_c - 1][4] = 48;
				tutorial_note[note_c - 1][5] = 48;
				delete_line_for_note(37, note_c);
				//version 1.0.1 40->37
				Sleep(100);



				//  노트 처리부분


				if ((GetAsyncKeyState(0x43) & 0x8000) && note_c == 20)
				{
					//version 1.0.1 40->37
					delete_line_for_note(37, 21);
					note_c = 0;
					good++;
					if_c_press++;
					hit++;
				}

				if ((GetAsyncKeyState(0x43) & 0x8000) && note_c == 21)
				{
					perfect++;
					if_c_press++;
					hit++;
				}








				gotoxy(56, 21); printf("GOOD %d", good);
				gotoxy(56, 22); printf("Perfect %d", perfect);

				if (hit == 2)
				{
					//version 1.0.1 40->37
					delete_line_for_note(37, 22);
					next_step++;
					break;
				}



			}
		}
		if (next_step == 11)
		{
			hit = 0;
			while (1)
			{
				gotoxy(0, 0);
				array_move(array);
				printf("%s", array);

				gotoxy(0, 29);
				array_remove(array_1);
				printf("%s", array_1);


				if_v_press = 0;


				if (note_v == 21)
				{
					//version 1.0.1 46->42
					delete_line_for_note(42, 22);
					note_v = 0;
				}


				tutorial_note[note_v][2] = 64;
				tutorial_note[note_v][3] = 64;
				tutorial_note[note_v][4] = 64;
				tutorial_note[note_v][5] = 64;


				//노트 출력부분 및 그루브 게이지 출력부분 시작

				for (i = 0; i < 15; i++)
				{
					for (j = 0; j < 2; j++)
					{
						//version 1.0.1 52->47
						gotoxy(47 + j, i + 8);
						if (groove_gauge[i][j] != 48)
						{
							printf("%c", groove_gauge[i][j]);
						}
						else
						{
							printf(" ");
						}
					}
					printf("\n");
				}
				//
				for (i = 0; i < 24; i++)
				{

					for (j = 0; j < 25; j++)
					{
						//version 1.0.1 44->40
						gotoxy(40 + j, i + 2);
						if (tutorial_note[i][j] == 64)
						{
							printf("%c", tutorial_note[i][j]);
						}

					}
				}


				//노트 출력부분 및 그루브 게이지 출력부분 끝	


				//잔상처리 

				note_v++;
				tutorial_note[note_v - 1][2] = 48;
				tutorial_note[note_v - 1][3] = 48;
				tutorial_note[note_v - 1][4] = 48;
				tutorial_note[note_v - 1][5] = 48;
				delete_line_for_note(42, note_v);
				//version 1.0.1 46->42
				Sleep(100);



				//  노트 처리부분


				if ((GetAsyncKeyState(0x56) & 0x8000) && note_v == 20)
				{
					//version 1.0.1 46->42
					delete_line_for_note(42, 21);
					note_v = 0;
					good++;
					if_v_press++;
					hit++;
				}

				if ((GetAsyncKeyState(0x56) & 0x8000) && note_v == 21)
				{
					perfect++;
					if_v_press++;
					hit++;
				}








				gotoxy(56, 21); printf("GOOD %d", good);
				gotoxy(56, 22); printf("Perfect %d", perfect);

				if (hit == 2)
				{
					//version 1.0.1 46->42
					delete_line_for_note(42, 22);
					next_step++;
					wait_1 = wait;
					break;
				}



			}
		}
		if (next_step == 12 && wait == wait_1 + 20)
		{

			gotoxy(60, 14); printf("이로써 튜토리얼을 모두 완수했습니다.");
		}
		if (next_step == 12 && wait == wait_1 + 40)
		{
			gotoxy(60, 14); printf("이로써 튜토리얼을 모두 완수했습니다.");
			system("cls");
			break;
		}


	}



}
void title_1()
{
	gotoxy(44, 4); SetColor(12);
	printf("! WARNING !");
	gotoxy(18, 6);
	printf("해당 문구와 게임에 나오는 모든 것들은 전부 허구성이 포함되어있습니다!");
	gotoxy(38, 7);
	printf("All of things are fiction!!");
	gotoxy(23, 8); SetColor(15);
	printf("THIS GAME IS ONLY FOR SALE AND USED IN REPUBLIC OF KOREA.");
	gotoxy(28, 9);
	printf("EXPORT, SALES, DISTRIBUTION, OPERATION OUTSIDE");
	gotoxy(30, 10);
	printf("THIS COUNTRY AND ILLEGAL EDIT & COPYING OF");
	gotoxy(23, 11);
	printf("THIS GAME IS PROHIBITED BY INTERNATIONAL COPYRIGHT LAWS");
	gotoxy(21, 11);
	printf("VIOLATORS ALL BE PROSECUTED TO THE MAXIMUM EXTENT POSSIBLE");
	gotoxy(25, 13);
	printf("이 게임은 대한민국 내에서만 유통 및 사용이 가능하며");
	gotoxy(23, 14);
	printf("이 게임 내의 모든 컨텐츠는 법적으로 보호 받고 있습니다.");
	gotoxy(27, 15);
	printf("타국으로의 수출, 판매 및 불법 개조, 복제 행위는");
	gotoxy(17, 16);
	printf("국제 법으로 금지되어있음으로 위반시 법적 처벌을 받을 수 있습니다.");
	gotoxy(23, 20);
	printf("E A Z Y  T O  D J  :  E N D L E S S   C I R C U L A T I O N");
	gotoxy(20, 21);
	printf("COPYRIGHT(C) 2016-2016 HYPERDIMENSION NEPTUNIA ALL RIGHT RESERVED\n");
	Sleep(5000);
	system("cls");
}
void title_2()
{
	int sec = 1;


	gotoxy(20, 3); SetColor(13); Sleep(sec * 100);
	printf("■■■■■■■■");
	gotoxy(16, 4); Sleep(sec * 100);
	printf("■■■■■■■■■■■■");
	gotoxy(12, 5); Sleep(sec * 100);
	printf("■■■■■■■■■■■■■■"); SetColor(15); printf("■■");
	gotoxy(8, 6); SetColor(13); Sleep(sec * 100);
	printf("■■■■■■■■■■■■■■■"); SetColor(15); printf("■■■■");
	gotoxy(12, 7); SetColor(13); Sleep(sec * 100);
	printf("■■■■■■■■■■■■■"); SetColor(15); printf("■■■■");
	gotoxy(12, 8); SetColor(13); Sleep(sec * 100);
	printf("■■■■■"); SetColor(14); printf("■"); SetColor(13); printf("■■■■■"); SetColor(14); printf("■"); SetColor(13); printf("■■"); SetColor(15); printf("■■");
	gotoxy(12, 9); SetColor(13); Sleep(sec * 100);
	printf("■■■■■"); SetColor(14); printf("■"); SetColor(13); printf("■■■■"); SetColor(14); printf("■■"); SetColor(13); printf("■■■■");
	gotoxy(10, 10); SetColor(13); Sleep(sec * 100);
	printf("■■■■■"); SetColor(14); printf("■"); SetColor(0); printf("  "); SetColor(14); printf("■"); SetColor(13); printf("■■"); SetColor(14); printf("■■"); SetColor(0); printf("  "); SetColor(14); printf("■"); SetColor(13); printf("■■■■");
	gotoxy(10, 11); SetColor(13); Sleep(sec * 100);
	printf("■■■■■"); SetColor(14); printf("■"); SetColor(0); printf("  "); SetColor(14); printf("■■■■■"); SetColor(0); printf("  "); SetColor(14); printf("■"); SetColor(13); printf("■■■■");
	gotoxy(10, 12); SetColor(13); Sleep(sec * 100);
	printf("■■■■■"); SetColor(14); printf("■"); SetColor(0); printf("  "); SetColor(14); printf("■■■■■"); SetColor(0); printf("  "); SetColor(14); printf("■"); SetColor(13); printf("■■■■");
	gotoxy(8, 13); SetColor(13); Sleep(sec * 100);
	printf("■■■■■■"); SetColor(14); printf("■■■■■■■■■"); SetColor(13); printf("■■■■■");
	gotoxy(8, 14); SetColor(13); Sleep(sec * 100);
	printf("■■■■■■■"); SetColor(14); printf("■■■■■■■"); SetColor(13); printf("■■■■■■");
	gotoxy(8, 15); SetColor(13); Sleep(sec * 100);
	printf("■■■"); SetColor(0); printf("  "); SetColor(13); printf("■■■"); SetColor(5); printf("■■"); SetColor(15); printf("■■■"); SetColor(5); printf("■■"); SetColor(13); printf("■■"); SetColor(0); printf("  "); SetColor(13); printf("■■■");
	gotoxy(10, 16); SetColor(13); Sleep(sec * 100);
	printf("■■"); SetColor(0); printf("  "); SetColor(13); printf("■■■"); SetColor(15); printf("■"); SetColor(5); printf("■■■■■"); SetColor(15); printf("■"); SetColor(13); printf("■■"); SetColor(0); printf("  "); SetColor(13); printf("■■■");
	gotoxy(10, 17); SetColor(13); Sleep(sec * 100);
	printf("■■■"); SetColor(0); printf("  "); SetColor(13); printf("■■"); SetColor(15); printf("■■"); SetColor(5); printf("■"); SetColor(15); printf("■"); SetColor(5); printf("■"); SetColor(15); printf("■■"); SetColor(13); printf("■"); SetColor(0); printf("  "); SetColor(13); printf("■■■");
	gotoxy(12, 18); SetColor(13); Sleep(sec * 100);
	printf("■■"); SetColor(0); printf("  "); SetColor(13); printf("■"); SetColor(15); printf("■■■"); SetColor(6); printf("■"); SetColor(15); printf("■"); SetColor(6); printf("■"); SetColor(15); printf("■■■"); SetColor(0); printf("  "); SetColor(13); printf("■■");
	gotoxy(12, 19); SetColor(13); Sleep(sec * 100);
	printf("■■"); SetColor(0); printf("  "); SetColor(13); printf("■"); SetColor(15); printf("■■■"); SetColor(6); printf("■"); SetColor(15); printf("■"); SetColor(6); printf("■"); SetColor(15); printf("■■■"); SetColor(0); printf("  "); SetColor(13); printf("■■");
	gotoxy(20, 20); SetColor(5); Sleep(sec * 100);
	printf("■■"); SetColor(6); printf("■"); SetColor(15); printf("■■■"); SetColor(6); printf("■"); SetColor(5); printf("■■");
	gotoxy(18, 21); SetColor(14); Sleep(sec * 100);
	printf("■"); SetColor(0); printf("  "); SetColor(15); printf("■■■"); SetColor(5); printf("■"); SetColor(15); printf("■■■"); SetColor(0); printf("  "); SetColor(14); printf("■");
	gotoxy(22, 22); SetColor(5); Sleep(sec * 100);
	printf("■■■"); printf("  "); SetColor(5); printf("■■■");
	gotoxy(24, 23); SetColor(15); Sleep(sec * 100);
	printf("■■"); printf("  "); SetColor(15); printf("■■");
	gotoxy(24, 24); SetColor(13); Sleep(sec * 100);
	printf("■■"); printf("  "); SetColor(13); printf("■■");
	gotoxy(24, 25); SetColor(5); Sleep(sec * 100);
	printf("■■"); printf("  "); SetColor(5); printf("■■");

	gotoxy(60, 12); SetColor(15);
	printf("H Y P E R D I M E N S I O N \n");
	gotoxy(60, 13);
	printf("      "); printf("N E P T U N I A \n");
	Sleep(2000);
	system("cls");


}

void square_circulation() //타이틀화면에서 움직이는 애니메이션, 게임시작 처리
{
	char circulation[10][10];
	char insert_coin;
	int a, b; // for문 변수
	int current_y;
	int A = 0, B = 0, C = 9, D = 9;
	int ready_to_play = 0;
	int current_credit = 0;
	int required_credit = 3;



	memset(circulation, 48, sizeof(char) * 10 * 10);

	for (b = 3; b < 7; b++)
	{
		circulation[3][b] = 42;
	}
	for (a = 4; a < 6; a++)
	{
		circulation[a][3] = 42;
		circulation[a][6] = 42;
	}
	for (b = 3; b < 7; b++)
	{
		circulation[6][b] = 42;
	}

	while (1)
	{

		if (_kbhit())
		{
			insert_coin = _getch();
			if (insert_coin == 0)
			{
				insert_coin = _getch();
				if (insert_coin == 59)
				{
					current_credit++;
					if (current_credit == 3)
					{

						ready_to_play++;
						current_credit = 0;
					}
				}
			}
			if (ready_to_play >= 1 && insert_coin == Enter)
			{
				ready_to_play--;
				break;
			}
		}

		current_y = 9;
		if (A == 9 && B == 9 && C == 0 && D == 0)
		{
			A = 0;
			B = 0;
			C = 9;
			D = 9;
		}

		circulation[0][A] = 64;
		circulation[B][9] = 64;
		circulation[9][C] = 64;
		circulation[D][0] = 64;

		for (a = 0; a < 10; a++)
		{
			gotoxy(40, current_y);
			for (b = 0; b < 10; b++)
			{
				if (circulation[a][b] != 48)
				{
					printf("%c ", circulation[a][b]);
				}
				else
				{
					printf("  ");
				}
			}
			printf("\n");
			current_y++;
		}


		A++;
		B++;
		C--;
		D--;

		circulation[0][A - 1] = 48;
		circulation[B - 1][9] = 48;
		circulation[9][C + 1] = 48;
		circulation[D + 1][0] = 48;

		if (ready_to_play >= 1)
		{


			gotoxy(36, 22);
			printf("PRESS ENTER BUTTON TO START");


		}


		Sleep(100);

		gotoxy(40, 27);
		printf("<CREDIT(S) %d  %d/%d>\n", ready_to_play, current_credit, required_credit);
		insert_coin = 10;
	}
}
void title_3()
{
	PlaySound(TEXT("main_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);



	gotoxy(0, 0);
	printf("E N D L E S S  C I R C U L A T I O N");
	gotoxy(47, 2);
	printf("******");
	gotoxy(46, 3);
	printf("*      *");
	gotoxy(40, 4);
	printf("E A Z Y  T O  D J™");
	gotoxy(46, 5);
	printf("*      *");
	gotoxy(47, 6);
	printf("******");
	gotoxy(37, 22);
	printf("Press F1 to INSERT COIN(S)");
	gotoxy(20, 25);
	printf("COPYRIGHT(C) 2016-2016 HYPERDIMENSION NEPTUNIA ALL RIGHT RESERVED\n");
	square_circulation();
	PlaySound(NULL, 0, 0);
	system("cls");




}
void title_to_tutorial() // 타이틀에서 튜토리얼로 가기 전 나오는 그림
{
	int second = 1;

	gotoxy(20, 3); SetColor(13); Sleep(second * 100);
	printf("■■■■■■■");
	gotoxy(16, 4); Sleep(second * 100);
	printf("■■■■■■■■■■■");
	gotoxy(14, 5); Sleep(second * 100);
	printf("■■■■■■■■■■■■■");
	gotoxy(12, 6); SetColor(15); Sleep(second * 100);
	printf("■■"); SetColor(13); printf("■■■■■■■■■■■"); SetColor(15); printf("■■");
	gotoxy(10, 7); SetColor(15); Sleep(second * 100);
	printf("■■■■"); SetColor(13); printf("■■■■■■■■■"); SetColor(15); printf("■■■■");
	gotoxy(10, 8); SetColor(15); Sleep(second * 100);
	printf("■■■■"); SetColor(13); printf("■■■■■■■■■"); SetColor(15); printf("■■■■");
	gotoxy(8, 9); SetColor(13); Sleep(second * 100);
	printf("■■"); SetColor(15); printf("■■"); SetColor(13); printf("■■■"); SetColor(14); printf("■"); SetColor(13); printf("■■■"); SetColor(14); printf("■"); SetColor(13); printf("■■■"); SetColor(15); printf("■■"); SetColor(13); printf("■■");
	gotoxy(6, 10); SetColor(13); Sleep(second * 100);
	printf("■■■■■■■"); SetColor(14); printf("■■■"); SetColor(13); printf("■■"); SetColor(14); printf("■■■"); SetColor(13); printf("■■■■■■■");
	gotoxy(10, 11); SetColor(13); Sleep(second * 100);
	printf("■■■■"); SetColor(14); printf("■"); SetColor(0); printf("■"); SetColor(14); printf("■■"); SetColor(13); printf("■■"); SetColor(14); printf("■■"); SetColor(0); printf("■"); SetColor(14); printf("■"); SetColor(13); printf("■■■■");
	gotoxy(8, 12); SetColor(13); Sleep(second * 100);
	printf("■■■■■"); SetColor(14); printf("■"); SetColor(0); printf("■"); SetColor(14); printf("■■■■■■"); SetColor(0); printf("■"); SetColor(14); printf("■"); SetColor(13); printf("■■■■■");
	gotoxy(6, 13); SetColor(13); Sleep(second * 100);
	printf("■■■■■■"); SetColor(14); printf("■"); SetColor(0); printf("■"); SetColor(14); printf("■■■■■■"); SetColor(0); printf("■"); SetColor(14); printf("■"); SetColor(13); printf("■■■■■■");
	gotoxy(10, 14); SetColor(13); Sleep(second * 100);
	printf("■"); SetColor(0); printf("■"); SetColor(13); printf("■■"); SetColor(14); printf("■■■■■■■■■■"); SetColor(13); printf("■■"); SetColor(0); printf("■"); SetColor(13); printf("■");
	gotoxy(14, 15); SetColor(13); Sleep(second * 100);
	printf("■■■"); SetColor(14); printf("■■■■■■■■"); SetColor(13); printf("■■■");
	gotoxy(16, 16); SetColor(13); Sleep(second * 100);
	printf("■"); SetColor(9); printf("■■"); SetColor(2); printf("■"); SetColor(15); printf("■■■■"); SetColor(2); printf("■"); SetColor(9); printf("■■"); SetColor(13); printf("■");
	gotoxy(20, 17); SetColor(9); Sleep(second * 100);
	printf("■■"); SetColor(2); printf("■■■■"); SetColor(9); printf("■■");
	gotoxy(20, 18); SetColor(15); Sleep(second * 100);
	printf("■■■"); SetColor(9); printf("■■"); SetColor(15); printf("■■■");
	gotoxy(20, 19); SetColor(15); Sleep(second * 100);
	printf("■■■"); SetColor(9); printf("■■"); SetColor(15); printf("■■■");
	gotoxy(18, 20); SetColor(15); Sleep(second * 100);
	printf("■■■■"); SetColor(2); printf("■■"); SetColor(15); printf("■■■■");
	gotoxy(18, 21); SetColor(9); Sleep(second * 100);
	printf("■"); SetColor(13); printf("■■"); SetColor(15); printf("■"); SetColor(2); printf("■■"); SetColor(15); printf("■"); SetColor(13); printf("■■"); SetColor(9); printf("■");
	gotoxy(16, 22); SetColor(14); Sleep(second * 100);
	printf("■"); SetColor(0); printf("■"); SetColor(13); printf("■"); SetColor(15); printf("■■"); SetColor(2); printf("■■"); SetColor(15); printf("■■"); SetColor(13); printf("■"); SetColor(0); printf("■"); SetColor(14); printf("■");
	gotoxy(20, 23); SetColor(9); Sleep(second * 100);
	printf("■■■"); SetColor(2); printf("■■"); SetColor(9); printf("■■■");
	gotoxy(22, 24); SetColor(14); Sleep(second * 100);
	printf("■■"); SetColor(0); printf("■■"); SetColor(14); printf("■■");
	gotoxy(22, 25); SetColor(2); Sleep(second * 100);
	printf("■■"); SetColor(0); printf("■■"); SetColor(2); printf("■■");
	gotoxy(22, 26); SetColor(9); Sleep(second * 100);
	printf("■■"); SetColor(0); printf("■■"); SetColor(9); printf("■■");

	gotoxy(60, 13); SetColor(15);
	printf("R U READY 2 GO INSINA DJ BOX?\n");
	Sleep(3000);
	system("cls"); gotoxy(34, 13);
	printf("E N D L E S S   C I R C U L A T I O N\n");
	Sleep(1000);
	system("cls");


}
void tutorial_1() // 튜토리얼 화면
{
	char tutorial_title[] = { "E N D L E S S   C I R C U L A T I O N               E N D L E S S   C I R C U L A T I O N               " };
	char tutorial_title_1[] = { "E N D L E S S   C I R C U L A T I O N               E N D L E S S   C I R C U L A T I O N               " };


	PlaySound(TEXT("tutorial_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);


	gotoxy(2, 28);
	printf("ENTER 버튼으로 SKIP 할 수 있습니다.");

	simultaneous_move_plus_extra(tutorial_title, tutorial_title_1);




}

void tutorial_to_select_music()
{
	gotoxy(41, 13);
	printf("이제 게임을 시작합니다!");
	gotoxy(38, 14);
	printf("R U READY 2 GO INSINA DJ BOX?\n");
	Sleep(3500);
	delete_line(41, 13);
	delete_line(38, 14);
}
void groove_gauge_set(char groove_gauge[][2], int miss)
{
	int i;

	if (miss >= 0)
	{
		for (i = miss; i >= 0; i--)
		{
			groove_gauge[i][0] = 42;
			groove_gauge[i][1] = 42;
		}
	}

	if (miss > 0 && miss < 16)
	{


		for (i = 0; i < miss; i++)
		{
			groove_gauge[i][0] = 48;
			groove_gauge[i][1] = 48;
		}



	}
}
void print_groove_gauge(char groove_gauge[][2])
{

	int i, j;



	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 2; j++)
		{ 
			//version 1.0.1 38->33 8->9
			gotoxy(33 + j, i + 9);
			if (groove_gauge[i][j] != 48)
			{
				printf("%c", groove_gauge[i][j]);
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}
int press_z_key(int judgement)
{
	if ((GetAsyncKeyState(0x5A) & 0x8000) && judgement == 1)
	{
		return 1;
	}

	if ((GetAsyncKeyState(0x5A) & 0x8000) && judgement == 2)
	{
		return 2;
	}

	return 0;
}
int press_x_key(int judgement)
{
	if ((GetAsyncKeyState(0x58) & 0x8000) && judgement == 1)
	{
		return 1;
	}

	if ((GetAsyncKeyState(0x58) & 0x8000) && judgement == 2)
	{
		return 2;
	}

	return 0;
}
int press_c_key(int judgement)
{
	if ((GetAsyncKeyState(0x43) & 0x8000) && judgement == 1)
	{
		return 1;
	}

	if ((GetAsyncKeyState(0x43) & 0x8000) && judgement == 2)
	{
		return 2;
	}

	return 0;
}
int press_v_key(int judgement)
{
	if ((GetAsyncKeyState(0x56) & 0x8000) && judgement == 1)
	{
		return 1;
	}

	if ((GetAsyncKeyState(0x56) & 0x8000) && judgement == 2)
	{
		return 2;
	}

	return 0;
}
void basic_pad()
{
	gotoxy(X1, 0);  printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 1);  printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 2);  printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 3);  printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 4);  printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 5);  printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 6);  printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 7);  printf("┃    ┃    ┃    ┃    ┣━┓\n");
	gotoxy(X1, 8);  printf("┃    ┃    ┃    ┃    ┃ ┗┓\n");
	gotoxy(X1, 9);  printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 10); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 11); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 12); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 13); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 14); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 15); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 16); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 17); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 18); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 19); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 20); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 21); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 22); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 23); printf("┃    ┃    ┃    ┃    ┃00┃\n");
	gotoxy(X1, 24); printf("┣━━━━╋━━━━╋━━━━╋━━━━╋━━┛\n");
	gotoxy(X1, 25); printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 26); printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 27); printf("┃ Ｚ ┃ Ｘ ┃ Ｃ ┃ Ｖ ┃\n");
	gotoxy(X1, 28); printf("┃    ┃    ┃    ┃    ┃\n");
	gotoxy(X1, 29); printf("┃    ┃    ┃    ┃    ┃\n");
}
void note(int x1, int i)
{
	gotoxy(x1, i); printf("@@@@");
}
void note_delete(int x1, int i)
{
	gotoxy(x1, i); printf("    ");
}
void score(int good, int perfect, int total_miss, int recover_groove_good, int recover_groove_perfect)
{
	gotoxy(70, 22); printf("recover_good %02d", recover_groove_good);
	gotoxy(70, 23); printf("recover_perfect %02d", recover_groove_perfect);
	gotoxy(70, 24); printf("MISS %02d", total_miss);
	gotoxy(70, 25); printf("GOOD %02d", good);
	gotoxy(70, 26); printf("Perfect %02d", perfect);

}
void result_score(int good, int perfect, int total_miss, int recover_groove_good, int recover_groove_perfect)
{
	gotoxy(40, 22); printf("recover_good %02d", recover_groove_good);
	gotoxy(40, 23); printf("recover_perfect %02d", recover_groove_perfect);
	gotoxy(40, 24); printf("MISS %02d", total_miss);
	gotoxy(40, 25); printf("GOOD %02d", good);
	gotoxy(40, 26); printf("Perfect %02d", perfect);
}
void note_first(int note_available, char key, int time)
{
	if (note_available == 1)
	{
		note(key, time);
	}
}
void note_advance(int note_available, char key, int time, int time1)
{
	if (note_available == 1 && time >= time1)
	{
		note(key, time - time1);
	}
}
void Music_1_Note()
{
	char groove_gauge[15][2];
	int z_1 = 1, z_2 = 1, z_3 = 1, z_4 = 1, z_5 = 1, z_6 = 1, z_7 = 1, z_8 = 1;
	int x_1 = 1, x_2 = 1, x_3 = 1, x_4 = 1, x_5 = 1, x_6 = 1, x_7 = 1, x_8 = 1;
	int c_1 = 1, c_2 = 1, c_3 = 1, c_4 = 1, c_5 = 1, c_6 = 1, c_7 = 1, c_8 = 1;
	int v_1 = 1, v_2 = 1, v_3 = 1, v_4 = 1, v_5 = 1, v_6 = 1, v_7 = 1, v_8 = 1;
	int judge = 0;
	int good = 0, perfect = 0, miss = 0, total_miss = 0;
	int recover_groove_good = 0; // 그루브게이지를 회복하기 위한 조건
	int recover_groove_perfect = 0; //그루브게이지를 회복하기 위한 조건
	int start_music = 1;
	int note_time = 0;
	int game_over_pass_key = 0;
	

	memset(groove_gauge, 42, sizeof(char) * 15 * 2);

	basic_pad();
	good = 0, perfect = 0, miss = 0, total_miss = 0;
	recover_groove_good = 0;
	recover_groove_perfect = 0;

	while (1)
	{
		//23->24
		if (note_time == 24)
		{

			PlaySound(TEXT("Music1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		}

		if ((groove_gauge[0][0] != 42) && recover_groove_good == 4)
		{
			miss--;
			recover_groove_good = 0;
		}

		if ((groove_gauge[0][0] != 42) && recover_groove_perfect == 2)
		{
			miss--;
			recover_groove_perfect = 0;
		}




		groove_gauge_set(groove_gauge, miss);
		print_groove_gauge(groove_gauge);

		note_first(z_1, z, note_time);
		note_advance(c_1, c, note_time, 6);
		note_advance(v_1, v, note_time, 10);

		note_advance(z_2, z, note_time, 25);
		note_advance(c_2, c, note_time, 31);
		note_advance(v_2, v, note_time, 35);

		note_advance(z_3, z, note_time, 50);
		note_advance(c_3, c, note_time, 54);
		note_advance(v_3, v, note_time, 58);
		note_advance(v_4, v, note_time, 66);
		note_advance(c_4, c, note_time, 74);
		note_advance(x_1, x, note_time, 80);
		note_advance(z_4, z, note_time, 84);

		note_advance(z_5, z, note_time, 98);
		note_advance(c_5, c, note_time, 104);
		note_advance(v_5, v, note_time, 108);

		note_advance(z_6, z, note_time, 123);
		note_advance(c_6, c, note_time, 129);
		note_advance(v_6, v, note_time, 133);

		note_advance(z_7, z, note_time, 148);
		note_advance(c_7, c, note_time, 152);
		note_advance(v_7, v, note_time, 156);
		note_advance(v_8, v, note_time, 164);
		note_advance(c_8, c, note_time, 172);
		note_advance(x_2, x, note_time, 178);
		note_advance(z_8, z, note_time, 182);


		//노트의 속도 현재 조정중..
		Sleep(104);


		if (z_1 == 1)
		{
			note_delete(z, note_time);
		}

		if (c_1 == 1 && note_time >= 6)
		{
			note_delete(c, note_time - 6);
		}

		if (v_1 == 1 && note_time >= 10)
		{
			note_delete(v, note_time - 10);
		}

		if (z_2 == 1 && note_time >= 25)
		{
			note_delete(z, note_time - 25);
		}

		if (c_2 == 1 && note_time >= 31)
		{
			note_delete(c, note_time - 31);
		}

		if (v_2 == 1 && note_time >= 35)
		{
			note_delete(v, note_time - 35);
		}

		if (z_3 == 1 && note_time >= 50)
		{
			note_delete(z, note_time - 50);
		}

		if (c_3 == 1 && note_time >= 54)
		{
			note_delete(c, note_time - 54);
		}

		if (v_3 == 1 && note_time >= 58)
		{
			note_delete(v, note_time - 58);
		}

		if (v_4 == 1 && note_time >= 66)
		{
			note_delete(v, note_time - 66);
		}

		if (c_4 == 1 && note_time >= 74)
		{
			note_delete(c, note_time - 74);
		}

		if (x_1 == 1 && note_time >= 80)
		{
			note_delete(x, note_time - 80);
		}

		if (z_4 == 1 && note_time >= 84)
		{
			note_delete(z, note_time - 84);
		}

		if (z_5 == 1 && note_time >= 98)
		{
			note_delete(z, note_time - 98);
		}

		if (c_5 == 1 && note_time >= 104)
		{
			note_delete(c, note_time - 104);
		}

		if (v_5 == 1 && note_time >= 108)
		{
			note_delete(v, note_time - 108);
		}

		if (z_6 == 1 && note_time >= 123)
		{
			note_delete(z, note_time - 123);
		}

		if (c_6 == 1 && note_time >= 129)
		{
			note_delete(c, note_time - 129);
		}

		if (v_6 == 1 && note_time >= 133)
		{
			note_delete(v, note_time - 133);
		}

		if (z_7 == 1 && note_time >= 148)
		{
			note_delete(z, note_time - 148);
		}

		if (c_7 == 1 && note_time >= 152)
		{
			note_delete(c, note_time - 152);
		}

		if (v_7 == 1 && note_time >= 156)
		{
			note_delete(v, note_time - 156);
		}

		if (v_8 == 1 && note_time >= 164)
		{
			note_delete(v, note_time - 164);
		}

		if (c_8 == 1 && note_time >= 172)
		{
			note_delete(c, note_time - 172);
		}

		if (x_2 == 1 && note_time >= 178)
		{
			note_delete(x, note_time - 178);
		}

		if (z_8 == 1 && note_time >= 182)
		{
			note_delete(z, note_time - 182);
		}
		//

		if (note_time == 22)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 23 && z_1 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 27)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 28 && c_1 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 31)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 32 && v_1 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 46)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 47 && z_2 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 52)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 53 && c_2 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 56)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 57 && v_2 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 71)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 72 && z_3 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 75)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 76 && c_3 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 79)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 80 && v_3 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 87)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 88 && v_4 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_4 = 0;
			}
			judge = 0;
		}

		if (note_time == 95)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 96 && c_4 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_4 = 0;
			}
			judge = 0;
		}

		if (note_time == 101)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 102 && x_1 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 105)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 106 && z_4 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_4 = 0;
			}
			judge = 0;
		}

		if (note_time == 119)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_5 = 0;
			}
			judge = 0;
		}
		if (note_time == 120 && z_5 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_5 = 0;
			}
			judge = 0;
		}

		if (note_time == 125)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_5 = 0;
			}
			judge = 0;
		}
		if (note_time == 126 && c_5 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_5 = 0;
			}
			judge = 0;
		}

		if (note_time == 129)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_5 = 0;
			}
			judge = 0;
		}
		if (note_time == 130 && v_5 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_5 = 0;
			}
			judge = 0;
		}

		if (note_time == 144)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_6 = 0;
			}
			judge = 0;
		}
		if (note_time == 145 && z_6 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_6 = 0;
			}
			judge = 0;
		}

		if (note_time == 150)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_6 = 0;
			}
			judge = 0;
		}
		if (note_time == 151 && c_6 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_6 = 0;
			}
			judge = 0;
		}

		if (note_time == 154)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_6 = 0;
			}
			judge = 0;
		}
		if (note_time == 155 && v_6 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_6 = 0;
			}
			judge = 0;
		}

		if (note_time == 169)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_7 = 0;
			}
			judge = 0;
		}
		if (note_time == 170 && z_7 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_7 = 0;
			}
			judge = 0;
		}

		if (note_time == 173)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_7 = 0;
			}
			judge = 0;
		}
		if (note_time == 174 && c_7 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_7 = 0;
			}
			judge = 0;
		}

		if (note_time == 177)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_7 = 0;
			}
			judge = 0;
		}
		if (note_time == 178 && v_7 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_7 = 0;
			}
			judge = 0;
		}

		if (note_time == 185)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_8 = 0;
			}
			judge = 0;
		}
		if (note_time == 186 && v_8 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_8 = 0;
			}
			judge = 0;
		}

		if (note_time == 193)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_8 = 0;
			}
			judge = 0;
		}
		if (note_time == 194 && c_8 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_8 = 0;
			}
			judge = 0;
		}

		if (note_time == 199)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 200 && x_2 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 203)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_8 = 0;
			}
			judge = 0;
		}
		if (note_time == 204 && z_8 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_8 = 0;
			}
			judge = 0;
		}


		note_time++;


		//      miss의 처리


		if ((note_time == 23 + Fix_Note) && z_1 == 1)
		{
			z_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 29 + Fix_Note) && c_1 == 1)
		{
			c_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 33 + Fix_Note) && v_1 == 1)
		{
			v_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 48 + Fix_Note) && z_2 == 1)
		{
			z_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 54 + Fix_Note) && c_2 == 1)
		{
			c_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 58 + Fix_Note) && v_2 == 1)
		{
			v_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 73 + Fix_Note) && z_3 == 1)
		{
			z_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 77 + Fix_Note) && c_3 == 1)
		{
			c_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 81 + Fix_Note) && v_3 == 1)
		{
			v_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 89 + Fix_Note) && v_4 == 1)
		{
			v_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 97 + Fix_Note) && c_4 == 1)
		{
			c_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 103 + Fix_Note) && x_1 == 1)
		{
			x_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 107 + Fix_Note) && z_4 == 1)
		{
			z_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 121 + Fix_Note) && z_5 == 1)
		{
			z_5 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 127 + Fix_Note) && c_5 == 1)
		{
			c_5 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 131 + Fix_Note) && v_5 == 1)
		{
			v_5 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 146 + Fix_Note) && z_6 == 1)
		{
			z_6 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 152 + Fix_Note) && c_6 == 1)
		{
			c_6 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 156 + Fix_Note) && v_6 == 1)
		{
			v_6 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 171 + Fix_Note) && z_7 == 1)
		{
			z_7 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 175 + Fix_Note) && c_7 == 1)
		{
			c_7 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 179 + Fix_Note) && v_7 == 1)
		{
			v_7 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 187 + Fix_Note) && v_8 == 1)
		{
			v_8 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 195 + Fix_Note) && c_8 == 1)
		{
			c_8 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 201 + Fix_Note) && x_2 == 1)
		{
			x_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if ((note_time == 205 + Fix_Note) && z_8 == 1)
		{
			z_8 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (miss == 15)
		{
			PlaySound(NULL, 0, 0);
			system("cls");
			break;

		}
		if (note_time == 218)
		{
			PlaySound(NULL, 0, 0);
			system("cls");
			break;
		}
		score(good, perfect, total_miss, recover_groove_good, recover_groove_perfect);
	}
	if (miss == 15)
	{
		PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		while (1)
		{
			gotoxy(45, 14); printf("ＧＡＭＥ ＯＶＥＲ");
			Sleep(100);

			if (_kbhit())
			{
				game_over_pass_key = _getch();
				if (game_over_pass_key == Enter)
				{
					PlaySound(NULL, 0, 0);
					break;
				}
			}
		}
	}

	if (note_time == 218 && miss < 15)
	{
		gotoxy(42, 14); printf("결과를 집계중입니다.");
		Sleep(3000);
		delete_line(42, 14);
		gotoxy(45, 10); printf(" R E S U L T");
		gotoxy(46, 12); printf("Perfect   %02d", perfect);
		gotoxy(46, 14); printf("GOOD      %02d", good);
		gotoxy(46, 16); printf("MISS      %02d", total_miss);
		gotoxy(46, 18); printf("Score     %04d", perfect * 300 + good * 100);
		Sleep(4000);
		stage++;
		music = 1;
		music_score = perfect * 300 + good * 100;
		draw_bitmap = true;
	}


}
void Music_2_Note()
{
	char groove_gauge[15][2];
	int z_1 = 1, z_2 = 1, z_3 = 1, z_4 = 1, z_5 = 1, z_6 = 1, z_7 = 1, z_8 = 1;
	int x_1 = 1, x_2 = 1, x_3 = 1, x_4 = 1, x_5 = 1, x_6 = 1, x_7 = 1, x_8 = 1;
	int c_1 = 1, c_2 = 1, c_3 = 1, c_4 = 1, c_5 = 1, c_6 = 1, c_7 = 1, c_8 = 1;
	int v_1 = 1, v_2 = 1, v_3 = 1, v_4 = 1, v_5 = 1, v_6 = 1, v_7 = 1, v_8 = 1;
	int judge = 0;
	int good = 0, perfect = 0, miss = 0, total_miss = 0;
	int recover_groove_good = 0; // 그루브게이지를 회복하기 위한 조건
	int recover_groove_perfect = 0; //그루브게이지를 회복하기 위한 조건
	int start_music = 1;
	int note_time = 0;
	int game_over_pass_key = 0;


	memset(groove_gauge, 42, sizeof(char) * 15 * 2);

	basic_pad();


	while (1)
	{

		if (note_time == 23)
		{

			PlaySound(TEXT("Music2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		}

		if ((groove_gauge[0][0] != 42) && recover_groove_good == 4)
		{
			miss--;
			recover_groove_good = 0;
		}

		if ((groove_gauge[0][0] != 42) && recover_groove_perfect == 2)
		{
			miss--;
			recover_groove_perfect = 0;
		}




		groove_gauge_set(groove_gauge, miss);
		print_groove_gauge(groove_gauge);

		note_first(z_1, z, note_time);
		note_advance(v_1, v, note_time, 16);
		note_advance(x_1, x, note_time, 30);
		note_advance(c_1, c, note_time, 44);

		note_advance(z_2, z, note_time, 60);
		note_advance(x_2, x, note_time, 74);
		note_advance(v_2, v, note_time, 88);
		note_advance(c_2, c, note_time, 124);

		note_advance(v_3, v, note_time, 138);
		note_advance(x_3, x, note_time, 152);
		note_advance(c_3, c, note_time, 164);
		note_advance(z_3, z, note_time, 178);

		note_advance(z_4, z, note_time, 192);
		note_advance(x_4, x, note_time, 210);




		Sleep(104);


		if (z_1 == 1)
		{
			note_delete(z, note_time);
		}

		if (v_1 == 1 && note_time >= 16)
		{
			note_delete(v, note_time - 16);
		}

		if (x_1 == 1 && note_time >= 30)
		{
			note_delete(x, note_time - 30);
		}

		if (c_1 == 1 && note_time >= 44)
		{
			note_delete(c, note_time - 44);
		}

		if (z_2 == 1 && note_time >= 60)
		{
			note_delete(z, note_time - 60);
		}

		if (x_2 == 1 && note_time >= 74)
		{
			note_delete(x, note_time - 74);
		}

		if (v_2 == 1 && note_time >= 88)
		{
			note_delete(v, note_time - 88);
		}

		if (c_2 == 1 && note_time >= 124)
		{
			note_delete(c, note_time - 124);
		}

		if (v_3 == 1 && note_time >= 138)
		{
			note_delete(v, note_time - 138);
		}

		if (x_3 == 1 && note_time >= 152)
		{
			note_delete(x, note_time - 152);
		}

		if (c_3 == 1 && note_time >= 164)
		{
			note_delete(c, note_time - 164);
		}

		if (z_3 == 1 && note_time >= 178)
		{
			note_delete(z, note_time - 178);
		}

		if (z_4 == 1 && note_time >= 192)
		{
			note_delete(z, note_time - 192);
		}

		if (x_4 == 1 && note_time >= 210)
		{
			note_delete(x, note_time - 210);
		}




		//

		if (note_time == 21)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 22 && z_1 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 37)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 38 && v_1 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 51)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 52 && x_1 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 65)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 66 && c_1 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 81)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 82 && z_2 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 95)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 96 && x_2 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 109)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 110 && v_2 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 145)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 146 && c_2 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 159)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 160 && v_3 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 173)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 174 && x_3 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 185)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 186 && c_3 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 199)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 200 && z_3 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 213)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 214 && z_4 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_4 = 0;
			}
			judge = 0;
		}

		if (note_time == 231)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 232 && x_4 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_4 = 0;
			}
			judge = 0;
		}




		note_time++;


		if (note_time == 23 + Fix_Note && z_1 == 1)
		{
			z_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 39 + Fix_Note && v_1 == 1)
		{
			v_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 53 + Fix_Note && x_1 == 1)
		{
			x_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 67 + Fix_Note && c_1 == 1)
		{
			c_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 83 + Fix_Note && z_2 == 1)
		{
			z_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 97 + Fix_Note && x_2 == 1)
		{
			x_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 111 + Fix_Note && v_2 == 1)
		{
			v_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 147 + Fix_Note && c_2 == 1)
		{
			c_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 161 + Fix_Note && v_3 == 1)
		{
			v_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 175 + Fix_Note && x_3 == 1)
		{
			x_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 187 + Fix_Note && c_3 == 1)
		{
			c_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 201 + Fix_Note && z_3 == 1)
		{
			z_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 215 + Fix_Note && z_4 == 1)
		{
			z_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 233 + Fix_Note && x_4 == 1)
		{
			x_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}
		if (miss == 15)
		{
			PlaySound(NULL, 0, 0);
			system("cls");
			break;

		}
		if (note_time == 260)
		{
			PlaySound(NULL, 0, 0);
			system("cls");
			break;
		}

		score(good, perfect, total_miss, recover_groove_good, recover_groove_perfect);
	}
	if (miss == 15)
	{
		PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		while (1)
		{
			gotoxy(45, 14); printf("ＧＡＭＥ ＯＶＥＲ");
			Sleep(100);

			if (_kbhit())
			{
				game_over_pass_key = _getch();
				if (game_over_pass_key == Enter)
				{
					PlaySound(NULL, 0, 0);
					break;
				}
			}
		}
	}

	if (note_time == 260 && miss < 15)
	{
		gotoxy(42, 14); printf("결과를 집계중입니다.");
		Sleep(3000);
		delete_line(42, 14);
		gotoxy(45, 10); printf(" R E S U L T");
		gotoxy(46, 12); printf("Perfect   %02d", perfect);
		gotoxy(46, 14); printf("GOOD      %02d", good);
		gotoxy(46, 16); printf("MISS      %02d", total_miss);
		gotoxy(46, 18); printf("Score     %04d", perfect * 300 + good * 100);
		Sleep(4000);
		stage++;
		music = 2;
		music_score = perfect * 300 + good * 100;
		draw_bitmap = true;
	}
}
void Music_3_Note()
{
	char groove_gauge[15][2];
	int z_1 = 1, z_2 = 1, z_3 = 1, z_4 = 1, z_5 = 1, z_6 = 1, z_7 = 1, z_8 = 1, z_9 = 1, z_10 = 1, z_11 = 1, z_12 = 1;
	int x_1 = 1, x_2 = 1, x_3 = 1, x_4 = 1, x_5 = 1, x_6 = 1, x_7 = 1, x_8 = 1, x_9 = 1, x_10 = 1, x_11 = 1, x_12 = 1;
	int c_1 = 1, c_2 = 1, c_3 = 1, c_4 = 1, c_5 = 1, c_6 = 1, c_7 = 1, c_8 = 1, c_9 = 1, c_10 = 1, c_11 = 1, c_12 = 1;
	int v_1 = 1, v_2 = 1, v_3 = 1, v_4 = 1, v_5 = 1, v_6 = 1, v_7 = 1, v_8 = 1, v_9 = 1, v_10 = 1, v_11 = 1, v_12 = 1;
	int judge = 0;
	int good = 0, perfect = 0, miss = 0, total_miss = 0;
	int recover_groove_good = 0; // 그루브게이지를 회복하기 위한 조건
	int recover_groove_perfect = 0; //그루브게이지를 회복하기 위한 조건
	int start_music = 1;
	int note_time = 0;
	int game_over_pass_key = 0;



	memset(groove_gauge, 42, sizeof(char) * 15 * 2);

	basic_pad();


	while (1)
	{

		if (note_time == 23)
		{

			PlaySound(TEXT("Music3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		}

		if ((groove_gauge[0][0] != 42) && recover_groove_good == 4)
		{
			miss--;
			recover_groove_good = 0;
		}

		if ((groove_gauge[0][0] != 42) && recover_groove_perfect == 2)
		{
			miss--;
			recover_groove_perfect = 0;
		}




		groove_gauge_set(groove_gauge, miss);
		print_groove_gauge(groove_gauge);

		note_first(z_1, z, note_time);
		note_advance(v_1, v, note_time, 4);
		note_advance(z_2, z, note_time, 9);
		note_advance(v_2, v, note_time, 13);
		note_advance(z_3, z, note_time, 17);
		note_advance(v_3, v, note_time, 21);
		note_advance(z_4, z, note_time, 25);
		note_advance(v_4, v, note_time, 29);

		note_advance(x_1, x, note_time, 34);
		note_advance(c_1, c, note_time, 38);
		note_advance(x_2, x, note_time, 42);
		note_advance(c_2, c, note_time, 46);
		note_advance(x_3, x, note_time, 50);
		note_advance(c_3, c, note_time, 54);
		note_advance(x_4, x, note_time, 58);
		note_advance(c_4, c, note_time, 62);

		note_advance(z_5, z, note_time, 67);
		note_advance(c_5, c, note_time, 71);
		note_advance(z_6, z, note_time, 75);
		note_advance(c_6, c, note_time, 79);
		note_advance(z_7, z, note_time, 83);
		note_advance(c_7, c, note_time, 87);
		note_advance(z_8, z, note_time, 91);
		note_advance(c_8, c, note_time, 95);

		note_advance(x_5, x, note_time, 100);
		note_advance(v_5, v, note_time, 104);
		note_advance(x_6, x, note_time, 108);
		note_advance(v_6, v, note_time, 112);
		note_advance(x_7, x, note_time, 116);
		note_advance(v_7, v, note_time, 120);
		note_advance(x_8, x, note_time, 124);
		note_advance(v_8, v, note_time, 128);

		note_advance(z_9, z, note_time, 133);
		note_advance(x_9, x, note_time, 137);
		note_advance(c_9, c, note_time, 141);
		note_advance(v_9, v, note_time, 145);
		note_advance(c_10, c, note_time, 149);
		note_advance(x_10, x, note_time, 153);
		note_advance(z_10, z, note_time, 157);
		note_advance(x_11, x, note_time, 161);

		note_advance(c_11, c, note_time, 166);
		note_advance(v_10, v, note_time, 170);
		note_advance(c_12, c, note_time, 174);
		note_advance(x_12, x, note_time, 178);
		note_advance(z_11, z, note_time, 182);

		note_advance(z_12, z, note_time, 191);






		Sleep(112);


		if (z_1 == 1)
		{
			note_delete(z, note_time);
		}

		if (v_1 == 1 && note_time >= 4)
		{
			note_delete(v, note_time - 4);
		}

		if (z_2 == 1 && note_time >= 9)
		{
			note_delete(z, note_time - 9);
		}

		if (v_2 == 1 && note_time >= 13)
		{
			note_delete(v, note_time - 13);
		}

		if (z_3 == 1 && note_time >= 17)
		{
			note_delete(z, note_time - 17);
		}

		if (v_3 == 1 && note_time >= 21)
		{
			note_delete(v, note_time - 21);
		}

		if (z_4 == 1 && note_time >= 25)
		{
			note_delete(z, note_time - 25);
		}

		if (v_4 == 1 && note_time >= 29)
		{
			note_delete(v, note_time - 29);
		}

		if (x_1 == 1 && note_time >= 34)
		{
			note_delete(x, note_time - 34);
		}

		if (c_1 == 1 && note_time >= 38)
		{
			note_delete(c, note_time - 38);
		}

		if (x_2 == 1 && note_time >= 42)
		{
			note_delete(x, note_time - 42);
		}

		if (c_2 == 1 && note_time >= 46)
		{
			note_delete(c, note_time - 46);
		}

		if (x_3 == 1 && note_time >= 50)
		{
			note_delete(x, note_time - 50);
		}

		if (c_3 == 1 && note_time >= 54)
		{
			note_delete(c, note_time - 54);
		}

		if (x_4 == 1 && note_time >= 58)
		{
			note_delete(x, note_time - 58);
		}

		if (c_4 == 1 && note_time >= 62)
		{
			note_delete(c, note_time - 62);
		}

		if (z_5 == 1 && note_time >= 67)
		{
			note_delete(z, note_time - 67);
		}

		if (c_5 == 1 && note_time >= 71)
		{
			note_delete(c, note_time - 71);
		}

		if (z_6 == 1 && note_time >= 75)
		{
			note_delete(z, note_time - 75);
		}

		if (c_6 == 1 && note_time >= 79)
		{
			note_delete(c, note_time - 79);
		}

		if (z_7 == 1 && note_time >= 83)
		{
			note_delete(z, note_time - 83);
		}

		if (c_7 == 1 && note_time >= 87)
		{
			note_delete(c, note_time - 87);
		}

		if (z_8 == 1 && note_time >= 91)
		{
			note_delete(z, note_time - 91);
		}

		if (c_8 == 1 && note_time >= 95)
		{
			note_delete(c, note_time - 95);
		}

		if (x_5 == 1 && note_time >= 100)
		{
			note_delete(x, note_time - 100);
		}

		if (v_5 == 1 && note_time >= 104)
		{
			note_delete(v, note_time - 104);
		}

		if (x_6 == 1 && note_time >= 108)
		{
			note_delete(x, note_time - 108);
		}

		if (v_6 == 1 && note_time >= 112)
		{
			note_delete(v, note_time - 112);
		}

		if (x_7 == 1 && note_time >= 116)
		{
			note_delete(x, note_time - 116);
		}

		if (v_7 == 1 && note_time >= 120)
		{
			note_delete(v, note_time - 120);
		}

		if (x_8 == 1 && note_time >= 124)
		{
			note_delete(x, note_time - 124);
		}

		if (v_8 == 1 && note_time >= 128)
		{
			note_delete(v, note_time - 128);
		}

		if (z_9 == 1 && note_time >= 133)
		{
			note_delete(z, note_time - 133);
		}

		if (x_9 == 1 && note_time >= 137)
		{
			note_delete(x, note_time - 137);
		}

		if (c_9 == 1 && note_time >= 141)
		{
			note_delete(c, note_time - 141);
		}

		if (v_9 == 1 && note_time >= 145)
		{
			note_delete(v, note_time - 145);
		}

		if (c_10 == 1 && note_time >= 149)
		{
			note_delete(c, note_time - 149);
		}

		if (x_10 == 1 && note_time >= 153)
		{
			note_delete(x, note_time - 153);
		}

		if (z_10 == 1 && note_time >= 157)
		{
			note_delete(z, note_time - 157);
		}

		if (x_11 == 1 && note_time >= 161)
		{
			note_delete(x, note_time - 161);
		}

		if (c_11 == 1 && note_time >= 166)
		{
			note_delete(c, note_time - 166);
		}

		if (v_10 == 1 && note_time >= 170)
		{
			note_delete(v, note_time - 170);
		}

		if (c_12 == 1 && note_time >= 174)
		{
			note_delete(c, note_time - 174);
		}

		if (x_12 == 1 && note_time >= 178)
		{
			note_delete(x, note_time - 178);
		}

		if (z_11 == 1 && note_time >= 182)
		{
			note_delete(z, note_time - 182);
		}

		if (z_12 == 1 && note_time >= 191)
		{
			note_delete(z, note_time - 191);
		}



		//

		if (note_time == 21)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 22 && z_1 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 25)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 26 && v_1 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 30)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 31 && z_2 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 34)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 35 && v_2 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 38)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 39 && z_3 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 42)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 43 && v_3 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 46)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 47 && z_4 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_4 = 0;
			}
			judge = 0;
		}

		if (note_time == 50)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 51 && v_4 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_4 = 0;
			}
			judge = 0;
		}

		if (note_time == 55)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 56 && x_1 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 59)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_1 = 0;
			}
			judge = 0;
		}
		if (note_time == 60 && c_1 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_1 = 0;
			}
			judge = 0;
		}

		if (note_time == 63)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 64 && x_2 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 67)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_2 = 0;
			}
			judge = 0;
		}
		if (note_time == 68 && c_2 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_2 = 0;
			}
			judge = 0;
		}

		if (note_time == 71)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 72 && x_3 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 75)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_3 = 0;
			}
			judge = 0;
		}
		if (note_time == 76 && c_3 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_3 = 0;
			}
			judge = 0;
		}

		if (note_time == 79)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 80 && x_4 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_4 = 0;
			}
			judge = 0;
		}

		if (note_time == 83)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_4 = 0;
			}
			judge = 0;
		}
		if (note_time == 84 && c_4 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_4 = 0;
			}
			judge = 0;
		}


		if (note_time == 88)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_5 = 0;
			}
			judge = 0;
		}
		if (note_time == 89 && z_5 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_5 = 0;
			}
			judge = 0;
		}

		if (note_time == 92)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_5 = 0;
			}
			judge = 0;
		}
		if (note_time == 93 && c_5 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_5 = 0;
			}
			judge = 0;
		}

		if (note_time == 96)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_6 = 0;
			}
			judge = 0;
		}
		if (note_time == 97 && z_6 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_6 = 0;
			}
			judge = 0;
		}

		if (note_time == 100)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_6 = 0;
			}
			judge = 0;
		}
		if (note_time == 101 && c_6 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_6 = 0;
			}
			judge = 0;
		}

		if (note_time == 104)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_7 = 0;
			}
			judge = 0;
		}
		if (note_time == 105 && z_7 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_7 = 0;
			}
			judge = 0;
		}

		if (note_time == 108)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_7 = 0;
			}
			judge = 0;
		}
		if (note_time == 109 && c_7 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_7 = 0;
			}
			judge = 0;
		}

		if (note_time == 112)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_8 = 0;
			}
			judge = 0;
		}
		if (note_time == 113 && z_8 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_8 = 0;
			}
			judge = 0;
		}

		if (note_time == 116)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_8 = 0;
			}
			judge = 0;
		}
		if (note_time == 117 && c_8 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_8 = 0;
			}
			judge = 0;
		}

		if (note_time == 121)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_5 = 0;
			}
			judge = 0;
		}
		if (note_time == 122 && x_5 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_5 = 0;
			}
			judge = 0;
		}

		if (note_time == 125)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_5 = 0;
			}
			judge = 0;
		}
		if (note_time == 126 && v_5 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_5 = 0;
			}
			judge = 0;
		}

		if (note_time == 129)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_6 = 0;
			}
			judge = 0;
		}
		if (note_time == 130 && x_6 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_6 = 0;
			}
			judge = 0;
		}

		if (note_time == 133)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_6 = 0;
			}
			judge = 0;
		}
		if (note_time == 134 && v_6 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_6 = 0;
			}
			judge = 0;
		}

		if (note_time == 137)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_7 = 0;
			}
			judge = 0;
		}
		if (note_time == 138 && x_7 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_7 = 0;
			}
			judge = 0;
		}

		if (note_time == 141)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_7 = 0;
			}
			judge = 0;
		}
		if (note_time == 142 && v_7 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_7 = 0;
			}
			judge = 0;
		}

		if (note_time == 145)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_8 = 0;
			}
			judge = 0;
		}
		if (note_time == 146 && x_8 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_8 = 0;
			}
			judge = 0;
		}

		if (note_time == 149)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_8 = 0;
			}
			judge = 0;
		}
		if (note_time == 150 && v_8 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_8 = 0;
			}
			judge = 0;
		}

		if (note_time == 154)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_9 = 0;
			}
			judge = 0;
		}
		if (note_time == 155 && z_9 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_9 = 0;
			}
			judge = 0;
		}

		if (note_time == 158)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_9 = 0;
			}
			judge = 0;
		}
		if (note_time == 159 && x_9 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_9 = 0;
			}
			judge = 0;
		}

		if (note_time == 162)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_9 = 0;
			}
			judge = 0;
		}
		if (note_time == 163 && c_9 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_9 = 0;
			}
			judge = 0;
		}

		if (note_time == 166)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_9 = 0;
			}
			judge = 0;
		}
		if (note_time == 167 && v_9 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_9 = 0;
			}
			judge = 0;
		}

		if (note_time == 170)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_10 = 0;
			}
			judge = 0;
		}
		if (note_time == 171 && c_10 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_10 = 0;
			}
			judge = 0;
		}

		if (note_time == 174)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_10 = 0;
			}
			judge = 0;
		}
		if (note_time == 175 && x_10 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_10 = 0;
			}
			judge = 0;
		}

		if (note_time == 178)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_10 = 0;
			}
			judge = 0;
		}
		if (note_time == 179 && z_10 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_10 = 0;
			}
			judge = 0;
		}

		if (note_time == 182)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_11 = 0;
			}
			judge = 0;
		}
		if (note_time == 183 && x_11 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_11 = 0;
			}
			judge = 0;
		}

		if (note_time == 187)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_11 = 0;
			}
			judge = 0;
		}
		if (note_time == 188 && c_11 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_11 = 0;
			}
			judge = 0;
		}

		if (note_time == 191)
		{
			judge = 1;
			if (press_v_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				v_10 = 0;
			}
			judge = 0;
		}
		if (note_time == 192 && v_10 == 1)
		{
			judge = 2;
			if (press_v_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				v_10 = 0;
			}
			judge = 0;
		}

		if (note_time == 195)
		{
			judge = 1;
			if (press_c_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				c_12 = 0;
			}
			judge = 0;
		}
		if (note_time == 196 && c_12 == 1)
		{
			judge = 2;
			if (press_c_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				c_12 = 0;
			}
			judge = 0;
		}

		if (note_time == 199)
		{
			judge = 1;
			if (press_x_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				x_12 = 0;
			}
			judge = 0;
		}
		if (note_time == 200 && x_12 == 1)
		{
			judge = 2;
			if (press_x_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				x_12 = 0;
			}
			judge = 0;
		}

		if (note_time == 203)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_11 = 0;
			}
			judge = 0;
		}
		if (note_time == 204 && z_11 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_11 = 0;
			}
			judge = 0;
		}

		if (note_time == 212)
		{
			judge = 1;
			if (press_z_key(judge) == 1)
			{
				good++;
				recover_groove_good++;
				z_12 = 0;
			}
			judge = 0;
		}
		if (note_time == 213 && z_12 == 1)
		{
			judge = 2;
			if (press_z_key(judge) == 2)
			{
				perfect++;
				recover_groove_perfect++;
				z_12 = 0;
			}
			judge = 0;
		}







		note_time++;


		if (note_time == 23 + Fix_Note && z_1 == 1)
		{
			z_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 27 + Fix_Note && v_1 == 1)
		{
			v_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 32 + Fix_Note && z_2 == 1)
		{
			z_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 36 + Fix_Note && v_2 == 1)
		{
			v_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 40 + Fix_Note && z_3 == 1)
		{
			z_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 44 + Fix_Note && v_3 == 1)
		{
			v_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 48 + Fix_Note && z_4 == 1)
		{
			z_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 52 + Fix_Note && v_4 == 1)
		{
			v_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 57 + Fix_Note && x_1 == 1)
		{
			x_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 61 + Fix_Note && c_1 == 1)
		{
			c_1 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 65 + Fix_Note && x_2 == 1)
		{
			x_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 69 + Fix_Note && c_2 == 1)
		{
			c_2 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 73 + Fix_Note && x_3 == 1)
		{
			x_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 77 + Fix_Note && c_3 == 1)
		{
			c_3 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 81 + Fix_Note && x_4 == 1)
		{
			x_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 85 + Fix_Note && c_4 == 1)
		{
			c_4 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 90 + Fix_Note && z_5 == 1)
		{
			z_5 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 94 + Fix_Note && c_5 == 1)
		{
			c_5 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 98 + Fix_Note && z_6 == 1)
		{
			z_6 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 102 + Fix_Note && c_6 == 1)
		{
			c_6 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 106 + Fix_Note && z_7 == 1)
		{
			z_7 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 110 + Fix_Note && c_7 == 1)
		{
			c_7 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 114 + Fix_Note && z_8 == 1)
		{
			z_8 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 118 + Fix_Note && c_8 == 1)
		{
			c_8 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 123 + Fix_Note && x_5 == 1)
		{
			x_5 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 127 + Fix_Note && v_5 == 1)
		{
			v_5 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 131 + Fix_Note && x_6 == 1)
		{
			x_6 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 135 + Fix_Note && v_6 == 1)
		{
			v_6 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 139 + Fix_Note && x_7 == 1)
		{
			x_7 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 143 + Fix_Note && v_7 == 1)
		{
			v_7 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 147 + Fix_Note && x_8 == 1)
		{
			x_8 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 151 + Fix_Note && v_8 == 1)
		{
			v_8 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 156 + Fix_Note && z_9 == 1)
		{
			z_9 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 160 + Fix_Note && x_9 == 1)
		{
			x_9 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 164 + Fix_Note && c_9 == 1)
		{
			c_9 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 168 + Fix_Note && v_9 == 1)
		{
			v_9 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 172 + Fix_Note && c_10 == 1)
		{
			c_10 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 176 + Fix_Note && x_10 == 1)
		{
			x_10 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 180 + Fix_Note && z_10 == 1)
		{
			z_10 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 184 + Fix_Note && x_11 == 1)
		{
			x_11 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 189 + Fix_Note && c_11 == 1)
		{
			c_11 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 193 + Fix_Note && v_10 == 1)
		{
			v_10 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 197 + Fix_Note && c_12 == 1)
		{
			c_12 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 201 + Fix_Note && x_12 == 1)
		{
			x_12 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 205 + Fix_Note && z_11 == 1)
		{
			z_11 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (note_time == 214 + Fix_Note && z_12 == 1)
		{
			z_12 = 0;
			miss++;
			total_miss++;
			recover_groove_perfect = 0;
			recover_groove_good = 0;
		}

		if (miss == 15)
		{
			PlaySound(NULL, 0, 0);
			system("cls");
			break;

		}
		if (note_time == 220)
		{
			PlaySound(NULL, 0, 0);
			system("cls");
			break;
		}
		score(good, perfect, total_miss, recover_groove_good, recover_groove_perfect);
	}
	if (miss == 15)
	{
		PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		while (1)
		{
			gotoxy(45, 14); printf("ＧＡＭＥ ＯＶＥＲ");
			Sleep(100);

			if (_kbhit())
			{
				game_over_pass_key = _getch();
				if (game_over_pass_key == Enter)
				{
					PlaySound(NULL, 0, 0);
					break;
				}
			}
		}
	}

	if (note_time == 220 && miss < 15)
	{
		gotoxy(42, 14); printf("결과를 집계중입니다.");
		Sleep(3000);
		delete_line(42, 14);
		gotoxy(45, 10); printf(" R E S U L T");
		gotoxy(46, 12); printf("Perfect   %02d", perfect);
		gotoxy(46, 14); printf("GOOD      %02d", good);
		gotoxy(46, 16); printf("MISS      %02d", total_miss);
		gotoxy(46, 18); printf("Score     %04d", perfect * 300 + good * 100);
		Sleep(4000);
		stage++;
		music = 3;
		music_score = perfect * 300 + good * 100;
		draw_bitmap = true;
	}
}
void Music_1(int music_score, int music)
{
	int p_0, p_1, p_2, p_3, p_4, p_5, p_6, p_7, p_8, p_9, p_10, p_11, p_12, p_13, p_14;
	char note[12][15] = { 0 };
	int i, j;
	int timer = 0;



	memset(note, 48, sizeof(char) * 12 * 15);

	PlaySound(TEXT("Music1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	timer = 0;

	delete_line_for_make_space(0, 9);
	delete_line_for_make_space(0, 10);
	delete_line_for_make_space(0, 25);
	delete_line_for_make_space(0, 26);
	delete_line_for_make_space(0, 28);

	if (draw_bitmap == true)
	{
		bmp_draw();
		//안그려지는 경우가 있을때 한번 더 그림.
		bmp_draw();
		draw_bitmap = false;
	}
	gotoxy(18, 9); printf("UNDERTALE Soundtrack");
	gotoxy(19, 10); printf("Music by Toby Fox");
	gotoxy(25, 25); printf("1번곡");
	gotoxy(22, 26); printf("2.Start Menu");
	gotoxy(21, 28); printf("High Score %05d", save_score1);


	if (music == 1)
	{
		save_score1 = music_score;
		gotoxy(21, 28); printf("High Score %05d", music_score);
	}






	while (1)
	{


		if (_kbhit())
		{

			break;
		}




		p_0 = 0 + rand() % 12;
		p_1 = 0 + rand() % 12;
		p_2 = 0 + rand() % 12;
		p_3 = 0 + rand() % 12;
		p_4 = 0 + rand() % 12;
		p_5 = 0 + rand() % 12;
		p_6 = 0 + rand() % 12;
		p_7 = 0 + rand() % 12;
		p_8 = 0 + rand() % 12;
		p_9 = 0 + rand() % 12;
		p_10 = 0 + rand() % 12;
		p_11 = 0 + rand() % 12;
		p_12 = 0 + rand() % 12;
		p_13 = 0 + rand() % 12;
		p_14 = 0 + rand() % 12;

		pattern(note, p_0, 0);
		pattern(note, p_1, 1);
		pattern(note, p_2, 2);
		pattern(note, p_3, 3);
		pattern(note, p_4, 4);
		pattern(note, p_5, 5);
		pattern(note, p_6, 6);
		pattern(note, p_7, 7);
		pattern(note, p_8, 8);
		pattern(note, p_9, 9);
		pattern(note, p_10, 10);
		pattern(note, p_11, 11);
		pattern(note, p_12, 12);
		pattern(note, p_13, 13);
		pattern(note, p_14, 14);



		for (i = 0; i < 12; i++)
		{

			for (j = 0; j < 15; j++)
			{
				gotoxy(j + 20, i + 12);
				if (note[i][j] == 64)
				{
					printf("%c", note[i][j]);
				}
				else
				{
					printf(" ");
				}
			}
			printf("\n");

		}

		for (i = 0; i < 12; i++)
		{
			for (j = 0; j < 15; j++)
			{
				note[i][j] = 48;
			}

		}




		Sleep(200);


		delete_line_for_make_space(0, 12);
		delete_line_for_make_space(0, 13);
		delete_line_for_make_space(0, 14);
		delete_line_for_make_space(0, 15);
		delete_line_for_make_space(0, 16);
		delete_line_for_make_space(0, 17);
		delete_line_for_make_space(0, 18);
		delete_line_for_make_space(0, 19);
		delete_line_for_make_space(0, 20);
		delete_line_for_make_space(0, 21);
		delete_line_for_make_space(0, 22);
		delete_line_for_make_space(0, 23);
		timer++;


		if (timer == 87)
		{
			timer = 0;
			PlaySound(NULL, 0, 0);
			PlaySound(TEXT("Music1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
		}




	}





}
void Music_2(int music_score, int music)
{
	int p_0, p_1, p_2, p_3, p_4, p_5, p_6, p_7, p_8, p_9, p_10, p_11, p_12, p_13, p_14;
	char note[12][15] = { 0 };
	int i, j;



	memset(note, 48, sizeof(char) * 12 * 15);

	delete_line_for_make_space(0, 9);
	delete_line_for_make_space(0, 10);
	delete_line_for_make_space(0, 25);
	delete_line_for_make_space(0, 26);
	delete_line_for_make_space(0, 28);


	gotoxy(18, 9); printf("UNDERTALE Soundtrack");
	gotoxy(19, 10); printf("Music by Toby Fox");
	gotoxy(25, 25); printf("2번곡");
	gotoxy(21, 26); printf("101.Good Night");
	gotoxy(21, 28); printf("High Score %05d", save_score2);

	if (music == 2)
	{
		save_score2 = music_score;
		gotoxy(21, 28); printf("High Score %05d", music_score);
	}

	PlaySound(TEXT("Music2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	while (1)
	{
		if (_kbhit())
		{

			break;
		}



		p_0 = 0 + rand() % 12;
		p_1 = 0 + rand() % 12;
		p_2 = 0 + rand() % 12;
		p_3 = 0 + rand() % 12;
		p_4 = 0 + rand() % 12;
		p_5 = 0 + rand() % 12;
		p_6 = 0 + rand() % 12;
		p_7 = 0 + rand() % 12;
		p_8 = 0 + rand() % 12;
		p_9 = 0 + rand() % 12;
		p_10 = 0 + rand() % 12;
		p_11 = 0 + rand() % 12;
		p_12 = 0 + rand() % 12;
		p_13 = 0 + rand() % 12;
		p_14 = 0 + rand() % 12;

		pattern(note, p_0, 0);
		pattern(note, p_1, 1);
		pattern(note, p_2, 2);
		pattern(note, p_3, 3);
		pattern(note, p_4, 4);
		pattern(note, p_5, 5);
		pattern(note, p_6, 6);
		pattern(note, p_7, 7);
		pattern(note, p_8, 8);
		pattern(note, p_9, 9);
		pattern(note, p_10, 10);
		pattern(note, p_11, 11);
		pattern(note, p_12, 12);
		pattern(note, p_13, 13);
		pattern(note, p_14, 14);



		for (i = 0; i < 12; i++)
		{

			for (j = 0; j < 15; j++)
			{
				gotoxy(j + 20, i + 12);
				if (note[i][j] == 64)
				{
					printf("%c", note[i][j]);
				}
				else
				{
					printf(" ");
				}
			}
			printf("\n");

		}

		for (i = 0; i < 12; i++)
		{
			for (j = 0; j < 15; j++)
			{
				note[i][j] = 48;
			}

		}




		Sleep(200);


		delete_line_for_make_space(0, 12);
		delete_line_for_make_space(0, 13);
		delete_line_for_make_space(0, 14);
		delete_line_for_make_space(0, 15);
		delete_line_for_make_space(0, 16);
		delete_line_for_make_space(0, 17);
		delete_line_for_make_space(0, 18);
		delete_line_for_make_space(0, 19);
		delete_line_for_make_space(0, 20);
		delete_line_for_make_space(0, 21);
		delete_line_for_make_space(0, 22);
		delete_line_for_make_space(0, 23);






	}



}
void Music_3(int music_score, int music)
{
	int p_0, p_1, p_2, p_3, p_4, p_5, p_6, p_7, p_8, p_9, p_10, p_11, p_12, p_13, p_14;
	char note[12][15] = { 0 };
	int i, j;


	memset(note, 48, sizeof(char) * 12 * 15);

	delete_line_for_make_space(0, 9);
	delete_line_for_make_space(0, 10);
	delete_line_for_make_space(0, 25);
	delete_line_for_make_space(0, 26);
	delete_line_for_make_space(0, 28);

	gotoxy(18, 9); printf("UNDERTALE Soundtrack");
	gotoxy(19, 10); printf("Music by Toby Fox");
	gotoxy(25, 25); printf("3번곡");
	gotoxy(19, 26); printf("3.Your Best Friend");
	gotoxy(21, 28); printf("High Score %05d", save_score3);
	if (music == 3)
	{
		save_score3 = music_score;
		gotoxy(21, 28); printf("High Score %05d", music_score);
	}

	PlaySound(TEXT("Music3.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	while (1)
	{
		if (_kbhit())
		{

			break;
		}



		p_0 = 0 + rand() % 12;
		p_1 = 0 + rand() % 12;
		p_2 = 0 + rand() % 12;
		p_3 = 0 + rand() % 12;
		p_4 = 0 + rand() % 12;
		p_5 = 0 + rand() % 12;
		p_6 = 0 + rand() % 12;
		p_7 = 0 + rand() % 12;
		p_8 = 0 + rand() % 12;
		p_9 = 0 + rand() % 12;
		p_10 = 0 + rand() % 12;
		p_11 = 0 + rand() % 12;
		p_12 = 0 + rand() % 12;
		p_13 = 0 + rand() % 12;
		p_14 = 0 + rand() % 12;

		pattern(note, p_0, 0);
		pattern(note, p_1, 1);
		pattern(note, p_2, 2);
		pattern(note, p_3, 3);
		pattern(note, p_4, 4);
		pattern(note, p_5, 5);
		pattern(note, p_6, 6);
		pattern(note, p_7, 7);
		pattern(note, p_8, 8);
		pattern(note, p_9, 9);
		pattern(note, p_10, 10);
		pattern(note, p_11, 11);
		pattern(note, p_12, 12);
		pattern(note, p_13, 13);
		pattern(note, p_14, 14);



		for (i = 0; i < 12; i++)
		{

			for (j = 0; j < 15; j++)
			{
				gotoxy(j + 20, i + 12);
				if (note[i][j] == 64)
				{
					printf("%c", note[i][j]);
				}
				else
				{
					printf(" ");
				}
			}
			printf("\n");

		}

		for (i = 0; i < 12; i++)
		{
			for (j = 0; j < 15; j++)
			{
				note[i][j] = 48;
			}

		}




		Sleep(200);


		delete_line_for_make_space(0, 12);
		delete_line_for_make_space(0, 13);
		delete_line_for_make_space(0, 14);
		delete_line_for_make_space(0, 15);
		delete_line_for_make_space(0, 16);
		delete_line_for_make_space(0, 17);
		delete_line_for_make_space(0, 18);
		delete_line_for_make_space(0, 19);
		delete_line_for_make_space(0, 20);
		delete_line_for_make_space(0, 21);
		delete_line_for_make_space(0, 22);
		delete_line_for_make_space(0, 23);






	}

}
void Select_Music(int stage)//노래 선택하기
{
	int move = 0;
	int pos = 1;





	if (stage == 1)
	{
		gotoxy(66, 3); printf("<%dst STAGE>", stage);
	}
	else if (stage == 2)
	{
		gotoxy(66, 3); printf("<%dnd STAGE>", stage);
	}
	else if (stage == 3)
	{
		gotoxy(66, 3); printf("<%drd STAGE>", stage);
	}
	else if (stage == 4)
	{
		gotoxy(65, 3); printf("<Bonus STAGE>");
	}
	while (1)
	{






		if (pos == 1)
		{

			Music_1(music_score, music);
		}
		else if (pos == 2)
		{

			Music_2(music_score, music);
		}
		else if (pos == 3)
		{

			Music_3(music_score, music);
		}

		if (_kbhit())
		{
			move = _getch();
			if (move == 13)
			{
				if (pos == 1)
				{
					PlaySound(NULL, 0, 0);
					system("cls");
					Music_1_Note();
				}
				else if (pos == 2)
				{
					PlaySound(NULL, 0, 0);
					system("cls");
					Music_2_Note();
				}
				else if (pos == 3)
				{
					PlaySound(NULL, 0, 0);
					system("cls");
					Music_3_Note();
				}
				system("cls");
				break;
			}

		}

		switch (move)
		{
		case right:
		{


			pos++;
			break;
		}
		case left:
		{


			pos--;

			break;
		}
		}

		if (pos == 4)
		{
			pos = 1;
		}
		if (pos == 0)
		{
			pos = 3;
		}


	}

}
void Select_Music_Decorate() //음악선택화면에서 꾸미기 역할
{


	gotoxy(88, 0); printf("┃");
	gotoxy(88, 1); printf("┃");
	gotoxy(88, 2); printf("┃      ∞");
	gotoxy(88, 3); printf("┃");
	gotoxy(88, 4); printf("┃   ＴＩＭＥ");
	gotoxy(88, 5); printf("┃");
	gotoxy(88, 6); printf("┗━━━━━━━━━━━━━━━━━━━");

	gotoxy(14, 3); printf("S E L E C T  α  M U S I C ");
	gotoxy(66, 1); printf("(p `>ω<´q) ");
	gotoxy(66, 3); printf("<1st STAGE>");
	gotoxy(3, 0);  printf("/");
	gotoxy(2, 1);  printf("/");
	gotoxy(1, 2);  printf("/");
	gotoxy(0, 3);  printf("/");
	gotoxy(0, 4);  printf("＼─");
	gotoxy(0, 5);  printf("＼─");
	gotoxy(4, 5);  printf("＼─");
	gotoxy(4, 6);  printf("＼─"); gotoxy(46, 5); printf("／─");
	gotoxy(8, 6);  printf("＼──────────────────／───────────────／");
	gotoxy(8, 7);  printf("＼─────────────────／───────────────／");
	gotoxy(50, 4); printf("／─");
	gotoxy(50, 5); printf("／─");
	gotoxy(54, 3); printf("／");
	gotoxy(54, 4); printf("／");
	gotoxy(49, 0); printf("＼");
	gotoxy(51, 1); printf("＼ ^(*)^");
	gotoxy(53, 2); printf("＼");
	gotoxy(61, 0); printf("_<");
	gotoxy(59, 1); printf("_<");
	gotoxy(57, 2); printf("_<");
	gotoxy(56, 3); printf("<");
	gotoxy(57, 4); printf("￣<");
	gotoxy(60, 5); printf("￣<");
	gotoxy(63, 6); printf("￣￣￣￣￣￣￣￣￣");
	gotoxy(87, 3); printf(">");
	gotoxy(84, 4); printf(">￣");
	gotoxy(81, 5); printf(">￣");
	gotoxy(81, 0); printf(">_<");
	gotoxy(83, 1); printf(">_<");
	gotoxy(85, 2); printf(">_<");

	gotoxy(10, 11); printf("┏");
	gotoxy(44, 11); printf("┓");
	gotoxy(10, 24); printf("┗");
	gotoxy(44, 24); printf("┛");

	Select_Music(stage);


}
void next_stage_to_2(int stage)
{
	if (stage == 2)
	{
		Select_Music_Decorate();
	}

}
void next_stage_to_3(int stage)
{
	if (stage == 3)
	{
		Select_Music_Decorate();
	}
}
void bonus_stage(int stage)
{
	if (stage == 4)
	{
		Select_Music_Decorate();
	}
}
void Ending_Credit()
{
	gotoxy(40, 14);  printf("플레이 해주셔서 감사합니다.");
	system("pause>null");
}
int main()
{

	system("title EASY TO DJ");
	delete_Cursor();
	title_1();
	title_2();
	title_3();
	title_to_tutorial();
	tutorial_1();
	tutorial_to_select_music();



	Select_Music_Decorate();

	next_stage_to_2(stage);
	next_stage_to_3(stage);
	bonus_stage(stage);

	Ending_Credit();








}