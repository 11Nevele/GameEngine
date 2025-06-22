#include "acpch.h"
#include "TimeSystem.h"

namespace ac
{
	void UpdateTimeSystem(World& world)
	{
		world.GetResourse<Time>().Update();
	}
}
