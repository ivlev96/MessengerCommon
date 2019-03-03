#include "message.h"
#include "common.h"

using namespace Common;

MessageIdGenerator::MessageIdGenerator()
	: internalId(0)
{

}

MessageIdType MessageIdGenerator::next()
{
	static MessageIdGenerator generator;
	return generator.internalId++;
}


Message::Message(
	const PersonIdType& from,
	const PersonIdType& to,
	const QDateTime& dateTime,
	const QString& text,
	const std::optional<MessageIdType>& id,
	Message::State state)
	: from(from)
	, to(to)
	, dateTime(dateTime)
	, text(text)
	, internalId()
	, id(id)
	, state(state)
{
	if (!id.has_value())
	{
		internalId = MessageIdGenerator::next();
	}
}

Message::Message(const QJsonObject& json)
	: from(json["from"].toInt())
	, to(json["to"].toInt())
	, dateTime(QDateTime::fromString(json["dateTime"].toString(), Common::dateTimeFormat))
	, text(json["text"].toString())
	, state(static_cast<Message::State>(json["state"].toInt()))
	, internalId()
	, id()
{
	if (!json["internalId"].isNull())
	{
		internalId = json["internalId"].toInt();
	}

	if (!json["id"].isNull())
	{
		id = json["id"].toInt();
	}

	ASSERT(!json.empty());
}

QJsonObject Message::toJson() const
{
	return 
	{
		{ "from", from },
		{ "to", to },
		{ "dateTime", dateTime.toString(Common::dateTimeFormat) },
		{ "text", text },
		{ "internalId", internalId.has_value() ? *internalId : QJsonValue() },
		{ "id", id.has_value() ? *id : QJsonValue() },
		{ "state", static_cast<int>(state) }
	};
}
