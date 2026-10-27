"""Where a quest lives and what its files are called, shared by the tools.

Files are named after the quest's namespace (q2026_05.cpp, not q05.cpp) so
they stay recognisable in IDEs that flatten the folder structure.
"""
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent


def quest_dir(event: str, quest: int) -> pathlib.Path:
    return ROOT / "events" / str(event) / f"q{quest:02d}"


def namespace(event: str, quest: int) -> str:
    """C++ namespace and file stem, e.g. q2026_05."""
    event_id = re.sub(r"[^A-Za-z0-9]", "", str(event))
    return f"q{event_id}_{quest:02d}"


def test_file(event: str, quest: int) -> pathlib.Path:
    """The quest's test file, falling back to the old qNN_test.cpp name if that's what exists."""
    path = quest_dir(event, quest) / f"{namespace(event, quest)}_test.cpp"
    legacy = path.with_name(f"q{quest:02d}_test.cpp")
    return legacy if not path.exists() and legacy.exists() else path
