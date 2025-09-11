#ifndef CUSTOMTABBAR_H
#define CUSTOMTABBAR_H

#include <QTabBar>
#include <QStylePainter>
#include <QStyleOptionTab>

class CustomTabBar : public QTabBar {
    Q_OBJECT
public:
    using QTabBar::QTabBar;

    QSize tabSizeHint(int index) const override {
        QSize s = QTabBar::tabSizeHint(index);
        // swap width and height so tab is taller for vertical orientation
        s.transpose();
        return s;
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for (int i = 0; i < count(); i++) {
            initStyleOption(&opt, i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);

            painter.save();

            QSize s = opt.rect.size();
            s.transpose();
            QRect r(QPoint(), s);
            r.moveCenter(opt.rect.center());
            opt.rect = r;

            QPoint c = tabRect(i).center();
            painter.translate(c);
            painter.rotate(90);
            painter.translate(-c);
            painter.drawControl(QStyle::CE_TabBarTabLabel, opt);

            painter.restore();
        }
    }
};

#endif // CUSTOMTABBAR_H

