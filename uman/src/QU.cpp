#include "QU.h"

#include <QRegExp>

/*!
 * Remove all "folder tags" like [SC], [VIDEO], a.s.o. from the given text.
 */
QString QU::withoutFolderTags(const QString &text) {
	QString result = text;
	QRegExp r("\\[.*\\]");
	r.setMinimal(true);

	return result.remove(r).trimmed();
}
