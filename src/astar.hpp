#ifndef __ASTAR_HPP__
#define __ASTAR_HPP__

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "graphics.hpp"

// A C++ version of A* pathfinding algorithm from https://dotnetcoretutorials.com/2020/07/25/a-search-pathfinding-algorithm-in-c/
// Most of the comments from the original version are preserved and/or have minor modifications.
//
// This version uses smart pointers
namespace AStar
{
    template <typename T>
    using Smart = std::shared_ptr<T>;

    // Cartesian coordinates (see Path class below)
    class Point
    {
    public:
        int X;

        int Y;

        Point()
        {
        }

        Point(int x, int y)
        {
            X = x;

            Y = y;
        }
    };

    // Path found by A* algorithm
    class Path
    {
    public:
        // List of coordinates of the path
        std::vector<AStar::Point> Points;

        Path()
        {
        }
    };

    // Class representing a node in the graph
    class Node
    {
    public:
        int X;

        int Y;

        int Cost = 0.0;

        int Distance = 0;

        Smart<AStar::Node> Parent = nullptr;

        Node()
        {
        }

        Node(int x, int y)
        {
            X = x;

            Y = y;
        }

        Node(int x, int y, int cost, Smart<AStar::Node> &parent)
        {
            X = x;

            Y = y;

            Cost = cost;

            Parent = parent;
        }

        // Total cost to traverse this node
        int CostDistance()
        {
            return (Cost + Distance);
        }

        // The distance is essentially the estimated distance, ignoring obstacles to our target.
        // So how many nodes left and right, up and down, ignoring obstacles, to get there.
        //
        // Computes the 2D Manhattan Distance
        void SetDistance(Smart<AStar::Node> &node)
        {
            Distance = std::abs(node->X - X) + std::abs(node->Y - Y);
        }
    };

    typedef std::vector<Smart<AStar::Node>> Moves;

    bool IsPassable(Map::Base &map, Smart<AStar::Node> &target, int X, int Y, bool isEnemy, bool ignore)
    {
        auto IsValid = (X >= 0 && X < map.Width && Y >= 0 && Y < map.Height);

        if (IsValid)
        {
            Map::Tile &Tile = map.Tiles[Y][X];

            auto NotOccupied = !Tile.IsOccupied();

            auto IsDestination = (Y == target->Y && X == target->X);

            auto IsPassable = Tile.IsPassable && (ignore || NotOccupied);

            auto IsPassableToEnemy = isEnemy && Tile.IsPassableToEnemy && (ignore || NotOccupied);

            auto IsExit = !isEnemy && Tile.IsExit() && NotOccupied;

            auto IsTarget = isEnemy && Tile.IsPlayer();

            return (IsPassable || IsDestination || IsPassableToEnemy || IsExit || IsTarget);
        }
        else
        {
            return false;
        }
    }

    // Get all traversible nodes from current node
    Moves Nodes(Map::Base &map, Smart<AStar::Node> &current, Smart<AStar::Node> &target, bool isEnemy, bool ignore)
    {
        // Define neighbors (X, Y): Up, Down, Left, Right
        std::vector<std::pair<int, int>> neighbors = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        auto traversable = Moves();

        if (map.Width > 0 && map.Height > 0)
        {
            auto index = 0;

            for (auto &neighbor : neighbors)
            {
                // Check if within map boundaries and if passable and/or leads to destination
                if (AStar::IsPassable(map, target, current->X + neighbor.first, current->Y + neighbor.second, isEnemy, ignore))
                {
                    auto X = current->X + neighbor.first;

                    auto Y = current->Y + neighbor.second;

                    auto Cost = current->Cost + 1;

                    traversable.push_back(std::make_shared<AStar::Node>(X, Y, Cost, current));

                    traversable[index]->SetDistance(target);

                    index++;
                }
            }
        }

        return traversable;
    }

    // Get index of node from a list
    Moves::iterator Find(Moves &nodes, Smart<AStar::Node> &node)
    {
        return std::find_if(nodes.begin(), nodes.end(), [node](const Smart<Node> &f) -> bool
                            { return f->X == node->X && f->Y == node->Y; });
        ;
    }

    // Remove node from list
    void Remove(Moves &nodes, Smart<AStar::Node> &node)
    {
        auto found = AStar::Find(nodes, node);

        if (found != nodes.end())
        {
            nodes.erase(found);
        }
    }

    // Check if node is on the list
    bool Is(Moves &nodes, Smart<AStar::Node> &node)
    {
        return AStar::Find(nodes, node) != nodes.end();
    }

    // Find path from src to dst using the A* algorithm
    AStar::Path FindPath(Map::Base &map, int srcX, int srcY, int dstX, int dstY, bool ignore)
    {
        auto path = AStar::Path();

        auto ValidX = srcX >= 0 && srcX < map.Width && dstX >= 0 && dstX < map.Width;

        auto ValidY = srcY >= 0 && srcY < map.Height && dstY >= 0 && dstY < map.Height;

        if (map.Width > 0 && map.Height > 0 && ValidX && ValidY)
        {
            auto start = std::make_shared<AStar::Node>(srcX, srcY);

            auto end = std::make_shared<AStar::Node>(dstX, dstY);

            start->SetDistance(end);

            // List of nodes to be checked
            auto active = Moves();

            // List of nodes already visited
            auto visited = Moves();

            active.push_back(start);

            auto isEnemy = map.Tiles[srcY][srcX].IsEnemy();

            while (!active.empty())
            {
                // Sort based on CostDistance
                std::sort(active.begin(), active.end(), [](Smart<AStar::Node> &src, Smart<AStar::Node> &dst)
                          { return src->CostDistance() < dst->CostDistance(); });

                auto check = active.front();

                if (check->X == end->X && check->Y == end->Y)
                {
                    // We found the destination and we can be sure (because of the sort order above)
                    // that it's the most low cost option.
                    auto node = check;

                    while (node)
                    {
                        path.Points.push_back(AStar::Point(node->X, node->Y));

                        node = node->Parent;
                    }

                    // Reverse list of coordinates so path leads from src to dst
                    std::reverse(path.Points.begin(), path.Points.end());

                    return path;
                }

                visited.push_back(check);

                AStar::Remove(active, check);

                auto nodes = AStar::Nodes(map, check, end, isEnemy, ignore);

                for (auto &node : nodes)
                {
                    // We have already visited this node so we don't need to do so again!
                    if (AStar::Is(visited, node))
                    {
                        continue;
                    }

                    // It's already in the active list, but that's OK, maybe this new node has a better value (e.g. We might zigzag earlier but this is now straighter).
                    if (AStar::Is(active, node))
                    {
                        auto existing = *AStar::Find(active, node);

                        if (existing->CostDistance() > node->CostDistance())
                        {
                            AStar::Remove(active, existing);

                            active.push_back(node);
                        }
                    }
                    else
                    {
                        // We've never seen this node before so add it to the list.
                        active.push_back(node);
                    }
                }
            }
        }

        return path;
    }

    AStar::Path FindPath(Map::Base &map, int srcX, int srcY, int dstX, int dstY)
    {
        return AStar::FindPath(map, srcX, srcY, dstX, dstY, false);
    }
}
#endif
