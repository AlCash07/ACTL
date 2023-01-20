AC_NO_UNIQUE_ADDRESS
================================

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

Reference
---------

.. doxygenfile:: memory/no_unique_address.hpp

:source:`Source code <memory/no_unique_address>`

:test:`Tests <memory/no_unique_address>`
