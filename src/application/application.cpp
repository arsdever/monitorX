#include "application.h"

#include "../collector/collector.h"
#include "../collector/device_informator.h"
#include "../client/data.h"
#include "../extras/comport.h"

#include "cpu_grapher.h"

#include <QLibrary>
#include <QMenuBar>

Application::Application(QWidget *parent)
	: QMainWindow(parent), __informator(new DeviceInformator), __data_collector(new Collector(__informator->GetInterface_DataRequestInterface(), 500)), __comport(nullptr)
{
	init();
	connect(__data_collector, SIGNAL(dataChanged()), this, SLOT(updateGraphs()));
}

void Application::init()
{
	setCentralWidget(new CPULoadGrapher);
	setMenuBar(new QMenuBar);
	typedef void (*Installer)(QMenuBar *);
	Installer install_comport_connection = (Installer)QLibrary::resolve("extras", "addMenu");

	if (install_comport_connection)
		install_comport_connection(menuBar());

	initMenuBar();
}

void Application::initMenuBar()
{
#if defined(_WIN32) || defined(_WIN64)
	QList<QMenu *> menus = menuBar()->findChildren<QMenu *>();
	for (QMenu *menu : menus)
	{
		if (menu->title() == "Connect")
		{
			for (QAction *action : menu->actions())
			{
				connect(action, SIGNAL(triggered()), this, SLOT(connectToPort()));
			}
		}
	}
#elif defined(__linux__)
	QList<QAction *> actions = menuBar()->findChildren<QAction *>();
	for (QAction *action : actions)
	{
		if (action->text() == "Connect")
		{
			connect(action, SIGNAL(triggered()), this, SLOT(connectToPort()));
		}
	}
#endif
}

void Application::updateGraphs()
{
	CPULoadGrapher *graphWidget = dynamic_cast<CPULoadGrapher *>(centralWidget());
	INFO lastInfo = *(INFO *)__data_collector->getLast();

	if (graphWidget != nullptr)
	{
		*graphWidget << (lastInfo.cpuUsageUser + lastInfo.cpuUsageKernel) / 100.0 << (lastInfo.cpuUsageKernel / 100.0);
	}

	if (__comport == nullptr || !__comport->is_opened())
		return;

	__comport->write_data((const char *)&lastInfo, sizeof(INFO));
}

void Application::connectToPort()
{
#if defined(_WIN32) || defined(_WIN64)
	QAction *action = qobject_cast<QAction *>(sender());
	if (action == nullptr)
		return;

	typedef Comport *(*ComConnector)(QString const &);
	ComConnector connectToPort = (ComConnector)QLibrary::resolve("extras", "openPort");
	if (connectToPort == nullptr)
		return;

	delete __comport;
	__comport = connectToPort(action->text());
	if (!__comport->is_opened())
		__comport->open_port();
#elif defined(__linux__)
	QString portname = requirePortName();

	typedef Comport *(*ComConnector)(QString const &);
	ComConnector connectToPort = (ComConnector)QLibrary::resolve("extras", "openPort");
	if (connectToPort == nullptr)
		return;

	delete __comport;
	__comport = connectToPort(portname);
	if (!__comport->is_opened())
		__comport->open_port();
#endif
}

#include <QDialog>
#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>

QString Application::requirePortName()
{
	QDialog widget;
	QGridLayout layout;
	widget.setLayout(&layout);
	QPushButton accepter("OK");
	QPushButton canceler("Cancel");
	layout.addWidget(&accepter, 1, 0);
	layout.addWidget(&canceler, 1, 1);
	QLineEdit editor;
	layout.addWidget(&editor, 0, 0, 1, 0);

	QObject::connect(&accepter, SIGNAL(clicked()), &widget, SLOT(close()));
	QObject::connect(&canceler, SIGNAL(clicked()), &widget, SLOT(close()));
	QObject::connect(&canceler, SIGNAL(clicked()), &editor, SLOT(clear()));

	widget.exec();
	return editor.text();
}