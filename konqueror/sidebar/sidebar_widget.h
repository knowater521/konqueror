/***************************************************************************
                               sidebar_widget.h
                             -------------------
    begin                : Sat June 2 16:25:27 CEST 2001
    copyright            : (C) 2001 Joseph Wenninger
    email                : jowenn@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef _SIDEBAR_WIDGET_
#define _SIDEBAR_WIDGET_
#include <kdockwidget.h>
#include <qptrvector.h>
#include <kurl.h>
#include <ktoolbar.h>
#include <kparts/part.h>
#include <qstring.h>
#include "konqsidebarplugin.h"
#include <kmultitabbar.h>
#include <qguardedptr.h>

class KDockWidget;
class QHBoxLayout;
class QSplitter;

class ButtonInfo: public QObject
{
	Q_OBJECT
	public:
	ButtonInfo(const QString& file_, class KDockWidget *dock_,
			const QString &url_,const QString &lib,
			const QString &dispName_, const QString &iconName_,
			QObject *parent)
		: QObject(parent), file(file_), dock(dock_), URL(url_),
		libName(lib), displayName(dispName_), iconName(iconName_)
		{
		copy = cut = paste = trash = del = shred = rename =false;
		}

	~ButtonInfo() {}

	class QString file;
	class KDockWidget *dock;
	class KonqSidebarPlugin *module;
	class QString URL;
	class QString libName;
	class QString displayName;
	class QString iconName;
	bool copy;
	bool cut;
	bool paste;
	bool trash;
	bool del;
	bool shred;
        bool rename;
};


class addBackEnd: public QObject
{
	Q_OBJECT
	public:
		addBackEnd(QObject *parent,class QPopupMenu *addmenu, const char *name=0);
		~addBackEnd(){;}
	private:
		QGuardedPtr<class QPopupMenu> menu;
		QPtrVector<QString> libNames;
		QPtrVector<QString> libParam;
		void doRollBack();
	protected slots:
		void aboutToShowAddMenu();
		void activatedAddMenu(int);
	signals:
		void updateNeeded();
		void initialCopyNeeded();
};

class Sidebar_Widget: public QWidget
{
  Q_OBJECT
  public:
	Sidebar_Widget(QWidget *parent, KParts::ReadOnlyPart *par,
						const char * name);
	~Sidebar_Widget();
	bool openURL(const class KURL &url);
	void stdAction(const char *handlestd);
	//virtual KParts::ReadOnlyPart *getPart();
	KParts::BrowserExtension *getExtension();
	

  static QString PATH;

  public slots:
	void addWebSideBar(const KURL& url, const QString& name);

  private:
	class KDockArea *Area;
	class KMultiTabBar *ButtonBar;
        QPtrVector<ButtonInfo> Buttons;
	bool addButton(const QString &desktoppath,int pos=-1);
	bool createView(ButtonInfo *data);
	//class KDockWidget *mainW;
	int latestViewed;
	class KonqSidebarPlugin *loadModule(QWidget *par,QString &desktopName,QString lib_name,ButtonInfo *bi);
	KURL storedUrl;
	bool stored_url;
	KParts::ReadOnlyPart *partParent;
	//ButtonInfo* getActiveModule();
	bool singleWidgetMode;
	bool showTabsRight;
	bool showExtraButtons;
	void readConfig();
	QHBoxLayout *myLayout;
	class QStringList visibleViews;
	class KPopupMenu *buttonPopup;
	class QPopupMenu *Menu;
	int popupFor;
	void initialCopy();
	void doLayout();
	void connectModule(QObject *mod);
	int savedWidth;
	bool somethingVisible;
	void collapseExpandSidebar();
	KDockWidget *dummyMainW;
	bool doEnableActions();
	bool noUpdate;
	bool deleting;
	bool m_initial;
  protected:
	virtual bool eventFilter(QObject*,QEvent*);
	virtual void mousePressEvent(QMouseEvent*);
	friend class ButtonInfo;
	QGuardedPtr<ButtonInfo>activeModule;
  protected slots:
	void showHidePage(int value);
	void createButtons();
	void finishRollBack();
	void activatedMenu(int id);
	void buttonPopupActivate(int);
  	void dockWidgetHasUndocked(KDockWidget*);
	void aboutToShowConfigMenu();
	void slotDeleted();
	void saveOpenViews();
  signals:
	void started(KIO::Job *);
	void completed();
	void fileSelection(const KFileItemList& iems);
	void fileMouseOver(const KFileItem& item);
  public:
	/* interface KonqSidebar_PluginInterface*/
	KInstance  *getInstance();
//        virtual void showError(QString &);      for later extension
//        virtual void showMessage(QString &);    for later extension
	/* end of interface implementation */


 /* The following public slots are wrappers for browserextension fields */
 public slots:
	void openURLRequest( const KURL &url, const KParts::URLArgs &args = KParts::URLArgs() );
  	void createNewWindow( const KURL &url, const KParts::URLArgs &args = KParts::URLArgs() );
	void createNewWindow( const KURL &url, const KParts::URLArgs &args,
             const KParts::WindowArgs &windowArgs, KParts::ReadOnlyPart *&part );

	void popupMenu( const QPoint &global, const KFileItemList &items );
  	void popupMenu( KXMLGUIClient *client, const QPoint &global, const KFileItemList &items );
	void popupMenu( const QPoint &global, const KURL &url,
		const QString &mimeType, mode_t mode = (mode_t)-1 );
	void popupMenu( KXMLGUIClient *client,
		const QPoint &global, const KURL &url,
		const QString &mimeType, mode_t mode = (mode_t)-1 );
	void enableAction( const char * name, bool enabled );

protected:
	void customEvent(QCustomEvent* ev);
	QSplitter *splitter() const;
};

#endif
