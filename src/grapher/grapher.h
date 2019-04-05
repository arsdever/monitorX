#pragma once

#include "grapher_global.h"
#include "../shared/macros.h"
#include "../shared/interfaces.h"

#include <QWidget>
#include <QVector>
#include <QPair>

class QPaintEvent;

class GRAPHER_EXPORT Grapher : public QWidget
{

	Q_OBJECT

		Q_PROPERTY(bool draw_background READ isDrawingBackground WRITE setDrawBackground NOTIFY drawBackgroundChanged)
		Q_PROPERTY(bool draw_grid READ isDrawingGrid WRITE setDrawGrid NOTIFY drawGridChanged)
		Q_PROPERTY(bool draw_label READ isDrawingLabel WRITE setDrawLabel NOTIFY drawLabelChanged)
		Q_PROPERTY(bool fill_graph READ isFillingGraph WRITE setFillGraph NOTIFY fillGraphChanged)
		Q_PROPERTY(QColor background_color READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
		Q_PROPERTY(QColor grid_color READ getGridColor WRITE setBackgroundColor NOTIFY gridColorChanged)
		Q_PROPERTY(QColor graph_color READ getGraphColor WRITE setGraphColor NOTIFY graphColorChanged)
		Q_PROPERTY(QColor label_color READ getLabelColor WRITE setLabelColor NOTIFY labelColorChanged)

public:
	Grapher(QWidget *parent = nullptr);

	quint32 count();

	virtual InfoConsumerInterface *GetInterface_InfoConsumerInterface() = 0;
	virtual QVector<qreal> const& getData() const = 0;
	virtual qreal getValueToShow() const = 0;
	virtual QPair<qreal, qreal> const& getVRange() const = 0;
	virtual void setFixedRange(qreal min, qreal max) = 0;
	virtual int getMaxDataCount() const;
	virtual void setMaxDataCount(int max_data_count);

	bool isDrawingBackground() const;
	bool isDrawingGrid() const;
	bool isDrawingLabel() const;
	bool isFillingGraph() const;
	QColor getBackgroundColor() const;
	QColor getGridColor() const;
	QColor getGraphColor() const;
	QColor getLabelColor() const;

	void setDrawBackground(bool value = true);
	void setDrawGrid(bool value = true);
	void setDrawLabel(bool value = true);
	void setFillGraph(bool value = true);
	void setBackgroundColor(QColor const &color);
	void setGridColor(QColor const &color);
	void setGraphColor(QColor const &color);
	void setLabelColor(QColor const &color);

signals:
	void drawBackgroundChanged();
	void drawGridChanged();
	void drawLabelChanged();
	void backgroundColorChanged();
	void gridColorChanged();
	void graphColorChanged();
	void labelColorChanged();
	void fillGraphChanged();

protected:
	void paintEvent(QPaintEvent *event) override;

	QPoint map(quint32 index, qreal point);

private:
	bool __range_fixed;
	bool __draw_background;
	bool __draw_grid;
	bool __draw_label;
	bool __fill_graph;
	int __max_data_count;
	QColor __background_color;
	QColor __grid_color;
	QColor __graph_color;
	QColor __label_color;
};
