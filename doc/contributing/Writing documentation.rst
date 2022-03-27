********************************
Writing documentation
********************************

One of the biggest problems of C++ is a lack of good and easy to use tools for writing documentation.
Here we describe the documentation tools chosen as the best fit for ACTL at this time.
They are based on the article `Clear, Functional C++ Documentation with Sphinx + Breathe + Doxygen + CMake
<https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/>`_ which you can read for more details.
The main difference is that shell scripts are used insted of CMake commands for simplicity, but this may be revisited to make scripts portable.

All the requirements can be installed using the `scripts/doc/setup.sh <https://github.com/AlCash07/ACTL/blob/master/scripts/doc/setup.sh>`_ script.

Doxygen
-------

The most complicated task, the C++ code parsing, is done by Doxygen.
Documentation comments that follow the `Doxygen manual <https://www.doxygen.nl/manual/docblocks.html>`_ are automatically extracted at this step,
but they don't automatically appear in any web pages.
The following style of comments is preferred in ACTL:

.. code::

  /// Description.

Only comments that explain what classes and functions do should be written in Doxygen.
For other things such as examples and design rationale, and also how to use Doxygen output, see Sphinx_ section.

Documentatition comments are actually strongly discouraged.
The goal is to have self-documenting code whenever possible:

- Use descriptive names for types, functions, parameters etc.
- Introduce more high level types.
  For example, if a class `Line` is used instead of two points, there's no need for a comment explaining that these two points represent a line.
- Use special utilities to improve clarity, such as `out` and `inout` wrappers.

Treat every comment as a failure to express yourself in code.
Write a comment only when it provides useful information that can't be written in the code itself.

Doxygen can be run using the `scripts/doc/run_doxygen.sh <https://github.com/AlCash07/ACTL/blob/master/scripts/doc/run_doxygen.sh>`_ script
or ``run doxygen`` VS Code task.

Doxygen configuration file can be found `here <https://github.com/AlCash07/ACTL/blob/master/doc/Doxyfile.in>`_.
The options of interest are:

- ``GENERATE_XML`` is set to ``YES``, because xml output is used for the following steps.
- ``GENERATE_HTML`` is set to ``NO``, but can be set to YES to check the output locally in the *build/doc/doxygen/html* folder.
- ``EXTRACT_ALL`` is set to ``YES``, because most of the library content is put into free functions instead of classes by design.
- ``EXCLUDE_SYMBOLS`` is set to ``detail``, as this is the canonical namespace for implementation details.
  Don't put public code such as base classes with public interface into detail, because they won't be documented.
- ``INLINE_INHERITED_MEMB`` is set to ``YES`` to make it easier to see the full class interface.
  Utilities such as iterators and ranges rely on base classes to define the full interface without writing repetitive boilerplate code.
- ``PREDEFINED`` contains macros that affect the structure of the code.
  They would be parsed incorrectly otherwise.

Sphinx
------

`Sphinx generator <https://www.sphinx-doc.org/en/master/>`_ is used to generate the final documentation web pages.
The sources in rst format are located in the `doc <https://github.com/AlCash07/ACTL/tree/master/doc>`_ folder.
Existing sources can be used as a reference, other useful links for writing them are:

- `reStructuredText quick reference <https://docutils.sourceforge.io/docs/user/rst/quickref.html>`_
- `Sphinx directives <https://www.sphinx-doc.org/en/master/usage/restructuredtext/directives.html>`_
- `Sphinx C++ support <https://www.sphinx-doc.org/en/1.5.1/domains.html#id2>`_
- `Breathe directives <https://breathe.readthedocs.io/en/latest/directives.html>`_.
  Breathe is an extension for Sphinx that allows to use Doxygen xml output.

Sphinx can be run using the `scripts/doc/run_sphinx.sh <https://github.com/AlCash07/ACTL/blob/master/scripts/doc/run_sphinx.sh>`_ script
or ``generate documentation`` VS Code task (together with Doxygen),
and its output can be examined by opening *build/doc/sphinx/index.html*

Publishing documentation
------------------------

Currently, the documentation is published manually only by the project maintainers using the
`scripts/doc/publish.sh <https://github.com/AlCash07/ACTL/blob/master/scripts/doc/publish.sh>`_ script
or ``publish documentation`` VS Code task (together with documentation generation).

Generated documentation is committed to a special `doc branch <https://github.com/AlCash07/ACTL/tree/doc>`_
and then automatically published to `GitHub pages <https://pages.github.com/>`_ at https://alcash07.github.io/ACTL/.

Note that *.nojekyll* file had to be created in that branch to support the *_static* folder used by Sphinx.
