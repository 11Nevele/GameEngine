#include "acpch.h"
#include "RenderTextSystems.h"
#include "Render/OpenGL/OpenGLRenderer.h"
#include "../EngineComponents/TextComponent.h"
namespace ac {



void RenderTextSystem(World& world) {
    auto& renderer = world.GetResourse<OpenGLRenderer>();
    
    // ��ȡ���о���Transform��TextComponent�����ʵ��
    auto view = world.View<Transform, Text>();
    
    // ��������Ⱦÿ���ı����
    view.ForEach([&renderer](Entity entity, Transform& transform, Text& textComp) {
        if (!textComp.visible || textComp.text.empty())
            return;
            
        
        // ����һ���µ�Transform������Ⱦ
        Transform renderTransform = transform;
        
        // Ӧ�������С��Ϊ��������
        float scaleFactor = textComp.fontSize / 48.0f; // ������������СΪ48
        renderTransform.scale.x = scaleFactor * transform.scale.x;
        renderTransform.scale.y = scaleFactor * transform.scale.y;
        renderTransform.scale.z = scaleFactor * transform.scale.z;
        
        // ������Ⱦ���ύ�ı�
        renderer.SubmitText(textComp.text, renderTransform, textComp.color, textComp.pivot);
    });
}

} // namespace ac