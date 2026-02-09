********************************
Regular
********************************

For all the standard concepts that are involved in the definition of
`std::regular <https://en.cppreference.com/w/cpp/concepts/regular>`_
we provide supplementary concepts that check whether all the required operations
are trivial or nothrow.
There are standard type traits for all these requirements,
but they weren't grouped into concepts.

These concepts are especially useful in unit tests.

Reference
=========

Concepts augmenting
`std::regular <https://en.cppreference.com/w/cpp/concepts/regular>`_:

.. ac-include:: actl/lifetime/Regular.hpp
.. doxygenconcept:: ac::NothrowRegular

Concepts augmenting
`std::semiregular <https://en.cppreference.com/w/cpp/concepts/semiregular>`_:

.. ac-include:: actl/lifetime/SemiRegular.hpp
.. doxygenconcept:: ac::TriviallySemiRegular
.. doxygenconcept:: ac::NothrowSemiRegular

See also

- :doc:`constructor`
- :doc:`assignment`
- :doc:`move/concepts`
- :doc:`copy/concepts`
- :doc:`swap`
