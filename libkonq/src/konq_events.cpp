/* This file is part of the KDE project
   Copyright (C) 2000      Simon Hausmann <hausmann@kde.org>
   Copyright (C) 2000-2007 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "konq_events.h"

const char *const KonqFileSelectionEvent::s_fileItemSelectionEventName = "Konqueror/FileSelection";
const char *const KonqFileMouseOverEvent::s_fileItemMouseOverEventName = "Konqueror/FileMouseOver";

KonqFileSelectionEvent::KonqFileSelectionEvent(const KFileItemList &selection, KParts::ReadOnlyPart *part)
    : KParts::Event(s_fileItemSelectionEventName), m_selection(selection), m_part(part)
{
}

KonqFileSelectionEvent::~KonqFileSelectionEvent()
{
}

KonqFileMouseOverEvent::KonqFileMouseOverEvent(const KFileItem &item, KParts::ReadOnlyPart *part)
    : KParts::Event(s_fileItemMouseOverEventName), m_item(item), m_part(part)
{
}

KonqFileMouseOverEvent::~KonqFileMouseOverEvent()
{
}
