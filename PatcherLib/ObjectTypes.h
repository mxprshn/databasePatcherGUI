#pragma once
#include <QHash>

namespace ObjectTypes
{
	enum Type
	{
		script,
		table,
		sequence,
		function,
		view,
		trigger,
		index,
		typeCount
	};

	extern const QHash<int, QString> typeIcons;
	extern const QHash<int, QString> typeNames;
}

