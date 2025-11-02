********************************
Concepts
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

Core language concepts
----------------------

For `std::convertible_to <https://en.cppreference.com/w/cpp/concepts/convertible_to>`_:

.. ac-include:: actl/meta/concepts/core/ConvertibleTo.hpp
.. doxygenconcept:: ac::NothrowExplicitlyConvertibleTo
.. doxygenconcept:: ac::NothrowConvertibleTo
.. doxygenconcept:: ac::MayThrowConvertibleTo

For `std::assignable_from <https://en.cppreference.com/w/cpp/concepts/assignable_from>`_:

.. ac-include:: actl/meta/concepts/core/AssignableFrom.hpp
.. doxygenconcept:: ac::TriviallyAssignableFrom
.. doxygenconcept:: ac::NothrowAssignableFrom

For `std::swappable, std::swappable_with <https://en.cppreference.com/w/cpp/concepts/swappable>`_:

.. ac-include:: actl/meta/concepts/core/Swappable.hpp
.. doxygenconcept:: ac::NothrowSwappable
.. doxygenconcept:: ac::NothrowSwappableWith

For `std::constructible_from <https://en.cppreference.com/w/cpp/concepts/constructible_from>`_:

.. ac-include:: actl/meta/concepts/core/ConstructibleFrom.hpp
.. doxygenconcept:: ac::TriviallyConstructibleFrom
.. doxygenconcept:: ac::NothrowConstructibleFrom

For `std::default_initializable <https://en.cppreference.com/w/cpp/concepts/default_initializable>`_:

.. ac-include:: actl/meta/concepts/core/DefaultInitializable.hpp
.. doxygenconcept:: ac::TriviallyDefaultInitializable
.. doxygenconcept:: ac::NothrowDefaultInitializable

For `std::move_constructible <https://en.cppreference.com/w/cpp/concepts/move_constructible>`_:

.. ac-include:: actl/meta/concepts/core/MoveConstructible.hpp
.. doxygenconcept:: ac::TriviallyMoveConstructible
.. doxygenconcept:: ac::NothrowMoveConstructible

For `std::copy_constructible <https://en.cppreference.com/w/cpp/concepts/copy_constructible>`_:

.. ac-include:: actl/meta/concepts/core/CopyConstructible.hpp
.. doxygenconcept:: ac::TriviallyCopyConstructible
.. doxygenconcept:: ac::NothrowCopyConstructible
.. doxygenconcept:: ac::MayThrowCopyConstructible

Comparison concepts
-------------------

For `std::equality_comparable <https://en.cppreference.com/w/cpp/concepts/equality_comparable>`_:

.. ac-include:: actl/meta/concepts/comparison/EqualityComparable.hpp
.. doxygenconcept:: ac::NothrowEqualityComparable

Object concepts
---------------

For `std::movable <https://en.cppreference.com/w/cpp/concepts/movable>`_:

.. ac-include:: actl/meta/concepts/object/Movable.hpp
.. doxygenconcept:: ac::TriviallyMovable
.. doxygenconcept:: ac::NothrowMovable

For `std::copyable <https://en.cppreference.com/w/cpp/concepts/copyable>`_:

.. ac-include:: actl/meta/concepts/object/Copyable.hpp
.. doxygenconcept:: ac::TriviallyCopyable
.. doxygenconcept:: ac::NothrowCopyable
.. doxygenconcept:: ac::MayThrowCopyable

For `std::semiregular <https://en.cppreference.com/w/cpp/concepts/semiregular>`_:

.. ac-include:: actl/meta/concepts/object/SemiRegular.hpp
.. doxygenconcept:: ac::TriviallySemiRegular
.. doxygenconcept:: ac::NothrowSemiRegular

For `std::regular <https://en.cppreference.com/w/cpp/concepts/regular>`_:

.. ac-include:: actl/meta/concepts/object/Regular.hpp
.. doxygenconcept:: ac::NothrowRegular
