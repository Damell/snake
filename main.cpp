#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#define POZADI 0
#define HAD 1
#define JIDLO 2
#define POLE 20
#define DIVISIONS 30
#define TIMER_SNAKE 1
#define TIMER_FOOD 2

int pole[DIVISIONS][DIVISIONS];
int direction[4][2] = {0, -1, 1, 0, 0, 1, -1, 0};
int direct;
BOOL end = 0;
int snake[100][2];
int length = 3;
int hungry = 1;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("Piskvorky") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     
     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("Program vyžaduje Windows NT!"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName, TEXT ("SNAKE"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          489, 515,
                          NULL, NULL, hInstance, NULL) ;
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static int  cxBlock, cyBlock ;
     HDC         hdc ;
     int         wnd_x, wnd_y ;
     PAINTSTRUCT ps ;
     POINT       point ;
     RECT        rect ;
     HPEN        hPen;
     HPEN        hPen_had;
     HPEN        hPen_jidlo;
     int         x, y;
     
     hPen = CreatePen(PS_SOLID, 1, RGB(0, 40, 0));
     hPen_had = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
     hPen_jidlo = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

     
     switch (message)
     {
     case WM_CREATE:
          snake[0][0]=20;
          snake[0][1]=20;
          SetTimer(hwnd, TIMER_SNAKE, 150, NULL);
          return 0;
          
     case WM_SIZE :
          cxBlock = LOWORD (lParam) / DIVISIONS ;
          cyBlock = HIWORD (lParam) / DIVISIONS ;
          return 0 ;
          
     case WM_SETFOCUS :
          ShowCursor (TRUE) ;
          return 0 ;
          
     case WM_KILLFOCUS :
          ShowCursor (FALSE) ;
          return 0 ;
          
     case WM_KEYDOWN :       
          switch (wParam)
          {
          case VK_UP :
               direct=0 ;
               break ;
               
          case VK_DOWN :
               direct=2 ;
               break ;
               
          case VK_LEFT :
               direct=3 ;
               break ;
               
          case VK_RIGHT :
               direct=1 ;
               break ;

          case VK_SPACE :
               break ;
          }          
          
          return 0 ;
          
     case WM_TIMER:
          for(int i=length; i>0; i--){
          snake[i][0] = snake[i-1][0];
          snake[i][1] = snake[i-1][1];
          }
          snake[0][0] += direction[direct][0];
          snake[0][1] += direction[direct][1];
          
          if(pole[snake[0][0]][snake[0][1]]==HAD){
          KillTimer(hwnd, TIMER_SNAKE);
          MessageBox(NULL, TEXT("GAME OVER"), TEXT("Message Box"), 0);
          }
          if(snake[0][0] < 0 || snake[0][0] > DIVISIONS-1){
          KillTimer(hwnd, TIMER_SNAKE);
          MessageBox(NULL, TEXT("GAME OVER"), TEXT("Message Box"), 0);
          }
          if(snake[0][1] < 0 || snake[0][1] > DIVISIONS-1){
          KillTimer(hwnd, TIMER_SNAKE);
          MessageBox(NULL, TEXT("GAME OVER"), TEXT("Message Box"), 0);
          }
          if(pole[snake[0][0]][snake[0][1]]==JIDLO){
          length++;
          hungry = 1;
          }
          rect.left   = snake[0][0] * cxBlock ;
          rect.top    = snake[0][1] * cyBlock ;
          rect.right  = (snake[0][0] + 1) * cxBlock ;
          rect.bottom = (snake[0][1] + 1) * cyBlock ;
          InvalidateRect (hwnd, &rect, FALSE) ;
          if(hungry){
                  do{
                  x = rand() % DIVISIONS;
                  y = rand() % DIVISIONS;
                  }while(pole[x][y]==HAD);
                  hungry = 0;
                  pole[x][y] = JIDLO;
                  rect.left   = x * cxBlock ;
                  rect.top    = y * cyBlock ;
                  rect.right  = (x + 1) * cxBlock ;
                  rect.bottom = (y + 1) * cyBlock ;
                  InvalidateRect (hwnd, &rect, FALSE) ;
          }
          rect.left   = snake[length][0] * cxBlock ;
          rect.top    = snake[length][1] * cyBlock ;
          rect.right  = (snake[length][0] + 1) * cxBlock ;
          rect.bottom = (snake[length][1] + 1) * cyBlock ;
          InvalidateRect (hwnd, &rect, FALSE) ;
          return 0;
 
     case WM_PAINT :
          hdc = BeginPaint (hwnd, &ps) ;
          pole[snake[0][0]][snake[0][1]]=HAD;
          pole[snake[length][0]][snake[length][1]]=0;
          SelectObject(hdc, GetStockObject(BLACK_BRUSH));

          for (wnd_x = 0 ; wnd_x < DIVISIONS ; wnd_x++)
          for (wnd_y = 0 ; wnd_y < DIVISIONS ; wnd_y++)
          {
               SelectObject(hdc, hPen);
               Rectangle (hdc, wnd_x * cxBlock, wnd_y * cyBlock,
                          (wnd_x + 1) * cxBlock, (wnd_y + 1) * cyBlock) ;

                    
               if (pole[wnd_x][wnd_y]==HAD)
               {
                    SelectObject(hdc, hPen_had);
                    Ellipse (hdc, wnd_x*cxBlock+1, wnd_y*cyBlock+1, (wnd_x+1)*cxBlock-1, (wnd_y+1)*cyBlock-1);   
               }
               else if(pole[wnd_x][wnd_y]==JIDLO)
               {
                    SelectObject(hdc, hPen_jidlo);
                    Ellipse (hdc, wnd_x*cxBlock+2, wnd_y*cyBlock+2, (wnd_x+1)*cxBlock-2, (wnd_y+1)*cyBlock-2);
               }
          }
          EndPaint (hwnd, &ps) ;
          return 0 ;
               
     case WM_DESTROY :
          DeleteObject(hPen);
          DeleteObject(hPen_had);
          DeleteObject(hPen_jidlo);
          KillTimer(hwnd, TIMER_SNAKE);
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}


