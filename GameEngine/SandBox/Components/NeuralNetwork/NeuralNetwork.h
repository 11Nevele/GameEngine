#pragma once
#include <vector>
struct NeuralNetwork
{
	//layer		nodes in layer		Edge to Next Layer 
	std::vector<std::vector<std::vector<float>>> network;
	float friendlyness;
	int score = 0;

	static NeuralNetwork Create(int layerCnt, const std::vector<int>& nodeInLayer);
	static NeuralNetwork Spawn(const NeuralNetwork& other);

};

