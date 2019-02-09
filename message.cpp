#include "message.h"
#include "common.h"

using namespace Common;

Message::Message(
	const PersonIdType& from,
	const PersonIdType& to,
	const QDateTime& dateTime,
	const QString& text)
	: from(from)
	, to(to)
	, dateTime(dateTime)
	, text(text)
{
}

Message::Message(const QJsonObject& json)
	: from(json["from"].toInt())
	, to(json["to"].toInt())
	, dateTime(QDateTime::fromString(json["dateTime"].toString(), Common::dateTimeFormat))
	, text(json["text"].toString())
{
	assert(!json.empty());
}

QJsonObject Message::toJson() const
{
	return 
	{
		{ "from", from },
		{ "to", to },
		{ "dateTime", dateTime.toString(Common::dateTimeFormat) },
		{ "text", text }
	};
}
