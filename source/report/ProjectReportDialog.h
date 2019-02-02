#ifndef TOM_UI_REPORTDIALOG_H
#define TOM_UI_REPORTDIALOG_H

#include <QtWidgets/QDialog>
#include <QStringListModel>

#ifdef TOM_REPORTS
#include <QtWebEngineWidgets/QWebEngineView>
#endif

#include "ui_reportDialog.h"
#include "source/data/Project.h"
#include "source/gotime/TomControl.h"
#include "reportsplitmodel.h"

class ProjectReportDialog : public QDialog, private Ui::ReportDialog {
public:
    ProjectReportDialog(QList<Project> projects, TomControl *control, QWidget *parent);

private slots:

    void updateReport();

private:
    void moveSplitSelection(int delta);

    QList<Project> _projects;
    TomControl *_control;
    ReportSplitModel *_splitModel;

    QTemporaryDir _tempDir;
    QString _tempFile;

#ifdef TOM_REPORTS
    QWebEngineView *_webView;
#endif
};

#endif //TOM_UI_REPORTDIALOG_H
