/* This file is part of the KDE project
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

#include <kdebug.h>
#include <klocale.h>
#include <kapplication.h>

#include "bookmarkiterator.h"
#include "listview.h"
#include "toplevel.h"
#include "updater.h"

#include "favicons.h"

FavIconsItrHolder *FavIconsItrHolder::s_self = 0;

FavIconsItrHolder::FavIconsItrHolder() 
   : BookmarkIteratorHolder() {
   // do stuff
}

void FavIconsItrHolder::doItrListChanged() {
   KEBApp::self()->setCancelFavIconUpdatesEnabled(m_itrs.count() > 0);
}

/* -------------------------- */

FavIconsItr::FavIconsItr(QValueList<KBookmark> bks)
   : BookmarkIterator(bks) {

   m_updater = 0;
   m_done = false;
}

FavIconsItr::~FavIconsItr() {
   delete m_updater;
   if (!m_done && curItem()) {
      curItem()->restoreStatus();
   }
}

void FavIconsItr::slotDone(bool succeeded) {
   kdDebug() << "FavIconsItr::slotDone()" << endl;
   m_done = true;
   curItem()->setTmpStatus(succeeded ? i18n("OK") : i18n("No favicon found"));
   delayedEmitNextOne();
}

bool FavIconsItr::isBlahable(const KBookmark &bk) {
   // quick hack. fix later on
   return (!bk.isGroup() && !bk.isSeparator() 
       && (bk.url().protocol().startsWith("http")) );
}

void FavIconsItr::doBlah() {
   kdDebug() << "FavIconsItr::doBlah()" << endl;
   m_done = false;
   curItem()->setTmpStatus(i18n("Updating favicon..."));
   if (!m_updater) {
      m_updater = new FavIconUpdater(kapp, "FavIconUpdater");
      connect(m_updater, SIGNAL( done(bool) ),
              this,      SLOT( slotDone(bool) ) );
   }
   m_updater->downloadIcon(m_book);
   // TODO - a single shot timeout?
}

#include "favicons.moc"
