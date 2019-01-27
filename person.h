#pragma once

#include <QJsonObject>
#include <QString>
#include <QUrl>
#include <QMetaType>

namespace Common
{

struct Person
{
public:
	Person(
		int id,
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
	int id;
	QString firstName;
	QString lastName;
	QUrl avatarUrl;
};

}

Q_DECLARE_METATYPE(Common::Person);