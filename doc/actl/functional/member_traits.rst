********************************
Member function traits
********************************

Reference
=========

.. ac-include:: actl/functional/traits/member_traits.hpp
.. doxygentypedef:: ac::class_parameter_t
.. doxygentypedef:: ac::unqualified_class_t
.. doxygentypedef:: ac::as_member_of_t

.. ac-tests:: tests/functional/traits/MemberFunction.cpp

Member qualifier traits
-----------------------

For each member function qualifier, the following traits are provided:

- Detection of the qualifier presence.
- Addition of a qualifier (doesn't change the type if already present).
- Removal of a qualifier (doesn't change the type if not present).

Table with the full list:

+-----------+--------------------------------+---------------------------------+-----------------------------+
| Qualifier | Detection                      | Addition                        | Removal                     |
+===========+================================+=================================+=============================+
| const     | `is_const_member_v`            | `add_member_const_t`            | `remove_member_const_t`     |
+-----------+--------------------------------+---------------------------------+-----------------------------+
| volatile  | `is_volatile_member_v`         | `add_member_volatile_t`         | `remove_member_volatile_t`  |
+-----------+--------------------------------+---------------------------------+-----------------------------+
|| const    | `is_cv_member_v`               | `add_member_cv_t`               | `remove_member_cv_t`        |
|| volatile |                                |                                 |                             |
+-----------+--------------------------------+---------------------------------+-----------------------------+
| &         | `is_lvalue_reference_member_v` | `add_member_lvalue_reference_t` | `remove_member_reference_t` |
+-----------+--------------------------------+---------------------------------+-----------------------------+
| &&        | `is_rvalue_reference_member_v` | `add_member_rvalue_reference_t` | `remove_member_reference_t` |
+-----------+--------------------------------+---------------------------------+-----------------------------+

Addition traits change the member function type in the following way,
and removal traits do an inverse operation.

=============================== ================================ =================================
Trait name                      Non-reference                    Reference                    
=============================== ================================ =================================
Input type                      `int(Class::*)()`                `int(Class::*)() &`              
`add_member_const_t`            `int(Class::*)() const`          `int(Class::*)() const&`         
`add_member_volatile_t`         `int(Class::*)() volatile`       `int(Class::*)() volatile&`      
`add_member_cv_t`               `int(Class::*)() const volatile` `int(Class::*)() const volatile&`
`add_member_lvalue_reference_t` `int(Class::*)() &`              `int(Class::*)() &`              
`add_member_rvalue_reference_t` `int(Class::*)() &&`             `int(Class::*)() &`              
=============================== ================================ =================================

.. note:: `add_member_rvalue_reference_t` doesn't change the l-value reference because of the C++
  `reference collapsing rules <https://stackoverflow.com/questions/13725747/what-are-the-reference-collapsing-rules-and-how-are-they-utilized-by-the-c-st>`_.

=========================== ================================ =================================
Trait name                  Non-reference                    Reference                        
=========================== ================================ =================================
Input type                  `int(Class::*)() const volatile` `int(Class::*)() const volatile&`
`remove_member_const_t`     `int(Class::*)() volatile`       `int(Class::*)() volatile&`      
`remove_member_volatile_t`  `int(Class::*)() const`          `int(Class::*)() const&`         
`remove_member_cv_t`        `int(Class::*)()`                `int(Class::*)() &`              
`remove_member_reference_t` `int(Class::*)() const volatile` `int(Class::*)() const volatile` 
=========================== ================================ =================================

.. ac-include:: actl/functional/traits/member_qualifiers.hpp
.. doxygenfile:: functional/traits/member_qualifiers.hpp

.. ac-tests:: tests/functional/traits/member_qualifiers.cpp
