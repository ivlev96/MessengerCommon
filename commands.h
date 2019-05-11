#pragma once

#include "person.h"
#include "message.h"

namespace Common
{

extern QString errorResponse;

extern QString registrationRequest;
extern QString registrationResponse;

extern QString logInRequest;
extern QString logInResponse;

extern QString getLastMessagesRequest;
extern QString getLastMessagesResponse;

extern QString sendMessagesRequest;
extern QString sendMessagesResponse;

extern QString getMessagesRequest;
extern QString getMessagesResponse;

extern QString newMessageCommand;

extern QString findFriendRequest;
extern QString findFriendResponse;

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


struct RegistrationRequest
{
public:
	RegistrationRequest(
		const QString& firstName, 
		const QString& lastName, 
		const QString& avatarUrl, 
		const QString& login, 
		const QString& password);

	explicit RegistrationRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	QString firstName;
	QString lastName;
	QString avatarUrl;

	QString login;
	QString password;
};

struct RegistrationResponse
{
public:
	RegistrationResponse(const std::optional<Person>& person);
	explicit RegistrationResponse(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	std::optional<Person> person;
};


struct LogInRequest
{
public:
	LogInRequest(const QString& login, const QString& password);
	explicit LogInRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	QString login;
	QString password;
};

struct LogInResponse
{
public:
	LogInResponse(const std::optional<Person>& person);
	explicit LogInResponse(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	std::optional<Person> person;
};


struct GetLastMessagesRequest
{
public:
	GetLastMessagesRequest(PersonIdType id, int count, const std::optional<MessageIdType>& before = {});
	explicit GetLastMessagesRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	PersonIdType id;
	int count;
	std::optional<MessageIdType> before;
};

struct GetLastMessagesResponse
{
public:
	GetLastMessagesResponse(PersonIdType id, 
		const std::vector<std::pair<Person, Message>>& messages, 
		const std::optional<MessageIdType>& before = {});

	explicit GetLastMessagesResponse(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	PersonIdType id;
	std::vector<std::pair<Person, Message>> messages;
	std::optional<MessageIdType> before;
};

struct SendMessagesRequest
{
public:
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

	SendMessagesResponse(const std::vector<Message>& messages);

	explicit SendMessagesResponse(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	std::vector<Message> messages;
};


struct GetMessagesRequest
{
public:
	GetMessagesRequest(PersonIdType id1, PersonIdType id2, int count, std::optional<MessageIdType> before = {});
	explicit GetMessagesRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	PersonIdType id1;
	PersonIdType id2;
	int count;
	std::optional<MessageIdType> before;
};

struct GetMessagesResponse
{
public:
	GetMessagesResponse(PersonIdType id1, PersonIdType id2, const std::vector<Message>& messages, std::optional<MessageIdType> before = {});
	explicit GetMessagesResponse(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	PersonIdType id1;
	PersonIdType id2;
	std::vector<Message> messages;
	std::optional<MessageIdType> before;
};


struct NewMessageCommand
{
public:
	NewMessageCommand(const Person& from, const Message& message);
	explicit NewMessageCommand(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	Person from;
	Message message;
};


struct FindFriendRequest
{
public:
	explicit FindFriendRequest(const QString& name);
	explicit FindFriendRequest(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	QString name;
};

struct FindFriendResponse
{
public:
	explicit FindFriendResponse(const std::vector<Person>& persons);
	explicit FindFriendResponse(const QJsonObject& json);

	QJsonObject toJson() const;

public:
	std::vector<Person> persons;
};

}