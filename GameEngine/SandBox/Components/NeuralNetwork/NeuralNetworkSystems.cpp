#include "acpch.h"
#include "NeuralNetworkSystems.h"
#include "NeuralNetwork.h"
#include "Achoium.h"

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

std::pair<float, float> NeuralNetworkSystems::EvaluatePrisonerDilemma(const NeuralNetwork& networkA, const NeuralNetwork& networkB)
{
    const int matrix[2][2]{ {-1, 5}, {-5, 1} };
    int scoreA = 0, scoreB = 0;
    int gameTime = rand() % 10 + 5;
    vector<float> aHistory, bHistory;
    aHistory.reserve(gameTime);
    bHistory.reserve(gameTime);
    for (int i = 0; i < gameTime; ++i)
    {
        int begin = max(i - 2, 0);

        float OutputA = Compute(networkA, std::vector<float>(bHistory.begin() + begin, bHistory.end()));
        float OutputB = Compute(networkB, std::vector<float>(aHistory.begin() + begin, aHistory.end()));
        OutputA = OutputA > 0.5f ? 1 : 0;
        OutputB = OutputB > 0.5f ? 1 : 0;
        scoreA += matrix[(int)OutputA][(int)OutputB];
        scoreB += matrix[(int)OutputB][(int)OutputA];
        aHistory.push_back(OutputA);
        bHistory.push_back(OutputB);
    }
    return { scoreA, scoreB };
}

void NeuralNetworkSystems::UpdateNeuralNetworkSystem(World& world)
{
    // Evaluate each network as an OR gate
    std::vector<networkScore> networksWithScores;
    world.View<NeuralNetwork>().ForEach([&networksWithScores](Entity id, NeuralNetwork& network)
        {
            networkScore tmp{0, id };
            networksWithScores.push_back(tmp);

        });
    for (int i = 0; i < networksWithScores.size(); ++i)
    {
        for (int j = i + 1; j < networksWithScores.size(); ++j)
        {
            auto res = EvaluatePrisonerDilemma(
                world.Get<NeuralNetwork>(networksWithScores[i].ID),
                world.Get<NeuralNetwork>(networksWithScores[j].ID));
            networksWithScores[i].score += res.first;
            networksWithScores[j].score += res.second;
        }
    }
    // Sort by score (descending)
    std::sort(networksWithScores.begin(), networksWithScores.end(),
        [](const auto& a, const auto& b) { return a.score > b.score; });

    // Keep top 10%
    int keepCount = std::max(1, static_cast<int>(networksWithScores.size() * 0.2));
    for (int i = keepCount; i < networksWithScores.size(); ++i)
    {
        int id = (i - keepCount) % (keepCount);
        world.Get<NeuralNetwork>(networksWithScores[i].ID) = NeuralNetwork::Spawn(
            world.Get<NeuralNetwork>(networksWithScores[id].ID));
    }
    for (int i = 0; i < keepCount; ++i)
    {
        cout << networksWithScores[networksWithScores.size() - 1 - i].score << "\n";
    }
    cout << "Next Round\n";
}

void NeuralNetworkSystems::UpdateNeuralNetworkSystemV2(World& world)
{
    world.View<NeuralNetwork,TilemapElement >().ForEach([&world](Entity entity, NeuralNetwork& nn, TilemapElement& tilemapElement)
        {
            Tilemap& tilemap = world.Get<Tilemap>(tilemapElement.tilemap);
            if (tilemapElement.x + 1 < tilemap.map.size())
            {
                Entity other = tilemap.map[tilemapElement.x + 1][tilemapElement.y];
                if (other != 0 && world.Has<NeuralNetwork>(other))
                {
                    NeuralNetwork& otherNN = world.Get<NeuralNetwork>(other);
                    auto res = EvaluatePrisonerDilemma(nn, otherNN);
                    nn.score += res.first;
                    otherNN.score += res.second;
                }
                
            }
            if (tilemapElement.y + 1 < tilemap.map[tilemapElement.x].size())
            {
                Entity other = tilemap.map[tilemapElement.x][tilemapElement.y + 1];
                if (other != 0 && world.Has<NeuralNetwork>(other))
                {
                    NeuralNetwork& otherNN = world.Get<NeuralNetwork>(other);
                    auto res = EvaluatePrisonerDilemma(nn, otherNN);
                    nn.score += res.first;
                    otherNN.score += res.second;
                }

            }
        });
}

void NeuralNetworkSystems::ReproduceNuralNetworkSystem(World& world)
{
    world.View<NeuralNetwork>().ForEach([&world](Entity entity, NeuralNetwork& nn)
        {
            if (nn.score <= 0)
            {
                world.Delete<NeuralNetwork>(entity);
            }
        });
    world.View<Tilemap>().ForEach([&world](Entity entity, Tilemap& map)
        {
            vector < pair<Entity, NeuralNetwork>> newNeuralNetworks;
            int dir[4][2]{ {1,0},{-1,0},{0, 1},{0,-1} };
            for (int x = 0; x < map.map.size(); ++x)
                for (int y = 0; y < map.map[x].size(); ++y)
                {
                    if (map.map[x][y] == 0)
                        continue;
                    Entity e = map.map[x][y];
                    if (world.Has<NeuralNetwork>(e))
                        continue;
                    uint32_t totScore = 0;
                    vector<uint32_t> scores(4);
                    vector<Entity> nns(4);
                    for (int i = 0; i < 4; ++i)
                    {
                        int nx = x + dir[i][0], ny = y + dir[i][1];
                        if (nx < 0 || nx >= map.map.size() || ny < 0 || ny >= map.map[nx].size())
                            continue;
                        Entity neibourgh = map.map[nx][ny];
                        if (neibourgh == 0 || !world.Has<NeuralNetwork>(neibourgh))
                            continue;
                        nns[i] = neibourgh;
                        scores[i] = world.Get<NeuralNetwork>(neibourgh).score;
                        totScore += scores[i];
                    }
                    if (totScore <= 0)
                        continue;
                    int choice = rand() % totScore;
                    int cur = 0;
                    for (int i = 0; i < 4; ++i)
                    {
                        cur += scores[i];
                        if (cur > choice)
                        {
                            newNeuralNetworks.emplace_back(e, NeuralNetwork::Spawn(world.Get<NeuralNetwork>(nns[i])));
                            break;
                        }
                    }
                }
            for (auto& i : newNeuralNetworks)
            {
                world.Add<NeuralNetwork>(i.first, std::move(i.second));
            }
        });
    
}

void NeuralNetworkSystems::ChangeNeuralNetworkColor(World& world)
{
    world.View<TilemapElement, Sprite>().ForEach([&world](Entity entity, TilemapElement& te, Sprite& sprite)
        {
            if (world.Has<NeuralNetwork>(entity))
            {

                NeuralNetwork& nn = world.Get<NeuralNetwork>(entity);
                sprite.color = { 1.0f - nn.friendlyness, nn.friendlyness,0 ,1 };
                //sprite.color = { 0,((float)nn.score) / 100.0 ,0,1};
            }
            else
            {
                sprite.color = { 0,0,0,1 };
            }
        });
}

