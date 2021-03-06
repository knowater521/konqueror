/* This file is part of the KDE project
   Copyright (C) 1998, 1999 David Faure <faure@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef _konq_misc_h
#define _konq_misc_h

#include "konqprivate_export.h"

#include <QUrl>

#include <kparts/browserextension.h>

class KonqMainWindow;
class KonqView;

namespace KonqMisc
{
/**
 * Creates a new window from the history of a view, copies the history
 * @param view the History is copied from this view
 * @param steps Restore currentPos() + steps
 */
KonqMainWindow *newWindowFromHistory(KonqView *view, int steps);

/**
 * Applies the URI filters to @p url, and convert it to a QUrl.
 *
 * @p parent is used in case of a message box.
 * @p url to be filtered.
 * @p currentDirectory the directory to use, in case the url is relative.
 */
QUrl konqFilteredURL(KonqMainWindow *parent, const QString &url, const QUrl &currentDirectory = QUrl());

/**
* These are some helper functions to encode/decode session filenames. The
* problem here is that windows doesn't like files with ':' inside.
*/

QString encodeFilename(QString filename);

QString decodeFilename(QString filename);
}

#endif
