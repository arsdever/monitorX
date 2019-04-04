#include "provider_global.h"

#include <QMenu>
#include <QApplication>
#include "../comport/comports.h"
#include "../application/application.h"

extern "C" PROVIDER_EXPORT void update_menu(QMenu *menu, Application *app)
{
	if (menu == nullptr || menu->title() != "Connect to")
		return;

	menu->clear();

	COMPORT_CLASS::updateComMap();
	if (COMPORT_CLASS::s_com_map.empty())
	{
		menu->addAction("No devices available")->setEnabled(false);
	}
	else
	{
		QList<QString> keys = COMPORT_CLASS::s_com_map.keys();
		for (QString const &port : keys)
		{
			menu->addAction(port, [app, port]() {
				Comport *comport = new COMPORT_CLASS(port.toStdString(), 57600);
				app->setComport(comport);
				comport->open_port();
			});
		}
	}
}

extern "C" PROVIDER_EXPORT QMenu *add_menu(QMenu *menu)
{
	if (menu == nullptr)
		return nullptr;

	return menu->addMenu(QIcon(":/Resources/connect.png"), "Connect to");
}
