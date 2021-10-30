// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef STARWARSSCHEMA_H
#define STARWARSSCHEMA_H

#include "graphqlservice/internal/Schema.h"

// Check if the library version is compatible with schemagen 3.6.0
static_assert(graphql::internal::MajorVersion == 3, "regenerate with schemagen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 6, "regenerate with schemagen: minor version mismatch");

#include <memory>
#include <string>
#include <vector>

namespace graphql {
namespace learn {

enum class Episode
{
	NEW_HOPE,
	EMPIRE,
	JEDI
};

struct ReviewInput
{
	response::IntType stars;
	std::optional<response::StringType> commentary;
};

namespace object {

class Human;
class Droid;
class Query;
class Review;
class Mutation;

} // namespace object

struct Character
{
	virtual service::FieldResult<response::StringType> getId(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::optional<response::StringType>> getName(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::optional<std::vector<std::shared_ptr<service::Object>>>> getFriends(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::optional<std::vector<std::optional<Episode>>>> getAppearsIn(service::FieldParams&& params) const = 0;
};

class Operations
	: public service::Request
{
public:
	explicit Operations(std::shared_ptr<object::Query> query, std::shared_ptr<object::Mutation> mutation);

private:
	std::shared_ptr<object::Query> _query;
	std::shared_ptr<object::Mutation> _mutation;
};

void AddHumanDetails(std::shared_ptr<schema::ObjectType> typeHuman, const std::shared_ptr<schema::Schema>& schema);
void AddDroidDetails(std::shared_ptr<schema::ObjectType> typeDroid, const std::shared_ptr<schema::Schema>& schema);
void AddQueryDetails(std::shared_ptr<schema::ObjectType> typeQuery, const std::shared_ptr<schema::Schema>& schema);
void AddReviewDetails(std::shared_ptr<schema::ObjectType> typeReview, const std::shared_ptr<schema::Schema>& schema);
void AddMutationDetails(std::shared_ptr<schema::ObjectType> typeMutation, const std::shared_ptr<schema::Schema>& schema);

std::shared_ptr<schema::Schema> GetSchema();

} // namespace learn
} // namespace graphql

#endif // STARWARSSCHEMA_H
