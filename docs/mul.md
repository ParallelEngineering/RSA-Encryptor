# Multiplication

The multiplication algorithm is a software-level implementation of **Grade-School Long Multiplication**, operating in Base-256 rather than Base-10. It systematically multiplies every byte of the first number by every byte of the second number, accumulating the sums and propagating carries.

## 1. Pre-allocation and Space Complexity
Mathematically, multiplying a number of length `N` by a number of length `M` will result in a product with a maximum length of `N + M`.
The algorithm leverages this mathematical guarantee by pre-allocating a `result` vector initialized entirely with zeros: `std::vector<uint8_t> result(aSize + bSize, 0)`.
This completely eliminates dynamic memory reallocations during the intensive double-loop matrix computation.

## 2. The Accumulator Matrix (Double Loop)
The algorithm iterates through every Base-256 digit (byte) of the multiplicand `A` (index `i`), and multiplies it by every byte of the multiplier `B` (index `x`).

Because the algorithm evaluates `A[i] * B[x]`, the resulting product mathematically belongs at the magnitude position `i + x`. The code reflects this directly by accumulating into `result[i + x]`.

## 3. The `uint16_t` Buffer Property
One of the most elegant aspects of Base-256 arithmetic is how perfectly it maps to standard hardware integer types. Inside the inner loop, the operation calculates:
`product = result[i + x] + carry + (A[i] * B[x])`

What is the absolute maximum value this expression can evaluate to?
* `A[i]` max is `255`
* `B[x]` max is `255`
* `carry` max is `255`
* Previous `result[i + x]` max is `255`
* Maximum calculation: $255 \times 255 + 255 + 255 = \mathbf{65535}$.

`65535` is exactly `0xFFFF`, which is the absolute maximum value of an unsigned 16-bit integer. By casting the accumulator to `uint16_t`, the algorithm perfectly encapsulates the multiplication without risking a single bit of overflow.

## 4. Carry Propagation
After calculating the 16-bit product:
* **The Current Byte:** `product & 0xFF` extracts the bottom 8 bits, which are stored in `result[i + x]`.
* **The Carry:** `product >> 8` extracts the top 8 bits, which become the `carry` for the next inner-loop iteration.
* **Row Carry:** When the inner loop finishes processing all bytes of `B`, any remaining `carry` is deposited at the end of the current matrix row: `result[i + bSize]`.

## 5. Normalization
Because `N + M` represents the *maximum* possible length, the result might have one trailing `0` byte (e.g., $9 \times 9 = 81$, 1 digit $\times$ 1 digit $=$ 2 digits. But $2 \times 3 = 6$, 1 digit $\times$ 1 digit $=$ 1 digit). The `normalizeVector` function cleanly strips this excess buffer byte if it is unused.