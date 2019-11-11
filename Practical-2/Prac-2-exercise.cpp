#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#pragma comment(lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

void drawStar();
void rotatePoint();
void expandBox();

int colorIndex, page = 1;
float boxScale = 1;
bool changeColor = false;
float x = 0, y = 0;
bool isRed = false, isGreen = false, isBlue = false, reset = true;

float rotateX = 0, rotateY = 0, rotateAngle = 0;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == VK_F1)
		{
			page = 1;
			changeColor = true;
			isRed = false;
			isGreen = false;
			isBlue = false;
			reset = false;
		}
		else if (wParam == VK_F2)
		{
			page = 2;
		}
		else if (wParam == VK_F3)
		{
			page = 3;
			boxScale = 1;
		}
		else if (wParam == VK_UP)
		{
			y += 0.1;
		}
		else if (wParam == VK_DOWN)
		{
			y -= 0.1;
		}
		else if (wParam == VK_RIGHT)
		{
			x += 0.1;
		}
		else if (wParam == VK_LEFT)
		{
			x -= 0.1;
		}
		else if (wParam == 0x52)
		{
			changeColor = false;
			isRed = true;
			isGreen = false;
			isBlue = false;
			reset = false;
		}
		else if (wParam == 0x47)
		{
			changeColor = false;
			isRed = false;
			isGreen = true;
			isBlue = false;
			reset = false;
		}
		else if (wParam == 0x42)
		{
			changeColor = false;
			isRed = false;
			isGreen = false;
			isBlue = true;
			reset = false;
		}
		else if (wParam == VK_SPACE)
		{
			changeColor = false;
			isRed = false;
			isGreen = false;
			isBlue = false;
			reset = true;
			x = 0;
			y = 0;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// handle resolution problem
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double w = 1920;
	double h = 1080;
	double ar = w / h;
	glOrtho(-2 * ar, 2 * ar, -2, 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (page)
	{
	case 1:
		drawStar();
		break;
	case 2:
		rotateX = rotateX + cos(rotateAngle) * 0.5;
		rotateY = rotateY + sin(rotateAngle) * 0.5;
		rotatePoint();
		Sleep(100);
		rotateAngle += 0.5;
		break;
	case 3:
		expandBox();
		boxScale += 0.01;
		break;
	default:
		break;
	}
}
//--------------------------------------------------------------------

void drawStar()
{
	float r[6] = { 33, 104, 244, 255, 76, 33 };
	float g[6] = { 148, 58, 67, 193, 175, 148 };
	float b[6] = { 243, 183, 54, 7, 79, 243 };

	float rotateDegree = 0;
	for (int i = 0; i < 5; i++)
	{
		if (changeColor)
		{
			colorIndex = colorIndex % 5;
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(x, y, 0.0);
			glRotatef(rotateDegree, 0, 0, 1.0);
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glBegin(GL_POLYGON);

				glColor4f(1, 1, 1, 0.8);
				glVertex2f(0, 0);

				glColor3ub(r[colorIndex + 1], g[colorIndex + 1], b[colorIndex + 1]);
				glVertex2f(0.12, 0.16);

				glColor3ub(r[colorIndex], g[colorIndex], b[colorIndex]);
				glVertex2f(0, 0.8);

				glVertex2f(-0.12, 0.16);
				glEnd();
			}
			glPopMatrix();
			rotateDegree -= 72;
			colorIndex++;
		}
		else
		{
			glLoadIdentity();
			glPushMatrix();
			glTranslatef(x, y, 0.0);
			glRotatef(rotateDegree, 0, 0, 1.0);
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glBegin(GL_POLYGON);

				if (isRed)
				{
					glColor3f(1, 0, 0);
				}
				else if (isGreen)
				{
					glColor3f(0, 1, 0);
				}
				else if (isBlue)
				{
					glColor3f(0, 0, 1);
				}
				else
				{
					glColor3f(1, 1, 1);
				}

				glVertex2f(0, 0);
				glVertex2f(0.12, 0.16);
				glVertex2f(0, 0.8);
				glVertex2f(-0.12, 0.16);
				glEnd();
			}
			glPopMatrix();
			rotateDegree -= 72;
		}
	}
}

void rotatePoint()
{
	glTranslatef(rotateX, rotateY - 0.7, 0.0);
	glPointSize(10);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	glVertex2f(0, 0);
	glEnd();
}

void expandBox()
{
	glScalef(boxScale, boxScale, 1);
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(0.1, 0.1);
	glVertex2f(-0.1, 0.1);
	glVertex2f(-0.1, -0.1);
	glVertex2f(0.1, -0.1);
	glEnd();
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc))
		return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc))
		return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	int changeStarColor = 0;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);

		if (changeColor)
		{
			if (changeStarColor % 150 == 0)
			{
				colorIndex++;
				changeStarColor = 0;
			}
			changeStarColor++;
		}
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
