#include "acpch.h"
#include "NeuralNetworkSystems.h"
#include "NeuralNetwork.h"

float NeuralNetworkSystems::Compute(const NeuralNetwork& net, const vector<float>& input)
{
	assert(net.network.size() > 0, "Neural Network is empty");
	vector<float> cur(net.network[0].size());
	if (input.size() > net.network[0].size())
		std::cout << "WARNING: input to network is " << input.size() <<
		" but the network can only take " << net.network[0].size() << "\n";
	for (int i = 0; i < cur.size(); ++i)
	{
		cur[i] = i < input.size() ? input[i] : net.friendlyness;
	}
	for (auto& i : net.network)
	{
		vector<float> next(i[0].size());
		for (int j = 0; j < i.size(); ++j)
		{
			for (int k = 0; k < i[j].size(); ++k)
				next[k] += i[j][k] * cur[j];
		}
		cur = next;
	}
	float sum = 0;
	for (float i : cur)
		sum += i;
	return sum;	
}

// Evaluate how well a network functions as an OR gate
float NeuralNetworkSystems::EvaluateAsOrGate(const NeuralNetwork& network)
{
    // Test cases for OR gate
    std::vector<std::pair<std::vector<float>, float>> testCases = {
        {{0, 0}, 0},
        {{0, 1}, 1},
        {{1, 0}, 1},
        {{1, 1}, 1}
    };

    float score = 0.0f;
    for (const auto& [input, expectedOutput] : testCases) {
        float actualOutput = Compute(network, input);
        actualOutput = std::clamp(actualOutput, 0.0f, 1.0f);
        //cout << actualOutput << " ";
        // Calculate error (closer to 0 is better)
        float error = std::abs(expectedOutput - actualOutput);

        // Add to score (1.0 - error gives higher score for better performance)
        score += 1.0f - error;
    }
    //cout << "\n";
    // Normalize score to [0, 1]
    return score / testCases.size();
}

void NeuralNetworkSystems::UpdateNeuralNetworkSystem(World& world)
{
    // Evaluate each network as an OR gate
    std::vector<networkScore> networksWithScores;
    world.View<NeuralNetwork>().ForEach([&networksWithScores](Entity id, NeuralNetwork& network)
        {
            networkScore tmp{EvaluateAsOrGate(network), id };
            networksWithScores.push_back(tmp);

        });

    // Sort by score (descending)
    std::sort(networksWithScores.begin(), networksWithScores.end(),
        [](const auto& a, const auto& b) { return a.score > b.score; });

    // Keep top 10%
    int keepCount = std::max(1, static_cast<int>(networksWithScores.size() * 0.5));
    for (int i = keepCount; i < networksWithScores.size(); ++i)
    {
        int id = (i - keepCount) % (keepCount);
        world.Get<NeuralNetwork>(networksWithScores[i].ID) = NeuralNetwork::Spawn(
            world.Get<NeuralNetwork>(networksWithScores[id].ID));
    }
    for (int i = 0; i < keepCount; ++i)
    {
        cout << networksWithScores[i].score << "\n";
    }
    cout << "Next Round\n";
}

