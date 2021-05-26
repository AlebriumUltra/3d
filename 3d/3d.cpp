// 3d.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "3d.h"
#include <Windows.h>
#include <complex>
#include <cmath>

#define MAX_LOADSTRING 100
#define PI 3.14159265358979323846

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

COLORREF color_black = RGB(0, 0, 0);
COLORREF color_white = RGB(255, 255, 255);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY3D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3D));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


float cube_cord[17][4] = 
{ {100, 100, 0, 0},
	{100, 300, 0, 1},
	{300, 300, 0, 1},
	{300, 100, 0, 1},
	{100, 100, 0, 1},
	{100, 100, 200, 0},
	{100, 300, 200, 1},
	{300, 300, 200, 1},
	{300, 100, 200, 1},
	{100, 100, 200, 1},
	{100, 100, 0, 1},
	{100, 300, 200, 0},
	{100, 300, 0, 1},
	{300, 300, 200, 0},
	{300, 300, 0, 1},
	{300, 100, 200, 0},
	{300, 100, 0, 1},
};


float test_cord[17][4] = 
{ {100, 100, 0, 0},
	{100, 300, 0, 1},
	{300, 300, 0, 1},
	{300, 100, 0, 1},
	{100, 100, 0, 1},
	{100, 100, 200, 0},
	{100, 300, 200, 1},
	{300, 300, 200, 1},
	{300, 100, 200, 1},
	{100, 100, 200, 1},
	{100, 100, 0, 1},
	{100, 300, 200, 0},
	{100, 300, 0, 1},
	{300, 300, 200, 0},
	{300, 300, 0, 1},
	{300, 100, 200, 0},
	{300, 100, 0, 1},
};


float current_cord[17][4] =
{ {100, 100, 0, 0},
	{100, 300, 0, 1},
	{300, 300, 0, 1},
	{300, 100, 0, 1},
	{100, 100, 0, 1},
	{100, 100, 200, 0},
	{100, 300, 200, 1},
	{300, 300, 200, 1},
	{300, 100, 200, 1},
	{100, 100, 200, 1},
	{100, 100, 0, 1},
	{100, 300, 200, 0},
	{100, 300, 0, 1},
	{300, 300, 200, 0},
	{300, 300, 0, 1},
	{300, 100, 200, 0},
	{300, 100, 0, 1},
};

//
//{ {100, 100, 0, 1, 0},
//{ 100, 300, 0, 1, 1 },
//{ 300, 300, 0, 1, 1 },
//{ 300, 100, 0, 1, 1 },
//{ 100, 100, 0, 1, 1 },
//{ 100, 100, 200, 1, 0 },
//{ 100, 300, 200, 1, 1 },
//{ 300, 300, 200, 1, 1 },
//{ 300, 100, 200, 1, 1 },
//{ 100, 100, 200, 1, 1 },
//{ 100, 100, 0, 1, 1 },
//{ 100, 300, 200, 1, 0 },
//{ 100, 300, 0, 1, 1 },
//{ 300, 300, 200, 1, 0 },
//{ 300, 300, 0, 1, 1 },
//{ 300, 100, 200, 1, 0 },
//{ 300, 100, 0, 1, 1 },
//};

//{ {100, 100, 0, 0},
//{ 100, 300, 0, 1 },
//{ 300, 300, 0, 1 },
//{ 300, 100, 0, 1 },
//{ 100, 100, 0, 1 },
//{ 100, 100, 200, 0 },
//{ 100, 300, 200, 1 },
//{ 300, 300, 200, 1 },
//{ 300, 100, 200, 1 },
//{ 100, 100, 200, 1 },
//{ 100, 100, 0, 1 },
//{ 100, 300, 200, 0 },
//{ 100, 300, 0, 1 },
//{ 300, 300, 200, 0 },
//{ 300, 300, 0, 1 },
//{ 300, 100, 200, 0 },
//{ 300, 100, 0, 1, 1 },
//};

float d = 1;
float angle_a = 30;
float angle_b = 90;
float R = 1000;

float copy_d = d;
float copy_a = angle_a;
float copy_b = angle_b;
float copy_R = R;



void reset_cord() {
	int size_h = 17;
	int size_w = 4;
	for (int i = 0; i < size_h; i++) {
		for (int j = 0; j < size_w; j++) {
			current_cord[i][j] = cube_cord[i][j];
		}
	}
}

void paint_cube(HDC hdc) {
	int size_h = 17;
	for (int i = 0; i < size_h; i++) {
		if (current_cord[i][3] == 0) {
			MoveToEx(hdc, current_cord[i][0], current_cord[i][1], NULL);
		}
		else {
			LineTo(hdc, current_cord[i][0], current_cord[i][1]);
		}
	}
}


void O_dot() {
	int size_h = 17;
	for (int i = 0; i < size_h; i++) {
		current_cord[i][0] -= 100;
		current_cord[i][1] -= 100;
		current_cord[i][2] -= 100; 
	}
}


void transform_views() {
	float fi = angle_a * PI / 180;
	float teta = angle_b * PI / 180;
	int size_h = 17;
	for (int i = 0; i < size_h; i++) {
		float Xe = -sin(teta) * current_cord[i][0] + cos(fi) * current_cord[i][1];
		float Ye = -cos(fi) * cos(teta) * current_cord[i][0] + -cos(fi) * sin(teta) * current_cord[i][1] + sin(fi) * current_cord[i][2];
		float Ze = -sin(fi) * cos(teta) * current_cord[i][0] + -sin(fi) * sin(teta) * current_cord[i][1] + -cos(fi) * current_cord[i][2] + R;
		current_cord[i][0] = Xe;
		current_cord[i][1] = Ye;
		current_cord[i][2] = Ze;
	}
}


void perspective() {
	for (int i = 0; i < 17; i++) {
		current_cord[i][0] = (d * current_cord[i][0]);
		current_cord[i][1] = (d * current_cord[i][1]);
	}
}

void screen_transform() {
	for (int i = 0; i < 17; i++) {
		current_cord[i][0] += 500;
		current_cord[i][1] += 500;
	}
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3D));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY3D);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}




//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC memdc;
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case VK_SPACE:
			reset_cord();
			angle_a = copy_a;
			angle_b = copy_b;
			R = copy_R;
			d = copy_d;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_LEFT:
			angle_a = angle_a - 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RIGHT:
			angle_a = angle_a + 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'C':
			angle_b = angle_b - 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'V':
			angle_b = angle_b + 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_UP:
			R = R + 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_DOWN:
			R = R - 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'Z':
			d = d - 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'X':
			d = d + 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		reset_cord();
		O_dot();
		transform_views();
		perspective();
		screen_transform();
		paint_cube(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
