callable_traits
================================

:cpp:struct:`ac::callable_traits` provide information about a callable, such as return and parameter types. The following callables are supported:

* free functions;
* pointers to free functions;
* pointers to member functions (in this case the first parameter is the enclosing class);
* function objects with a single function call operator;
* references and rvalue references to all of the above.

This is a lightweight analog of `Boost.CallableTraits <https://www.boost.org/doc/libs/1_78_0/libs/callable_traits/doc/html/index.html>`_ with limited functionality.

Outline
-------

The most convenient way to use the traits is through the following interface:

.. doxygentypedef:: ac::return_type_t
.. doxygenvariable:: ac::arity_v
.. doxygentypedef:: ac::parameter_type_t
.. doxygenvariable:: ac::is_member_function_v
.. doxygenvariable:: ac::is_noexcept_v

The same information is available as members of

.. cpp:struct:: template<class Function> \
                ac::callable_traits

or, only for function objects, as members of

.. cpp:struct:: template<class Function> \
                ac::function_object_traits

Design
------

The following `reference <https://functionalcpp.wordpress.com/2013/08/05/function-traits/>`_ was used at first, but it was heavily expanded.
