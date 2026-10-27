# sample · Quest 0

A toy quest that shows the layout. Not a real Everybody Codes puzzle.

## Part 1
**Idea:** `vmc::ints` pulls every integer (including negatives) out of the text; sum them.

## Part 2
**Idea:** Parse with `vmc::parseGrid`, then check each `#` against `DIRS4`. `grid.get(p, '.')` treats off-grid cells as `.`, so edges need no special case.

## Part 3
**Idea:** Breadth-first search from `S`. `std::hash<vmc::Point>` is defined, so `Point` drops straight into `unordered_map`.

## What I learned
Registration via `VMC_QUEST` means a new quest needs no edits anywhere else.
