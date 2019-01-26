#include "message.h"

using namespace Common;

Message::Message(
	const Person& from,
	const Person& to,
	const QDateTime& dateTime,
	const QString& text)
	: from(from)
	, to(to)
	, dateTime(dateTime)
	, text(text)
{
}

Message::Message(const QJsonObject& json)
	: from(json["from"].toObject())
	, to(json["to"].toObject())
	, dateTime(QDateTime::fromString(json["dateTime"].toString(), "dd.MM.yyyy hh:mm:ss"))
	, text(json["text"].toString())
{

}

QJsonObject Message::toJson() const
{
	return 
	{
		{ "from", from.toJson() },
		{ "to", to.toJson() },
		{ "dateTime", dateTime.toString("dd.MM.yyyy hh:mm:ss") },
		{ "text", text }
	};
}
