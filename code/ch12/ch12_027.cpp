struct WEVertex {
    double x, y, z;            // position
    int edgeIdx;               // one incident edge (any)
};

struct WEFace {
    int edgeIdx;               // one edge of this face (any)
    int valence;               // number of edges/vertices in face (3 for triangles)
};

struct WingedEdge {
    int v1, v2;                // endpoint vertex indices
    int f1, f2;                // adjacent face indices (f2 = -1 for boundary)
    int eCW1, eCCW1;          // edges CW/CCW around f1
    int eCW2, eCCW2;          // edges CW/CCW around f2
};

class WingedEdgeMesh {
    vector<WEVertex> verts;
    vector<WEFace>   faces;
    vector<WingedEdge> edges;

public:
    int addVertex(double x, double y, double z) {
        int idx = verts.size();
        verts.push_back({x, y, z, -1});
        return idx;
    }

    // Add a directed edge; caller must supply face-adjacent edge links
    int addEdge(int v1, int v2, int f1, int f2,
                int eCW1, int eCCW1, int eCW2, int eCCW2) {
        int idx = edges.size();
        edges.push_back({v1, v2, f1, f2, eCW1, eCCW1, eCW2, eCCW2});
        if (verts[v1].edgeIdx == -1) verts[v1].edgeIdx = idx;
        if (verts[v2].edgeIdx == -1) verts[v2].edgeIdx = idx;
        return idx;
    }

    // Get the other vertex of an edge (not v)
    int otherVertex(int edgeIdx, int v) const {
        const auto& e = edges[edgeIdx];
        return (e.v1 == v) ? e.v2 : e.v1;
    }

    // Get the opposite face of an edge (not f)
    int otherFace(int edgeIdx, int f) const {
        const auto& e = edges[edgeIdx];
        return (e.f1 == f) ? e.f2 : e.f1;
    }

    // Walk clockwise around a face, collecting edges
    vector<int> faceEdges(int faceIdx) const {
        vector<int> result;
        int startEdge = faces[faceIdx].edgeIdx;
        int curr = startEdge;
        do {
            result.push_back(curr);
            const auto& e = edges[curr];
            if (e.f1 == faceIdx)
                curr = e.eCW1;
            else
                curr = e.eCW2;
        } while (curr != startEdge && curr != -1);
        return result;
    }

    // Walk edges incident to a vertex (one ring)
    vector<int> vertexEdges(int vertIdx) const {
        vector<int> result;
        int startEdge = verts[vertIdx].edgeIdx;
        if (startEdge == -1) return result;
        int curr = startEdge;
        do {
            result.push_back(curr);
            int nextV = otherVertex(curr, vertIdx);
            // find an edge around nextV that brings us back
            const auto& e = edges[curr];
            int nextEdge = -1;
            if (e.f1 != -1 && e.f2 != -1) {
                // interior edge: go to opposite face, take the other edge there
                if (e.f1 == faces[curr].edgeIdx)
                    nextEdge = e.eCCW2;
                else
                    nextEdge = e.eCCW1;
            } else {
                // boundary: reverse direction
                nextEdge = (e.f1 != -1) ? e.eCCW1 : e.eCCW2;
            }
            curr = nextEdge;
        } while (curr != startEdge && curr != -1);
        return result;
    }

    int numVertices() const { return verts.size(); }
    int numEdges()    const { return edges.size(); }
    int numFaces()    const { return faces.size(); }
    const WEVertex& vertex(int i) const { return verts[i]; }
    const WEFace&   face(int i)   const { return faces[i]; }
    const WingedEdge& edge(int i) const { return edges[i]; }
};
