constant
================================

Many C++ libraries including the standard one have inconsistent interfaces
for values known at runtime or compilation time. For example:

- Element access at the given position: `array[2]` and `std::get<2>(tuple)`.
- First elements of a span: `span.first(2)` and `span.first<2>()`.

Unfortunately, it's impossible to support syntax `tuple[2]`,
because 2 has type `int`, not `std::integral_constant<int, 2>`,
and the result type can be different for different indices.
The best thing we can do is to support syntax `tuple[2_c]` with the help of
:doc:`constant_literals`.

Throughout the library, :struct:`ac::constant` is used for return and parameter
types when they are known at compilation time to achieve consistent interfaces.
Labeling a function as `constexpr` isn't enough, because it doesn't guarantee
that the function result can always be computed at compilation time,
and detecting that isn't trivial.
Standard facilities such as `std::true_type` still can be used for type traits.

Reference
---------

.. doxygenstruct:: ac::constant

.. doxygentypedef:: ac::bool_constant
.. doxygenvariable:: ac::bool_c

.. doxygentypedef:: ac::int_constant
.. doxygenvariable:: ac::int_c

.. doxygentypedef:: ac::size_constant
.. doxygenvariable:: ac::size_c

Constant detection utilities

.. doxygenstruct:: ac::is_constant
.. doxygenvariable:: ac::is_constant_v

:source:`Source code <meta/constant.hpp>`

:test:`Tests <meta/constant.cpp>`
