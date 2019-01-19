#ifndef TOM_UI_FRAMETABLEVIEW_H
#define TOM_UI_FRAMETABLEVIEW_H

#include <QtCore/QArgument>
#include <QtWidgets/QTableView>

#include "gotime/GotimeControl.h"
#include "data/Project.h"
#include "model/frametableviewmodel.h"

class FrameTableView : public QTableView {
Q_OBJECT
public:
    explicit FrameTableView(QWidget *parent);

    void setup(GotimeControl *control);

public slots:

    void onProjectSelected(const Project &project);

private slots:

    void onCustomContextMenuRequested(const QPoint &pos);

private:
    GotimeControl *_control;
    QSortFilterProxyModel *_sortedModel;
    FrameTableViewModel *_sourceModel;

    void showContextMenu(Frame *frame, QPoint globalPos);
};


#endif //TOM_UI_FRAMETABLEVIEW_H
