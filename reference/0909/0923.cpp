// 0909.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "0909.h"
#include <stdlib.h>
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
    LoadStringW(hInstance, IDC_MY0909, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY0909));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY0909));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY0909);
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
// 나와 상대의 변수 및 초기 좌표 설정
//->create 부분에 g_me.right = 10; 이렇게 선언 해도 됨.
RECT g_me, g_you, g_item;
RECT g_bar;
//최대 0~499 위치를 갈 수 있음. 50 크기의 아이템이 만들어 짐.
/// 상대의 속도 제어를 위한 전역 변수
int g_timer = 1000;
/// 아이템이 보여지는 여부
int g_view = 1;
/// 게임 끝날 때 자신 API도 멈춤
bool g_over = false;
/// 게임시간
int g_gametime = 30;
/// 게임점수
int g_score;
/// 타이머 속도를 줄이기 위한 전역변수
int s_timer = 1000;

/*
g_me.left = 10;
g_me.top = 10;
g_me.right = 100;
g_me.bottom = 100;
*/


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    // 클래스의 생성자와 완전히 동일한 형태로 동작
    // 프로그램이 실행된 이후에 단 한번만 OS레 의해 호출된다.
    // 프로그램의 초기값이나 한번만 실행하는 함수들을 작성하면됨.

    // 나의 키 입력을 통해 나의 좌표 값을 변경하는 코드
    
    RECT a = { 0,0,0,0 }; ///겹침이 발생한 좌표값.(가져옴)
    //g_box는 전체 창의 크기로 초기화 된 상태이다 (GetClientRect로 가져옴)
    RECT g_box;
    GetClientRect(hWnd, &g_box);
    switch (message)
    {
    case WM_CREATE:
    {
        // 랜덤 시드 결정. 단 한번만!
        srand(time(NULL));
        //상대 객체의 이동을 위한 타이머의 설치
        //os가 지정한 시간이 되면, WM_TIMER를 호출해준다.
        SetTimer(hWnd, 1, g_timer, NULL);
        //상대가 1초마다 속도를 변경하기 위해 필요한 타이머
        SetTimer(hWnd, 2, 1000, NULL);
        //게임 시간 제어를 위한 타이머
        SetTimer(hWnd, 3, 1000, NULL);
        //게임 점수를 위한 타이머;
        SetTimer(hWnd, 4, s_timer, NULL);
        //게임 점수 -> 1초마다 속도가 빨라지게 만든다
        SetTimer(hWnd, 5, 1000, NULL);

        /// 내 객체의 초기 정보
        g_me.left = 50;
        g_me.top = 50;
        g_me.right = 140;
        g_me.bottom = 140;

        /// 상대 객체의 초기 정보
        g_you.left = 330;
        g_you.top = 330;
        g_you.right = 420;
        g_you.bottom = 420;

        /// 아이템 객체의 초기 정보
        g_item.left = rand() % (g_box.right - 80);
        g_item.top = rand() % (g_box.bottom - 80);
        g_item.right = g_item.left + 50;
        g_item.bottom = g_item.top + 50;

        /// 시간 바의 초기 값
        g_bar.left = g_box.right - 30;
        g_bar.top = g_box.top + 10;
        g_bar.right = g_box.right - 10;
        g_bar.bottom = g_box.bottom - 10;
    }
    break;

    case WM_SIZE:
    {        
        GetClientRect(hWnd, &g_box);

        /// 시간 바의 초기 값
        g_bar.left = g_box.right - 30;
        g_bar.top = g_box.top + 10;
        g_bar.right = g_box.right - 10;
        g_bar.bottom = g_box.bottom - 10;
    }
    break;

    case WM_TIMER:
    {
        ///wParam은 SetTimer에서 지정한 ID값을 가지고 호출된다.
        if (1 == wParam)
        {
        /// 상대가 나의 좌표를 보고 상대의 X 좌표를 변경하는 코드 ☆
        if (g_me.left > g_you.left)    // 나는 상대의 오른쪽에 있다.
        {
            g_you.left += 10;
            g_you.right += 10;
        }
        else // 나는 상대의 왼쪽에 있다.
        {
            g_you.left -= 10;
            g_you.right -= 10;
        }

        /// 상대가 나의 좌표를 보고 상대의 Y 좌표를 변경하는 코드 ☆
        if (g_me.top > g_you.top)    //나는 상대의 아래에 있다.
        {
            g_you.top += 10;
            g_you.bottom += 10;
        }
        else    //나는 상대의 위에 있다.
        {
            g_you.top -= 10;
            g_you.bottom -= 10;
        }
        ///상대가 나를 잡았는지 확인하는 단계
        /// 좌표 겹칩 확인
        if (IntersectRect(&a, &g_me, &g_you))
        {
            /// 타이머를 멈추는 API
            KillTimer(hWnd, 1);            
            KillTimer(hWnd, 2);
            KillTimer(hWnd, 3);
            KillTimer(hWnd, 4);
            KillTimer(hWnd, 5);
            MessageBox(hWnd, L"Game Over", L"게임 오버", MB_OK);
            g_over = true;
        }

        }
        else if (2 == wParam)
        {
            ///상대의 속도를 조절
            g_timer -= 100;
            if (g_timer <= 100)
                g_timer = 100;
            KillTimer(hWnd, 1);
            SetTimer(hWnd, 1, g_timer, NULL);
        }
        else if (3 == wParam)
        {
            g_gametime -= 1;

            int barHeight = g_box.top - g_box.bottom;
            g_bar.top = g_box.bottom + (barHeight * g_gametime) / 30;

            if (0 == g_gametime)
            {
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);
                KillTimer(hWnd, 3);
                KillTimer(hWnd, 4);
                KillTimer(hWnd, 5);
                MessageBox(hWnd, L"Time Over", L"타임 오버", MB_OK);
                g_over = true;
            }

            if (g_bar.top > g_bar.bottom)
            {
                g_bar.bottom = g_bar.top;
            }
        }
        else if (4 == wParam) 
        {
            g_score += 100;

            if (s_timer < 100) {
                s_timer = 100;
            }   
        }
        else if (5 == wParam)
        {
            s_timer -= 50;

            if (s_timer < 200) {
                s_timer = 200;
            }
            KillTimer(hWnd, 4);
            SetTimer(hWnd, 4, s_timer, NULL);
        }

        //상대의 좌표 값을 조정한 상태 ==> 화면 무효화 요청
        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;
    case WM_KEYDOWN:
    {
        if (g_over == false)
        {
            switch (wParam)
            {
            case VK_UP:
                g_me.top -= 10;
                g_me.bottom -= 10;
                break;
            case VK_DOWN:
                g_me.top += 10;
                g_me.bottom += 10;
                break;
            case VK_LEFT:
                g_me.left -= 10;
                g_me.right -= 10;
                break;
            case VK_RIGHT:
                g_me.right += 10;
                g_me.left += 10;
                break;
            }
        }

        ///☆-1 좌표 이동 제한 - 좌표 고정
        if (g_me.left < 0)
        {
            g_me.left = 0;
            g_me.right = g_me.left + 90;
        }
        if (g_me.bottom > g_box.bottom)
        {
            g_me.bottom = g_box.bottom;
            g_me.top = g_me.bottom - 90;
        }
        if (g_me.right > g_box.right)
        {
            g_me.right = g_box.right;
            g_me.left = g_me.right - 90;
        }
        if (g_me.top < 0)
        {
            g_me.top = 0;
            g_me.bottom = g_me.top + 90;
        }

        if (IntersectRect(&a, &g_me, &g_item)) {
            g_score += 50;
            g_item.left = rand() % 500;
            g_item.top = rand() % 500;
            g_item.right = g_item.left + 50;
            g_item.bottom = g_item.top + 50;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
        //좌표 값 수정 완료 => 화면을 다시 그릴 필요가 있다!
        //프로그래머가 WM_PAINT호출 OS에게 요청!

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
    /// 임시 그리기가 지워지는 4가지 경우 ==> os는 WM_PAINT를 호출
    /// 임시 그리기가 아닌 내용은 모두 WM_PAINT 내부에서 그려야 한다.
    /* 
        1.창을 최대화
        2. 창을 원래 크기로 되돌린 경우
        3. 창을 최소화 한 경우
        4. 프로그래머가 요청한 경우
        (win10에서 없어진 것 => 다른 창이 앞을 가리는 경우)
     */
    case WM_PAINT:
        {

            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

                // 나의 객체를 위한 브러시 설정 (파란색)
            HBRUSH hBrushMe = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 브러시 생성
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushMe);

            // 나의 객체 그리기
            Rectangle(hdc, g_me.left, g_me.top, g_me.right, g_me.bottom);

            // 원래 브러시로 되돌리기
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrushMe);

            // 상대 객체를 위한 브러시 설정 (빨간색)
            HBRUSH hBrushYou = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 브러시 생성
            SelectObject(hdc, hBrushYou);

            // 상대 객체 그리기
            Rectangle(hdc, g_you.left, g_you.top, g_you.right, g_you.bottom);

            // 원래 브러시로 되돌리기
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrushYou);

            //아이템 객체 그리기
            Rectangle(hdc, g_item.left, g_item.top, g_item.right, g_item.bottom);

            /// 타이머 바
            Rectangle(hdc, g_bar.left, g_bar.top, g_bar.right, g_bar.bottom);

            // 게임 점수를 출력하는 코드
            wchar_t scoreText[50];
            wsprintf(scoreText, L"Score: %d", g_score);
            TextOut(hdc, 0, 0, scoreText, lstrlen(scoreText));

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
