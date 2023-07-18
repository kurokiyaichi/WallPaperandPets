#pragma once
#include <QLabel>
#include <QWidget>
#include <QPixmap>

class DesktopWidget  : public QWidget
{
	Q_OBJECT

public:
	DesktopWidget(QWidget *parent = nullptr);
	~DesktopWidget();
	void setAllWallpaper();//·ÅÖÃÎª¸¸Ä¿Â¼´°¿Ú
	void setPixmap(const QString &fileName);//ÇÐ»»±ÚÖ½

private:
	QLabel* bklabel;//´æ·Å±ÚÖ½
	QPixmap bkPixmap;


};
