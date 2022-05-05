// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef BENCHMARKCLIENT_H
#define BENCHMARKCLIENT_H

#include "graphqlservice/GraphQLClient.h"
#include "graphqlservice/GraphQLParse.h"
#include "graphqlservice/GraphQLResponse.h"

#include "graphqlservice/internal/Version.h"

// Check if the library version is compatible with clientgen 4.2.0
static_assert(graphql::internal::MajorVersion == 4, "regenerate with clientgen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 2, "regenerate with clientgen: minor version mismatch");

#include <optional>
#include <string>
#include <vector>

namespace graphql::client {

/// <summary>
/// Operation: query (unnamed)
/// </summary>
/// <code class="language-graphql">
/// # Copyright (c) Microsoft Corporation. All rights reserved.
/// # Licensed under the MIT License.
/// 
/// query {
///   appointments {
///     pageInfo {
///       hasNextPage
///     }
///     edges {
///       node {
///         id
///         when
///         subject
///         isNow
///       }
///     }
///   }
/// }
/// </code>
namespace benchmark {

// Return the original text of the request document.
[[nodiscard]] const std::string& GetRequestText() noexcept;

// Return a pre-parsed, pre-validated request object.
[[nodiscard]] const peg::ast& GetRequestObject() noexcept;

} // namespace benchmark

namespace query::Query {

using benchmark::GetRequestText;
using benchmark::GetRequestObject;

// Return the name of this operation in the shared request document.
[[nodiscard]] const std::string& GetOperationName() noexcept;

struct [[nodiscard]] Response
{
	struct [[nodiscard]] appointments_AppointmentConnection
	{
		struct [[nodiscard]] pageInfo_PageInfo
		{
			bool hasNextPage {};
		};

		struct [[nodiscard]] edges_AppointmentEdge
		{
			struct [[nodiscard]] node_Appointment
			{
				response::IdType id {};
				std::optional<response::Value> when {};
				std::optional<std::string> subject {};
				bool isNow {};
			};

			std::optional<node_Appointment> node {};
		};

		pageInfo_PageInfo pageInfo {};
		std::optional<std::vector<std::optional<edges_AppointmentEdge>>> edges {};
	};

	appointments_AppointmentConnection appointments {};
};

[[nodiscard]] Response parseResponse(response::Value&& response);

} // namespace query::Query
} // namespace graphql::client

#endif // BENCHMARKCLIENT_H
