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

#ifndef QMPDSTATUS_H_
#define QMPDSTATUS_H_

#include <QtCore/QTime>
#include <mpd/client.h>

class QMpdStatus
{
public:
    QMpdStatus(mpd_status *status = 0);
    virtual ~QMpdStatus();

public:
    enum State
    {
        StateUnknown,
        StateStop,
        StatePlay,
        StatePause
    };

    enum Mode
    {
        ModeUnknown,
        ModeNormal,
        ModeRepeat,
        ModeRandom,
        ModeRandomRepeat
    };

public:
    void  setStatus(mpd_status *status);
    bool  isUpdating() const;
    int   playlistVersion() const;
    State state() const;
    bool  isRepeat() const;
    bool  isRandom() const;
    Mode  mode() const;
    int   songId() const;
    int   songPos() const;
    int   elapsedSeconds() const;
    int   volume() const;
    QString error() const;

private:
    bool  updating_;
    int   playlistVersion_;
    State state_;
    bool  repeat_;
    bool  random_;
    Mode  mode_;
    int   songId_;
    int   songPos_;
    int   elapsedSeconds_;
    int   volume_;
    QString error_;
};

#endif // QMPDSTATUS_H_
