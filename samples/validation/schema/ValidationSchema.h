// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef VALIDATIONSCHEMA_H
#define VALIDATIONSCHEMA_H

#include "graphqlservice/internal/Schema.h"

// Check if the library version is compatible with schemagen 4.2.0
static_assert(graphql::internal::MajorVersion == 4, "regenerate with schemagen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 2, "regenerate with schemagen: minor version mismatch");

#include <array>
#include <memory>
#include <string>
#include <string_view>

namespace graphql {
namespace validation {

enum class [[nodiscard]] DogCommand
{
	SIT,
	DOWN,
	HEEL
};

[[nodiscard]] constexpr auto getDogCommandNames() noexcept
{
	using namespace std::literals;

	return std::array<std::string_view, 3> {
		R"gql(SIT)gql"sv,
		R"gql(DOWN)gql"sv,
		R"gql(HEEL)gql"sv
	};
}

[[nodiscard]] constexpr auto getDogCommandValues() noexcept
{
	using namespace std::literals;

	return std::array<std::pair<std::string_view, DogCommand>, 3> {
		std::make_pair(R"gql(SIT)gql"sv, DogCommand::SIT),
		std::make_pair(R"gql(DOWN)gql"sv, DogCommand::DOWN),
		std::make_pair(R"gql(HEEL)gql"sv, DogCommand::HEEL)
	};
}

enum class [[nodiscard]] CatCommand
{
	JUMP
};

[[nodiscard]] constexpr auto getCatCommandNames() noexcept
{
	using namespace std::literals;

	return std::array<std::string_view, 1> {
		R"gql(JUMP)gql"sv
	};
}

[[nodiscard]] constexpr auto getCatCommandValues() noexcept
{
	using namespace std::literals;

	return std::array<std::pair<std::string_view, CatCommand>, 1> {
		std::make_pair(R"gql(JUMP)gql"sv, CatCommand::JUMP)
	};
}

struct ComplexInput
{
	std::optional<std::string> name {};
	std::optional<std::string> owner {};
};

namespace object {

class Sentient;
class Pet;
class Node;
class Resource;

class CatOrDog;
class DogOrHuman;
class HumanOrAlien;

class Query;
class Dog;
class Alien;
class Human;
class Cat;
class Mutation;
class MutateDogResult;
class Subscription;
class Message;
class Arguments;

} // namespace object

class [[nodiscard]] Operations final
	: public service::Request
{
public:
	explicit Operations(std::shared_ptr<object::Query> query, std::shared_ptr<object::Mutation> mutation, std::shared_ptr<object::Subscription> subscription);

	template <class TQuery, class TMutation, class TSubscription>
	explicit Operations(std::shared_ptr<TQuery> query, std::shared_ptr<TMutation> mutation, std::shared_ptr<TSubscription> subscription)
		: Operations { std::make_shared<object::Query>(std::move(query)), std::make_shared<object::Mutation>(std::move(mutation)), std::make_shared<object::Subscription>(std::move(subscription)) }
	{
	}

private:
	std::shared_ptr<object::Query> _query;
	std::shared_ptr<object::Mutation> _mutation;
	std::shared_ptr<object::Subscription> _subscription;
};

void AddSentientDetails(const std::shared_ptr<schema::InterfaceType>& typeSentient, const std::shared_ptr<schema::Schema>& schema);
void AddPetDetails(const std::shared_ptr<schema::InterfaceType>& typePet, const std::shared_ptr<schema::Schema>& schema);
void AddNodeDetails(const std::shared_ptr<schema::InterfaceType>& typeNode, const std::shared_ptr<schema::Schema>& schema);
void AddResourceDetails(const std::shared_ptr<schema::InterfaceType>& typeResource, const std::shared_ptr<schema::Schema>& schema);

void AddCatOrDogDetails(const std::shared_ptr<schema::UnionType>& typeCatOrDog, const std::shared_ptr<schema::Schema>& schema);
void AddDogOrHumanDetails(const std::shared_ptr<schema::UnionType>& typeDogOrHuman, const std::shared_ptr<schema::Schema>& schema);
void AddHumanOrAlienDetails(const std::shared_ptr<schema::UnionType>& typeHumanOrAlien, const std::shared_ptr<schema::Schema>& schema);

void AddQueryDetails(const std::shared_ptr<schema::ObjectType>& typeQuery, const std::shared_ptr<schema::Schema>& schema);
void AddDogDetails(const std::shared_ptr<schema::ObjectType>& typeDog, const std::shared_ptr<schema::Schema>& schema);
void AddAlienDetails(const std::shared_ptr<schema::ObjectType>& typeAlien, const std::shared_ptr<schema::Schema>& schema);
void AddHumanDetails(const std::shared_ptr<schema::ObjectType>& typeHuman, const std::shared_ptr<schema::Schema>& schema);
void AddCatDetails(const std::shared_ptr<schema::ObjectType>& typeCat, const std::shared_ptr<schema::Schema>& schema);
void AddMutationDetails(const std::shared_ptr<schema::ObjectType>& typeMutation, const std::shared_ptr<schema::Schema>& schema);
void AddMutateDogResultDetails(const std::shared_ptr<schema::ObjectType>& typeMutateDogResult, const std::shared_ptr<schema::Schema>& schema);
void AddSubscriptionDetails(const std::shared_ptr<schema::ObjectType>& typeSubscription, const std::shared_ptr<schema::Schema>& schema);
void AddMessageDetails(const std::shared_ptr<schema::ObjectType>& typeMessage, const std::shared_ptr<schema::Schema>& schema);
void AddArgumentsDetails(const std::shared_ptr<schema::ObjectType>& typeArguments, const std::shared_ptr<schema::Schema>& schema);

std::shared_ptr<schema::Schema> GetSchema();

} // namespace validation

namespace service {

template <>
[[nodiscard]] constexpr bool isInputType<validation::ComplexInput>() noexcept
{
	return true;
}

} // namespace service
} // namespace graphql

#endif // VALIDATIONSCHEMA_H
