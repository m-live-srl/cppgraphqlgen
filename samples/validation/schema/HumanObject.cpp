// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "HumanObject.h"
#include "PetObject.h"

#include "graphqlservice/internal/Schema.h"

#include "graphqlservice/introspection/IntrospectionSchema.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::validation {
namespace object {

Human::Human(std::unique_ptr<const Concept>&& pimpl) noexcept
	: service::Object{ getTypeNames(), getResolvers() }
	, _pimpl { std::move(pimpl) }
{
}

service::TypeNames Human::getTypeNames() const noexcept
{
	return {
		R"gql(Sentient)gql"sv,
		R"gql(DogOrHuman)gql"sv,
		R"gql(HumanOrAlien)gql"sv,
		R"gql(Human)gql"sv
	};
}

service::ResolverMap Human::getResolvers() const noexcept
{
	return {
		{ R"gql(name)gql"sv, [this](service::ResolverParams&& params) { return resolveName(std::move(params)); } },
		{ R"gql(pets)gql"sv, [this](service::ResolverParams&& params) { return resolvePets(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } }
	};
}

void Human::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void Human::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

service::AwaitableResolver Human::resolveName(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getName(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<std::string>::convert(std::move(result), std::move(params));
}

service::AwaitableResolver Human::resolvePets(service::ResolverParams&& params) const
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = _pimpl->getPets(service::FieldParams(service::SelectionSetParams{ params }, std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Pet>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

service::AwaitableResolver Human::resolve_typename(service::ResolverParams&& params) const
{
	return service::Result<std::string>::convert(std::string{ R"gql(Human)gql" }, std::move(params));
}

} // namespace object

void AddHumanDetails(const std::shared_ptr<schema::ObjectType>& typeHuman, const std::shared_ptr<schema::Schema>& schema)
{
	typeHuman->AddInterfaces({
		std::static_pointer_cast<const schema::InterfaceType>(schema->LookupType(R"gql(Sentient)gql"sv))
	});
	typeHuman->AddFields({
		schema::Field::Make(R"gql(name)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv))),
		schema::Field::Make(R"gql(pets)gql"sv, R"md()md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(Pet)gql"sv)))))
	});
}

} // namespace graphql::validation
