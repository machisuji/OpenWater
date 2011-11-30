uniform int time;

void main() 
{ 
    // Transforming The Vertex 
	gl_Vertex.y = gl_Vertex.y + sin(gl_Vertex.x) + time;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  
	gl_TexCoord[0]  = gl_MultiTexCoord0;
}