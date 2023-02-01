constant_literals
================================

constant_literals provide the most convenient way possible in the current C++
to create objects of `ac::constant`, for example, `42_c`.

Reference
---------

Similarly to the standard namespaces with literals, `namespace constant_literals`
is an inline namespace within `namespace ac`.
Access to its content can be gained with `using namespace ac::constant_literals`.

.. doxygennamespace:: ac::constant_literals

:source:`Source code <meta/constant_literals.hpp>`

:test:`Tests <meta/constant_literals.cpp>`

Design
------

See the following
`article <https://blog.mattbierner.com/stupid-template-tricks-stdintegral_constant-user-defined-literal/>`_
for the approach explanation, in particular why it has to be so complicated.
