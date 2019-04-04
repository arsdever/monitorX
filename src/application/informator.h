#pragma once

#include "../client/data.h"

#include <QLabel>

class Informator : public QLabel
{
public:
	Informator(INFO const *source, QWidget *parent);
	Informator(QWidget *parent = nullptr);

	void setDataSource(INFO const *source);

public slots:
	void dataChanged();

private:
	INFO const *__source;
};
