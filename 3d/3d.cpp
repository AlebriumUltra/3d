// 3d.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "3d.h"

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
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY3D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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


const float cord[][4] = { {220, 120, 1, 1},
			{ 20, 120, 1, 0},
			{ 60, 200, 1, 0},
			{180, 200, 1, 0},
			{220, 120, 1, 0},
			{180, 120, 1, 1},
			{140, 30, 1, 0},
			{100, 120, 1, 0},
			{100, 20, 1, 0},
			{60, 20, 1, 0},
			{80, 30, 1, 0},
			{60, 40, 1, 0},
			{100, 40, 1, 0} };

float current_cord[][4] = { {220, 120, 1, 1},
			{ 20, 120, 1, 0},
			{ 60, 200, 1, 0},
			{180, 200, 1, 0},
			{220, 120, 1, 0},
			{180, 120, 1, 1},
			{140, 30, 1, 0},
			{100, 120, 1, 0},
			{100, 20, 1, 0},
			{60, 20, 1, 0},
			{80, 30, 1, 0},
			{60, 40, 1, 0},
			{100, 40, 1, 0} };


void return_cord(float array[][4]) {
	int row = 13;
	int col = 4;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			array[i][j] = cord[i][j];
		}
	}
}

void multi_matrix(float array[][4], float affin[3][3]) {
	int sum = 0;
	int row = 13;
	int col = 4;

	float buf[3];
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < 3; j++) {
			for (int c = 0; c < 3; c++) {
				sum += array[i][c] * affin[c][j];
			}
			buf[j] = sum;
			sum = 0;
		}
		for (int h = 0; h < 3; h++) {
			array[i][h] = buf[h];
		}
	}
}

void paint(HDC hdc, float cord[][4]) {
	int row = 13;
	for (int i = 0; i < row; i++) {
		if (cord[i][3] == 1)
			MoveToEx(hdc, cord[i][0], cord[i][1], NULL);
		else
			LineTo(hdc, cord[i][0], cord[i][1]);
	}
}

void move(float array[][4], float m, float n) {
	float move[3][3] = {
		{1, 0, 0},
		{0, 1, 0},
		{m, n, 1} 
	};

	multi_matrix(array, move);
}

void scale(float array[][4], float a, float b) {
	float scale[3][3] = { 
		{a, 0, 0},
		{0, b, 0},
		{0, 0, 1} 
	};

	multi_matrix(array, scale);
}

void reflect(float array[][4]) {
	float reflect[3][3] = { 
		{0, 1, 0},
		{1, 0, 0},
		{0, 0, 1} 
	};

	multi_matrix(array, reflect);
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
		case VK_UP:
			move(current_cord, 1.5, -3);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RIGHT:
			scale(current_cord, 3, 3);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_LEFT:
			reflect(current_cord);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_DOWN:
			return_cord(current_cord);
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
		paint(hdc, current_cord);
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
