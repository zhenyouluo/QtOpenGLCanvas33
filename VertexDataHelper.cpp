
#include "AVector.h"
#include "ALine.h"
#include "ATriangle.h"
#include "ABox.h"
#include "APath.h"
#include "VertexDataHelper.h"

/**
 * Reza Adhitya Saputra
 * radhitya@uwaterloo.ca
 * February 2016
 */

VertexDataHelper::VertexDataHelper(QOpenGLShaderProgram* shaderProgram)
{
    this->_shaderProgram      = shaderProgram;
    this->_colorLocation      = _shaderProgram->attributeLocation("vertexColor");
    this->_vertexLocation     = _shaderProgram->attributeLocation("vert");
    this->_use_color_location = _shaderProgram->uniformLocation("use_color");
    this->_texCoordLocation   = _shaderProgram->attributeLocation("uv");
}

VertexDataHelper::~VertexDataHelper()
{
}

void VertexDataHelper::BuildPathVertexData(APath aPath, QOpenGLBuffer* linesVbo, QOpenGLVertexArrayObject* linesVao, QVector3D vecCol)
{
	if (aPath.points.size() == 0) { return; }
	bool isInit = false;
	if (!linesVao->isCreated())
	{
		linesVao->create();
		linesVao->bind();
		isInit = true;
	}
	size_t path_length = aPath.points.size();
	QVector<VertexData> data;
	for (uint a = 0; a < path_length - 1; a++)
	{
		//if (a < _points.size() - 1) { lines.push_back(ALine(_points[a], _points[a + 1])); }
		//else { lines.push_back(ALine(_points[a], _points[0])); }
		data.append(VertexData(QVector3D(aPath.points[a].x, aPath.points[a].y, 0), QVector2D(), vecCol));
		data.append(VertexData(QVector3D(aPath.points[a + 1].x, aPath.points[a + 1].y, 0), QVector2D(), vecCol));
	}
	if (aPath.isClosed)
	{
		data.append(VertexData(QVector3D(aPath.points[path_length - 1].x, aPath.points[path_length - 1].y, 0), QVector2D(), vecCol));
		data.append(VertexData(QVector3D(aPath.points[0].x, aPath.points[0].y, 0), QVector2D(), vecCol));
	}
	BuildVboWithColor(data, linesVbo);
	if (isInit) { linesVao->release(); }
}

size_t VertexDataHelper::BuildPathsVertexData(std::vector<APath> paths, QOpenGLBuffer* linesVbo, QOpenGLVertexArrayObject* linesVao, std::vector<QVector3D> colors)
{
	if (paths.size() == 0) { return 0; }

	bool isInit = false;
	if (!linesVao->isCreated())
	{
		linesVao->create();
		linesVao->bind();
		isInit = true;
	}

	QVector<VertexData> data;
	for (size_t a = 0; a < paths.size(); a++)
	{
		// your code here
		APath aPath = paths[a];
		QVector3D vecCol = colors[a];

		size_t path_length = aPath.points.size();
		
		for (uint a = 0; a < path_length - 1; a++)
		{
			//if (a < _points.size() - 1) { lines.push_back(ALine(_points[a], _points[a + 1])); }
			//else { lines.push_back(ALine(_points[a], _points[0])); }
			data.append(VertexData(QVector3D(aPath.points[a].x, aPath.points[a].y, 0), QVector2D(), vecCol));
			data.append(VertexData(QVector3D(aPath.points[a + 1].x, aPath.points[a + 1].y, 0), QVector2D(), vecCol));
		}
		if (aPath.isClosed)
		{
			data.append(VertexData(QVector3D(aPath.points[path_length - 1].x, aPath.points[path_length - 1].y, 0), QVector2D(), vecCol));
			data.append(VertexData(QVector3D(aPath.points[0].x, aPath.points[0].y, 0), QVector2D(), vecCol));
		}
	}

	BuildVboWithColor(data, linesVbo);
	if (isInit) { linesVao->release(); }

	return data.size();
}

void VertexDataHelper::BuildLinesVertexData(std::vector<AVector> points, QOpenGLBuffer* linesVbo, QOpenGLVertexArrayObject* linesVao, QVector3D vecCol)
{
	if (points.size() == 0) { return; }

    bool isInit = false;
    if(!linesVao->isCreated())
    {
        linesVao->create();
        linesVao->bind();
        isInit = true;
    }

    QVector<VertexData> data;
    for(uint a = 0; a < points.size() - 1; a ++)
    {
        data.append(VertexData(QVector3D(points[a].x, points[a].y,  0), QVector2D(), vecCol));
        data.append(VertexData(QVector3D(points[a+1].x, points[a+1].y,  0), QVector2D(), vecCol));
    }

    BuildVboWithColor(data, linesVbo);

    if(isInit) { linesVao->release(); }
}

void VertexDataHelper::BuildLinesVertexData(std::vector<ALine> lines, QOpenGLBuffer* linesVbo, QOpenGLVertexArrayObject* linesVao, QVector3D vecCol1, QVector3D vecCol2)
{
    bool isInit = false;
    if(!linesVao->isCreated())
    {
        linesVao->create();
        linesVao->bind();
        isInit = true;
    }

    QVector<VertexData> data;
    for(uint a = 0; a < lines.size(); a++)
    {
        data.append(VertexData(QVector3D(lines[a].XA, lines[a].YA,  0), QVector2D(), vecCol1));
        data.append(VertexData(QVector3D(lines[a].XB, lines[a].YB,  0), QVector2D(), vecCol2));
    }

    BuildVboWithColor(data, linesVbo);

    if(isInit) { linesVao->release(); }
}

void VertexDataHelper::BuildQuadsVertexData(std::vector<ABox> boxes, QOpenGLBuffer* vbo, QOpenGLVertexArrayObject* vao)
{
	if (vao->isCreated()) { vao->destroy(); }

	vao->create();
	vao->bind();

	QVector<VertexData> data;
	// this diagram is wrong
	/*
	(0, 0)           (witdh, 0)
	ptB ------------ ptD
	 |				  |
	 |				  |
	 |				  |
	 |				  |
	 |				  |
	ptA ------------ ptC
	(0, height)      (width, height)
	*/
	for (uint a = 0; a < boxes.size(); a++)
	{
		data.append(VertexData(QVector3D(boxes[a]._ptA.x, boxes[a]._ptA.y, 0), QVector2D(0, 1)));
		data.append(VertexData(QVector3D(boxes[a]._ptB.x, boxes[a]._ptB.y, 0), QVector2D(0, 0)));
		data.append(VertexData(QVector3D(boxes[a]._ptD.x, boxes[a]._ptD.y, 0), QVector2D(1, 0))); // flipped
		data.append(VertexData(QVector3D(boxes[a]._ptC.x, boxes[a]._ptC.y, 0), QVector2D(1, 1))); // flipped		
	}

	BuildVboWithTexture(data, vbo);

	vao->release();
}

void VertexDataHelper::BuildQuadsVertexData(std::vector<ABox> boxes, QOpenGLBuffer* vbo, QOpenGLVertexArrayObject* vao, QVector3D vecCol)
{
	if (vao->isCreated()) { vao->destroy(); }

	vao->create();
	vao->bind();

	QVector<VertexData> data;
	for (uint a = 0; a < boxes.size(); a++)
	{
		data.append(VertexData(QVector3D(boxes[a]._ptA.x, boxes[a]._ptA.y, 0), QVector2D(), vecCol));
		data.append(VertexData(QVector3D(boxes[a]._ptB.x, boxes[a]._ptB.y, 0), QVector2D(), vecCol));
		data.append(VertexData(QVector3D(boxes[a]._ptD.x, boxes[a]._ptD.y, 0), QVector2D(), vecCol)); // flipped
		data.append(VertexData(QVector3D(boxes[a]._ptC.x, boxes[a]._ptC.y, 0), QVector2D(), vecCol)); // flipped		
	}

	BuildVboWithColor(data, vbo);

	vao->release();
}


void VertexDataHelper::BuildTrianglesVertexData(std::vector<ATriangle> triangles, QOpenGLBuffer* vbo, QOpenGLVertexArrayObject* vao, QVector3D vecCol)
{
	if (vao->isCreated()) { vao->destroy(); }

	vao->create();
	vao->bind();

	QVector<VertexData> data;
	for (uint a = 0; a < triangles.size(); a++)
	{
		data.append(VertexData(QVector3D(triangles[a]._ptA.x, triangles[a]._ptA.y, 0), QVector2D(), vecCol));
		data.append(VertexData(QVector3D(triangles[a]._ptB.x, triangles[a]._ptB.y, 0), QVector2D(), vecCol));
		data.append(VertexData(QVector3D(triangles[a]._ptC.x, triangles[a]._ptC.y, 0), QVector2D(), vecCol));
	}

	BuildVboWithColor(data, vbo);
	
	vao->release();
}


void VertexDataHelper::BuildLinesVertexData(std::vector<ALine> lines, QOpenGLBuffer* linesVbo, QOpenGLVertexArrayObject* linesVao, QVector3D vecCol)
{
    bool isInit = false;
    if(!linesVao->isCreated())
    {
        linesVao->create();
        linesVao->bind();
        isInit = true;
    }

    QVector<VertexData> data;
    for(uint a = 0; a < lines.size(); a++)
    {
        data.append(VertexData(QVector3D(lines[a].XA, lines[a].YA,  0), QVector2D(), vecCol));
        data.append(VertexData(QVector3D(lines[a].XB, lines[a].YB,  0), QVector2D(), vecCol));
    }

    BuildVboWithColor(data, linesVbo);

    if(isInit) { linesVao->release(); }
}


void VertexDataHelper::BuildPointsVertexData(std::vector<AVector> points, QOpenGLBuffer* ptsVbo, QOpenGLVertexArrayObject* ptsVao, QVector3D vecCol)
{
    bool isInit = false;
    if(!ptsVao->isCreated())
    {
        ptsVao->create();
        ptsVao->bind();
        isInit = true;
    }

    QVector<VertexData> data;
    for(uint a = 0; a < points.size(); a++)
        { data.append(VertexData(QVector3D(points[a].x, points[a].y,  0), QVector2D(), vecCol)); }

    BuildVboWithColor(data, ptsVbo);

    if(isInit) { ptsVao->release(); }
}



void VertexDataHelper::BuildVboWithColor(QVector<VertexData> data, QOpenGLBuffer* vbo)
{
    if(!vbo->isCreated()) { vbo->create(); }
    vbo->bind();
    vbo->allocate(data.data(), data.size() * sizeof(VertexData));

    quintptr offset = 0;

    _shaderProgram->enableAttributeArray(_vertexLocation);
    _shaderProgram->setAttributeBuffer(_vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);
    offset += sizeof(QVector2D);

    _shaderProgram->enableAttributeArray(_colorLocation);
    _shaderProgram->setAttributeBuffer(_colorLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	vbo->release();
}

void VertexDataHelper::BuildVboWithTexture(QVector<VertexData>  data, QOpenGLBuffer* vbo)
{
	if (!vbo->isCreated()) { vbo->create(); }
	vbo->bind();
	vbo->allocate(data.data(), 4 * sizeof(VertexData));

	// Offset for position
	quintptr offset = 0;

	// vertex
	int vertexLocation = _shaderProgram->attributeLocation("vert");
	_shaderProgram->enableAttributeArray(vertexLocation);
	_shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	offset += sizeof(QVector3D);

	// uv
	int texcoordLocation = _shaderProgram->attributeLocation("uv");
	_shaderProgram->enableAttributeArray(texcoordLocation);
	_shaderProgram->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

	vbo->release();
}


