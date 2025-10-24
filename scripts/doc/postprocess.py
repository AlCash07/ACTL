import sys
import os
import re

substr_to_remove = [
    # : public std::false_type
    '<span class="w"> </span><span class="p"><span class="pre">:</span></span>'
    '<span class="w"> </span><span class="k"><span class="pre">public</span></span>'
    '<span class="w"> </span><span class="n"><span class="pre">std</span></span>'
    '<span class="p"><span class="pre">::</span></span><span class="n">'
    '<span class="pre">false_type</span></span>',
]
regex_to_remove = [
    '<p>Subclassed by .*</p>\n',
    '<em>#include &lt;.*&gt;</em></div>\n',
]

doc_dir = sys.argv[1]

for root, _, file_names in os.walk(doc_dir):
    for file_name in file_names:
        if os.path.splitext(file_name)[1] == '.html':
            file_path = os.path.join(root, file_name)
            with open(file_path, 'r') as file:
                file_data = file.read()
            for s in substr_to_remove:
                file_data = file_data.replace(s, '')
            for r in regex_to_remove:
                file_data = re.sub(r, '', file_data)
            with open(file_path, 'w') as file:
                file.write(file_data)
