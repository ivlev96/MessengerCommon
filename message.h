#pragma once
#include "person.h"

#include <QDateTime>
#include <QString>
#include <QUrl>
#include <QJsonObject>
#include <QMetaType>

namespace Common
{

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
		const QString& text);

	explicit Message(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	PersonIdType from;
	PersonIdType to;
	QDateTime dateTime;
	QString text;
};

}

Q_DECLARE_METATYPE(Common::Message);
Q_DECLARE_METATYPE(Common::Message::State);