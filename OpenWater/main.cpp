#include <osg/Node>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/Stencil>
#include <osg/ColorMask>
#include <osg/Depth>
#include <osg/ClipNode>
#include <osg/AnimationPath>
#include <osg/PositionAttitudeTransform>
#include <osg/ShaderAttribute>

#include <osgText/String>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>

#include <iostream>

osg::Drawable* createSurface(float width, float height, float z)
{
	int size = (int)(width * height);
    osg::Geometry* geom = new osg::Geometry;
    osg::Vec3Array* coords = new osg::Vec3Array();
	osg::Vec2Array* tcoords = new osg::Vec2Array();
	osg::Vec3Array* normals = new osg::Vec3Array();

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			coords->push_back(osg::Vec3(	x,		y + 1,	0	));
			coords->push_back(osg::Vec3(	x,		y,		0	));
			coords->push_back(osg::Vec3(	x + 1,	y,		0	));

			coords->push_back(osg::Vec3(	x + 1,	y,		0	));
			coords->push_back(osg::Vec3(	x + 1,	y + 1,	0	));
			coords->push_back(osg::Vec3(	x,		y + 1,	0	));

			normals->push_back(osg::Vec3(0, 1, 0));
			normals->push_back(osg::Vec3(0, 1, 0));
			normals->push_back(osg::Vec3(0, 1, 0));

			normals->push_back(osg::Vec3(0, 1, 0));
			normals->push_back(osg::Vec3(0, 1, 0));
			normals->push_back(osg::Vec3(0, 1, 0));
		}
	}

	geom->setVertexArray(coords);

    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    
    osg::Vec4Array* colours = new osg::Vec4Array(1);
    (*colours)[0].set(1.0f, 0.5f, 0.3, 0.0f);

    geom->setColorArray(colours);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);

	int instances = width * 2 * (height - 1) - 2;
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, width * height * 6));

    return geom;
}

osg::Node* createCoordinateSystem()
{
    osg::Geometry* geom = new osg::Geometry;
    osg::Vec3Array* coords = new osg::Vec3Array();
	osg::Vec2Array* tcoords = new osg::Vec2Array();
	osg::Vec3Array* normals = new osg::Vec3Array();

	coords->push_back(osg::Vec3(0, 0, 0));
	coords->push_back(osg::Vec3(10, 0, 0));

	coords->push_back(osg::Vec3(0, 0, 0));
	coords->push_back(osg::Vec3(0, 10, 0));

	coords->push_back(osg::Vec3(0, 0, 0));
	coords->push_back(osg::Vec3(0, 0, 10));

	geom->setVertexArray(coords);
    
    osg::Vec4Array* colours = new osg::Vec4Array(6);
    (*colours)[0].set(1.0f, 0.0f, 0.0, 0.0f); // x red
	(*colours)[1].set(1.0f, 0.0f, 0.0, 0.0f);

	(*colours)[2].set(0.0f, 1.0f, 0.0, 0.0f); // y green
	(*colours)[3].set(0.0f, 1.0f, 0.0, 0.0f);

	(*colours)[4].set(0.0f, 0.0f, 1.0, 0.0f); // z blue
	(*colours)[5].set(0.0f, 0.0f, 1.0, 0.0f);

    geom->setColorArray(colours);
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 6));

	osg::Geode* geode = new osg::Geode;
    geode->addDrawable(geom);

    return geode;
}

void addPixelShader(osg::Node* node, const std::string &vertFile, const std::string &fragFile)
{
	if (vertFile.empty() && fragFile.empty()) return;

	osg::Program* program = new osg::Program;
	osg::StateSet* state = node->getOrCreateStateSet();

	if (!vertFile.empty())
	{
		osg::Shader* shader = osgDB::readShaderFile(osg::Shader::VERTEX, vertFile);
		program->addShader(shader);
	}
	
	if (!fragFile.empty())
	{
		osg::Shader* shader = osgDB::readShaderFile(osg::Shader::FRAGMENT, fragFile);
		program->addShader(shader);
	}

	osg::Uniform* timeU = new osg::Uniform("time", osg::FrameStamp().getFrameNumber());
	
	state->addUniform(timeU, osg::FrameStamp().getFrameNumber());
	state->setAttributeAndModes(program, osg::StateAttribute::ON);
}

osg::Geode *createCube() {
	// vertex array
	osg::Vec3Array *vertexArray = new osg::Vec3Array();

	// bottom front left
	vertexArray->push_back(osg::Vec3(-1, -1,  0));
	vertexArray->push_back(osg::Vec3(-1, -1,  0));
	vertexArray->push_back(osg::Vec3(-1, -1,  0));
	// bottom front right
	vertexArray->push_back(osg::Vec3(+1, -1,  0));
	vertexArray->push_back(osg::Vec3(+1, -1,  0));
	vertexArray->push_back(osg::Vec3(+1, -1,  0));
	// bottom back right
	vertexArray->push_back(osg::Vec3(+1, +1,  0));
	vertexArray->push_back(osg::Vec3(+1, +1,  0));
	vertexArray->push_back(osg::Vec3(+1, +1,  0));
	// bottom back left
	vertexArray->push_back(osg::Vec3(-1, +1,  0));
	vertexArray->push_back(osg::Vec3(-1, +1,  0));
	vertexArray->push_back(osg::Vec3(-1, +1,  0));

	// top front left
	vertexArray->push_back(osg::Vec3(-1, -1,  2));
	vertexArray->push_back(osg::Vec3(-1, -1,  2));
	vertexArray->push_back(osg::Vec3(-1, -1,  2));
	// top front right
	vertexArray->push_back(osg::Vec3(+1, -1,  2));
	vertexArray->push_back(osg::Vec3(+1, -1,  2));
	vertexArray->push_back(osg::Vec3(+1, -1,  2));
	// top back right
	vertexArray->push_back(osg::Vec3(+1, +1,  2));
	vertexArray->push_back(osg::Vec3(+1, +1,  2));
	vertexArray->push_back(osg::Vec3(+1, +1,  2));
	// top back left
	vertexArray->push_back(osg::Vec3(-1, +1,  2));
	vertexArray->push_back(osg::Vec3(-1, +1,  2));
	vertexArray->push_back(osg::Vec3(-1, +1,  2));


	// face array
	osg::DrawElementsUInt *faceArray = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);

	// bottom
	faceArray->push_back(0); // face 1
	faceArray->push_back(9);
	faceArray->push_back(3);
	faceArray->push_back(9); // face 2
	faceArray->push_back(6);
	faceArray->push_back(3);
	// top
	faceArray->push_back(21);  //face 3
	faceArray->push_back(12);
	faceArray->push_back(18);
	faceArray->push_back(12);  //face 4
	faceArray->push_back(15);
	faceArray->push_back(18);
	// left
	faceArray->push_back(22);  //face 5
	faceArray->push_back(10);
	faceArray->push_back(13);
	faceArray->push_back(10);  //face 6
	faceArray->push_back(1);
	faceArray->push_back(13);
	// right
	faceArray->push_back(16);  //face 7
	faceArray->push_back(4);
	faceArray->push_back(19);
	faceArray->push_back(4);  //face 8
	faceArray->push_back(7);
	faceArray->push_back(19);
	// front
	faceArray->push_back(14);  //face 9
	faceArray->push_back(2);
	faceArray->push_back(17);
	faceArray->push_back(2);   //face 10
	faceArray->push_back(5);
	faceArray->push_back(17);
	// back
	faceArray->push_back(20);  //face 11
	faceArray->push_back(8);
	faceArray->push_back(23);
	faceArray->push_back(8);   //face 12
	faceArray->push_back(11);
	faceArray->push_back(23);

	// normal array
	osg::Vec3Array *normalArray = new osg::Vec3Array();
	normalArray->push_back(osg::Vec3(+1, 0, 0));
	normalArray->push_back(osg::Vec3(-1, 0, 0));
	normalArray->push_back(osg::Vec3(0, +1, 0));
	normalArray->push_back(osg::Vec3(0, -1, 0));
	normalArray->push_back(osg::Vec3(0, 0, +1));
	normalArray->push_back(osg::Vec3(0, 0, -1));

	// normal index
	osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 24, 4> *normalIndexArray;
	normalIndexArray = new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 24, 4>();

	// bottom front left					
	normalIndexArray->push_back(5);
	normalIndexArray->push_back(3);
	normalIndexArray->push_back(0);
	// bottom front right
	normalIndexArray->push_back(5);
	normalIndexArray->push_back(2);
	normalIndexArray->push_back(0);
	// bottom back right
	normalIndexArray->push_back(5);
	normalIndexArray->push_back(2);
	normalIndexArray->push_back(1);
	// bottom back left
	normalIndexArray->push_back(5);
	normalIndexArray->push_back(3);
	normalIndexArray->push_back(1);

	// top front left					
	normalIndexArray->push_back(4);
	normalIndexArray->push_back(3);
	normalIndexArray->push_back(0);
	// top front right
	normalIndexArray->push_back(4);
	normalIndexArray->push_back(2);
	normalIndexArray->push_back(0);
	// top back right
	normalIndexArray->push_back(4);
	normalIndexArray->push_back(2);
	normalIndexArray->push_back(1);
	// top back left
	normalIndexArray->push_back(4);
	normalIndexArray->push_back(3);
	normalIndexArray->push_back(1);

	osg::Geometry *geometry = new osg::Geometry();
	geometry->setVertexArray(vertexArray);

	geometry->setNormalArray(normalArray);
	geometry->setNormalIndices(normalIndexArray);
	geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	geometry->addPrimitiveSet(faceArray);

	osg::Geode *cube = new osg::Geode();
	cube->addDrawable(geometry);

	return cube;
}

int main( int argc, char **argv )
{
    osg::ArgumentParser arguments(&argc,argv);

    // construct the viewer.
    osgViewer::Viewer viewer;
	osg::Group* scene = new osg::Group;

	osg::PositionAttitudeTransform* waterPos = new osg::PositionAttitudeTransform();
	osg::Geode* waterSurface = new osg::Geode;
    waterSurface->addDrawable(createSurface(32, 20, 0));
	addPixelShader(waterSurface, "", ""); // waves.vert, red.frag
	waterPos->addChild(waterSurface);
	//waterPos->setPosition(osg::Vec3(0, 0, 0));
	//waterPos->setAttitude(osg::Quat(45, osg::Vec3(1, 0, 0)));

	osg::Node* homeModel = osgDB::readNodeFile("home.dw");
	osg::PositionAttitudeTransform* homePos = new osg::PositionAttitudeTransform();
	homePos->addChild(homeModel);
	homePos->setPosition(osg::Vec3(10, -4, -3));
	homePos->setAttitude(osg::Quat(70, osg::Vec3(0, 0, 1)));
	homePos->setScale(osg::Vec3d(0.2, 0.2, 0.2));

	osg::PositionAttitudeTransform* cubePos = new osg::PositionAttitudeTransform();
	cubePos->addChild(createCube());
	cubePos->setPosition(osg::Vec3(5, 10, -5));

	scene->addChild(homePos);
	scene->addChild(waterPos);
	scene->addChild(cubePos);
	scene->addChild(createCoordinateSystem());

    // set the scene to render
    viewer.setSceneData(scene);

    return viewer.run();
}
