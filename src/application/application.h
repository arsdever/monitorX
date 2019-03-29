#pragma once

#include <QMainWindow>

class Collector;
class DeviceInformator;
class Comport;

class Application : public QMainWindow
{

	Q_OBJECT

public:
	Application(QWidget *parent = nullptr);
	void init();

public slots:
	void updateGraphs();
	void connectToPort();

private:
	DeviceInformator *__informator;
	Collector *__data_collector;
	Comport *__comport;
};
