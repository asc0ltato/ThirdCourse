#include "stdafx.h"

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}
//------------------------------------------------------------------------------
// Устанавливает размеры прямоугольной области
void CRectD::SetRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

//------------------------------------------------------------------------------
// Возвращает размеры области как структура CSizeD (ширина и высота)
CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);	// Ширина прямоугольной области
	cz.cy = fabs(top - bottom);	// Высота прямоугольной области
	return cz;
}

// ============================== АФФИННЫЕ ПРЕОБРАЗОВАНИЯ ==============================

// 1. Создаёт матрицу для смещения объекта по осям X и Y
CMatrix CreateTranslate2D(double dx, double dy)
// Формирует матрицу для преобразования координат объекта при его смещении 
// на dx по оси X и на dy по оси Y в фиксированной системе координат
// --- ИЛИ ---
// Формирует матрицу для преобразования координат объекта при смещении начала
// системы координат на -dx оси X и на -dy по оси Y при фиксированном положении объекта 
{
	CMatrix TM(3, 3);
	TM(0, 0) = 1; TM(0, 2) = dx; // Смещение по оси X
	TM(1, 1) = 1;  TM(1, 2) = dy; // Смещение по оси Y
	TM(2, 2) = 1; // Коэффициент однородности
	return TM;
}

// 1. Создаёт матрицу для поворота объекта на угол fi (в градусах)
//------------------------------------------------------------------------------------
CMatrix CreateRotate2D(double fi)
// Формирует матрицу для преобразования координат объекта при его повороте
// на угол fi (при fi>0 против часовой стрелки)в фиксированной системе координат
// --- ИЛИ ---
// Формирует матрицу для преобразования координат объекта при повороте начала
// системы координат на угол -fi при фиксированном положении объекта 
// fi - угол в градусах
{
	// Нормализуем угол (оставляем в пределах 0 - 360 градусов)
	double fg = fmod(fi, 360.0);
	double ff = (fg / 180.0) * pi; // Перевод в радианы
	CMatrix RM(3, 3);
	RM(0, 0) = cos(ff); RM(0, 1) = -sin(ff);
	RM(1, 0) = sin(ff);  RM(1, 1) = cos(ff);
	RM(2, 2) = 1;
	return RM;
}


// ============================== ПРЕОБРАЗОВАНИЕ КООРДИНАТ ==============================

CMatrix SpaceToWindow(CRectD& RS, CRect& RW)
// Возвращает матрицу пересчета координат из мировых в оконные
// RS - область в мировых координатах - double
// RW - область в оконных координатах - int
{
	CMatrix M(3, 3);
	CSize sz = RW.Size();	 // Размер области в ОКНЕ
	int dwx = sz.cx;	     // Ширина
	int dwy = sz.cy;	     // Высота

	CSizeD szd = RS.SizeD(); // Размер области в МИРОВЫХ координатах
	double dsx = szd.cx;    // Ширина в мировых координатах
	double dsy = szd.cy;    // Высота в мировых координатах

	double kx = (double)dwx / dsx;   // Масштаб по x
	double ky = (double)dwy / dsy;   // Масштаб по y

	// Создание матрицы пересчёта
	M(0, 0) = kx;  M(0, 1) = 0;    M(0, 2) = (double)RW.left - kx * RS.left;
	M(1, 0) = 0;   M(1, 1) = -ky;  M(1, 2) = (double)RW.bottom + ky * RS.bottom;
	M(2, 0) = 0;   M(2, 1) = 0;    M(2, 2) = 1;
	return M;
}


// ============================== УСТАНОВКА РЕЖИМА ОТОБРАЖЕНИЯ ==============================

//4.Позиционирование по центру окна
void SetMyMode(CDC& dc, CRectD& RS, CRect& RW)
// Устанавливает режим отображения MM_ANISOTROPIC и его параметры
// dc - ссылка на класс CDC MFC
// RS -  область в мировых координатах - int
// RW -	 Область в оконных координатах - int  
{
	double dsx = RS.right - RS.left;
	double dsy = RS.top - RS.bottom;
	double xsL = RS.left;
	double ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywH = RW.bottom;

	// Устанавливаем режим аффинного преобразования координат
	dc.SetMapMode(MM_ANISOTROPIC);

	// Масштабируем окно в соответствии с размерами области
	dc.SetWindowExt((int)dsx, (int)dsy);
	dc.SetViewportExt(dwx, -dwy);

	// Центрируем изображение
	dc.SetWindowOrg((int)xsL, (int)ysL);
	dc.SetViewportOrg(xwL, ywH);
}


// ============================== ОПРЕДЕЛЕНИЕ ОБЪЕКТА "ЛОПАСТЬ" ==============================

// 2. Конструктор для задания параметров фигуры
CBlade::CBlade()            
{
	//Конструктивные параметры фигуры (рис.1) определить само-стоятельно в мировой (!) системе координат
	double rS = 30;
	double h = 10 * rS;
	double d = 2 * h;

	//Размеры области D (ширину и высоту) в мировой системе координат для отображения фигуры
	RS.SetRectD(-d, d, d, -d);					// Область системы в мировых координатах
	RW.SetRect(0, 0, 600, 600);					// Область в окне
	MainPoint.SetRect(-rS, rS, rS, -rS);		// Центр вращения

	// Параметры точек лопасти
	FirstTop.SetRect(-5, 5, 5, -5);
	SecondTop.SetRect(-5, 5, 5, -5);
	FirstBootom.SetRect(-5, 5, 5, -5);
	SecondBootom.SetRect(-5, 5, 5, -5);	
	EarthOrbit.SetRect(-h, h, h, -h);	// Орбита вращения

	// Начальные углы вращения
	fiSB = 215;
	fiFB = 205;			
	fiST = 25;			
	fiFT = 35;
	fiSB2 = 295;
	fiFB2 = 305;
	fiST2 = 125;
	fiFT2 = 115;

	// Угловая скорость вращения
	wPoint = -8;

	// Интервал дискретизации
	dt = 0.1;
	FTCoords.RedimMatrix(3);
	STCoords.RedimMatrix(3);
	FBCoords.RedimMatrix(3);
	SBCoords.RedimMatrix(3);

	FT2Coords.RedimMatrix(3);
	ST2Coords.RedimMatrix(3);
	FB2Coords.RedimMatrix(3);
	SB2Coords.RedimMatrix(3);
}

// ============================== ОБНОВЛЕНИЕ ПОЛОЖЕНИЯ ==============================
// 
//Параметр вращения реализуется через угол, на который поворачивается фигура при каждом вызове пересчёта координат
void CBlade::SetNewCoords()
//Вычисляет новые координаты планет в СК Солнца через интервал времени dt
{
	//Параметр вращения фигуры определяется как угол на который поворачивается фигура между двумя последовательными перерисовками.  
	// Новые координаты для первой верхней точки 
	double RoV = (EarthOrbit.right - EarthOrbit.left) / 2;
	double ff = (fiFT / 90.0) * pi;

	// Первая верхняя точка
	double x = RoV * cos(ff);
	double y = RoV * sin(ff);
	FTCoords(0) = x;
	FTCoords(1) = y;
	FTCoords(2) = 1;

	// Поворот на угловую скорость wPoint * dt
	fiFT += wPoint * dt;
	CMatrix P = CreateRotate2D(fiFT);
	FTCoords = P * FTCoords;

	// Новые координаты для второй верхней точки 
	RoV = (EarthOrbit.right - EarthOrbit.left) / 2;
	ff = (fiST / 90.0) * pi;

	x = RoV * cos(ff);
	y = RoV * sin(ff);

	STCoords(0) = x;
	STCoords(1) = y;
	STCoords(2) = 1;

	fiST += wPoint * dt;
	P = CreateRotate2D(fiST);
	STCoords = P * STCoords;

	// Новые координаты для первой нижней точки 
	RoV = (EarthOrbit.right - EarthOrbit.left) / 2;
	ff = (fiFB / 90.0) * pi;

	x = RoV * cos(ff);
	y = RoV * sin(ff);

	FBCoords(0) = x;
	FBCoords(1) = y;
	FBCoords(2) = 1;

	fiFB += wPoint * dt;
	P = CreateRotate2D(fiFB);
	FBCoords = P * FBCoords;

	// Новые координаты для второй верхней точки 
	RoV = (EarthOrbit.right - EarthOrbit.left) / 2;
	ff = (fiSB / 90.0) * pi;
	x = RoV * cos(ff);
	y = RoV * sin(ff);
	SBCoords(0) = x;
	SBCoords(1) = y;
	SBCoords(2) = 1;

	fiSB += wPoint * dt;
	P = CreateRotate2D(fiSB);
	SBCoords = P * SBCoords;

	// =============================================

	// Новые координаты для первой верхней точки 
	RoV = (EarthOrbit.right - EarthOrbit.left) / 2;
	ff = (fiFT2 / 90.0) * pi;

	x = RoV * cos(ff);
	y = RoV * sin(ff);

	FT2Coords(0) = x;
	FT2Coords(1) = y;
	FT2Coords(2) = 1;

	fiFT2 += wPoint * dt;
	P = CreateRotate2D(fiFT2);
	FT2Coords = P * FT2Coords;

	// Новые координаты для второй верхней точки 
	RoV = (EarthOrbit.right - EarthOrbit.left) / 2;
	ff = (fiST2 / 90.0) * pi;

	x = RoV * cos(ff);
	y = RoV * sin(ff);

	ST2Coords(0) = x;
	ST2Coords(1) = y;
	ST2Coords(2) = 1;

	fiST2 += wPoint * dt;
	P = CreateRotate2D(fiST2);
	ST2Coords = P * ST2Coords;

	// Новые координаты для первой нижней точки 
	RoV = (EarthOrbit.right - EarthOrbit.left) / 2;
	ff = (fiFB2 / 90.0) * pi;

	x = RoV * cos(ff);
	y = RoV * sin(ff);

	FB2Coords(0) = x;
	FB2Coords(1) = y;
	FB2Coords(2) = 1;

	fiFB2 += wPoint * dt;
	P = CreateRotate2D(fiFB2);
	FB2Coords = P * FB2Coords;

	// Новые координаты для второй верхней точки 
	RoV = (EarthOrbit.right - EarthOrbit.left) / 2;
	ff = (fiSB2 / 90.0) * pi;
	x = RoV * cos(ff);
	y = RoV * sin(ff);
	SB2Coords(0) = x;
	SB2Coords(1) = y;
	SB2Coords(2) = 1;

	fiSB2 += wPoint * dt;
	P = CreateRotate2D(fiSB2);
	SB2Coords = P * SB2Coords;
}

// ============================== РИСОВАНИЕ ==============================

//7.Рисование лопастей
void CBlade::Draw(CDC& dc)
{
	CBrush SBrush, EBrush, MBrush, VBrush, * pOldBrush;
	CRect R;

	SBrush.CreateSolidBrush(RGB(0, 255, 0));
	EBrush.CreateSolidBrush(RGB(0, 0, 255));
	MBrush.CreateSolidBrush(RGB(0, 255, 0));
	VBrush.CreateSolidBrush(RGB(128, 64, 64));

	dc.SelectStockObject(NULL_BRUSH);		
	dc.Ellipse(EarthOrbit);		 // Рисуем орбиту вращения			

	// Рисуем точки у углов лопастей
	int d = FirstTop.right;				// Первая верхняя
	R.SetRect(FTCoords(0) - d, FTCoords(1) + d, FTCoords(0) + d, FTCoords(1) - d);

	d = SecondTop.right;				// Вторая верхняя
	R.SetRect(STCoords(0) - d, STCoords(1) + d, STCoords(0) + d, STCoords(1) - d);

	d = FirstBootom.right;				// Первая нижняя
	R.SetRect(FBCoords(0) - d, FBCoords(1) + d, FBCoords(0) + d, FBCoords(1) - d);

	d = SecondBootom.right;				// Вторая нижняя
	R.SetRect(SBCoords(0) - d, SBCoords(1) + d, SBCoords(0) + d, SBCoords(1) - d);

	// ===============================================

	// Рисуем точки у углов лопастей
	d = FirstTop2.right;				// Первая верхняя
	R.SetRect(FT2Coords(0) - d, FT2Coords(1) + d, FT2Coords(0) + d, FT2Coords(1) - d);

	d = SecondTop2.right;				// Вторая верхняя
	R.SetRect(ST2Coords(0) - d, ST2Coords(1) + d, ST2Coords(0) + d, ST2Coords(1) - d);

	d = FirstBootom2.right;				// Первая нижняя
	R.SetRect(FB2Coords(0) - d, FB2Coords(1) + d, FB2Coords(0) + d, FB2Coords(1) - d);

	d = SecondBootom2.right;				// Вторая нижняя
	R.SetRect(SB2Coords(0) - d, SB2Coords(1) + d, SB2Coords(0) + d, SB2Coords(1) - d);

	// Рисуем первую лопасть
	DrawTriangle(FTCoords, STCoords, dc);

	// Рисуем вторую лопасть
	DrawTriangle(FBCoords, SBCoords, dc);

	// ==========================================

	// Рисуем третью лопасть
	DrawTriangle2(FT2Coords, ST2Coords, dc);

	// Рисуем четвёртую лопасть
	DrawTriangle2(FB2Coords, SB2Coords, dc);

	pOldBrush = dc.SelectObject(&SBrush);	// Цвет зеленый

	// Рисуем точку по центру
	dc.Ellipse(MainPoint);  

	dc.SelectObject(pOldBrush);				//Восстанавливаем контекст по pOldBrush 
}

void CBlade::DrawTriangle(CMatrix FTCoords, CMatrix STCoords, CDC& dc) {
	CPen Pen(PS_SOLID, 1, RGB(255, 0, 0));
	float dy = max(FTCoords(1), STCoords(1)) - min(FTCoords(1), STCoords(1));
	float dx = max(FTCoords(0), STCoords(0)) - min(FTCoords(0), STCoords(0));
	float y = 0;
	float x1 = 0;
	int x2 = 0;
	dc.SelectObject(&Pen);
	if (FTCoords(0) < STCoords(0) && FTCoords(1) > STCoords(1)) {
		if (dx > dy) {
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x <= STCoords(0); x++) {
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) - y);
				if (dy > y) {
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y >= STCoords(1); y--) {
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) + x1, y);
				if (dx > x1) {
					x1 += deltaX;
				}
			}
		}

	}
	if (FTCoords(0) > STCoords(0) && FTCoords(1) > STCoords(1)) {
		if (dx > dy) {
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x >= STCoords(0); x--) {
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) - y);
				if (dy > y) {
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y >= STCoords(1); y--) {
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) - x1, y);
				if (dx > x1) {
					x1 += deltaX;
				}
			}
		}
	}
	if (FTCoords(0) > STCoords(0) && FTCoords(1) < STCoords(1)) {
		if (dx > dy) {
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x >= STCoords(0); x--) {
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) + y);
				if (dy > y) {
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y <= STCoords(1); y++) {
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) - x1, y);
				if (dx > x1) {
					x1 += deltaX;
				}
			}
		}
	}
	if (FTCoords(0) < STCoords(0) && FTCoords(1) < STCoords(1)) {
		if (dx > dy) {
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x <= STCoords(0); x++) {
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) + y);
				if (dy > y) {
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y <= STCoords(1); y++) {
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) + x1, y);
				if (dx > x1) {
					x1 += deltaX;
				}
			}
		}
	}
}

void CBlade::DrawTriangle2(CMatrix FTCoords, CMatrix STCoords, CDC& dc) {
	CPen Pen(PS_SOLID, 1, RGB(0, 0, 255));
	float dy = max(FTCoords(1), STCoords(1)) - min(FTCoords(1), STCoords(1));
	float dx = max(FTCoords(0), STCoords(0)) - min(FTCoords(0), STCoords(0));
	float y = 0;
	float x1 = 0;
	int x2 = 0;
	dc.SelectObject(&Pen);
	if (FTCoords(0) < STCoords(0) && FTCoords(1) > STCoords(1)) {
		if (dx > dy) {
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x <= STCoords(0); x++) {
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) - y);
				if (dy > y) {
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y >= STCoords(1); y--) {
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) + x1, y);
				if (dx > x1) {
					x1 += deltaX;
				}
			}
		}

	}
	if (FTCoords(0) > STCoords(0) && FTCoords(1) > STCoords(1)) {
		if (dx > dy) {
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x >= STCoords(0); x--) {
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) - y);
				if (dy > y) {
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y >= STCoords(1); y--) {
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) - x1, y);
				if (dx > x1) {
					x1 += deltaX;
				}
			}
		}
	}
	if (FTCoords(0) > STCoords(0) && FTCoords(1) < STCoords(1)) {
		if (dx > dy) {
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x >= STCoords(0); x--) {
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) + y);
				if (dy > y) {
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y <= STCoords(1); y++) {
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) - x1, y);
				if (dx > x1) {
					x1 += deltaX;
				}
			}
		}
	}
	if (FTCoords(0) < STCoords(0) && FTCoords(1) < STCoords(1)) {
		if (dx > dy) {
			float deltaY = dy / dx;
			for (int x = FTCoords(0); x <= STCoords(0); x++) {
				dc.MoveTo(0, 0);
				dc.LineTo(x, FTCoords(1) + y);
				if (dy > y) {
					y += deltaY;
				}
			}
		}
		else {
			float deltaX = dx / dy;
			for (int y = FTCoords(1); y <= STCoords(1); y++) {
				dc.MoveTo(0, 0);
				dc.LineTo(FTCoords(0) + x1, y);
				if (dx > x1) {
					x1 += deltaX;
				}
			}
		}
	}
}


void CBlade::GetRS(CRectD& RSX)
// RS - структура, куда записываются параметры области графика
{
	RSX.left = RS.left;
	RSX.top = RS.top;
	RSX.right = RS.right;
	RSX.bottom = RS.bottom;
}