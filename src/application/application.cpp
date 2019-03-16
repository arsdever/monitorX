#include "application.h"

#include "../collector/collector.h"
#include "../collector/device_informator.h"
#include "../shared/data.h"

#include "cpu_grapher.h"

#include <QLibrary>
#include <QMenuBar>

Application::Application(QWidget *parent)
	: QMainWindow(parent)
	, __informator(new DeviceInformator)
	, __data_collector(new Collector(__informator->GetInterface_DataRequestInterface(), 500))
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
	if(install_comport_connection)
		install_comport_connection(menuBar());
}

void Application::updateGraphs()
{
	CPULoadGrapher *graphWidget = dynamic_cast<CPULoadGrapher*>(centralWidget());
	Q_ASSERT(graphWidget != nullptr);
	INFO lastInfo = *(INFO*)__data_collector->getLast();
	*graphWidget << (lastInfo.cpuUsageUser + lastInfo.cpuUsageKernel) / 100.0 << (lastInfo.cpuUsageKernel / 100.0);
}
