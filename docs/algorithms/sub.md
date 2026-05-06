# Subtraction
Subtraction implements a "borrow" propagation system to ensure mathematical correctness within an unsigned integer space:

1. **Underflow Protection:** Before processing, `isBigger` is invoked. If `b > a`, the result is clamped to `0` because negative numbers are not handled.
2. **Borrow Propagation:** If `a[i] < b[i]`, the operation sets a `borrow` flag.
3. **Regrouping:** The algorithm effectively borrows `256` from the next magnitude (`index i + 1`), performing `a[i] + 256 - b[i]`.
4. **Normalization:** Post-calculation, `normalizeVector` is called to prune leading zeros, ensuring the vector size remains proportional to the actual magnitude.