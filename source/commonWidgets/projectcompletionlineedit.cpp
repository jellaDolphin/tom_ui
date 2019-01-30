#include <QKeyEvent>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QMenu>

#include "source/gotime/StartStopProjectAction.h"
#include "source/commonModels/projectlistmodel.h"
#include "source/projectlookup/projectlookup.h"
#include "projectcompletionlineedit.h"

ProjectCompletionLineEdit::ProjectCompletionLineEdit(QWidget *parent) : QLineEdit(parent), _control(nullptr) {
    setPlaceholderText(tr("Enter project name or press %1 for recent projects").arg(tr("down arrow")));
}

void ProjectCompletionLineEdit::keyPressEvent(QKeyEvent *event) {
    // display a menu of the most recent proejcts on arrow down
    qDebug() << "keyPressEvent" << event;
    if (event->key() == Qt::Key_Down) {
        event->accept();

        QMenu menu(tr("Recent projects"), this);
        const QList<Project> &projects = _control->loadRecentProjects();
        if (projects.isEmpty()) {
            menu.addSection(tr("No recent projects available"));
        } else {
            menu.addSection(tr("Recent projects"));
            for (const auto &project:projects) {
                auto *action = new StartProjectAction(project, _control, &menu);
                connect(action, &QAction::triggered, [this] { emit completionAccepted(); });
                menu.addAction(action);
            }
        }

        const QRect &r = rect();
        menu.setFixedWidth(r.width());
        menu.exec(mapToGlobal(r.bottomLeft()));
        return;
    }

    QLineEdit::keyPressEvent(event);
}

void ProjectCompletionLineEdit::setup(TomControl *control) {
    _control = control;

    auto *model = new ProjectListModel(control, this);
    auto *completer = new QCompleter(model, this);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(true);
    completer->setMaxVisibleItems(15);
    completer->setCompletionRole(Qt::DisplayRole);
    completer->setFilterMode(Qt::MatchContains);

    setCompleter(completer);
    connect(completer, QOverload<const QModelIndex &>::of(&QCompleter::activated), [this] {
        emit completionAccepted();
    });
}