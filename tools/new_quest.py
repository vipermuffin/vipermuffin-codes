#!/usr/bin/env python3
"""Create events/<event>/qNN/ from the templates (and optionally fetch notes).

The source files are named after the quest's namespace, e.g. events/2026/q03/
gets q2026_03.h, q2026_03.cpp and q2026_03_test.cpp.

    tools/new_quest.py 2026 3            # scaffold quest 3 of the 2026 event
    tools/new_quest.py 2026 3 --fetch    # ...and download your part 1 notes

Existing files are never overwritten, so it's safe to re-run.
"""
import argparse
import re
import subprocess
import sys

from quest_names import ROOT, namespace, quest_dir

TEMPLATES = ROOT / "templates"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("event", help="event id, e.g. 2026 (or a story number)")
    ap.add_argument("quest", type=int, help="quest number")
    ap.add_argument("--fetch", action="store_true", help="also run tools/fetch.py")
    args = ap.parse_args()

    qq = f"{args.quest:02d}"
    dest = quest_dir(args.event, args.quest)
    dest.mkdir(parents=True, exist_ok=True)

    event_id = re.sub(r"[^A-Za-z0-9]", "", args.event)
    # googletest suite names should be plain identifiers without underscores.
    suite = f"Event{event_id}Quest{qq}"
    ns = namespace(args.event, args.quest)  # e.g. q2026_05, also the file stem
    subs = {"@EVENT@": args.event, "@QUEST@": str(args.quest), "@QQ@": qq,
            "@SUITE@": suite, "@NS@": ns}

    for template, name in [
        ("quest.h.in", f"{ns}.h"),
        ("quest.cpp.in", f"{ns}.cpp"),
        ("quest_test.cpp.in", f"{ns}_test.cpp"),
        ("NOTES.md.in", "NOTES.md"),
    ]:
        out = dest / name
        rel = out.relative_to(ROOT)
        if out.exists():
            print(f"exists   {rel}")
            continue
        text = (TEMPLATES / template).read_text()
        for key, value in subs.items():
            text = text.replace(key, value)
        out.write_text(text)
        print(f"created  {rel}")

    if args.fetch:
        subprocess.run([sys.executable, str(ROOT / "tools" / "fetch.py"), args.event, str(args.quest)])

    print(f"\nNext: cmake --build build && ./build/vmc {args.event} {args.quest}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
