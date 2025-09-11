#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include "CustomTabBar.h"

class CustomTabWidget : public QTabWidget {
    Q_OBJECT
public:
    using QTabWidget::QTabWidget;

    CustomTabWidget(QWidget *parent = nullptr)
        : QTabWidget(parent)
    {
        setTabBar(new CustomTabBar(this));
        setTabPosition(QTabWidget::West);
    }
};

#endif // CUSTOMTABWIDGET_H

