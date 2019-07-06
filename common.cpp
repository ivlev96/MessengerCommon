#include "common.h"

QString Common::dateFormat("dd MMM");
QString Common::timeFormat("hh:mm");
QString Common::dateTimeFormat("dd.MM.yyyy hh:mm:ss");
int Common::defaultMessagesCount(20);
int Common::defaultPersonsCount(20);
quint32 Common::defaultTimeot(5000);
quint16 Common::serverPort(8000);
QUrl Common::serverUrl(QString("ws://192.168.0.6:%1").arg(Common::serverPort));