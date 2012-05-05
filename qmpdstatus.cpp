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

#include "qmpdstatus.h"

QMpdStatus::QMpdStatus(mpd_status *status)
{
    setStatus(status);
}

QMpdStatus::~QMpdStatus()
{
}

void QMpdStatus::setStatus(mpd_status *status)
{
    if (status)
    {
        updating_ = mpd_status_get_update_id(status);
        playlistVersion_ = mpd_status_get_queue_version(status);
        state_  = State(mpd_status_get_state(status));
        repeat_ = mpd_status_get_repeat(status);
        random_ = mpd_status_get_random(status);
        mode_   = Mode(((random_ << 1) | repeat_) + 1);
        songId_ = mpd_status_get_song_id(status);
        songPos_ = mpd_status_get_song_pos(status);
        elapsedSeconds_ = mpd_status_get_elapsed_time(status);
        volume_ = mpd_status_get_volume(status);
        error_  = QString(mpd_status_get_error(status));

        mpd_status_free(status);
    }
    else
    {
        updating_ = false;
        playlistVersion_ = 0;
        state_  = StateUnknown;
        repeat_ = false;
        random_ = false;
        mode_   = ModeUnknown;
        songId_ = -1;
        songPos_ = -1;
        elapsedSeconds_ = 0;
        volume_ = -1;
        error_  = QString();
    }
}

bool QMpdStatus::isUpdating() const
{
    return updating_;
}

int QMpdStatus::playlistVersion() const
{
    return playlistVersion_;
}

QMpdStatus::State QMpdStatus::state() const
{
    return state_;
}

bool QMpdStatus::isRepeat() const
{
    return repeat_;
}

bool QMpdStatus::isRandom() const
{
    return random_;
}

QMpdStatus::Mode QMpdStatus::mode() const
{
    return mode_;
}

int QMpdStatus::songId() const
{
    return songId_;
}

int QMpdStatus::songPos() const
{
    return songPos_;
}

int QMpdStatus::elapsedSeconds() const
{
    return elapsedSeconds_;
}

int QMpdStatus::volume() const
{
    return volume_;
}

QString QMpdStatus::error() const
{
    return error_;
}
