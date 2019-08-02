#include "BuilderHandler.h"
#include <QProcess>
#include <QIODevice>

QString BuilderHandler::program = "PatchBuilder_exe.exe";
QString BuilderHandler::templatesPath = "Templates.ini";
QProcess BuilderHandler::builderProcess;
QIODevice *BuilderHandler::outputDevice;

void BuilderHandler::setOutputDevice(QIODevice &newDevice)
{
	outputDevice = &newDevice;
}

bool BuilderHandler::buildPatch(const QString& database, const QString& user, const QString& password
	, const QString& server, int port, const QString &patchDir, const QString &buildListDir)
{
	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(database).arg(user).arg(password).arg(server)
		.arg(port);
	const QStringList arguments = { "-d", patchDir, "-p", buildListDir, "-c", connectionInfo, "-t", templatesPath };

	connect(&builderProcess, &QProcess::readyReadStandardOutput, []()
	{
		if (outputDevice)
		{
			outputDevice->write(builderProcess.readAllStandardOutput());
		}
	});

	connect(&builderProcess, &QProcess::readyReadStandardError, []()
	{
		if (outputDevice)
		{
			outputDevice->write(builderProcess.readAllStandardError());
		}
	});

	builderProcess.start(program, arguments);

	if (!builderProcess.waitForStarted())
	{
		return false;
	}

	// Add exit code processing

	if (!builderProcess.waitForFinished() || builderProcess.exitCode() != 0)
	{
		return false;
	}

	return true;
}
