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

#include "qmpdentity.h"

QMpdEntity::QMpdEntity(const mpd_entity *entity)
{
    setEntity(entity);
}

QMpdEntity::QMpdEntity(mpd_entity *entity)
{
    setEntity(entity);
}

QMpdEntity::~QMpdEntity()
{
}

void QMpdEntity::setEntity(const mpd_entity *entity)
{
    if (entity)
    {
        type_ = mpd_entity_get_type(entity);
        switch (type_) {
            case MPD_ENTITY_TYPE_DIRECTORY:
                directory_ = mpd_entity_get_directory(entity);
                break;
            case MPD_ENTITY_TYPE_SONG:
                song_ = mpd_entity_get_song(entity);
                break;
            case MPD_ENTITY_TYPE_PLAYLIST:
                playlist_ = mpd_entity_get_playlist(entity);
                break;
            case MPD_ENTITY_TYPE_UNKNOWN:
            default:
                break;
        }
    }
    else
    {
        type_ = MPD_ENTITY_TYPE_UNKNOWN;
    }
}

void QMpdEntity::setEntity(mpd_entity *entity)
{
    setEntity((const mpd_entity *)entity);
    if (entity)
    {
        mpd_entity_free(entity);
    }
}

const QMpdDirectory& QMpdEntity::getDirectory() const {
    return directory_;
}

const QMpdPlaylist& QMpdEntity::getPlaylist() const {
    return playlist_;
}

const QMpdSong& QMpdEntity::getSong() const {
    return song_;
}

mpd_entity_type QMpdEntity::getType() const {
    return type_;
}
