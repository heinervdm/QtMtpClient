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

#ifndef QMPDENTITY_H_
#define QMPDENTITY_H_

#include <mpd/client.h>
#include "qmpdsong.h"
#include "qmpddirectory.h"
#include "qmpdplaylist.h"

class QMpdEntity
{
public:
    QMpdEntity(const mpd_entity *entity = 0);
    QMpdEntity(mpd_entity *entity);
    ~QMpdEntity();

public:
    void setEntity(const mpd_entity *entity);
    void setEntity(mpd_entity *entity);

    mpd_entity_type getType() const;
    const QMpdSong& getSong() const;
    const QMpdDirectory& getDirectory() const;
    const QMpdPlaylist& getPlaylist() const;

private:
    QMpdSong song_;
    QMpdDirectory directory_;
    QMpdPlaylist playlist_;
    mpd_entity_type type_;
};

typedef QList<QMpdEntity> QMpdEntityList;

#endif // QMPDENTITY_H_
