# Common Field Parameters

The `resolveField` methods generated by `schemagen` will unpack any arguments
matching the `schema` from the `query` and pass those to the `getField` method
defined by the implementer. However, the implementer might need to inspect
shared state or `directives` from the `query`, so the `resolveField` method
also packs that information into a `graphql::service::FieldParams` struct and
passes it to every `getField` method as the first parameter.

This parameter is optional. The type-erased implementation of `graphql::service::Object`
for each `Object` type in the schema declares a pair of `methods::ObjectHas::getFieldWithParams`
and `methods::ObjectHas::getField` concepts for each field getter. If the implementation
type supports passing the `graphql::service::FieldParams` struct as the first parameter,
the type-erased `object::Object` invokes the `getFieldWithParams` version, otherwise it
drops the parameter and calls the `getField` version with whatever other field
arguments the schema specified.

## Details of Field Parameters

The `graphql::service::FieldParams` struct is declared in [GraphQLService.h](../include/graphqlservice/GraphQLService.h):
```cpp
// Pass a common bundle of parameters to all of the generated Object::getField accessors in a
// SelectionSet
struct [[nodiscard]] SelectionSetParams
{
	// Context for this selection set.
	const ResolverContext resolverContext;

	// The lifetime of each of these borrowed references is guaranteed until the future returned
	// by the accessor is resolved or destroyed. They are owned by the OperationData shared pointer.
	const std::shared_ptr<RequestState>& state;
	const Directives& operationDirectives;
	const std::shared_ptr<FragmentDefinitionDirectiveStack> fragmentDefinitionDirectives;

	// Fragment directives are shared for all fields in that fragment, but they aren't kept alive
	// after the call to the last accessor in the fragment. If you need to keep them alive longer,
	// you'll need to explicitly copy them into other instances of Directives.
	const std::shared_ptr<FragmentSpreadDirectiveStack> fragmentSpreadDirectives;
	const std::shared_ptr<FragmentSpreadDirectiveStack> inlineFragmentDirectives;

	// Field error path to this selection set.
	std::optional<field_path> errorPath;

	// Async launch policy for sub-field resolvers.
	const await_async launch {};
};

// Pass a common bundle of parameters to all of the generated Object::getField accessors.
struct [[nodiscard]] FieldParams : SelectionSetParams
{
	GRAPHQLSERVICE_EXPORT explicit FieldParams(
		SelectionSetParams&& selectionSetParams, Directives directives);

	// Each field owns its own field-specific directives. Once the accessor returns it will be
	// destroyed, but you can move it into another instance of response::Value to keep it alive
	// longer.
	Directives fieldDirectives;
};
```

### Resolver Context

The `SelectionSetParams::resolverContext` enum member informs the `getField`
accessors about what type of operation is being resolved:
```cpp
// Resolvers may be called in multiple different Operation contexts.
enum class [[nodiscard]] ResolverContext {
	// Resolving a Query operation.
	Query,

	// Resolving a Mutation operation.
	Mutation,

	// Adding a Subscription. If you need to prepare to send events for this Subsciption
	// (e.g. registering an event sink of your own), this is a chance to do that.
	NotifySubscribe,

	// Resolving a Subscription event.
	Subscription,

	// Removing a Subscription. If there are no more Subscriptions registered this is an
	// opportunity to release resources which are no longer needed.
	NotifyUnsubscribe,
};
```

### Request State

The `SelectionSetParams::state` member is a reference to the
`std::shared_ptr<graphql::service::RequestState>` parameter passed to
`Request::resolve` (see [resolvers.md](./resolvers.md) for more info):
```cpp
// The RequestState is nullable, but if you have multiple threads processing requests and there's
// any per-request state that you want to maintain throughout the request (e.g. optimizing or
// batching backend requests), you can inherit from RequestState and pass it to Request::resolve to
// correlate the asynchronous/recursive callbacks and accumulate state in it.
struct [[nodiscard]] RequestState : std::enable_shared_from_this<RequestState>
{
	virtual ~RequestState() = default;
};
```

### Scoped Directives

Each of the `directives` members contains the values of the `directives` and
any of their arguments which were in effect at that scope of the `query`.
Implementers may inspect those values in the call to `getField` and alter their
behavior based on those custom `directives`:
```cpp
// Directive order matters, and some of them are repeatable. So rather than passing them in a
// response::Value, pass directives in something like the underlying response::MapType which
// preserves the order of the elements without complete uniqueness.
using Directives = std::vector<std::pair<std::string_view, response::Value>>;

// Traversing a fragment spread adds a new set of directives.
using FragmentDefinitionDirectiveStack = std::list<std::reference_wrapper<const Directives>>;
using FragmentSpreadDirectiveStack = std::list<Directives>;
```

As noted in the comments, the `fragmentSpreadDirectives` and
`inlineFragmentDirectives` are stacks of directives passed down through nested
inline fragments and fragment spreads. The `Directives` object for each frame of
the stack is shared accross calls to multiple `getField` methods in a single fragment,
but they will be popped from the stack when the last field has been visited. The
`fieldDirectives` member is passed by value and is not shared with other `getField`
method calls. It's up to the implementer to capture the values in these `directives`
which they might need for asynchronous evaulation after the call to the current
`getField` method has returned.

The implementer does not need to capture the values of `operationDirectives`
or `fragmentDefinitionDirectives` because those are kept alive until the
`operation` and all of its `std::future` results are resolved. Although they
passed by `const` reference, the reference should always be valid as long as
there's a pending result from the `getField` call.

### Error Path

The `SelectionSetParams::errorPath` member should be considered an opaque
implementation detail by client code. It automatically propagates through the
field resolvers, and if there is a schema exception or one of the `getField`
accessors throws another exception derived from `std::exception`, the
`graphqlservice` library will automatically add the resulting path to the error
report, accoring to the [spec](https://spec.graphql.org/October2021/#sec-Errors).

### Launch Policy

See the [Awaitable](./awaitable.md) document for more information about
`service::await_async`.

## Related Documents

1. The `getField` methods are discussed in more detail in [resolvers.md](./resolvers.md).
2. Awaitable types are covered in [awaitable.md](./awaitable.md).
3. Built-in and custom `directives` are discussed in [directives.md](./directives.md).
4. Subscription resolvers may be called 2 extra times, inside of `subscribe` and `unsubscribe`.
See [subscriptions.md](./subscriptions.md) for more details.
