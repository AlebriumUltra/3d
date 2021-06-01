// 3d.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "3d.h"
#include <Windows.h>
#include <complex>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>

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


std::vector<double>vertexes;
std::vector<std::vector<double>>faces;

std::vector<double>copy_vertexes;
std::vector<std::vector<double>>copy_faces;

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

float d = 1500;
float angle_a = 30;
float angle_b = 30;
float R = -200;

float copy_d = d;
float copy_a = angle_a;
float copy_b = angle_b;
float copy_R = R;

float move_x = 500;
float move_y = 300;

float copy_x = move_x;
float copy_y = move_y;

bool model_is_exist = false;

void paint(HDC hdc) {
	size_t size_v = vertexes.size();
	size_t size_f = faces.size();
	size_t size_current_f;
	int count = 0;
	for (int i = 0; i < size_f; i++) {
		size_current_f = faces[i].size();
		MoveToEx(hdc, vertexes[(faces[i][0] - 1) * 3], vertexes[(faces[i][0] - 1) * 3 + 1], NULL);
		for (int j = 0; j < size_current_f; j++) {
			LineTo(hdc, vertexes[(faces[i][j] - 1) * 3], vertexes[(faces[i][j] - 1) * 3 + 1]);
		}
	}
}

void transform_views() {
	double fi = angle_a * PI / 180;
	double teta = angle_b * PI / 180;
	size_t size = vertexes.size();
	for (int i = 0; i < size; i+=3) {
		double Xe = -sin(fi) * vertexes[i+0] + cos(fi) * vertexes[i+1];
		double Ye = -cos(fi) * cos(teta) * vertexes[i+0] + -cos(teta) * sin(fi) * vertexes[i+1] + sin(teta) * vertexes[i+2];
		double Ze = -cos(teta) * cos(fi) * vertexes[i+0] + -sin(fi) * sin(teta) * vertexes[i+1] + -cos(teta) * vertexes[i+2] + R;
		vertexes[i+0] = Xe;
		vertexes[i+1] = Ye;
		vertexes[i+2] = Ze;
	}
}


void perspective() {
	size_t size = vertexes.size();
	for (int i = 0; i < size; i+=3) {
		vertexes[i+0] = d * vertexes[i + 0] / vertexes[i + 2];
		vertexes[i + 1] = d * vertexes[i + 1] / vertexes[i + 2];
	}
}

void screen_transform() {
	size_t size = vertexes.size();
	int x = move_x;
	int y = move_y;
	for (int i = 0; i < size; i+=3) {
		vertexes[i + 0] += x;
		vertexes[i + 1] += y;
	}
}


void read_file() {
	std::ifstream fin;
	fin.open("hamer.obj");
	std::string line;
	char delim = ' ';
	std::string token;
	int count_v = 0;
	int face_count = 0;
	vertexes.clear();
	faces.clear();
	while (!fin.eof()) {
		getline(fin, line);
		if (line[0] == 'v') {
			count_v++;
			line.erase(0, 2);
			size_t pos;
			while ((pos = line.find(delim)) != std::string::npos) {
				token = line.substr(0, pos);
				line.erase(0, pos + 1);
				vertexes.push_back(strtod(token.c_str(), NULL));
			}
			vertexes.push_back(strtod(line.c_str(), NULL));
		}
		if (line[0] == 'f') {
			line.erase(0, 2);
			size_t pos;
			faces.push_back(std::vector<double>());
			while ((pos = line.find(delim)) != std::string::npos) {
				token = line.substr(0, pos);
				line.erase(0, pos + 1);
				faces[face_count].push_back(strtod(token.c_str(), NULL));
			}
			faces[face_count].push_back(strtod(line.c_str(), NULL));
			face_count++;
		}
	}
	model_is_exist = true;
	copy_vertexes = vertexes;
	copy_faces = faces;
}

void reset_model() {
	if (!model_is_exist) {
		read_file();
	}
	else {
		vertexes = copy_vertexes;
		faces = copy_faces;
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
			angle_a = copy_a;
			angle_b = copy_b;
			R = copy_R;
			d = copy_d;
			move_x = copy_x;
			move_y = move_y;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'F':
			angle_a = angle_a - 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'D':
			angle_a = angle_a + 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'C':
			angle_b = angle_b - 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'V':
			angle_b = angle_b + 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'A':
			R = R + 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'S':
			R = R - 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'Z':
			d = d - 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 'X':
			d = d + 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_UP:
			move_y = move_y - 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_DOWN:
			move_y = move_y + 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_LEFT:
			move_x = move_x - 10;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RIGHT:
			move_x = move_x + 10;
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
		reset_model();
		transform_views();
 		perspective();
		screen_transform();
		paint(hdc);
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
