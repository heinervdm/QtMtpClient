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

#ifndef QMPDCLIENT_H_
#define QMPDCLIENT_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "qmpdstatus.h"
#include "qmpdmonitor.h"
#include "qmpdentity.h"
#include "qmpdsong.h"

class QMpdClient : public QObject
{
    Q_OBJECT

public:
    static QMpdClient & instance()
    {
        static QMpdClient instance_;
        return instance_;
    }

    virtual ~QMpdClient();

public:
    bool connectToServer(const QString &host = QString(), int port = 0, int timeout = 0);
    void disconnectFromServer();
    int  updateDB(const QString &path = QString());

    QMpdStatus   status();
    QMpdSong     song();
    QMpdSongList getSongList();
    QMpdSongList syncPlaylist();
    QMpdSongList playlist();
    QMpdEntityList* ls(const QString &path = "");

    int  addToPlaylist(const QString &uri, int pos = -1);
    void shufflePlaylist();
    void clearPlaylist();
    void play(int id = -1);
    void playPosition(int position);
    void pause();
    void stop();
    void next();
    void previous();
    void setRepeat(bool on);
    void setRandom(bool on);
    void setMode(QMpdStatus::Mode mode);
    void setVolume(int volume);
    int  latestSongId() const;
    int  latestSongPosition() const;

Q_SIGNALS:
    void initialized();
    void databaseUpdated(bool changed);
    void playlistChanged();
    void stateChanged(QMpdStatus::State state);
    void modeChanged(QMpdStatus::Mode mode);
    void songChanged(const QMpdSong& song);
    void elapsedSecondsAtStatusChange(int elapsedSeconds);
    void volumeChanged(int volume);

private Q_SLOTS:
    void onInitialized();
    void onDatabaseUpdated(bool changed);
    void onPlaylistChanged();
    void onStateChanged(QMpdStatus::State state);
    void onModeChanged(QMpdStatus::Mode mode);
    void onSongChanged(const QMpdSong& song);
    void onElapsedSecondsAtStatusChange(int elapsedSeconds);
    void onVolumeChanged(int volume);

private:
    QMpdClient(QObject *parent = 0);

private:
    QString        host_;
    int            port_;
    int            timeout_;
    mpd_connection *connection_;
    QMpdMonitor    *monitor_;

    QMpdSongList songList_;
    int id_;
    int pos_;
};

#endif // QMPDCLIENT_H_
