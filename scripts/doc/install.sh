#!/bin/bash

brew install doxygen
brew install sphinx-doc
python3 -m venv .venv
echo -e "[global]\nindex-url = https://pypi.org/simple/" > .venv/pip.conf
source .venv/bin/activate
pip3 install sphinx-bootstrap-theme
pip3 install breathe
deactivate
