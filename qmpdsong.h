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

#ifndef QMPDSONG_H_
#define QMPDSONG_H_

#include <QtCore/QString>
#include <QtCore/QTime>
#include <mpd/client.h>

class QMpdSong
{
public:
    QMpdSong(const mpd_song *song = 0);
    QMpdSong(mpd_song *song);
    ~QMpdSong();

public:
    void setSong(const mpd_song *song);
    void setSong(mpd_song *song);

    QString title() const;
    QString artist() const;
    QString album() const;
    int     track() const;
    QString genre() const;
    int     year() const;
    QTime   totalTime() const;
    int     duration() const;
    QString uri() const;

private:
    QString title_;
    QString artist_;
    QString album_;
    int     track_;
    QString genre_;
    int     year_;
    int     duration_;
    QString uri_;
};

typedef QList<QMpdSong> QMpdSongList;

#endif // QMPDSONG_H_
