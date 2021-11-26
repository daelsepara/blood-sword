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

        std::shared_ptr<AStar::Node> Parent = nullptr;

        Node()
        {
        }

        Node(int x, int y, int cost, std::shared_ptr<AStar::Node> &parent)
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
        void SetDistance(std::shared_ptr<AStar::Node> &node)
        {
            Distance = std::abs(node->X - X) + std::abs(node->Y - Y);
        }
    };

    bool IsPassable(Map::Base &map, std::shared_ptr<AStar::Node> &target, int X, int Y, bool isEnemy, bool ignore)
    {
        return (X >= 0 && X < map.Width && Y >= 0 && Y < map.Height && ((map.Tiles[Y][X].IsPassable && ((ignore && isEnemy) || !map.Tiles[Y][X].IsOccupied())) || (Y == target->Y && X == target->X) || (isEnemy && map.Tiles[Y][X].IsPassableToEnemy && (ignore || !map.Tiles[Y][X].IsOccupied())) || (!isEnemy && map.Tiles[Y][X].IsExit())));
    }

    // Get all traversible nodes from current node
    std::vector<std::shared_ptr<AStar::Node>> Nodes(Map::Base &map, std::shared_ptr<AStar::Node> &current, std::shared_ptr<AStar::Node> &target, bool isEnemy, bool ignore)
    {
        // Define neighbors (X, Y): Up, Down, Left, Right
        std::vector<std::pair<int, int>> neighbors = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        auto traversable = std::vector<std::shared_ptr<AStar::Node>>();

        if (map.Width > 0 && map.Height > 0)
        {
            for (auto i = 0; i < neighbors.size(); i++)
            {
                auto index = 0;

                // Check if within map boundaries and if passable and/or leads to destination
                if (AStar::IsPassable(map, target, current->X + neighbors[i].first, current->Y + neighbors[i].second, isEnemy, ignore))
                {
                    auto X = current->X + neighbors[i].first;
                    auto Y = current->Y + neighbors[i].second;

                    auto Cost = current->Cost + 1;

                    if (X >= 0 && X < map.Width && Y >= 0 && Y < map.Height && isEnemy && map.Tiles[Y][X].IsEnemy())
                    {
                        // Enemies avoid other enemies as much as possible
                        Cost += 1;
                    }

                    traversable.push_back(std::make_shared<AStar::Node>(X, Y, Cost, current));

                    traversable[index]->SetDistance(target);

                    index++;
                }
            }
        }

        return traversable;
    }

    // Get index of node from a list
    int Index(std::vector<std::shared_ptr<AStar::Node>> &nodes, std::shared_ptr<AStar::Node> &node)
    {
        auto index = -1;

        for (auto i = 0; i < nodes.size(); i++)
        {
            if (nodes[i]->X == node->X && nodes[i]->Y == node->Y)
            {
                index = i;

                break;
            }
        }

        return index;
    }

    // Remove node from list
    void Remove(std::vector<std::shared_ptr<AStar::Node>> &nodes, std::shared_ptr<AStar::Node> &node)
    {
        auto index = AStar::Index(nodes, node);

        if (index >= 0 && index < nodes.size())
        {
            nodes.erase(nodes.begin() + index);
        }
    }

    // Check if node is on the list
    bool Any(std::vector<std::shared_ptr<AStar::Node>> &nodes, std::shared_ptr<AStar::Node> &node)
    {
        auto index = AStar::Index(nodes, node);

        return (index >= 0 && index < nodes.size());
    }

    // Find path from src to dst using the A* algorithm
    AStar::Path FindPath(Map::Base &map, int srcX, int srcY, int dstX, int dstY, bool ignore)
    {
        auto path = AStar::Path();

        if (map.Width > 0 && map.Height > 0)
        {
            auto start = std::make_shared<AStar::Node>();

            start->X = srcX;
            start->Y = srcY;

            auto end = std::make_shared<AStar::Node>();

            end->X = dstX;
            end->Y = dstY;

            start->SetDistance(end);

            // List of nodes to be checked
            auto active = std::vector<std::shared_ptr<AStar::Node>>();

            // List of nodes already visited
            auto visited = std::vector<std::shared_ptr<AStar::Node>>();

            active.push_back(start);

            auto isEnemy = map.Tiles[srcY][srcX].IsEnemy();

            while (active.size() > 0)
            {
                // Sort based on CostDistance
                std::sort(active.begin(), active.end(), [](std::shared_ptr<AStar::Node> &src, std::shared_ptr<AStar::Node> &dst)
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

                for (auto i = 0; i < nodes.size(); i++)
                {
                    auto node = nodes[i];

                    // We have already visited this node so we don't need to do so again!
                    if (AStar::Any(visited, node))
                    {
                        continue;
                    }

                    // It's already in the active list, but that's OK, maybe this new node has a better value (e.g. We might zigzag earlier but this is now straighter).
                    if (AStar::Any(active, node))
                    {
                        auto existing = active[AStar::Index(active, node)];

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
