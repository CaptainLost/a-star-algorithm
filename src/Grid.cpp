#include "Grid.h"
#include "SFML/Graphics.hpp"
#include "AStar.h"

const float gridPaddingX = 1.f, gridPaddingY = 1.f;
const sf::Vector2f nodeSize{ 10, 10 };

const sf::Color gridColor(128, 128, 128);
const sf::Color notCheckedGridColor(64, 64, 64);
const sf::Color wallColor(186, 41, 46);
const sf::Color startColor(0, 128, 0);
const sf::Color endColor(0, 0, 128);
const sf::Color pathColor(255, 255, 0);

GridManager::GridManager()
{
	nodes = new GridNode[gridSizeX * gridSizeY];

	nodeStart = &nodes[0];
	nodeEnd = &nodes[gridSizeX * gridSizeY - 1];

	// Tworzymy polaczenia z sasiadami
	for (int x = 0; x < gridSizeX; x++)
	{
		for (int y = 0; y < gridSizeY; y++)
		{
			GridNode* currentNode = &nodes[y * gridSizeX + x];

			currentNode->nodePosition.x = x;
			currentNode->nodePosition.x = y;

			if (y > 0)				currentNode->closeNodes.push_back(&nodes[(y - 1) * gridSizeX + (x + 0)]);
			if (y < gridSizeY - 1)	currentNode->closeNodes.push_back(&nodes[(y + 1) * gridSizeX + (x + 0)]);

			if (x > 0)				currentNode->closeNodes.push_back(&nodes[(y + 0) * gridSizeX + (x - 1)]);
			if (x < gridSizeX - 1)	currentNode->closeNodes.push_back(&nodes[(y + 0) * gridSizeX + (x + 1)]);
		}
	}
}

GridManager::~GridManager()
{
	delete[] nodes;
}

void GridManager::ResetWalls()
{
	for (int i = 0; i < Get()->gridSizeX; i++)
	{
		for (int j = 0; j < Get()->gridSizeY; j++)
		{
			GridNode* node = GridManager::GetNode(i, j);

			node->isWall = false;
		}
	}

	AStar::FindPath();
}

GridNode* GridManager::GetNode(const int& x, const int& y)
{
	int currentPos = y * GridManager::Get()->gridSizeX + x;
	return &GridManager::Get()->nodes[currentPos];
}

void GridManager::UpdateImpl(sf::RenderWindow& window)
{
	const sf::Vector2f mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	static sf::RectangleShape shape;

	for (int i = 0; i < gridSizeX; i++)
	{
		for (int j = 0; j < gridSizeY; j++)
		{
			GridNode* currentNode = GetNode(i, j);

			shape.setSize(nodeSize);
			shape.setPosition(
				nodeSize.x * (i - gridSizeX * 0.5f),
				nodeSize.y * (j - gridSizeY * 0.5f));

			currentNode->nodeCenterPos = sf::Vector2f(shape.getPosition().x + nodeSize.x * 0.5f, shape.getPosition().y + nodeSize.y * 0.5f);

			// Lepszym pomyslem obslugi inputu byl by osobny system, ale aby nie komplikowac jest tutaj
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (mouseWorldPos.x > shape.getPosition().x && mouseWorldPos.x < shape.getPosition().x + nodeSize.x &&
					mouseWorldPos.y > shape.getPosition().y && mouseWorldPos.y < shape.getPosition().y + nodeSize.y)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						if (!currentNode->switchedStatus)
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
							{
								if (currentNode != nodeEnd)
								{
									nodeStart = currentNode;
									currentNode->isWall = false;
								}
							}
							else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
							{
								if (currentNode != nodeStart)
								{
									nodeEnd = currentNode;
									currentNode->isWall = false;
								}
							}
							else
							{
								if (currentNode != nodeStart && currentNode != nodeEnd)
									currentNode->isWall = !currentNode->isWall;
							}

							currentNode->switchedStatus = true;
							AStar::FindPath();
						}
					}
				}
			}
			else currentNode->switchedStatus = false;

			shape.setFillColor(currentNode->isWall ? wallColor : currentNode->checked ? gridColor : notCheckedGridColor);

			if (nodeStart == currentNode)
			{
				shape.setFillColor(startColor);
			}
			else if (nodeEnd == currentNode)
			{
				shape.setFillColor(endColor);
			}

			// Obrys
			shape.setOutlineThickness(1.f);
			shape.setOutlineColor(sf::Color(32, 32, 32));

			window.draw(shape);

			//for (auto n : currentNode->closeNodes)
			//{
			//	sf::Vertex line[] =
			//	{
			//		sf::Vertex(currentNode->nodeCenterPos),
			//		sf::Vertex(n->nodeCenterPos)
			//	};

			//	window.draw(line, 2, sf::Lines);
			//}
		}
	}

	std::vector<sf::Vertex> lineVerts;
	int vertToDraw = 0;
	for (GridNode* p = nodeEnd; p->parent; p = p->parent)
	{
		lineVerts.push_back(sf::Vertex(p->nodeCenterPos, pathColor));
		lineVerts.push_back(sf::Vertex(p->parent->nodeCenterPos, pathColor));

		vertToDraw += 2;
	}

	if (vertToDraw > 0)
	{
		window.draw(lineVerts.data(), vertToDraw, sf::Lines);
	}
}