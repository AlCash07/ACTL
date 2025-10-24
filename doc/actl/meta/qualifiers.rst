********************************
Type qualifier traits
********************************

Traits related to cv (`const` and `volatile`)
`type qualifiers <https://en.cppreference.com/w/cpp/language/cv.html>`_,
as well as `reference declaration <https://en.cppreference.com/w/cpp/language/reference.html>`_.

Reference
=========

Inner qualifiers modification
-----------------------------

Traits that add or remove type qualifiers:

- `const`;
- `volatile`;
- cv for `const` and `volatile`.

If the type is a reference, then the referenced type is modified.

====================== ==================== =====================
Trait name             Non-reference        Reference            
====================== ==================== =====================
Input type             `int`                `int&`               
`add_inner_const_t`    `int const`          `int const&`         
`add_inner_volatile_t` `int volatile`       `int volatile&`      
`add_inner_cv_t`       `int const volatile` `int const volatile&`
====================== ==================== =====================

========================= ==================== =====================
Trait name                Non-reference        Reference            
========================= ==================== =====================
Input type                `int const volatile` `int const volatile&`
`remove_inner_const_t`    `int volatile`       `int volatile&`      
`remove_inner_volatile_t` `int const`          `int const&`         
`remove_inner_cv_t`       `int`                `int&`               
========================= ==================== =====================

.. ac-include:: actl/meta/qualifiers/inner.hpp
.. doxygentypedef:: add_inner_const_t
.. doxygentypedef:: remove_inner_const_t
.. doxygentypedef:: add_inner_volatile_t
.. doxygentypedef:: remove_inner_volatile_t
.. doxygentypedef:: add_inner_cv_t
.. doxygentypedef:: remove_inner_cv_t

.. ac-tests:: tests/meta/qualifiers/inner.cpp
