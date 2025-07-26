#pragma once
#include "acpch.h"
#include "Achoium.h"
#include <glm/glm.hpp>
#include <string>

namespace ac {

// �ı�����������ַ����������С�����ĵ�
struct Text {
    std::string text;      // Ҫ��Ⱦ���ı�
    float fontSize;        // �����С
    glm::vec2 pivot;       // ���ĵ� (x, y ��Χ��0��1�����磺(0,0)Ϊ���Ͻǣ�(1,1)Ϊ���½ǣ�(0.5,0.5)Ϊ����)
    glm::vec3 color;       // �ı���ɫ
    bool visible;          // �Ƿ�ɼ�
    
    // Ĭ�Ϲ��캯��
    Text()
        : text(""), fontSize(24.0f), pivot(0.0f, 0.0f), color(1.0f, 1.0f, 1.0f), visible(true)
    {}
    
    // �������Ĺ��캯��
    Text(const std::string& text, float fontSize = 24.0f, const glm::vec2& pivot = glm::vec2(0.0f, 0.0f),
                  const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), bool visible = true)
        : text(text), fontSize(fontSize), pivot(pivot), color(color), visible(visible)
    {}
    
    // ��̬��������
    static Text Create(const std::string& text, float fontSize = 24.0f,
                                const glm::vec2& pivot = glm::vec2(0.0f, 0.0f),
                                const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f))
    {
        return Text(text, fontSize, pivot, color);
    }

    // ����Ԥ�����ĵ�
    static const glm::vec2 PivotTopLeft;
    static const glm::vec2 PivotTopCenter;
    static const glm::vec2 PivotTopRight;
    static const glm::vec2 PivotMiddleLeft;
    static const glm::vec2 PivotMiddleCenter;
    static const glm::vec2 PivotMiddleRight;
    static const glm::vec2 PivotBottomLeft;
    static const glm::vec2 PivotBottomCenter;
    static const glm::vec2 PivotBottomRight;
};

// ����Ԥ�����ĵ�Ķ���
inline const glm::vec2 Text::PivotTopLeft = glm::vec2(0.0f, 0.0f);
inline const glm::vec2 Text::PivotTopCenter = glm::vec2(0.5f, 0.0f);
inline const glm::vec2 Text::PivotTopRight = glm::vec2(1.0f, 0.0f);
inline const glm::vec2 Text::PivotMiddleLeft = glm::vec2(0.0f, 0.5f);
inline const glm::vec2 Text::PivotMiddleCenter = glm::vec2(0.5f, 0.5f);
inline const glm::vec2 Text::PivotMiddleRight = glm::vec2(1.0f, 0.5f);
inline const glm::vec2 Text::PivotBottomLeft = glm::vec2(0.0f, 1.0f);
inline const glm::vec2 Text::PivotBottomCenter = glm::vec2(0.5f, 1.0f);
inline const glm::vec2 Text::PivotBottomRight = glm::vec2(1.0f, 1.0f);

} // namespace ac