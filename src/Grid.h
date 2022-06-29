#pragma once
#include <vector>

#include "SFML/Graphics.hpp"

struct GridNode
{
	GridNode() {};

	sf::Vector2i nodePosition;
	sf::Vector2f nodeCenterPos;

	bool isWall = false;
	bool checked = false;
	bool switchedStatus = false;

	float localScore;
	float globalScore;

	std::vector<GridNode*> closeNodes;
	GridNode* parent = nullptr;
};

class GridManager
{
public:
	static GridManager* Get() { static GridManager g; return &g; };

	static void Update(sf::RenderWindow& window) { Get()->UpdateImpl(window); };
	static void ResetWalls();

	static GridNode* GetNode(const int& x, const int& y);

	GridManager();
	~GridManager();

	int gridSizeX = 20, gridSizeY = 20;

	GridNode* nodes;

	GridNode* nodeStart;
	GridNode* nodeEnd;
private:
	void UpdateImpl(sf::RenderWindow& window);
};