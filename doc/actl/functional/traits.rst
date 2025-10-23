********************************
Function traits
********************************

Function traits provide the following inspection and synthesis utilities.

For traits specific to member functions, see :doc:`traits/member_traits`.

============================== =============================== ================================
Trait name                     Example 1                       Example 2                       
============================== =============================== ================================
Input type                     `void(int, char) noexcept`      `int(const char*, ...)`         
`return_t`                     `void`                          `int`                           
`parameters_t`                 `type_list<int, char>`          `type_list<const char*>`        
`arity_v`                      `2`                             `1`                             
`parameter_at_t` for 0         `int`                           `const char*`                   
`is_noexcept_v`                `true`                          `false`                         
`add_noexcept_t`               `void(int, char) noexcept`      `int(const char*, ...) noexcept`
`remove_noexcept_t`            `void(int, char)`               `int(const char*, ...)`         
`accepts_variadic_arguments_v` `false`                         `true`                          
`add_variadic_arguments_t`     `void(int, char, ...) noexcept` `int(const char*, ...)`         
`remove_variadic_arguments_t`  `void(int, char) noexcept`      `int(const char*)`              
============================== =============================== ================================

Traits above are visualized on the free function examples for simplicify,
but the following categories of functions are also supported:

* free functions;
* free function pointers;
* member function pointers (in this case the first parameter is the enclosing class);
* function objects with a single function call operator;
* const and reference-qualified versions of the above types.

This is a lightweight analog of `Boost.CallableTraits
<https://www.boost.org/doc/libs/latest/libs/callable_traits/doc/html/index.html>`_
with limited functionality.

Reference
=========

The most convenient way to use the traits is through the following interface:

.. doxygentypedef:: ac::return_t
.. doxygenvariable:: ac::arity_v
.. doxygentypedef:: ac::parameter_at_t
.. doxygenvariable:: ac::is_noexcept_v
.. doxygenvariable:: ac::accepts_variadic_arguments_v

The following concepts are provided:

.. doxygenconcept:: ac::FreeFunction
.. doxygenconcept:: ac::MemberFunction
.. doxygenconcept:: ac::FunctionObject

:source:`Source code <functional/traits>`

:test:`Tests <functional/traits>`

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
