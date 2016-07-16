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
#include <QtCore/QString>

#include "qmpdclient.h"
#include "qmpdentity.h"
#include "qmpdsong.h"
#include "qmpddirectory.h"
#include "qmpdplaylist.h"

QMpdClient::QMpdClient(QObject *p)
    : QObject(p)
{
    host_ = QString();
    port_ = 0;
    timeout_ = 0;
    connection_ = 0;
    monitor_ = 0;

    id_ = -1;
    pos_ = -1;
}

QMpdClient::~QMpdClient()
{
    disconnectFromServer();
}

bool QMpdClient::connectToServer(const QString &host, int port, int timeout)
{
    if (!connection_)
    {
        char *hostPtr = 0;

        host_ = host;
        port_ = port;
        timeout_ = timeout;

        if (!host_.isEmpty())
        {
            hostPtr = host_.toLocal8Bit().data();
        }

        connection_ = mpd_connection_new(hostPtr, port_, timeout_);
        if (connection_ && mpd_connection_get_error(connection_) != MPD_ERROR_SUCCESS)
        {
            mpd_connection_free(connection_);
            connection_ = 0;
        }
        else
        {
            monitor_ = new QMpdMonitor(host, port, timeout, this);

            connect(monitor_, SIGNAL(initialized()), this, SLOT(onInitialized()));
            connect(monitor_, SIGNAL(databaseUpdated(bool)), this, SLOT(onDatabaseUpdated(bool)));
            connect(monitor_, SIGNAL(playlistChanged()), this, SLOT(onPlaylistChanged()));
            connect(monitor_, SIGNAL(stateChanged(QMpdStatus::State)), this, SLOT(onStateChanged(QMpdStatus::State)));
            connect(monitor_, SIGNAL(modeChanged(QMpdStatus::Mode)), this, SLOT(onModeChanged(QMpdStatus::Mode)));
            connect(monitor_, SIGNAL(songChanged(const QMpdSong &)), this, SLOT(onSongChanged(const QMpdSong &)));
            connect(monitor_, SIGNAL(elapsedSecondsAtStatusChange(int)), this, SLOT(onElapsedSecondsAtStatusChange(int)));
            connect(monitor_, SIGNAL(volumeChanged(int)), this, SLOT(onVolumeChanged(int)));

            monitor_->start();

            syncPlaylist();
        }
    }

    return (connection_ != 0);
}

void QMpdClient::disconnectFromServer()
{
    if (connection_)
    {
        monitor_->stop();
        setVolume(status().volume());    // wake up thread
        monitor_->wait();
        delete monitor_;

        mpd_connection_free(connection_);
        connection_ = 0;
    }
}

int QMpdClient::updateDB(const QString &path)
{
    if (connection_)
    {
        char *pathPtr = 0;

        if (!path.isEmpty())
        {
            pathPtr = path.toLocal8Bit().data();
        }

        return mpd_run_update(connection_, pathPtr);
    }

    return 0;
}

QMpdStatus QMpdClient::status()
{
    QMpdStatus st;

    if (connection_)
    {
        st.setStatus(mpd_run_status(connection_));
    }

    return st;
}

QMpdSong QMpdClient::song()
{
    QMpdSong s;

    if (connection_)
    {
        s.setSong(mpd_run_current_song(connection_));
    }

    return s;
}

QList<QMpdEntity> QMpdClient::ls(const QString &path)
{
    QList<QMpdEntity> list;
    if (connection_)
    {
        QMpdEntity entity;
        mpd_entity *e;

        mpd_send_list_meta(connection_, path.toStdString().c_str());
        while ((e = mpd_recv_entity(connection_)) != 0)
        {
            entity.setEntity(e);
            list.append(entity);
        }

        mpd_response_finish(connection_);
    }
    return list;
}

QMpdSongList QMpdClient::syncPlaylist()
{
    songList_.clear();

    if (connection_ && mpd_send_list_queue_meta(connection_))
    {
        mpd_song *s;

        while ((s = mpd_recv_song(connection_)) != 0)
        {
            songList_.append(QMpdSong(s, mpd_song_get_id(s)));
        }

        mpd_response_finish(connection_);
    }

    return songList_;
}

QMpdSongList QMpdClient::playlist()
{
    return songList_;
}

int QMpdClient::addToPlaylist(const QString &uri, int pos)
{
    id_ = pos_ = -1;

    if (connection_)
    {
        if (pos >= 0)
        {
            id_ = mpd_run_add_id_to(connection_, uri.toStdString().data(), pos);

            if (id_ != -1)
            {
                pos_ = pos;
                songList_.insert(pos, QMpdSong(mpd_run_get_queue_song_id(connection_, id_), id_));
            }
            else
            {
                mpd_connection_clear_error(connection_);
            }
        }
        else
        {
            for (int i = 0; i < songList_.size(); i++)
            {
                if (uri == songList_.at(i).uri())
                {
                    return songList_.at(i).id();
                }
            }

            id_ = mpd_run_add_id(connection_, uri.toStdString().data());

            if (id_ != -1)
            {
                songList_.append(QMpdSong(mpd_run_get_queue_song_id(connection_, id_), id_));
            }
            else
            {
                mpd_connection_clear_error(connection_);

                if (mpd_run_add(connection_, uri.toStdString().data()))
                {
                    syncPlaylist();
                }
                else
                {
                    mpd_connection_clear_error(connection_);
                }
            }
        }
    }

    return id_;
}

void QMpdClient::shufflePlaylist()
{
    if (connection_ && mpd_run_shuffle(connection_))
    {
        syncPlaylist();
    }
}

void QMpdClient::clearPlaylist()
{
    if (connection_ && mpd_run_clear(connection_))
    {
        syncPlaylist();
    }
}

void QMpdClient::play(int id)
{
    if (connection_)
    {
        if (id == -1)
        {
            mpd_run_play(connection_);
        }
        else
        {
            mpd_run_play_id(connection_, id);
        }
    }
}

void QMpdClient::playPosition(int position)
{
    if (connection_)
    {
        mpd_run_play_pos(connection_, position);
    }
}

void QMpdClient::pause()
{
    if (connection_)
    {
        mpd_run_pause(connection_, true);
    }
}

void QMpdClient::stop()
{
    if (connection_)
    {
        mpd_run_stop(connection_);
    }
}

void QMpdClient::next()
{
    if (connection_)
    {
        mpd_run_next(connection_);
    }
}

void QMpdClient::previous()
{
    if (connection_)
    {
        mpd_run_previous(connection_);
    }
}

void QMpdClient::setRepeat(bool on)
{
    if (connection_)
    {
        mpd_run_repeat(connection_, on);
    }
}

void QMpdClient::setRandom(bool on)
{
    if (connection_)
    {
        mpd_run_random(connection_, on);
    }
}

void QMpdClient::setMode(QMpdStatus::Mode mode)
{
    if (connection_)
    {
        switch (mode)
        {
        case QMpdStatus::ModeNormal:
            mpd_run_repeat(connection_, false);
            mpd_run_random(connection_, false);
            break;
        case QMpdStatus::ModeRepeat:
            mpd_run_repeat(connection_, true);
            mpd_run_random(connection_, false);
            break;
        case QMpdStatus::ModeRandom:
            mpd_run_repeat(connection_, false);
            mpd_run_random(connection_, true);
            break;
        case QMpdStatus::ModeRandomRepeat:
            mpd_run_repeat(connection_, true);
            mpd_run_random(connection_, true);
            break;
        default:
            break;
        }
    }
}

void QMpdClient::setVolume(int volume)
{
    if (connection_)
    {
        mpd_run_set_volume(connection_, volume);
    }
}

int QMpdClient::latestSongId() const
{
    return id_;
}

int QMpdClient::latestSongPosition() const
{
    return pos_;
}

void QMpdClient::loadPlaylist(QString name)
{
	mpd_run_load(connection_, name.toStdString().c_str());
}

void QMpdClient::onInitialized()
{
    emit initialized();
}

void QMpdClient::onDatabaseUpdated(bool changed)
{
    emit databaseUpdated(changed);
}

void QMpdClient::onPlaylistChanged()
{
    emit playlistChanged();
}

void QMpdClient::onStateChanged(QMpdStatus::State state)
{
    emit stateChanged(state);
}

void QMpdClient::onModeChanged(QMpdStatus::Mode mode)
{
    emit modeChanged(mode);
}

void QMpdClient::onSongChanged(const QMpdSong& s)
{
    emit songChanged(s);
}

void QMpdClient::onElapsedSecondsAtStatusChange(int elapsedSeconds)
{
    emit elapsedSecondsAtStatusChange(elapsedSeconds);
}

void QMpdClient::onVolumeChanged(int volume)
{
    emit volumeChanged(volume);
}
