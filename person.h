#pragma once

#include <QJsonObject>
#include <QString>
#include <QUrl>
#include <QMetaType>

namespace Common
{
typedef int PersonIdType;

struct Person
{
public:
	Person(
		PersonIdType id,
		const QString& firstName,
		const QString& lastName,
		const QString& avatarUrl);

	explicit Person(const QJsonObject& json);

	Person() = default;
	Person(const Person&) = default;
	Person& operator = (const Person&) = default;

	QString name() const;
	QJsonObject toJson() const;

	bool operator==(const Person& other) const;
	bool operator!=(const Person& other) const;
	
public:
	PersonIdType id;
	QString firstName;
	QString lastName;
	QString avatarUrl;
};

}

Q_DECLARE_METATYPE(Common::Person);