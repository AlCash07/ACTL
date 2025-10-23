import sys
from pathlib import Path

sys.path.append(str(Path('_ext').resolve()))

project = "ACTL"
copyright = "2022, Oleksandr Bacherikov"
author = "Oleksandr Bacherikov"

templates_path = []
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

extensions = ["breathe"]
breathe_default_project = "ACTL"
highlight_language = "cpp"
primary_domain = "cpp"

extensions.append("sphinx.ext.githubpages")

import sphinx_bootstrap_theme
html_theme = 'bootstrap'
html_theme_path = sphinx_bootstrap_theme.get_html_theme_path()
html_copy_source = False

extensions.append("sphinx.ext.autosectionlabel")
# make sure all the targets are unique
autosectionlabel_prefix_document = True

extensions.append("ac.links")

rst_prolog = """
.. role:: cpp(code)
  :language: cpp
  :class: highlight

.. default-role:: cpp
"""
