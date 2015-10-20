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

#include "qmpddirectory.h"

QMpdDirectory::QMpdDirectory(const mpd_directory *directory)
{
    setDirectory(directory);
}

QMpdDirectory::QMpdDirectory(mpd_directory *directory)
{
    setDirectory(directory);
}

QMpdDirectory::~QMpdDirectory()
{
}

void QMpdDirectory::setDirectory(const mpd_directory *directory)
{
    if (directory)
    {
        path_ = QString(mpd_directory_get_path(directory));
        mtime_.fromMSecsSinceEpoch(mpd_directory_get_last_modified(directory));
    }
    else
    {
        path_ = QString();
        mtime_.fromMSecsSinceEpoch(0);
    }
}

void QMpdDirectory::setDirectory(mpd_directory *directory)
{
    setDirectory((const mpd_directory *)directory);
    if (directory)
    {
        mpd_directory_free(directory);
    }
}

QString& QMpdDirectory::getPath() {
	return path_;
}

QDateTime& QMpdDirectory::getLastModifiedTime() {
	return mtime_;
}
