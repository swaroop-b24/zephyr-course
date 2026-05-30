/*
 * Ring Buffer Module - Homework Test Implementation
 * l8-task1: All 7 ZTEST stubs implemented
 *
 * Run:
 *   west twister -T tests/ring_buf -p qemu_x86   (Windows)
 *   west twister -T tests/ring_buf -p native_sim  (Linux/WSL)
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state.
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	/* Push a value to dirty the state */
	zassert_equal(rb_push(99), 0, "push(99) must succeed");

	/* Re-initialize with same capacity */
	zassert_equal(rb_init(4), 0, "rb_init(4) must succeed");

	/* Buffer must be empty and count must be 0 after reinit */
	zassert_true(rb_is_empty(), "Buffer must be empty after reinit");
	zassert_equal(rb_count(), 0, "Count must be 0 after reinit");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	int v = 0;

	/* Push 42 and pop it back */
	zassert_equal(rb_push(42), 0, "push(42) must return 0");
	zassert_equal(rb_pop(&v), 0, "pop must return 0");
	zassert_equal(v, 42, "popped value must be 42");

	/* Buffer must be empty after single push/pop */
	zassert_true(rb_is_empty(), "Buffer must be empty after pop");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	int v = 0;

	/* Push 1, 2, 3 */
	zassert_equal(rb_push(1), 0, "push(1) must return 0");
	zassert_equal(rb_push(2), 0, "push(2) must return 0");
	zassert_equal(rb_push(3), 0, "push(3) must return 0");

	/* Pop and verify FIFO order: 1, 2, 3 */
	zassert_equal(rb_pop(&v), 0, "first pop must return 0");
	zassert_equal(v, 1, "first pop must yield 1");

	zassert_equal(rb_pop(&v), 0, "second pop must return 0");
	zassert_equal(v, 2, "second pop must yield 2");

	zassert_equal(rb_pop(&v), 0, "third pop must return 0");
	zassert_equal(v, 3, "third pop must yield 3");

	/* Buffer must be empty after all pops */
	zassert_true(rb_is_empty(), "Buffer must be empty after all pops");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	/* Fill buffer to capacity of 4 */
	zassert_equal(rb_push(1), 0, "push(1) must return 0");
	zassert_equal(rb_push(2), 0, "push(2) must return 0");
	zassert_equal(rb_push(3), 0, "push(3) must return 0");
	zassert_equal(rb_push(4), 0, "push(4) must return 0");

	/* Buffer must be full */
	zassert_true(rb_is_full(), "Buffer must be full after 4 pushes");

	/* Push one more must return -ENOSPC */
	zassert_equal(rb_push(99), -ENOSPC, "push past capacity must return -ENOSPC");

	/* Count must still be 4 - rejected push did not consume a slot */
	zassert_equal(rb_count(), 4, "Count must remain 4 after rejected push");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	int v = 0;

	/* Push 7 */
	zassert_equal(rb_push(7), 0, "push(7) must return 0");

	/* First peek - must return 7 */
	zassert_equal(rb_peek(&v), 0, "first peek must return 0");
	zassert_equal(v, 7, "first peek must yield 7");

	/* Reset v to confirm second peek writes it again */
	v = 0;

	/* Second peek - must still return 7 (not consumed) */
	zassert_equal(rb_peek(&v), 0, "second peek must return 0");
	zassert_equal(v, 7, "second peek must yield 7");

	/* Count must still be 1 - peek must not consume */
	zassert_equal(rb_count(), 1, "Count must remain 1 after two peeks");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	/* rb_pop(NULL) must return -EINVAL */
	zassert_equal(rb_pop(NULL), -EINVAL, "pop(NULL) must return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	/* Push 4 values to fill the buffer */
	zassert_equal(rb_push(1), 0, "push(1) must return 0");
	zassert_equal(rb_push(2), 0, "push(2) must return 0");
	zassert_equal(rb_push(3), 0, "push(3) must return 0");
	zassert_equal(rb_push(4), 0, "push(4) must return 0");

	/* Buffer must be full and count must be 4 */
	zassert_true(rb_is_full(), "Buffer must be full after 4 pushes");
	zassert_equal(rb_count(), 4, "Count must be 4 after filling buffer");
}
