#include "acpch.h"
#include "Interface.h"
#include "../NeuralNetwork/NeuralNetwork.h"
#include <fstream>

void Interface::ControlEditMode(ac::World& world)
{
	InputManager& input = world.GetResourse<InputManager>();
    if(input.IsKeyDown(AC_KEY_C))
		SaveConfig(world);
    if (input.IsKeyDown(AC_KEY_V))
    {
		LoadConfig(world);
    }
}

void Interface::AddNeuralNetwork(World& world)
{

}

void Interface::EraseNeuralNetwork(World& world)
{

}

void Interface::AddWall(World& world)
{

}

void Interface::EraseWall(World& world)
{

}

void Interface::SaveConfig(World& world)
{
    // 替换当前的文件创建代码
    std::string configDir = CURPATH + "\\Configs";
    // 确保目录存在
    std::filesystem::create_directories(configDir);
    std::string path = configDir + "\\" + std::to_string(time(0)) + ".cfg";
    std::ofstream ofs(path);
    if (!ofs.is_open()) {
        std::cerr << "File to create config file: " << path << std::endl;
    }
    else {
        std::cout << "Config file created: " << path << std::endl;
    }
    world.View<NeuralNetwork, TilemapElement>().ForEach([&ofs](Entity e, NeuralNetwork& nn, TilemapElement& te)
    {
        // Write friendlyness and score
        ofs << nn.friendlyness << " " << nn.score << "\n";
        // Write network structure
        ofs << nn.network.size() << "\n";
        for (const auto& layer : nn.network) {
            ofs << layer.size() << "\n";
            for (const auto& node : layer) {
                ofs << node.size();
                for (float edge : node) {
                    ofs << " " << edge;
                }
                ofs << "\n";
            }
        }
    });
    ofs.close();
}

void Interface::LoadConfig(World& world)
{
    // 使用 Windows API 打开文件对话框
    char filename[MAX_PATH] = {0};
    
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL; // 无所有者窗口
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = "配置文件\0*.cfg\0所有文件\0*.*\0";
    ofn.nFilterIndex = 1;
    
    std::string configDir = CURPATH + "\\Configs";
    std::filesystem::create_directories(configDir);
    ofn.lpstrInitialDir = configDir.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    if (GetOpenFileNameA(&ofn) == TRUE)
    {
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            std::cerr << "无法打开配置文件: " << filename << std::endl;
            return;
        }
        
        std::cout << "从以下文件加载配置: " << filename << std::endl;
        
        // 首先，移除所有现有的神经网络及其实体
        std::vector<Entity> entitiesToDelete;
        world.View<NeuralNetwork, TilemapElement>().ForEach([&entitiesToDelete](Entity e, NeuralNetwork& nn, TilemapElement& te) {
            entitiesToDelete.push_back(e);
        });
        
        for (Entity e : entitiesToDelete) {
            world.DeleteEntity(e);
        }
        
        // 读取并创建新的神经网络
        while (!ifs.eof()) {
            float friendlyness;
            int score;
            
            // 尝试读取友好度和分数
            if (!(ifs >> friendlyness >> score)) {
                break;  // 文件结束或格式无效
            }
            
            int layerCount;
            if (!(ifs >> layerCount)) {
                break;
            }
            
            // 创建神经网络结构
            std::vector<std::vector<std::vector<float>>> network;
            network.resize(layerCount);
            
            for (int i = 0; i < layerCount; i++) {
                int nodeCount;
                if (!(ifs >> nodeCount)) {
                    break;
                }
                
                network[i].resize(nodeCount);
                
                for (int j = 0; j < nodeCount; j++) {
                    int edgeCount;
                    if (!(ifs >> edgeCount)) {
                        break;
                    }
                    
                    network[i][j].resize(edgeCount);
                    
                    for (int k = 0; k < edgeCount; k++) {
                        if (!(ifs >> network[i][j][k])) {
                            break;
                        }
                    }
                }
            }
            
            // 创建一个新实体并添加神经网络
            Entity entity = world.CreateEntity();
            NeuralNetwork nn;
            nn.network = network;
            nn.friendlyness = friendlyness;
            nn.score = score;
            world.Add<NeuralNetwork>(entity, std::move(nn));
            
            // 创建一个默认的TilemapElement
            TilemapElement te;
            world.Add<TilemapElement>(entity, std::move(te));
        }
        
        ifs.close();
    }
}
