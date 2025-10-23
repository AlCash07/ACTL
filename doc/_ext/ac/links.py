from __future__ import annotations
import os

from docutils import nodes

from sphinx.application import Sphinx
from sphinx.util.docutils import SphinxDirective
from sphinx.util.typing import ExtensionMetadata


def link(path):
    assert os.path.exists(path), f"Path doesn't exist: {path}"
    github_link = "https://github.com/AlCash07/ACTL/tree/master"
    return github_link + "/" + path


class IncludeDirective(SphinxDirective):
    required_arguments = 1

    def run(self) -> list[nodes.Node]:
        path = self.arguments[0]
        assert str.startswith(
            path, "actl/"
        ), f"Include path doesn't start with `actl/`: {path}"
        # TODO: use nodes.literal_block instead of raw html
        return [
            nodes.raw(
                "",
                f'<pre class="literal-block">#include &lt;<a class="reference external" href="{link("include/" + path)}">{path}</a>&gt;</pre>\n',
                format="html",
            )
        ]


class TestsDirective(SphinxDirective):
    required_arguments = 1

    def run(self) -> list[nodes.Node]:
        path = self.arguments[0]
        assert str.startswith(
            path, "tests/"
        ), f"Tests path doesn't start with `tests/`: {path}"
        return [
            nodes.paragraph(
                "",
                "See tests at ",
                nodes.reference(path, path, internal=False, refuri=link(path)),
            )
        ]


def setup(app: Sphinx) -> ExtensionMetadata:
    app.add_directive("ac-include", IncludeDirective)
    app.add_directive("ac-tests", TestsDirective)
    return {
        "version": "1.0",
        "parallel_read_safe": True,
        "parallel_write_safe": True,
    }
