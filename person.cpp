#include "person.h"

using namespace Common;

Person::Person(
	int id,
	const QString& firstName,
	const QString& lastName,
	const QString& avatarUrl)
	: id(id)
	, firstName(firstName)
	, lastName(lastName)
	, avatarUrl(avatarUrl)
{
}

Person::Person(const QJsonObject& json)
	: id(json["id"].toInt())
	, firstName(json["firstName"].toString())
	, lastName(json["lastName"].toString())
	, avatarUrl(json["avatar"].toString())
{
}

QString Person::name() const
{
	return QString("%1 %2").arg(firstName, lastName);
}

QJsonObject Person::toJson() const
{
	return 
	{
		{ "id", id },
		{ "firstName", firstName },
		{ "lastName", lastName },
		{ "avatarUrl", avatarUrl.toString() }
	};
}

bool Common::Person::operator==(const Person& other) const
{
	return id == other.id;
}

bool Common::Person::operator!=(const Person & other) const
{
	return !(*this == other);
}
