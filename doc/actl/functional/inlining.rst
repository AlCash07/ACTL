********************************
Inlining
********************************

Function inlining is an important concept for performance optimization.
However, standard C++ provides only a single
`inline <https://en.cppreference.com/w/cpp/language/inline>`_ specifier,
which doesn't have its behavior defined precisely.
We address this by exposing related compiler extensions in a portable way.

Reference
=========

.. ac-include:: actl/functional/inlining/AC_ALWAYS_INLINE.hpp
.. doxygendefine:: AC_ALWAYS_INLINE

.. ac-include:: actl/functional/inlining/AC_NEVER_INLINE.hpp
.. doxygendefine:: AC_NEVER_INLINE

.. ac-tests:: tests/functional/inlining.cpp

Design
======

Compiler extension for `AC_NEVER_INLINE` is typically called `noinline`.
Aiming for improved clarity, we chose a different name
which is symmetric to `AC_ALWAYS_INLINE`.
