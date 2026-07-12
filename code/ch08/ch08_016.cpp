#include <vector>
#include <map>
#include <string>
#include <functional>
#include <cmath>
#include <algorithm>

class DHTNode {
    int id;                        // this node's identifier
    int m;                         // number of bits in id space
    std::vector<int> fingerTable;  // successor for each finger
    std::map<int, std::string> store;  // local key-value storage

    // Compute the start of finger i: (id + 2^i) mod 2^m
    int fingerStart(int i) const {
        return (id + (1 << i)) % (1 << m);
    }

public:
    DHTNode(int nodeId, int bits)
        : id(nodeId), m(bits), fingerTable(bits) {
        for (int i = 0; i < m; i++)
            fingerTable[i] = id;  // placeholder
    }

    int getId() const { return id; }

    // Find the successor node responsible for a given key
    // Uses finger table to skip ahead -- O(log n) hops
    DHTNode* find_successor(DHTNode* allNodes[], int n,
                            int key) {
        // Check if key falls in (this, successor]
        int succ = fingerTable[0];
        if (key > id && key <= succ)
            return this;

        // Find closest preceding node using finger table
        DHTNode* next = nullptr;
        for (int i = m - 1; i >= 0; i--) {
            if (fingerTable[i] > id &&
                fingerTable[i] < key) {
                // In a real system, we would contact
                // the node with this id over the network
                for (int j = 0; j < n; j++) {
                    if (allNodes[j]->id == fingerTable[i]) {
                        next = allNodes[j];
                        break;
                    }
                }
                break;
            }
        }
        if (!next) next = this;  // wrap around

        // Recurse: delegate lookup to the next node
        return next->find_successor(allNodes, n, key);
    }

    // Insert a key-value pair
    void put(int key, const std::string& value) {
        store[key] = value;
    }

    // Retrieve a value by key
    std::string get(int key) const {
        auto it = store.find(key);
        if (it != store.end()) return it->second;
        return "";
    }

    // Build finger table given the full set of nodes
    void buildFingers(DHTNode* allNodes[], int n) {
        for (int i = 0; i < m; i++) {
            int start = fingerStart(i);
            int best = allNodes[0]->id;
            for (int j = 0; j < n; j++) {
                int nid = allNodes[j]->id;
                if (nid >= start) {
                    best = nid;
                    break;
                }
            }
            fingerTable[i] = best;
        }
    }
};
