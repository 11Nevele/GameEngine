#pragma once
#include "acpch.h"

namespace ac
{
    // 前向声明
    class World;
    
    class AudioSystem
    {
    public:
        // 更新所有音频组件
        static void UpdateAudio(World& world);
    };
}