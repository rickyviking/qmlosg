// computed values
varying vec3 _normalDir;
varying vec3 _viewDir;
varying vec4 _baseColor;

void main()
{
    vec4 vertex = gl_Vertex;

    _baseColor = gl_Color; //colorHeight.rgb;

    // transform the vertex
    gl_Position = gl_ModelViewProjectionMatrix * vertex;

    // normal and lightDir computation
    _normalDir = gl_NormalMatrix * gl_Normal;
    //_normalDir = gl_NormalMatrix * normal;

    vec3 dir = -vec3(gl_ModelViewMatrix * vertex);
    _viewDir = dir;
}
