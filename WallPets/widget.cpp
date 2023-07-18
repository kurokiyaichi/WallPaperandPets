#include "widget.h"
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QWindow>
#include <QListWidgetItem>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QVboxLayout>

Widget::Widget(QWidget* parent)
	: QWidget(parent), rolelabel(new QLabel(this)),currFrame(0),desktopWidget(new DesktopWidget)
{
	//ȥ�����ڱ߿�
	setWindowFlags(Qt::WindowType::FramelessWindowHint);

	//͸������
	setAttribute(Qt::WA_TranslucentBackground);

	//��ʱ�����¶���
	QTimer* updateTimer = new QTimer(this);
	connect(updateTimer,&QTimer::timeout,this, &Widget::updateRoleAnimation);//�����źźͲ����ӣ�1.�汾��û��callOnTimeout 2.���ȶ�
	updateTimer->start(1500);

	//���ô�����Ӱ
	QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
	shadow->setColor(QColor(230, 230, 230));
	shadow->setBlurRadius(10);
	this->setGraphicsEffect(shadow);

	//��װ�¼�������
	this->installEventFilter(this);

	rolelabel->resize(900,900);
	initBtn();
	desktopWidget->show();
}

Widget::~Widget()
{}

void Widget::onPushButton_clicked()
{
	if (!istop)
	{
		        Qt::WindowFlags m_flags = windowFlags();
		        setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
		        show();
		qDebug() << "always top";

		QWindow* pWin = this->windowHandle();
		pWin->setFlags(Qt::Widget | Qt::WindowStaysOnTopHint);
	}
	else {
		Qt::WindowFlags m_flags = windowFlags();
        setWindowFlags(Qt::Widget);
        setWindowFlags(NULL);
        show();

		QWindow* pWin = this->windowHandle();
		pWin->setFlags(Qt::Widget);
		qDebug() << "no always top";
	}
	istop = !istop;
}


void Widget::updateRoleAnimation()
{
	QString qss("background-repeat:no-repeat;");
	rolelabel->setStyleSheet(qss+QString("background-image:url(:/resource/chise/%1.png);").arg(currFrame));
	currFrame = (currFrame + 1) % 5;//5��ͼ�ڲ��ϱ�֡
}

bool Widget::eventFilter(QObject* watched, QEvent* event)
{
	QMouseEvent* mouse = static_cast<QMouseEvent*> (event);//����ǿת����
	//�ж�����������¼�
	static QPoint begpos;//�����øõ��ƶ��������ƶ��Ľ�����ʽΪ�������Ͻǿ�ʼ�ƶ�
	if (event->type() == QEvent::MouseButtonPress && mouse->buttons() & Qt::MouseButton::LeftButton)
	{
		//������굱ǰ��������λ���봰��ԭ�㣨���Ͻǣ�����������
		begpos = mouse->globalPos() - this->pos();
	}
	//�ж�����ƶ��¼�
	else if (event->type() == QEvent::MouseMove && mouse->buttons() & Qt::MouseButton::LeftButton)
	{
		this->move(mouse->globalPos() - begpos);
	}


	return false;
}

void Widget::initBtn()
{
	exitBtn = new QPushButton(this);
	switchBtn = new QPushButton(this);
	openBtn = new QPushButton(this);

	exitBtn->setGeometry(300, 200, 50, 50);
	switchBtn->setGeometry(300, 260, 50, 50);
	openBtn->setGeometry(300, 320, 50, 50);



	exitBtn->setStyleSheet("border-image:url(:/resource/common/exit.png);");
	switchBtn->setStyleSheet("border-image:url(:/resource/common/switch.png);");
	openBtn->setStyleSheet("border-image:url(:/resource/common/open.png);");
	setStyleSheet("QPushButton{background-color:rgb(64,173,250);");
	setStyleSheet("QPushButton{border:none;border-radius:20px;}QPushButton:hover{background-color:rgb(233,31,48);}");

	//�������ý��������ʼ���ö�����ֹ�л���ֽʱ���ֵı�ֽ�����ں��ڸǳ��ﵼ���޷���ʾ��bug
	setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

	connect(exitBtn, &QPushButton::pressed, this, [=]() 
		{
			Widget::close();
			desktopWidget->close();
		});
	connect(switchBtn, &QPushButton::pressed, this, [=]() 
		{
			QString filename = QFileDialog::getOpenFileName(nullptr, "ѡ���ֽ", "./", "Image(*.jpg *.png)");
			if (filename.isEmpty())
			{
				return;
			}
			desktopWidget->setPixmap(filename);
			connect(exitBtn, &QPushButton::pressed, this, [=]()
				{
					desktopWidget->clearFocus();
					desktopWidget->close();
				});
		});//�л�ͼ�����ڸ�����ֽ
	connect(openBtn, &QPushButton::pressed, this, [=]()
		{
			playlist = new QMediaPlaylist(this);
			QString medianame = QFileDialog::getOpenFileName(nullptr, "ѡ���ļ�", "./", "");
			player = new QMediaPlayer;
			player->setMedia(QUrl::fromLocalFile(medianame));
			player->setVolume(40);
			if (player->state() == QMediaPlayer::PlayingState)
			{
				player->pause();
			}
			else
			{
				player->play();
			}

			//����Ǵﵽ���ʱ��Ӧ�ü�������ٴδ�0��ʼ

			connect(exitBtn, &QPushButton::pressed, this, [=]() 
				{
					if (player != nullptr)
					{
						delete player;
						player = nullptr;
					}
				});

		});
}

//void Widget::nextMusic()
//{
//	QListWidget* listWidget = new QListWidget(this);
//	//���жϵ�ǰλ���Ƿ�Ϊβ�ˣ����ǣ���musicValue��Ϊ0
//	if (musicValue++ == listWidget->count())
//	{
//		musicValue = 0;
//	}
//	else
//	{
//		musicValue++;
//	}
//	QListWidgetItem* item = listWidget->item(musicValue);
//	item->setSelected(true);
//	player->stop();
//	player->setMedia(QUrl::fromLocalFile("/" + item->text()));
//	player->play();
//}