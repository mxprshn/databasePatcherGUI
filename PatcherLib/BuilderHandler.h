#pragma once

#include <QObject>

class QString;
class QIODevice;
class QProcess;

class BuilderHandler : public QObject
{
	Q_OBJECT

public:
	// Error handling???
	// And should I disconnect connections?
	static void setOutputDevice(QIODevice &newDevice);
	static bool buildPatch(const QString &database, const QString &user, const QString &password,
		const QString &server, int port, const QString &patchDir, const QString &buildListDir);
private:
	const static QString program;
	const static QString templatesPath;
	static QIODevice *outputDevice;
};
