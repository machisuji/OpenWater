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
			coords->push_back(osg::Vec3(	x,		0,	y + 1	));
			coords->push_back(osg::Vec3(	x,		0,	y		));
			coords->push_back(osg::Vec3(	x + 1,	0,	y		));

			coords->push_back(osg::Vec3(	x + 1,	0,	y		));
			coords->push_back(osg::Vec3(	x + 1,	0,	y + 1	));
			coords->push_back(osg::Vec3(	x,		0,	y + 1	));

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

int main( int argc, char **argv )
{
    osg::ArgumentParser arguments(&argc,argv);

    // construct the viewer.
    osgViewer::Viewer viewer;

	osg::Geode* geode = new osg::Geode;
    geode->addDrawable(createSurface(32, 20, 0));
	addPixelShader(geode, "waves.vert", ""); // waves.vert, red.frag

    // set the scene to render
    viewer.setSceneData(geode);

    return viewer.run();
}
