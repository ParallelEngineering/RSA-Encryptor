#include "cli.h"
#include <iostream>
#include <vector>
#include <string>
#include "../vec/base256.h"
#include "../core/key.h"

void cli::help() {
    std::cout << "Welcome to RSA-Encryptor" << std::endl;
}

void cli::keyManager::create() {
    key::createRSAKey();
}

void cli::keyManager::list() {
}

void cli::keyManager::print(const std::string &name, bool publicKey, bool privateKey) {
}

// Binary Modular Exponentiation: (base ^ exp) % mod
operations::Base256 modPow(operations::Base256 base, operations::Base256 exp, const operations::Base256 &mod) {
    operations::Base256 result(1);
    operations::Base256 zero(0);
    operations::Base256 two(2);

    base %= mod;

    while (exp > zero) {
        if (exp % two == operations::Base256(1)) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= two;
    }
    return result;
}

// RSA Encryption: C = M^e % n
operations::Base256 encrypt(const operations::Base256 &message, const operations::Base256 &e, const operations::Base256 &n) {
    return modPow(message, e, n);
}

// RSA Decryption: M = C^d % n
operations::Base256 decrypt(const operations::Base256 &ciphertext, const operations::Base256 &d, const operations::Base256 &n) {
    return modPow(ciphertext, d, n);
}

void cli::runBase256Poc() {
    std::cout << "\n============================================\n";
    std::cout << "  RSA Base256 Proof of Concept (PoC) \n";
    std::cout << "============================================\n\n";

    // --- 1. SET UP RSA PARAMETERS ---
    // p and q fit inside standard 64-bit bounds (~10^18)
    operations::Base256 p(1000000000000000003ULL);
    operations::Base256 q(1200000000000000011ULL);

    // n is calculated dynamically as p * q (~1.2 * 10^36)
    operations::Base256 n = p * q;

    // e is the public exponent
    operations::Base256 e(65537);

    // d is initialized from its raw base-256 byte representation to avoid integer overflow
    operations::Base256 d(std::vector<uint8_t>{77, 68, 38, 44, 239, 12, 32, 94, 60, 124, 198, 184, 54, 49, 130});

    // Print Keys
    std::cout << "[+] Public Key:\n";
    std::cout << "    e = "; e.print();
    std::cout << "    n = "; n.print();
    std::cout << "\n[+] Private Key:\n";
    std::cout << "    d = "; d.print();
    std::cout << "    n = "; n.print();
    std::cout << "\n";

    // Prepare message (12 characters fits comfortably under the 14-character limit)
    std::string textMessage = "Hello World!";
    std::vector<uint8_t> textBytes(textMessage.begin(), textMessage.end());
    operations::Base256 textVal(textBytes);

    std::cout << "[+] Original Message:\n";
    std::cout << "    Text      : \"" << textMessage << "\"\n";
    std::cout << "    Numeric   : "; textVal.print();
    std::cout << "\n";

    // Encrypt
    operations::Base256 textCipher = encrypt(textVal, e, n);
    std::cout << "[+] Encrypted:\n";
    std::cout << "    Ciphertext: "; textCipher.print();
    std::cout << "\n";

    // Decrypt
    operations::Base256 textDecryptedVal = decrypt(textCipher, d, n);
    std::vector<uint8_t> decryptedBytes = textDecryptedVal.getData();
    std::string decryptedText(decryptedBytes.begin(), decryptedBytes.end());

    std::cout << "[+] Decrypted:\n";
    std::cout << "    Numeric   : "; textDecryptedVal.print();
    std::cout << "    Text      : \"" << decryptedText << "\"\n";
    std::cout << "\n";

    // Verify
    if (textDecryptedVal == textVal) {
        std::cout << "[-] Verification: SUCCESS\n";
    } else {
        std::cout << "[-] Verification: FAILED\n";
    }

    std::cout << "\n============================================\n\n";
}