// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "MutateClient.h"

#include "graphqlservice/internal/SortedMap.h"

#include <algorithm>
#include <array>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>

using namespace std::literals;

namespace graphql::client {
namespace mutate {

const std::string& GetRequestText() noexcept
{
	static const auto s_request = R"gql(
		# Copyright (c) Microsoft Corporation. All rights reserved.
		# Licensed under the MIT License.
		
		mutation CompleteTaskMutation($input: CompleteTaskInput = {id: "ZmFrZVRhc2tJZA==", isComplete: true, clientMutationId: "Hi There!", testTaskState: Unassigned}, $skipClientMutationId: Boolean!) {
		  completedTask: completeTask(input: $input) {
		    completedTask: task {
		      completedTaskId: id
		      title
		      isComplete
		    }
		    clientMutationId @skip(if: $skipClientMutationId)
		  }
		}
	)gql"s;

	return s_request;
}

const peg::ast& GetRequestObject() noexcept
{
	static const auto s_request = []() noexcept {
		auto ast = peg::parseString(GetRequestText());

		// This has already been validated against the schema by clientgen.
		ast.validated = true;

		return ast;
	}();

	return s_request;
}

CompleteTaskInput::CompleteTaskInput(
		response::IdType idArg,
		std::optional<TaskState> testTaskStateArg,
		std::optional<bool> isCompleteArg,
		std::optional<std::string> clientMutationIdArg) noexcept
	: id { std::move(idArg) }
	, testTaskState { std::move(testTaskStateArg) }
	, isComplete { std::move(isCompleteArg) }
	, clientMutationId { std::move(clientMutationIdArg) }
{
}

CompleteTaskInput::CompleteTaskInput(const CompleteTaskInput& other)
	: id { ModifiedVariable<response::IdType>::duplicate(other.id) }
	, testTaskState { ModifiedVariable<TaskState>::duplicate<TypeModifier::Nullable>(other.testTaskState) }
	, isComplete { ModifiedVariable<bool>::duplicate<TypeModifier::Nullable>(other.isComplete) }
	, clientMutationId { ModifiedVariable<std::string>::duplicate<TypeModifier::Nullable>(other.clientMutationId) }
{
}

CompleteTaskInput::CompleteTaskInput(CompleteTaskInput&& other) noexcept
	: id { std::move(other.id) }
	, testTaskState { std::move(other.testTaskState) }
	, isComplete { std::move(other.isComplete) }
	, clientMutationId { std::move(other.clientMutationId) }
{
}

CompleteTaskInput& CompleteTaskInput::operator=(const CompleteTaskInput& other)
{
	return *this = CompleteTaskInput { other };
}

CompleteTaskInput& CompleteTaskInput::operator=(CompleteTaskInput&& other) noexcept
{
	id = std::move(other.id);
	testTaskState = std::move(other.testTaskState);
	isComplete = std::move(other.isComplete);
	clientMutationId = std::move(other.clientMutationId);

	return *this;
}

} // namespace mutate

using namespace mutate;

template <>
response::Value Variable<TaskState>::serialize(TaskState&& value)
{
	static const std::array<std::string_view, 4> s_names = {
		R"gql(Unassigned)gql"sv,
		R"gql(New)gql"sv,
		R"gql(Started)gql"sv,
		R"gql(Complete)gql"sv
	};

	response::Value result { response::Type::EnumValue };

	result.set<std::string>(std::string { s_names[static_cast<size_t>(value)] });

	return result;
}

template <>
response::Value Variable<CompleteTaskInput>::serialize(CompleteTaskInput&& inputValue)
{
	response::Value result { response::Type::Map };

	result.emplace_back(R"js(id)js"s, ModifiedVariable<response::IdType>::serialize(std::move(inputValue.id)));
	result.emplace_back(R"js(testTaskState)js"s, ModifiedVariable<TaskState>::serialize<TypeModifier::Nullable>(std::move(inputValue.testTaskState)));
	result.emplace_back(R"js(isComplete)js"s, ModifiedVariable<bool>::serialize<TypeModifier::Nullable>(std::move(inputValue.isComplete)));
	result.emplace_back(R"js(clientMutationId)js"s, ModifiedVariable<std::string>::serialize<TypeModifier::Nullable>(std::move(inputValue.clientMutationId)));

	return result;
}

template <>
TaskState Response<TaskState>::parse(response::Value&& value)
{
	if (!value.maybe_enum())
	{
		throw std::logic_error { R"ex(not a valid TaskState value)ex" };
	}

	static const std::array<std::pair<std::string_view, TaskState>, 4> s_values = {
		std::make_pair(R"gql(New)gql"sv, TaskState::New),
		std::make_pair(R"gql(Started)gql"sv, TaskState::Started),
		std::make_pair(R"gql(Complete)gql"sv, TaskState::Complete),
		std::make_pair(R"gql(Unassigned)gql"sv, TaskState::Unassigned)
	};

	const auto result = internal::sorted_map_lookup<internal::shorter_or_less>(
		s_values,
		std::string_view { value.get<std::string>() });

	if (!result)
	{
		throw std::logic_error { R"ex(not a valid TaskState value)ex" };
	}

	return *result;
}

template <>
mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload::completedTask_Task Response<mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload::completedTask_Task>::parse(response::Value&& response)
{
	mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload::completedTask_Task result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(completedTaskId)js"sv)
			{
				result.completedTaskId = ModifiedResponse<response::IdType>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(title)js"sv)
			{
				result.title = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(isComplete)js"sv)
			{
				result.isComplete = ModifiedResponse<bool>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload Response<mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload>::parse(response::Value&& response)
{
	mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(completedTask)js"sv)
			{
				result.completedTask = ModifiedResponse<mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload::completedTask_Task>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(clientMutationId)js"sv)
			{
				result.clientMutationId = ModifiedResponse<std::string>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

namespace mutation::CompleteTaskMutation {

const std::string& GetOperationName() noexcept
{
	static const auto s_name = R"gql(CompleteTaskMutation)gql"s;

	return s_name;
}

response::Value serializeVariables(Variables&& variables)
{
	response::Value result { response::Type::Map };

	result.emplace_back(R"js(input)js"s, ModifiedVariable<CompleteTaskInput>::serialize<TypeModifier::Nullable>(std::move(variables.input)));
	result.emplace_back(R"js(skipClientMutationId)js"s, ModifiedVariable<bool>::serialize(std::move(variables.skipClientMutationId)));

	return result;
}

Response parseResponse(response::Value&& response)
{
	Response result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(completedTask)js"sv)
			{
				result.completedTask = ModifiedResponse<mutation::CompleteTaskMutation::Response::completedTask_CompleteTaskPayload>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

[[nodiscard]] const std::string& Traits::GetRequestText() noexcept
{
	return mutate::GetRequestText();
}

[[nodiscard]] const peg::ast& Traits::GetRequestObject() noexcept
{
	return mutate::GetRequestObject();
}

[[nodiscard]] const std::string& Traits::GetOperationName() noexcept
{
	return CompleteTaskMutation::GetOperationName();
}

[[nodiscard]] response::Value Traits::serializeVariables(Traits::Variables&& variables)
{
	return CompleteTaskMutation::serializeVariables(std::move(variables));
}

[[nodiscard]] Traits::Response Traits::parseResponse(response::Value&& response)
{
	return CompleteTaskMutation::parseResponse(std::move(response));
}

} // namespace mutation::CompleteTaskMutation
} // namespace graphql::client
