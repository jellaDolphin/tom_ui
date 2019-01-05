//
// Created by jansorg on 05.01.19.
//

#ifndef GOTIME_UI_TIMESPAN_H
#define GOTIME_UI_TIMESPAN_H

#include <QtCore/QDateTime>
#include <QtCore/qfloat16.h>

class Timespan {
public:
    static Timespan of(const QDateTime &start, const QDateTime &end);

    explicit Timespan(qint64 millis);

    const double asHours() const;

    const double asMinutes() const;

    const double asSeconds() const;

    const QString format() const;

    const QString formatDecimal() const;

private:
    qint64 _msecs;

    static const qint64 _msecsPerSecond = 1000;
    static const qint64 _msecsPerMinute = 60 * _msecsPerSecond;
    static const qint64 _msecsPerHour = 60 * _msecsPerMinute;
};


#endif //GOTIME_UI_TIMESPAN_H
