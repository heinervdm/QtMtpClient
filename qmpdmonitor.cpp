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

#include <QtCore/QDebug>

#include "qmpdmonitor.h"

QMpdMonitor::QMpdMonitor(const QString &host, int port, int timeout, QObject *parent)
    : QThread(parent)
{
    qRegisterMetaType<QMpdStatus::State>();
    qRegisterMetaType<QMpdStatus::Mode>();
    qRegisterMetaType<QMpdSong>();

    host_ = host;
    port_ = port;
    timeout_ = timeout;
    connection_ = 0;
}

QMpdMonitor::~QMpdMonitor()
{
}

void QMpdMonitor::run()
{
    int mpdEvent = MPD_IDLE_QUEUE | MPD_IDLE_PLAYER | MPD_IDLE_MIXER | MPD_IDLE_OPTIONS;
    bool firstRun = true;

    qDebug() << "QMpdMonitor: monitor thread started";

    connectToServer();

    while(active_)
    {
        QMpdStatus status(mpd_run_status(connection_));

        if (mpdEvent & (MPD_IDLE_QUEUE | MPD_IDLE_PLAYER))
        {
            if (mpdEvent & MPD_IDLE_QUEUE)
            {
                mpdEvent &= ~MPD_IDLE_QUEUE;

                if (status_.playlistVersion() != status.playlistVersion())
                {
                    emit playlistChanged();
                }
            }

            if (mpdEvent & MPD_IDLE_PLAYER)
            {
                mpdEvent &= ~MPD_IDLE_PLAYER;

                if (status_.state() != status.state())
                {
                    emit stateChanged(status.state());
                }

                emit elapsedSecondsAtStatusChange(status.elapsedSeconds());
            }

            if (status_.songId() != status.songId())
            {
                emit songChanged(QMpdSong(mpd_run_current_song(connection_)));
            }
        }

        if (mpdEvent & MPD_IDLE_MIXER)
        {
            mpdEvent &= ~MPD_IDLE_MIXER;

            if (status_.volume() != status.volume())
            {
                emit volumeChanged(status.volume());
            }
        }

        if (mpdEvent & MPD_IDLE_OPTIONS)
        {
            mpdEvent &= ~MPD_IDLE_OPTIONS;

            if (status_.mode() != status.mode())
            {
                emit modeChanged(status.mode());
            }
        }

        if (mpdEvent & (MPD_IDLE_DATABASE | MPD_IDLE_UPDATE))
        {
            if (mpdEvent & MPD_IDLE_DATABASE)
            {
                emit databaseUpdated(true);
            }
            else if (!status.isUpdating())
            {
                emit databaseUpdated(false);
            }

            mpdEvent &= ~(MPD_IDLE_DATABASE | MPD_IDLE_UPDATE);
        }


        for (int j = 0;; j++)
        {
            int i = 1 << j;
            QString name(mpd_idle_name(mpd_idle(i)));

            if (name.isEmpty())
            {
                break;
            }

            if (mpdEvent & i)
            {
                qDebug() << "unhandled mpd event:" << name;
            }
        }

        status_ = status;

        if (firstRun)
        {
            firstRun = false;
            emit initialized();
        }

        mpdEvent = int(mpd_run_idle(connection_));
    }

    if (connection_)
    {
        mpd_connection_free(connection_);
        connection_ = 0;
    }

    qDebug() << "QMpdMonitor: monitor thread stopped";
}

void QMpdMonitor::stop()
{
    active_ = false;
}

QMpdStatus QMpdMonitor::status()
{
    return status_;
}

void QMpdMonitor::connectToServer()
{
    char *hostPtr = 0;

    if (!host_.isEmpty())
    {
        hostPtr = host_.toLocal8Bit().data();
    }

    connection_ = mpd_connection_new(hostPtr, port_, timeout_);
    if (connection_)
    {
        if (mpd_connection_get_error(connection_) == MPD_ERROR_SUCCESS)
        {
            active_ = true;
        }
        else
        {
            mpd_connection_free(connection_);
            connection_ = 0;
            active_ = false;
        }
    }
    else
    {
        active_ = false;
    }
}
