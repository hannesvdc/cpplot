/*
 * cpp.cpp
 *
 *  Created on: 1 May 2020
 *      Author: Hannes Vandecasteele
 */

#ifndef _CPP_
#define _CPP_

#include <algorithm>
#include <memory>
#include <vector>
#include <string>

#include <QtWidgets/QWidget>
#include <QtCore/QRect>
#include <QtGui/QPainter>

#include "cpp_plot.h"

namespace cpplot {

class Window : public QWidget {
public:
	Window(void) : pWidth(0.9),
				   pHeight(0.9),
				   xmin(0),
				   xmax(0),
				   ymin(0),
				   ymax(0),
				   xscale(""),
				   yscale(""),
				   plotObjects(std::vector<PlotObject>()) {
		this->setGeometry(QRect(500, 500, 1000, 1000));

	}

	void registerPlot(std::shared_ptr<PlotObject> object) {
		this->plotObjects.push_back(object);

		this->xmin = std::min<int>(this->xmin, object->xmin);
		this->xmax = std::max<int>(this->xmax, object->xmax);
		this->ymin = std::min<int>(this->ymin, object->ymin);
		this->ymax = std::min<int>(this->ymax, object->ymax);

		if ( this->xscale == "" ) {
			this->xscale = object->xscale;
			this->yscale = object->yscale;
		}

	}
	void paintEvent(QPaintEvent *event) override {
		QPainter painter(this);
		painter.setPen(Qt::white);

		int x_left = 0.5*(1. - this->pWidth)*this->width();
		int y_up = 0.5*(1. - this->pHeight)*this->height();
		int x_width = this->pWidth*this->width();
		int y_height = this->pHeight*this->height();
		painter.drawRect(x_left, y_up, x_width, y_height);

		// Draw axis and so on


		// give the painter to each of the plot objects.
		for( std::shared_ptr<PlotObject> plotObject : this->plotObjects) {
			plotObject->draw(painter, this->xmin, this->xmax, this->ymin, this->ymax, x_left, x_left+x_width, y_up+y_height, y_up);
		}
	}

private:
	double pWidth, pHeight;
	int xmin, xmax, ymin, ymax;
	std::string xscale, yscale;

	std::vector<std::shared_ptr<PlotObject> > plotObjects;
};

} // namespace cpplot

#endif
