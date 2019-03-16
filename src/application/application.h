#pragma once

#include <QMainWindow>

class Collector;
class DeviceInformator;

class Application : public QMainWindow
{

	Q_OBJECT

public:
	Application(QWidget *parent = nullptr);
	void init();

public slots:
	void updateGraphs();

private:
	DeviceInformator *__informator;
	Collector *__data_collector;
};
