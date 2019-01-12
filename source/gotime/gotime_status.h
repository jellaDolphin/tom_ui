#ifndef GOTIME_UI_GOTIMESTATUS_H
#define GOTIME_UI_GOTIMESTATUS_H

#include "data/Project.h"

class gotime_status {
public:
    explicit gotime_status();

    explicit gotime_status(bool valid, Project &activeProject, QDateTime &startTime);

    const Project &currentProject() const;

    const QDateTime &startTime() const;

    const bool isValid;

private:
    const Project _project;
    const QDateTime _startTime;
};

#endif 
