#include "common.h"

QString Common::dateTimeFormat("dd.MM.yyyy hh:mm:ss");
int Common::defaultMessagesCount(20);
quint32 Common::defaultTimeot(5000);
quint16 Common::serverPort(8000);
QUrl Common::serverUrl(QString("127.0.0.1:%1").arg(Common::serverPort));