#include "application.h"
#include "app_tray_icon.h"
#include "../grapher/grapher_types/cpu_grapher.h"

#include <QDesktopWidget>
#include <QApplication>
#include "line_grapher.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Application app;
	ApplicationTrayIcon tray_icon(&app);

	tray_icon.registerDataSource(app.GetInterface_InterfaceRegistratorInterface());
	tray_icon.show();

	LineGrapher grapher;
	grapher.setFixedSize(QApplication::desktop()->screenGeometry().width(), 2);
	grapher.move(0, 0);
	app.GetInterface_InterfaceRegistratorInterface()->registerInterface(grapher.GetInterface_InfoConsumerInterface());
	grapher.setWindowFlags(grapher.windowFlags() | Qt::WindowTransparentForInput | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	grapher.setAttribute(Qt::WA_TranslucentBackground);
	grapher.setAttribute(Qt::WA_AlwaysStackOnTop);
	grapher.setDrawBackground(false);
	grapher.setDrawGrid(false);
	grapher.setDrawLabel(false);
	grapher.setGraphColor(QColor(48, 255, 0));
	grapher.setMaxDataCount(200);
	grapher.show();

	return a.exec();
}