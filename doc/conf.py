project = "ACTL"
copyright = "2022, Oleksandr Bacherikov"
author = "Oleksandr Bacherikov"

templates_path = []
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]

extensions = ["breathe"]
breathe_default_project = "ACTL"
highlight_language = "cpp"
primary_domain = "cpp"

extensions.append("sphinxjp.themes.basicstrap")
html_theme = "basicstrap"
html_copy_source = False

extensions.append("sphinx.ext.autosectionlabel")
# make sure all the targets are unique
autosectionlabel_prefix_document = True

extensions.append("sphinx.ext.extlinks")
extlinks = {
    "source": ("https://github.com/AlCash07/ACTL/tree/master/include/actl/%s.hpp", "Source code"),
    "test": ("https://github.com/AlCash07/ACTL/tree/master/tests/%s.cpp", "Tests")
}

rst_prolog = """
.. role:: cpp(code)
  :language: cpp
  :class: highlight

.. default-role:: cpp
"""
