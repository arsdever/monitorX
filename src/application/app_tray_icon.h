#pragma once

#include <QSystemTrayIcon>

class InterfaceRegistratorInterface;
class Grapher;
class Application;

class ApplicationTrayIcon: public QSystemTrayIcon
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
	void updateInfo(qreal);
	void changeIconType(IconType);
	virtual void activation(QSystemTrayIcon::ActivationReason reason);

private:
	void initContextMenu();
	IconType __type;
	Application *__app;
	Grapher *__renderer;
	Grapher **__renderers;
};

