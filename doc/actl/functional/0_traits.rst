********************************
Function traits
********************************

Traits for function type inspection and assembly.
For traits specific to member functions, see :doc:`1_member_traits`.

This is a complete analog of `Boost.CallableTraits
<https://www.boost.org/doc/libs/latest/libs/callable_traits/doc/html/index.html>`_
implemented with much less code, so expect reduced compilation times!
The only feature not supported is `transaction_safe` from Transactional Memory TS,
because it's still experimental for a long time without notable progress.

Reference
=========

Concepts
--------

Free functions are typically used in examples for simplicify,
but most traits support all the following function concepts,
including their const and reference-qualified versions:

.. ac-include:: actl/functional/traits/FreeFunction.hpp
.. doxygenconcept:: ac::FreeFunction

.. ac-include:: actl/functional/traits/MemberFunction.hpp
.. doxygenconcept:: ac::MemberFunction

.. ac-include:: actl/functional/traits/FunctionObject.hpp
.. doxygenconcept:: ac::FunctionObject

.. ac-include:: actl/functional/traits/Callable.hpp
.. doxygenconcept:: ac::Callable

.. note::

  Some libraries support
  `pointers to data members <https://en.cppreference.com/w/cpp/language/pointer.html#Pointers_to_data_members>`_
  similarly to pointers to member functions.
  Data members are obviously not functions,
  but they are indeed related and this makes sense, for example, for
  `std::mem_fn <https://en.cppreference.com/w/cpp/utility/functional/mem_fn.html>`_.
  However, the result of applying a pointer to data member
  to a class object inherits the qualifiers of this object.
  So, it defines not a single function, but an overload set,
  which is not supported by the library as of now.

Traits
------

.. ac-include:: actl/functional/traits/function_traits.hpp
.. doxygenstruct:: ac::function_traits
.. doxygenenum:: ac::function_category

Return type
-----------

.. ac-include:: actl/functional/traits/return.hpp

.. doxygentypedef:: ac::return_t
.. doxygenvariable:: ac::returns_void_v
.. doxygentypedef:: ac::with_return_type_t

============================== =============================== =================================
Trait name                     Example 1                       Example 2                        
============================== =============================== =================================
Input type `Fn`                `float(int, int) noexcept`      `void(const char*, ...)`         
`return_t`                     `float`                         `void`                           
`returns_void_v`               `false`                         `true`                           
`with_return_type_t<Fn, int>`  `int(int, int) noexcept`        `int(const char*, ...)`          
============================== =============================== =================================

Parameters
----------

.. ac-include:: actl/functional/traits/parameters.hpp

.. doxygentypedef:: ac::parameters_t
.. doxygenvariable:: ac::arity_v
.. doxygentypedef:: ac::parameter_at_t
.. doxygentypedef:: ac::unique_parameters_t

============================== =============================== =================================
Trait name                     Example 1                       Example 2                        
============================== =============================== =================================
Input type `Fn`                `float(int, int) noexcept`      `void(const char*, ...)`         
`parameters_t`                 `ac::type_list<int, int>`       `ac::type_list<const char*>`     
`arity_v`                      `2`                             `1`                              
`parameter_at_t<Fn, 0>`        `int`                           `const char*`                    
============================== =============================== =================================

Variadic arguments
------------------

.. ac-include:: actl/functional/traits/variadic_arguments.hpp

.. doxygenvariable:: ac::accepts_variadic_arguments_v
.. doxygentypedef:: ac::add_variadic_arguments_t
.. doxygentypedef:: ac::remove_variadic_arguments_t

============================== =============================== =================================
Trait name                     Example 1                       Example 2                        
============================== =============================== =================================
Input type                     `float(int, int) noexcept`      `void(const char*, ...)`         
`accepts_variadic_arguments_v` `false`                         `true`                           
`add_variadic_arguments_t`     `float(int, int, ...) noexcept` `void(const char*, ...)`         
`remove_variadic_arguments_t`  `float(int, int) noexcept`      `void(const char*)`              
============================== =============================== =================================

`noexcept`
----------

.. ac-include:: actl/functional/traits/noexcept.hpp

.. doxygenvariable:: ac::is_noexcept_v
.. doxygentypedef:: ac::add_noexcept_t
.. doxygentypedef:: ac::remove_noexcept_t

============================== =============================== =================================
Trait name                     Example 1                       Example 2                        
============================== =============================== =================================
Input type                     `float(int, int) noexcept`      `void(const char*, ...)`         
`is_noexcept_v`                `true`                          `false`                          
`add_noexcept_t`               `float(int, int) noexcept`      `void(const char*, ...) noexcept`
`remove_noexcept_t`            `float(int, int)`               `void(const char*, ...)`         
============================== =============================== =================================

Function type assembly
----------------------

.. ac-include:: actl/functional/traits/assemble_function.hpp
.. doxygentypedef:: ac::assemble_function_t
.. doxygentypedef:: ac::as_free_function_t

.. ac-tests:: tests/functional/traits/

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
