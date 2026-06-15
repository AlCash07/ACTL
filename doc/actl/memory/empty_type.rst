********************************
Empty type optimizations
********************************

C++20 introduced
`[[no_unique_address]] <https://en.cppreference.com/w/cpp/language/attributes/no_unique_address>`_
attribute, which makes empty class fields not waste any memory.
In generic code it's a good practice to apply it to all the class fields,
unless a field is known to be non-empty.
Previously, the only way to achieve a similar result was to use
`empty base optimization <https://en.cppreference.com/w/cpp/language/ebo>`_
and compressed pairs based on it,
which is very inconvenient and doesn't work for `final` classes.

Unfortunately, the standard attribute is ignored in the current MSVC version.
For portability we need a custom wrapper until that's fixed. See
`MSVC blog <https://devblogs.microsoft.com/cppblog/msvc-cpp20-and-the-std-cpp20-switch/#c20-no_unique_address>`_,
`github issue <https://github.com/microsoft/STL/issues/1364>`_.
Moreover, MSVC has issues with multiple inheritance support,
requiring even move workarounds:
`MSVC blog <https://devblogs.microsoft.com/cppblog/optimizing-the-layout-of-empty-base-classes-in-vs2015-update-2-3>`_.

Reference
=========

.. ac-include:: actl/memory/empty_type/EmptyTrivial.hpp
.. doxygenconcept:: ac::EmptyTrivial

.. ac-tests:: tests/memory/empty_type/EmptyTrivial.cpp

.. ac-include:: actl/memory/empty_type/AC_NO_UNIQUE_ADDRESS.hpp
.. doxygendefine:: AC_NO_UNIQUE_ADDRESS

.. ac-tests:: tests/memory/empty_type/AC_NO_UNIQUE_ADDRESS.cpp

.. ac-include:: actl/memory/empty_type/AC_EMPTY_BASES.hpp
.. doxygendefine:: AC_EMPTY_BASES

.. ac-tests:: tests/memory/empty_type/AC_EMPTY_BASES.cpp
