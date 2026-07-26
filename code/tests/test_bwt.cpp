#include "bwt.h"
#include <cassert>
#include <print>
#include <string>

int main() {
    // BWT encode/decode roundtrip
    {
        std::string orig = "BANANA";
        auto bwt = dsa::bwt_encode(orig);
        auto decoded = dsa::bwt_decode(bwt);
        assert(decoded == orig);
    }
    {
        std::string orig = "MISSISSIPPI";
        auto bwt = dsa::bwt_encode(orig);
        auto decoded = dsa::bwt_decode(bwt);
        assert(decoded == orig);
    }
    {
        std::string orig = "abracadabra";
        auto bwt = dsa::bwt_encode(orig);
        auto decoded = dsa::bwt_decode(bwt);
        assert(decoded == orig);
    }
    {
        auto bwt = dsa::bwt_encode("");
        auto decoded = dsa::bwt_decode(bwt);
        assert(decoded == "");
    }

    // FM-Index
    {
        dsa::fm_index fm("ACGTACGTACGT");
        assert(fm.count("ACG") == 3);
        assert(fm.count("T") == 3);
        assert(fm.count("ZZZ") == 0);
        assert(fm.count("") >= 0);
    }
    {
        dsa::fm_index fm("ABRACADABRA");
        auto pos = fm.locate("ABRA");
        assert(pos.size() == 2);
        assert(pos[0] == 0 && pos[1] == 7);
    }
    {
        dsa::fm_index fm("banana");
        assert(fm.count("ana") == 2);
        auto pos = fm.locate("ana");
        assert(pos.size() == 2);
    }
    {
        // Extract roundtrip
        dsa::fm_index fm("hello world");
        assert(fm.extract() == "hello world");
    }

    std::println("All BWT/FM-index tests passed");
    return 0;
}
