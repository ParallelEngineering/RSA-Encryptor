# Addition
The addition algorithm uses a `uint16_t` accumulator to detect overflows beyond the 8-bit boundary (255).

1. **Iteration:** The algorithm traverses from the LSB (`index 0`) to the MSB (`index N`).
2. **Carry Propagation:** At each position `i`, it calculates:
   `sum = a[i] + b[i] + carry`
3. **Overflow Handling:** The carry for the next iteration is determined by `sum >> 8`. The digit stored in the current position is determined by `sum & 0xFF`.
4. **Finalization:** If a carry remains after the final byte is processed, an additional byte is pushed to the vector, extending the total magnitude of the number.