#pragma once

#include "patcherlib_global.h"
#include "UiController.h"
#include <QObject>

class QAbstractItemModel;
class DatabaseProvider;
class BuilderHandler;
class InstallerHandler;
class BuildListModel;
class InstallListModel;
class DependenciesListModel;

class PATCHERLIB_EXPORT UiController : public QObject
{
	Q_OBJECT

public:
	explicit UiController(QObject *parent = Q_NULLPTR);
	BuildListModel* getBuildListModel() const;
	InstallListModel* getInstallListModel() const;
	QAbstractItemModel* getDependenciesListModel() const;
private:
	DatabaseProvider *databaseProvider;
	BuilderHandler *builderHandler;
	InstallerHandler *installerHandler;
	BuildListModel *buildListModel;
	InstallListModel *installListModel;
	DependenciesListModel *dependenciesListModel;
private slots:
	void connectToDatabase(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port);
	void disconnectFromDatabase();
	void testDependencies();
	void installPatch();
	void buildPatch();
	void openPatchFile(const QString &filePath);
signals:
	void connectedToDatabase(const QString &database, const QString &user, const QString &password,
		const QString &server, const int port);
	void notConnectedToDatabase(const QString &errorMessage);
	void disconnectedFromDatabase();
	void testPassed();
	void patchInstalled();
	void patchNotInstalled(const QString &errorMessage);
	void patchBuilt();
	void patchNotBuilt(const QString &errorMessage);
	void patchFileOpened();
	void patchFileNotOpened(const QString &errorMessage);
};
