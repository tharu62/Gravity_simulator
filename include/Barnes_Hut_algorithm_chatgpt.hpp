#ifndef BARNESHUT_HPP
#define BARNESHUT_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <chrono>
#include "SFML/Graphics.hpp"
#include "celestial_body.hpp"
#include "vector_operator.hpp"  // must provide .length() and .lengthSquared()

#define THETA 0.1f
#define G_ 1.f

extern float MAX_VISIBLE_SIZE;
extern int GALAXY_DIMENSION;

// ---------------------------------------------------------------
//  Barnes–Hut Quadtree Data Structures
// ---------------------------------------------------------------

namespace Barnes_Hut_struct {

    constexpr float MIN_R2_LIMIT = 1.f; // squared distance limit (avoid self)
    constexpr u_int32_t NO_CHILD = std::numeric_limits<u_int32_t>::max();

    struct Node {
        u_int32_t next[4];     // indices of 4 children (or NO_CHILD)
        float size;            // half-width of square region
        float mass;            // total mass
        sf::Vector2f center;   // geometric center of node
        sf::Vector2f centerOfMass;

        Node() : next{NO_CHILD, NO_CHILD, NO_CHILD, NO_CHILD}, size(0.f), mass(0.0f), center(0.f, 0.f), centerOfMass(0.f, 0.f) {}
    };

    struct Quadtree {
        std::vector<Node> qtree;

        // -----------------------------------------------------------
        // Initialization
        // -----------------------------------------------------------
        void init() {
            qtree.clear();
            qtree.reserve(GALAXY_DIMENSION * 8); // generous estimate
            qtree.emplace_back();
            qtree[0].center = {640.f, 360.f};
            qtree[0].size   = MAX_VISIBLE_SIZE;
            qtree[0].mass   = 0.0f;
            qtree[0].centerOfMass = {0.f, 0.f};
        }

        // -----------------------------------------------------------
        // Quadrant selector: returns 0,1,2,3 child index
        // -----------------------------------------------------------
        inline int getQuadrant(const Node &n, const sf::Vector2f &p) const {
            bool right = (p.x >= n.center.x);
            bool down  = (p.y >= n.center.y);
            if ( right && !down ) return 0; // NE
            if (!right && !down ) return 1; // NW
            if (!right &&  down ) return 2; // SW
            return 3;                       // SE
        }

        // -----------------------------------------------------------
        // Subdivide: create four child nodes and link them
        // -----------------------------------------------------------
        void subdivide(u_int32_t idx) {
            Node &n = qtree[idx];
            float half = n.size / 2.f;
            size_t base = qtree.size();
            qtree.reserve(base + 4);

            qtree.emplace_back(); // 0 NE
            qtree.emplace_back(); // 1 NW
            qtree.emplace_back(); // 2 SW
            qtree.emplace_back(); // 3 SE

            qtree[base + 0].center = { n.center.x + half, n.center.y - half };
            qtree[base + 1].center = { n.center.x - half, n.center.y - half };
            qtree[base + 2].center = { n.center.x - half, n.center.y + half };
            qtree[base + 3].center = { n.center.x + half, n.center.y + half };

            for (int i = 0; i < 4; ++i) {
                qtree[base + i].size = half;
                n.next[i] = static_cast<u_int32_t>(base + i);
            }
        }

        // -----------------------------------------------------------
        // Safe insert: move existing body into child, then recompute parent mass & COM
        // -----------------------------------------------------------
        void insertBody(u_int32_t node, float mass, const sf::Vector2f &pos) {
            Node &n = qtree[node];

            // ensure body lies within node bounds
            if (std::fabs(pos.x - n.center.x) > n.size || std::fabs(pos.y - n.center.y) > n.size)
                return;

            // empty leaf → insert here
            if (n.mass == 0.0f && n.next[0] == NO_CHILD) {
                n.mass = mass;
                n.centerOfMass = pos;
                return;
            }

            // leaf with existing mass → subdivide and redistribute
            if (n.next[0] == NO_CHILD) {
                float oldMass = n.mass;
                sf::Vector2f oldCOM = n.centerOfMass;

                subdivide(node);
                insertBody(n.next[getQuadrant(n, oldCOM)], oldMass, oldCOM);

                // clear parent; will be recomputed
                n.mass = 0.0f;
                n.centerOfMass = {0.f, 0.f};
            }

            // insert new mass into correct child
            insertBody(n.next[getQuadrant(n, pos)], mass, pos);

            // recompute parent mass and center of mass from children
            float totalMass = 0.0;
            float cx = 0.0, cy = 0.0;
            for (int c = 0; c < 4; ++c) {
                u_int32_t child = n.next[c];
                if (child != NO_CHILD && qtree[child].mass > 0.0f) {
                    float m = qtree[child].mass;
                    totalMass += m;
                    cx += static_cast<float>(qtree[child].centerOfMass.x) * m;
                    cy += static_cast<float>(qtree[child].centerOfMass.y) * m;
                }
            }

            if (totalMass > 0.0) {
                n.centerOfMass.x = static_cast<float>(cx / totalMass);
                n.centerOfMass.y = static_cast<float>(cy / totalMass);
            } else {
                n.centerOfMass = {0.f, 0.f};
            }
            n.mass = static_cast<float>(totalMass);
        }

        // -----------------------------------------------------------
        // Insert all bodies
        // -----------------------------------------------------------
        void insertAll(Celestial_body *galaxy) {
            for (int k = 0; k < GALAXY_DIMENSION; ++k) {
                galaxy[k].acceleration = {0.f, 0.f};
                if (std::fabs(galaxy[k].position.x) <= MAX_VISIBLE_SIZE && std::fabs(galaxy[k].position.y) <= MAX_VISIBLE_SIZE) {
                    insertBody(0, galaxy[k].mass, galaxy[k].position);
                } else {
                    galaxy[k].velocity = {0.f, 0.f};
                }
            }
        }

        // -----------------------------------------------------------
        // Optimized Barnes–Hut acceleration update
        // -----------------------------------------------------------
        sf::Vector2f updateAcceleration(const sf::Vector2f &pos) {
            sf::Vector2f acc{0.f, 0.f};
            std::vector<u_int32_t> stack;
            stack.reserve(128);
            stack.push_back(0);

            while (!stack.empty()) {
                u_int32_t idx = stack.back();
                stack.pop_back();

                const Node &node = qtree[idx];
                if (node.mass <= 0.f) continue;

                sf::Vector2f rvec = node.centerOfMass - pos;
                float r2 = rvec.x * rvec.x + rvec.y * rvec.y;
                if (r2 < MIN_R2_LIMIT) continue; // avoid self-interaction

                float invR = 1.0f / std::sqrt(r2);
                float sizeOverR = (node.size * 2.f) * invR;

                if (node.next[0] == NO_CHILD || sizeOverR < THETA) {
                    float invR3 = invR * invR * invR;
                    float scale = G_ * node.mass * invR3;
                    acc.x += rvec.x * scale;
                    acc.y += rvec.y * scale;
                } else {
                    for (int c = 0; c < 4; ++c) {
                        u_int32_t child = node.next[c];
                        if (child != NO_CHILD) stack.push_back(child);
                    }
                }
            }
            return acc;
        }

        // -----------------------------------------------------------
        // Update acceleration for all bodies
        // -----------------------------------------------------------
        void updateAllAccelerations(Celestial_body *galaxy) {
            for (int i = 0; i < GALAXY_DIMENSION; ++i) {
                galaxy[i].acceleration = updateAcceleration(galaxy[i].position);
            }
        }
    };

} // namespace Barnes_Hut_struct

// ---------------------------------------------------------------
//  High-level function: compute all forces
// ---------------------------------------------------------------

namespace Barnes_Hut {

    using namespace Barnes_Hut_struct;

    void compute_forces(Celestial_body *galaxy, Quadtree &tree) {
        using namespace std::chrono;
        tree.init();

        auto t_insert_start = high_resolution_clock::now();
        tree.insertAll(galaxy);
        auto t_insert_end = high_resolution_clock::now();

        auto t_update_start = high_resolution_clock::now();
        tree.updateAllAccelerations(galaxy);
        auto t_update_end = high_resolution_clock::now();

        auto insert_us = duration_cast<microseconds>(t_insert_end - t_insert_start).count();
        auto update_us = duration_cast<microseconds>(t_update_end - t_update_start).count();

        std::cout << "insertAll: " << insert_us << "us, updateAllAccelerations: " << update_us << "us\n";
    }

} // namespace Barnes_Hut

#endif // BARNESHUT_HPP
