#include "string_compression.h"
#include <cassert>
#include <print>
#include <string>

int main() {
    // RLE
    {
        assert(dsa::rle_encode("AAABBBCCCD") == "A3B3C3D");
        assert(dsa::rle_decode("A3B3C3D") == "AAABBBCCCD");
    }
    {
        assert(dsa::rle_encode("AB") == "AB");
        assert(dsa::rle_decode("AB") == "AB");
    }
    {
        assert(dsa::rle_encode("") == "");
        assert(dsa::rle_decode("") == "");
    }
    {
        std::string orig = "WWWWWWWWWWWWBWWWWWWWWWWWWBBWWWWWWWWWWWW";
        auto enc = dsa::rle_encode(orig);
        assert(dsa::rle_decode(enc) == orig);
    }

    // LZ77
    {
        std::string text = "ABCABCABCABCABCABC";
        auto tokens = dsa::lz77_encode(text);
        auto decoded = dsa::lz77_decode(tokens);
        assert(decoded == text);
    }
    {
        std::string text = "ABABABABABAB";
        auto tokens = dsa::lz77_encode(text);
        auto decoded = dsa::lz77_decode(tokens);
        assert(decoded == text);
    }
    {
        auto tokens = dsa::lz77_encode("");
        auto decoded = dsa::lz77_decode(tokens);
        assert(decoded == "");
    }

    // LZ78
    {
        std::string text = "ABCABCABC";
        auto tokens = dsa::lz78_encode(text);
        auto decoded = dsa::lz78_decode(tokens);
        assert(decoded == text);
    }
    {
        std::string text = "ABABABA";
        auto tokens = dsa::lz78_encode(text);
        auto decoded = dsa::lz78_decode(tokens);
        assert(decoded == text);
    }

    // LZW
    {
        std::string text = "ABCABCABCABC";
        auto codes = dsa::lzw_encode(text);
        auto decoded = dsa::lzw_decode(codes);
        assert(decoded == text);
    }
    {
        std::string text = "ABABABAB";
        auto codes = dsa::lzw_encode(text);
        auto decoded = dsa::lzw_decode(codes);
        assert(decoded == text);
    }

    std::println("All string compression tests passed");
    return 0;
}
