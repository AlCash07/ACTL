********************************
iterator interface
********************************

Writing iterators is necessary to implement custom ranges
to take advantage of their ease of use and composability.
However, satisfying all the standard iterator requirements is
too tedious and error-prone, there's a good example in the
`std::iterator_interface <https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2024/p2727r4.html#the-story-for-everyday-users-writing-stl-iterators-is-not-great>`_
proposal.

To address this problem, we provide utilities that are similar to the proposed
`std::iterator_interface <https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2024/p2727r4.html>`_,
which is based on
`Boost.STLInterfaces <https://www.boost.org/doc/libs/latest/doc/html/boost_stlinterfaces/tutorial___iterator_interface_.html>`_,
but with multiple improvements over both of them listed in the #Design section.

Examples of using them include:

- `linked list iterators <https://github.com/AlCash07/ACTL/blob/main/include/actl/range/iterator/linked_list_iterator.hpp>`_
  for forward and bidirectional iterators;
- `integer_iterator <https://github.com/AlCash07/ACTL/blob/main/include/actl/range/iterator/integer_iterator.hpp>`_
  for random access iterator;
- `filtered_range iterator <https://github.com/AlCash07/ACTL/blob/main/include/actl/range/iterator/integer_iterator.hpp>`_
  for ac::iterator_adaptor usage.

Specifically, it's enough to manually specify only a couple of nested types
and implement the following core operations,
and the rest will be correctly derived from them.

============= ================================ ==================================================================
Operation     Return Type                      Semantics
============= ================================ ==================================================================
`*i`          reference                        Returns dereferenced `i`                                          
`i == j`      Convertible to `bool`            Returns true if and only if `i` and `j` refererence the same value
`i - j`       Convertible to `difference_type` Returns offset between `i` and `j`                                
`++i`         `Iterator&`                      Increments `i`                                                    
`--i`         `Iterator&`                      Decrements `i`                                                    
`i += offset` `Iterator&`                      Increments `i` `offset` times, decrements if `offset < 0`         
============= ================================ ==================================================================

It's never necessary to implement all of them,
here's the requirements for each iterator concept:

+----------------------------------------------------------------------------------------------------+----------------+------------------+
| Concept                                                                                            | Operations     | Nested types     |
+====================================================================================================+================+==================+
| `std::output_iterator <https://en.cppreference.com/w/cpp/iterator/output_iterator>`_               || `*i`          || value_type      |
|                                                                                                    || `++i`         |                  |
+----------------------------------------------------------------------------------------------------+----------------+------------------+
| `std::input_iterator <https://en.cppreference.com/w/cpp/iterator/input_iterator>`_                 || `*i`          || value_type      |
|                                                                                                    || `++i`         || difference_type |
+----------------------------------------------------------------------------------------------------+----------------+------------------+
| `std::forward_iterator <https://en.cppreference.com/w/cpp/iterator/forward_iterator>`_             || `*i`          || value_type      |
|                                                                                                    || `++i`         || difference_type |
|                                                                                                    || `i == j`      |                  |
+----------------------------------------------------------------------------------------------------+----------------+------------------+
| `std::bidirectional_iterator <https://en.cppreference.com/w/cpp/iterator/bidirectional_iterator>`_ || `*i`          || value_type      |
|                                                                                                    || `++i`         || difference_type |
|                                                                                                    || `i == j`      |                  |
|                                                                                                    || `--i`         |                  |
+----------------------------------------------------------------------------------------------------+----------------+------------------+
| `std::random_access_iterator <https://en.cppreference.com/w/cpp/iterator/random_access_iterator>`_ || `*i`          | value_type       |
|                                                                                                    || `i += offset` |                  |
|                                                                                                    || `i - j`       |                  |
+----------------------------------------------------------------------------------------------------+----------------+------------------+

.. note::
   If some of the derived operations don't have
   the most efficient implementations,
   it's always possible to manually override them.

Reference
=========

.. ac-include:: actl/range/iterator/interface/basic_iterator_interface.hpp
.. doxygenclass:: ac::basic_iterator_interface

.. ac-include:: actl/range/iterator/interface/output_iterator_interface.hpp
.. doxygenclass:: ac::output_iterator_interface

.. ac-include:: actl/range/iterator/interface/input_iterator_interface.hpp
.. doxygenclass:: ac::input_iterator_interface

.. ac-include:: actl/range/iterator/interface/forward_iterator_interface.hpp
.. doxygenclass:: ac::forward_iterator_interface

.. ac-include:: actl/range/iterator/interface/bidirectional_iterator_interface.hpp
.. doxygenclass:: ac::bidirectional_iterator_interface

.. ac-include:: actl/range/iterator/interface/random_access_iterator_interface.hpp
.. doxygenclass:: ac::random_access_iterator_interface

.. ac-include:: actl/range/iterator/interface/iterator_interface_selector.hpp
.. doxygentypedef:: ac::iterator_interface_selector_t

.. ac-include:: actl/range/iterator/interface/iterator_adaptor.hpp
.. doxygenclass:: ac::iterator_adaptor

.. ac-tests:: tests/range/iterator/interface/

Design
=========

Practical experience with using `std::iterator_interface`
surfaced multiple usabiliy improvements that we implemented:

1. Hierarchy of interfaces instead of a single `std::iterator_interface`.

   We consider this important becuase:

   - Iterator concepts form a natural hierarchy, which we can just follow here.
   - It's easier to see which operations exactly are added for each concept
     compared to reading a huge combined single list
     with all the conditionally enabled operations.
   - Doxygen is unable to determince which of the conditionally enabled
     operations are actually relevant for a particular iterator,
     for example, a forward iterator.
     Deriving from a class in the hierarchy with just the relevant operations
     solves this problem.

   .. note::
      we still provide `ac::iterator_interface_selector_t`
      as a single class entry point when it's necessary.

2. Named nested types instead of template parameters.
   
   - Just compare the clarity between our implementation and the standard proposal:

     .. code::

        struct repeated_chars_iterator
            : ac::random_access_iterator_interface<repeated_chars_iterator> {
            using value_type = char;
            ...

     .. code::

        struct repeated_chars_iterator :
            std::iterator_interface<
                std::random_access_iterator_tag,
                char,
                char>
        {
            ...

     How would a reader guess what these two `char` template parameters mean
     without navigating to the `std::iterator_interface` declaration?
     In our case, it's obvious by just looking at the code
     (and there's no need to manually specify the second `char` as a `reference`).

   - Even more noticeable, see how many unnecessary type specifications
     we avoid when a custom `difference_type` is needed:

     .. code::

        struct my_forward_iterator
            : ac::forward_iterator_interface<my_forward_iterator> {
            using value_type = char;
            using difference_type = int;
            ...

     .. code::

        struct my_forward_iterator :
            std::iterator_interface<
                std::random_access_iterator_tag,
                char,
                char&,
                char*,
                int>
        {
            ...

   .. note::
      `ac::basic_iterator_interface` constructor provides clear error messages
      when the necessary nested types are not specified.

3. There's no `proxy_iterator_interface`.

   We can automatically detect if the reference type is not a built-in reference,
   so there seems to be no benefit in forcing the user to know
   about a different interface type for this.

4. Separate `ac::iterator_adaptor` class instead of
   merging it into `std::iterator_interface`.

   Adapting an iterator requires exposing all of its core operations
   and allowing to override the necessary ones.
   Which is conceptually quite different from
   providing all the core operations needed by the iterator interface.
   For this reason, we consider it confusing to provide
   all of this functionality in a single `std::iterator_interface` class.

5. Post-increment and post-decrement are implemented
   as friend functions instead of methods.

   As a result, the user code doesn't have to include
   this confusing code for forward iterators

   .. code::

      using base_type = std::iterator_interface<std::forward_iterator_tag, T>;
      using base_type::operator++;

   and this for bidirectional iterators

   .. code::

      using base_type::operator--;

   There's a whole section in
   `Boost.STLInterfaces <https://www.boost.org/doc/libs/latest/doc/html/boost_stlinterfaces/tutorial___iterator_interface_.html#boost_stlinterfaces.tutorial___iterator_interface_.an_important_note_about__code__phrase_role__keyword__operator__phrase__phrase_role__special________phrase___code__and__code__phrase_role__keyword__operator__phrase__phrase_role__special________phrase___code_>`_
   discussing this problem, which we don't have to worry about.

6. `operator==(T const&) = default;` definition is supported
   for forward and bidirectional iterators
   intead of forcing users to manually implement it.

7. `DifferenceType` is a template parameter in all the operations where it's used.

   This allows to support cases like aligned iterators,
   where the alignment can change if the `DifferenceType`
   isn't guaranteed to be a multiple of alignment.
