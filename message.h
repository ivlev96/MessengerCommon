#pragma once
#include "person.h"

#include <QDateTime>
#include <QString>
#include <QUrl>
#include <QJsonObject>
#include <QMetaType>
#include <optional>

namespace Common
{
typedef int MessageIdType;

class MessageIdGenerator //internal ids are unique only in current session
{
public:
	static MessageIdType next();

private:
	MessageIdGenerator();
	MessageIdGenerator(const MessageIdGenerator&) = delete;
	MessageIdGenerator& operator=(const MessageIdGenerator&) = delete;

private:
	MessageIdType internalId;
};

struct Message
{
public:
	enum class State : quint8
	{
		NotSent,
		Sent,
		Delivered,
		Read,
		StatesCount
	};

	Message() = default;
	Message(const Message&) = default;
	Message& operator = (const Message&) = default;

	Message(
		const PersonIdType& from,
		const PersonIdType& to,
		const QDateTime& dateTime,
		const QString& text,
		const std::optional<MessageIdType>& id = {},
		Message::State state = State::NotSent);

	explicit Message(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	PersonIdType from;
	PersonIdType to;
	QDateTime dateTime;
	QString text;
	std::optional<MessageIdType> internalId;
	std::optional<MessageIdType> id;
	State state;
};

}

Q_DECLARE_METATYPE(Common::Message);
Q_DECLARE_METATYPE(Common::Message::State);