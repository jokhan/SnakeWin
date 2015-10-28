#include <windows.h>
#include <time.h>
#define LENGTHMAX 1000    //�ߵ���󳤶�
POINT lpSnake[100];   //��¼����
POINT FOOD;           //��¼ʳ��ص�
int X=50,Y=50;              //ʳ������
int dr=1;             //ʳ��뾶
int flag1=0;          //�ж�ʳ�����λ�õĺϷ���,Ϊ��ʱ���Ϸ�
int flag;           //�ж����Ƿ�����,Ϊ��ʱ����
int length=40;      //��¼�ߵĳ���
int left,right,down,up;    //�����ߵ��˶�����
int pause;            //��ͣ���ţ�Ϊ0��ʾ��ͣ

void ProFood()                         //����ʳ��
{
	do 
	{
		SYSTEMTIME st;		
		GetLocalTime(&st);
		srand(st.wMilliseconds);
		X=rand()%490+10;		            //ȡ������ʳ��ų�������Ļ��
		Y=rand()%490+10;		
		for(int j=0;j<length;j++)
		{
			if(X!=lpSnake[j].x||Y!=lpSnake[j].y)          //���ܳ�������������
				flag1=1;
		}
	} while (flag1==0);
	FOOD.x=X;
	FOOD.y=Y;
}

void Start()           //��ʼ������
{
	for(int j=0;j<length;j++)         //��ʼ���ߵ�λ���볤��
	{
		lpSnake[j].x=50;
		lpSnake[j].y=j*2+300;
	}
	flag=1;         //���ǻ��
	left=0;
	right=0;
	down=0;
	up=1;                           //Ĭ�������˶�
	pause=0;
	PlaySound(TEXT("Snake.wav"),NULL, SND_ASYNC|SND_LOOP|SND_NODEFAULT);
	ProFood();           //����ʳ��
}



void EatFood()                   //����ʳ��
{
	if(abs(FOOD.x-lpSnake[0].x)<4&&abs(FOOD.y-lpSnake[0].y)<4)
	{
		ProFood();
		if(length<LENGTHMAX)       //�ﵽ��Ͳ�����
			length++;
	}
}

void SnakeDie()                //С������
{
	if(lpSnake[0].y<=10||lpSnake[0].y>=500||lpSnake[0].x<=10||lpSnake[0].x>=500)          //ײ���߽���
		flag=0;
	else
		for (int j=2;j<length;j++)                 //ײ���Լ���
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
	hwnd=CreateWindow("MY_DRAWING","̰����",WS_SYSMENU|WS_MINIMIZEBOX ,
		0,0,550,600,NULL,NULL,hInstance,NULL);

	SendMessage(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), "LOGO"));     //ͼ��

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
	char lpsz_1[]="�ո�����ƿ�ʼ/��ͣ=_=����������ߵ��˶�����";
	char lpsz_2[]="���߾����ܶ�Զ���!=_=���ܴ�������ǽ�ں�����";
	switch(iMessage)
	{
	case WM_CREATE:		
		Start();
		break;
	case WM_PAINT:    				         //�����ͼ��Ϣ
		{
		hDC=BeginPaint(hWnd,&PtStr);

		/*��д��Ϸ����Ͳ���˵��*/
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
				 "����");
		SelectObject(hDC,hF);
		GetTextMetrics(hDC,&tm);
		SetTextColor(hDC,RGB(0,0,255));
		TextOut(hDC,1,510,lpsz_1,strlen(lpsz_1));          //��д��Ϸ����
		TextOut(hDC,1,540,lpsz_2,strlen(lpsz_2));
		DeleteObject(hF);


		/*�����ƶ�����*/
		if(up==1)
			lpSnake[0].y-=2;      //�����ƶ�2
		else if(down==1)
			lpSnake[0].y+=2;
		else if(left==1)
			lpSnake[0].x-=2;
		else if(right==1)
			lpSnake[0].x+=2;
		for (int j=length;j>0;j--)     //��β����
		{
			lpSnake[j]=lpSnake[j-1];
		}

		hPen_she=CreatePen(PS_DASH,3,RGB(0,0,0));
		hPen_food=CreatePen(PS_SOLID,4,RGB(255,0,0));
		hPen_wall=(HPEN)GetStockObject(BLACK_PEN);
                /*����Χ��ǽ*/
		hB=(HBRUSH)CreateSolidBrush(RGB(255,0,0));
		SelectObject(hDC,hPen_wall);
		Rectangle(hDC,10,10,500,500);              //����
		DeleteObject(hPen_wall);
                  /*����*/
		SelectObject(hDC,hPen_she);
		Polyline(hDC,lpSnake,length);           //�����߻��ߣ���Ϊ����
		DeleteObject(hPen_she);
                 /*��ʳ��*/
		SelectObject(hDC,hPen_food);
		SelectObject(hDC,hB);
		Ellipse(hDC,X-dr,Y-dr,X+dr,Y+dr);        //�����ʳ����һ����С��Բ��ʾ
		DeleteObject(hPen_food);              //drΪ�뾶

		EatFood();
		SnakeDie();
		if(flag==0)                       //������
		{
			KillTimer(hWnd,1);                //û�����᲻ͣ��ʾgame over
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
				if(pause==0)                   //�л�Ϊ��ʼ״̬
				{					
					SetTimer(hWnd,1,20,NULL);
					pause=1;
				}
				else                          //�л�Ϊ��ͣ״̬
				{
					KillTimer(hWnd,1);
					pause=0;
				}
				break;
			case VK_LEFT:
					if(left==1||right==1)       //��ת��ʱ������Ӧ
						break;
					else                        //�������Ϊ��
					{
						left=1;
						up=0;
						down=0;
					}
					break;
			  case VK_RIGHT:
				  if(left==1||right==1)       //��ת��ʱ������Ӧ
					  break;
				  else                        //�������Ϊ��
				  {
					  right=1;
					  up=0;
					  down=0;
				  }
					break;
			  case VK_UP:
				  if(up==1||down==1)       //��ת��ʱ������Ӧ
					  break;
				  else                        //�������Ϊ��
				  {
					  up=1;
					  left=0;
					  right=0;
				  }
					break;
			  case VK_DOWN:
				  if(up==1||down==1)       //��ת��ʱ������Ӧ
					  break;
				  else                        //�������Ϊ��
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
	default:					          //������Ϣ�������
		return(DefWindowProc(hWnd,iMessage,wParam,lParam));
	}
	return 0;
}