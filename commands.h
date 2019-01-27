#pragma once

#include "person.h"
#include "message.h"

#include <QDateTime>
#include <QString>
#include <QUrl>
#include <QJsonObject>
#include <vector>

namespace Common
{

extern QString sendMessageRequest;
extern QString sendMessageResponse;

extern QString getMessagesRequest;
extern QString getMessagesResponse;

extern QString authorizationRequest;
extern QString authorizationResponse;

extern QString registrationRequest;
extern QString registrationResponse;

extern QString typeField;

struct SendMessageRequest
{
public:
	SendMessageRequest(const SendMessageRequest&) = default;

	SendMessageRequest() = delete;
	SendMessageRequest& operator = (const SendMessageRequest&) = delete;

	SendMessageRequest(
		const Person& from,
		const Person& to,
		const QString& text);

	explicit SendMessageRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	Message message;
};

struct SendMessageResponse
{
public:
	SendMessageResponse(const SendMessageResponse&) = default;

	SendMessageResponse() = delete;
	SendMessageResponse& operator = (const SendMessageResponse&) = delete;

	SendMessageResponse(const SendMessageRequest& request, State state);

	explicit SendMessageResponse(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	Message message;
	State state;
};


struct GetMessagesRequest
{
public:
	GetMessagesRequest(int id1, int id2, int count);
	explicit GetMessagesRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	int id1;
	int id2;
	int count;
};

struct GetMessagesResponse
{
public:
	GetMessagesResponse(int id1, int id2, const std::vector<Message>& messages);
	explicit GetMessagesResponse(const QJsonObject& json);

	QJsonObject toJson() const;

private:
	QJsonArray messagesToJson() const;

public:
	int id1;
	int id2;
	std::vector<Message> messages;
};



}