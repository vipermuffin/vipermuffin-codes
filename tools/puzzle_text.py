"""Best-effort download of the puzzle text, used to pre-fill test examples.

The puzzle text is encrypted with the same per-part keys as your notes. The
endpoint below is an assumption, not documented anywhere; if the site serves
the text differently, fetch.py just reports it and carries on.
"""
from __future__ import annotations

import html
import json
import pathlib
import re
from html.parser import HTMLParser

DESCRIPTION_URL = "https://everybody.codes/assets/{event}/{quest}/description.json"


class PuzzleTextError(Exception):
    pass


def fetch_descriptions(event, quest: int) -> dict[str, str]:
    """Return {"1": html, ...} for the unlocked parts. Raises PuzzleTextError."""
    from ecd import _impl  # private module of everybody-codes-data; may change between versions

    try:
        keys = _impl.get_keys(quest, event)
    except Exception as exc:
        raise PuzzleTextError(f"couldn't get quest keys ({exc})") from exc

    url = DESCRIPTION_URL.format(event=event, quest=quest)
    resp = _impl.get_http().request("GET", url)
    if resp.status != 200:
        raise PuzzleTextError(f"HTTP {resp.status} from {url}")
    try:
        data = json.loads(resp.data)
    except ValueError as exc:
        start = resp.data[:60].decode(errors="replace").strip()
        raise PuzzleTextError(f"{url} didn't return JSON (starts with {start!r})") from exc

    if not isinstance(data, dict):
        raise PuzzleTextError(f"unexpected JSON shape from {url}: {type(data).__name__}")

    parts = {}
    for part, value in data.items():
        key = keys.get(f"key{part}")
        if not key or not isinstance(value, str):
            continue
        try:
            parts[str(part)] = _impl.decrypt(value, key)
        except Exception as exc:
            raise PuzzleTextError(f"couldn't decrypt part {part} text ({exc}); JSON keys: {list(data)}") from exc
    if not parts:
        raise PuzzleTextError(f"nothing decryptable at {url}; JSON keys: {list(data)}")
    return parts


_VOID_TAGS = {"area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "source", "wbr"}


class _PreBlocks(HTMLParser):
    """Collects every <pre> with its own CSS classes and those of its ancestors."""

    def __init__(self) -> None:
        super().__init__(convert_charrefs=True)
        self.blocks: list[dict] = []
        self._stack: list[tuple[str, set[str]]] = []  # open elements: (tag, classes)
        self._pre: dict | None = None
        self._pre_depth = 0
        self._examples_seen = 0

    def handle_starttag(self, tag, attrs):
        classes = set((dict(attrs).get("class") or "").split())
        if self._pre is not None:
            if tag == "br":
                self._pre["buf"].append("\n")
            elif tag in ("b", "strong"):
                self._pre["bold"] = True
        if tag in _VOID_TAGS:
            return
        if "example" in classes:
            self._examples_seen += 1
        if tag == "pre":
            if self._pre is None:
                ancestors = set().union(*(c for _, c in self._stack)) if self._stack else set()
                self._pre = {"classes": classes, "ancestors": ancestors, "buf": [],
                             "example_index": self._examples_seen}
            self._pre_depth += 1
        self._stack.append((tag, classes))

    def handle_endtag(self, tag):
        # Pop back to the matching open tag; tolerates unclosed <p>/<li> etc.
        if not any(t == tag for t, _ in self._stack):
            return
        while self._stack:
            open_tag, _ = self._stack.pop()
            if open_tag == "pre":
                self._pre_depth -= 1
                if self._pre_depth == 0 and self._pre is not None:
                    self._pre["text"] = "".join(self._pre.pop("buf"))
                    self.blocks.append(self._pre)
                    self._pre = None
            if open_tag == tag:
                break

    def handle_data(self, data):
        if self._pre is not None:
            self._pre["buf"].append(data)


def _tidy(text: str) -> str:
    return html.unescape(text).strip("\n").rstrip()


def code_blocks(page: str) -> list[str]:
    """Text of every <pre> block, tags stripped, surrounding blank lines removed."""
    parser = _PreBlocks()
    parser.feed(page)
    return [t for t in (_tidy(b["text"]) for b in parser.blocks) if t]


def example_inputs(page: str) -> list[str]:
    """The example notes boxes (the dashed green ones on the site):
    <pre class="note"> inside <div class="example-note">. Falls back to any
    <pre class="note">, then to any <pre>, if the markup ever changes."""
    parser = _PreBlocks()
    parser.feed(page)
    for wanted in (
        lambda b: "note" in b["classes"] and "example-note" in b["ancestors"],
        lambda b: "note" in b["classes"],
        lambda b: True,
    ):
        found = [t for t in (_tidy(b["text"]) for b in parser.blocks if wanted(b)) if t]
        if found:
            return found
    return []


def example_answer(page: str) -> str | None:
    """The example's answer: the last boxed (<pre>) value in the first
    <div class="example">, after the notes box. The walkthrough boxes small
    values inline (e.g. <pre> R3 </pre>) and ends on the answer, which is
    usually bold (<pre><b>Fyrryn</b></pre>). Multi-line boxes are diagrams."""
    parser = _PreBlocks()
    parser.feed(page)
    inside = [b for b in parser.blocks if "example" in b["ancestors"] and "note" not in b["classes"]]
    # Only the first example div counts if a part ever shows several.
    first_example = [b for b in inside if b["example_index"] == inside[0]["example_index"]] if inside else []
    candidates = [(b, html.unescape(b["text"]).strip()) for b in first_example]
    candidates = [(b, t) for b, t in candidates if t and "\n" not in t]
    bold = [t for b, t in candidates if b.get("bold")]
    if bold:
        return bold[-1]
    return candidates[-1][1] if candidates else None


def _cpp_string(s: str) -> str:
    return s.replace("\\", "\\\\").replace('"', '\\"')


def fill_example(test_file: pathlib.Path, part: int, example: str, answer: str | None = None) -> str:
    """Put `example` (and `answer`, if given) into the PartNExample test if
    its raw string is still empty. The expected answer is only written when
    it's still "".

    Returns "added", "filled" (already has an example) or "unsafe" (the text
    contains the raw-string terminator, so it needs a manual paste).
    """
    source = test_file.read_text()
    empty = re.compile(r'(checkExample\(%d, R"EX\(\n)(\)EX", )("[^"\n]*")' % part)
    match = empty.search(source)
    if not match:
        return "filled"
    if ')EX"' in example:
        return "unsafe"
    expected = match.group(3)
    if answer and expected == '""':
        expected = f'"{_cpp_string(answer)}"'
    replacement = match.group(1) + example + "\n" + match.group(2) + expected
    test_file.write_text(source[: match.start()] + replacement + source[match.end():])
    return "added"
