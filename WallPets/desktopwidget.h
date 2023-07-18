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
	void setAllWallpaper();//����Ϊ��Ŀ¼����
	void setPixmap(const QString &fileName);//�л���ֽ

private:
	QLabel* bklabel;//��ű�ֽ
	QPixmap bkPixmap;


};
