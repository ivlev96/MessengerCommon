#include "commands.h"
#include <QJsonArray>

QString Common::errorResponse("error");

QString Common::sendMessagesRequest("sendMessagesRequest");
QString Common::sendMessagesResponse("sendMessagesResponse");

QString Common::getMessagesRequest("getMessagesRequest");
QString Common::getMessagesResponse("getMessagesResponse");

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

SendMessagesResponse::SendMessagesResponse(const SendMessagesRequest& request, State state)
	: messages(request.messages)
	, state(state)
{

}

SendMessagesResponse::SendMessagesResponse(const QJsonObject& json)
	: state(static_cast<State>(json["state"].toInt()))
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
	assert(state < State::StatesCount);
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

GetMessagesRequest::GetMessagesRequest(int id1, int id2, int count, int from)
	: id1(id1)
	, id2(id2)
	, count(count)
	, from(from)
{
}

GetMessagesRequest::GetMessagesRequest(const QJsonObject& json)
	: id1(json["id1"].toInt())
	, id2(json["id2"].toInt())
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
		{ "count", count },
		{ "from", from }
	};
}

GetMessagesResponse::GetMessagesResponse(int id1, int id2, const std::vector<Message>& messages)
	: id1(id1)
	, id2(id2)
	, messages(messages)
{
}

GetMessagesResponse::GetMessagesResponse(const QJsonObject& json)
	: id1(json["id1"].toInt())
	, id2(json["id2"].toInt())
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
