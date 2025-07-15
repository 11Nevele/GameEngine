#pragma once
#include "Achoium.h"
using namespace ac;
class Interface
{
public:
	static void ControlEditMode(ac::World& world);
	static void AddNeuralNetwork(World& world);
	static void EraseNeuralNetwork(World& world);
	static void AddWall(World& world);
	static void EraseWall(World& world);
private:
	static void SaveConfig(World& world);
	static void LoadConfig(World& world);
};