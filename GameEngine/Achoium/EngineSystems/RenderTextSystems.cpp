#include "acpch.h"
#include "RenderTextSystems.h"
#include "Render/OpenGL/OpenGLRenderer.h"
#include "../EngineComponents/TextComponent.h"
namespace ac {



void RenderTextSystem(World& world) {
    auto& renderer = world.GetResourse<OpenGLRenderer>();
    
    // 获取所有具有Transform和TextComponent组件的实体
    auto view = world.View<Transform, Text>();
    
    // 遍历并渲染每个文本组件
    view.ForEach([&renderer](Entity entity, Transform& transform, Text& textComp) {
        if (!textComp.visible || textComp.text.empty())
            return;
            
        
        // 创建一个新的Transform用于渲染
        Transform renderTransform = transform;
        
        // 应用字体大小作为缩放因子
        float scaleFactor = textComp.fontSize / 48.0f; // 假设基础字体大小为48
        renderTransform.scale.x = scaleFactor * transform.scale.x;
        renderTransform.scale.y = scaleFactor * transform.scale.y;
        renderTransform.scale.z = scaleFactor * transform.scale.z;
        
        // 调用渲染器提交文本
        renderer.SubmitText(textComp.text, renderTransform, textComp.color, textComp.pivot);
    });
}

} // namespace ac