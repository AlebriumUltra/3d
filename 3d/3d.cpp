﻿// 3d.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "3d.h"
#include <Windows.h>
#include <complex>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




typedef struct param {
	double scr_x = 266;
	double scr_y = 200;

	double Cre_start = -2;
	double Cre_finish = 0.5;
	double Cim_start = -1.25;
	double Cim_finish = 1.25;

	const int iterations = 80;
};
param parameters;

const double zoom = 0.8;
const double move = 0.1;

const float c_cre = (abs(parameters.Cre_start) + abs(parameters.Cre_finish)) * move;
const float c_cim = (abs(parameters.Cim_start) + abs(parameters.Cim_finish)) * move;





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

COLORREF set_color(int i) {
	COLORREF color;
	int r, g, b;

	if (i < 20) {
		color = RGB(255, 132, 132);
	}
	else if (i < 40) {
		color = RGB(243, 138, 249);
	}
	else if (i < 60) {
		color = RGB(230, 225, 47);
	}
	else {
		color = RGB(79, 217, 89);
	}
	return color;
}

void gray_gradient(HDC hdc) {
	COLORREF pix_color;
	float red, green, blue;
	for (int x = 0; x < parameters.scr_x; x++) {
		for (int y = 0; y < parameters.scr_y; y++) {
			pix_color = GetPixel(hdc, x, y);
			red = (GetRValue(pix_color) + GetGValue(pix_color) + GetBValue(pix_color)) / 3;
			blue = green = red;
			SetPixel(hdc, x, y, RGB(red, green, blue));
		}
	}
}

void monochrome(HDC hdc, float threshold) {
	COLORREF pix_color;
	float red;
	for (int x = 0; x < parameters.scr_x; x++) {
		for (int y = 0; y < parameters.scr_y; y++) {
			pix_color = GetPixel(hdc, x, y);
			red = GetRValue(pix_color);
			if (red <= threshold) {
				SetPixel(hdc, x, y, color_black);
			}
			else {
				SetPixel(hdc, x, y, color_white);
			}
		}
	}
}

void multi_brightness(HDC hdc, const float k) {
	COLORREF pix_color;
	float red, green, blue;
	for (int x = 0; x < parameters.scr_x; x++) {
		for (int y = 0; y < parameters.scr_y; y++) {
			pix_color = GetPixel(hdc, x, y);
			red = GetRValue(pix_color);
			green = GetGValue(pix_color);
			blue = GetBValue(pix_color);
			red = (int)(red * k) % 255;
			green = (int)(green * k) % 255;
			blue = (int)(blue * k) % 255;
			SetPixel(hdc, x, y, RGB(red, green, blue));
		}
	}
}


void sum_brightness(HDC hdc, const float c) {
	COLORREF pix_color;
	float red, green, blue;
	for (int x = 0; x < parameters.scr_x; x++) {
		for (int y = 0; y < parameters.scr_y; y++) {
			pix_color = GetPixel(hdc, x, y);
			red = GetRValue(pix_color);
			green = GetGValue(pix_color);
			blue = GetBValue(pix_color);
			red = (int)(red + c) % 255;
			green = (int)(green + c) % 255;
			blue = (int)(blue + c) % 255;
			SetPixel(hdc, x, y, RGB(red, green, blue));
		}
	}
}


//
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
		case VK_F1:
			parameters.Cim_start *= zoom;
			parameters.Cim_finish *= zoom;
			parameters.Cre_start *= zoom;
			parameters.Cre_finish *= zoom;
			break;
		case VK_F2:
			parameters.Cim_start /= zoom;
			parameters.Cim_finish /= zoom;
			parameters.Cre_start /= zoom;
			parameters.Cre_finish /= zoom;
			break;
		case VK_RIGHT:
			parameters.Cre_start = parameters.Cre_start - (parameters.Cre_start * move);
			parameters.Cre_finish = parameters.Cre_finish - (parameters.Cre_finish * move);
			break;
		case VK_LEFT:
			parameters.Cre_start += parameters.Cre_start * move;
			parameters.Cre_finish += parameters.Cre_finish * move;
			break;
		case VK_UP:
			parameters.Cim_start -= parameters.Cim_start * move;
			parameters.Cim_finish -= parameters.Cim_finish * move;
			break;
		case VK_DOWN:
			parameters.Cim_start += parameters.Cim_start * move;
			parameters.Cim_finish += parameters.Cim_finish * move;
			break;
		case VK_SPACE:
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

		float step_x = (parameters.Cre_finish - parameters.Cre_start) / parameters.scr_x;
		float step_y = (parameters.Cim_finish - parameters.Cim_start) / parameters.scr_y;
		float bias_x;
		float bias_y;
		float red, green, blue;
		COLORREF color;
		for (int x = 0; x < parameters.scr_x; x++) {
			for (int y = 0; y < parameters.scr_y; y++) {
				bias_x = parameters.Cre_start + step_x * x;
				bias_y = parameters.Cim_start + step_y * y;
				std::complex<float>c(bias_x, bias_y);
				std::complex<float>z(0, 0);
				red = 0;
				green = 0;
				blue = 0;
				for (int i = 0; i < parameters.iterations; i++) {
					z = (z * z) + c;
					if (abs(z) > 2) {
						color = set_color(i);
						SetPixel(hdc, x, y, color);
						break;
					}
				}
				if (abs(z) <= 2) {
					SetPixel(hdc, x, y, color_black);
				}
			}
		}

		COLORREF pix_color;
		multi_brightness(hdc, 5);
		sum_brightness(hdc, 100);
		gray_gradient(hdc);
		monochrome(hdc, 200);
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
