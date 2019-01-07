#ifndef GOTIME_UI_GOTIMECONTROL_H
#define GOTIME_UI_GOTIMECONTROL_H

#include <QtCore>
#include <QtCore/QString>
#include <data/frame.h>

#include "data/Project.h"
#include "CommandStatus.h"
#include "GotimeStatus.h"

class GotimeControl : public QObject {
Q_OBJECT

public:
    explicit GotimeControl(QString gotimePath, bool bashScript, QObject *parent);

//    void createProject(String name);
//    void createTag(String name);

//    void removeProject(Project project);

//    QList<Frame> loadFrames();
    QList<Project> loadProjects();
//    QList<Tag> loadTags();

    GotimeStatus status();

    bool isStarted(Project &project);

    QList<Frame*> loadFrames(QString projectID, bool includeSubprojects);

signals:

    void projectStarted(const Project &);

    void projectStopped(const Project &);

    void projectCancelled(const Project &);


public slots:
    bool startProject(Project &project);

    bool stopActivity();

    bool cancelActivity();

private:
    CommandStatus run(QStringList &args);

    QString _activeProjectID;

    bool _bashScript;
    QString _gotimePath;
};

#endif //GOTIME_UI_GOTIMECONTROL_H
