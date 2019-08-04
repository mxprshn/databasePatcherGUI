#include "InstallerHandler.h"
#include <QBitArray>
#include <QProcess>
#include <QIODevice>

QString InstallerHandler::program = "PatchInstaller_exe.exe";
QProcess InstallerHandler::installerProcess;
QIODevice *InstallerHandler::outputDevice;
// Init device???

void InstallerHandler::setProgram(const QString &newProgram)
{
	program = newProgram;
}

void InstallerHandler::setOutputDevice(QIODevice &newDevice)
{
	outputDevice = &newDevice;
}

bool InstallerHandler::installPatch(const QString &database, const QString &user, const QString &password,
	const QString &server, int port, const QString &path)
{
	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(database).arg(user).arg(password).arg(server)
		.arg(port);
	const QStringList arguments = { connectionInfo, "install", path };
	// Add connection to output stream!
	connect(&installerProcess, &QProcess::readyReadStandardError, [] ()
	{
		if (outputDevice)
		{
			outputDevice->write(installerProcess.readAllStandardError());
		}
	});

	installerProcess.start(program, arguments);

	if (!installerProcess.waitForStarted())
	{
		return false;
	}

	if (!installerProcess.waitForFinished() || installerProcess.exitCode() != 0)
	{
		return false;
	}

	return true;
}

QBitArray InstallerHandler::checkDependencies(const QString &database, const QString &user, const QString &password,
	const QString &server, int port, const QString &path, bool &isSuccessful)
{
	QBitArray checkResult;

	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(database).arg(user).arg(password).arg(server)
		.arg(port);
	const QStringList arguments = { connectionInfo, "check", path };

	// Not sure if it is ok when process destructed.
	// Fix code table for output
	connect(&installerProcess, &QProcess::readyReadStandardError, [] ()
	{
		if (outputDevice)
		{
			outputDevice->write(installerProcess.readAllStandardError());
		}
	});

	installerProcess.start(program, arguments);

	if (!installerProcess.waitForStarted())
	{
		isSuccessful = false;
		return checkResult;
	}

	// Add processing of the return value!

	if (!installerProcess.waitForFinished())
	{
		isSuccessful = false;
		return checkResult;
	}

	installerProcess.setReadChannel(QProcess::ProcessChannel::StandardOutput);
	QByteArray readData = installerProcess.readAll();
	checkResult.resize(readData.count());

	for (auto i = 0; i < checkResult.count(); ++i)
	{
		switch (readData[i])
		{
			case '0':
			{
				checkResult[i] = false;
				break;
			}
			case '1':
			{
				checkResult[i] = true;
				break;
			}
			default:
			{
				isSuccessful = false;
				checkResult.clear();
				return checkResult;
			}
		}
	}

	isSuccessful = true;
	return checkResult;
}