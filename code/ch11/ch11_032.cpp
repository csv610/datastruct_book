struct vec3 {
    float x, y, z;
    vec3(float x = 0, float y = 0, float z = 0)
        : x(x), y(y), z(z) {}
    vec3 operator+(const vec3& o) const
        { return {x+o.x, y+o.y, z+o.z}; }
    vec3 operator*(float s) const
        { return {x*s, y*s, z*s}; }
};

struct rect3d {
    vec3 min, max;
    vec3 center() const {
        return {(min.x+max.x)/2,
                (min.y+max.y)/2,
                (min.z+max.z)/2};
    }
    float size() const {
        return max.x - min.x;
    }
};

class octree {
    static constexpr int MAX_OBJECTS = 8;

    struct node {
        rect3d bounds;
        std::vector<vec3> objects;
        node* children[8];
        bool divided;

        node(const rect3d& b)
            : bounds(b), divided(false) {
            for (int i = 0; i < 8; ++i)
                children[i] = nullptr;
        }

        int octant(const vec3& p) const {
            vec3 c = bounds.center();
            int idx = 0;
            if (p.x >= c.x) idx |= 1;
            if (p.y >= c.y) idx |= 2;
            if (p.z >= c.z) idx |= 4;
            return idx;
        }

        void subdivide() {
            vec3 c = bounds.center();
            vec3 mn = bounds.min;
            vec3 mx = bounds.max;

            // 0: (-,-,-)
            children[0] = new node({mn, c});
            // 1: (+,-,-)
            children[1] = new node(
                {{c.x, mn.y, mn.z},
                 {mx.x, c.y, c.z}});
            // 2: (-,+,-)
            children[2] = new node(
                {{mn.x, c.y, mn.z},
                 {c.x, mx.y, c.z}});
            // 3: (+,+,-)
            children[3] = new node(
                {{c.x, c.y, mn.z},
                 {mx.x, mx.y, c.z}});
            // 4: (-,-,+)
            children[4] = new node(
                {{mn.x, mn.y, c.z},
                 {c.x, c.y, mx.z}});
            // 5: (+,-,+)
            children[5] = new node(
                {{c.x, mn.y, c.z},
                 {mx.x, c.y, mx.z}});
            // 6: (-,+,+)
            children[6] = new node(
                {{mn.x, c.y, c.z},
                 {c.x, mx.y, mx.z}});
            // 7: (+,+,+)
            children[7] = new node(
                {{c.x, c.y, c.z}, {mx, mx}});
            divided = true;

            std::vector<vec3> old =
                std::move(objects);
            for (auto& p : old)
                children[octant(p)]->insert(p);
        }

        void insert(const vec3& p) {
            if (!bounds_contains(p))
                return;
            if (!divided) {
                objects.push_back(p);
                if ((int)objects.size() >
                    MAX_OBJECTS)
                    subdivide();
                return;
            }
            children[octant(p)]->insert(p);
        }

        bool bounds_contains(
                const vec3& p) const {
            return (p.x >= bounds.min.x &&
                    p.x <= bounds.max.x &&
                    p.y >= bounds.min.y &&
                    p.y <= bounds.max.y &&
                    p.z >= bounds.min.z &&
                    p.z <= bounds.max.z);
        }

        ~node() {
            for (int i = 0; i < 8; ++i)
                delete children[i];
        }
    };

    node* root;

    static bool sphere_intersects(
            const rect3d& box,
            const vec3& center, float radius) {
        float r2 = radius * radius;
        float sq = 0;
        if (center.x < box.min.x)
            sq += (center.x - box.min.x) *
                  (center.x - box.min.x);
        else if (center.x > box.max.x)
            sq += (center.x - box.max.x) *
                  (center.x - box.max.x);
        if (center.y < box.min.y)
            sq += (center.y - box.min.y) *
                  (center.y - box.min.y);
        else if (center.y > box.max.y)
            sq += (center.y - box.max.y) *
                  (center.y - box.max.y);
        if (center.z < box.min.z)
            sq += (center.z - box.min.z) *
                  (center.z - box.min.z);
        else if (center.z > box.max.z)
            sq += (center.z - box.max.z) *
                  (center.z - box.max.z);
        return sq <= r2;
    }

    static float dist(const vec3& a,
                      const vec3& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float dz = a.z - b.z;
        return dx*dx + dy*dy + dz*dz;
    }

public:
    octree(const rect3d& universe)
        : root(new node(universe)) {}

    void insert(const vec3& p) {
        root->insert(p);
    }

    void range_query(node* n,
                     const vec3& center,
                     float r,
                     std::vector<vec3>& result) {
        if (!n) return;
        if (!sphere_intersects(
                n->bounds, center, r))
            return;
        for (auto& p : n->objects)
            if (dist(p, center) <= r)
                result.push_back(p);
        if (n->divided)
            for (int i = 0; i < 8; ++i)
                range_query(n->children[i],
                            center, r, result);
    }

    std::vector<vec3> range_query(
            const vec3& center, float r) {
        std::vector<vec3> res;
        range_query(root, center, r, res);
        return res;
    }

    vec3 nearest(node* n, const vec3& target,
                 float& best_dist, vec3 best) {
        if (!n) return best;
        if (!sphere_intersects(
                n->bounds, target, best_dist))
            return best;
        for (auto& p : n->objects) {
            float d = dist(p, target);
            if (d < best_dist) {
                best_dist = d;
                best = p;
            }
        }
        if (n->divided)
            for (int i = 0; i < 8; ++i)
                best = nearest(n->children[i],
                               target,
                               best_dist, best);
        return best;
    }

    vec3 nearest(const vec3& target) {
        float best_dist = 1e30f;
        return nearest(root, target,
                       best_dist, {0, 0, 0});
    }
};
