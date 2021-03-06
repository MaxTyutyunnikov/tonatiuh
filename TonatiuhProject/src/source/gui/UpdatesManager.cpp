/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, I�aki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu.
***************************************************************************/

#include <iostream>

#include <QApplication>
#include <QAuthenticator>
#include <QBuffer>
#include <QByteArray>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcessEnvironment>
#include <QSslError>
#include <QUrl>

#include "UpdatesManager.h"

UpdatesManager::UpdatesManager( QString currentVersion )
:QObject(),
 m_networkAccessManager( new QNetworkAccessManager() ),
 m_proxyEnabled( false ),
 m_proxyHostName( QLatin1String( "" ) ),
 m_proxyPort( -1 ),
 m_systemProxyEnabled( false ),
 m_reply( 0 ),
 m_httpRequestAborted( false ),
 m_osType( QLatin1String( "" ) ),
 m_currentVersion( currentVersion ),
 m_latestVersion( QLatin1String( "" ) ),
 m_filelist(),
 m_fileReply( 0 ),
 m_fileRequestAborted( false ),
 m_file( 0 ),
 m_filedownloaded( 0 )
{
	DeleteOldFiles();


#if defined( Q_WS_MAC )
	m_osType = QLatin1String( "mac" );
#endif

#if defined( Q_WS_WIN )
	m_osType = QLatin1String( "win" );
#endif

#if defined( Q_WS_X11 )
	m_osType = QLatin1String( "linux" );
#endif

#if defined( __x86_64__ )
	m_osType += QLatin1String( "_64" );
#endif

}

/*!
 * Destroyes manager object.
 */
UpdatesManager::~UpdatesManager()
{
	delete m_networkAccessManager;
}

/*!
 * Checks for new Tonatiuh updates.
 */
void UpdatesManager::CheckForUpdates()
{
	QUrl url( QLatin1String( "http://tonatiuh.googlecode.com/svn/updates/latest.xml" ) );

    // schedule the request
    m_httpRequestAborted = false;

    /*
    if( m_proxyEnabled  )
    {
    	int port = m_proxyPort;
    	QString hostname = m_proxyHostName;
    	if( m_systemProxyEnabled )
    	{



    		QString urlEnv = QProcessEnvironment::systemEnvironment().value( "http_proxy" );
    		if (!urlEnv.isEmpty() )
    		{
    			QUrl url = QUrl(urlEnv, QUrl::TolerantMode);
    			hostname = url.host();
    			port = url.port();
    		}

    	}
    	QNetworkProxy proxy;
    	proxy.setType( QNetworkProxy::HttpCachingProxy );
    	proxy.setHostName( hostname );
    	proxy.setPort( port );
       	m_networkAccessManager->setProxy( proxy );
    }
    */
    m_reply = m_networkAccessManager->get( QNetworkRequest( url ) );
    connect( m_reply, SIGNAL( finished() ), this, SLOT( CheckLastUpdate() ) );
    connect( m_reply, SIGNAL( readyRead() ), this, SLOT( Read() ) );

}

/*!
 * If the user defined proxy use is enabled returns the host name defined for the proxy.
 * Otherwise, returns an empty hostname.
 */
QString UpdatesManager::GetProxyHostName() const
{
	if( m_proxyEnabled && !m_systemProxyEnabled ) return m_proxyHostName;
	return QString();
}


/*!
 * If the proxy use is enabled returns the port defined for the proxy.
 * Otherwise, returns -1.
 */
int UpdatesManager::GetPorxyPort() const
{
	if( m_proxyEnabled && !m_systemProxyEnabled  )	return m_proxyPort;
	return -1;
}

/*!
 * Returns true if the proxy is enabled for connections.
 */
bool UpdatesManager::IsProxyEnabled() const
{
	return m_proxyEnabled;
}

/*!
 * Returns true if the configuration for the proxy
 * is defined at the system.
 */
bool UpdatesManager::IsSystemProxy() const
{
	return m_systemProxyEnabled;
}

/*!
 * Sets to user defined proxy for connections.
 * Configure the proxy with the \a name for the host name and \a port number.
 */
void UpdatesManager::SetManualProxyConfiguration( QString name, int port)
{
	m_proxyEnabled = true;
	m_systemProxyEnabled = false;
	m_proxyHostName = name;
	m_proxyPort = port;
   	QNetworkProxy proxy;
   	proxy.setType( QNetworkProxy::HttpCachingProxy );
   	proxy.setHostName( m_proxyHostName );
   	proxy.setPort( m_proxyPort );
    QNetworkProxy::setApplicationProxy( proxy );
}

/*!
 * If \a enabled is true, sets proxy enabled for network connections.
 * Otherwise, sets the proxy will not be used.
 */
void UpdatesManager::SetProxyEnabled( bool enabled )
{
	m_proxyEnabled = enabled;
}

/*!
 * Sets to enabled the use of the system proxy configuration.
 */
void UpdatesManager::SetSystemProxyConfiguration()
{
	m_proxyEnabled = true;
	m_systemProxyEnabled = true;

	 QNetworkProxyQuery npq( QUrl( QLatin1String( "http://code.google.com/p/tonatiuh/" ) ) );
	 QList< QNetworkProxy > listOfProxies = QNetworkProxyFactory::systemProxyForQuery( npq );
	 if (listOfProxies.size() > 0 && listOfProxies[0].type() != QNetworkProxy::NoProxy)
		 QNetworkProxy::setApplicationProxy( listOfProxies[0] );
}

/*!
 * Reads latest update version and this version files.
 * If the Sthepanie installed version is oldder, checks if the user want to update to this version.
 */
void UpdatesManager::CheckLastUpdate()
{
	if( m_httpRequestAborted )
	{
		m_reply->deleteLater();
		return;
	}

	// Check file type
	QDomDocument doc( QLatin1String( "XML" ) );
	if( !doc.setContent( m_lastUpdateData, true ) )	return;


	QDomElement root = doc.documentElement();
	if( root.tagName() != QLatin1String( "appname" ) ) return;


	// Loop over main nodes
	m_latestVersion.clear();
	m_filelist.clear();
	m_executableFilelist.clear();

	QDomNode mainnode = root.firstChild();
	while( !mainnode.isNull() )
	{
		QDomNode subnode = mainnode.firstChild();
        if( !subnode.isNull() )
        {
            // Loop over each elements in subnodes
            while( !subnode.isNull() )
            {
                QDomElement e = subnode.toElement();
                if( !e.isNull() )
                {
                	if( e.tagName() == QLatin1String( "version" ) )
                		m_latestVersion = e.attribute( QLatin1String( "value" ), QLatin1String( "" ));

                	if( ( e.tagName() == QLatin1String( "os" )  ) && ( e.attribute( QLatin1String( "value" ), QLatin1String( "" )  ) == m_osType ) )
                	{
                		QDomNode ossubnode = subnode.firstChild();
                		while( !ossubnode.isNull())
                		{
                			QDomElement e = ossubnode.toElement();
                			if( e.tagName() == QLatin1String( "file" ) )
                			{
                				QString type = e.attribute( QLatin1String( "type" ), QLatin1String( "" ) );
                				QString fileName = e.attribute( QLatin1String( "value" ), QLatin1String( "" ) );
                				m_filelist.insertMulti( type, fileName );


                				QString executable = e.attribute( QLatin1String( "executable" ), QLatin1String( "false" ) );
                				if( executable == "true" )	m_executableFilelist.push_back( fileName );
                			}

                			ossubnode = ossubnode.nextSibling();
                		}
                	}


				}
				subnode = subnode.nextSibling();
			}
		}
		mainnode = mainnode.nextSibling();
	}

	// Compare current version with the one on the server
	if( m_currentVersion < m_latestVersion )
	{
		QString text =  QString( tr( "New Version of Tonatiuh is available: %1\nWould you like to download and install it?" )  ).arg( m_latestVersion );


		// Check if user wants to download
		if( QMessageBox::question( 0, QLatin1String( "Tonatiuh" ),
				text, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes )
			== QMessageBox::No )	return;

		// Start Download
		m_filedownloaded = 0;
		UpdateDownload( 0 );
	}
	else{
		// Update success dialog
		QMessageBox::information( 0, QLatin1String("Tonatiuh"), tr("You are using the latest version." ) );
	}
	m_lastUpdateData = QLatin1String( "" );
}

/*!
 * Reads reply data to \a m_lastUpdateData.
 */
void UpdatesManager::Read()
{
	m_lastUpdateData.append( QString::fromLatin1( m_reply->readAll() ) );
}


/*!
 * Reads reply data to \a m_file.
 */
void UpdatesManager::ReadFile()
{
	if( m_file ) m_file->write( m_fileReply->readAll() );
}

/*!
 * Finishes file download.
 */
void UpdatesManager::FileDownloadComplete()
{
	m_file->close();

	if( m_fileRequestAborted )
	{
		if( m_file )
		{
			m_file->close();
			m_file->remove();
			delete m_file;
			m_file = 0;
		}
		m_fileReply->deleteLater();
		return;
	}
	else
		m_filedownloaded++;

	m_fileReply->deleteLater();
	m_fileReply = 0;
	delete m_file;
	m_file = 0;

	UpdateDownload( m_filedownloaded );

}



void UpdatesManager::UpdateDownload( int fileIndex )
{
	QDir currentDir( qApp->applicationDirPath() );
	currentDir.cdUp();
	currentDir.cdUp();

	// Create a temporary folder
	if( !currentDir.exists( QLatin1String( "tmp" ) ) )	currentDir.mkdir( QLatin1String( "tmp" ) );
	QDir tmpDir( currentDir.absoluteFilePath( QLatin1String( "tmp" ) ) );
	int nNewFiles = m_filelist.count( QLatin1String( "new" ) );
	int nReplaceFiles =  m_filelist.count( QLatin1String( "replace" ) );

	if( ( fileIndex < nNewFiles ) & ( fileIndex < m_filelist.size() ) )
	{
		QList< QString>  newFiles = m_filelist.values ( QLatin1String( "new" ) );

		QString urlPath = QString( QLatin1String( "http://tonatiuh.googlecode.com/svn/updates/release-%1/%2/%3" ) ).arg( m_latestVersion, m_osType, newFiles[fileIndex] );
		QString saveFileName = tmpDir.absoluteFilePath( newFiles[fileIndex] );

		DownloadFile( urlPath, saveFileName );
	}
	else if( ( fileIndex >= nNewFiles ) && ( fileIndex < ( nReplaceFiles + nNewFiles ) ) && ( fileIndex < m_filelist.size() ) )
	{
		int replaceFileIndex = fileIndex - nNewFiles;
		QList< QString>  replaceFiles = m_filelist.values ( QLatin1String( "replace" ) );

		QString urlPath = QString( QLatin1String( "http://tonatiuh.googlecode.com/svn/updates/release-%1/%2/%3" ) ).arg( m_latestVersion, m_osType, replaceFiles[replaceFileIndex] );
		QString saveFileName = tmpDir.absoluteFilePath( replaceFiles[replaceFileIndex] );

		DownloadFile( urlPath, saveFileName );
	}
	else if( m_filedownloaded == (  nReplaceFiles + nNewFiles  ) )
	{
		//New files
		QList<QString> newFiles = m_filelist.values ( QLatin1String( "new" ) );

		for( int i = 0; i < newFiles.size(); i++ )
		{
			QString currentFileName = currentDir.absoluteFilePath( newFiles[i] );
			QString newFileName = tmpDir.absoluteFilePath( newFiles[i] );


			QFileInfo currentFileInfo( currentFileName );
			currentDir.relativeFilePath ( currentFileInfo.absolutePath() );
			currentDir.mkpath( currentDir.relativeFilePath ( currentFileInfo.absolutePath() ) );

			QFile newFile( newFileName );
			newFile.rename( currentFileName );
			if( m_executableFilelist.contains( newFiles[i] ) )	newFile.setPermissions( QFile::ExeOwner | QFile::ReadOwner	| QFile::WriteOwner );
		}

		//Replace files. Backup old files and rename them
		QList<QString>  replaceFiles = m_filelist.values ( QLatin1String( "replace" ) );

		// Backup old files and rename them
		for( int i = 0; i < replaceFiles.size(); i++ )
		{
			QString currentFileName = currentDir.absoluteFilePath( replaceFiles[i] );
			QString newFileName = tmpDir.absoluteFilePath( replaceFiles[i] );
			QString oldFileName = currentFileName + QLatin1String( ".old" );

			QFile currentFile( currentFileName );
			currentFile.rename( oldFileName );

			QFile newFile( newFileName );
			newFile.rename( currentFileName );
			if( m_executableFilelist.contains( replaceFiles[i] ) )	newFile.setPermissions( QFile::ExeOwner | QFile::ReadOwner	| QFile::WriteOwner );

		}

		//Remove files. Backup old files
		QList< QString>  removeFiles = m_filelist.values ( QLatin1String( "remove" ) );

		// Backup old files and rename them
		for( int i = 0; i < removeFiles.size(); i++ )
		{
			QString currentFileName = currentDir.absoluteFilePath( removeFiles[i] );
			QString oldFileName = currentFileName + QLatin1String( ".old" );

			QFile currentFile( currentFileName );
			currentFile.rename( oldFileName );
		}

		//Removes tmp dir
		RemoveDir( tmpDir.absolutePath() );

		QMessageBox::information( 0,  QLatin1String( "Tonatiuh"), tr("Please restart Tonatiuh for updates to take effect." ) );
	}
 }


/*!
 * Removes \a dirName directory old files.
 */
void UpdatesManager::DeleteDirOldFiles( QString dirName )
{
	QDir currentDir( dirName );
	currentDir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
	currentDir.setSorting( QDir::DirsFirst  | QDir::Name );

	QStringList fileList = currentDir.entryList();
	for( int i = 0; i < fileList.size(); ++i )
	{
		QFileInfo fileOrDir( currentDir, fileList[i] );
		if( fileOrDir.isDir() )	DeleteDirOldFiles( currentDir.absoluteFilePath( fileList[i] ) );
		else if( ( !fileOrDir.isDir() ) && (  fileOrDir.suffix() == QLatin1String( "old" ) ) )
		{
			QFile oldFile( currentDir.absoluteFilePath( fileList[i] ) );
			oldFile.remove();
		}
	}

}

/*!
 * Removes application old files.
 */
void UpdatesManager::DeleteOldFiles()
{
	QDir appDir( qApp->applicationDirPath() );
	appDir.cdUp();
	appDir.cdUp();
	DeleteDirOldFiles( appDir.absolutePath() );
}

/*!
 * Downloads fileName file from updates server.
 */
void UpdatesManager::DownloadFile( QString urlPath, QString saveFileName  )
{
	QUrl url( urlPath );
	if( QFile::exists( saveFileName ) )
	{
		if( QMessageBox::question( 0, QLatin1String("Tonatiuh"), tr("There already exists a file called %1 in "
			"the current directory. Overwrite?").arg( saveFileName ),
			QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel )
			== QMessageBox::Cancel )
			return;
		QFile::remove( saveFileName );
	}
	else
	{
		QFileInfo fileInfo( saveFileName );
		if( !fileInfo.dir().exists() )
		{
			QDir currentDir( qApp->applicationDirPath() );
			currentDir.cdUp();
			currentDir.cdUp();

			currentDir.mkpath( fileInfo.dir().path() );
		}

	}

	m_file = new QFile( saveFileName );

	if( !m_file->open( QIODevice::WriteOnly ) )
	{
		QMessageBox::information( 0, QLatin1String( "Tonatiuh" ),
			tr("Unable to save the file %1: %2.").arg( saveFileName, m_file->errorString() ) );
		delete m_file;
		m_file = 0;
		return;
	}

	m_fileRequestAborted = false ;

	m_fileReply = m_networkAccessManager->get( QNetworkRequest( url ) );
	connect( m_fileReply, SIGNAL( finished() ), this, SLOT( FileDownloadComplete() ) );
	connect( m_fileReply, SIGNAL( readyRead() ), this, SLOT( ReadFile() ) );

}

/*!
 * Removes the file hierarchy rooted by \a dirName.
 */
void UpdatesManager::RemoveDir( QString dirName )
{
	QDir currentDir( dirName );
	currentDir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
	currentDir.setSorting( QDir::DirsFirst  | QDir::Name );

	QStringList fileList = currentDir.entryList( );

	for( int i = 0; i < fileList.size(); ++i )
	{
		QFileInfo fileOrDir( currentDir, fileList[i] );
		if( fileOrDir.isDir() )
		{
			RemoveDir( fileOrDir.filePath() );
			currentDir.rmdir( fileList[i] );
		}
		else currentDir.remove( fileList[i] );
	}
	currentDir.cdUp();

	currentDir.rmdir( dirName );
}

