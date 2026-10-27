# Everybody Codes in C++

My solutions to [Everybody Codes](https://everybody.codes) quests, in C++20 with CMake and googletest.

Each quest has three parts, and each part has its own notes (input) that unlock when the previous part is solved. Every quest folder has a `NOTES.md` with my approach, so you can read the reasoning alongside the code.

## Layout

| Path | What's there |
|------|--------------|
| `events/<event>/qNN/` | One quest, files named after its namespace (e.g. `q2026_03`): `q2026_03.h` (pieces exposed for unit tests), `q2026_03.cpp` (solution), `q2026_03_test.cpp` (tests), `NOTES.md` (write-up). Not committed: `notes_pN.txt` (my inputs), `puzzle_pN.html` (puzzle text) |
| `events/sample/q00/` | A toy quest showing the pattern and the helpers |
| `lib/vmc/` | Header-only helpers: parsing (`ints`, `lines`, `blocks`), `Grid`/`Point`, and quest registration |
| `runner/main.cpp` | The `vmc` command that runs a quest against its notes |
| `tests/lib_test.cpp` | Tests for the helpers |
| `templates/` | Files copied into each new quest |
| `tools/` | `new_quest.py` (scaffold a quest) and `fetch.py` (download notes) |

## Setup

Needs CMake ≥ 3.21, a C++20 compiler (recent clang, gcc 11+, or MSVC) and Python 3.10+ for the fetch tool (macOS's built-in 3.9 is too old: `uv venv --python 3.13` or Homebrew's `python@3.13` works). googletest is downloaded on the first configure.

```sh
cmake --preset release            # also: debug (sanitizers on), xcode
cmake --build --preset release
./build/vmc sample 0               # run the sample quest
ctest --preset release            # run all tests
```

To fetch notes automatically:

```sh
python3 -m venv .venv                        # repo-local, gitignored
.venv/bin/pip install -r tools/requirements.txt
mkdir -p ~/.config/ecd
# paste the "Session Cookie" value from your everybody.codes profile:
echo "<cookie>" > ~/.config/ecd/token
```

The tools find `.venv` automatically, so `tools/fetch.py` works without activating it.

## Solving a quest

```sh
tools/new_quest.py 2026 3 --fetch   # scaffold events/2026/q03, get part 1 notes (+ example if it can)
# add part 1's expected answer in q2026_03_test.cpp (paste the example too if it wasn't filled in)
# declare pieces to unit test in q2026_03.h (namespace q2026_03), write tests, then solve in q2026_03.cpp
cmake --build build && ctest --test-dir build -R Quest03
./build/vmc 2026 3 1                 # run on your notes, submit the answer
tools/fetch.py 2026 3               # part 2's notes are unlocked now
```

Other runner forms: `./build/vmc` lists quests and which notes are present; `./build/vmc 2026 3` runs every part that has notes; `./build/vmc 2026 3 2 -f other.txt` runs part 2 on any file.

A new quest needs no CMake edits. The build globs `events/**`, and `VMC_QUEST(...)` at the bottom of each quest file registers it with the runner and the tests.

After a part is accepted, put the answer in the `accepted` array in that quest's `MyAnswers` test. That makes refactoring safe. The test skips parts whose notes aren't on disk, so it passes in a fresh clone.

## VS Code

Open the folder and install the recommended extensions when prompted (CMake Tools, clangd, CodeLLDB, C++ TestMate, Claude Code).

- **Build:** ⇧⌘B builds release. Tasks also include `build debug`, `new quest` and `fetch notes`.
- **Tests:** the Testing sidebar lists every googletest case. Run or debug one with a click.
- **Debug:** the "Debug quest" launch config builds with sanitizers and asks for event, quest and part.
- **Claude:** `CLAUDE.md` keeps it in tutor/reviewer mode for this repo.

Prefer Xcode? `cmake --preset xcode` generates a project in `build-xcode/`. Re-run it after adding a quest.

## Notes for readers

- My `notes_p*.txt` inputs are gitignored, so your clone runs the examples and skips the answer checks. Use your own notes to run the solutions for real.
- Grid coordinates are `(row, col)`, with row growing downward, matching the puzzle text.
