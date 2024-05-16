#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H
#define S_D(Class)  \
    private: \
    Class(const Class&) = delete; \
    Class &operator=(const Class&)=delete; \
    friend class Class##Private; \
    Class##Private * const d;
#define S_Q(Class)  \
    private: \
    Class##Private(const Class##Private &)=delete; \
    Class##Private(Class##Private &&)=delete; \
    Class##Private &operator=(const Class##Private &)=delete; \
    Class##Private &operator=(Class##Private &&)=delete; \
    friend Class; \
    Class * const q;

#include <QWidget>

class QAbstractButton;

class AbstractTitleBar : public QWidget
{
	Q_OBJECT
public:
	explicit AbstractTitleBar(QWidget* parent = nullptr);
	//最大化按钮
	[[nodiscard]] virtual QAbstractButton* maximizeButton() const = 0;

	inline void mouseDoubleClick()
	{
		mouseDoubleClickEvent(nullptr);
	}
protected:
	void mousePressEvent(QMouseEvent* event)override;
	void mouseDoubleClickEvent(QMouseEvent* event)override;
	void paintEvent(QPaintEvent* event)override;

	//用于setWindowTitle时调用该函数，更改自定义窗口标题
	virtual void windowTitleEvent(const QString& title);
	//用于setWindowIcon时调用该函数，更改自定义窗口图标
	virtual void windowIconEvent(const QIcon& icon);
	//窗口状态变化时调用，更改最大化图标
	virtual void windowStateEvent(bool state);
	//用于insertWidget时调用该函数，在标题栏中添加控件
	virtual void insertWidgetEvent(int index, QWidget* widget, int stretch, Qt::Alignment alignment);
	//用于removeWidget时调用该函数，自定义标题栏移除控件
	virtual void removeWidgetEvent(QWidget* widget);
	//自定义事件
	virtual void flagsEvent(unsigned int flags);
	//用于控制标题栏布局
	virtual void marginsEvent(int left, int top, int right, int bottom);
	//用于控制标题栏高度
	virtual void heightEvent(int h);
	//用于控制主题下控件状态
	virtual void windowThemeEvent(int id = 0);

private:
	int m_theme_id = 0;
	friend class framelessWidget;
};

class QPushButton;
class QLabel;
class TitleBarPrivate;
class TitleBar : public AbstractTitleBar
{
	Q_OBJECT
		S_D(TitleBar)

public:
	enum TitleFlag
	{
		IconHint = 0x01,
		TitleHint = 0x02,
		MinimizeHint = 0x04,
		MaximizeHint = 0x08,
		CloseHint = 0x10
	};
	friend inline TitleFlag operator |(TitleFlag lhs, TitleFlag rhs)
	{
		return static_cast<TitleBar::TitleFlag>(static_cast<int>(lhs) | static_cast<int>(rhs));
	}

	explicit TitleBar(QWidget* parent = nullptr);
	~TitleBar()override;

	//返回自定义控件，用于设置属性
	[[nodiscard]] QAbstractButton* maximizeButton()const override;
	[[nodiscard]] QPushButton* titleBarIcon()const;
	[[nodiscard]] QLabel* titleBarTitle()const;
	[[nodiscard]] QPushButton* minimizeButton()const;
	[[nodiscard]] QPushButton* closeButton()const;

protected:
	void windowTitleEvent(const QString& title)override;
	void windowIconEvent(const QIcon& icon)override;
	void windowStateEvent(bool state)override;
	void insertWidgetEvent(int index, QWidget* widget, int stretch, Qt::Alignment alignment)override;
	void removeWidgetEvent(QWidget* widget)override;
	void flagsEvent(unsigned int flags)override;
	void marginsEvent(int left, int top, int right, int bottom)override;
	void heightEvent(int h)override;
	void windowThemeEvent(int themeid = 0)override;

private:
	int m_theme_id = 0;
};

// class TitleBarPrivate{
//     S_Q(TitleBar)

// };

class framelessWidgetPrivate;

class framelessWidget : public QWidget
{
	Q_OBJECT

		S_D(framelessWidget)
public:
	explicit framelessWidget(QWidget* parent = nullptr);
	~framelessWidget()override;
	void setTitleBarWidget(AbstractTitleBar* title);
	void setCentralWidget(QWidget* widget);
	void setWindowIcon(const QIcon& icon);
	void setWindowTitle(const QString& title);
	//设置标题栏自定义事件
	void setTitleBarFlags(unsigned int flags);
	//设置标题栏单个事件
	void setTitleBarFlag(unsigned int flag, bool on = true);

	//设置标题栏边距
	void setTitleBarMargins(int left, int top, int right, int bottom);

	//设置标题栏高度
	void setTitleBarHeight(int h);

	//向标题栏插入控件
	void insertWidget(int index, QWidget* widget, int stretch = 0, Qt::Alignment alignment = Qt::AlignHCenter);

	//移除标题栏控件
	void removeWidget(QWidget* widget);

	//返回标题栏
	[[nodiscard]] AbstractTitleBar* titleBarWidget()const;
	//返回中心部件
	[[nodiscard]] QWidget* centralWidget()const;
	//返回标题栏自定义事件
	[[nodiscard]] unsigned int titleBarFlag()const;

	void setTheme(int id = 0);

protected:
	void changeEvent(QEvent* event)override;
	bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result)override;
signals:

private:
	int theme_id = 0;
};

#endif // FRAMELESSWIDGET_H
