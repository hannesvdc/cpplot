/*
 * cpp_plot.cpp
 *
 *  Created on: 3 May 2020
 *      Author: Hannes Vandecasteele
 */

#ifndef _CPP_PLOT_
#define _CPP_PLOT_

#include <memory>

#include <QtWidgets/QApplication>
#include <QtGui/QPainter>

#include "cpp.h"

namespace cpplot {

// Reference to the active application;
std::shared_ptr<QApplication> app = std::make_shared<QApplication>();

// Reference to the active Window.
std::shared_ptr<Window> activeWindow = std::make_shared<Window>();


class PlotObject {
public:
	PlotObject(double xmin,
			   double xmax,
			   double ymin,
			   double ymax,
			   std::string xscale,
			   std::string yscale) :
			   xmin(xmin),
			   xmax(xmax),
			   ymin(ymin),
			   ymax(ymax),
			   xscale(xscale),
			   yscale(yscale) {}

	virtual ~PlotObject() {}

	virtual void draw(QPainter &painter,
			          int xmin,
					  int xmax,
					  int ymin,
					  int ymax,
					  int x_left,
					  int x_right,
					  int y_down,
					  int y_up) {}

public:
	double xmin, ymin, xmax, ymax;
	std::string xscale, yscale;

};


class Plot2D : public PlotObject {
public:
	Plot2D(std::vector<double>& x,
		   std::vector<double>& y,
		   std::string xscale,
		   std::string yscale) :
		   PlotObject(std::min(x.begin(), x.end()), std::max(x.begin(), x.end()), std::min(y.begin(), y.end()), std::max(y.begin(), y.end()), xscale, yscale),
		   x(x),
		   y(y) {}

	virtual void draw(QPainter &painter,
					 int xmin,
				     int xmax,
					 int ymin,
					 int ymax,
					 int x_left,
					 int x_right,
					 int y_down,
					 int y_up) {
		int x_denom = x_right - x_left;
		int y_denom = y_down - y_up;

		int xprev = int(std::floor((this->x[0] - xmin)/x_denom));
		int yprev = int(std::floor((this->y[0] - y_up)/y_denom));
		for(int i= 1; i < this->x.size(); ++i) {
			int x = int(std::floor((this->x[i] - xmin)/x_denom));
			int y = int(std::floor((this->y[i] - y_up)/y_denom));
			painter.drawLine(xprev, yprev, x, y);

			xprev = x;
			yprev = y;
		}
	}

private:
	std::vector<double> &x, &y;
};


void plot(std::vector<double>& x, std::vector<double>& y) {
	std::shared_ptr<Plot2D> plot = std::make_shared<Plot2D>(x, y, "linear", "linear");
	activeWindow->registerPlot(plot);
}

int show() {
	activeWindow->show();

	return app->exec();
}

} // namespace cpplot

#endif

