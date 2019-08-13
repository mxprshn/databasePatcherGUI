#pragma once

#include <QIODevice>

class QTextEdit;

// Output device class which redirects its output to QTextEdit
class LogOutputDevice : public QIODevice
{
	Q_OBJECT

public:
	LogOutputDevice(QObject *parent = nullptr);
	void setTextEdit(QTextEdit *textEdit);
private:
	// QTextEdit object to which output is redirected
	QTextEdit *textEdit;
protected:
	qint64 readData(char* data, qint64 maxlen) override;
	qint64 writeData(const char* data, qint64 len) override;
};