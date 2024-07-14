********************************
out/inout parameters
********************************

One of the most important aspects of understanding the code behavior
is to clearly see which values can change and at which point.

Modern C++ strongly encourages returning by value over output parameters.
Structured binding and types like `std::optional` are the signs of that.
However, output parameters are still useful in some cases,
and C++ provides no built-in mechanism to distinguish them.

For example, can you imagine a C++ newcomer to guess which parameter
represents the output range of this algorithm?

.. code::

  std::ranges::transform(x, y, z);

We aim to make such code more self-documenting with the help of simple wrappers:

.. code::

  ac::ranges::transform(x, ac::out{y}, z);

Difference between out and inout parameters
===========================================

:cpp:class:`ac::inout` shouldn't be used just because
we read something from the parameter.
It should be used only if we read the referenced data.

For example, `std::span` references a contiguous block of memory.
But in order to access this memory, we clearly need to read
`.data()` and `.size()` from the span.
It's recommended to still use `ac::out<std::span<T>>`
if we don't read to referenced memory itself.

Reference
=========

.. doxygenclass:: ac::out
    :undoc-members:
    :members:

.. doxygenclass:: ac::inout

Interface of :cpp:class:`ac::inout` basically repeats :cpp:class:`ac::out`.

Detection traits
----------------

.. doxygenvariable:: ac::is_out_v
.. doxygenvariable:: ac::is_inout_v

:source:`Source code <functional/parameters>`

:test:`Tests <functional/out_inout.cpp>`

Design
======

out/inout wrappers are designed to be as simple as possible,
so they don't enforce any guarantees about the parameters
being actually written to.
There are more advanced designs available with such guarantees,
for example the one from this article about the
`output parameter <https://www.foonathan.net/2016/10/output-parameter>`_.
But the added benefits don't seem to be worth the extra complexity.
Especially considering that returning by value should be strongly preferred
over output parameters, so the latter are left to handle not single object,
but ranges and the like.

Google C++ style guide recognized the problem of output parameters,
but the proposed solution to use pointers was clearly misguided:

“In fact it is a very strong convention in Google code that
input arguments are values or const references
while output arguments are pointers.”

After many years this paragraph is finally removed from the guide.
But there's no alternative provided, which we're addressing here.
