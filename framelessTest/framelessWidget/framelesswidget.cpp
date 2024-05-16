#include "framelesswidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QStyleOption>
#include <QStyle>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#ifdef Q_OS_WIN
#include <QWindow>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#ifdef Q_CC_MINGW
#pragma comment(lib, "libdwmapi.a")
#else
#pragma comment(lib, "dwmapi")

#endif
#pragma comment(lib, "user32.lib")
#endif

AbstractTitleBar::AbstractTitleBar(QWidget* parent) : QWidget(parent) {}
void AbstractTitleBar::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)window()->windowHandle()->startSystemMove();
}
void AbstractTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	window()->windowHandle() ? window()->showNormal() : window()->showMaximized();
}

void AbstractTitleBar::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

//抽象虚函数，由派生类实现
void AbstractTitleBar::windowTitleEvent(const QString& title) {}
void AbstractTitleBar::windowIconEvent(const QIcon& icon) {}
void AbstractTitleBar::windowStateEvent(bool state) {}
void AbstractTitleBar::insertWidgetEvent(int index, QWidget* widget, int stretch, Qt::Alignment alignment) {}
void AbstractTitleBar::removeWidgetEvent(QWidget* widget) {}
void AbstractTitleBar::flagsEvent(unsigned int flags) {}
void AbstractTitleBar::marginsEvent(int left, int top, int right, int bottom) {}
void AbstractTitleBar::heightEvent(int h) {}
void AbstractTitleBar::windowThemeEvent(int id) {}

class TitleBarPrivate
{
	S_Q(TitleBar)
		QHBoxLayout* m_TitleLayout;
	QPushButton* m_IconLabel;
	QLabel* m_TitleLabel;

	QPushButton* m_MinimizeButton;
	QPushButton* m_MaximizeButton;
	QPushButton* m_CloseButton;

	explicit TitleBarPrivate(TitleBar* ptr) : q(ptr)
	{
		m_TitleLayout = new QHBoxLayout(q);
		m_TitleLayout->setContentsMargins(5, 2, 10, 2);

		m_IconLabel = new QPushButton(q);
		m_IconLabel->setObjectName("TitleBarIcon");
		m_IconLabel->setFixedWidth(25);
		m_IconLabel->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
		m_IconLabel->setIconSize(QSize(20, 20));
		m_IconLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		m_TitleLabel = new QLabel(QApplication::applicationName(), q);
		m_TitleLabel->setObjectName("TitleBarTitle");

		m_MinimizeButton = new QPushButton(q);
		m_MinimizeButton->setObjectName("TitleBarMinimize");
		m_MinimizeButton->setFixedWidth(25);
		m_MinimizeButton->setIcon(QIcon(":/icons/minium.png"));
		m_MinimizeButton->setIconSize(QSize(20, 20));
		m_MinimizeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		m_MaximizeButton = new QPushButton(q);
		m_MaximizeButton->setObjectName("TitleBarMaximize");
		m_MaximizeButton->setFixedWidth(25);
		m_MaximizeButton->setIcon(QIcon(":/icons/maxsize.png"));
		m_MaximizeButton->setIconSize(QSize(15, 15));
		m_MaximizeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		m_CloseButton = new QPushButton(q);
		m_CloseButton->setObjectName("TitleBarClose");
		m_CloseButton->setFixedWidth(25);
		m_CloseButton->setIcon(QIcon(":/icons/close.png"));
		m_CloseButton->setIconSize(QSize(15, 15));
		m_CloseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		m_TitleLayout->addWidget(m_IconLabel);
		m_TitleLayout->addWidget(m_TitleLabel);
		m_TitleLayout->addWidget(m_MinimizeButton);
		m_TitleLayout->addWidget(m_MaximizeButton);
		m_TitleLayout->addWidget(m_CloseButton);

		QObject::connect(m_MinimizeButton, &QPushButton::clicked,
			q->window(), &QWidget::showMinimized);
		QObject::connect(m_MaximizeButton, &QPushButton::clicked,
			q, &TitleBar::mouseDoubleClick);
		QObject::connect(m_CloseButton, &QPushButton::clicked,
			q->window(), &QWidget::close);
	}

};

TitleBar::TitleBar(QWidget* parent) : AbstractTitleBar(parent), d(new TitleBarPrivate(this))
{
}

TitleBar::~TitleBar()
{
	delete d;
}

QAbstractButton* TitleBar::maximizeButton()const
{
	return d->m_MaximizeButton;
}
QPushButton* TitleBar::titleBarIcon()const
{
	return d->m_IconLabel;
}
QLabel* TitleBar::titleBarTitle()const
{
	return d->m_TitleLabel;
}
QPushButton* TitleBar::minimizeButton()const
{
	return d->m_MinimizeButton;
}
QPushButton* TitleBar::closeButton()const
{
	return d->m_CloseButton;
}

void TitleBar::windowTitleEvent(const QString& title)
{
	d->m_TitleLabel->setText(title);
}
void TitleBar::windowIconEvent(const QIcon& icon)
{
	d->m_IconLabel->setIcon(icon);
}

void TitleBar::windowStateEvent(bool state)
{
	switch (m_theme_id)
	{
	case 0:
	{
		d->m_MaximizeButton->setIcon(QIcon(state ? ":/icons/normal.png" : ":/icons/maxsize.png"));
		d->m_CloseButton->setIcon(QIcon(":/icons/close.png"));
		d->m_MinimizeButton->setIcon(QIcon(":/icons/minium.png"));
		break;
	}
	case 1:
	{
		d->m_CloseButton->setIcon(QIcon(":/icons/close_darktheme.png"));
		d->m_MaximizeButton->setIcon(QIcon(state ? ":/icons/normal_darktheme.png" : ":/icons/maxium_darktheme.png"));
		d->m_MinimizeButton->setIcon(QIcon(":/icons/minium_darktheme.png"));
		break;
	}
	default:
		break;
	}


}

void TitleBar::insertWidgetEvent(int index, QWidget* widget, int stretch, Qt::Alignment alignment)
{
	d->m_TitleLayout->insertWidget(index, widget, stretch, alignment);
}
void TitleBar::removeWidgetEvent(QWidget* widget)
{
	d->m_TitleLayout->removeWidget(widget);
}

void TitleBar::flagsEvent(unsigned int flags)
{
	d->m_IconLabel->setVisible(flags & IconHint);
	d->m_TitleLabel->setVisible(flags & TitleHint);
	d->m_MinimizeButton->setVisible(flags & MinimizeHint);
	d->m_MaximizeButton->setVisible(flags & MaximizeHint);
	d->m_CloseButton->setVisible(flags & CloseHint);
}
void TitleBar::marginsEvent(int left, int top, int right, int bottom)
{
	d->m_TitleLayout->setContentsMargins(left, top, right, bottom);
}

void TitleBar::heightEvent(int h)
{
	setFixedHeight(h);
}

void TitleBar::windowThemeEvent(int themeid)
{
	m_theme_id = themeid;
	windowStateEvent(windowState());
}

class framelessWidgetPrivate
{
	S_Q(framelessWidget)
		unsigned int m_Flags{ 0x00 };
	QVBoxLayout* m_CentralLayout;
	AbstractTitleBar* m_TitleBar;
	QWidget* m_CentralWidget;

	explicit framelessWidgetPrivate(framelessWidget* ptr) : q(ptr)
	{
		m_CentralLayout = new QVBoxLayout(q);
		m_CentralLayout->setContentsMargins(0, 0, 0, 0);
		m_CentralLayout->setSpacing(0);

		m_TitleBar = new TitleBar(q);
		m_TitleBar->setFixedHeight(30);

		m_CentralWidget = new QWidget(q);

		m_CentralLayout->addWidget(m_TitleBar);
		m_CentralLayout->addWidget(m_CentralWidget);
	}
};

framelessWidget::framelessWidget(QWidget* parent)
	: QWidget{ parent }, d(new framelessWidgetPrivate(this))
{
	setWindowFlags(Qt::Window
		| Qt::WindowMaximizeButtonHint
		| Qt::WindowMinimizeButtonHint
		| Qt::FramelessWindowHint);

	HWND hwnd = (HWND)winId();
	LONG style = GetWindowLongW(hwnd, GWL_STYLE);
	SetWindowLong(
		hwnd,
		GWL_STYLE,
		style
		| WS_MINIMIZEBOX
		| WS_MAXIMIZEBOX
		| WS_CAPTION
		| CS_DBLCLKS
		| WS_THICKFRAME
	);
	d->m_TitleBar->maximizeButton()->setFixedWidth(50);
	setTitleBarFlags(TitleBar::IconHint |
		TitleBar::TitleHint |
		TitleBar::MinimizeHint |
		TitleBar::MaximizeHint |
		TitleBar::CloseHint);
	//default
	setTheme(0);
}

framelessWidget::~framelessWidget()
{
	delete d;
}

void framelessWidget::setTitleBarWidget(AbstractTitleBar* title)
{
	d->m_CentralLayout->replaceWidget(d->m_TitleBar, title);
	d->m_TitleBar->deleteLater();
	d->m_TitleBar = title;
}
void framelessWidget::setCentralWidget(QWidget* widget)
{
	d->m_CentralLayout->replaceWidget(d->m_CentralWidget, widget);
	d->m_CentralWidget->deleteLater();
	d->m_CentralWidget = widget;
}

void framelessWidget::setWindowIcon(const QIcon& icon)
{
	QWidget::setWindowIcon(icon);
	d->m_TitleBar->windowIconEvent(icon);
}

void framelessWidget::setWindowTitle(const QString& title)
{
	QWidget::setWindowTitle(title);
	d->m_TitleBar->windowTitleEvent(title);
}

void framelessWidget::setTitleBarFlags(unsigned int flags)
{
	d->m_Flags = flags;
	d->m_TitleBar->flagsEvent(flags);
}

void framelessWidget::setTitleBarFlag(unsigned int flag, bool on)
{
	setTitleBarFlags(on ? (d->m_Flags | flag) : (d->m_Flags & ~flag));
}

void framelessWidget::setTitleBarMargins(int left, int top, int right, int bottom)
{
	d->m_TitleBar->marginsEvent(left, top, right, bottom);
}

void framelessWidget::setTitleBarHeight(int h)
{
	d->m_TitleBar->heightEvent(h);
}

void framelessWidget::insertWidget(int index, QWidget* widget, int stretch, Qt::Alignment alignment)
{
	d->m_TitleBar->insertWidgetEvent(index, widget, stretch, alignment);
}

void framelessWidget::removeWidget(QWidget* widget)
{
	d->m_TitleBar->removeWidgetEvent(widget);
}

void framelessWidget::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::WindowStateChange)
	{
		switch (windowState())
		{
		case Qt::WindowMaximized:
		{
			d->m_TitleBar->windowStateEvent(true);
			int border = GetSystemMetrics(SM_CXSIZEFRAME);
			setContentsMargins(border, border, border, border);
			break;
		}
		case Qt::WindowNoState:
		{
			d->m_TitleBar->windowStateEvent(false);
			setContentsMargins(0, 0, 0, 0);
			break;
		}
		default:
			break;
		}
	}
	QWidget::changeEvent(event);
}


bool framelessWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
	const static int itemWidth = 2;
	MSG* msg = static_cast<MSG*>(message);
	switch (msg->message)
	{
		//无边框
	case WM_NCCALCSIZE:
	{
		*result = HTNOWHERE;
		return true;
	}
	//win11 的圆角
	case WM_ACTIVATE:
	{
		MARGINS margins = { 1, 1, 0, 1 };
		HRESULT hr = DwmExtendFrameIntoClientArea(msg->hwnd, &margins);
		*result = hr;
		return true;
	}
	case WM_NCHITTEST:
	{
		POINT mouse = { GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam) };
		RECT rc;
		GetWindowRect(msg->hwnd, &rc);
		RECT rcF = { 0 };
		AdjustWindowRectEx(&rcF, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

		bool left = (mouse.x >= rc.left && mouse.x < rc.left + itemWidth);
		bool right = (mouse.x < rc.right && mouse.x >= rc.right - itemWidth);
		bool top = (mouse.y >= rc.top && mouse.y < rc.top + itemWidth);
		bool bottom = (mouse.y < rc.bottom && mouse.y >= rc.bottom - itemWidth);
		if (top && left)
		{
			*result = HTTOPLEFT;
			return true;
		}
		else if (top && right)
		{
			*result = HTTOPRIGHT;
			return true;
		}
		else if (bottom && left)
		{
			*result = HTBOTTOMLEFT;
			return true;
		}
		else if (bottom && right)
		{
			*result = HTBOTTOMRIGHT;
			return true;
		}
		else if (top)
		{
			*result = HTTOP;
			return true;
		}
		else if (bottom)
		{
			*result = HTBOTTOM;
			return true;
		}
		else if (left)
		{
			*result = HTLEFT;
			return true;
		}
		else if (right)
		{
			*result = HTRIGHT;
			return true;
		}

		QAbstractButton* maximize = d->m_TitleBar->maximizeButton();
		assert(d->m_TitleBar != nullptr && maximize != nullptr &&
			"titleBar or maximizeButton() returned a null pointer!");
		QPoint globalPos(mouse.x, mouse.y);
		//qDebug() << "globalPos:" << globalPos;
		QWindow* handle = window()->windowHandle();
		if (handle && handle->screen())
		{
			QScreen* screen = handle->screen();
			QPoint offset = screen->geometry().topLeft();
			globalPos = (globalPos - offset) / screen->devicePixelRatio() + offset;
		}
		const QPoint& localPos = maximize->mapFromGlobal(globalPos);
		if (maximize->rect().contains(localPos))
		{
			QMouseEvent mouseEvent(maximize->underMouse() ?
				QEvent::MouseMove : QEvent::Enter,
				localPos, localPos,
				Qt::NoButton, Qt::NoButton, Qt::NoModifier);
			QCoreApplication::sendEvent(maximize, &mouseEvent);
			maximize->update();
			*result = HTMAXBUTTON;
			return true;
		}
		else
		{
			if (maximize->underMouse())
			{
				QMouseEvent mouseEvent(QEvent::Leave,
					localPos, globalPos,
					Qt::NoButton, Qt::NoButton,
					Qt::NoModifier);
				QCoreApplication::sendEvent(maximize, &mouseEvent);
				maximize->update();
				return true;
			}
		}
		*result = HTCLIENT;
		return false;
	}
	//最大化按下
	case WM_NCLBUTTONDOWN:
	{
		if (msg->wParam == HTMAXBUTTON)
		{
			QAbstractButton* maximize = d->m_TitleBar->maximizeButton();
			QMouseEvent mouseEvent(QEvent::MouseButtonPress,
				QPoint(), QPoint(),
				Qt::LeftButton, Qt::LeftButton,
				Qt::NoModifier);
			QCoreApplication::sendEvent(maximize, &mouseEvent);
			*result = HTNOWHERE;
			return true;
		}
	}
	//最大化释放
	case WM_NCLBUTTONUP:
	{
		if (msg->wParam == HTMAXBUTTON)
		{
			QAbstractButton* maximize = d->m_TitleBar->maximizeButton();
			QMouseEvent mouseEvent(QEvent::MouseButtonRelease,
				QPoint(), QPoint(),
				Qt::LeftButton, Qt::LeftButton,
				Qt::NoModifier);
			QCoreApplication::sendEvent(maximize, &mouseEvent);
			*result = HTNOWHERE;
			return true;
		}
	}
	//鼠标移入移出移动事件
	case WM_NCMOUSEHOVER:
	case WM_NCMOUSELEAVE:
	case WM_NCMOUSEMOVE:
	{
		if (msg->wParam == HTMAXBUTTON)
		{
			*result = HTNOWHERE;
			return true;
		}
	}
	}
	return false;
}

AbstractTitleBar* framelessWidget::titleBarWidget()const
{
	return d->m_TitleBar;
}
QWidget* framelessWidget::centralWidget()const
{
	return d->m_CentralWidget;
}
unsigned int framelessWidget::titleBarFlag()const
{
	return d->m_Flags;
}

void framelessWidget::setTheme(int id)
{
	theme_id = id;
	d->m_TitleBar->windowThemeEvent(id);
	switch (id)
	{
	case 0://light
		setStyleSheet(R"(
    TitleBar{border-bottom:1px solid #CFCFCF;}
    #TitleBarMinimize,#TitleBarMaximize,#TitleBarClose{border-radius:3px;}
    #TitleBarMinimize:hover,#TitleBarMaximize:hover{background-color:#656769;}
    #TitleBarClose:hover{background-color:rgba(255,0,0,200);}
    #TitleBarTitle{color:#000000;font-size:14px;font-family: Microsoft YaHei;}
    #TitleBarIcon{border:none;padding:0px;background-color:transparent;}
    )");

		break;
	case 1://dark
		setStyleSheet(R"(
    TitleBar{border-bottom:1px solid #CFCFCF;background-color:black;}
    #TitleBarMinimize,#TitleBarMaximize,#TitleBarClose{border-radius:3px;}
    #TitleBarMinimize:hover,#TitleBarMaximize:hover{background-color:#656769;}
    #TitleBarClose:hover{background-color:rgba(255,0,0,200);}
    #TitleBarTitle{color:#FFFFFF;font-size:14px;font-family: Microsoft YaHei;}
    #TitleBarIcon{border:none;padding:0px;background-color:transparent;}
    )");
		break;
	default:
		break;
	}
}