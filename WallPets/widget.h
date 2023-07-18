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
	void updateRoleAnimation();//���½�ɫ��ֶ�����͸�����ڣ�
	//ʵ���϶�����д����װ�¼�������
	bool eventFilter(QObject* watched,QEvent* event) override;
	void initBtn();//��ʼ����ť����
	bool istop = false;
	void onPushButton_clicked();//����л��ö�ʱ����show�����µ���˸bug

private:
	QLabel* rolelabel;
	qint8 currFrame;//��ǰ֡
	QPushButton* exitBtn;//�رհ�ť
	QPushButton* switchBtn;//�л���ť
	QPushButton* openBtn;//���ļ���ť
	QMediaPlaylist* playlist;
	QMediaPlayer* player;
	int musicMount = 0;
	int musicValue = 0;//��ʼ�������ı�ź�����ֵ
	void nextMusic();

	DesktopWidget* desktopWidget;


};
