# AGENTS.md — SimpleMtkWlan (fork)

Rules for every AI agent (OpenCode, Claude Code, others) and every human working on this repository.
Read this whole file before your first action. When you are unsure about anything, section 3 tells you
what to do: **stop and ask**. A wrong guess in kernel code costs the user a kernel panic and lost time;
a question costs one message.

## 0. How to read this file

- **Hard rules (section 2) are absolute.** Only an explicit message from the user that names the rule
  can lift one. Silence, ambiguity, "just do it" or "use your judgement" never lifts a hard rule.
- **Never fill a gap with a guess.** If an instruction is vague, incomplete, contradictory, or could be
  read two ways, go to section 3. Do not pick the interpretation you find most plausible and proceed.
- **Label what you claim.** In reports, tag statements as `[VERIFIED]` (you read it in the repo or saw it
  in a log), `[README]` (stated by the README, not checked), `[ASSUMPTION]` or `[UNKNOWN]`. Never present
  an assumption as a fact.
- **Language.** Talk to the user in French, short and direct; be detailed only when explaining a
  complex task. Code, comments, commit messages, and documentation in this repo are in English.
- If you discover that something in this file is wrong or outdated, do not silently work around it and do
  not silently edit it: tell the user and propose the edit.

## 1. Project snapshot

- **What:** an experimental macOS kernel extension (kext) for MediaTek MT792x PCIe Wi-Fi adapters. `[README]`
- **Origin:** this repository is a fork. Fork: `ItzVyral/SimpleMtkWlan`. Upstream (original author
  laobamac): `laobamac/SimpleMtkWlan`. Keep the original attribution everywhere.
- **Supported devices** `[README]` — nothing else is supported:
  - MT7921, PCI ID `14c3:7961`
  - MT7921K / RZ608, PCI ID `14c3:0608`
  - MT7922, PCI ID `14c3:7922`
- **Upstream status** `[README]`: "under active hardware bring-up", "not release-stable yet". Work in
  progress listed by the README: PCIe probe/attach, firmware loading, DMA queue setup, hardware scan,
  association/authentication TX path debugging. **Do not assume that connecting to a Wi-Fi network works
  end to end.**
- **License:** GPLv2 (`LICENSE`). Some files carry their own, more specific upstream notices (GPL / BSD /
  OpenBSD / NetBSD / FreeBSD). Those notices stay in place and apply to their files. `[README]`
- **Build target** (told by the user, confirm if a task depends on it): macOS Sequoia (15.x), **x86_64**
  Hackintosh. The kext is built on a GitHub Actions macOS runner (section 6), never locally on
  Windows/Linux.
- **Goal of this fork:** **stability** of what already exists: no kernel panic, no hang, no leak, reliable
  load/unload, reliable sleep/wake, bounded error recovery. It is not "add features".
  - Making association/authentication work, supporting other chips, adding features, or rewriting
    modules are **separate tasks that need explicit approval** (section 3).

## 2. Hard rules (never break)

### 2.1 Never hang or crash the kernel

A bug in a kext is a kernel panic or a frozen machine, not an exception. This rule outranks feature work:
do not ship a feature by adding a risky path, and fix a crash before building on top of it. Details are in
section 7; the headline rules are:

1. Every hardware wait/poll has a timeout and a bounded retry count. No unbounded loops. No unbounded
   reset loops.
2. Every resource acquired on a path is released on **every** path (success, error, `stop()`, `free()`).
3. Every value derived from hardware, firmware, over-the-air frames, or user space is hostile: bounds-check
   it before use (sizes, offsets, indexes, counts). Use checked arithmetic.
4. No blocking, sleeping, allocation that can sleep, or heavy logging in interrupt/filter context or while
   holding a spinlock.
5. Teardown mirrors setup in reverse. `free()` must tolerate a partially initialised object.
6. Never touch the hardware after it has stopped responding (see section 7.7).

### 2.2 No fabrication

- **Never invent** APIs, macros, registers, bit fields, firmware commands, struct layouts, IOKit calls,
  Xcode build settings, or file contents. If you have not seen it in this repository, in the SDK headers
  available in the build, or in a source the user gave you, it is `[UNKNOWN]`: say so and ask.
- **Never claim a change "works", "fixes", or "is stable".** You cannot run this kext. You can only reason
  about it and compile it in CI. The only evidence of behaviour is logs and results from the user's
  machine. Use wording like "should address X; needs testing on hardware".
- Never invent firmware, never "reconstruct" missing generated files, never write placeholder firmware data.

### 2.3 Licensing and third-party code

- Everything added must be compatible with GPLv2 (MIT, BSD-2/3, ISC are fine; **Apache-2.0 is not
  compatible with GPL-2.0-only**; proprietary, "no-derivatives" and non-commercial licenses are not
  allowed). If unsure about a license, the code does not go in; ask.
- Never copy code you cannot trace to a license. Never paste code from decompiled/disassembled Apple (or
  any vendor) binaries. Never copy Apple private headers or files extracted from macOS.
- Never remove, shorten, or reword a license header or copyright notice. Never change `LICENSE`.
- Any new third-party material (code, header, firmware, script, data) must be listed in `CREDITS.md` with
  author, source, and license, and requires user approval first (section 3).
- Dependencies, submodules, and downloads (SDKs, firmware, other kexts) are never added or fetched without
  explicit approval.

### 2.4 Scope and safety

- Do only what the current, approved task says. No drive-by refactors, renames, reformatting, dead-code
  removal, dependency bumps, or "while I'm here" fixes. Report them as suggestions instead.
- Never push to a remote, open a PR, rewrite history, or force-push unless the user explicitly asks.
  Never touch `main` directly.
- Never put secrets, keys, MAC addresses, SSIDs, passwords, or Wi-Fi keys (PMK/PTK/PSK) in code, logs, commit
  messages, or reports.
- Never change the user's EFI / OpenCore / boot-args configuration, and never recommend lowering macOS
  security settings, unless the user asks you to.
- No network code, telemetry, or phoning home in the kext.

## 3. Decision protocol (what you may decide, what you must ask)

### 3.1 You may do without asking

- Read any file in the repo, run read-only commands, list the tree, read build logs.
- Produce audits, analyses, and reports (no edits).
- Within an **approved** task: edit only the files and functions that task covers, with the smallest
  possible diff.
- Create a local branch named `agent/<short-topic>` and make local commits (section 9).
- Run the CI workflow through `gh` if it is installed and authenticated (section 6.5).

### 3.2 You must STOP and ask first

Ask before doing any of the following, and wait for the answer:

- Any change not covered by an item the user has approved (including "small obvious fixes" you notice).
- Anything touching: PCI IDs or device matching (`IOKitPersonalities`, `IOPCIMatch`), `Info.plist`
  (versions, `OSBundleLibraries`, personalities), Xcode project settings (SDK, deployment target,
  architectures, build settings, file membership, schemes), the firmware loading sequence, DMA ring
  layout/sizes, memory barriers/ordering, interrupt mode (legacy/MSI/MSI-X), the locking model, the
  threading model (work loop / command gate), PCIe power management (ASPM, link state, D-states),
  power-state callbacks, or any public/user-space interface.
- Adding, removing, updating, regenerating, or hand-editing any third-party file, firmware, generated
  file, dependency, or submodule (section 4).
- Modifying `mtk80211/` in more than a trivial local fix, or modifying any file that carries an upstream
  license notice, beyond a minimal and clearly scoped diff.
- Touching `.github/`, `.gitignore`, `LICENSE`, `CREDITS.md`, or `README.md`.
- Deleting, renaming, or moving files or functions.
- Changing the target macOS version, the Xcode version used by CI, or the architecture.
- Downloading anything.
- Choosing between two valid approaches that have different trade-offs (explain both, recommend one,
  let the user pick).
- Anything where the user's instruction could reasonably be read in more than one way.
- Pushing, opening a PR, or any history rewrite.

### 3.3 How to ask

Be brief. In French. One message, concrete, answerable quickly:

```
Question: <the single decision you need>
Contexte: <what you found, with file:line references and [VERIFIED]/[ASSUMPTION] tags>
Options: A) ...  B) ...  (what each changes, risk of each)
Ma recommandation: <A or B and why>
Je ne touche à rien d'ici ta réponse.
```

Ask at most 3 questions per message, ordered by importance. If the answer is a simple yes/no, say so.

### 3.4 Forbidden even if the user seems to ask indirectly

If the user's wording seems to ask for something in section 2, point out the conflict and ask them to
confirm explicitly. Do not infer permission.

## 4. Third-party files and how to treat them

The README states that some files inherit notices from upstream GPL/BSD/OpenBSD/NetBSD/FreeBSD components.
Treat the following as third-party until proven otherwise:

| Item | What it is | Rule |
|---|---|---|
| `mtk80211/` | Compatibility layer, OpenBSD net80211, support code `[README]` | Upstream-derived. Smallest possible diffs. Keep headers. No reformatting, no renames, no "cleanup". Prefer fixing problems in the glue code (`SimpleMtkWlan/`) rather than in upstream-derived code; if the fix must be here, ask. |
| Any file with a license/copyright header | Inherited notice | Keep the header byte for byte. Check the header **before** editing any file. |
| Firmware blobs / embedded firmware data / generated firmware table in `include/` | MediaTek firmware, transformed by a script `[README]` | Never hand-edit, regenerate, replace, or add. Never download firmware. The origin and license of the firmware must be confirmed in `CREDITS.md`; if it is not, report it. |
| `scripts/` (firmware generation helpers) `[README]` | Build-time generator | See section 10: its presence must be verified first. Do not recreate it. |
| Apple SDK headers (Kernel.framework, IOKit) | Come with Xcode, not in the repo | Never copy them into the repo. Never use private APIs/headers not already used here. |
| Any kernel SDK headers "matching the local development setup" `[README]` | Unknown exact source | Find out exactly what is used (section 10) and report. Do not add or download an SDK. |
| `CREDITS.md`, `LICENSE` | Attribution and license | Do not modify without asking. |
| Reference drivers (Linux `mt76`/`mt7921`, OpenBSD/FreeBSD Wi-Fi drivers, vendor sources) | Not in the repo | May be used as a *reference for understanding hardware behaviour* only if the user provides them or says to. Do not paste code from them without checking the license (section 2.3) and asking. |

**Before editing any file:** (1) open its first ~40 lines and look for a license/copyright/origin notice;
(2) if there is one, classify the file as third-party and apply the strictest row above; (3) say in your
report which files you classified as third-party.

**Adding any new third-party material** (after approval): put its original license text/header in place,
add an entry to `CREDITS.md` (author, source URL, license, what was taken, date), and mention it in the
commit message.

## 5. Repository layout

`[README]` (verify with `ls`/`tree` and report differences):

- `SimpleMtkWlan/` — kext entry point and MediaTek HAL
- `SimpleMtkWlan/hal_mwx/` — MT7921/MT7922 driver implementation
- `mtk80211/` — compatibility, OpenBSD net80211, and support code
- `include/` — shared driver interfaces and generated firmware table
- `scripts/` — firmware generation helpers (**see section 10: may be missing**)
- `SimpleMtkWlan.xcodeproj` — Xcode project; the only supported build system
- `CREDITS.md`, `LICENSE`, `README.md`, `.gitignore`

Never assume anything about the contents of a file you have not opened. Never assume a function does what
its name suggests: read it.

## 6. Building with GitHub Actions

You cannot build on the machine you are likely running on. All builds happen on a GitHub-hosted macOS runner.

### 6.1 The workflow

- File: `.github/workflows/build-kext.yml` (name: "Build kext"). It is provided by the user. **If it is
  missing, do not improvise one: tell the user.** Do not edit it without asking.
- Triggers: manual (`workflow_dispatch`, with an input `configuration` = `Debug` or `Release`, default
  `Debug`), push to `main` and to branches matching `agent/**`, and pull requests.
- Runner: `macos-15` (Apple Silicon, macOS 15 Sequoia), pinned on purpose. `macos-latest` now points to
  macOS 26 and must not be used here.
- Because the runner is Apple Silicon and the target is x86_64, the build passes `ARCHS=x86_64
  ONLY_ACTIVE_ARCH=NO`. Code signing is disabled in CI (`CODE_SIGNING_ALLOWED=NO`), so the produced kext is
  **unsigned**.
- Steps: checkout → environment diagnostics (`sw_vers`, `xcodebuild -version`, `-showsdks`,
  `xcodebuild -list`) → `xcodebuild` build → check output and architecture (`lipo -info`) → zip with
  `ditto` + SHA-256 → upload artifact.
- Core command (same as the README, plus CI flags):
  `xcodebuild -project SimpleMtkWlan.xcodeproj -scheme SimpleMtkWlan -configuration <Debug|Release> build`
- Output: artifact `SimpleMtkWlan-<configuration>-<commit-sha>` containing `SimpleMtkWlan.kext.zip`,
  `SimpleMtkWlan.kext.zip.sha256` and `build.log`. GitHub wraps these in an outer zip, so downloading gives a
  zip that contains `SimpleMtkWlan.kext.zip`. Artifacts are kept 30 days.

### 6.2 First-time setup (done by the user, not by you)

1. Put the workflow at `.github/workflows/build-kext.yml` in the fork.
2. On a fork, GitHub disables Actions by default: the user must open the **Actions** tab and click
   "I understand my workflows, go ahead and enable them".
3. If the build cannot start because of this, tell the user instead of trying workarounds.

### 6.3 Triggering a build

- By the user: **Actions** tab → "Build kext" → "Run workflow" → pick the branch and configuration.
- By push: pushing a branch named `agent/<topic>` triggers a build automatically. You may push only if the
  user explicitly allowed it (section 2.4).
- By you, if `gh` is installed and authenticated (check with `gh auth status`):
  `gh workflow run build-kext.yml --ref <branch> -f configuration=Debug`
  then `gh run list --workflow build-kext.yml --limit 5`.
  The ref must already exist on GitHub, i.e. have been pushed.
- If you have no `gh` access: say so, give the user the branch/commit to build, and ask them to run it
  and paste the failing step's log.

### 6.4 Reading results

- Follow a run: `gh run watch <run-id>`.
- Failed steps only: `gh run view <run-id> --log-failed`.
- Download: `gh run download <run-id> -n <artifact-name>`.
- Without `gh`: the user opens the run → the failed step → copies the relevant lines; the full `build.log`
  is in the artifact.
- Quote the **first** compiler/linker error, not only the last line. Later errors are often consequences.

### 6.5 What a green build does and does not prove

- Green = it compiles and links for x86_64 with the runner's Xcode/SDK. **It says nothing about runtime
  behaviour.** Never say "tested" or "works" after a green build. Say "builds; not tested on hardware".
- Check in the log that there are no new warnings in the files you touched.

### 6.6 When the build fails

Work in this order and **ask before step 4**:
1. Read the diagnostics step output (Xcode version, SDKs, schemes).
2. Find the first error. Classify: (a) your change, (b) pre-existing in the repo, (c) environment (Xcode/SDK
   version, scheme not shared, missing SDK headers, missing generated file).
3. For (a): fix inside the approved task. For (b) and (c): report to the user with the log lines.
4. Do not change CI, Xcode project settings, SDK, Xcode version, or add dependencies to make the build
   pass without asking. Typical environment causes to report (not to fix on your own): scheme
   "SimpleMtkWlan" not listed by `xcodebuild -list` (not a shared scheme), kernel SDK headers missing,
   missing firmware table or generator script, an Xcode version mismatch.

## 7. Kernel coding rules (strict)

Follow the existing conventions of the file you edit (naming, allocation helpers, locking helpers, logging
macros). Do not introduce a new pattern when the repo already has one. If you cannot find how the repo does
something, ask rather than inventing a pattern.

### 7.1 Language subset
No C++ exceptions, no RTTI, no STL, no `std::` containers or smart pointers, no static constructors with
side effects, no floating point in kernel code, no large stack buffers or deep recursion. Use only language
and library features already used in this repo. Allocate objects the way the surrounding code does.

### 7.2 Execution contexts
For every function you touch, determine which context(s) it can run in: interrupt/filter, work loop /
command gate, timer, a user-space-initiated call, or a power-management callback. Write it in your report.
Never call a function from a context where it is not allowed.
- Interrupt/filter context: minimal work only; no allocation, no `IOLog`, no blocking, no sleeping locks.
- Never block the work loop for long; it stalls all work queued to it.
- Never call into a command gate in a way that can deadlock on the thread that owns the work loop.

### 7.3 Locking
- Every lock has an owner comment saying what it protects. New locks need approval (section 3).
- Define and respect a lock order; never call out to other subsystems (net80211 callbacks, IOKit, user
  space) while holding a spinlock.
- Do not take a non-recursive lock twice. Release on every exit path, including error paths.
- Shared state accessed from more than one context must be protected or use the atomic primitives already
  used by the repo.

### 7.4 Memory and DMA
- Pair every allocation with a free of the same size/kind on every path. Set pointers to NULL after release.
- Check every allocation and every `IOMemoryDescriptor`/`IODMACommand`/mapping call for failure.
- DMA buffers: prepare/complete, map/unmap, release in a consistent and reversed order. Never free memory
  the device might still write to: stop the DMA engine and make sure it is quiesced first.
- Do not change descriptor layouts, ring sizes, alignment, or memory-barrier placement without asking.

### 7.5 Hardware access
- Use the repo's existing register accessors. Never add raw pointer dereferences to MMIO space.
- Polling loops: timeout + bounded iterations + a defined error result. Short busy-waits only for
  very small delays; longer waits go through a context that may sleep.
- Check return values of every IOKit, PCI config, DMA, and firmware call. Silent failure is a bug.

### 7.6 Untrusted input
Treat as hostile and validate before use: values read from hardware registers, firmware headers and
responses/events, received 802.11 frames (anyone nearby can send them), and anything coming from user
space. Check lengths before reading fields, offsets before indexing, counts before looping, and use
checked arithmetic for size computations (watch for 32-bit wraparound and signed/unsigned mixing).

### 7.7 Device not responding
A register read that returns all ones (`0xFFFFFFFF`) or an unexpected constant usually means the device is
gone, powered down, or the PCIe link is down. Stop touching the hardware, fail the current operation,
and log once. Do not spin waiting for it.

### 7.8 Lifecycle, sleep/wake, recovery
- `start()` failure paths unwind in reverse order. `stop()` stops interrupts and DMA first, then frees
  queues and mappings. `free()` tolerates a partially initialised object and is safe to call after a failed
  `start()`.
- Sleep/wake: before sleep, quiesce interrupts and DMA; after wake, do not assume any device state
  survived (firmware, queues, registers); re-initialise through the normal path. Do not change
  power-state handling without asking.
- Recovery/reset: bounded retries with a limit; after the limit, disable the interface cleanly and log
  once. Never loop forever, never reset in a tight loop.

### 7.9 Logging
- Use the repo's logging helper. Rate-limit anything that can repeat. No logging in hot paths or interrupt
  context. Never log keys, passwords, SSIDs, or MAC addresses.
- Logging may help diagnosis but must not change program logic. Debug and Release builds must behave
  the same apart from logging.

## 8. Stability workflow

The default way of working. Do not skip steps.

1. **Preflight (section 10).** Report before any code change.
2. **Audit, no edits.** Produce a list of findings: for each, file and function, category (leak, race,
   missing timeout, unchecked error, bad teardown, sleep/wake gap, untrusted input, other), severity,
   evidence `[VERIFIED]`/`[ASSUMPTION]`, and the symptom it could cause. Rank by risk of kernel panic/hang.
3. **Wait** for the user to choose which findings to fix. Do not start fixing on your own.
4. **Fix one finding at a time**, smallest diff, one commit per fix, covered by section 7.
5. **Build in CI** (section 6) and report the result honestly.
6. **Hand over for hardware testing** with the exact logs to collect (section 11). Do not start the next
   fix until the user reports on the previous one, unless they tell you to continue.

Reports must also list what you did **not** change but noticed.

## 9. Git workflow

- Branch: `agent/<short-topic>` created from the branch the user names (ask if not named). Never commit to
  `main`.
- One logical fix per commit. Message format:
  ```
  fix(<area>): <short imperative summary>

  Symptom: <what goes wrong, from logs if available>
  Cause: <[VERIFIED] or [HYPOTHESIS] with file:function>
  Change: <what this commit does>
  Risk: <what could regress>
  Test: <what the user should do and which logs to send>
  ```
  Areas: `hal`, `pci`, `fw`, `dma`, `irq`, `pm`, `net80211`, `build`, `docs`.
- Do not commit build outputs (`build/`, `*.kext`, zips, logs), editor files, or secrets. Check `git status`
  before every commit.
- Keep the upstream remote (`upstream` → `laobamac/SimpleMtkWlan`) untouched. Do not merge or rebase from
  upstream without asking.
- No force-push, no history rewriting, no squashing of the user's commits.

## 10. Preflight (task 0): verify before any change

Before touching code, check the points below and send the user one short report. Facts seen from outside
the repo, to confirm or refute `[README]`:

1. **Root listing seen so far** was: `SimpleMtkWlan.xcodeproj`, `SimpleMtkWlan`, `include`, `mtk80211`,
   `.gitignore`, `CREDITS.md`, `LICENSE`, `README.md`. There was **no `scripts/` directory**, although the
   README lists it, and **no `AGENTS.md` / `CLAUDE.md`** before this file. Check whether `scripts/` really
   exists and whether the firmware table in `include/` is present and complete. **If the firmware
   generator or data is missing, STOP and ask; never recreate or fabricate firmware.**
2. **Kernel SDK headers:** find which headers/SDK the project actually uses (check the `.xcodeproj` for
   `SDKROOT`, header search paths, framework search paths, deployment target, architectures, and any
   reference to an external kernel SDK). Report. Do not download or add an SDK.
3. **Scheme:** confirm the `SimpleMtkWlan` scheme is shared (present under
   `SimpleMtkWlan.xcodeproj/xcshareddata/xcschemes/`) and that `.gitignore` does not exclude it. If not shared, CI
   cannot build it: report.
4. **Product name:** confirm the build produces `SimpleMtkWlan.kext` (the CI step checks this path).
5. **Info.plist:** note bundle id, version, `OSBundleLibraries`, and the matching personalities; do not
   modify.
6. **Third-party map:** list the files under `mtk80211/` and any other file with an external license
   header, with the license found in each. Compare with `CREDITS.md` and report gaps.
7. **Workflow:** check `.github/workflows/build-kext.yml` exists. If not, tell the user.
8. **Language/conventions:** report which allocation, locking, logging, and register-access helpers the code
   base uses, so later changes follow them.
9. **Open questions:** list anything unclear that blocks a decision.

## 11. Evidence to request from the user

Never diagnose a runtime failure without evidence. Ask for, as relevant:

- What the user was doing when it failed: boot, kext load, scan, connect, idle, sleep, wake, unload.
- The exact machine and the exact Wi-Fi adapter (PCI ID), and the macOS version.
- The panic report (look in `/Library/Logs/DiagnosticReports/` for `.panic` files).
- Kernel log lines for the kext, for example:
  `log show --last 15m --predicate 'eventMessage CONTAINS[c] "SimpleMtkWlan"'`
- Whether the kext is loaded: `kmutil showloaded --list-only | grep -i mtk`
  (or `kextstat | grep -i mtk` on setups where it still works).
- IORegistry entry: `ioreg -l | grep -i -B2 -A12 SimpleMtkWlan`
- The commit SHA / artifact name of the build they tested.

Suggesting boot-args for verbose/symbolicated logs is allowed **as a suggestion only** (never edit their
config), and you must say they are optional.

## 12. Report format after every task

Keep it short (French, bullets are fine here):

- **Fait:** what changed (files, functions) and which commit(s).
- **Pourquoi:** the symptom or finding it addresses, tagged `[VERIFIED]`/`[HYPOTHESIS]`.
- **Risque:** what could regress; which contexts the touched code runs in (section 7.2).
- **Tiers:** third-party files touched or added (section 4), or "aucun".
- **CI:** build result and link to the run, or "non lancé".
- **Non vérifié:** everything that needs hardware testing.
- **À tester / logs à envoyer:** exact steps and commands (section 11).
- **Remarqué mais non modifié:** other issues noticed.
- **Questions:** anything blocking, in the format of section 3.3.

## 13. Quick checklist before you hand anything over

- [ ] The change is inside the approved task, smallest possible diff.
- [ ] No hard rule from section 2 was bent.
- [ ] No third-party header was altered; third-party files touched are listed.
- [ ] Every new allocation/lock/resource has a release on all paths.
- [ ] Every new loop/wait has a timeout; every new input-derived value is bounds-checked.
- [ ] Nothing claimed as "works"; statements are tagged `[VERIFIED]`/`[ASSUMPTION]`.
- [ ] CI run triggered (or the user told how), result reported honestly.
- [ ] No build outputs, secrets, or logs with personal data committed.