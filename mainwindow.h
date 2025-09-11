#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QUrl>
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void goHome();   // slot for homepage action
    void newTab(const QUrl &url = QUrl("about:blank"));
    void newHomespace();
    void closeCurrentTab();
    void goBack();
    void goForward();
    void reloadPage();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
