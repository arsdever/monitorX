#pragma once

#include <QSystemTrayIcon>

class InterfaceRegistratorInterface;
class Grapher;
class Application;
class QEvent;

class ApplicationTrayIcon : public QSystemTrayIcon
{
	Q_OBJECT

  public:
	enum IconType
	{
		CPU,
		RAM,
		HDD,
		Invalid
	};

  public:
	ApplicationTrayIcon(Application *parent = nullptr);

	void registerDataSource(InterfaceRegistratorInterface *application) const;
	IconType currentType() const;

  public slots:
	void updateMenu();
	void updateInfo(qreal);
	void changeIconType(IconType);

  private:
	void initContextMenu();
	IconType __type;
	Application *__app;
	Grapher *__renderer;
	Grapher **__renderers;
};
