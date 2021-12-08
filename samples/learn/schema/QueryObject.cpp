// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "StarWarsObjects.h"

#include "graphqlservice/introspection/Introspection.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::learn {
namespace object {

Query::Query(std::unique_ptr<Concept>&& pimpl) noexcept
	: service::Object{ getTypeNames(), getResolvers() }
	, _schema { GetSchema() }
	, _pimpl { std::move(pimpl) }
{
}

service::TypeNames Query::getTypeNames() const noexcept
{
	return {
		R"gql(Query)gql"sv
	};
}

service::ResolverMap Query::getResolvers() const noexcept
{
	return {
		{ R"gql(hero)gql"sv, [this](service::ResolverParams&& params) { return resolveHero(std::move(params)); } },
		{ R"gql(droid)gql"sv, [this](service::ResolverParams&& params) { return resolveDroid(std::move(params)); } },
		{ R"gql(human)gql"sv, [this](service::ResolverParams&& params) { return resolveHuman(std::move(params)); } },
		{ R"gql(__type)gql"sv, [this](service::ResolverParams&& params) { return resolve_type(std::move(params)); } },
		{ R"gql(__schema)gql"sv, [this](service::ResolverParams&& params) { return resolve_schema(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } }
	};
}

void Query::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void Query::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

service::AwaitableResolver Query::resolveHero(service::ResolverParams&& params) const
{
	auto argEpisode = service::ModifiedArgument<learn::Episode>::require<service::TypeModifier::Nullable>("episode", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getHero(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argEpisode));
	resolverLock.unlock();

	return service::ModifiedResult<Character>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveHuman(service::ResolverParams&& params) const
{
	auto argId = service::ModifiedArgument<std::string>::require("id", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getHuman(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argId));
	resolverLock.unlock();

	return service::ModifiedResult<Human>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolveDroid(service::ResolverParams&& params) const
{
	auto argId = service::ModifiedArgument<std::string>::require("id", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getDroid(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)), std::move(argId));
	resolverLock.unlock();

	return service::ModifiedResult<Droid>::convert<service::TypeModifier::Nullable>(std::move(result), std::move(params));
}

service::AwaitableResolver Query::resolve_typename(service::ResolverParams&& params) const
{
	return service::ModifiedResult<std::string>::convert(std::string{ R"gql(Query)gql" }, std::move(params));
}

service::AwaitableResolver Query::resolve_schema(service::ResolverParams&& params) const
{
	return service::ModifiedResult<service::Object>::convert(std::static_pointer_cast<service::Object>(std::make_shared<introspection::object::Schema>(std::make_shared<introspection::Schema>(_schema))), std::move(params));
}

service::AwaitableResolver Query::resolve_type(service::ResolverParams&& params) const
{
	auto argName = service::ModifiedArgument<std::string>::require("name", params.arguments);
	const auto& baseType = _schema->LookupType(argName);
	std::shared_ptr<introspection::object::Type> result { baseType ? std::make_shared<introspection::object::Type>(std::make_shared<introspection::Type>(baseType)) : nullptr };

	return service::ModifiedResult<introspection::object::Type>::convert<service::TypeModifier::Nullable>(result, std::move(params));
}

} // namespace object

void AddQueryDetails(const std::shared_ptr<schema::ObjectType>& typeQuery, const std::shared_ptr<schema::Schema>& schema)
{
	typeQuery->AddFields({
		schema::Field::Make(R"gql(hero)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType(R"gql(Character)gql"sv), {
			schema::InputValue::Make(R"gql(episode)gql"sv, R"md()md"sv, schema->LookupType(R"gql(Episode)gql"sv), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(human)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType(R"gql(Human)gql"sv), {
			schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(droid)gql"sv, R"md()md"sv, std::nullopt, schema->LookupType(R"gql(Droid)gql"sv), {
			schema::InputValue::Make(R"gql(id)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
		})
	});
}

} // namespace graphql::learn
