# SkillPath v2 — Polymorphic Skill Hierarchy

## What changed from v1

v1 modeled every skill as one flat `SkillNode` struct — same shape for
everything, no behavior, no OOP beyond data grouping. v2 replaces that with:

1. **An abstract `Skill` base class** with pure virtual `estimatedHours()`,
   `difficultyScore()`, and `category()` — real polymorphism, since these
   are called through a base reference and dispatch to different logic
   depending on the concrete type underneath.

2. **Three concrete subclasses** — `TechnicalSkill`, `TheorySkill`,
   `RepertoireSkill` — each computing cost differently. `RepertoireSkill`
   is the one with genuinely stateful behavior: after
   `markRelatedSkillMastered()` is called, `estimatedHours()` returns a
   discounted value (0.7x), which automatically flows through the existing
   Kahn's-algorithm scheduler and the critical-path DP with **zero changes**
   to either — they already call `estimatedHours()` polymorphically.

3. **`SkillFactory`** — mirrors `SplitStrategyFactory` in BalancioX.
   `SkillGraph::addNode()` never needs to know or branch on which concrete
   subclass it's building.

4. **`SkillVisitor` (Visitor pattern)** — lets you add new *operations* over
   the skill hierarchy (`RoadmapPrinterVisitor`, `JsonExportVisitor`)
   without ever touching `Skill`, `SkillGraph`, or the traversal code.
   Adding a third visitor requires zero edits to any existing file.

## New CLI options

- **7**: mark a `RepertoireSkill`'s related skill as mastered, applying the
  discount live.
- **5 / 6**: print or export the roadmap via a Visitor — try running 6
  before and after option 7 on the same repertoire skill to see the
  discount reflected in the JSON output.
- **4** (critical path) and **8** (bottleneck analysis) will report
  different numbers before/after option 7 is used on a skill that's on
  the critical path — proof the polymorphism is load-bearing, not cosmetic.

## Reviewed and hardened

The downcast in option 7 (marking a `RepertoireSkill` discounted) originally
checked `skill.category() != "Repertoire"` before a `static_cast`. That
works, but it trusts a string label to stay in sync with the real type —
fragile if a category string ever changes. Swapped to `dynamic_cast`, which
asks the runtime type system itself and returns `nullptr` on a mismatch
instead of relying on a label:

```cpp
if (auto *rep = dynamic_cast<RepertoireSkill *>(&skill)) {
  rep->markRelatedSkillMastered();
  ...
} else {
  // not actually a RepertoireSkill
}
```

Everything else was verified end-to-end: clean compile with `-Wall -Wextra`
and zero warnings, cycle detection still throws correctly, and the discount
mechanic was traced through manually — marking `Fusion_Soloing` (40h)
discounted correctly dropped it to 28h (0.7x), which reduced both the
sequential total (152 → 140) and the critical path (125 → 113) by exactly
the expected 12 hours, confirming the polymorphism is genuinely load-bearing
and not just a cosmetic label.

## Build

```
make
./skillpath_v2
```

or

```
cmake -B build && cmake --build build
```
