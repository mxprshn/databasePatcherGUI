#pragma once

#include <QObject>

class QBitArray;
class QString;
class QStringList;
class QIODevice;
class QProcess;

class InstallerHandler : QObject
{
	Q_OBJECT

public:
	// Error handling???
	// And should I disconnect connections?
	static void setProgram(const QString &newProgram);
	static void setOutputDevice(QIODevice &newDevice);
	static bool installPatch(const QString &database, const QString &user, const QString &password,
		const QString &server, int port, const QString &path);
	static QBitArray checkDependencies(const QString &database, const QString &user, const QString &password,
		const QString &server, int port, const QString &path);
private:
	static QString program;
	static QIODevice *outputDevice;
	static QProcess installerProcess;
};
