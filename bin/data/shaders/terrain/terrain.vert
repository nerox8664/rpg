#version 120

precision highp float;

varying vec2 texture_coordinate;

void main()
{

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix *gl_Vertex;
	//gl_Position = ftransform();

    // Passing The Texture Coordinate Of Texture Unit 0 To The Fragment Shader
    texture_coordinate = vec2(gl_MultiTexCoord0.s, gl_MultiTexCoord0.t);


}
