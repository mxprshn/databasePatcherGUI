#include "LogOutputDevice.h"
#include <QTextEdit>
#include <QScrollBar>

LogOutputDevice::LogOutputDevice(QObject *parent)
	: QIODevice(parent)
{
}

void LogOutputDevice::setTextEdit(QTextEdit *textEdit)
{
	this->textEdit = textEdit;
}

qint64 LogOutputDevice::readData(char *data, qint64 maxlen)
{
	return 0;
}

qint64 LogOutputDevice::writeData(const char *data, qint64 len)
{
	textEdit->append(data);
	textEdit->verticalScrollBar()->setValue(textEdit->verticalScrollBar()->maximum());
	return len;
}