#pragma once
#include "NeuralNetwork.h"
#include "Achoium.h"
using namespace ac;
class NeuralNetworkSystems
{
private:
	class networkScore {
	public:
		float score;
		uint32_t ID;
		networkScore& operator = (const networkScore& other)
		{
			this->score = other.score;
			this->ID = other.ID;
			return *this;
		}
	};
	static float Compute(const NeuralNetwork& net, const vector<float>& input);
	static float EvaluateAsOrGate(const NeuralNetwork& network);
public:
	static void UpdateNeuralNetworkSystem(World& world);
};

