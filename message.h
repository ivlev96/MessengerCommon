#pragma once
#include <QDateTime>
#include <QString>
#include <QUrl>
#include <QJsonObject>
#include "person.h"

namespace Common
{
	
enum class State : quint8
{
	NotSent,
	Sent,
	Delivered,
	Read,
	StatesCount
};

struct Message
{
public:
	Message() = default;
	Message(const Message&) = default;
	Message& operator = (const Message&) = default;

	Message(
		const Person& from,
		const Person& to,
		const QDateTime& dateTime,
		const QString& text);

	explicit Message(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	Person from;
	Person to;
	QDateTime dateTime;
	QString text;
};

}