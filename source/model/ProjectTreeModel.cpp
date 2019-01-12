#include <QtGui/QColor>

#include "gotime/GotimeControl.h"
#include "ProjectTreeModel.h"

ProjectTreeModel::ProjectTreeModel(GotimeControl *control, QObject *parent) : _control(control),
                                                                              QAbstractItemModel(parent) {
    QList<QVariant> headers;
    headers << "Name" << "Today" << "This week" << "This month";

    _rootItem = new ProjectTreeItem(headers, Project());
    _projects << _control->loadProjects();

    // fixme update status regularly?
    _status = control->projectsStatus();

    refreshProjects(_rootItem);
//    printProjects(0, _rootItem);
}

ProjectTreeModel::~ProjectTreeModel() {
    delete _rootItem;
}

void ProjectTreeModel::refreshProjects(ProjectTreeItem *root) {
    for (const auto &project : _projects) {
        if (project.getParentID().isEmpty()) {
            root->appendChild(createModelItem(_projects, project, root));
        }
    }
}

ProjectTreeItem *
ProjectTreeModel::createModelItem(const QList<Project> &allProjects, const Project &project, ProjectTreeItem *parent) {
    QString projectID = project.getID();
    ProjectStatus state = _status.get(projectID);

    QList<QVariant> items;
    items << project.getShortName()
          << state.dayTotal.formatShort()
          << state.weekTotal.formatShort()
          << state.monthTotal.formatShort();

    auto *item = new ProjectTreeItem(items, project, parent);
    for (const auto &p: allProjects) {
        if (p.getParentID() == project.getID()) {
            item->appendChild(createModelItem(allProjects, p, item));
        }
    }
    return item;
}

int ProjectTreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return static_cast<ProjectTreeItem *>(parent.internalPointer())->columnCount();
    } else {
        return _rootItem->columnCount();
    }
}

QVariant ProjectTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole && index.column() >= 1) {
        return Qt::AlignRight + Qt::AlignVCenter;
    }

    if (role == Qt::ForegroundRole) {
        auto *item = static_cast<ProjectTreeItem *>(index.internalPointer());
        if (index.column() >= 1 && item->data(index.column()).toString() == QString("0:00h")) {
            return QVariant(QColor(Qt::lightGray));
        };
    }

    if (role == Qt::DisplayRole) {
        auto *item = static_cast<ProjectTreeItem *>(index.internalPointer());
        return item->data(index.column());
    }

    return QVariant();
}

Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}

QVariant ProjectTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return _rootItem->data(section);
    }

    if (role == Qt::TextAlignmentRole) {
        if (section >= 1) {
            return Qt::AlignRight;
        }
        return Qt::AlignLeft;
    }

    return QVariant();
}

QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    ProjectTreeItem *parentItem;
    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<ProjectTreeItem *>(parent.internalPointer());
    }

    ProjectTreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    return {};
}

QModelIndex ProjectTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return {};
    }

    auto *childItem = static_cast<ProjectTreeItem *>(index.internalPointer());
    auto *parentItem = childItem->parentItem();

    if (parentItem == _rootItem) {
        return {};
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectTreeModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    ProjectTreeItem *parentItem;
    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<ProjectTreeItem *>(parent.internalPointer());
    }
    return parentItem->childCount();
}

void ProjectTreeModel::printProjects(int level, ProjectTreeItem *root) {
    qDebug() << QString(" ").repeated(level) << root->data(0).toString();
    for (int i = 0; i < root->childCount(); ++i) {
        printProjects(level + 1, root->child(i));
    }
}

