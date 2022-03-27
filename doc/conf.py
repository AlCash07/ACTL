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

rst_prolog = """
.. role:: cpp(code)
    :language: cpp
    :class: highlight

.. default-role:: cpp
"""
