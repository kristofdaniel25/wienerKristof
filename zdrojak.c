#include "pbPlots.h"
#include "supportlib.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

double* fill(int size, double sigma) //plnenie pola oborov hodnot pomocou Boxovej–Mullerovej transformacie
{
	double two_pi = 2.0 * M_PI;
	int i;
	double* yc = (double*)malloc(size * sizeof(double)); 	
	double mag, z0, z1;
	double u1;
	double u2;
	yc[0] = 0.;
	for (i = 1; i < size; i++)
	{
		u1 = (double)rand() / (double)(RAND_MAX);
		u2 = (double)rand() / (double)(RAND_MAX);
		mag = sigma*sqrt(-2.0 * log(u1));
		z0 = mag * cos(two_pi * u2);
		z1 = mag * sin(two_pi * u2);
		yc[i] = yc[i - 1] + z0;
		u2 = z1;
	}
	return yc;
}

int main() {
	
	_Bool success;
	StringReference* errorMessage;
	int i;

	double interval = 4.;
	int lng = 100; //pocet drah
	int size = 4; //pocet bodov na intervale [0,interval]
	
	srand(time(NULL)); 

	double* xc = (double*)malloc(size * sizeof(double));
	double** ys;
	ys = (double**)malloc(lng * sizeof(double*));
	for (i = 0; i < size; i++)
	{
		xc[i] = i * interval / (size * 1.);
	}
	int count = 0;
	double sig = interval/ (size * 1.),v2,v3,v4;
	ScatterPlotSeries** series = (ScatterPlotSeries*)malloc(lng * sizeof(ScatterPlotSeries*));
	for (i = 0; i < lng; i++)
	{
		ys[i] = (double*)malloc(size * sizeof(double));
		ys[i] = fill(size, sig);
		series[i] = GetDefaultScatterPlotSeriesSettings();
		series[i]->xs = xc;
		series[i]->xsLength = size;
		series[i]->ys = ys[i];
		series[i]->ysLength = size;
		series[i]->linearInterpolation = true;

		if ((ys[i][1] >= -1) && (ys[i][1] <= 1) && (ys[i][3] >= ys[i][1]) )
		{
			count++;
			series[i]->color = CreateRGBColor(1., 0., 0.);
		}
		else
			series[i]->color = CreateRGBColor(0.8, 0.8, 0.8);
		//series[i]->pointType = L"dots";
		//series[i]->pointTypeLength = wcslen(series[i]->pointType);

	}
	printf("P = %f", (1. * count) / (1. * lng));
	ScatterPlotSettings* settings = GetDefaultScatterPlotSettings();

	settings->width = 600;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	
	ScatterPlotSeries* s = series;
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = lng;
	
	RGBABitmapImageReference* canvasReference = CreateRGBABitmapImageReference();
	errorMessage = (StringReference*)malloc(sizeof(StringReference));
	success = DrawScatterPlotFromSettings(canvasReference, settings, errorMessage);
	
	if (success) {
		
		size_t length;
		double* pngdata = ConvertToPNG(&length, canvasReference->image);
		WriteToFile(pngdata, length, "plotVis.png");
		DeleteImage(canvasReference->image);
	}
	else {
		fprintf(stderr, "Error: ");
		for (int i = 0; i < errorMessage->stringLength; i++) {
			fprintf(stderr, "%c", errorMessage->string[i]);
		}
		fprintf(stderr, "\n");
	}
	free(ys);
	free(xc);
	return success ? 0 : 1;
}