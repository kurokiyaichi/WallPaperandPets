#pragma once
#include <QPixmap>
#include <QWidget>
#include <qlabel.h>
#include <QPushButton>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include "desktopwidget.h"


class Widget  : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent =nullptr);
	~Widget();
	void updateRoleAnimation();//更新角色差分动画（透明窗口）
	//实现拖动，重写并安装事件过滤器
	bool eventFilter(QObject* watched,QEvent* event) override;
	void initBtn();//初始化按钮函数
	bool istop = false;
	void onPushButton_clicked();//解决切换置顶时必须show所导致的闪烁bug

private:
	QLabel* rolelabel;
	qint8 currFrame;//当前帧
	QPushButton* exitBtn;//关闭按钮
	QPushButton* switchBtn;//切换按钮
	QPushButton* openBtn;//打开文件按钮
	QMediaPlaylist* playlist;
	QMediaPlayer* player;
	int musicMount = 0;
	int musicValue = 0;//初始化歌曲的编号和序列值
	void nextMusic();

	DesktopWidget* desktopWidget;


};
