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

#ifndef QMPDPLAYLIST_H_
#define QMPDPLAYLIST_H_

#include <mpd/client.h>

#include <QList>

class QMpdPlaylist
{
public:
    QMpdPlaylist(const mpd_playlist *song = 0);
    QMpdPlaylist(mpd_playlist *song);
    ~QMpdPlaylist();

public:
    void setPlaylist(const mpd_playlist *song);
    void setPlaylist(mpd_playlist *song);

private:

};

typedef QList<QMpdPlaylist> QMpdPlaylistList;

#endif // QMPDPLAYLIST_H_
