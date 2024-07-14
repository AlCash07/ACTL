********************************
Inlining
********************************

Function inlining is an important concept for performance optimizations.
However, standard C++ provides only a single
`inline <https://en.cppreference.com/w/cpp/language/inline>`_ specifier,
which doesn't have its behavior defined precisely.
We address this by exposing related compiler extensions in a portable way.

Reference
=========

.. doxygendefine:: AC_ALWAYS_INLINE
.. doxygendefine:: AC_NEVER_INLINE

:source:`Source code <functional/inlining>`

:test:`Tests <functional/inlining>`
