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

#ifndef QMPDDIRECTORY_H_
#define QMPDDIRECTORY_H_

#include <mpd/client.h>

#include <QtCore/QDateTime>

class QMpdDirectory
{
public:
    QMpdDirectory(const mpd_directory *song = 0);
    QMpdDirectory(mpd_directory *song);
    ~QMpdDirectory();

public:
    void setDirectory(const mpd_directory *song);
    void setDirectory(mpd_directory *song);

    QString& getPath();
    QDateTime& getLastModifiedTime();

private:
    QString path_;
    QDateTime mtime_;

};

typedef QList<QMpdDirectory> QMpdDirectoryList;

#endif // QMPDDIRECTORY_H_
