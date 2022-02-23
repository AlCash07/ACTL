#!/bin/bash

sphinx_source=doc
sphinx_build=build/doc/sphinx

sphinx-build -b html \
    -Dbreathe_projects.ACTL=../build/doc/doxygen/xml \
    $sphinx_source $sphinx_build
