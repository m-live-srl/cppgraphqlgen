// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "graphqlservice/internal/Introspection.h"

#include <algorithm>
#include <array>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <vector>

using namespace std::literals;

namespace graphql {
namespace service {

static const std::array<std::string_view, 8> s_namesTypeKind = {
	R"gql(SCALAR)gql"sv,
	R"gql(OBJECT)gql"sv,
	R"gql(INTERFACE)gql"sv,
	R"gql(UNION)gql"sv,
	R"gql(ENUM)gql"sv,
	R"gql(INPUT_OBJECT)gql"sv,
	R"gql(LIST)gql"sv,
	R"gql(NON_NULL)gql"sv
};

template <>
introspection::TypeKind ModifiedArgument<introspection::TypeKind>::convert(const response::Value& value)
{
	if (!value.maybe_enum())
	{
		throw service::schema_exception { { R"ex(not a valid __TypeKind value)ex" } };
	}

	const auto itr = std::find(s_namesTypeKind.cbegin(), s_namesTypeKind.cend(), value.get<std::string>());

	if (itr == s_namesTypeKind.cend())
	{
		throw service::schema_exception { { R"ex(not a valid __TypeKind value)ex" } };
	}

	return static_cast<introspection::TypeKind>(itr - s_namesTypeKind.cbegin());
}

template <>
service::AwaitableResolver ModifiedResult<introspection::TypeKind>::convert(service::FieldResult<introspection::TypeKind> result, ResolverParams params)
{
	return resolve(std::move(result), std::move(params),
		[](introspection::TypeKind value, const ResolverParams&)
		{
			response::Value result(response::Type::EnumValue);

			result.set<std::string>(std::string { s_namesTypeKind[static_cast<size_t>(value)] });

			return result;
		});
}

static const std::array<std::string_view, 19> s_namesDirectiveLocation = {
	R"gql(QUERY)gql"sv,
	R"gql(MUTATION)gql"sv,
	R"gql(SUBSCRIPTION)gql"sv,
	R"gql(FIELD)gql"sv,
	R"gql(FRAGMENT_DEFINITION)gql"sv,
	R"gql(FRAGMENT_SPREAD)gql"sv,
	R"gql(INLINE_FRAGMENT)gql"sv,
	R"gql(VARIABLE_DEFINITION)gql"sv,
	R"gql(SCHEMA)gql"sv,
	R"gql(SCALAR)gql"sv,
	R"gql(OBJECT)gql"sv,
	R"gql(FIELD_DEFINITION)gql"sv,
	R"gql(ARGUMENT_DEFINITION)gql"sv,
	R"gql(INTERFACE)gql"sv,
	R"gql(UNION)gql"sv,
	R"gql(ENUM)gql"sv,
	R"gql(ENUM_VALUE)gql"sv,
	R"gql(INPUT_OBJECT)gql"sv,
	R"gql(INPUT_FIELD_DEFINITION)gql"sv
};

template <>
introspection::DirectiveLocation ModifiedArgument<introspection::DirectiveLocation>::convert(const response::Value& value)
{
	if (!value.maybe_enum())
	{
		throw service::schema_exception { { R"ex(not a valid __DirectiveLocation value)ex" } };
	}

	const auto itr = std::find(s_namesDirectiveLocation.cbegin(), s_namesDirectiveLocation.cend(), value.get<std::string>());

	if (itr == s_namesDirectiveLocation.cend())
	{
		throw service::schema_exception { { R"ex(not a valid __DirectiveLocation value)ex" } };
	}

	return static_cast<introspection::DirectiveLocation>(itr - s_namesDirectiveLocation.cbegin());
}

template <>
service::AwaitableResolver ModifiedResult<introspection::DirectiveLocation>::convert(service::FieldResult<introspection::DirectiveLocation> result, ResolverParams params)
{
	return resolve(std::move(result), std::move(params),
		[](introspection::DirectiveLocation value, const ResolverParams&)
		{
			response::Value result(response::Type::EnumValue);

			result.set<std::string>(std::string { s_namesDirectiveLocation[static_cast<size_t>(value)] });

			return result;
		});
}

} // namespace service

namespace introspection {

void AddTypesToSchema(const std::shared_ptr<schema::Schema>& schema)
{
	schema->AddType(R"gql(Boolean)gql"sv, schema::ScalarType::Make(R"gql(Boolean)gql"sv, R"md(Built-in type)md"sv, R"url(https://spec.graphql.org/October2021/#sec-Boolean)url"sv));
	schema->AddType(R"gql(Float)gql"sv, schema::ScalarType::Make(R"gql(Float)gql"sv, R"md(Built-in type)md"sv, R"url(https://spec.graphql.org/October2021/#sec-Float)url"sv));
	schema->AddType(R"gql(ID)gql"sv, schema::ScalarType::Make(R"gql(ID)gql"sv, R"md(Built-in type)md"sv, R"url(https://spec.graphql.org/October2021/#sec-ID)url"sv));
	schema->AddType(R"gql(Int)gql"sv, schema::ScalarType::Make(R"gql(Int)gql"sv, R"md(Built-in type)md"sv, R"url(https://spec.graphql.org/October2021/#sec-Int)url"sv));
	schema->AddType(R"gql(String)gql"sv, schema::ScalarType::Make(R"gql(String)gql"sv, R"md(Built-in type)md"sv, R"url(https://spec.graphql.org/October2021/#sec-String)url"sv));
	auto typeTypeKind = schema::EnumType::Make(R"gql(__TypeKind)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(__TypeKind)gql"sv, typeTypeKind);
	auto typeDirectiveLocation = schema::EnumType::Make(R"gql(__DirectiveLocation)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(__DirectiveLocation)gql"sv, typeDirectiveLocation);
	auto typeSchema = schema::ObjectType::Make(R"gql(__Schema)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(__Schema)gql"sv, typeSchema);
	auto typeType = schema::ObjectType::Make(R"gql(__Type)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(__Type)gql"sv, typeType);
	auto typeField = schema::ObjectType::Make(R"gql(__Field)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(__Field)gql"sv, typeField);
	auto typeInputValue = schema::ObjectType::Make(R"gql(__InputValue)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(__InputValue)gql"sv, typeInputValue);
	auto typeEnumValue = schema::ObjectType::Make(R"gql(__EnumValue)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(__EnumValue)gql"sv, typeEnumValue);
	auto typeDirective = schema::ObjectType::Make(R"gql(__Directive)gql"sv, R"md()md"sv);
	schema->AddType(R"gql(__Directive)gql"sv, typeDirective);

	typeTypeKind->AddEnumValues({
		{ service::s_namesTypeKind[static_cast<size_t>(introspection::TypeKind::SCALAR)], R"md()md"sv, std::nullopt },
		{ service::s_namesTypeKind[static_cast<size_t>(introspection::TypeKind::OBJECT)], R"md()md"sv, std::nullopt },
		{ service::s_namesTypeKind[static_cast<size_t>(introspection::TypeKind::INTERFACE)], R"md()md"sv, std::nullopt },
		{ service::s_namesTypeKind[static_cast<size_t>(introspection::TypeKind::UNION)], R"md()md"sv, std::nullopt },
		{ service::s_namesTypeKind[static_cast<size_t>(introspection::TypeKind::ENUM)], R"md()md"sv, std::nullopt },
		{ service::s_namesTypeKind[static_cast<size_t>(introspection::TypeKind::INPUT_OBJECT)], R"md()md"sv, std::nullopt },
		{ service::s_namesTypeKind[static_cast<size_t>(introspection::TypeKind::LIST)], R"md()md"sv, std::nullopt },
		{ service::s_namesTypeKind[static_cast<size_t>(introspection::TypeKind::NON_NULL)], R"md()md"sv, std::nullopt }
	});
	typeDirectiveLocation->AddEnumValues({
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::QUERY)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::MUTATION)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::SUBSCRIPTION)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::FIELD)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::FRAGMENT_DEFINITION)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::FRAGMENT_SPREAD)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::INLINE_FRAGMENT)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::VARIABLE_DEFINITION)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::SCHEMA)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::SCALAR)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::OBJECT)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::FIELD_DEFINITION)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::ARGUMENT_DEFINITION)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::INTERFACE)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::UNION)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::ENUM)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::ENUM_VALUE)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::INPUT_OBJECT)], R"md()md"sv, std::nullopt },
		{ service::s_namesDirectiveLocation[static_cast<size_t>(introspection::DirectiveLocation::INPUT_FIELD_DEFINITION)], R"md()md"sv, std::nullopt }
	});

	AddSchemaDetails(typeSchema, schema);
	AddTypeDetails(typeType, schema);
	AddFieldDetails(typeField, schema);
	AddInputValueDetails(typeInputValue, schema);
	AddEnumValueDetails(typeEnumValue, schema);
	AddDirectiveDetails(typeDirective, schema);

	schema->AddDirective(schema::Directive::Make(R"gql(skip)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::FIELD,
		introspection::DirectiveLocation::FRAGMENT_SPREAD,
		introspection::DirectiveLocation::INLINE_FRAGMENT
	}, {
		schema::InputValue::Make(R"gql(if)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(Boolean)gql"sv)), R"gql()gql"sv)
	}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(include)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::FIELD,
		introspection::DirectiveLocation::FRAGMENT_SPREAD,
		introspection::DirectiveLocation::INLINE_FRAGMENT
	}, {
		schema::InputValue::Make(R"gql(if)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(Boolean)gql"sv)), R"gql()gql"sv)
	}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(deprecated)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::FIELD_DEFINITION,
		introspection::DirectiveLocation::ENUM_VALUE
	}, {
		schema::InputValue::Make(R"gql(reason)gql"sv, R"md()md"sv, schema->LookupType(R"gql(String)gql"sv), R"gql("No longer supported")gql"sv)
	}, false));
	schema->AddDirective(schema::Directive::Make(R"gql(specifiedBy)gql"sv, R"md()md"sv, {
		introspection::DirectiveLocation::SCALAR
	}, {
		schema::InputValue::Make(R"gql(url)gql"sv, R"md()md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType(R"gql(String)gql"sv)), R"gql()gql"sv)
	}, false));
}

} // namespace introspection
} // namespace graphql
