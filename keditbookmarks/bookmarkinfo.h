// vim: set ts=4 sts=4 sw=4 et:
/* This file is part of the KDE project
   Copyright (C) 2000 David Faure <faure@kde.org>
   Copyright (C) 2002-2003 Alexander Kellett <lypanov@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License version 2 as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __bookmarkinfo_h
#define __bookmarkinfo_h

#include <kbookmark.h>
#include <qwidget.h>
#include <klineedit.h>


class BookmarkLineEdit : public KLineEdit {
    Q_OBJECT
public:
    BookmarkLineEdit( QWidget * );
public slots:
    virtual void cut ();
};

class BookmarkInfoWidget : public QWidget {
    Q_OBJECT
public:
    BookmarkInfoWidget(QWidget * = 0, const char * = 0);
    void showBookmark(const KBookmark &bk);
    void saveBookmark(const KBookmark &bk);
    bool connected() { return m_connected; };
    void setConnected(bool b) { m_connected = b; };
public slots:
    void slotTextChangedURL(const QString &);
    void slotTextChangedTitle(const QString &);
    void slotTextChangedComment(const QString &);
signals:
    void updateListViewItem();
private:
    BookmarkLineEdit *m_title_le, *m_url_le,
        *m_comment_le;
    KLineEdit  *m_visitdate_le, *m_credate_le,
              *m_visitcount_le;
    KBookmark m_bk;
    bool m_connected;
};

#endif
