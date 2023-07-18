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
	//去除窗口边框
	setWindowFlags(Qt::WindowType::FramelessWindowHint);

	//透明背景
	setAttribute(Qt::WA_TranslucentBackground);

	//定时器更新动画
	QTimer* updateTimer = new QTimer(this);
	connect(updateTimer,&QTimer::timeout,this, &Widget::updateRoleAnimation);//采用信号和槽连接：1.版本中没有callOnTimeout 2.更稳定
	updateTimer->start(1500);

	//设置窗口阴影
	QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
	shadow->setColor(QColor(230, 230, 230));
	shadow->setBlurRadius(10);
	this->setGraphicsEffect(shadow);

	//安装事件过滤器
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
	currFrame = (currFrame + 1) % 5;//5张图内不断变帧
}

bool Widget::eventFilter(QObject* watched, QEvent* event)
{
	QMouseEvent* mouse = static_cast<QMouseEvent*> (event);//父类强转子类
	//判断鼠标左键点击事件
	static QPoint begpos;//不能让该点移动，否则移动的交互方式为窗口左上角开始移动
	if (event->type() == QEvent::MouseButtonPress && mouse->buttons() & Qt::MouseButton::LeftButton)
	{
		//保存鼠标当前点击坐标的位置与窗口原点（左上角）的物理距离差
		begpos = mouse->globalPos() - this->pos();
	}
	//判断鼠标移动事件
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

	//个人设置将桌面宠物始终置顶，防止切换壁纸时出现的壁纸进程在后方掩盖宠物导致无法显示的bug
	setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

	connect(exitBtn, &QPushButton::pressed, this, [=]() 
		{
			Widget::close();
			desktopWidget->close();
		});
	connect(switchBtn, &QPushButton::pressed, this, [=]() 
		{
			QString filename = QFileDialog::getOpenFileName(nullptr, "选择壁纸", "./", "Image(*.jpg *.png)");
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
		});//切换图标用于更换壁纸
	connect(openBtn, &QPushButton::pressed, this, [=]()
		{
			playlist = new QMediaPlaylist(this);
			QString medianame = QFileDialog::getOpenFileName(nullptr, "选择文件", "./", "");
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

			//当标记达到最后时，应该检测让其再次从0开始

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
//	//先判断当前位置是否为尾端，若是，将musicValue置为0
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