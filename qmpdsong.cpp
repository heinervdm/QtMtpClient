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

#include "qmpdsong.h"

QMpdSong::QMpdSong(const mpd_song *song, int i)
{
    setSong(song, i);
}

QMpdSong::QMpdSong(mpd_song *song, int i)
{
    setSong(song, i);
}

QMpdSong::~QMpdSong()
{
}

void QMpdSong::setSong(const mpd_song *song, int i)
{
    if (song)
    {
        title_ = QString(mpd_song_get_tag(song, MPD_TAG_TITLE, 0));
        artist_ = QString(mpd_song_get_tag(song, MPD_TAG_ARTIST, 0));
        album_ = QString(mpd_song_get_tag(song, MPD_TAG_ALBUM, 0));
        track_ = QString(mpd_song_get_tag(song, MPD_TAG_TRACK, 0)).toInt();
        genre_ = QString(mpd_song_get_tag(song, MPD_TAG_GENRE, 0));
        year_ = QString(mpd_song_get_tag(song, MPD_TAG_DATE, 0)).toInt();
        duration_ = mpd_song_get_duration(song);
        uri_ = QString(mpd_song_get_uri(song));
		id_ = i;
    }
    else
    {
        title_ = QString();
        artist_ = QString();
        album_ = QString();
        track_ = 0;
        genre_ = QString();
        year_ = 0;
        duration_ = 0;
        uri_ = QString();
		id_ = i;
    }
}

void QMpdSong::setSong(mpd_song *song, int i)
{
    setSong((const mpd_song *)song, i);
    if (song)
    {
        mpd_song_free(song);
    }
}

QString QMpdSong::title() const
{
    return title_;
}

QString QMpdSong::artist() const
{
    return artist_;
}

QString QMpdSong::album() const
{
    return album_;
}

int QMpdSong::track() const
{
    return track_;
}

QString QMpdSong::genre() const
{
    return genre_;
}

int QMpdSong::year() const
{
    return year_;
}

QTime QMpdSong::totalTime() const
{
    QTime tt(0, 0);

    return tt.addSecs(duration_);
}

int QMpdSong::duration() const
{
    return duration_;
}

QString QMpdSong::uri() const
{
    return uri_;
}

int QMpdSong::id() const
{
	return id_;
}
