#include "grapher.h"

#include <QPainter>

Grapher::Grapher(QWidget *parent)
	: QWidget(parent)
	, __range_fixed(false)
	, __draw_background(true)
	, __draw_grid(true)
	, __draw_label(true)
	, __fill_graph(true)
	, __max_data_count(50)
	, __background_color(32,32,32)
	, __grid_color(74,74,74)
	, __graph_color(65,186,21)
	, __label_color(125,125,125)
{
	setAttribute(Qt::WA_TranslucentBackground, true);
}

quint32 Grapher::count()
{
	return getData().size();
}

void Grapher::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);

	if (isDrawingBackground())
	{
		painter.fillRect(rect(), getBackgroundColor());
	}

	QPainterPath grid;
	for (int i = 1; i < 10; ++i)
	{
		grid.moveTo(0, (qreal)height() / 10 * i);
		grid.lineTo(width(), (qreal)height() / 10 * i);
		grid.moveTo((qreal)width() / 10 * i, 0);
		grid.lineTo((qreal)width() / 10 * i, height());
	}
	if (isDrawingLabel())
	{
		painter.setPen(QPen(getLabelColor(), 1));
		painter.setFont(QFont("Ubuntu", 72));
		painter.drawText(rect(), Qt::AlignCenter, tr("%1").arg(qRound(getValueToShow() * 100)));
	}

	painter.setBrush(Qt::NoBrush);

	if (isDrawingGrid())
	{
		painter.setPen(getGridColor());
		painter.drawPath(grid);
	}

	QColor fill_color;
	if (isFillingGraph())
	{
		fill_color = getGraphColor();
		fill_color.setAlpha(fill_color.alpha()* .5);
		painter.setBrush(fill_color);
	}

	QPainterPath path;
	path.moveTo(-1, height());
	for (int i = 0; i < getData().size(); ++i)
	{
		path.lineTo(map(i, getData().at(i)));
	}
	path.lineTo(width(), height());
	path.closeSubpath();

	painter.setPen(QPen(getGraphColor(), 1));
	painter.drawPath(path);
}

QPoint Grapher::map(quint32 index, qreal point)
{
	QSize s = size();
	QPoint result;
	point -= getVRange().first;

	if (getVRange().second - getVRange().first != 0)
		result.setY(s.height() - s.height() * point / (getVRange().second - getVRange().first));
	else
		result.setY(s.height() / 2);

	if (index != getData().size())
		result.setX(s.width() * (qreal)index / (qreal)(getData().size() - 1));
	else
		result.setX(s.width() / 2);

	return result;
}

int Grapher::getMaxDataCount() const { return __max_data_count; }
void Grapher::setMaxDataCount(int max_data_count) { __max_data_count = max_data_count; }

bool Grapher::isDrawingBackground() const { return __draw_background; }
bool Grapher::isDrawingLabel() const { return __draw_label; }
bool Grapher::isDrawingGrid() const { return __draw_grid; }
bool Grapher::isFillingGraph() const { return __fill_graph; }
QColor Grapher::getBackgroundColor() const { return __background_color; }
QColor Grapher::getGridColor() const { return __grid_color; }
QColor Grapher::getGraphColor() const { return __graph_color; }
QColor Grapher::getLabelColor() const { return __label_color; }

void Grapher::setDrawBackground(bool value) { __draw_background = value; emit drawBackgroundChanged();}
void Grapher::setDrawGrid(bool value) { __draw_grid = value; emit drawGridChanged();}
void Grapher::setDrawLabel(bool value) { __draw_label = value; emit drawLabelChanged();}
void Grapher::setFillGraph(bool value) { __fill_graph = value; emit fillGraphChanged();}
void Grapher::setBackgroundColor(QColor const &color) { __background_color = color; emit backgroundColorChanged();}
void Grapher::setGridColor(QColor const &color) { __grid_color = color;emit gridColorChanged();}
void Grapher::setGraphColor(QColor const &color) { __graph_color = color; emit graphColorChanged();}
void Grapher::setLabelColor(QColor const &color) { __label_color = color; emit labelColorChanged();}