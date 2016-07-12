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

#include "qmpdplaylist.h"
#include <QString>

QMpdPlaylist::QMpdPlaylist(const mpd_playlist *playlist)
{
    setPlaylist(playlist);
}

QMpdPlaylist::QMpdPlaylist(mpd_playlist *playlist)
{
    setPlaylist(playlist);
}

QMpdPlaylist::~QMpdPlaylist()
{
}

void QMpdPlaylist::setPlaylist(const mpd_playlist *playlist)
{
    if (playlist)
    {
		playlist_ = mpd_playlist_get_path(playlist);
    }
    else
    {
		playlist_ = "";
    }
}

void QMpdPlaylist::setPlaylist(mpd_playlist *playlist)
{
    setPlaylist((const mpd_playlist *)playlist);
    if (playlist)
    {
        mpd_playlist_free(playlist);
    }
}

QString QMpdPlaylist::getName()
{
	return playlist_;
}
