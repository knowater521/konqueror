/* This file is part of the KDE project
   Copyright (C) 1998, 1999 David Faure <faure@kde.org>

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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <qdir.h>

#include <kapp.h>
#include <kurl.h>
#include <ksimpleconfig.h>
#include <klineeditdlg.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <kdesktopfile.h>

#include <kio_interface.h>
#include <kio_job.h>
#include <kuserpaths.h>

#include <opUIUtils.h>

#include "kpropsdlg.h"
#include "knewmenu.h"

QStringList * KNewMenu::templatesList = 0L;
int KNewMenu::templatesVersion = 0;

KNewMenu::KNewMenu( OpenPartsUI::Menu_ptr menu) : menuItemsVersion(0)
{
    if ( CORBA::is_nil( menu ) )
    {
      m_bUseOPMenu = false;
      m_pMenu = new QPopupMenu;
      m_vMenu = 0L;

      QObject::connect( m_pMenu, SIGNAL(activated(int)),
                        this, SLOT(slotNewFile(int)) );
      QObject::connect( m_pMenu, SIGNAL(aboutToShow()),
                        this, SLOT(slotCheckUpToDate()) );
    }
    else
    {
      m_bUseOPMenu = true;
      m_vMenu = OpenPartsUI::Menu::_duplicate( menu );
      m_pMenu = 0L;
    }

    fillMenu();
}

void KNewMenu::setPopupFiles(QStringList & _files)
{
  popupFiles = _files;
}

void KNewMenu::slotCheckUpToDate( )
{
    if (menuItemsVersion < templatesVersion)
    {
        fillMenu();
        menuItemsVersion = templatesVersion;
    }
}

void KNewMenu::fillMenu()
{
    if ( m_bUseOPMenu && CORBA::is_nil( m_vMenu ) )
      return;

    if (!templatesList) { // No templates list up to now
        templatesList = new QStringList();
        fillTemplates();
        menuItemsVersion = templatesVersion;
    }

    if ( m_bUseOPMenu )
    {
      m_vMenu->clear();
      QString text = i18n( "Folder" );
      m_vMenu->insertItem( text, CORBA::Object::_nil(), "", 0 );
    }
    else
    {
      m_pMenu->clear();
      m_pMenu->insertItem( i18n( "Folder" ) );
    }

    QStringList::Iterator templ = templatesList->begin(); // skip 'Folder'
    for ( ++templ; templ != templatesList->end(); ++templ)
    {
        KSimpleConfig config(KUserPaths::templatesPath() + *templ, true);
        config.setDesktopGroup();
        QString name = *templ;
        if ( name.right(8) == ".desktop" )
            name.truncate( name.length() - 8 );
        if ( name.right(7) == ".kdelnk" )
            name.truncate( name.length() - 7 );
        if ( m_bUseOPMenu )
	  {
	    QString text = config.readEntry("Name", name);
	    m_vMenu->insertItem( text, CORBA::Object::_nil(), "", 0 );
	  }    
	else
	  m_pMenu->insertItem( config.readEntry("Name", name ) );
    }
}

void KNewMenu::fillTemplates()
{
    templatesVersion++;

    templatesList->clear();
    templatesList->append( "Folder" );

    QDir d( KUserPaths::templatesPath() );
    const QFileInfoList *list = d.entryInfoList();
    if ( list == 0L )
        warning(i18n("ERROR: Template does not exist '%s'"),
		KUserPaths::templatesPath().ascii());
    else
    {
	QFileInfoListIterator it( *list );      // create list iterator
	QFileInfo *fi;                          // pointer for traversing

	while ( ( fi = it.current() ) != 0L )
	{
	    if ( strcmp( fi->fileName(), "." ) != 0 &&
		 strcmp( fi->fileName(), ".." ) != 0 &&
                 !fi->isDir() && fi->isReadable())
	    {
		templatesList->append( fi->fileName() );
	    }
	    ++it;                               // goto next list element
	}
    }
}

void KNewMenu::slotNewFile( int _id )
{
    if ( m_bUseOPMenu )
    {
      if ( CORBA::is_nil( m_vMenu ) )
        return;

      if ( m_vMenu->text( (CORBA::Long)_id ) == 0)
	return;
    }
    else if ( m_pMenu->text( _id ) == 0 )
      return;

    m_sDest.clear();
    m_sDest.setAutoDelete(true);

    QString sFile;
    
    if ( m_bUseOPMenu )
      sFile = *(templatesList->at( m_vMenu->indexOf( _id ) ));
    else
      sFile = *(templatesList->at( m_pMenu->indexOf( _id ) ));
    kdebug(0, 1203, "sFile = %s",sFile.ascii());
      
    QString sName ( sFile );
    QString text, value;

    if ( sName != "Folder" ) {
      QString x = KUserPaths::templatesPath() + sFile;
      if (!QFile::exists(x)) {
          kdebug(KDEBUG_WARN, 1203, "%s doesn't exist", x.ascii());
          KMessageBox::sorry( 0L, i18n("Source file doesn't exist anymore !"));
          return;
      }
      if ( KDesktopFile::isDesktopFile( x ) )
      {
          QStringList::Iterator it = popupFiles.begin();
          for ( ; it != popupFiles.end(); ++it )
          {
              (void) new PropertiesDialog( x, *it, sFile );
          }
          return; // done, exit.
      }

        /*
      KSimpleConfig config(x, true);
      config.setDesktopGroup();
      if ( sName.right(8) == ".desktop" )
	sName.truncate( sName.length() - 8 );
      if ( sName.right(7) == ".kdelnk" )
	sName.truncate( sName.length() - 7 );
      sName = config.readEntry("Name", sName);
        */

      // Not a desktop file, nor a folder.
      text = i18n("New ") + sName + ":";
      value = sFile;
    } else {
      value = "";
      text = i18n("New Folder :");
    }

    KLineEditDlg l( text, value, 0L, true );
    if ( l.exec() )
    {
	QString name = l.text();
	if ( name.length() == 0 )
	    return;

        QStringList::Iterator it = popupFiles.begin();
	if ( sFile =="Folder" )
	{
            for ( ; it != popupFiles.end(); ++it )
	    {
     	      KIOJob * job = new KIOJob;
	      KURL url( *it );
	      url.addPath( name );
	      QString u2 = url.url();
	      // --- Sven's check if this is global apps/mime start ---
	      // This is a bug fix for bug report from A. Pour from
	      // Mietierra (sp?)
	      // User wants to create dir in global mime/apps dir;

	      //	      Kfm::setUpDest(&u2); // this checks & repairs destination
	      // --- Sven's check if global apps/mime end ---
	      job->mkdir( u2, -1 );
            }
	}
	else
	{
	    QString src = KUserPaths::templatesPath() + sFile;
            for ( ; it != popupFiles.end(); ++it )
            {
                KIOJob * job = new KIOJob( );
		KURL dest( *it );
		dest.addPath( name );

		//QString u2 = url.url();
		// debugT("Command copy '%s' '%s'\n",src.ascii(),dest.ascii());

		// --- Sven's check if this is global apps/mime start ---
		// This is a bug fix for bug report from A. Pour from
		// Mietierra (sp?)
		// User wants to create new entry in global mime/apps dir;
		//		Kfm::setUpDest(&u2);
		// --- Sven's check if global apps/mime end ---

                /*
                if ( ( sFile.right(7) == ".kdelnk" ) ||
		     ( sFile.right(8) == ".desktop" ) )
                {
                  m_sDest.insert( job->id(), new QString( dest.url() ) );
                  connect(job, SIGNAL( finished( int ) ), this, SLOT( slotCopyFinished( int ) ) );
                }
                */

                job->copy( src, dest.url() );
            }
	}
    }
}

    /*
void KNewMenu::slotCopyFinished( int id )
{
  // Now open the properties dialog on the file, as it was a 
  // desktop entry
  (void) new PropertiesDialog( m_sDest.find( id )->ascii() );
  m_sDest.remove( id );
}
*/

#include "knewmenu.moc"
