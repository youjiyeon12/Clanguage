// work1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "work1.h"
#include <time.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WORK1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WORK1));

    MSG msg;

    // 기본 메시지 루프입니다:
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
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WORK1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WORK1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// 0 : 직선 그리기, 1 : 자유선 그리기, 2 : 사각형 그리기, 3 : 타원 그리기
int g_drawType = 0; // 기본값은 0

// 4 : 흰색, 5 : 빨강색, 6 : 노랑색, 7 : 파랑색
int g_colorType = 4; // 기본값은 4

//이전 좌표 저장
int g_x, g_y;

//끝 좌표 (사각형 그리기 할 때)
int g_xx, g_yy;

//마우스 클릭상태 확인(처음 상태는 false)
bool m_click = false;

HWND child;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        child = FindWindow(NULL, L"child");
        srand(time(NULL));s
        CreateWindow(L"button", L"지우기", WS_VISIBLE | WS_CHILD, 10, 10, 200, 100, hWnd, (HMENU)3, hInst, NULL);
    }
    break;

    //자유선 그리기
    case WM_LBUTTONDOWN:
    {
        g_x = LOWORD(lParam);
        g_y = HIWORD(lParam);

        m_click = true;

        PostMessage(child, WM_LBUTTONDOWN, wParam, lParam);
    }
    break;

    case WM_MOUSEMOVE:
    {
        if (m_click == true && g_drawType == 1) {
            HDC hdc = GetDC(hWnd);

            //이전 좌표
            MoveToEx(hdc, g_x, g_y, NULL);


            //이전 좌표 변수에 현재 좌표 적재
            g_y = HIWORD(lParam);
            g_x = LOWORD(lParam);

            //이전 좌표에 선 연결
            LineTo(hdc, g_x, g_y);

            //해제
            ReleaseDC(hWnd, hdc);

            PostMessage(child, WM_MOUSEMOVE, wParam, lParam);
        }
    }
    break;


    // 마우스 UP 되면 그림을 그리고 나서 m_click을 false로 바꿈
    case WM_LBUTTONUP:
    {
        
        if (m_click) {
            HDC hdc = GetDC(hWnd);

            // 마우스 UP 했을 때 끝 좌표 저장
            g_xx = LOWORD(lParam);
            g_yy = HIWORD(lParam);

            // 선택된 색상에 따라 브러시 생성
            HBRUSH myBrush = NULL; // 브러시 변수를 이곳에서 정의
            HBRUSH osBrush = NULL;

            // 나의 브러쉬 색상을 바꿈
            if (g_colorType == 4) {
                myBrush = CreateSolidBrush(RGB(255, 255, 255)); // 흰색
            }
            else if (g_colorType == 5) {
                myBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨강
            }
            else if (g_colorType == 6) {
                myBrush = CreateSolidBrush(RGB(255, 255, 0)); // 노랑
            }
            else if (g_colorType == 7) {
                myBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파랑
            }

            // 직선 그리기
            if (g_drawType == 0) 
            {
                HDC hdc = GetDC(hWnd);

                MoveToEx(hdc, g_x, g_y, NULL);

                g_x = LOWORD(lParam);
                g_y = HIWORD(lParam);

                LineTo(hdc, g_x, g_y);
            }

            // 자유선 그리기
            if (g_drawType == 1) 
            {
                m_click = false;
            }

            // 사각형 그리기
            if (g_drawType == 2) { 
                osBrush = (HBRUSH)SelectObject(hdc, myBrush);
                Rectangle(hdc, g_x, g_y, g_xx, g_yy);
            }

            // 타원 그리기
            else if (g_drawType == 3) { 
                osBrush = (HBRUSH)SelectObject(hdc, myBrush);
                Ellipse(hdc, g_x, g_y, g_xx, g_yy);
            }

            if (myBrush != NULL && osBrush != NULL) {
                SelectObject(hdc, osBrush);
                DeleteObject(myBrush);
            }

            ReleaseDC(hWnd, hdc);
        }
        m_click = false; // 마우스 클릭 상태 해제

        PostMessage(child, WM_LBUTTONUP, wParam, lParam);
    }
    break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_A1: // 직선
                g_drawType = 0; 
                SendMessage(child, WM_USER, wParam, ID_A1);
                break;
            case ID_A2: // 자유선
                g_drawType = 1; 
                SendMessage(child, WM_USER, wParam, ID_A2);
                break;
            case ID_A3: // 사각형
                g_drawType = 2; 
                SendMessage(child, WM_USER, wParam, ID_A3);
                break;
            case ID_A4: // 타원형
                g_drawType = 3; 
                SendMessage(child, WM_USER, wParam, ID_A4);
                break;
            case ID_B1: // 흰색
                g_colorType = 4; 
                SendMessage(child, WM_USER, wParam, ID_B1);
                break;
            case ID_B2: // 빨강색
                g_colorType = 5; 
                SendMessage(child, WM_USER, wParam, ID_B2);
                break;
            case ID_B3: // 노랑색
                g_colorType = 6; 
                SendMessage(child, WM_USER, wParam, ID_B3);
                break;
            case ID_B4: // 파랑색
                g_colorType = 7; 
                SendMessage(child, WM_USER, wParam, ID_B4);
                break;

            case 3: // 버튼 클릭 시 화면 지움
                InvalidateRect(hWnd, NULL, TRUE);
                SendMessage(child, WM_USER, wParam, 3);
                break;

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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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

// 정보 대화 상자의 메시지 처리기입니다.
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
