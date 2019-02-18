#include "common.h"

QString Common::dateFormat("dd.MM.yyyy");
QString Common::timeFormat("hh:mm:ss");
QString Common::dateTimeFormat("dd.MM.yyyy hh:mm:ss");
int Common::defaultMessagesCount(20);
quint32 Common::defaultTimeot(5000);
quint16 Common::serverPort(8000);
QUrl Common::serverUrl(QString("ws://localhost:%1").arg(Common::serverPort));