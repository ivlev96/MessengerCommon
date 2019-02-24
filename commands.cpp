#include "commands.h"

QString Common::errorResponse("error");

QString Common::registrationRequest("registrationRequest");
QString Common::registrationResponse("registrationResponse");

QString Common::logInRequest("logInRequest");
QString Common::logInResponse("logInResponse");

QString Common::getLastMessagesRequest("getLastMessagesRequest");
QString Common::getLastMessagesResponse("getLastMessagesResponse");

QString Common::sendMessagesRequest("sendMessagesRequest");
QString Common::sendMessagesResponse("sendMessagesResponse");

QString Common::getMessagesRequest("getMessagesRequest");
QString Common::getMessagesResponse("getMessagesResponse");

QString Common::newMessageCommand("newMessageCommand");

QString Common::typeField("_type");

using namespace Common;

ErrorResponse::ErrorResponse(const QString& error)
	: error(error)
{
}

ErrorResponse::ErrorResponse(const QJsonObject& json)
	: error(json["error"].toString())
{
	assert(json[typeField].toString() == errorResponse);
}

QJsonObject Common::ErrorResponse::toJson() const
{
	return 
	{
		{ typeField, errorResponse },
		{ "error", error }
	};
}

RegistrationRequest::RegistrationRequest(
	const QString& firstName,
	const QString& lastName,
	const QString& avatarUrl,
	const QString& login,
	const QString& password)
	: firstName(firstName)
	, lastName(lastName)
	, avatarUrl(avatarUrl)
	, login(login)
	, password(password)
{
}

RegistrationRequest::RegistrationRequest(const QJsonObject& json)
	: firstName(json["firstName"].toString())
	, lastName(json["lastName"].toString())
	, avatarUrl(json["avatarUrl"].toString())
	, login(json["login"].toString())
	, password(json["password"].toString())
{
	assert(json[typeField].toString() == registrationRequest);
}

QJsonObject RegistrationRequest::toJson() const
{
	return 
	{
		{ typeField, registrationRequest },
		{ "firstName", firstName },
		{ "lastName", lastName },
		{ "avatarUrl", avatarUrl },
		{ "login", login },
		{ "password", password },
	};
}

RegistrationResponse::RegistrationResponse(const std::optional<Person>& person)
	: person(person)
	, ok(person.has_value())
{
}

RegistrationResponse::RegistrationResponse(const QJsonObject& json)
	: ok(json["ok"].toBool())
	, person()
{
	assert(json[typeField].toString() == registrationResponse);
	assert(ok != json["person"].isNull());

	if (ok)
	{
		person = Person(json["person"].toObject());
	}
}

QJsonObject RegistrationResponse::toJson() const
{
	const QJsonValue personValue = ok ? person->toJson() : QJsonValue();

	return
	{
		{ typeField, registrationResponse },
		{ "ok", ok },
		{ "person", personValue }
	};
}

LogInRequest::LogInRequest(const QString& login, const QString& password)
	: login(login)
	, password(password)
{

}

LogInRequest::LogInRequest(const QJsonObject& json)
	: login(json["login"].toString())
	, password(json["password"].toString())
{
	assert(json[typeField].toString() == logInRequest);
}

QJsonObject LogInRequest::toJson() const
{
	return 
	{
		{ typeField, logInRequest },
		{ "login", login },
		{ "password", password }
	};
}

LogInResponse::LogInResponse(const std::optional<Person>& person)
	: person(person)
	, ok(person.has_value())
{
}

LogInResponse::LogInResponse(const QJsonObject& json)
	: ok(json["ok"].toBool())
	, person()
{
	assert(json[typeField].toString() == logInResponse);
	assert(ok != json["person"].isNull());

	if (ok)
	{
		person = Person(json["person"].toObject());
	}
}

QJsonObject LogInResponse::toJson() const
{
	const QJsonValue personValue = ok ? person->toJson() : QJsonValue();

	return
	{
		{ typeField, logInResponse },
		{ "ok", ok },
		{ "person", personValue }
	};
}

GetLastMessagesRequest::GetLastMessagesRequest(PersonIdType id, int count, const std::optional<MessageIdType>& before)
	: id(id)
	, count(count)
	, before(before)
{
	assert(count > 0);
}

GetLastMessagesRequest::GetLastMessagesRequest(const QJsonObject& json)
	: id(json["id"].toInt())
	, count(json["count"].toInt())
	, before()
{
	assert(json[typeField].toString() == getLastMessagesRequest);

	if (!json["before"].isNull())
	{
		before = json["before"].toInt();
	}
}

QJsonObject Common::GetLastMessagesRequest::toJson() const
{
	return
	{
		{ typeField, getMessagesRequest },
		{ "id", id },
		{ "count", count },
		{ "before", before.has_value() ? *before : QJsonValue() }
	};
}

GetLastMessagesResponse::GetLastMessagesResponse(PersonIdType id,
	const std::vector<std::pair<Person, Message>>& messages,
	const std::optional<MessageIdType>& before)
	: id(id)
	, messages(messages)
	, before(before)
{
}

GetLastMessagesResponse::GetLastMessagesResponse(const QJsonObject& json)
	: id(json["id"].toInt())
	, before()
{
	assert(json[typeField].toString() == getLastMessagesResponse);

	if (!json["before"].isNull())
	{
		before = json["before"].toInt();
	}

	const QJsonValue messagesValue = json["messages"];
	assert(messagesValue.isArray());

	const QJsonArray messagesArray = messagesValue.toArray();

	messages.resize(messagesArray.size());
	std::transform(messagesArray.constBegin(), messagesArray.constEnd(), messages.begin(),
		[](const QJsonValue& value)
	{
		const QJsonObject json = value.toObject();
		return std::make_pair<Person, Message>(
			Person(json["person"].toObject()),
			Message(json["message"].toObject()));
	});
}

QJsonObject GetLastMessagesResponse::toJson() const
{
	QJsonArray messagesJson;

	std::transform(messages.cbegin(), messages.cend(), std::back_inserter(messagesJson),
		[](const auto& pair) -> QJsonObject
	{
		const auto[person, message] = pair;
		return
		{
			{ "person", person.toJson() },
			{ "message", message.toJson() }
		};
	});

	return
	{
		{ typeField, getLastMessagesResponse },
		{ "id", id },
		{ "messages", messagesJson },
		{ "before",  before.has_value() ? *before : QJsonValue() }
	};
}

SendMessagesRequest::SendMessagesRequest(const std::vector<Message>& messages)
	: messages(messages)
{

}

SendMessagesRequest::SendMessagesRequest(const QJsonObject& json)
	: messages()
{
	assert(json[typeField].toString() == sendMessagesRequest);
	
	assert(json["messages"].isArray());
	QJsonArray jsonArray = json["messages"].toArray();

	messages.resize(jsonArray.size());
	std::transform(jsonArray.constBegin(), jsonArray.constEnd(), messages.begin(),
		[](const QJsonValue& value)
		{
			assert(value.isObject());
			return Message(value.toObject());
		});
}

QJsonObject SendMessagesRequest::toJson() const
{
	QJsonArray array;
	std::transform(messages.cbegin(), messages.cend(), std::back_inserter(array),
		[](const Message& message)
		{
			return message.toJson();
		});

	return 
	{
		{ typeField, sendMessagesRequest },
		{ "messages", array }
	};
}

SendMessagesResponse::SendMessagesResponse(const std::vector<Message>& messages)
	: messages(messages)
{

}

SendMessagesResponse::SendMessagesResponse(const QJsonObject& json)
{
	assert(json[typeField].toString() == sendMessagesResponse);

	assert(json["messages"].isArray());
	QJsonArray jsonArray = json["messages"].toArray();

	messages.resize(jsonArray.size());
	std::transform(jsonArray.constBegin(), jsonArray.constEnd(), messages.begin(),
		[](const QJsonValue& value)
	{
		assert(value.isObject());
		return Message(value.toObject());
	});
}

QJsonObject SendMessagesResponse::toJson() const
{
	QJsonArray array;
	std::transform(messages.cbegin(), messages.cend(), std::back_inserter(array),
		[](const Message& message)
	{
		return message.toJson();
	});

	return 
	{
		{ typeField, sendMessagesResponse },
		{ "messages", array }
	};
}

GetMessagesRequest::GetMessagesRequest(PersonIdType id1, PersonIdType id2, int count, std::optional<MessageIdType> before)
	: id1(id1)
	, id2(id2)
	, count(count)
	, before(before)
{
}

GetMessagesRequest::GetMessagesRequest(const QJsonObject& json)
	: id1(json["id1"].toInt())
	, id2(json["id2"].toInt())
	, count(json["count"].toInt())
	, before()
{
	assert(json[typeField].toString() == getMessagesRequest);

	if (!json["before"].isNull())
	{
		before = json["before"].toInt();
	}
}

QJsonObject GetMessagesRequest::toJson() const
{
	return 
	{
		{ typeField, getMessagesRequest },
		{ "id1", id1 },
		{ "id2", id2 },
		{ "count", count },
		{ "before", before.has_value() ? *before : QJsonValue() }
	};
}

GetMessagesResponse::GetMessagesResponse(PersonIdType id1, PersonIdType id2, const std::vector<Message>& messages, std::optional<MessageIdType> before)
	: id1(id1)
	, id2(id2)
	, messages(messages)
	, before(before)
{
}

GetMessagesResponse::GetMessagesResponse(const QJsonObject& json)
	: id1(json["id1"].toInt())
	, id2(json["id2"].toInt())
	, before()
{
	assert(json[typeField].toString() == getMessagesResponse);
	assert(json["messages"].isArray());

	if (!json["before"].isNull())
	{
		before = json["before"].toInt();
	}

	QJsonArray messagesInJson = json["messages"].toArray();
	messages.resize(messagesInJson.size());

	std::transform(messagesInJson.constBegin(), messagesInJson.constEnd(), messages.begin(), 
		[](const QJsonValue& value)
		{
			assert(value.isObject());
			return Message(value.toObject());
		});
}

QJsonObject GetMessagesResponse::toJson() const
{
	QJsonArray messagesJson;

	std::transform(messages.cbegin(), messages.cend(), std::back_inserter(messagesJson),
		[](const Message& message)
	{
		return message.toJson();
	});

	return 
	{
		{ typeField, getMessagesResponse },
		{ "id1", id1 },
		{ "id2", id2 },
		{ "messages", messagesJson },
		{ "before", before.has_value() ? *before : QJsonValue() }
	};
}

NewMessageCommand::NewMessageCommand(const Person& from, const Message& message)
	: from(from)
	, message(message)
{
}

NewMessageCommand::NewMessageCommand(const QJsonObject& json)
	: from(json["from"].toObject())
	, message(json["message"].toObject())
{
	assert(json[typeField].toString() == newMessageCommand);
}

QJsonObject NewMessageCommand::toJson() const
{
	return
	{
		{ typeField, newMessageCommand },
		{ "from", from.toJson() },
		{ "message", message.toJson() }
	};
}
