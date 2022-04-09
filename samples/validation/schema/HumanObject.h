// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef HUMANOBJECT_H
#define HUMANOBJECT_H

#include "ValidationSchema.h"

namespace graphql::validation::object {
namespace implements {

template <class I>
concept HumanIs = std::is_same_v<I, Sentient> || std::is_same_v<I, DogOrHuman> || std::is_same_v<I, HumanOrAlien>;

} // namespace implements

namespace methods::HumanHas {

template <class TImpl>
concept getNameWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableScalar<std::string> { impl.getName(std::move(params)) } };
};

template <class TImpl>
concept getName = requires (TImpl impl)
{
	{ service::AwaitableScalar<std::string> { impl.getName() } };
};

template <class TImpl>
concept getPetsWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableObject<std::vector<std::shared_ptr<Pet>>> { impl.getPets(std::move(params)) } };
};

template <class TImpl>
concept getPets = requires (TImpl impl)
{
	{ service::AwaitableObject<std::vector<std::shared_ptr<Pet>>> { impl.getPets() } };
};

template <class TImpl>
concept beginSelectionSet = requires (TImpl impl, const service::SelectionSetParams params)
{
	{ impl.beginSelectionSet(params) };
};

template <class TImpl>
concept endSelectionSet = requires (TImpl impl, const service::SelectionSetParams params)
{
	{ impl.endSelectionSet(params) };
};

} // namespace methods::HumanHas

class Human final
	: public service::Object
{
private:
	service::AwaitableResolver resolveName(service::ResolverParams&& params) const;
	service::AwaitableResolver resolvePets(service::ResolverParams&& params) const;

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		virtual service::AwaitableScalar<std::string> getName(service::FieldParams&& params) const = 0;
		virtual service::AwaitableObject<std::vector<std::shared_ptr<Pet>>> getPets(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::AwaitableScalar<std::string> getName(service::FieldParams&& params) const final
		{
			if constexpr (methods::HumanHas::getNameWithParams<T>)
			{
				return { _pimpl->getName(std::move(params)) };
			}
			else if constexpr (methods::HumanHas::getName<T>)
			{
				return { _pimpl->getName() };
			}
			else
			{
				throw std::runtime_error(R"ex(Human::getName is not implemented)ex");
			}
		}

		service::AwaitableObject<std::vector<std::shared_ptr<Pet>>> getPets(service::FieldParams&& params) const final
		{
			if constexpr (methods::HumanHas::getPetsWithParams<T>)
			{
				return { _pimpl->getPets(std::move(params)) };
			}
			else if constexpr (methods::HumanHas::getPets<T>)
			{
				return { _pimpl->getPets() };
			}
			else
			{
				throw std::runtime_error(R"ex(Human::getPets is not implemented)ex");
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::HumanHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::HumanHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	Human(std::unique_ptr<const Concept>&& pimpl) noexcept;

	// Interfaces which this type implements
	friend Sentient;

	// Unions which include this type
	friend DogOrHuman;
	friend HumanOrAlien;

	template <class I>
	static constexpr bool implements() noexcept
	{
		return implements::HumanIs<I>;
	}

	service::TypeNames getTypeNames() const noexcept;
	service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const final;
	void endSelectionSet(const service::SelectionSetParams& params) const final;

	const std::unique_ptr<const Concept> _pimpl;

public:
	template <class T>
	Human(std::shared_ptr<T> pimpl) noexcept
		: Human { std::unique_ptr<const Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}

	constexpr static std::string_view static_typename = std::string_view(
		"Human"
	);
};

} // namespace graphql::validation::object

#endif // HUMANOBJECT_H
