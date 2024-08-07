********************************
Character classification
********************************

Character classification similar to the one provided in
<`cctype <https://en.cppreference.com/w/cpp/header/cctype>`_>.
The difference is that functions here:

- Don't implicitly account for the locale.
- Are a few times faster than the standard ones,
  because they don't account for the locale and can be efficiently inlined.
- Return `bool` as expected.
- Don't try to handle the `EOF` character.

Reference
=========

.. doxygenfile:: string/char_classes.hpp

:source:`Source code <string/char_classes.hpp>`

:test:`Tests <string/char_classes.cpp>`
