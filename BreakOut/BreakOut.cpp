// Test.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "BreakOut.h"
#include "Player.h"
#include "Ball.h"
#include "Brick.h"

#include <iostream>
#include <list>
#include <utility>
#include <ctime>
#include <cstring>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
std::pair<int, int> windowSize = { 300, 400 };  // 창 크기

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
    LoadStringW(hInstance, IDC_BREAKOUT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BREAKOUT));

    // 콘솔창 생성
    /*if (!AllocConsole())
    {
        OutputDebugStringW(L"allocConsole fail\n");
        // ...
    }

    // 콘솔창에 printf 출력할 수 있도록 연결하는 코드
    FILE* fCout;
    _wfreopen_s(&fCout, L"CONOUT$", L"w", stdout);*/

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BREAKOUT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
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

   RECT rt = { 0, 0, windowSize.first, windowSize.second };
   AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

   HWND hWnd = CreateWindowW(szWindowClass, (LPCWSTR)"BREAK OUT", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
       0, 0, rt.right - rt.left, rt.bottom - rt.top, nullptr, nullptr, hInstance, nullptr);
   DWORD style = GetWindowLong(hWnd, GWL_STYLE);
   style &= ~WS_MAXIMIZEBOX;
   SetWindowLong(hWnd, GWL_STYLE, style);
   SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

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

int stage = 0;
int death = 0;
int maxStage = 7;

Player player;
Ball ball;

bool leftPressed = false;
long leftPressedTime = 0;
long leftReleasedTime = 0;

bool rightPressed = false;
long rightPressedTime = 0;
long rightReleasedTime = 0;

long prevTime = 0;
long curTime = 0;

std::list<Brick> bricks;

void initialSetting() {
    bricks.clear();
    for (int i = 0; i < stage + 1; i++) {
        for (int j = 0; j < 6; j++) {
            bricks.push_back(Brick(25 + j * 50, 55 + i * 20, i));
        }
    }
    ball.setInitial();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
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
    case WM_SYSKEYDOWN:
        {
            switch (wParam) {
            }
        }
    case WM_KEYDOWN:
        {
            switch (wParam) {
            case VK_LEFT:
                if (!leftPressed) {
                    leftPressed = true;
                    leftPressedTime = clock();
                }
                break;
            case VK_RIGHT:
                if (!rightPressed) {
                    rightPressed = true;
                    rightPressedTime = clock();
                }
                break;
            case VK_UP:
                break;
            case VK_DOWN:
                break;
            case VK_SPACE:
                if (stage == maxStage) {
                    stage = 0;
                    death = 0;
                }
                else {
                    if (ball.getVelocity() == 0) {
                        initialSetting();
                    }
                }
                break;
            }
        }
        break;
    case WM_KEYUP:
        {
            switch (wParam) {
            case VK_RIGHT:
                rightPressed = false;
                rightReleasedTime = clock();
                break;
            case VK_LEFT:
                leftPressed = false;
                leftReleasedTime = clock();
                break;
            }
        }
        break;
    case WM_PAINT:
        {

            curTime = clock();
            if (curTime - prevTime < 10) {
                break;
            }
            //std::cout << (curTime = clock()) <<'\n';
            PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            //Rectangle(hdc, x, y, x + 100, y + 100);

            //Vector만들고 vector안에 있는 모든 요소 그리기
            
            static HDC hdc, MemDC, tmpDC;
            static HBITMAP BackBit, oldBackBit;
            static RECT bufferRT;
            hdc = BeginPaint(hWnd, &ps);

            GetClientRect(hWnd, &bufferRT);
            MemDC = CreateCompatibleDC(hdc);
            BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
            oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
            PatBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
            tmpDC = hdc;
            hdc = MemDC;
            MemDC = tmpDC;

            player.update();
            ball.update();

            char str[100];

            if (stage < maxStage) {
                sprintf_s(str, "STAGE %d", stage + 1);
                SetTextAlign(hdc, TA_TOP | TA_CENTER);
                SetTextCharacterExtra(hdc, 0);
                TextOut(hdc, 150, 15, str, strlen(str));

                sprintf_s(str, "DEATH: %d", death);
                SetTextAlign(hdc, TA_TOP | TA_RIGHT);
                SetTextCharacterExtra(hdc, 0);
                TextOut(hdc, 280, 15, str, strlen(str));

                if (ball.getVelocity() == 0) {
                    char str[] = "PRESS SPACE!";
                    /*HFONT newFont = CreateFontA(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
                        , VARIABLE_PITCH || FF_SWISS, "");
                    SelectObject(hdc, newFont);*/
                    SetTextAlign(hdc, TA_TOP | TA_CENTER);
                    SetTextCharacterExtra(hdc, 1);
                    TextOut(hdc, 150, 200, str, strlen(str));
                }
            }
            else {
                SetTextAlign(hdc, TA_TOP | TA_CENTER);
                SetTextCharacterExtra(hdc, 0);
                TextOut(hdc, 150, 170, "CONGRATULATIONS!!", strlen("CONGRATULATIONS!!"));

                sprintf_s(str, "DEATH: %d", death);
                SetTextAlign(hdc, TA_TOP | TA_CENTER);
                SetTextCharacterExtra(hdc, 0);
                TextOut(hdc, 150, 190, str, strlen(str));


                SetTextAlign(hdc, TA_TOP | TA_CENTER);
                SetTextCharacterExtra(hdc, 0);
                TextOut(hdc, 150, 220, "PRESS SPACE TO RESTART!", strlen("PRESS SPACE TO RESTART!"));
            }

            ball.paint(hdc);
            player.paint(hdc);

            for (std::list<Brick>::iterator iter = bricks.begin(); iter != bricks.end();iter++) {
                (*iter).paint(hdc);
            }

            tmpDC = hdc;
            hdc = MemDC;
            MemDC = tmpDC;
            GetClientRect(hWnd, &bufferRT);
            BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
            SelectObject(MemDC, oldBackBit);
            DeleteObject(BackBit);
            DeleteDC(MemDC);

            EndPaint(hWnd, &ps);
            prevTime = curTime;
            InvalidateRect(hWnd, NULL, false);
            UpdateWindow(hWnd);

            /*EndPaint(hWnd, &ps);
            prevTime = curTime;
            InvalidateRect(hWnd, NULL, true);
            UpdateWindow(hWnd);*/
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