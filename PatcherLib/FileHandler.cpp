#include <QFile>
#include <QTextStream>
#include "BuildListModel.h"
#include "FileHandler.h"

FileHandler::FileHandler(QObject *parent)
	: QObject(parent)
{
}

bool FileHandler::makePatchList(const BuildListModel &model, const QString &path)
{
	QFile file(path);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::NewOnly))
	{
		return false;
	}

	QTextStream patchList(&file);

	for (auto i = 0; i < model.count(); ++i)
	{
		if (model.getType(i) != script)
		{
			patchList << model.getSchemeName(i) << " ";
		}

		patchList << model.getName(i) << " " << model.getTypeName(i);

		if (model.getType(i) == function)
		{
			patchList << " " << getParametersString(model.getParameters(i));
		}

		patchList << endl;
	}

	file.close();
	return true;
}

QString FileHandler::getParametersString(const QStringList& parameters)
{
	QString result = "( ";

	for (auto i = 0; i < parameters.count(); ++i)
	{
		result += parameters[i] + " ";
	}

	return result + ")";
}
