#include <limits>
#include <list>
#include <chrono>
#include <ctime>
#include <ratio>
#include <iostream>
#include "AStar.h"
#include "Grid.h"

float Distance2Vecs(sf::Vector2i a, sf::Vector2i b)
{
	return sqrtf(float(a.x - b.x) * float(a.x - b.x) + float(a.y - b.y) * float(a.y - b.y));
}

float NodeDistance(GridNode* a, GridNode* b)
{
	return Distance2Vecs(a->nodePosition, b->nodePosition);
}

float Hueristic(GridNode* a, GridNode* b)
{
	return NodeDistance(a, b);
}

void AStar::FindPath()
{
	using namespace std::chrono;
	high_resolution_clock::time_point startTime = high_resolution_clock::now();

	for (int i = 0; i < GridManager::Get()->gridSizeX; i++)
	{
		for (int j = 0; j < GridManager::Get()->gridSizeY; j++)
		{
			GridNode* node = GridManager::GetNode(i, j);

			node->checked = false;
			node->globalScore = std::numeric_limits<float>::max();
			node->localScore = std::numeric_limits<float>::max();
			node->parent = nullptr;
		}
	}

	GridNode* checkedNode = GridManager::Get()->nodeStart;
	checkedNode->localScore = 0.f;
	checkedNode->globalScore = NodeDistance(GridManager::Get()->nodeStart, GridManager::Get()->nodeEnd);

	std::list<GridNode*> toCheckNodes;
	toCheckNodes.push_back(checkedNode);

	while (!toCheckNodes.empty() && checkedNode != GridManager::Get()->nodeEnd)
	{
		toCheckNodes.sort([](const GridNode* a, const GridNode* b) { return a->globalScore < b->globalScore; });

		while (!toCheckNodes.empty() && toCheckNodes.front()->checked) toCheckNodes.pop_front();

		if (toCheckNodes.empty())
			break;

		checkedNode = toCheckNodes.front();
		checkedNode->checked = true;

		for (GridNode*& n : checkedNode->closeNodes)
		{
			if (!n->checked && !n->isWall)
				toCheckNodes.push_back(n);

			float lowestParentDistance = checkedNode->localScore + NodeDistance(checkedNode, n);

			if (lowestParentDistance < n->localScore)
			{
				n->parent = checkedNode;
				n->localScore = lowestParentDistance;

				n->globalScore = n->localScore + Hueristic(n, GridManager::Get()->nodeEnd);
			}
		}
	}

	duration<double> time_span = duration_cast<duration<double>>(high_resolution_clock::now() - startTime);
	std::cout << "Calculation took: " << std::fixed << time_span.count() << " seconds\n";
}