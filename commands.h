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

extern QString errorResponse;

extern QString sendMessagesRequest;
extern QString sendMessagesResponse;

extern QString getMessagesRequest;
extern QString getMessagesResponse;

extern QString getPersonsRequest;
extern QString getPersonsResponse;

extern QString authorizationRequest;
extern QString authorizationResponse;

extern QString registrationRequest;
extern QString registrationResponse;

extern QString typeField;


struct ErrorResponse
{
	explicit ErrorResponse(const QString& error);
	explicit ErrorResponse(const QJsonObject& json);

	ErrorResponse(const ErrorResponse&) = default;

	ErrorResponse() = delete;
	ErrorResponse& operator = (const ErrorResponse&) = delete;

	QJsonObject toJson() const;

public:
	QString error;
};


struct SendMessagesRequest
{
public:
	SendMessagesRequest(const SendMessagesRequest&) = default;

	SendMessagesRequest() = delete;
	SendMessagesRequest& operator = (const SendMessagesRequest&) = delete;

	SendMessagesRequest(const std::vector<Message>& messages);

	explicit SendMessagesRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	std::vector<Message> messages;
};

struct SendMessagesResponse
{
public:
	SendMessagesResponse(const SendMessagesResponse&) = default;

	SendMessagesResponse() = delete;
	SendMessagesResponse& operator = (const SendMessagesResponse&) = delete;

	SendMessagesResponse(const SendMessagesRequest& request, Message::State state);

	explicit SendMessagesResponse(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	std::vector<Message> messages;
	Message::State state;
};


struct GetMessagesRequest
{
public:
	GetMessagesRequest(PersonIdType id1, PersonIdType id2, int count, int from = 0);
	explicit GetMessagesRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	PersonIdType id1;
	PersonIdType id2;
	int count;
	int from;
};

struct GetMessagesResponse
{
public:
	GetMessagesResponse(PersonIdType id1, PersonIdType id2, const std::vector<Message>& messages);
	explicit GetMessagesResponse(const QJsonObject& json);

	QJsonObject toJson() const;

private:
	QJsonArray messagesToJson() const;

public:
	PersonIdType id1;
	PersonIdType id2;
	std::vector<Message> messages;
};


struct GetPersonsRequest
{
public:
	GetPersonsRequest(PersonIdType friendsOf);
	explicit GetPersonsRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	PersonIdType friendsOf;
};

struct GetPersonsResponse
{
public:
	GetPersonsResponse(const std::vector<Person>& persons);
	explicit GetPersonsResponse(const QJsonObject& json);

	QJsonObject toJson() const;

private:
	QJsonArray personsToJson() const;

public:
	std::vector<Person> persons;
};

}