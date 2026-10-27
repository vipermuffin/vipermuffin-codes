#!/usr/bin/env python3
"""Download and decrypt your notes for a quest into events/<event>/qNN/.

    tools/fetch.py 2026 3

Notes for each part unlock when you solve the previous part, so run this
again after every accepted answer to pick up the next part's notes.

It also tries to download the puzzle text (saved as puzzle_pN.html, not
committed) and copies each part's example notes and its answer into that
part's example test, if the test's example is still empty.

Uses the everybody-codes-data package, installed in the repo's .venv
(see README). The script finds .venv by itself; no need to activate it.
It reads your session token from ~/.config/ecd/token or the ECD_TOKEN
environment variable; copy it from the "Session Cookie" section of your
everybody.codes profile.
"""
import argparse
import os
import pathlib
import subprocess
import sys

import quest_names
from quest_names import ROOT


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("event", help="event id, e.g. 2026")
    ap.add_argument("quest", type=int, help="quest number")
    ap.add_argument("--no-examples", action="store_true", help="only fetch notes, skip the puzzle text")
    args = ap.parse_args()

    try:
        from ecd import get_inputs
    except ImportError:
        # Not installed in this interpreter: rerun with the repo's .venv if there is one,
        # so the script works without activating the venv first.
        venv = ROOT / ".venv"
        venv_python = venv / ("Scripts/python.exe" if os.name == "nt" else "bin/python")
        if venv_python.exists() and pathlib.Path(sys.prefix).resolve() != venv.resolve():
            return subprocess.call([str(venv_python), __file__, *sys.argv[1:]])
        print("Missing dependency. Set up the venv:\n"
              "  python3 -m venv .venv && .venv/bin/pip install -r tools/requirements.txt",
              file=sys.stderr)
        return 1

    event = int(args.event) if args.event.isdigit() else args.event
    try:
        data = get_inputs(quest=args.quest, event=event)
    except Exception as exc:  # network, auth or locked-quest errors from ecd
        print(f"Couldn't fetch notes: {exc}", file=sys.stderr)
        print("Check your token (~/.config/ecd/token or ECD_TOKEN) and that the quest is open.", file=sys.stderr)
        return 1

    dest = quest_names.quest_dir(args.event, args.quest)
    dest.mkdir(parents=True, exist_ok=True)
    for part in sorted(data):
        path = dest / f"notes_p{part}.txt"
        path.write_text(data[part])
        print(f"wrote    {path.relative_to(ROOT)}")

    locked = [p for p in ("1", "2", "3") if p not in data]
    if locked:
        print(f"locked   part {', '.join(locked)} (solve the previous part, then run this again)")

    if not args.no_examples:
        add_examples(event, args.quest, dest, quest_names.test_file(args.event, args.quest))
    return 0


def add_examples(event, quest: int, dest: pathlib.Path, test_file: pathlib.Path) -> None:
    """Best effort: save the puzzle text and pre-fill empty example tests."""
    import puzzle_text

    try:
        pages = puzzle_text.fetch_descriptions(event, quest)
    except puzzle_text.PuzzleTextError as exc:
        print(f"examples: couldn't get the puzzle text ({exc}); paste examples by hand")
        return

    for part in sorted(pages):
        page_file = dest / f"puzzle_p{part}.html"
        page_file.write_text(pages[part])
        blocks = puzzle_text.example_inputs(pages[part])
        extra = f" ({len(blocks)} examples, used the first; the rest are in {page_file.name})" if len(blocks) > 1 else ""
        if not blocks:
            print(f"examples part {part}: no example found; see {page_file.relative_to(ROOT)}")
        elif not test_file.exists():
            print(f"examples part {part}: {test_file.name} doesn't exist yet (run tools/new_quest.py)")
        else:
            answer = puzzle_text.example_answer(pages[part])
            status = puzzle_text.fill_example(test_file, int(part), blocks[0], answer)
            if status == "added" and answer:
                print(f"examples part {part}: added to {test_file.name} with expected answer "
                      f"{answer!r}{extra}; check it against the puzzle text")
            elif status == "added":
                print(f"examples part {part}: added to {test_file.name}{extra}; fill in the expected answer")
            elif status == "unsafe":
                print(f"examples part {part}: text contains )EX\" so it needs a manual paste; see {page_file.name}")
            else:
                print(f"examples part {part}: already filled in, left as is")


if __name__ == "__main__":
    sys.exit(main())
