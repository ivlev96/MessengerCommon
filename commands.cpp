#include "commands.h"

QString Common::errorResponse("error");

QString Common::logInRequest("logInRequest");
QString Common::logInResponse("logInResponse");

QString Common::sendMessagesRequest("sendMessagesRequest");
QString Common::sendMessagesResponse("sendMessagesResponse");

QString Common::getMessagesRequest("getMessagesRequest");
QString Common::getMessagesResponse("getMessagesResponse");

QString Common::getPersonsRequest("getPersonsRequest");
QString Common::getPersonsResponse("getPersonsResponse");

QString Common::authorizationRequest("authorizationResponse");
QString Common::authorizationResponse("authorizationRequest");

QString Common::registrationRequest("registrationRequest");
QString Common::registrationResponse("registrationResponse");

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
	assert(ok == !json["person"].isNull());

	if (ok)
	{
		person = Person(json["person"].toObject());
	}
}

QJsonObject LogInResponse::toJson() const
{
	if (ok)
	{
		return
		{
			{ typeField, logInRequest },
			{ "ok", ok },
			{ "person", person->toJson() }
		};
	}

	return
	{
		{ typeField, logInRequest },
		{ "ok", ok }
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

SendMessagesResponse::SendMessagesResponse(const SendMessagesRequest& request, Message::State state)
	: messages(request.messages)
	, state(state)
{

}

SendMessagesResponse::SendMessagesResponse(const QJsonObject& json)
	: state(static_cast<Message::State>(json["state"].toInt()))
{
	assert(json["messages"].isArray());
	QJsonArray jsonArray = json["messages"].toArray();

	messages.resize(jsonArray.size());
	std::transform(jsonArray.constBegin(), jsonArray.constEnd(), messages.begin(),
		[](const QJsonValue& value)
	{
		assert(value.isObject());
		return Message(value.toObject());
	});

	assert(json[typeField].toString() == sendMessagesResponse);
	assert(state < Message::State::StatesCount);
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
		{ "state", static_cast<int>(state) },
		{ "messages", array }
	};
}

GetMessagesRequest::GetMessagesRequest(PersonIdType id1, PersonIdType id2, bool isNew, int count, int from)
	: id1(id1)
	, id2(id2)
	, isNew(isNew)
	, count(count)
	, from(from)
{
}

GetMessagesRequest::GetMessagesRequest(const QJsonObject& json)
	: id1(json["id1"].toInt())
	, id2(json["id2"].toInt())
	, isNew(json["isNew"].toBool())
	, count(json["count"].toInt())
	, from(json["from"].toInt())
{
	assert(json[typeField].toString() == getMessagesRequest);
}

QJsonObject GetMessagesRequest::toJson() const
{
	return 
	{
		{ typeField, getMessagesRequest },
		{ "id1", id1 },
		{ "id2", id2 },
		{ "isNew", isNew },
		{ "count", count },
		{ "from", from }
	};
}

GetMessagesResponse::GetMessagesResponse(PersonIdType id1, PersonIdType id2, bool isNew, const std::vector<Message>& messages)
	: id1(id1)
	, id2(id2)
	, isNew(isNew)
	, messages(messages)
{
}

GetMessagesResponse::GetMessagesResponse(const QJsonObject& json)
	: id1(json["id1"].toInt())
	, id2(json["id2"].toInt())
	, isNew(json["isNew"].toBool())
{
	assert(json[typeField].toString() == getMessagesResponse);
	assert(json["messages"].isArray());

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
	return 
	{
		{ typeField, getMessagesResponse },
		{ "id1", id1 },
		{ "id2", id2 },
		{ "isNew", isNew },
		{ "messages", messagesToJson() },
	};
}

QJsonArray GetMessagesResponse::messagesToJson() const
{
	QJsonArray json;

	std::transform(messages.cbegin(), messages.cend(), std::back_inserter(json), 
		[](const Message& message)
		{
			return message.toJson();
		});

	return json;
}

GetPersonsRequest::GetPersonsRequest(PersonIdType friendsOf)
	: friendsOf(friendsOf)
{

}

GetPersonsRequest::GetPersonsRequest(const QJsonObject& json)
	: friendsOf(json["friendsOf"].toInt())
{
	assert(json[typeField].toString() == getPersonsRequest);
}

QJsonObject GetPersonsRequest::toJson() const
{
	return 
	{
		{ typeField, getPersonsRequest },
		{ "friendsOf", friendsOf }
	};
}

GetPersonsResponse::GetPersonsResponse(const std::vector<Person>& persons)
	: persons(persons)
{
}

GetPersonsResponse::GetPersonsResponse(const QJsonObject& json)
{
	assert(json[typeField].toString() == getMessagesResponse);

	assert(json["persons"].isArray());

	QJsonArray personsInJson = json["persons"].toArray();
	persons.resize(personsInJson.size());

	std::transform(personsInJson.constBegin(), personsInJson.constEnd(), persons.begin(),
		[](const QJsonValue& value)
	{
		assert(value.isObject());
		return Person(value.toObject());
	});
}
