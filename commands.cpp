#include "commands.h"
#include <QJsonArray>

QString Common::sendMessageRequest("sendMessageRequest");
QString Common::sendMessageResponse("sendMessageResponse");

QString Common::getMessagesRequest("getMessagesRequest");
QString Common::getMessagesResponse("getMessagesResponse");

QString Common::authorizationRequest("authorizationResponse");
QString Common::authorizationResponse("authorizationRequest");

QString Common::registrationRequest("registrationRequest");
QString Common::registrationResponse("registrationResponse");

QString Common::typeField("_type");

using namespace Common;

SendMessageRequest::SendMessageRequest(
	const Person& from,
	const Person& to,
	const QString& text)
	: message(from, to, QDateTime::currentDateTime(), text)
{

}

SendMessageRequest::SendMessageRequest(const QJsonObject& json)
	: message(json["message"].toObject())
{
	assert(json[typeField].toString() == sendMessageRequest);
}

QJsonObject SendMessageRequest::toJson() const
{
	return 
	{
		{ typeField, sendMessageRequest },
		{ "message", message.toJson() }
	};
}

SendMessageResponse::SendMessageResponse(const SendMessageRequest& request, State state)
	: message(request.message)
	, state(state)
{

}

SendMessageResponse::SendMessageResponse(const QJsonObject& json)
	: message(json["message"].toObject())
	, state(static_cast<State>(json["state"].toInt()))
{
	assert(json[typeField].toString() == sendMessageResponse);
	assert(state < State::StatesCount);
}

QJsonObject SendMessageResponse::toJson() const
{
	return 
	{
		{ typeField, sendMessageResponse },
		{ "state", static_cast<int>(state) },
		{ "message", message.toJson() }
	};
}

GetMessagesRequest::GetMessagesRequest(int id1, int id2, int count)
	: id1(id1)
	, id2(id2)
	, count(count)
{
}

GetMessagesRequest::GetMessagesRequest(const QJsonObject& json)
	: id1(json["id1"].toInt())
	, id2(json["id2"].toInt())
	, count(json["count"].toInt())
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
		{ "count", count }
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

	std::transform(messages.begin(), messages.end(), std::back_inserter(json), 
		[](const Message& message)
		{
			return message.toJson();
		});

	return json;
}
