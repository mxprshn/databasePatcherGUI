#pragma once

#include <QIODevice>

class QTextEdit;

class LogOutputDevice : public QIODevice
{
	Q_OBJECT

public:
	LogOutputDevice(QObject *parent = nullptr);
	void setTextEdit(QTextEdit *textEdit);
private:
	QTextEdit *textEdit;
protected:
	qint64 readData(char* data, qint64 maxlen) override;
	qint64 writeData(const char* data, qint64 len) override;
};
