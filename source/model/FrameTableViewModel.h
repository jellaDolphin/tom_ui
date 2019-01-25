#ifndef GOTIME_UI_FRAMETABLEVIEWMODEL_H
#define GOTIME_UI_FRAMETABLEVIEWMODEL_H

#include <QtCore/QAbstractTableModel>
#include <gotime/TomControl.h>

#include "data/Frame.h"

static const QString &FRAMES_MIME_TYPE = "application/x-tom-frames";

class FrameTableViewModel : public QAbstractTableModel {
public:
    explicit FrameTableViewModel(TomControl *control, QObject *parent);

    ~FrameTableViewModel() override;

    void loadFrames(const Project &project);

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Frame *frameAt(const QModelIndex &index) const;

    bool removeRows(int row, int count, const QModelIndex &parent) override;

    Qt::DropActions supportedDragActions() const override;

    QStringList mimeTypes() const override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    static const int COL_START_DATE = 0;
    static const int COL_START = 1;
    static const int COL_END = 2;
    static const int COL_DURATION = 3;
    static const int COL_TAGS = 4;
    static const int COL_PROJECT = 5;
    static const int COL_NOTES = 6;

    static const int COLUMN_COUNT = COL_NOTES + 1;

private slots:

    void onFramesRemoved(const QStringList &frameIDs, const QString &projectID);

    void onFramesMoved(const QStringList &frameIDs, const QString &oldProjectID, const QString &newProjectID);

    void onProjectUpdated(const Project &project);

private:
    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    int findRow(const QString &frameID);

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    TomControl *_control;

    QList<Frame *> _frames;
    Project _currentProject;
};


#endif //GOTIME_UI_FRAMETABLEVIEWMODEL_H
