#include "desktopwidget.h"
#include <QWidget>
#include <QHboxLayout>
#include <qt_windows.h>
#include <QDebug>
#include "widget.h"


DesktopWidget::DesktopWidget(QWidget *parent)
	: QWidget(parent),bklabel (new QLabel)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->addWidget(bklabel);

	setPixmap(":/resource/wallpaper/1.jpg");

	setAllWallpaper();
}

DesktopWidget::~DesktopWidget()
{}

void DesktopWidget::setAllWallpaper()
{
	//找到桌面的句柄（标识）
	HWND desktopHwd = FindWindow(L"Program",L"Program Manager");
	if (!desktopHwd)
	{
		qDebug() << "查找失败！";
		return;
	}
	//将该窗口设置给找到的窗口
	SetParent((HWND)this->winId(), desktopHwd);
}

void DesktopWidget::setPixmap(const QString & fileName)
{
	if (QPixmap(fileName).isNull())
	{
		return;
	}
	bkPixmap.load(fileName);
	bklabel->setPixmap(bkPixmap);
	this->hide();
	this->showFullScreen();//全屏显示壁纸

	
}

