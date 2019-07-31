#include "BuilderHandler.h"
#include <QProcess>
#include <QIODevice>

QString BuilderHandler::program = "PatchBuilder_exe.exe";
QProcess BuilderHandler::builderProcess;
QIODevice *BuilderHandler::outputDevice;

void BuilderHandler::setProgram(const QString &newProgram)
{
	program = newProgram;
}

void BuilderHandler::setOutputDevice(QIODevice &newDevice)
{
	outputDevice = &newDevice;
}

bool BuilderHandler::buildPatch(const QString& database, const QString& user, const QString& password
	, const QString& server, int port, const QString& path)
{
	// This part should be adjusted to Timur's builder
	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(database).arg(user).arg(password).arg(server)
		.arg(port);
	const QStringList arguments = { connectionInfo, "install", path };

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

	if (!builderProcess.waitForFinished() || builderProcess.exitCode() != 0)
	{
		return false;
	}

	return true;
}
