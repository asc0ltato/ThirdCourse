
// ChildView.cpp: реализация класса CChildView
//

#include "stdafx.h"
#include "Lab02.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_TESTS_F1, &CChildView::OnTestsF1)
	ON_COMMAND(ID_TESTS_F2, &CChildView::OnTestsF2)
	ON_COMMAND(ID_TESTS_F3, &CChildView::OnTestsF3)
END_MESSAGE_MAP()



// Обработчики сообщений CChildView

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // контекст устройства для рисования
	if (Index == 1)
	{
		Graph.Draw(dc, 1, 1);
	}

	if (Index == 2)
	{
		Graph.GetRS(RS);
		SetMyMode(dc, CRectD(1, 10, 5, 10), CRect(1, 1, 2, 2));	    // Устанавливает режим отображения MM_ANISOTROPIC 
		Graph.Draw1(dc, 1, 1);
		dc.SetMapMode(MM_TEXT);										// Устанавливает режим отображения MM_TEXT
	}
}

double CChildView::MyF1(double x)
{
	double y = sin(x) / x;
	return y;
}

double CChildView::MyF2(double x)
{
	double y = sqrt(fabs(x))* sin(x);
	return y;
}

void CChildView::OnTestsF1() // MM_TEXT
{
	Invalidate(); // Перерисовка окна
	CPaintDC dc(this);

	double xL = -3 * pi;
	double xH = -xL;
	double dx = pi / 36; // шаг
	int N = static_cast<int>(round((xH - xL) / dx));

	// Изменяет размер массивов с уничтожением старых данных
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);

	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF1(X(i));
	}

	// Параметры пера для графика (красный цвет, толщина 1, сплошная линия)
	PenLine.Set(PS_SOLID, 1, RGB(255, 0, 0));
	// Параметры пера для осей координат (синий цвет, толщина 2)
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));

	// Прямоугольная область для графика (по центру окна)
	RW.SetRect(200, 200, 600, 600);

	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);

	Index = 1;

	// Вызов для отображения графика в режиме MM_TEXT (1, 1 = рисовать график и оси)
	Graph.Draw(dc, 1, 1);
}

void CChildView::OnTestsF2()
{
	Invalidate(); // Перерисовка окна
	CPaintDC dc(this);

	double xH = 4 * pi;
	double xL = -xH;
	double dx = pi / 36;
	int N = static_cast<int>(round((xH - xL) / dx));

	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);

	for (int i = 0; i <= N; i++)
	{
		X(i) = xL + i * dx;
		Y(i) = MyF2(X(i));
	}

	// Параметры пера для графика (штрих-пунктирная линия, красный цвет, толщина 3)
	PenLine.Set(PS_DASHDOT, 3, RGB(255, 0, 0));
	// Параметры пера для осей координат (сплошная линия, черный цвет, толщина 2)
	PenAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));

	// Прямоугольник области графика (по центру окна)
	RW.SetRect(200, 200, 600, 600);

	Graph.SetParams(X, Y, RW);
	Graph.SetPenLine(PenLine);
	Graph.SetPenAxis(PenAxis);

	// Вызов для отображения графика в режиме MM_TEXT (все единицы = показать график и оси)
	Graph.Draw(dc, 1, 1);

	// Если нужно сразу показать обновленный график:
	Index = 1;
}

void CChildView::OnTestsF3()
{
	Invalidate();

	CPaintDC dc(this);

	int N = 8;  // Восьмиугольник
	double R = 10; // Радиус окружности
	X.RedimMatrix(N + 1);
	Y.RedimMatrix(N + 1);

	for (int i = 0; i < N; i++)
	{
		double angle = 2 * pi * i / N; // Угол в радианах
		X(i) = R * cos(angle);
		Y(i) = R * sin(angle);
	}

	X(N) = X(0);
	Y(N) = Y(0);

	PenLine.Set(PS_SOLID, 3, RGB(255, 0, 0));
	RW.SetRect(200, 200, 600, 600);
	Graph.SetParams(X - 0.01, Y, RW);
	Graph.SetPenLine(PenLine);
	// Передаем параметр, отключающий оси
	Graph.Draw(dc, 0, -1);  // Измени значение на -1 или другой параметр, отключающий оси

	N = 1800;
	X.RedimMatrix(N);
	Y.RedimMatrix(N);

	double radius = 10.0;

	for (int i = 0; i < N; i++)
	{
		double angle = 2.0 * 3.14 * i / N;
		X(i) = radius * cos(angle);
		Y(i) = radius * sin(angle);
	}

	PenLine.Set(PS_SOLID, 2, RGB(0, 0, 255));
	RW.SetRect(200, 200, 600, 600);
	Graph.SetParams(X - 0.01, Y, RW);
	Graph.SetPenLine(PenLine);
	// Передаем параметр, отключающий оси
	Graph.Draw(dc, 0, -1);  // Измени значение на -1 или другой параметр, отключающий оси
}