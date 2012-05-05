/*  Copyright (C) 2011-2012 OpenBOOX
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef QMPDMONITOR_H_
#define QMPDMONITOR_H_

#include <QThread>
#include <QMetaType>

#include "qmpdstatus.h"
#include "qmpdsong.h"

class QMpdMonitor : public QThread
{
    Q_OBJECT

public:
    QMpdMonitor(const QString &host = QString(), int port = 0, int timeout = 0, QObject *parent = 0);
    virtual ~QMpdMonitor();

public:
    void run();
    void stop();

    QMpdStatus status();

Q_SIGNALS:
    void initialized();
    void databaseUpdated(bool changed);
    void playlistChanged();
    void stateChanged(QMpdStatus::State state);
    void modeChanged(QMpdStatus::Mode mode);
    void songChanged(const QMpdSong& song);
    void elapsedSecondsAtStatusChange(int elapsedSeconds);
    void volumeChanged(int volume);

private:
    void connectToServer();

private:
    QString    host_;
    int        port_;
    int        timeout_;
    mpd_connection *connection_;

    bool       active_;
    QMpdStatus status_;
};

Q_DECLARE_METATYPE(QMpdStatus::State)
Q_DECLARE_METATYPE(QMpdStatus::Mode)
Q_DECLARE_METATYPE(QMpdSong)

#endif // QMPDMONITOR_H_
