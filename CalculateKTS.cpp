#include <iostream>
#include <math.h>
#include <stdio.h>

/*Прототипы функций*/
float pgenPSI (float, int);
float ttTin (float, float, float);
float ttPSI (float, float, char, int);
float q0pr (float);
float t0pr (float, float);
float g0pr (float);
float coeffA (float, float, float, float, float, float);
float coeffB (float, float, float, float, float, float, float);
float matrixKramer(float*, float*, int);
void coeffSquadReg (float, float, float, float, float, float, float* , float*, float*);
void calcKTS (float, float, float, float, float, float, float, char, float*, float*, float*, float*, float*, float*, float*, float*, float*, float*);
/*=====================================================================================*/

/*Основной алгоритм*/
int main() {

float 	pa0 	= 760.0f	, // Значение атмосферного давления при проведении ПСИ (из формуляра двигателя), мм.рт.ст.
		tv0 	= 15.0f		, // Значение температуры наружного воздуха при проведении ПСИ (из формуляра двигателя), °C
		graph3F	= 0.0f		, // Значение коэффициента "F" из уравнения прямой (график 3)
		graph3G = 0.0f		, // Значение коэффициента "G" из уравнения прямой (график 3)
		graph3H	= 0.0f		, // Значение коэффициента "H" из уравнения прямой (график 3)
		Qv 		= 8071.0f	, // Наименьшая объёмная теплота сгорания ТГ (из протокола компонентного состава газа), ккал/м3
		rotg 	= 0.683f	, // Плотность топливного газа (из протокола компонентного состава газа), кг/м3
		TinD 	= 12.0f		, // Температура воздуха на входе в двигатель, °C
		Pa 		= 745.0f	, // Атмосферное давление, мм.рт.ст.
		Tt 		= 320.0f	, // Температура газов за турбиной, °C
		Pact 	= 1000.0f	, // Активная мощность генератора, кВт
		Qtg 	= 890.0f	, // Объемный расход топливного газа, нм3/час
		kpdD 	= 0.0f		, // Эффективный КПД двигателя, %
		kpdEt	= 0.0f		, // Эффективный КПД двигателя (ЭТАЛОН), %
		dKPD	= 0.0f		, // Отклонение эффективного отклонения КПД от эталонного значения, %отн
		gtg		= 0.0f		, // Массовый расход топливного газа, кг/час
		gtgpr	= 0.0f		, // Приведенный массовый расход топливного газа, кг/час
		g0et	= 0.0f		, // Расход топливного газа (ЭТАЛОН), кг/час
		ktsTG	= 0.0f		, // КТС по расходу топливного газа
		ttpr	= 0.0f		, // Приведенная температура газов за турбиной, °C
		pactet	= 0.0f		, // Мощность на клеммах генератора (ЭТАЛОН), кВт
		ktsP	= 0.0f		; // КТС по мощности

char 	pos_on 	= false		; // Положение ПОС (FALSE = 0 = Закрыт, TRUE = 1 = Открыт)

calcKTS(Qv, rotg, TinD, Pa, Tt, Pact, Qtg, pos_on, &kpdD, &kpdEt, &dKPD, &gtg, &gtgpr, &g0et, &ktsTG, &ttpr, &pactet, &ktsP);

}
/*==================================================================================================================================================================*/

void calcKTS (float Qv, float rotg, float TinD, float Pa, float Tt, float Pact, float Qtg, char pos_on, float *kpdD, float *kpdEt, float *dKPD, float *gtg, float *gtgpr, float *g0et, float *ktsTG, float *ttpr, float *pactet, float *ktsP) {

static const float	graph2C = 0.000000000950f	, // Значение коэффициента "C" из уравнения прямой (график 2)
                    graph2D = -0.000006769918f	, // Значение коэффициента "D" из уравнения прямой (график 2)
                    graph2E = 0.019583057010f	; // Значение коэффициента "E" из уравнения прямой (график 2)

float //pa0 	= 760.0f	, // Значение атмосферного давления при проведении ПСИ (из формуляра двигателя), мм.рт.ст.
		tv0 	= 15.0f		, // Значение температуры наружного воздуха при проведении ПСИ (из формуляра двигателя), °C
		graph3F	= 0.0f		, // Значение коэффициента "F" из уравнения прямой (график 3)
		graph3G = 0.0f		, // Значение коэффициента "G" из уравнения прямой (график 3)
		graph3H	= 0.0f		; // Значение коэффициента "H" из уравнения прямой (график 3)

int max = 0	, // Константная величина для расчета максимальной мощности и температуры
	mid = 1	, // Константная величина для расчета средней мощности и температуры
	min = 2	; // Константная величина для расчета минимальной мощности и температуры

float 	pmax = pgenPSI(Pact, max), // Максимальная мощность на клеммах генератора при проведении ПСИ (из формуляра двигателя)
		pmid = pgenPSI(Pact, mid), // Средняя мощность на клеммах генератора при проведении ПСИ (из формуляра двигателя)
		pmin = pgenPSI(Pact, min); // Минимальная мощность на клеммах генератора при проведении ПСИ (из формуляра двигателя)

float 	tmax = ttPSI(TinD, Pact, pos_on, max), // Максимальная температура газов за турбиной при проведении ПСИ (из формуляра двигателя)
		tmid = ttPSI(TinD, Pact, pos_on, mid), // Средняя температура газов за турбиной при проведении ПСИ (из формуляра двигателя)
		tmin = ttPSI(TinD, Pact, pos_on, min); // Минимальная температура газов за турбиной при проведении ПСИ (из формуляра двигателя)

float 	q0pr_max = q0pr(pmax), // Максимальный приведенный объёмный расхода топливного газа (к 15 °С и 760 мм.рт.ст.)
		q0pr_mid = q0pr(pmid), // Средний приведенный объёмный расхода топливного газа (к 15 °С и 760 мм.рт.ст.)
		q0pr_min = q0pr(pmin); // Минимальный приведенный объёмный расхода топливного газа (к 15 °С и 760 мм.рт.ст.)

float 	t0pr_max = t0pr(tmax, tv0), // Максимальное приведенное значение температуры газа за турбиной (к 15 °С и 760 мм.рт.ст.)
		t0pr_mid = t0pr(tmid, tv0), // Среднее приведенное значение температуры газа за турбиной (к 15 °С и 760 мм.рт.ст.)
		t0pr_min = t0pr(tmin, tv0); // Минимальное приведенное значение температуры газа за турбиной (к 15 °С и 760 мм.рт.ст.)

float 	g0pr_max = g0pr(q0pr_max), // Максимальный приведенный массовый расхода топливного газа (к 15 °С и 760 мм.рт.ст.)
		g0pr_mid = g0pr(q0pr_mid), // Средний приведенный массовый расхода топливного газа (к 15 °С и 760 мм.рт.ст.)
		g0pr_min = g0pr(q0pr_min); // Минимальный приведенный массовый расхода топливного газа (к 15 °С и 760 мм.рт.ст.)

float 	A = coeffA(g0pr_max, g0pr_mid, g0pr_min, pmax, pmid, pmin),		// Значение углового коэффициента "А" из уравнения прямой (график 1)
		B = coeffB(g0pr_max, g0pr_mid, g0pr_min, pmax, pmid, pmin, A);	// Значение коэффициента "В" (точка пересечения с осью абсцисс) из уравнения прямой (график 1)

coeffSquadReg(t0pr_max, t0pr_mid, t0pr_min, pmax, pmid, pmin, &graph3F, &graph3G, &graph3H); // Вызов функции расчета коэффициентов F, G, H графика 3

/*Расчет отклонения КПД двигателя от эталонных значений*/
*kpdD 	= 860.0f * Pact / (Qtg * Qv) * 100.0f								; // Расчет эффективного КПД двигателя, %
*kpdEt	= graph2C * pow(Pact, 3) + graph2D * pow(Pact, 2) + graph2E * Pact	; // Расчет эффективного КПД двигателя (ЭТАЛОН), %
*dKPD	= (*kpdEt - *kpdD) / *kpdEt * 100.0f								; // Расчет отклонения эффективного отклонения КПД от эталонного значения, %отн
/*===========================================================================*/

/*Расчет КТС по расходу топливного газа*/
*gtg	= Qtg * rotg								        							; // Массовый расход топливного газа, кг/час
*gtgpr	= *gtg * (760.0f / Pa) * pow((288.0f / (TinD + 273.15f)), 0.5f) * Qv / 8000.0f	; // Приведенный массовый расход топливного газа, кг/час
*g0et	= A * Pact + B																	; // Расход топливного газа (ЭТАЛОН), кг/час
*ktsTG	= *gtgpr / *g0et																; // КТС по расходу топливного газа
/*===========================================================================*/

/*Расчет КТС по мощности*/
*ttpr	= (Tt + 273.15f) * 288.15f / (TinD + 273.15f) - 273.15f	        ; // Приведенная температура газов за турбиной, °C
*pactet	= graph3F * pow(*ttpr, 2) + 	graph3G * *ttpr + graph3H		; // Мощность на клеммах генератора (ЭТАЛОН), кВт
*ktsP	= Pact / *pactet												; // КТС по мощности

printf("\nPgenPSI_max = %5.0f \nPgenPSI_mid = %5.0f \nPgenPSI_min = %5.0f", pmax, pmid, pmin);
printf("\n\nTtPSI_max = %5.0f \nTtPSI_mid = %5.0f \nTtPSI_min = %5.0f", tmax, tmid, tmin);
printf("\n\nq0pr_max = %5.0f \nq0pr_mid = %5.0f \nq0pr_min = %5.0f", q0pr_max, q0pr_mid, q0pr_min);
printf("\n\nt0pr_max = %5.0f \nt0pr_mid = %5.0f \nt0pr_min = %5.0f", t0pr_max, t0pr_mid, t0pr_min);
printf("\n\ng0pr_max = %5.0f \ng0pr_mid = %5.0f \ng0pr_min = %5.0f", g0pr_max, g0pr_mid, g0pr_min);
printf("\n\ncoefficient A = %4.2f \ncoefficient B = %4.2f", A, B);
printf("\n\ncoefficient graph3 F = %4.2f \ncoefficient graph3 G = %4.2f \ncoefficient graph3 H = %4.2f", graph3F, graph3G, graph3H);
printf("\n\ncoefficient graph2 C = %2.10f \ncoefficient graph2 D = %2.10f \ncoefficient graph2 E = %2.10f", graph2C, graph2D, graph2E);
printf("\n\nKPD = %4.2f \nKPD Etalon = %4.2f \ndKPD = %4.2f", *kpdD, *kpdEt, *dKPD);
printf("\n\nGtg = %4.2f \nGtgpr = %4.2f \nG0Etalon = %4.2f \nKTStg = %4.2f", *gtg, *gtgpr, *g0et, *ktsTG);
printf("\n\nTtpr = %4.2f \nPact ETALON = %4.2f \nKTSP = %4.2f", *ttpr, *pactet, *ktsP);
}

/*Функция расчета мощности на клеммах генератора при проведении ПСИ (из формуляра двигателя), кВт (N0)*/
float pgenPSI (float p, int type) {
float pform;
switch(type) {
/*Максимальная мощность*/
case 0:		if (p < 1000.0f) pform = 1000.0f;
			else if ((p >= 1000.0f) && (p < 1750.0f)) pform = 1500.0f;
			else if ((p >= 1750.0f) && (p <= 3000.0f)) pform = 3000.0f;
			else pform = 0.0f;
			break;
/*Средняя мощность*/
case 1:		if (p < 1000.0f) pform = 500.0f;
			else if ((p >= 1000.0f) && (p < 1750.0f)) pform = 1250.0f;
			else if ((p >= 1750.0f) && (p <= 3000.0f)) pform = 2500.0f;
			else pform = 0.0f;
			break;
/*Минимальная мощность*/
case 2:		if (p < 1000.0f) pform = 0.0f;
			else if ((p >= 1000.0f) && (p < 1750.0f)) pform = 1000.0f;
			else if ((p >= 1750.0f) && (p <= 3000.0f)) pform = 2000.0f;
			else pform = 0.0f;
			break; }
return pform;
}
/*=================================================================================================*/

/*Функция зависимости температуры газа за турбиной от температуры на входе в двигатель*/
float ttTin (float tin, float pact, float tconst) {
if (pact <= 1500.0f) {
	if (tin  <= -35.0f) 					return (tconst-100.0f);
	if ((tin <= -30.0f) && (tin > -35.0f)) 	return (tconst-30.0f);
	if ((tin <= -25.0f) && (tin > -30.0f)) 	return (tconst-40.0f);
	if ((tin <= -20.0f) && (tin > -25.0f))	return (tconst-90.0f);
	if ((tin <= -15.0f) && (tin > -20.0f)) 	return (tconst-70.0f);
	if ((tin <= -10.0f) && (tin > -15.0f)) 	return (tconst-55.0f);
	if ((tin <= 0.0f) 	&& (tin > -10.0f)) 	return (tconst-120.0f);
	if ((tin <= 15.0f) 	&& (tin > 0.0f)) 	return (tconst-120.0f);
	if ((tin <= 25.0f) 	&& (tin > 15.0f)) 	return (tconst-130.0f);
	if ((tin <= 35.0f) 	&& (tin > 25.0f)) 	return (tconst-135.0f);
	if ((tin <= 50.0f) 	&& (tin > 35.0f)) 	return (tconst-140.0f);
	}
else if ((pact > 1500.0f) && (pact <= 3000.0f)) {
	if (tin  <= -35.0f) 					return (tconst-75.0f);
	if ((tin <= -30.0f) && (tin > -35.0f)) 	return (tconst-85.0f);
	if ((tin <= -25.0f) && (tin > -30.0f)) 	return (tconst-95.0f);
	if ((tin <= -20.0f) && (tin > -25.0f)) 	return (tconst-105.0f);
	if ((tin <= -15.0f) && (tin > -20.0f)) 	return (tconst-110.0f);
	if ((tin <= -10.0f) && (tin > -15.0f)) 	return (tconst-120.0f);
	if ((tin <= -5.0f) 	&& (tin > -10.0f)) 	return (tconst-125.0f);
	if ((tin <= 0.0f) 	&& (tin > -5.0f)) 	return (tconst-110.0f);
	if ((tin <= 5.0f) 	&& (tin > 0.0f)) 	return (tconst-120.0f);
	if ((tin <= 15.0f) 	&& (tin > 5.0f)) 	return (tconst-130.0f);
	if ((tin <= 25.0f) 	&& (tin > 15.0f)) 	return (tconst-140.0f);
	if ((tin <= 45.0f) 	&& (tin > 25.0f)) 	return (tconst-130.0f);
	if ((tin <= 50.0f) 	&& (tin > 45.0f)) 	return (tconst-150.0f);
	}
else return 0.0;
}
/*=================================================================================================*/

/*Функция расчета температуры газа за турбиной при проведении ПСИ (из формуляра двигателя), °C (Tg0)*/
float ttPSI (float tin, float p, char pos, int type) {
float tform, calcPOS;
(pos) ? calcPOS = 1.13f : calcPOS = 1.0f;
switch(type) {
/*Максимальная температура*/
case 0:		if (p < 1000.0f) tform = ttTin(tin, p, 410.0f) * calcPOS;
			else if ((p >= 1000.0f) && (p < 1700.0f)) tform = ttTin(tin, p, 450.0f) * calcPOS;
			else if ((p >= 1700.0f) && (p <= 3000.0f)) tform = ttTin(tin, p, 475.0f) * calcPOS;
			else tform = 0.0f;
			break;
/*Средняя температура*/
case 1:		if (p < 1000.0f) tform = ttTin(tin, p, 430.0f) * calcPOS;
			else if ((p >= 1000.0f) && (p < 1700.0f)) tform = ttTin(tin, p, 430.0f) * calcPOS;
			else if ((p >= 1700.0f) && (p <= 3000.0f)) tform = ttTin(tin, p, 440.0f) * calcPOS;
			else tform = 0.0f;
			break;
/*Минимальная температура*/
case 2:		if (p < 1000.0f) tform = ttTin(tin, p, 450.0f) * calcPOS;
			else if ((p >= 1000.0f) && (p < 1700.0f)) tform = ttTin(tin, p, 410.0f) * calcPOS;
			else if ((p >= 1700.0f) && (p <= 3000.0f)) tform = ttTin(tin, p, 430.0f) * calcPOS;
			else tform = 0.0f;
			break; }
return tform;
}
/*=================================================================================================*/

/*Функция расчета приведенного объемного расхода топливного газа (к 15°C и 760 мм.рт.ст.), м3/час*/
float q0pr (float pform) {
float calcQ0Pr;
calcQ0Pr = (0.3201 * pform) + 450.91;
return calcQ0Pr;
}
/*=================================================================================================*/

/*Функция расчета приведенного значения температуры газа за турбиной (к 15 °С и 760 мм.рт.ст.), °C*/
float t0pr (float tform, float tv0) {
float calcT0Pr;
calcT0Pr = (tform + 273.15) * (288.15/(tv0 + 273.15)) - 273.15;
return calcT0Pr;
}
/*=================================================================================================*/

/*Функция расчета приведенного массового расхода топливного газа (к 15 °С и 760 мм.рт.ст.), кг/час*/
float g0pr (float q0pr) {
float calcG0Pr;
calcG0Pr = q0pr * 0.7;
return calcG0Pr;
}
/*=================================================================================================*/

/*Функция расчета углового коэффициента линейной функции*/
float coeffA (float gmax, float gmid, float gmin, float pmax, float pmid, float pmin) {
static float a1 = 0.0f, a2 = 0.0f, a3 = 0.0f, calcCoeffA = 0.0f;
a1 = (gmax - gmid)/(pmax - pmid);
a2 = (gmid - gmin)/(pmid - pmin);
a3 = (gmax - gmin)/(pmax - pmin);
calcCoeffA = (a1 + a2 + a3) / 3.0;
return calcCoeffA;
}
/*=================================================================================================*/

/*Функция расчета точки пересечения линейной функции с осью абсцисс*/
float coeffB (float gmax, float gmid, float gmin, float pmax, float pmid, float pmin, float coeffA) {
static float b1 = 0.0f, b2 = 0.0f, b3 = 0.0f, calcCoeffB = 0.0f;
b1 = gmax - (pmax * coeffA);
b2 = gmid - (pmid * coeffA);
b3 = gmin - (pmin * coeffA);
calcCoeffB = (b1 + b2 + b3) / 3.0;
return calcCoeffB;
}
/*=================================================================================================*/

/*Функция расчета коэффициентов квадратичной функции методом Крамера*/
float matrixKramer(float* massA, float* massB, int type) {
int i = 0, j = 0;
int N = 0;
long long sumx = 0, sumy = 0, sumx2 = 0, sumx3 = 0, sumx4 = 0, sumxy = 0, sumx2y = 0;
long determinant = 0;
long long detA = 0, detB = 0, detC = 0;
float calcCoeff = 0.0f;
	for (i=0;i<3;i++) {
		N += 1;
		sumx += massA[i];
		sumy += massB[i];
		sumx2 += pow(massA[i],2);
		sumx3 += pow(massA[i],3);
		sumx4 += pow(massA[i],4);
		sumxy += massA[i]*massB[i];
		sumx2y += pow(massA[i],2)*massB[i];
	}
static const long long deterM[3][3] = {{sumx2, sumx, N}, {sumx3, sumx2, sumx}, {sumx4, sumx3, sumx2}};
determinant = deterM[0][0]*deterM[1][1]*deterM[2][2] +
			  deterM[0][1]*deterM[1][2]*deterM[2][0] +
			  deterM[0][2]*deterM[1][0]*deterM[2][1] -
			  deterM[0][2]*deterM[1][1]*deterM[2][0] -
			  deterM[1][0]*deterM[0][1]*deterM[2][2] -
			  deterM[2][1]*deterM[1][2]*deterM[0][0];
switch(type) {
case 0: 	static const long long deterA[3][3] = {{sumy, sumx, N}, {sumxy, sumx2, sumx}, {sumx2y, sumx3, sumx2}};
			detA = deterA[0][0]*deterA[1][1]*deterA[2][2] +
			deterA[0][1]*deterA[1][2]*deterA[2][0] +
			deterA[0][2]*deterA[1][0]*deterA[2][1] -
			deterA[0][2]*deterA[1][1]*deterA[2][0] -
			deterA[1][0]*deterA[0][1]*deterA[2][2] -
			deterA[2][1]*deterA[1][2]*deterA[0][0];
			calcCoeff = float(detA)/float(determinant);
			break;
case 1:		static const long long deterB[3][3] = {{sumx2, sumy, N}, {sumx3, sumxy, sumx}, {sumx4, sumx2y, sumx2}};
			detB = deterB[0][0]*deterB[1][1]*deterB[2][2] +
			deterB[0][1]*deterB[1][2]*deterB[2][0] +
			deterB[0][2]*deterB[1][0]*deterB[2][1] -
			deterB[0][2]*deterB[1][1]*deterB[2][0] -
			deterB[1][0]*deterB[0][1]*deterB[2][2] -
			deterB[2][1]*deterB[1][2]*deterB[0][0];
			calcCoeff = float(detB)/float(determinant);
			break;
case 2:		static const long long deterC[3][3] = {{sumx2, sumx, sumy}, {sumx3, sumx2, sumxy}, {sumx4, sumx3, sumx2y}};
			detC = deterC[0][0]*deterC[1][1]*deterC[2][2] +
			deterC[0][1]*deterC[1][2]*deterC[2][0] +
			deterC[0][2]*deterC[1][0]*deterC[2][1] -
			deterC[0][2]*deterC[1][1]*deterC[2][0] -
			deterC[1][0]*deterC[0][1]*deterC[2][2] -
			deterC[2][1]*deterC[1][2]*deterC[0][0];
			calcCoeff = float(detC)/float(determinant);
			break;
			}
return calcCoeff;
}
/*=================================================================================================*/

/*Функция для формирования и передачи данных в квадратичную функцию с последующим расчетом и выводом определителей матрицы*/
void coeffSquadReg (float tmax, float tmid, float tmin, float pmax, float pmid, float pmin, float *coeffA, float *coeffB, float *coeffC) {
int typeA = 0, typeB = 1, typeC = 2;
float massA[3] = {tmin, tmid, tmax}, massB[3] = {pmin, pmid, pmax};
*coeffA = matrixKramer(massA, massB, typeA);
*coeffB = matrixKramer(massA, massB, typeB);
*coeffC = matrixKramer(massA, massB, typeC);
}
/*=================================================================================================*/

