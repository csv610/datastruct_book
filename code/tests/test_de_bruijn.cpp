#include "de_bruijn.h"
#include <cassert>
#include <print>
#include <string>
#include <vector>

int main() {
    // de Bruijn graph assembly
    {
        dsa::de_bruijn_graph dbg(3);
        // Reads from "ABABA" (3-mers: ABA, BAB, ABA)
        dbg.add_edge("ABA");
        dbg.add_edge("BAB");
        dbg.add_edge("ABA");

        assert(dbg.edge_count() == 3);
        assert(dbg.nodes().size() == 2);  // "AB" and "BA"

        auto path = dbg.eulerian_path();
        assert(!path.empty());
        auto seq = dbg.reconstruct(path);
        assert(seq.size() >= 3);
    }

    // Build from sequence
    {
        dsa::de_bruijn_graph dbg(3);
        dbg.add_sequence("ACGTACG");
        // k-mers: ACG, CGT, GTA, TAC, ACG
        assert(dbg.edge_count() == 5);
    }

    // Build from reads
    {
        dsa::de_bruijn_graph dbg(4);
        dbg.add_reads({"ABCD", "CDEF", "EFGH"});
        assert(dbg.edge_count() == 3);
    }

    // de Bruijn sequence (classical)
    {
        // B(2,3) should contain all 3-bit binary strings: 000,001,010,011,100,101,110,111
        auto seq = dsa::de_bruijn_sequence(2, 3);
        assert(seq.size() == 8);
        // Should contain "000"
        assert(seq.find("000") != std::string::npos);
        assert(seq.find("111") != std::string::npos);
        assert(seq.find("101") != std::string::npos);
    }

    {
        // B(2,2): 00,01,10,11
        auto seq = dsa::de_bruijn_sequence(2, 2);
        assert(seq.size() == 4);
        assert(seq.find("00") != std::string::npos);
        assert(seq.find("11") != std::string::npos);
    }

    // Simple assembly test
    {
        dsa::de_bruijn_graph dbg(3);
        // "TACTAG" -> 3-mers: TAC, ACT, CTA, TAG
        dbg.add_sequence("TACTAG");
        auto path = dbg.eulerian_path();
        assert(!path.empty());
        auto assembled = dbg.reconstruct(path);
        assert(assembled.size() >= 4);  // at least k-1 + number_of_edges
    }

    std::println("All de Bruijn tests passed");
    return 0;
}
