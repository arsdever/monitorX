#include "app_tray_icon.h"
#include "../grapher/grapher_types.h"
#include "application.h"

#include <QApplication>
#include <QLibrary>
#include <QMenu>
#include <QPainter>
#include <QMouseEvent>

ApplicationTrayIcon::ApplicationTrayIcon(Application *parent)
	: QSystemTrayIcon(parent), __type(CPU), __renderer(nullptr), __renderers(new Grapher *[3])
{
	__renderers[0] = new CPULoadGrapher;
	__renderers[1] = new RAMUsageGrapher;
	__renderers[2] = new HDDUsageGrapher;

	for (uint8_t i = 0; i < 3; ++i)
	{
		__renderers[i]->resize(128, 128);
		__renderers[i]->setMaxDataCount(10);
		__renderers[i]->setBackgroundColor(QColor(5, 42, 15));
		__renderers[i]->setGraphColor(QColor(5, 255, 96));
	}

	__renderer = __renderers[0];
	setIcon(QIcon(":/Resource/icon.png"));
	initContextMenu();

	connect(__renderer, SIGNAL(dataChanged(qreal)), this, SLOT(updateInfo(qreal)));
}

void ApplicationTrayIcon::initContextMenu()
{
	setContextMenu(new QMenu);
	typedef QMenu *(*installer)(QMenu *);
	installer install_comport_connection = (installer)QLibrary::resolve("provider", "add_menu");

	if (install_comport_connection)
		install_comport_connection(contextMenu());

	connect(contextMenu(), SIGNAL(aboutToShow()), this, SLOT(updateMenu()));

	QMenu *icon_type_menu = contextMenu()->addMenu("Change icon info...");
	icon_type_menu->addAction(QIcon(":/Resources/cpu.png"), "CPU usage", [this]() { changeIconType(CPU); });
	icon_type_menu->addAction(QIcon(":/Resources/ram.png"), "RAM usage", [this]() { changeIconType(RAM); });
	icon_type_menu->addAction(QIcon(":/Resources/hdd.png"), "HDD usage", [this]() { changeIconType(HDD); });
	contextMenu()->addSeparator();
	contextMenu()->addAction(QIcon(":/Resources/exit.png"), "Quit", qApp, SLOT(quit()));
}

void ApplicationTrayIcon::changeIconType(IconType type)
{
	if (type == currentType())
		return;

	__type = type;
	switch (type)
	{
	case CPU:
		__renderer = __renderers[0];
		break;
	case RAM:
		__renderer = __renderers[1];
		break;
	case HDD:
		__renderer = __renderers[2];
		break;
	default:
		__renderer = nullptr;
		__type = Invalid;
	}
}

void ApplicationTrayIcon::updateMenu()
{
	QList<QMenu *> menus = contextMenu()->findChildren<QMenu *>();

	// find menu by name "Connect to"
	QMenu *connect_menu = nullptr;
	for (QMenu *menu : menus)
	{
		if (menu != nullptr && menu->title() == "Connect to")
		{
			connect_menu = menu;
			break;
		}
	}

	if (connect_menu == nullptr)
		return;

	// update available comport list
	typedef void (*updater)(QMenu *, Application *);
	updater comport_list_update = (updater)QLibrary::resolve("provider", "update_menu");

	if (comport_list_update)
		comport_list_update(connect_menu, qobject_cast<Application *>(parent()));
}

ApplicationTrayIcon::IconType ApplicationTrayIcon::currentType() const { return __type; }

void ApplicationTrayIcon::registerDataSource(InterfaceRegistratorInterface *application) const
{
	application->registerInterface(__renderers[0]->GetInterface_InfoConsumerInterface());
	application->registerInterface(__renderers[1]->GetInterface_InfoConsumerInterface());
	application->registerInterface(__renderers[2]->GetInterface_InfoConsumerInterface());
}

void ApplicationTrayIcon::updateInfo(qreal usage)
{
	setToolTip(QString("CPU usage: %1%").arg(usage));
	if (!__renderer || __type == Invalid)
	{
		setIcon(QIcon(":/Resources/icon.png"));
		return;
	}

	QPixmap pixmap(128, 128);
	QPainter icon_drawer(&pixmap);
	icon_drawer.setRenderHint(QPainter::Antialiasing);
	__renderer->render(&icon_drawer);
	setIcon(pixmap);
}
