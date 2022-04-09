// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "ValidationSchema.h"

namespace graphql::validation::object {
namespace methods::QueryHas {

template <class TImpl>
concept getDogWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableObject<std::shared_ptr<Dog>> { impl.getDog(std::move(params)) } };
};

template <class TImpl>
concept getDog = requires (TImpl impl)
{
	{ service::AwaitableObject<std::shared_ptr<Dog>> { impl.getDog() } };
};

template <class TImpl>
concept getHumanWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableObject<std::shared_ptr<Human>> { impl.getHuman(std::move(params)) } };
};

template <class TImpl>
concept getHuman = requires (TImpl impl)
{
	{ service::AwaitableObject<std::shared_ptr<Human>> { impl.getHuman() } };
};

template <class TImpl>
concept getPetWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableObject<std::shared_ptr<Pet>> { impl.getPet(std::move(params)) } };
};

template <class TImpl>
concept getPet = requires (TImpl impl)
{
	{ service::AwaitableObject<std::shared_ptr<Pet>> { impl.getPet() } };
};

template <class TImpl>
concept getCatOrDogWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableObject<std::shared_ptr<CatOrDog>> { impl.getCatOrDog(std::move(params)) } };
};

template <class TImpl>
concept getCatOrDog = requires (TImpl impl)
{
	{ service::AwaitableObject<std::shared_ptr<CatOrDog>> { impl.getCatOrDog() } };
};

template <class TImpl>
concept getArgumentsWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableObject<std::shared_ptr<Arguments>> { impl.getArguments(std::move(params)) } };
};

template <class TImpl>
concept getArguments = requires (TImpl impl)
{
	{ service::AwaitableObject<std::shared_ptr<Arguments>> { impl.getArguments() } };
};

template <class TImpl>
concept getResourceWithParams = requires (TImpl impl, service::FieldParams params)
{
	{ service::AwaitableObject<std::shared_ptr<Resource>> { impl.getResource(std::move(params)) } };
};

template <class TImpl>
concept getResource = requires (TImpl impl)
{
	{ service::AwaitableObject<std::shared_ptr<Resource>> { impl.getResource() } };
};

template <class TImpl>
concept getFindDogWithParams = requires (TImpl impl, service::FieldParams params, std::optional<ComplexInput> complexArg)
{
	{ service::AwaitableObject<std::shared_ptr<Dog>> { impl.getFindDog(std::move(params), std::move(complexArg)) } };
};

template <class TImpl>
concept getFindDog = requires (TImpl impl, std::optional<ComplexInput> complexArg)
{
	{ service::AwaitableObject<std::shared_ptr<Dog>> { impl.getFindDog(std::move(complexArg)) } };
};

template <class TImpl>
concept getBooleanListWithParams = requires (TImpl impl, service::FieldParams params, std::optional<std::vector<bool>> booleanListArgArg)
{
	{ service::AwaitableScalar<std::optional<bool>> { impl.getBooleanList(std::move(params), std::move(booleanListArgArg)) } };
};

template <class TImpl>
concept getBooleanList = requires (TImpl impl, std::optional<std::vector<bool>> booleanListArgArg)
{
	{ service::AwaitableScalar<std::optional<bool>> { impl.getBooleanList(std::move(booleanListArgArg)) } };
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

} // namespace methods::QueryHas

class Query final
	: public service::Object
{
private:
	service::AwaitableResolver resolveDog(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveHuman(service::ResolverParams&& params) const;
	service::AwaitableResolver resolvePet(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveCatOrDog(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveArguments(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveResource(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveFindDog(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveBooleanList(service::ResolverParams&& params) const;

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		virtual service::AwaitableObject<std::shared_ptr<Dog>> getDog(service::FieldParams&& params) const = 0;
		virtual service::AwaitableObject<std::shared_ptr<Human>> getHuman(service::FieldParams&& params) const = 0;
		virtual service::AwaitableObject<std::shared_ptr<Pet>> getPet(service::FieldParams&& params) const = 0;
		virtual service::AwaitableObject<std::shared_ptr<CatOrDog>> getCatOrDog(service::FieldParams&& params) const = 0;
		virtual service::AwaitableObject<std::shared_ptr<Arguments>> getArguments(service::FieldParams&& params) const = 0;
		virtual service::AwaitableObject<std::shared_ptr<Resource>> getResource(service::FieldParams&& params) const = 0;
		virtual service::AwaitableObject<std::shared_ptr<Dog>> getFindDog(service::FieldParams&& params, std::optional<ComplexInput>&& complexArg) const = 0;
		virtual service::AwaitableScalar<std::optional<bool>> getBooleanList(service::FieldParams&& params, std::optional<std::vector<bool>>&& booleanListArgArg) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::AwaitableObject<std::shared_ptr<Dog>> getDog(service::FieldParams&& params) const final
		{
			if constexpr (methods::QueryHas::getDogWithParams<T>)
			{
				return { _pimpl->getDog(std::move(params)) };
			}
			else if constexpr (methods::QueryHas::getDog<T>)
			{
				return { _pimpl->getDog() };
			}
			else
			{
				throw std::runtime_error(R"ex(Query::getDog is not implemented)ex");
			}
		}

		service::AwaitableObject<std::shared_ptr<Human>> getHuman(service::FieldParams&& params) const final
		{
			if constexpr (methods::QueryHas::getHumanWithParams<T>)
			{
				return { _pimpl->getHuman(std::move(params)) };
			}
			else if constexpr (methods::QueryHas::getHuman<T>)
			{
				return { _pimpl->getHuman() };
			}
			else
			{
				throw std::runtime_error(R"ex(Query::getHuman is not implemented)ex");
			}
		}

		service::AwaitableObject<std::shared_ptr<Pet>> getPet(service::FieldParams&& params) const final
		{
			if constexpr (methods::QueryHas::getPetWithParams<T>)
			{
				return { _pimpl->getPet(std::move(params)) };
			}
			else if constexpr (methods::QueryHas::getPet<T>)
			{
				return { _pimpl->getPet() };
			}
			else
			{
				throw std::runtime_error(R"ex(Query::getPet is not implemented)ex");
			}
		}

		service::AwaitableObject<std::shared_ptr<CatOrDog>> getCatOrDog(service::FieldParams&& params) const final
		{
			if constexpr (methods::QueryHas::getCatOrDogWithParams<T>)
			{
				return { _pimpl->getCatOrDog(std::move(params)) };
			}
			else if constexpr (methods::QueryHas::getCatOrDog<T>)
			{
				return { _pimpl->getCatOrDog() };
			}
			else
			{
				throw std::runtime_error(R"ex(Query::getCatOrDog is not implemented)ex");
			}
		}

		service::AwaitableObject<std::shared_ptr<Arguments>> getArguments(service::FieldParams&& params) const final
		{
			if constexpr (methods::QueryHas::getArgumentsWithParams<T>)
			{
				return { _pimpl->getArguments(std::move(params)) };
			}
			else if constexpr (methods::QueryHas::getArguments<T>)
			{
				return { _pimpl->getArguments() };
			}
			else
			{
				throw std::runtime_error(R"ex(Query::getArguments is not implemented)ex");
			}
		}

		service::AwaitableObject<std::shared_ptr<Resource>> getResource(service::FieldParams&& params) const final
		{
			if constexpr (methods::QueryHas::getResourceWithParams<T>)
			{
				return { _pimpl->getResource(std::move(params)) };
			}
			else if constexpr (methods::QueryHas::getResource<T>)
			{
				return { _pimpl->getResource() };
			}
			else
			{
				throw std::runtime_error(R"ex(Query::getResource is not implemented)ex");
			}
		}

		service::AwaitableObject<std::shared_ptr<Dog>> getFindDog(service::FieldParams&& params, std::optional<ComplexInput>&& complexArg) const final
		{
			if constexpr (methods::QueryHas::getFindDogWithParams<T>)
			{
				return { _pimpl->getFindDog(std::move(params), std::move(complexArg)) };
			}
			else if constexpr (methods::QueryHas::getFindDog<T>)
			{
				return { _pimpl->getFindDog(std::move(complexArg)) };
			}
			else
			{
				throw std::runtime_error(R"ex(Query::getFindDog is not implemented)ex");
			}
		}

		service::AwaitableScalar<std::optional<bool>> getBooleanList(service::FieldParams&& params, std::optional<std::vector<bool>>&& booleanListArgArg) const final
		{
			if constexpr (methods::QueryHas::getBooleanListWithParams<T>)
			{
				return { _pimpl->getBooleanList(std::move(params), std::move(booleanListArgArg)) };
			}
			else if constexpr (methods::QueryHas::getBooleanList<T>)
			{
				return { _pimpl->getBooleanList(std::move(booleanListArgArg)) };
			}
			else
			{
				throw std::runtime_error(R"ex(Query::getBooleanList is not implemented)ex");
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::QueryHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::QueryHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	Query(std::unique_ptr<const Concept>&& pimpl) noexcept;

	service::TypeNames getTypeNames() const noexcept;
	service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const final;
	void endSelectionSet(const service::SelectionSetParams& params) const final;

	const std::unique_ptr<const Concept> _pimpl;

public:
	template <class T>
	Query(std::shared_ptr<T> pimpl) noexcept
		: Query { std::unique_ptr<const Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}

	constexpr static std::string_view static_typename = std::string_view(
		"Query"
	);
};

} // namespace graphql::validation::object

#endif // QUERYOBJECT_H
