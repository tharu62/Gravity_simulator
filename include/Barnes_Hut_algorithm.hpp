#ifndef BARNESHUT_HPP
#define BARNESHUT_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "vector_operator.hpp"
#include "celestial_body.hpp"

extern int GALAXY_DIMENSION;
extern float MAX_VISIBLE_SIZE;

constexpr float THETA = 1.f;
constexpr float MAX_DISTANCE = 0.000001f;  // CRITICAL FOR REALISTIC SIMULATION

namespace Barnes_Hut_struct {

    /**
     * @brief Represents a node in the quadtree.
     */
    struct Node {
        std::uint32_t next[4]{0, 0, 0, 0};
        float size{0.f};
        float mass{0.f};
        sf::Vector2f center{0.f, 0.f};
        sf::Vector2f centerOfMass{0.f, 0.f};
    };

    /**
     * @brief Quadtree data structure for the Barnes–Hut algorithm.
     */
    struct Quadtree {
        std::vector<Node> qtree;
        std::vector<std::uint32_t> heap_stack;
        std::stack<std::uint32_t> stack;

        // -----------------------------------------------------------
        // Initialization
        // -----------------------------------------------------------
        void init() {
            qtree.clear();
            qtree.reserve(static_cast<std::size_t>(GALAXY_DIMENSION * 2 * std::log2(GALAXY_DIMENSION)));
            qtree.emplace_back();
            qtree[0].size = MAX_VISIBLE_SIZE;
        }

        // -----------------------------------------------------------
        // Subdivide node into 4 quadrants
        // -----------------------------------------------------------
        void subdivide(std::uint32_t node){

            // if (node < 0 || node >= qtree.size()) {
            //     std::cerr << "Error: Node index out of bounds in subdivide()." << std::endl;
            //     return;
            // }

            std::uint32_t qtree_size = qtree.size();
            float new_node_size = qtree[node].size/2;

            // node I
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size].center = {(qtree[node].center.x + new_node_size), (qtree[node].center.y - new_node_size)};
            qtree[qtree_size].size = new_node_size;
            qtree[node].next[0] = qtree_size;

            // node II
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +1].center = {(qtree[node].center.x - new_node_size), (qtree[node].center.y - new_node_size)};
            qtree[qtree_size +1].size = new_node_size;
            qtree[node].next[1] = qtree_size +1;

            // node III
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +2].center = {(qtree[node].center.x - new_node_size), (qtree[node].center.y + new_node_size)};
            qtree[qtree_size +2].size = new_node_size;
            qtree[node].next[2] = qtree_size +2;

            // node IV
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +3].center = {(qtree[node].center.x + new_node_size), (qtree[node].center.y + new_node_size)};
            qtree[qtree_size +3].size = new_node_size;
            qtree[node].next[3] = qtree_size +3;
        }

        // -----------------------------------------------------------
        // Insert single mass into tree using Stack
        // -----------------------------------------------------------
        void insertBodyStack(float mass, const sf::Vector2f& pos, std::uint32_t qtree_idx) {
            while(!stack.empty()) stack.pop();
            stack.push(qtree_idx);
            std::uint32_t i;

            while(!stack.empty()){
                
                i = stack.top();
                stack.pop();
                Node& node = qtree[i];

                // If the node does not contain a body, put the new body here.
                if (node.mass == 0.f) { 
                    node.centerOfMass = pos;
                    node.mass = mass;
                }

                // If the node is an internal node, update the center-of-mass and total mass of the node and 
                // iterativly insert the body in the appropriate quadrant.
                else if(node.next[0] != 0){
                    node.centerOfMass = (node.centerOfMass * node.mass + pos * mass) / (node.mass + mass);
                    node.mass += mass;
                    for (std::uint32_t j = node.next[0]; j < node.next[0] + 4; ++j) {
                        if (std::fabs(qtree[j].center.x - pos.x) <= qtree[j].size && std::fabs(qtree[j].center.y - pos.y) <= qtree[j].size) {
                            stack.push(j);
                            break;
                        }
                    }
                }

                // If the node is an external node, say containing a body named B, then there are two bodies A and B in the same region. 
                // Subdivide the region further by creating four children. Then, iterativly insert both A and B into the appropriate quadrant(s). 
                // Since A and B may still end up in the same quadrant, there may be several subdivisions during a single insertion. 
                // Finally, update the center-of-mass and total mass of the node.
                else {
                    subdivide(i);
                    for (std::uint32_t j = node.next[0]; j < node.next[0] + 4; ++j) { 
                        Node& child = qtree[j];
                        if (std::fabs(child.center.x - node.centerOfMass.x) <= child.size && std::fabs(child.center.y - node.centerOfMass.y) <= child.size) {
                            child.centerOfMass = node.centerOfMass;
                            child.mass = node.mass;
                        }
                        if(std::fabs(child.center.x - pos.x) <= child.size && std::fabs(child.center.y - pos.y) <= child.size) {
                            stack.push(j);
                        }
                    }
                    node.centerOfMass = (node.centerOfMass * node.mass + pos * mass) / (node.mass + mass);
                    node.mass += mass; 
                }
            }
        }

        // -----------------------------------------------------------
        // Insert single mass into tree using Heap
        // -----------------------------------------------------------
        void insertBodyHeap(float mass, const sf::Vector2f& pos, std::uint32_t qtree_idx) {
            heap_stack.clear();
            heap_stack.push_back(qtree_idx);
            std::uint32_t i;

            while(!heap_stack.empty()){
                
                i = heap_stack.back();
                heap_stack.pop_back();
                Node& node = qtree[i];

                // If the node does not contain a body, put the new body here.
                if (node.mass == 0.f) { 
                    node.centerOfMass = pos;
                    node.mass = mass;
                }

                // If the node is an internal node, update the center-of-mass and total mass of the node and 
                // iterativly insert the body in the appropriate quadrant.
                else if(node.next[0] != 0){
                    node.centerOfMass = (node.centerOfMass * node.mass + pos * mass) / (node.mass + mass);
                    node.mass += mass;
                    for (std::uint32_t j = node.next[0]; j < node.next[0] + 4; ++j) {
                        if (std::fabs(qtree[j].center.x - pos.x) <= qtree[j].size && std::fabs(qtree[j].center.y - pos.y) <= qtree[j].size) {
                            heap_stack.push_back(j);
                            break;
                        }
                    }
                }

                // If the node is an external node, say containing a body named B, then there are two bodies A and B in the same region. 
                // Subdivide the region further by creating four children. Then, iterativly insert both A and B into the appropriate quadrant(s). 
                // Since A and B may still end up in the same quadrant, there may be several subdivisions during a single insertion. 
                // Finally, update the center-of-mass and total mass of the node.
                else {
                    subdivide(i);
                    for (std::uint32_t j = node.next[0]; j < node.next[0] + 4; ++j) { 
                        Node& child = qtree[j];
                        if (std::fabs(child.center.x - node.centerOfMass.x) <= child.size && std::fabs(child.center.y - node.centerOfMass.y) <= child.size) {
                            child.centerOfMass = node.centerOfMass;
                            child.mass = node.mass;
                        }
                        if(std::fabs(child.center.x - pos.x) <= child.size && std::fabs(child.center.y - pos.y) <= child.size) {
                            heap_stack.push_back(j);
                        }
                    }
                    node.centerOfMass = (node.centerOfMass * node.mass + pos * mass) / (node.mass + mass);
                    node.mass += mass; 
                }
            }
        }

        // --------------------------------------------------------------
        // Iterative acceleration computation for a position using Stack
        // --------------------------------------------------------------
        sf::Vector2f computeAccelerationStack(const sf::Vector2f& pos) {
            sf::Vector2f acc{0.f, 0.f};
            while (!stack.empty()) stack.pop(); // clear
            stack.push(0);

            while (!stack.empty()) {
                std::uint32_t idx = stack.top();
                stack.pop();

                const Node& node = qtree[idx];
                if (node.mass == 0.f) continue;

                sf::Vector2f rvec = node.centerOfMass - pos;
                float r2 = rvec.x * rvec.x + rvec.y * rvec.y;
                float r = std::sqrt(r2);

                if (r < MAX_DISTANCE) continue;    // CRITICAL POINT OF FAILURE FOR SIMULATION

                if (node.next[0] == 0 || (node.size * 2.f / r) < THETA) {
                    acc += rvec * (node.mass / (r2 * r));
                } else {
                    for (int c = 0; c < 4; ++c) {
                        if (node.next[c] != 0) stack.push(node.next[c]);
                    }
                }
            }
            return acc;

        }

        // -------------------------------------------------------------
        // Iterative acceleration computation for a position using Heap
        // -------------------------------------------------------------
        sf::Vector2f computeAccelerationHeap(const sf::Vector2f& pos) {
            sf::Vector2f acc{0.f, 0.f};
            heap_stack.clear();
            heap_stack.push_back(0);        
            
            while (!heap_stack.empty()) {
                std::uint32_t idx = heap_stack.back();
                heap_stack.pop_back();

                const Node& node = qtree[idx];
                if (node.mass == 0.f) continue;

                sf::Vector2f rvec = node.centerOfMass - pos;
                float r2 = rvec.x * rvec.x + rvec.y * rvec.y;
                float r = std::sqrt(r2);

                if (r < MAX_DISTANCE) continue;    // CRITICAL POINT OF FAILURE FOR SIMULATION

                if (node.next[0] == 0 || (node.size * 2.f / r) < THETA) {
                    acc += rvec * (node.mass / (r2 * r));
                } else {
                    for (int c = 0; c < 4; ++c) {
                        if (node.next[c] != 0) heap_stack.push_back(node.next[c]);
                    }
                }
            }
            return acc;
        }

    };

}

namespace Barnes_Hut {

    using namespace Barnes_Hut_struct;
    // using namespace std::chrono;

    /**
     * @brief Computes an approximated acceleration for each body in galaxy using the given Quadtree.
     */
    void compute_forces(Celestial_body* galaxy, Quadtree& qtree) {
       
        qtree.init();

        // Insert bodies
        // auto t_insert_start = high_resolution_clock::now();
        for (int i = 0; i < GALAXY_DIMENSION; ++i) {
            const auto& body = galaxy[i];
            if (std::fabs(body.position.x) <= MAX_VISIBLE_SIZE && std::fabs(body.position.y) <= MAX_VISIBLE_SIZE) {
                qtree.insertBodyStack(body.mass, body.position, 0);
                // qtree.insertBodyHeap(body.mass, body.position, 0); //heap is slower but has more memory => more bodies
            } else {
                galaxy[i].acceleration = {0.f, 0.f};
                galaxy[i].velocity = {0.f, 0.f};
            }
        }
        // auto t_insert_end = high_resolution_clock::now();

        // Compute accelerations
        // auto t_update_start = high_resolution_clock::now();
        for (int i = 0; i < GALAXY_DIMENSION; ++i) {
            const auto& pos = galaxy[i].position;
            if (std::fabs(pos.x) <= MAX_VISIBLE_SIZE && std::fabs(pos.y) <= MAX_VISIBLE_SIZE) {
                galaxy[i].acceleration = qtree.computeAccelerationStack(pos);
                // galaxy[i].acceleration = qtree.computeAccelerationHeap(pos);  //heap is slower but has more memory => more bodies
            }
        }
        // auto t_update_end = high_resolution_clock::now();

        // auto insert_us = duration_cast<microseconds>(t_insert_end - t_insert_start).count();
        // auto update_us = duration_cast<microseconds>(t_update_end - t_update_start).count();
        // std::cout << "insertAll: " << insert_us << "us, updateAllAccelerations: " << update_us << "us\n";
    }

} // namespace BarnesHut

#endif // BARNESHUT_HPP
