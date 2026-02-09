********************************
ACTL documentation
********************************

ACTL library aims to provide accessible generic implementation
of advanced algorithms not available in the C++ standard library,
and also improve the ones available there if possible.

ACTL stands for Algorithm Competition Template Library,
because competitive programming was the original inspiration,
but the content has much wider applicability.

Contents
========

Low-level supplementary libraries useful in various contexts:

.. toctree::
   :maxdepth: 1

   actl/functional
   actl/meta
   actl/memory
   actl/lifetime
   actl/platform
   actl/preprocessor

High-level thematic libraries:

.. toctree::
   :maxdepth: 1

   actl/range
   actl/string

Design
======

The main guiding design principles for the library are:

- consistency;
- composability;
- configurability;
- clarity;
- convenience.

C++ standard library facilities turned out to be insufficient
to implement more advanced algorithms following these principles,
so the following foundational facilities were devised
and widely used across the library:

.. toctree::
   :maxdepth: 1

   actl/meta/constant

Contributing
============

.. toctree::
   :glob:
   :maxdepth: 1

   contributing/*
