#include "OSGTest.h"


//生成AABB
osg::ref_ptr<osg::Geode> createBoundingBox(osg::Vec3 center, float lenx, float leny, float lenz)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;

	osg::ref_ptr<osg::ShapeDrawable>  drawable = new osg::ShapeDrawable(new osg::Box(center, lenx, leny, lenz));
	drawable->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));//
	osg::ref_ptr<osg::StateSet> stateset = drawable->getOrCreateStateSet();
	osg::ref_ptr<osg::PolygonMode> polygon = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	stateset->setAttributeAndModes(polygon);
	//设置线宽
	osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(3.0);
	stateset->setAttribute(linewidth);
	geode->addDrawable(drawable);
	return geode;
}

//生成OBB
osg::ref_ptr<osg::Geode> createOBBBox(render_system::OBB obb, std::shared_ptr<RendertoTexture> renderTex)
{
	osg::ref_ptr<osg::Vec3Array> point;
	obb.getCorners(point);
	vector<osg::Vec3> obbPnt = point->asVector();
	osg::ref_ptr<osg::Geode> box = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertexes = new osg::Vec3Array;
	for (int i = 0; i < 3; i++)
		for (size_t j = 0; j < obbPnt.size(); j++)
			vertexes->push_back(obbPnt[j]);
	geometry->setVertexArray(vertexes);//给mode设置顶点
	//设置索引
	GLuint elements[] = {
		//front
		0,1,2,
		0,2,3,
		//back
		6,4,7,
		6,5,4,
		//left
		8,9,14,
		8,14,15,
		//right
		11,12,13,
		11,13,10,
		//top
		16,20,19,
		16,23,20,
		//bottom
		17,22,21,
		17,21,18
	};

	// House texture coordinates
	osg::ref_ptr<osg::Vec2Array> texcoords1 = new osg::Vec2Array(24);
	(*texcoords1)[0].set(0.0, 1.0);
	(*texcoords1)[1].set(0.0, 0.0);
	(*texcoords1)[2].set(1.0, 1.0);
	(*texcoords1)[3].set(1.0, 0.0);

	osg::ref_ptr<osg::Vec2Array> texcoords2 = new osg::Vec2Array(24);
	(*texcoords2)[4].set(0.0, 1.0);
	(*texcoords2)[5].set(0.0, 0.0);
	(*texcoords2)[6].set(1.0, 1.0);
	(*texcoords2)[7].set(1.0, 0.0);

	osg::ref_ptr<osg::Vec2Array> texcoords3 = new osg::Vec2Array(24);
	(*texcoords3)[8].set(0.0, 1.0);
	(*texcoords3)[9].set(0.0, 0.0);
	(*texcoords3)[10].set(1.0, 1.0);
	(*texcoords3)[11].set(1.0, 0.0);
	
	osg::ref_ptr<osg::Vec2Array> texcoords4 = new osg::Vec2Array(24);
	(*texcoords4)[12].set(0.0, 1.0);
	(*texcoords4)[13].set(0.0, 0.0);
	(*texcoords4)[14].set(1.0, 1.0);
	(*texcoords4)[15].set(1.0, 0.0);
	
	osg::ref_ptr<osg::Vec2Array> texcoords5 = new osg::Vec2Array(24);
	(*texcoords5)[16].set(0.0, 1.0);
	(*texcoords5)[17].set(0.0, 0.0);
	(*texcoords5)[18].set(1.0, 1.0);
	(*texcoords5)[19].set(1.0, 0.0);
	
	osg::ref_ptr<osg::Vec2Array> texcoords6 = new osg::Vec2Array(24);
	(*texcoords6)[20].set(0.0, 1.0);
	(*texcoords6)[21].set(0.0, 0.0);
	(*texcoords6)[22].set(1.0, 1.0);
	(*texcoords6)[23].set(1.0, 0.0);

	//设置纹理环境，模式为BLEND
	osg::ref_ptr<osg::TexEnv> texenv = new osg::TexEnv;
	texenv->setMode(osg::TexEnv::Mode::BLEND);
	texenv->setColor(osg::Vec4(1,1, 1, 0.0));
	osg::ref_ptr<osg::StateSet> state = new osg::StateSet;
	state->setTextureAttribute(2, texenv.get());
	geometry->setStateSet(state.get());

	//设置材质
	osg::ref_ptr<osg::Material>material = new osg::Material;
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 0));
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 0));
	material->setShininess(osg::Material::FRONT_AND_BACK, 20);
	material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 0));
	geometry->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON);

	//设置法向量（六个需要六个法线）
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	for (int i = 0; i < 4; i++)
		normals->push_back(obb._zAxis);
	for (int i = 0; i < 4; i++)
		normals->push_back(-obb._zAxis);

	normals->push_back(-obb._xAxis);
	normals->push_back(-obb._xAxis);
	for (int i = 0; i < 4; i++)
		normals->push_back(obb._xAxis);
	normals->push_back(-obb._xAxis);
	normals->push_back(-obb._xAxis);

	normals->push_back(obb._yAxis);
	normals->push_back(-obb._yAxis);
	normals->push_back(-obb._yAxis);
	normals->push_back(obb._yAxis);
	normals->push_back(obb._yAxis);
	normals->push_back(-obb._yAxis);
	normals->push_back(-obb._yAxis);
	normals->push_back(obb._yAxis);

	//设置法线
	geometry->setNormalArray(normals, osg::Array::BIND_PER_VERTEX);

	/*
	//线框模式
	osg::ref_ptr<osg::StateSet> stateset = geometry->getOrCreateStateSet();
	osg::ref_ptr<osg::PolygonMode> polygon = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL);
	stateset->setAttributeAndModes(polygon);
	osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth(3);
	stateset->setAttribute(linewidth);
	*/

	int pntNum = sizeof(elements) / sizeof(GLuint);
	//DrawElementsUInt接受一个对顶点数组的索引数组 以TRIANGLES三角面的方式绘制图元
	osg::ref_ptr<osg::DrawElementsUInt> elementsArray = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, pntNum, elements);//PrimitiveSet用来添加和删除图元
	//所有图元类都继承于PrimitiveSet，不同的图元类可以以不同的方式解释顶点数组
	geometry->addPrimitiveSet(elementsArray);

	geometry->setTexCoordArray(0, texcoords1.get());
	geometry->setTexCoordArray(1, texcoords2.get());
	geometry->setTexCoordArray(2, texcoords3.get());
	//geometry->setTexCoordArray(3, texcoords4.get());
	//geometry->setTexCoordArray(4, texcoords5.get());
	//geometry->setTexCoordArray(5, texcoords6.get());
	geometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, new osg::Texture2D(osgDB::readImageFile("Images/primitives.gif")));
	geometry->getOrCreateStateSet()->setTextureAttributeAndModes(1, new osg::Texture2D(osgDB::readImageFile("Images/primitives.gif")));
	geometry->getOrCreateStateSet()->setTextureAttributeAndModes(2, new osg::Texture2D(osgDB::readImageFile("Images/primitives.gif")));
	//geometry->getOrCreateStateSet()->setTextureAttributeAndModes(3, new osg::Texture2D(osgDB::readImageFile("Images/primitives.gif")));
	/*geometry->getOrCreateStateSet()->setTextureAttributeAndModes(4, new osg::Texture2D(osgDB::readImageFile("Images/primitives.gif")));
	geometry->getOrCreateStateSet()->setTextureAttributeAndModes(5, new osg::Texture2D(osgDB::readImageFile("Images/primitives.gif")));
*/
	//绘制
	box->addDrawable(geometry);
	return box;
}

//mode2:加载多个模型 OSG获取模型坐标点、索引、法向量、纹理等数据
void multModel() {

	srand((unsigned)time(NULL));
	int textSize = 0;
	cout << "请输入提示文字的字体大小：";
	cin >> textSize;
	if (textSize <= 0 || textSize > 50)
		textSize = 5;
	cin.ignore();
	while (true)
	{
		vector<string> fileNames = readAbsolutPath();
		if (fileNames.size() == 0)
			cout << "输入的有效的文件路径为0" << endl;
		else
		{
			osgViewer::Viewer viewer;
			osg::ref_ptr<osg::Group> root = new osg::Group;
			osg::ref_ptr<osgDB::Options> options = new osgDB::Options("noRotation");
			for (size_t i = 0; i < fileNames.size(); i++)
			{
				string name = fileNames[i];
				osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(name, options);
				string modelName = Utility::getFileNameFromPath(name);
				PositionVisitor visitor = PositionVisitor(modelName, textSize);
				node->accept(visitor);
				root->addChild(visitor.createRandomColorOsgNode(i));
			}
			root->addChild(Utility::createCoorAxis(textSize));
			viewer.setSceneData(root);
			viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
			viewer.addEventHandler(new osgViewer::StatsHandler);
			viewer.addEventHandler(new ChangeViewerSize());
			viewer.setUpViewOnSingleScreen(1);//这里是单屏幕显示
			viewer.run();
		}
	}
}


osg::ref_ptr<osg::Group> calculationAABBandOBB(osg::ref_ptr<osg::Group> node ,string modelName,std::shared_ptr<RendertoTexture> renderTex) {
	osg::ref_ptr<osg::Group> root = new osg::Group;
	PositionVisitor visitor = PositionVisitor(modelName);
	//PositionVisitor visitor = PositionVisitor(modelName, 1);//节点访
	//问器
	node->accept(visitor);
	//OBB包围盒
	for (Geom* geom : visitor.getAllGeom())
	{
		osg::ref_ptr<osg::Vec3Array> array = new osg::Vec3Array;
		for (Vertex* vertex : geom->vertices)
			array->push_back(vertex->coor);
		render_system::OBB obb = render_system::OBB(array);
		//通过协方差->特征向量->OBB主轴->中心点xyz都可求得
		root->addChild(createOBBBox(obb, renderTex));

	}
	////AABB包围盒
	//for (size_t i = 0; i < node->getNumChildren(); i++)
	//{
	//	osg::ref_ptr<osg::Node> child = node->getChild(i);
	//	osg::ComputeBoundsVisitor boundvisitor;
	//	child->accept(boundvisitor);
	//	osg::BoundingBox box = boundvisitor.getBoundingBox();
	//	root->addChild(createBoundingBox(box.center(), box.xMax() - box.xMin(), box.yMax() - box.yMin(), box.zMax() - box.zMin()));
	//}
	osg::ref_ptr<osg::StateSet> stateset = node->getOrCreateStateSet();
	osg::ref_ptr<osg::Material> mat = new osg::Material();
	mat->setColorMode(osg::Material::ColorMode::DIFFUSE);
	stateset->setAttribute(mat);

	return root;
}

osg::ref_ptr<osg::StateSet> createTexture2DState(osg::ref_ptr<osg::Texture2D> texture)
{ 
	//创建状态集对象
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	texture->setDataVariance(osg::Object::DYNAMIC);

	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);

	return stateset.get();
}

/*
创建二维纹理状态对象
*/
osg::ref_ptr<osg::StateSet> createTexture2DState(osg::ref_ptr<osg::Image> image)
{
	//创建状态集对象
	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet();

	//创建二维纹理对象
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D();
	texture->setDataVariance(osg::Object::DYNAMIC);
	//设置贴图
	texture->setImage(image.get());

	stateset->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);

	return stateset.get();
}

/*
保存为obj文件
*/
void saveObjFile(osg::ref_ptr<osg::Group> root) {
	string directory;


	if (std::string::npos != last_slash_idx)
	{
		directory = filename.substr(0, last_slash_idx);
	}
	//保存当前场景到一个root.osg文件中
	osgDB::writeNodeFile(*(root.get()), +"root.obj");
}

int  testRendertoTexture(std::shared_ptr<RendertoTexture> renderToText) {
	//四边形
	osg::ref_ptr<osg::Geode> quad1 = new osg::Geode;
	quad1->addDrawable(osg::createTexturedQuadGeometry(
		osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0)));
	quad1->getOrCreateStateSet()->setTextureAttributeAndModes(0, renderToText->textureVec[0]);
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(quad1.get());
	root->addChild(renderToText->cameraVec[0]);

	osg::ref_ptr<osg::Geode> quad2 = new osg::Geode;
	quad2->addDrawable(osg::createTexturedQuadGeometry(
		osg::Vec3(1.5, 1.5, 1.5), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0)));
	quad2->getOrCreateStateSet()->setTextureAttributeAndModes(0, renderToText->textureVec[1]);
	root->addChild(quad2.get());
	root->addChild(renderToText->cameraVec[1]);

	osg::ref_ptr<osg::Geode> quad3 = new osg::Geode;
	quad3->addDrawable(osg::createTexturedQuadGeometry(
		osg::Vec3(3.0, 3.0, 3.0), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0)));
	quad3->getOrCreateStateSet()->setTextureAttributeAndModes(0, renderToText->textureVec[2]);
	root->addChild(quad3.get());
	root->addChild(renderToText->cameraVec[2]);

	osg::ref_ptr<osg::Geode> quad4 = new osg::Geode;
	quad4->addDrawable(osg::createTexturedQuadGeometry(
		osg::Vec3(4.5, 3.0, 3.0), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0)));
	quad4->getOrCreateStateSet()->setTextureAttributeAndModes(0, renderToText->textureVec[3]);
	root->addChild(quad4.get());
	root->addChild(renderToText->cameraVec[3]);

	osg::ref_ptr<osg::Geode> quad5 = new osg::Geode;
	quad5->addDrawable(osg::createTexturedQuadGeometry(
		osg::Vec3(6.0, 3.0, 3.0), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0)));
	quad5->getOrCreateStateSet()->setTextureAttributeAndModes(0, renderToText->textureVec[4]);
	root->addChild(quad5.get());
	root->addChild(renderToText->cameraVec[4]);

	osg::ref_ptr<osg::Geode> quad6 = new osg::Geode;
	quad6->addDrawable(osg::createTexturedQuadGeometry(
		osg::Vec3(7.5, 3.0, 3.0), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0)));
	quad6->getOrCreateStateSet()->setTextureAttributeAndModes(0, renderToText->textureVec[5]);
	root->addChild(quad6.get());
	root->addChild(renderToText->cameraVec[5]);


	osgViewer::Viewer viewer;
	viewer.setSceneData(root.get());
	viewer.addEventHandler(new ChangeViewerSize());
	return viewer.run();
}


int main()
{
	istringstream(Utility::readTxt(modePath)) >> boolalpha >> mode;
	if (mode) {

		osgViewer::Viewer viewer;
		//在加载obj模型的时候，会默认将模型绕x轴逆时针旋转90度(大坑)
		osg::ref_ptr<osgDB::Options> options = new osgDB::Options("noRotation");
		osg::ref_ptr<osg::Group> node = (osg::Group*)osgDB::readNodeFile(Utility::readTxt(filePath), options);
		string modelName = Utility::getFileNameFromPath(Utility::readTxt(filePath));

		auto renderToText = std::shared_ptr<RendertoTexture>(new RendertoTexture());
		renderToText->calculationOBBBoxLengthHightDeep(node, modelName);
		renderToText->createFrontBackLeftRightTopBottomTexture(node, renderToText->cameraVec, renderToText->textureVec, renderToText->calculationOBBBoxLengthHightDeep(node, modelName));
		//testRendertoTexture(renderToText);

		osg::ref_ptr<osg::Group> root = calculationAABBandOBB(node, modelName,renderToText);

		root->addChild(node);
		viewer.setSceneData(root);
		//事件入口
		viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));
		viewer.addEventHandler(new osgViewer::StatsHandler);
		viewer.addEventHandler(new ChangeViewerSize());
		viewer.setUpViewOnSingleScreen(1);//这里是单屏幕显示

		saveObjFile(root);
		viewer.run();
	}
	else {
		multModel();
	}
}
