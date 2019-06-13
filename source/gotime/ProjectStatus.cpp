#include <QtCore>

#include "ProjectStatus.h"

const QString ProjectStatus::OVERALL_ID = "ALL";

ProjectStatus::ProjectStatus(QString &id,
                             Timespan all, Timespan allTotal,
                             Timespan year, Timespan yearTotal,
                             Timespan month, Timespan monthTotal,
                             Timespan week, Timespan weekTotal,
                             Timespan yesterday, Timespan yesterdayTotal,
                             Timespan day, Timespan dayTotal) : id(id),
                                                                all(all), allTotal(allTotal),
                                                                year(year), yearTotal(yearTotal),
                                                                month(month),
                                                                monthTotal(monthTotal),
                                                                week(week), weekTotal(weekTotal),
                                                                yesterday(yesterday), yesterdayTotal(yesterdayTotal),
                                                                day(day), dayTotal(dayTotal) {
}

ProjectStatus::ProjectStatus() = default;

ProjectsStatus::ProjectsStatus(const QHash<QString, ProjectStatus> &mapping) : _map(mapping) {}

ProjectsStatus::ProjectsStatus() = default;

ProjectStatus ProjectsStatus::get(const QString &projectID) const {
    if (!_map.contains(projectID)) {
        return ProjectStatus();
    }
    return _map[projectID];
}

int ProjectsStatus::size() const {
    return _map.size();
}

QStringList ProjectsStatus::getProjectIDs() {
    return QStringList(_map.keys());
}

const QHash<QString, ProjectStatus> &ProjectsStatus::getMapping() const {
    return _map;
}

ProjectStatus ProjectsStatus::getOverallStatus() {
    return get("ALL");
}
