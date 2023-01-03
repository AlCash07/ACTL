Callable traits
================================

:struct:`ac::callable_traits` provides information about a callable,
such as return and parameter types.
The following callables are supported:

* free functions;
* free function pointers;
* member function pointers (in this case the first parameter is the enclosing class);
* function objects with a single function call operator;
* const and reference-qualified versions of the above types.

This is a lightweight analog of `Boost.CallableTraits
<https://www.boost.org/doc/libs/1_78_0/libs/callable_traits/doc/html/index.html>`_
with limited functionality.

Reference
---------

The most convenient way to use the traits is through the following interface:

.. doxygentypedef:: ac::return_t
.. doxygenvariable:: ac::arity_v
.. doxygentypedef:: ac::parameter_at_t
.. doxygenvariable:: ac::is_noexcept_v

The same information is available as members of

.. doxygenstruct:: ac::callable_traits
.. doxygenstruct:: ac::free_function_traits
.. doxygenstruct:: ac::member_function_traits
.. doxygenstruct:: ac::function_object_traits

The following concepts are provided:

.. doxygenconcept:: ac::FreeFunction
.. doxygenconcept:: ac::MemberFunction
.. doxygenconcept:: ac::FunctionObject

:source:`Source code <functional/traits/callable_traits>`

:test:`Tests <functional/traits/callable_traits>`

Design
------

The following `reference <https://functionalcpp.wordpress.com/2013/08/05/function-traits/>`_
was used at first, but it was heavily expanded.
