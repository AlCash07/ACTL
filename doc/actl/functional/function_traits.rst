********************************
Function traits
********************************

:struct:`ac::function_traits` provides information about a function,
such as return and parameter types.
The following function types are supported:

* free functions;
* free function pointers;
* member function pointers (in this case the first parameter is the enclosing class);
* function objects with a single function call operator;
* const and reference-qualified versions of the above types.

This is a lightweight analog of `Boost.CallableTraits
<https://www.boost.org/doc/libs/1_85_0/libs/callable_traits/doc/html/index.html>`_
with limited functionality.

Reference
=========

The most convenient way to use the traits is through the following interface:

.. doxygentypedef:: ac::return_t
.. doxygenvariable:: ac::arity_v
.. doxygentypedef:: ac::parameter_at_t
.. doxygenvariable:: ac::is_noexcept_v

The same information is available as members of

.. doxygenstruct:: ac::function_traits

The following concepts are provided:

.. doxygenconcept:: ac::FreeFunction
.. doxygenconcept:: ac::MemberFunction
.. doxygenconcept:: ac::FunctionObject

:source:`Source code <functional/traits>`

:test:`Tests <functional/traits/callable_traits.cpp>`

Design
======

The following `reference <https://functionalcpp.wordpress.com/2013/08/05/function-traits/>`_
was used at first, but it was heavily expanded.

Qualified free functions
------------------------

`Boost.CallableTraits
<https://www.boost.org/doc/libs/1_85_0/libs/callable_traits/doc/html/index.html>`_
supports qualified free function types like

.. code::

  Return(Args...) const volatile &&

These types indeed exist in C++, but declaring a function like
`void f() const {}` results in a compilation error.
This is confirmed by
`cppreference <https://en.cppreference.com/w/cpp/language/function>`_:

  cv	-	const/volatile qualification, only allowed in non-static member function declarations

  ref	-	(since C++11) ref-qualification, only allowed in non-static member function declarations

It's not clear why such types exist if they cannot be used in a declaration.
For this reason, our function traits don't support them.
