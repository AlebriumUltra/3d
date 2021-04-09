// 3d.cpp : Определяет точку входа для приложения.
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
	double scr_x = 800;
	double scr_y = 600;

	double Cre_start = -2;
	double Cre_finish = 0.5;
	double Cim_start = -1.25;
	double Cim_finish = 1.25;

	const int iterations = 80;
};

const double zoom = 0.5;
const double move = 0.3;
param parameters;




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

void step_color(double &red, double &green, double& blue) {
	red = red + 5 < 255 ? red + 5 : 255;
	if (red == 255) {
		green = green + 3.5 < 255 ? green + 3.5 : 255;
	}
	if (green == 255) {
		blue = blue + 2.2 < 255 ? blue + 2.2 : 255;
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
		
		double step_x = (parameters.Cre_finish - parameters.Cre_start) / parameters.scr_x;
		double step_y = (parameters.Cim_finish - parameters.Cim_start) / parameters.scr_y;
		double bias_x;
		double bias_y;
		double red, green, blue;
		COLORREF color;
		for (int x = 0; x < parameters.scr_x; x++) {
			for (int y = 0; y < parameters.scr_y; y++) {
				bias_x = parameters.Cre_start + step_x * x;
				bias_y = parameters.Cim_start + step_y * y;
				std::complex<double>c(bias_x, bias_y);
				std::complex<double>z(0, 0);
				red = 0;
				green = 0;
				blue = 0;
				for (int i = 0; i < parameters.iterations; i++) {
					z = (z * z) + c;
					if (abs(z) > 2) {
						color = RGB(red,green,blue);
						SetPixel(hdc, x, y, color);
						break;
					}
					step_color(red, green, blue);
				}
				if (abs(z) <= 2) {
					SetPixel(hdc, x, y, color_black);
				}
			}
		}

		/*COLORREF pix_color;
		const double k = 3;
		const double c = 10;




		for (int x = 0; x < parameters.scr_x; x++) {
			for (int y = 0; y < parameters.scr_y; y++) {
				pix_color = GetPixel(hdc, x, y);
				red = GetRValue(pix_color);
				green = GetGValue(pix_color);
				blue = GetBValue(pix_color);
				pix_color = (int)(pix_color * k) % 255;
				red = (int)(red * k) % 255;
				green = (int)(green * k) % 255;
				blue = (int)(blue * k) % 255;
				SetPixel(hdc, x, y, RGB(red, green, blue));
			}
		}


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


		
		
		int threshold = 0;
		for (int x = 0; x < parameters.scr_x; x++) {
			for (int y = 0; y < parameters.scr_y; y++) {
				pix_color = GetPixel(hdc, x, y);
				if (pix_color / 3 <= threshold) {
					SetPixel(hdc, x, y, color_black);
				}
				else {
					SetPixel(hdc, x, y, color_white);
				}
			}
		}
*/


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
