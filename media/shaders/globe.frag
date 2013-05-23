
// computed varyings
varying vec3 _normalDir;
varying vec3 _viewDir;
varying vec4 _baseColor;

//////////////////////////////////////////////////////////////////////////////////////
void main()
{
    // Normal (from vertex shader)
    vec3 normalDir = normalize(_normalDir);

    // Light dir (fixed)
    vec3 lightDir = normalize (vec3(0.0, 0.8, 1.0));
    //   vec3 lightDir = normalize (vec3(0.7, 0.5, 3.0));
    vec3 fillLightDir = normalize (vec3(-0.9, 0.5, 1.0));
    //   vec3 fillLightDir = normalize (vec3(-0.3, 0.2, -0.5));
    // View dir (from vert shader)
    vec3 viewDir = normalize(_viewDir);

    // ambient term
    float ambientFactor = 0.5;
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0) * ambientFactor;
    // diffuse term
    float diffuse = max(dot(lightDir, normalDir), 0.0);
    float diffuseFill = max(dot(fillLightDir, normalDir), 0.0);

    color.rgb = _baseColor.rgb * (ambientFactor + diffuse * 0.5 + diffuseFill * 0.5);

    // // specular term
    // if (diffuse > 0.0)
    // {
    // vec3 specLightDir = vec3(0.3, 0.6, 1.0);
    // vec3 halfDir = normalize(lightDir + viewDir);
    // vec3 specColor = vec3(0.9, 0.9, 0.9);
    // color.rgb += 0.3*specColor * pow(max(dot(halfDir, normalDir), 0.0), 100.0);
    // }

    color.a = _baseColor.a;

    gl_FragColor = color;
}
