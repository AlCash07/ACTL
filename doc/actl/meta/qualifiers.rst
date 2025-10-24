********************************
Type qualifier traits
********************************

Traits related to cv (`const` and `volatile`)
`type qualifiers <https://en.cppreference.com/w/cpp/language/cv.html>`_,
as well as `reference declaration <https://en.cppreference.com/w/cpp/language/reference.html>`_.

Reference
=========

Qualifiers copy between types
-----------------------------

Traits that replace type qualifiers with the ones from the source type.

Table below shows the effect of different source types
on the following destination types: `int`, `int const`, `int&`.

It's important to understand the behavior of a referenced type qualifiers.
Unless `ac::copy_cvref_t` is used:

- Referenced type qualifiers of a source type don't affect the result.
- Referenced type qualifiers of a destination type are not changed.

================================ =========== ===================== ============= =================
Expression                       unqualified cv-qualified          l-value ref   r-value ref      
================================ =========== ===================== ============= =================
Source type S                    `char`      `char const volatile` `char const&` `char volatile&&`
`copy_const_t<int, S>`           `int`       `int const`           `int`         `int`            
`copy_const_t<int const, S>`     `int`       `int const`           `int`         `int`            
`copy_const_t<int&, S>`          `int&`      `int&`                `int&`        `int&`           
`copy_volatile_t<int, S>`        `int`       `int volatile`        `int`         `int`            
`copy_volatile_t<int const, S>`  `int const` `int const volatile`  `int const`   `int const`      
`copy_volatile_t<int&, S>`       `int&`      `int&`                `int&`        `int&`           
`copy_cv_t<int, S>`              `int`       `int const volatile`  `int`         `int`            
`copy_cv_t<int const, S>`        `int`       `int const volatile`  `int`         `int`            
`copy_cv_t<int&, S>`             `int&`      `int&`                `int&`        `int&`           
`copy_reference_t<int, S>`       `int`       `int`                 `int&`        `int&&`          
`copy_reference_t<int const, S>` `int const` `int const`           `int const&`  `int const&&`    
`copy_reference_t<int&, S>`      `int`       `int`                 `int&`        `int&&`          
`copy_cvref_t<int, S>`           `int`       `int const volatile`  `int const&`  `int volatile&&` 
`copy_cvref_t<int const, S>`     `int`       `int const volatile`  `int const&`  `int volatile&&` 
`copy_cvref_t<int&, S>`          `int`       `int const volatile`  `int const&`  `int volatile&&` 
================================ =========== ===================== ============= =================

.. note::

  `Boost.TypeTraits <https://www.boost.org/doc/libs/latest/libs/type_traits/doc/html/index.html>`_,
  for example,
  `boost::copy_reference <https://www.boost.org/doc/libs/latest/libs/type_traits/doc/html/boost_typetraits/reference/copy_reference.html>`_,
  don't remove existing qualifiers and only add new ones.
  Our copy is different and instead fully replaces qualifiers,
  because this behavior is consistent with data copy.

.. ac-include:: actl/meta/qualifiers/copy.hpp
.. doxygentypedef:: copy_const_t
.. doxygentypedef:: copy_volatile_t
.. doxygentypedef:: copy_cv_t
.. doxygentypedef:: copy_reference_t
.. doxygentypedef:: copy_cvref_t

Inner qualifiers modification
-----------------------------

Traits that add or remove type qualifiers.
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
