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

QBitArray InstallerHandler::testDependencies(const QString &database, const QString &user, const QString &password,
	const QString &server, int port, const QString &path)
{
	QBitArray testResult(1, true);

	const auto connectionInfo = QString("%1:%2:%3:%4:%5").arg(database).arg(user).arg(password).arg(server)
		.arg(port);
	const QStringList arguments = { connectionInfo, "check", path };

	// Not sure if it is ok when process destructed.
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
		testResult[0] = false;
		return testResult;
	}

	if (!installerProcess.waitForFinished() || installerProcess.exitCode() != 0)
	{
		testResult[0] = false;
		return testResult;
	}
	
	installerProcess.setReadChannel(QProcess::ProcessChannel::StandardOutput);
	QByteArray readData = installerProcess.readAllStandardOutput();
	testResult.resize(readData.count() + 1);

	for (auto i = 1; i <= testResult.count(); ++i)
	{
		switch (readData[i])
		{
			case '0':
			{
				testResult[i] = false;
				break;
			}
			case '1':
			{
				testResult[i] = true;
				break;
			}
			default:
			{
				testResult[0] = false;
				return testResult;
			}
		}
	}

	return testResult;
}