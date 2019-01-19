#include <QtWidgets/QTreeView>
#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMainWindow>

#include "version.h"
#include "icons.h"
#include "main_window.h"
#include "model/frametableviewmodel.h"
#include "model/ProjectTreeModel.h"
#include "view/projecttreeview.h"

MainWindow::MainWindow(GotimeControl *control, ProjectStatusManager* statusManager, QMainWindow *parent) : QMainWindow(parent), gotimeControl(control) {
//#ifdef Q_OS_LINUX
    setWindowIcon(QIcon(":/images/logo32.png"));
//#endif

    ui.setupUi(this);
    ui.projectTree->setup(control, statusManager);
    ui.frameView->setup(control);

    ui.actionQuit->setIcon(Icons::exit());
    ui.actionHelpAbout->setIcon(Icons::about());

    connect(ui.projectTree, &ProjectTreeView::projectSelected, ui.frameView, &FrameTableView::onProjectSelected);
    connect(ui.actionQuit, &QAction::triggered, &QCoreApplication::quit);

    createActions();
    refreshData();
}

void MainWindow::refreshData() {
    ui.projectTree->refresh();
}

MainWindow::~MainWindow() = default;


void MainWindow::createActions() {
}

void MainWindow::helpAbout() {
    QString about = QString("Tom is a simple UI for the <a href=\"https://github.com/jansorg/tom-ui\">tom time tracker</a> command line application.<br><br>Version: %1").arg(PROJECT_VERSION);
    QMessageBox::about(this, "About Tom", about);
}

void MainWindow::createProject() {

}
