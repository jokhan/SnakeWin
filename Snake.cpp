#include <windows.h>
#include <time.h>
#define LENGTHMAX 1000    //蛇的最大长度
POINT lpSnake[100];   //记录蛇身
POINT FOOD;           //记录食物地点
int X=50,Y=50;              //食物坐标
int dr=1;             //食物半径
int flag1=0;          //判断食物出现位置的合法性,为零时不合法
int flag;           //判断蛇是否死亡,为零时死亡
int length=40;      //记录蛇的长度
int left,right,down,up;    //控制蛇的运动方向
int pause;            //暂停符号，为0表示暂停

void ProFood()                         //产生食物
{
	do 
	{
		SYSTEMTIME st;		
		GetLocalTime(&st);
		srand(st.wMilliseconds);
		X=rand()%490+10;		            //取余数后食物才出现在屏幕内
		Y=rand()%490+10;		
		for(int j=0;j<length;j++)
		{
			if(X!=lpSnake[j].x||Y!=lpSnake[j].y)          //不能出现在蛇身体里
				flag1=1;
		}
	} while (flag1==0);
	FOOD.x=X;
	FOOD.y=Y;
}

void Start()           //初始化界面
{
	for(int j=0;j<length;j++)         //初始化蛇的位置与长度
	{
		lpSnake[j].x=50;
		lpSnake[j].y=j*2+300;
	}
	flag=1;         //蛇是活的
	left=0;
	right=0;
	down=0;
	up=1;                           //默认向上运动
	pause=0;
	PlaySound(TEXT("Snake.wav"),NULL, SND_ASYNC|SND_LOOP|SND_NODEFAULT);
	ProFood();           //出现食物
}



void EatFood()                   //吞噬食物
{
	if(abs(FOOD.x-lpSnake[0].x)<4&&abs(FOOD.y-lpSnake[0].y)<4)
	{
		ProFood();
		if(length<LENGTHMAX)       //达到最长就不长了
			length++;
	}
}

void SnakeDie()                //小蛇死亡
{
	if(lpSnake[0].y<=10||lpSnake[0].y>=500||lpSnake[0].x<=10||lpSnake[0].x>=500)          //撞到边界了
		flag=0;
	else
		for (int j=2;j<length;j++)                 //撞到自己了
		{
			if(lpSnake[0].x!=lpSnake[j].x||lpSnake[0].y!=lpSnake[j].y)
				continue;
			else
			{
				flag=0;
				break;
			}
		}
}

LRESULT CALLBACK WndCenProc(
							HWND hwnd,      
							UINT uMsg,      
							WPARAM wParam,  
							LPARAM lParam   
							);
int WINAPI WinMain(
				   HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow              
				   )
{
	WNDCLASS wc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hIcon=LoadIcon(NULL,"LOGO");
	wc.hInstance=hInstance;
	wc.lpfnWndProc=WndCenProc;
	wc.lpszClassName="MY_DRAWING";
	wc.lpszMenuName=NULL;
	wc.style=CS_HREDRAW |CS_VREDRAW;
	RegisterClass(&wc);
	
	HWND hwnd;
	hwnd=CreateWindow("MY_DRAWING","贪吃蛇",WS_SYSMENU|WS_MINIMIZEBOX ,
		0,0,550,600,NULL,NULL,hInstance,NULL);

	SendMessage(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), "LOGO"));     //图标

	ShowWindow(hwnd,SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT CALLBACK WndCenProc(HWND hWnd,UINT iMessage,
							UINT wParam,LONG lParam)
{
	HDC hDC;
	PAINTSTRUCT PtStr;
	HPEN hPen_she;
	HPEN hPen_food;
	HPEN hPen_wall;
	HFONT hF;
	TEXTMETRIC tm;
	HBRUSH hB;
	char lpsz_1[]="空格键控制开始/暂停=_=方向键控制蛇的运动方向";
	char lpsz_2[]="让蛇尽可能多吃豆子!=_=不能触碰四周墙壁和身体";
	switch(iMessage)
	{
	case WM_CREATE:		
		Start();
		break;
	case WM_PAINT:    				         //处理绘图消息
		{
		hDC=BeginPaint(hWnd,&PtStr);

		/*书写游戏规则和操作说明*/
		hF=CreateFont(                      
			     20,
				 0,
				 0,
				 0,
				 400,
				 0,
				 0,
				 0,
				 GB2312_CHARSET,
				 OUT_DEFAULT_PRECIS,
				 CLIP_DEFAULT_PRECIS,
				 DEFAULT_QUALITY,
				 DEFAULT_PITCH|FF_DONTCARE,
				 "楷体");
		SelectObject(hDC,hF);
		GetTextMetrics(hDC,&tm);
		SetTextColor(hDC,RGB(0,0,255));
		TextOut(hDC,1,510,lpsz_1,strlen(lpsz_1));          //书写游戏规则
		TextOut(hDC,1,540,lpsz_2,strlen(lpsz_2));
		DeleteObject(hF);


		/*控制移动方向*/
		if(up==1)
			lpSnake[0].y-=2;      //向上移动2
		else if(down==1)
			lpSnake[0].y+=2;
		else if(left==1)
			lpSnake[0].x-=2;
		else if(right==1)
			lpSnake[0].x+=2;
		for (int j=length;j>0;j--)     //蛇尾跟上
		{
			lpSnake[j]=lpSnake[j-1];
		}

		hPen_she=CreatePen(PS_DASH,3,RGB(0,0,0));
		hPen_food=CreatePen(PS_SOLID,4,RGB(255,0,0));
		hPen_wall=(HPEN)GetStockObject(BLACK_PEN);
                /*画外围的墙*/
		hB=(HBRUSH)CreateSolidBrush(RGB(255,0,0));
		SelectObject(hDC,hPen_wall);
		Rectangle(hDC,10,10,500,500);              //矩形
		DeleteObject(hPen_wall);
                  /*画蛇*/
		SelectObject(hDC,hPen_she);
		Polyline(hDC,lpSnake,length);           //用折线画蛇，较为逼真
		DeleteObject(hPen_she);
                 /*画食物*/
		SelectObject(hDC,hPen_food);
		SelectObject(hDC,hB);
		Ellipse(hDC,X-dr,Y-dr,X+dr,Y+dr);        //这里的食物用一个很小的圆表示
		DeleteObject(hPen_food);              //dr为半径

		EatFood();
		SnakeDie();
		if(flag==0)                       //蛇死亡
		{
			KillTimer(hWnd,1);                //没有它会不停提示game over
			PlaySound("Over.wav", NULL, SND_ASYNC|SND_NODEFAULT);
			if(IDYES==MessageBox(hWnd,"Game Over!\nPlay again?","Snake",MB_YESNO))
			{				
				Start();
				InvalidateRect(hWnd,NULL,1);
			}
			else
				DestroyWindow(hWnd);
		}

		EndPaint(hWnd,&PtStr);
		}
		break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_SPACE:
				if(pause==0)                   //切换为开始状态
				{					
					SetTimer(hWnd,1,20,NULL);
					pause=1;
				}
				else                          //切换为暂停状态
				{
					KillTimer(hWnd,1);
					pause=0;
				}
				break;
			case VK_LEFT:
					if(left==1||right==1)       //不转弯时无需响应
						break;
					else                        //方向调整为左
					{
						left=1;
						up=0;
						down=0;
					}
					break;
			  case VK_RIGHT:
				  if(left==1||right==1)       //不转弯时无需响应
					  break;
				  else                        //方向调整为右
				  {
					  right=1;
					  up=0;
					  down=0;
				  }
					break;
			  case VK_UP:
				  if(up==1||down==1)       //不转弯时无需响应
					  break;
				  else                        //方向调整为上
				  {
					  up=1;
					  left=0;
					  right=0;
				  }
					break;
			  case VK_DOWN:
				  if(up==1||down==1)       //不转弯时无需响应
					  break;
				  else                        //方向调整为下
				  {
					  down=1;
					  left=0;
					  right=0;
				  }
					break;
			}
		}
		break;
	case WM_TIMER:
		InvalidateRect(hWnd,NULL,1);
		break;
	case WM_DESTROY:
		KillTimer(hWnd,1);
		PostQuitMessage(0);
		break;
	default:					          //其他消息处理程序
		return(DefWindowProc(hWnd,iMessage,wParam,lParam));
	}
	return 0;
}