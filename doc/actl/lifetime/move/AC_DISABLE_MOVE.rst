********************************
AC_DISABLE_MOVE
********************************

Reference
=========

.. ac-include:: actl/lifetime/move/AC_DISABLE_MOVE.hpp
.. doxygendefine:: AC_DISABLE_MOVE

.. ac-tests:: tests/lifetime/move/AC_DISABLE_MOVE.cpp

Design
======

AC_DISABLE_MOVE has effect similar to
`boost::noncopyable <https://www.boost.org/doc/libs/latest/libs/core/doc/html/core/noncopyable.html>`_.
However, the base class approach of `boost::noncopyable` has multiple issues:

- Extra base class makes class hierarchy more complex,
  which can reduce dynamic_cast efficiency.

- If multiple empty classes derive from the same `boost::noncopyable`
  then their storage cannot be merged together using EBO.

- Namespace of the base class is added to ADL,
  which adds extra work for the compiler
  and can have surprising effects.

- Aggregate initialization requires extra braces for the base class:
  `Derived{{}, members...}`.

- operators == and <=> cannot be defaulted, that is the following code won't compile

  .. code::

    bool operator==(const Derived&) const = default;
