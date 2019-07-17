#pragma once

#include <QObject>

class QProcess;
class QBitArray;

class InstallerHandler : public QObject
{
	Q_OBJECT

public:
	InstallerHandler(QObject *parent, const QString &program);
	~InstallerHandler();
	//bool install(const QStringList &loginData);
	QBitArray testDependencies(const QStringList &loginData);

private:
	const QString program;
	QProcess *installProcess;

signals:
	void testPassed();
	//void installFinished();
};
