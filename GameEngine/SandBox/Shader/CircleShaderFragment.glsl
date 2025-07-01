#version 330 core


// Circle properties
uniform vec4 u_Color;     // Circle color (RGBA)
uniform vec2 u_Center;
uniform float u_Radius; // Circle radius
uniform float u_Thickness; // Circle thickness
uniform float u_rotation; // Circle rotation angle in radians

void main()
{
    // 减去0.5是为了让圆心在画布中央
    vec2 st=(gl_FragCoord.xy - u_Center);

    // 计算任何一个像素点距离原点坐标的距离
    float r = length(st);
    float s;
    if(r > u_Radius) {
        s = 0;
    }
    else if(r < (u_Radius - u_Thickness)) {
        s = 0;
    }
    else
        s= 1;
    vec2 rotation = vec2(cos(u_rotation), sin(u_rotation));
    vec2 v_norm = normalize(st); // Normalize input vector
    float dot_product = dot(v_norm, rotation);

    if(dot_product > 0.9999)
     s = 1;

    //s = 1;
    // gl_FragColor的取值可能为vec4(0., 0., 0., 1.)或vec4(1.,1.,1.,1.)
    gl_FragColor=vec4(u_Color.rgb, s);
}