#include <vector>
#include <cmath>
#include <algorithm>

struct vec3 {
    float x, y, z;
    vec3(float x=0, float y=0, float z=0)
        : x(x), y(y), z(z) {}
    vec3 operator-(const vec3& o) const
        { return {x-o.x, y-o.y, z-o.z}; }
    vec3 operator+(const vec3& o) const
        { return {x+o.x, y+o.y, z+o.z}; }
    vec3 operator*(float s) const
        { return {x*s, y*s, z*s}; }
};

struct plane {
    vec3 normal;           // unit normal
    float d;               // distance from origin

    plane() : d(0) {}
    plane(const vec3& n, float d)
        : normal(n), d(d) {}

    // Signed distance of point p to plane
    float dist(const vec3& p) const {
        return normal.x*p.x + normal.y*p.y
             + normal.z*p.z - d;
    }
};

struct polygon {
    std::vector<vec3> verts;
};

class bsp_tree {
    struct node {
        plane split;
        node* front;
        node* back;
        std::vector<polygon> polys;

        node() : front(nullptr), back(nullptr) {}
        ~node() { delete front; delete back; }
    };

    node* root;

    // Classify polygon against plane and split
    void classify(const polygon& poly,
                  const plane& pl,
                  std::vector<polygon>& front_list,
                  std::vector<polygon>& back_list,
                  std::vector<polygon>& on_list) {
        int n = poly.verts.size();
        std::vector<float> dists(n);
        for (int i = 0; i < n; ++i)
            dists[i] = pl.dist(poly.verts[i]);

        bool all_front = true, all_back = true;
        for (int i = 0; i < n; ++i) {
            if (dists[i] < -1e-6f)
                all_front = false;
            if (dists[i] >  1e-6f)
                all_back  = false;
        }

        if (all_front && !all_back) {
            front_list.push_back(poly);
            return;
        }
        if (all_back && !all_front) {
            back_list.push_back(poly);
            return;
        }
        if (std::abs(dists[0]) <= 1e-6f &&
            all_back) {
            on_list.push_back(poly);
            return;
        }

        // Polygon spans the plane; clip to both
        polygon front_poly, back_poly;
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            vec3 vi = poly.verts[i];
            vec3 vj = poly.verts[j];
            float di = dists[i];
            float dj = dists[j];

            front_poly.verts.push_back(vi);
            if (di * dj < -1e-6f) {
                float t = di / (di - dj);
                vec3 xpt = vi + (vj - vi) * t;
                front_poly.verts.push_back(xpt);
                back_poly.verts.insert(
                    back_poly.verts.begin(), xpt);
            }
            if (dj >= -1e-6f)
                back_poly.verts.push_back(vj);
        }
        if (!front_poly.verts.empty())
            front_list.push_back(front_poly);
        if (!back_poly.verts.empty())
            back_list.push_back(back_poly);
    }

    void build(node* n,
               std::vector<polygon>& polys) {
        if (polys.empty()) return;

        n->split = choose_plane(polys[0]);
        std::vector<polygon> front_list, back_list;

        for (auto& p : polys) {
            std::vector<polygon> on;
            classify(p, n->split,
                     front_list, back_list, on);
            if (!on.empty())
                n->polys.insert(
                    n->polys.end(),
                    on.begin(), on.end());
        }

        if (!front_list.empty()) {
            n->front = new node();
            build(n->front, front_list);
        }
        if (!back_list.empty()) {
            n->back = new node();
            build(n->back, back_list);
        }
    }

    // Traverse back-to-front relative to cam_pos
    void traverse(const node* n,
                  const vec3& cam_pos,
                  std::vector<polygon>& result) {
        if (!n) return;

        float side = n->split.dist(cam_pos);

        const node* first  = (side >= 0) ?
                             n->back  : n->front;
        const node* second = (side >= 0) ?
                             n->front : n->back;

        traverse(first, cam_pos, result);
        result.insert(result.end(),
            n->polys.begin(), n->polys.end());
        traverse(second, cam_pos, result);
    }

    static plane choose_plane(
            const polygon& poly) {
        vec3 v0 = poly.verts[0];
        vec3 v1 = poly.verts[1];
        vec3 edge = v1 - v0;
        vec3 n = {0, 0, 1};
        return {n, 0};
    }

public:
    bsp_tree() : root(nullptr) {}
    ~bsp_tree() { delete root; }

    void build(std::vector<polygon>& polys) {
        delete root;
        root = new node();
        build(root, polys);
    }

    std::vector<polygon> render_order(
            const vec3& cam_pos) const {
        std::vector<polygon> result;
        traverse(root, cam_pos, result);
        return result;
    }
};
