/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>

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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "konq_actions.h"

#include <qlabel.h>

#include <ktoolbar.h>

KonqComboAction::KonqComboAction( const QString& text, int accel,
			      QObject* parent, const char* name )
    : KSelectAction( text, accel, parent, name )
{
}

KonqComboAction::KonqComboAction( const QString& text, int accel,
			      QObject* receiver, const char* slot, QObject* parent,
			      const char* name )
    : KSelectAction( text, accel, parent, name )
{
}

KonqComboAction::KonqComboAction( const QString& text, const QIconSet& pix, int accel,
			      QObject* parent, const char* name )
    : KSelectAction( text, pix, accel, parent, name )
{
}

KonqComboAction::KonqComboAction( const QString& text, const QIconSet& pix, int accel,
			      QObject* receiver, const char* slot, QObject* parent,
			      const char* name )
    : KSelectAction( text, pix, accel, receiver, slot, parent, name )
{
}

KonqComboAction::KonqComboAction( QObject* parent, const char* name )
    : KSelectAction( parent, name )
{
}

int KonqComboAction::plug( QWidget *w )
{
  if ( w->inherits( "KToolBar" ) )
  {
    QLabel *label = new QLabel( plainText(), w );
    label->adjustSize();
    ((KToolBar *)w)->insertWidget( 15234, label->width(), label );
  }

  int container = KSelectAction::plug( w );
  
  if ( container != -1 && w->inherits( "KToolBar" ) )
  {
    ((KToolBar *)w)->setItemAutoSized( menuId( container ), true );
    ((KToolBar *)w)->getCombo( menuId( container ) )->setAutoCompletion( true );
    ((KToolBar *)w)->getCombo( menuId( container ) )->setMaxCount( 10 );
    ((KToolBar *)w)->getCombo( menuId( container ) )->setInsertionPolicy( QComboBox::AfterCurrent );
  }    
    
  return container;
}

void KonqComboAction::unplug( QWidget *w )
{
  if ( w->inherits( "KToolBar" ) )
  {
    int idx = findContainer( w );
    QWidget *l;
    if ( ( l = ((KToolBar *)w)->getWidget( menuId( idx ) - 1 ) )->inherits( "QLabel" ) )
    {
      ((KToolBar *)w)->removeItem( menuId( idx ) - 1 );
      delete l;
    }	
  }
  KSelectAction::unplug( w );
}

void KonqComboAction::changeItem( const QString &text, int index )
{
#warning Torben, please read :-) (Simon)
  // Hey Torben, if you ever read this :-) , then please consider implemeting
  // QSelectAction::cangeItem() in Qt, as this "hack" breaks m_list !!
  // (Simon)

  if ( index == -1 )
    index = currentItem();

  // this *always* creates a QPopupMenu, even if we don't use it :-( , but
  // the problem is that QSelectAction doesn't allow us to check for m_menu..
  QPopupMenu *popup = popupMenu();

  popup->changeItem( index, text );
  
  int len = containerCount();
  for ( int i = 0; i < len; i++ )
  {
    QWidget *w = container( i );
    
    if ( w->inherits( "QToolBar" ) )
    {
      QWidget *r = representative( i );
      
      if ( !r->inherits( "QComboBox" ) )
        continue;
	
      ((QComboBox *)r)->changeItem( text, index );
    }
    else if ( w->inherits( "KToolBar" ) )
    {
      ((KToolBar *)w)->getCombo( menuId( i ) )->changeItem( text, index );
    }
    else if ( w->inherits( "QActionWidget" ) )
      ((QActionWidget *)w)->updateAction( this );
  }
}

#include "konq_actions.moc"
