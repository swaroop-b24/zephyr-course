# Lecture 8 — Task 2: Ring Buffer Coverage Analysis

This branch contains the code-coverage results for the Zephyr `ring_buf` unit-test assignment.

## Test Result

- Platform: `native_sim`
- Test scenario: `homework.unit.ring_buf`
- Test cases passed: **8 of 8**
- Failed: **0**
- Errored: **0**
- Skipped: **0**

## Coverage Result

| Metric | Coverage | Executed / Total |
|---|---:|---:|
| Lines | **81.4%** | 35 / 43 |
| Functions | **100.0%** | 7 / 7 |
| Branches | **64.3%** | 9 / 14 |

## Live gcovr Report

[Open the live GCC Code Coverage Report](https://swaroop-b24.github.io/zephyr-course/)

Click `ring_buf.c` in the report to view line-by-line coverage.

## Committed Coverage Files

The generated gcovr report is committed in this branch under:

```text
twister-out/coverage/
```

Main report file:

```text
twister-out/coverage/index.html
```

[Browse the committed coverage files](./twister-out/coverage/)

> GitHub's normal Code view displays HTML as repository content. Use the live-report link above to view the fully rendered report.

## Coverage Command

The report was generated on WSL using:

```bash
west twister \
    -T tests/ring_buf \
    -p native_sim \
    --coverage \
    --coverage-tool gcovr \
    --coverage-basedir app/modules/ring_buf
```

## Local Viewing

Open the report directly from WSL:

```bash
explorer.exe "$(wslpath -w "$PWD/twister-out/coverage/index.html")"
```

Alternatively, serve it as a local website:

```bash
cd twister-out/coverage
python3 -m http.server 8000 --bind 127.0.0.1
```

Then open:

```text
http://localhost:8000/
```

## Assignment References

- Task 1 tag: `l8-task1`
- Task 2 tag: `l8-task2`
- Task 2 coverage commit contains `twister-out/coverage/`
