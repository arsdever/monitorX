#include "application.h"

#include "../collector/collector.h"
#include "../collector/device_informator.h"
#include "../client/data.h"
#include "../extras/comport.h"

#include "cpu_grapher.h"

#include <QLibrary>
#include <QMenuBar>

Application::Application(QWidget *parent)
	: QMainWindow(parent)
	, __informator(new DeviceInformator)
	, __data_collector(new Collector(__informator->GetInterface_DataRequestInterface(), 500))
	, __comport(nullptr)
{
	init();
	connect(__data_collector, SIGNAL(dataChanged()), this, SLOT(updateGraphs()));
}

void Application::init()
{
	setCentralWidget(new CPULoadGrapher);
	setMenuBar(new QMenuBar);
	typedef void(*Installer)(QMenuBar*);
	Installer install_comport_connection = (Installer)QLibrary::resolve("extras", "addMenu");
	if (install_comport_connection)
	{
		install_comport_connection(menuBar());
		QList<QMenu*> menus = menuBar()->findChildren<QMenu*>();
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
	}
}

void Application::updateGraphs()
{
	CPULoadGrapher *graphWidget = dynamic_cast<CPULoadGrapher*>(centralWidget());
	INFO lastInfo = *(INFO*)__data_collector->getLast();

	if (graphWidget != nullptr)
	{
		*graphWidget << (lastInfo.cpuUsageUser + lastInfo.cpuUsageKernel) / 100.0 << (lastInfo.cpuUsageKernel / 100.0);
	}
		
	if (__comport == nullptr || !__comport->is_opened())
		return;

	__comport->write_data((const char*)&lastInfo, sizeof(INFO));
}

void Application::connectToPort()
{
	QAction *action = qobject_cast<QAction*>(sender());
	if (action == nullptr)
		return;

	typedef Comport*(*ComConnector)(QString const&);
	ComConnector connectToPort = (ComConnector)QLibrary::resolve("extras", "openPort");
	if (connectToPort == nullptr)
		return;

	delete __comport;
	__comport = connectToPort(action->text());
	if (!__comport->is_opened())
		__comport->open_port();
}
