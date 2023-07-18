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
	//�ҵ�����ľ������ʶ��
	HWND desktopHwd = FindWindow(L"Program",L"Program Manager");
	if (!desktopHwd)
	{
		qDebug() << "����ʧ�ܣ�";
		return;
	}
	//���ô������ø��ҵ��Ĵ���
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
	this->showFullScreen();//ȫ����ʾ��ֽ

	
}

