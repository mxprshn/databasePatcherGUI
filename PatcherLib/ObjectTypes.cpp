#include "ObjectTypes.h"

namespace ObjectTypes
{
	const QHash<int, QString> typeIcons = QHash<int, QString>({ {script, ":/images/script.svg"}, {table, ":/images/table.svg"}
		, {sequence, ":/images/sequence.svg"}, {function, ":/images/function.svg"}, {view, ":/images/view.svg"}
		, {trigger, ":/images/trigger.svg"}, {index, ":/images/index.svg"} });

	const QHash<int, QString> typeNames = QHash<int, QString>({ {script, "script"}, {table, "table"}
		, {sequence, "sequence"}, {function, "function"}, {view, "view"}, {trigger, "trigger"}
		, {index, "index"} });
}