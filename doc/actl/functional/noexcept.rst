********************************
`noexcept` specifier
********************************

C++ has automatic return type deduction since C++11.
However, in C++20 there's still no support for automatic noexcept specification.
The following utilities make it easier to write noexcept-correct code
until we get `noexcept(auto)` in the language.

One caveat worth noting when using these utilities
is that operator `noexcept(T{})` checks not only that the constructor is
non-throwing, but also the destructor.
Unfortunately, `std::is_nothrow_constructible
<https://en.cppreference.com/w/cpp/types/is_constructible>`_
doesn't help either (see Notes).

Reference
=========

.. ac-include:: actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_AND_RETURN.hpp
.. doxygendefine:: AC_DEDUCE_NOEXCEPT_AND_RETURN

.. ac-include:: actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN.hpp
.. doxygendefine:: AC_DEDUCE_NOEXCEPT_DECLTYPE_AND_RETURN

.. ac-include:: actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN.hpp
.. doxygendefine:: AC_DEDUCE_NOEXCEPT_REQUIRES_AND_RETURN

.. ac-include:: actl/functional/noexcept/AC_DEDUCE_NOEXCEPT_AND_INITIALIZE.hpp
.. doxygendefine:: AC_DEDUCE_NOEXCEPT_AND_INITIALIZE

.. ac-tests:: tests/functional/noexcept

Design
======

These utilities are inspired by `BOOST_HOF_RETURNS
<https://www.boost.org/doc/libs/latest/libs/hof/doc/html/include/boost/hof/returns.html>`_.
