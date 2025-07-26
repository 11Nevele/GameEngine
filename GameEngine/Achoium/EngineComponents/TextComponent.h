#pragma once
#include "acpch.h"
#include "Achoium.h"
#include <glm/glm.hpp>
#include <string>

namespace ac {

// 文本组件，包含字符串、字体大小和轴心点
struct Text {
    std::string text;      // 要渲染的文本
    float fontSize;        // 字体大小
    glm::vec2 pivot;       // 轴心点 (x, y 范围从0到1，例如：(0,0)为左上角，(1,1)为右下角，(0.5,0.5)为中心)
    glm::vec3 color;       // 文本颜色
    bool visible;          // 是否可见
    
    // 默认构造函数
    Text()
        : text(""), fontSize(24.0f), pivot(0.0f, 0.0f), color(1.0f, 1.0f, 1.0f), visible(true)
    {}
    
    // 带参数的构造函数
    Text(const std::string& text, float fontSize = 24.0f, const glm::vec2& pivot = glm::vec2(0.0f, 0.0f),
                  const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), bool visible = true)
        : text(text), fontSize(fontSize), pivot(pivot), color(color), visible(visible)
    {}
    
    // 静态创建函数
    static Text Create(const std::string& text, float fontSize = 24.0f,
                                const glm::vec2& pivot = glm::vec2(0.0f, 0.0f),
                                const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f))
    {
        return Text(text, fontSize, pivot, color);
    }

    // 常用预设轴心点
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

// 常用预设轴心点的定义
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