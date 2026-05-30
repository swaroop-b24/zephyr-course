# l8-task1: Ring Buffer Unit Tests

## Overview
This task implements 7 ZTEST unit tests for the `ring_buf` module — a circular FIFO buffer — using the Zephyr ZTEST framework.

---

## Module Under Test

```
app/modules/ring_buf/
├── include/ring_buf.h     ← public API
└── src/ring_buf.c         ← implementation (static circular buffer)
```

### Public API
```c
int rb_init(uint32_t capacity);  /* init buffer, capacity 1-16 */
int rb_push(int value);          /* push to back, -ENOSPC if full */
int rb_pop(int *value);          /* pop from front, -ENODATA if empty */
int rb_peek(int *value);         /* peek without consuming */
bool rb_is_full(void);           /* true if no room left */
bool rb_is_empty(void);          /* true if no elements */
uint32_t rb_count(void);         /* number of elements */
```

---

## Test Structure

```
tests/ring_buf/
├── CMakeLists.txt
├── Kconfig
├── prj.conf
├── testcase.yaml
├── TEST_SPEC.md           ← full test specification
├── HOMEWORK.md            ← homework instructions
└── src/
    └── test_ring_buf.c    ← all 8 tests implemented here
```

---

## Tests Implemented

### Suite: `ring_buf_init` (2 tests)

| Test | Description |
|------|-------------|
| `test_fresh_state` | Fresh buffer is empty and count is 0 *(provided)* |
| `test_reinit_clears_state` | Push a value, reinit, verify empty and count=0 |

### Suite: `ring_buf_push_pop` (3 tests)

| Test | Description |
|------|-------------|
| `test_single_push_pop` | Push 42, pop it back, verify value and empty state |
| `test_fifo_order` | Push 1,2,3 — pop yields 1,2,3 in order |
| `test_push_full_returns_enospc` | Fill buffer, push one more → `-ENOSPC` |

### Suite: `ring_buf_boundaries` (3 tests)

| Test | Description |
|------|-------------|
| `test_peek_does_not_consume` | Peek twice → same value, count unchanged |
| `test_pop_null_returns_einval` | `rb_pop(NULL)` → `-EINVAL` |
| `test_is_full_after_fill` | Push 4 → `rb_is_full()` true, count=4 |

---

## Build and Run

### Build only
```bat
west build -b qemu_x86 F:\swaroopb_zephyr\zephyr-course\tests\ring_buf -p
```

### Run tests
```bat
west build -t run
```

### Expected output
```
SUITE PASS - 100.00% [ring_buf_boundaries]: pass = 3, fail = 0, skip = 0
 - PASS - [ring_buf_boundaries.test_is_full_after_fill]
 - PASS - [ring_buf_boundaries.test_peek_does_not_consume]
 - PASS - [ring_buf_boundaries.test_pop_null_returns_einval]

SUITE PASS - 100.00% [ring_buf_init]: pass = 2, fail = 0, skip = 0
 - PASS - [ring_buf_init.test_fresh_state]
 - PASS - [ring_buf_init.test_reinit_clears_state]

SUITE PASS - 100.00% [ring_buf_push_pop]: pass = 3, fail = 0, skip = 0
 - PASS - [ring_buf_push_pop.test_fifo_order]
 - PASS - [ring_buf_push_pop.test_push_full_returns_enospc]
 - PASS - [ring_buf_push_pop.test_single_push_pop]

PROJECT EXECUTION SUCCESSFUL
```

---

## Git Tag
```bash
git tag l8-task1
git push origin refs/tags/l8-task1
```
