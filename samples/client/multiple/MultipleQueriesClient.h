// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef MULTIPLEQUERIESCLIENT_H
#define MULTIPLEQUERIESCLIENT_H

#include "graphqlservice/GraphQLClient.h"
#include "graphqlservice/GraphQLParse.h"
#include "graphqlservice/GraphQLResponse.h"

#include "graphqlservice/internal/Version.h"

// Check if the library version is compatible with clientgen 4.5.0
static_assert(graphql::internal::MajorVersion == 4, "regenerate with clientgen: major version mismatch");
static_assert(graphql::internal::MinorVersion == 5, "regenerate with clientgen: minor version mismatch");

#include <optional>
#include <string>
#include <vector>

namespace graphql::client {

/// <summary>
/// Operations: query Appointments, query Tasks, query UnreadCounts, query Miscellaneous, mutation CompleteTaskMutation
/// </summary>
/// <code class="language-graphql">
/// # Copyright (c) Microsoft Corporation. All rights reserved.
/// # Licensed under the MIT License.
/// 
/// query Appointments {
///   appointments {
///     edges {
///       node {
///         id
///         subject
///         when
///         isNow
///         __typename
///       }
///     }
///   }
/// }
/// 
/// query Tasks {
///   tasks {
///     edges {
///       node {
///         id
///         title
///         isComplete
///         __typename
///       }
///     }
///   }
/// }
/// 
/// query UnreadCounts {
///   unreadCounts {
///     edges {
///       node {
///         id
///         name
///         unreadCount
///         __typename
///       }
///     }
///   }
/// }
/// 
/// query Miscellaneous {
///   # Read a field with an enum type
///   testTaskState
/// 
///   # Try a field with a union type
///   anyType(ids: ["ZmFrZVRhc2tJZA=="]) {
///     __typename
///     ...on Node {
///       id
///     }
///     ...on Task {
///       id
///       title
///       isComplete
///     }
///     ...on Appointment {
///       id
///       subject
///       when
///       isNow
///     }
///   }
/// 
///   # Try a field with a C++ keyword
///   default
/// }
/// 
/// mutation CompleteTaskMutation($input: CompleteTaskInput = {id: "ZmFrZVRhc2tJZA==", isComplete: true, clientMutationId: "Hi There!"}, $skipClientMutationId: Boolean!) {
///   completedTask: completeTask(input: $input) {
///     completedTask: task {
///       completedTaskId: id
///       title
///       isComplete
///     }
///     clientMutationId @skip(if: $skipClientMutationId)
///   }
/// }
/// </code>
namespace multiple {

// Return the original text of the request document.
[[nodiscard]] const std::string& GetRequestText() noexcept;

// Return a pre-parsed, pre-validated request object.
[[nodiscard]] const peg::ast& GetRequestObject() noexcept;

enum class [[nodiscard]] TaskState
{
	New,
	Started,
	Complete,
	Unassigned,
};

struct [[nodiscard]] CompleteTaskInput
{
	explicit CompleteTaskInput(
		response::IdType idArg = response::IdType {},
		std::optional<TaskState> testTaskStateArg = std::optional<TaskState> {},
		std::optional<bool> isCompleteArg = std::optional<bool> {},
		std::optional<std::string> clientMutationIdArg = std::optional<std::string> {}) noexcept;
	CompleteTaskInput(const CompleteTaskInput& other);
	CompleteTaskInput(CompleteTaskInput&& other) noexcept;

	CompleteTaskInput& operator=(const CompleteTaskInput& other);
	CompleteTaskInput& operator=(CompleteTaskInput&& other) noexcept;

	response::IdType id {};
	std::optional<TaskState> testTaskState {};
	std::optional<bool> isComplete {};
	std::optional<std::string> clientMutationId {};
};

} // namespace multiple

namespace query::Appointments {

using multiple::GetRequestText;
using multiple::GetRequestObject;

// Return the name of this operation in the shared request document.
[[nodiscard]] const std::string& GetOperationName() noexcept;

struct [[nodiscard]] Response
{
	struct [[nodiscard]] appointments_AppointmentConnection
	{
		struct [[nodiscard]] edges_AppointmentEdge
		{
			struct [[nodiscard]] node_Appointment
			{
				response::IdType id {};
				std::optional<std::string> subject {};
				std::optional<response::Value> when {};
				bool isNow {};
				std::string _typename {};
			};

			std::optional<node_Appointment> node {};
		};

		std::optional<std::vector<std::optional<edges_AppointmentEdge>>> edges {};
	};

	appointments_AppointmentConnection appointments {};
};

[[nodiscard]] Response parseResponse(response::Value&& response);

} // namespace query::Appointments

namespace query::Tasks {

using multiple::GetRequestText;
using multiple::GetRequestObject;

// Return the name of this operation in the shared request document.
[[nodiscard]] const std::string& GetOperationName() noexcept;

struct [[nodiscard]] Response
{
	struct [[nodiscard]] tasks_TaskConnection
	{
		struct [[nodiscard]] edges_TaskEdge
		{
			struct [[nodiscard]] node_Task
			{
				response::IdType id {};
				std::optional<std::string> title {};
				bool isComplete {};
				std::string _typename {};
			};

			std::optional<node_Task> node {};
		};

		std::optional<std::vector<std::optional<edges_TaskEdge>>> edges {};
	};

	tasks_TaskConnection tasks {};
};

[[nodiscard]] Response parseResponse(response::Value&& response);

} // namespace query::Tasks

namespace query::UnreadCounts {

using multiple::GetRequestText;
using multiple::GetRequestObject;

// Return the name of this operation in the shared request document.
[[nodiscard]] const std::string& GetOperationName() noexcept;

struct [[nodiscard]] Response
{
	struct [[nodiscard]] unreadCounts_FolderConnection
	{
		struct [[nodiscard]] edges_FolderEdge
		{
			struct [[nodiscard]] node_Folder
			{
				response::IdType id {};
				std::optional<std::string> name {};
				int unreadCount {};
				std::string _typename {};
			};

			std::optional<node_Folder> node {};
		};

		std::optional<std::vector<std::optional<edges_FolderEdge>>> edges {};
	};

	unreadCounts_FolderConnection unreadCounts {};
};

[[nodiscard]] Response parseResponse(response::Value&& response);

} // namespace query::UnreadCounts

namespace query::Miscellaneous {

using multiple::GetRequestText;
using multiple::GetRequestObject;

// Return the name of this operation in the shared request document.
[[nodiscard]] const std::string& GetOperationName() noexcept;

using multiple::TaskState;

struct [[nodiscard]] Response
{
	struct [[nodiscard]] anyType_UnionType
	{
		std::string _typename {};
		response::IdType id {};
		std::optional<std::string> title {};
		bool isComplete {};
		std::optional<std::string> subject {};
		std::optional<response::Value> when {};
		bool isNow {};
	};

	TaskState testTaskState {};
	std::vector<std::optional<anyType_UnionType>> anyType {};
	std::optional<std::string> default_ {};
};

[[nodiscard]] Response parseResponse(response::Value&& response);

} // namespace query::Miscellaneous

namespace mutation::CompleteTaskMutation {

using multiple::GetRequestText;
using multiple::GetRequestObject;

// Return the name of this operation in the shared request document.
[[nodiscard]] const std::string& GetOperationName() noexcept;

using multiple::TaskState;

using multiple::CompleteTaskInput;

struct [[nodiscard]] Variables
{
	std::unique_ptr<CompleteTaskInput> input {};
	bool skipClientMutationId {};
};

[[nodiscard]] response::Value serializeVariables(Variables&& variables);

struct [[nodiscard]] Response
{
	struct [[nodiscard]] completedTask_CompleteTaskPayload
	{
		struct [[nodiscard]] completedTask_Task
		{
			response::IdType completedTaskId {};
			std::optional<std::string> title {};
			bool isComplete {};
		};

		std::optional<completedTask_Task> completedTask {};
		std::optional<std::string> clientMutationId {};
	};

	completedTask_CompleteTaskPayload completedTask {};
};

[[nodiscard]] Response parseResponse(response::Value&& response);

} // namespace mutation::CompleteTaskMutation
} // namespace graphql::client

#endif // MULTIPLEQUERIESCLIENT_H
