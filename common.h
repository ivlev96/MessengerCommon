#pragma once

namespace Common
{
	extern QString dateFormat;
	extern QString timeFormat;
	extern QString dateTimeFormat;
	extern int defaultMessagesCount;
	extern int defaultPersonsCount;
	extern quint32 defaultTimeot;
	extern quint16 serverPort;
	extern QUrl serverUrl;

	template<typename T>
	QString toString(const T& object)
	{
		return QJsonDocument(object.toJson()).toJson();
	}
}
