project = 'ACTL'
copyright = '2022, Oleksandr Bacherikov'
author = 'Oleksandr Bacherikov'

templates_path = []
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

extensions = ["breathe"]
breathe_default_project = "ACTL"

extensions.append("sphinxjp.themes.basicstrap")
html_theme = 'basicstrap'
html_static_path = ['_static']
html_copy_source = False
