#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QTabWidget>
#include <QWebEngineView>
#include <QPushButton>
#include <QTabBar>
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->setStyleSheet(R"(
        QTabWidget::pane {
            border: 1px solid #444;
            background: #222;   /* content area background */
        }

        QTabWidget::tab-bar {
            alignment: left;
            background: #222;   /* space behind the tabs */
        }

        QTabBar {
            background: #222;   /* ensures consistency */
        }

        QTabBar::tab {
            background: #333;
            color: white;
            padding: 8px;
        }

        QTabBar::tab:selected {
            background: #555;
        }

        QPushButton {
            background: #444;
            color: white;
            border: 1px solid #fff;
            padding: 5px 10px;
            border-radius: 3px;
        }

        QPushButton:hover {
            background: #555;
        }

        QLabel {
            color: #ddd;
            font-size: 14px;
        }
        QWidget {
            background: #151515;
            color: white;
        }
)");
    while (ui->Homespace->count() > 0) {
        QWidget *w = ui->Homespace->widget(0);
        ui->Homespace->removeTab(0);
        delete w;
    }
    // First homespace
    newHomespace();

    // Menu action: go home in current tab
    connect(ui->actionHomepage, &QAction::triggered, this, &MainWindow::goHome);
    connect(ui->goBackBtn, &QPushButton::clicked, this, &MainWindow::goBack);
    connect(ui->forwardBtn, &QPushButton::clicked, this, &MainWindow::goForward);
    connect(ui->reloadBtn, &QPushButton::clicked, this, &MainWindow::reloadPage);
    // Close homespace itself
    connect(ui->Homespace, &QTabWidget::tabCloseRequested,
            this, [this](int index){
                QWidget *w = ui->Homespace->widget(index);
                ui->Homespace->removeTab(index);
                delete w;
            });
    // add new homespace button
    //define button
    QPushButton *addHomespaceBtn = new QPushButton("+");
    addHomespaceBtn->setFixedSize(24, 24); // small button
    ui->Homespace->setCornerWidget(addHomespaceBtn, Qt::TopRightCorner);
    ui->Homespace->setUsesScrollButtons(true);
    ui->Homespace->tabBar()->setExpanding(false);
    //functionality
    connect(addHomespaceBtn, &QPushButton::clicked, this, &MainWindow::newHomespace);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::goHome()
{
    QWidget *container = qobject_cast<QWidget *>(ui->Homespace->currentWidget());
    if (!container) return;

    QTabWidget *inner = container->findChild<QTabWidget *>();
    if (!inner) return;

    QWebEngineView *view = qobject_cast<QWebEngineView *>(inner->currentWidget());
    if (!view) return;

    QString path = QCoreApplication::applicationDirPath() + "/start_page.html";
    view->setUrl(QUrl::fromLocalFile(path));
}



void MainWindow::newTab(const QUrl &url)
{
    // current homespace container
    QWidget *container = qobject_cast<QWidget *>(ui->Homespace->currentWidget());
    if (!container) return;

    // find the inner QTabWidget inside the container
    QTabWidget *inner = container->findChild<QTabWidget *>();
    if (!inner) return;

    // Create a container for this tab
    QWidget *tabPage = new QWidget;
    QVBoxLayout *tabLayout = new QVBoxLayout(tabPage);
    tabLayout->setContentsMargins(0,0,0,0);
    tabLayout->setSpacing(2);

    QLabel *tabLabel = new QLabel("New Tab");
    tabLabel->setAlignment(Qt::AlignLeft);
    tabLayout->addWidget(tabLabel);

    QWebEngineView *view = new QWebEngineView;
    view->setUrl(url);
    tabLayout->addWidget(view, 1);

    // Add the container into the inner QTabWidget
    int index = inner->addTab(tabPage, ""); // leave tab text empty
    inner->setCurrentIndex(index);

    // Keep the label synced with the web page title
    connect(view, &QWebEngineView::titleChanged, this,
            [tabLabel](const QString &title){
                if (!title.isEmpty())
                    tabLabel->setText(title);
            });
}
void MainWindow::newHomespace()
{
    QString path = QCoreApplication::applicationDirPath() + "/start_page.html";

    // Create a container widget for this homespace
    QWidget *container = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(2);
    // Create the inner QTabWidget
    QTabWidget *inner = new QTabWidget;
    inner->setTabsClosable(true);
    inner->setMovable(true);
    inner->setTabPosition(QTabWidget::West);
    layout->addWidget(inner);  // stretch = 1 to expand

    // Add "+" button to the inner tab bar
    QPushButton *addTabBtn = new QPushButton("+");
    addTabBtn->setFixedSize(24, 24);
    inner->setCornerWidget(addTabBtn, Qt::TopRightCorner);

    connect(addTabBtn, &QPushButton::clicked, this, [this, path]() {
        newTab(QUrl::fromLocalFile(path));
    });

    connect(inner, &QTabWidget::tabCloseRequested, this,
            [inner](int index){
                QWidget *w = inner->widget(index);
                inner->removeTab(index);
                delete w;
            });

    // Determine homespace index (1-based)
    int nextIndex = ui->Homespace->count() + 1;

    // Add this container as a new Homespace tab with index in title
    int hsIndex = ui->Homespace->addTab(container, QString("Homespace %1").arg(nextIndex));
    ui->Homespace->setCurrentIndex(hsIndex);

    // First page inside
    // First page inside
    QWidget *tabPage = new QWidget;
    QVBoxLayout *tabLayout = new QVBoxLayout(tabPage);
    tabLayout->setContentsMargins(0,0,0,0);
    tabLayout->setSpacing(2);

    QLabel *tabLabel = new QLabel("Start Page");
    tabLabel->setAlignment(Qt::AlignCenter);
    tabLayout->addWidget(tabLabel);

    QWebEngineView *view = new QWebEngineView;
    view->setUrl(QUrl::fromLocalFile(path));
    tabLayout->addWidget(view, 1);

    inner->addTab(tabPage, "");
}

void MainWindow::closeCurrentTab()
{
    auto *inner = qobject_cast<QTabWidget *>(ui->Homespace->currentWidget());
    if (!inner) return;

    int index = inner->currentIndex();
    if (index >= 0) {
        QWidget *w = inner->widget(index);
        inner->removeTab(index);
        delete w;
    }
}
void MainWindow::goBack() {
    QWidget *container = qobject_cast<QWidget *>(ui->Homespace->currentWidget());
    if (!container) return;

    QTabWidget *inner = container->findChild<QTabWidget *>();
    if (!inner) return;

    QWidget *tabPage = inner->currentWidget();
    if (!tabPage) return;

    QWebEngineView *view = tabPage->findChild<QWebEngineView *>();
    if (view) view->back();
}

void MainWindow::goForward() {
    QWidget *container = qobject_cast<QWidget *>(ui->Homespace->currentWidget());
    if (!container) return;

    QTabWidget *inner = container->findChild<QTabWidget *>();
    if (!inner) return;

    QWidget *tabPage = inner->currentWidget();
    if (!tabPage) return;

    QWebEngineView *view = tabPage->findChild<QWebEngineView *>();
    if (view) view->forward();
}

void MainWindow::reloadPage() {
    QWidget *container = qobject_cast<QWidget *>(ui->Homespace->currentWidget());
    if (!container) return;

    QTabWidget *inner = container->findChild<QTabWidget *>();
    if (!inner) return;

    QWidget *tabPage = inner->currentWidget();
    if (!tabPage) return;

    QWebEngineView *view = tabPage->findChild<QWebEngineView *>();
    if (view) view->reload();
}
