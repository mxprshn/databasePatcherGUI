#include "InstallerHandler.h"
#include <QBitArray>
#include <QProcess>

InstallerHandler::InstallerHandler(QObject *parent, const QString &program)
	: QObject(parent)
	, program(program)
{
	installProcess = new QProcess(this);
}

QBitArray InstallerHandler::testDependencies(const QStringList &loginData)
{
	connect(this->installProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(testFinished()));
	installProcess->start(program, loginData);
	installProcess->waitForFinished();

	QBitArray testResult;
	installProcess->setReadChannel(QProcess::ProcessChannel::StandardOutput);
	QByteArray readData = installProcess->readAllStandardOutput();
	testResult.resize(readData.count());

	for (auto i = 0; i < readData.count(); ++i)
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
		}
	}

	return testResult;
}

InstallerHandler::~InstallerHandler()
{
}
