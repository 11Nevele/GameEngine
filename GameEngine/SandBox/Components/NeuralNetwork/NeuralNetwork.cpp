#include "acpch.h"
#include "NeuralNetwork.h"
#include <random>
NeuralNetwork NeuralNetwork::Create(int layerCnt, const std::vector<int>& nodeInLayer)
{
    NeuralNetwork network{
        std::vector<std::vector<std::vector<float>>>(layerCnt),
        (double)(rand() % 100) / 100.0
    };
    for (int i = 0; i < layerCnt; ++i)
    {
        network.network[i] = std::vector<std::vector<float>>
            (nodeInLayer[i],
            std::vector<float>(i == layerCnt - 1 ? 1 : nodeInLayer[i + 1]));
        for(auto &j :network.network[i])
            for (auto& k : j)
            {
                k = (double)(rand() % 200 - 100) / 100.0;
            }
    }
    return network;
}

NeuralNetwork NeuralNetwork::Spawn(const NeuralNetwork& other)
{
    NeuralNetwork network = other;
	// Randomly mutate some weights
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 eng(rd());  // Seed the generator
    std::uniform_real_distribution<> distr( - 0.1, 0.1); // Define the range
    for (auto& layer : network.network)
    {
        for (auto& node : layer)
        {
            for (auto& weight : node)
            {
                if (distr(eng) + 0.1 < 0.2) // 5% chance to mutate each weight
                {
                    weight += distr(eng);
                }
            }
        }
    }
	return network;
}
