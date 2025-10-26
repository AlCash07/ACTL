********************************
Function traits
********************************

Function traits provide the following inspection and synthesis utilities.

For traits specific to member functions, see :doc:`member_traits`.

============================== =============================== =================================
Trait name                     Example 1                       Example 2                        
============================== =============================== =================================
Input type                     `float(int, int) noexcept`      `void(const char*, ...)`         
`return_t`                     `float`                         `void`                           
`returns_void_v`               `false`                         `true`                           
`parameters_t`                 `type_list<int, int>`           `type_list<const char*>`         
`arity_v`                      `2`                             `1`                              
`parameter_at_t` for 0         `int`                           `const char*`                    
`accepts_variadic_arguments_v` `false`                         `true`                           
`add_variadic_arguments_t`     `float(int, int, ...) noexcept` `void(const char*, ...)`         
`remove_variadic_arguments_t`  `float(int, int) noexcept`      `void(const char*)`              
`is_noexcept_v`                `true`                          `false`                          
`add_noexcept_t`               `float(int, int) noexcept`      `void(const char*, ...) noexcept`
`remove_noexcept_t`            `float(int, int)`               `void(const char*, ...)`         
============================== =============================== =================================

Traits above are visualized on the free function examples for simplicify,
but the following categories of functions are also supported:

* free functions;
* free `function pointers <https://en.cppreference.com/w/cpp/language/pointer.html#Pointers_to_functions>`_;
* member function pointers (in this case the first parameter is the enclosing class);
* function objects with a non-overloaded function call operator;
* const and reference-qualified versions of the above types.

This is a lightweight analog of `Boost.CallableTraits
<https://www.boost.org/doc/libs/latest/libs/callable_traits/doc/html/index.html>`_
with limited functionality.

Reference
=========

Concepts
--------

.. ac-include:: actl/functional/traits/FreeFunction.hpp
.. doxygenconcept:: ac::FreeFunction

.. ac-include:: actl/functional/traits/MemberFunction.hpp
.. doxygenconcept:: ac::MemberFunction

.. ac-include:: actl/functional/traits/FunctionObject.hpp
.. doxygenconcept:: ac::FunctionObject

Traits
------

.. ac-include:: actl/functional/traits/parameters.hpp
.. doxygentypedef:: ac::return_t
.. doxygenvariable:: ac::returns_void_v
.. doxygentypedef:: ac::parameters_t
.. doxygenvariable:: ac::arity_v
.. doxygentypedef:: ac::parameter_at_t

.. ac-include:: actl/functional/traits/variadic_arguments.hpp
.. doxygenvariable:: ac::accepts_variadic_arguments_v
.. doxygentypedef:: ac::add_variadic_arguments_t
.. doxygentypedef:: ac::remove_variadic_arguments_t

.. ac-include:: actl/functional/traits/noexcept.hpp
.. doxygenvariable:: ac::is_noexcept_v
.. doxygentypedef:: ac::add_noexcept_t
.. doxygentypedef:: ac::remove_noexcept_t

.. ac-include:: actl/functional/traits/FreeFunction.hpp
.. doxygentypedef:: ac::as_free_function_t

.. ac-tests:: tests/functional/traits

Design
======

The following `reference <https://functionalcpp.wordpress.com/2013/08/05/function-traits/>`_
was used at first, but it was heavily expanded.

Qualified free functions
------------------------

`Boost.CallableTraits
<https://www.boost.org/doc/libs/latest/libs/callable_traits/doc/html/index.html>`_
supports qualified free function types like

.. code::

  Return(Args...) const volatile &

These types indeed exist in C++, but declaring a function like
`void f() const {}` results in a compilation error.
This is confirmed by
`cppreference <https://en.cppreference.com/w/cpp/language/function>`_:

  cv	-	const/volatile qualification, only allowed in non-static member function declarations

  ref	-	(since C++11) ref-qualification, only allowed in non-static member function declarations

It's not clear why such types exist if they cannot be used in a declaration.
For this reason, we don't support them.
