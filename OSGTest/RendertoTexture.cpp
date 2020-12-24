#include "RendertoTexture.h"

osg::Texture2D * RendertoTexture::createRttTexture(int texWidth, int texHeight)
{
	//创建一个空的二维纹理，用来保存将要烘焙的对象
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setInternalFormat(GL_RGBA);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

	texture->setTextureSize(texWidth, texHeight);
	return texture.release();
}

osg::Camera * RendertoTexture::createRttCamera(int texWidth, int texHeight, const osg::BoundingSphere & bs)
{
	osg::ref_ptr<osg::Camera> rttCamera = new osg::Camera;
	//清理Camera只需要清理color与deep缓存
	rttCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//设置相机参考坐标
	rttCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);

	rttCamera->setViewport(0, 0, texWidth, texHeight);
	//在主场景之前渲染，在主场景之前就完成烘焙
	rttCamera->setRenderOrder(osg::Camera::POST_RENDER);
	//设置为FBO相机，子场景渲染到纹理
	rttCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

	double viewDistance = 2.0 * bs.radius();
	double znear = viewDistance - bs.radius();
	double zfar = viewDistance + bs.radius();
	float top = 0.6 * znear;
	float right = 0.8 * znear;
	//透视投影
	rttCamera->setProjectionMatrixAsFrustum(-right, right, -top, top, znear, zfar);

	//设置中心与观察矩阵 任何相机都必须
	/*osg::Vec3d upDirection(0.0, 0.0, 1.0);
	osg::Vec3d viewDirection(0.0, -1.0, 0.0);*/
	osg::Vec3d upDirection(1.0, 0.0, 0.0);
	osg::Vec3d viewDirection(0.0, 1.0, 0.0);
	osg::Vec3d center = bs.center();
	osg::Vec3d eyePoint = center + viewDirection * viewDistance;
	rttCamera->setViewMatrixAsLookAt(eyePoint, center, upDirection);

	return rttCamera.release();
}

/*
计算OBB box的长深高,要求纹理烘焙的下相机视口大小与纹理尺寸保持一致
*/
OBBBoxLengthHightDeep RendertoTexture::calculationOBBBoxLengthHightDeep(osg::ref_ptr<osg::Group> node, std::string modelName)
{
	render_system::OBB obb;
	PositionVisitor visitor = PositionVisitor(modelName);
	node->accept(visitor);
	for (Geom* geom : visitor.getAllGeom())
	{
		osg::ref_ptr<osg::Vec3Array> array = new osg::Vec3Array;
		for (Vertex* vertex : geom->vertices)
			array->push_back(vertex->coor);
		obb = render_system::OBB(array);
	
		//float vectorAngle;
		//osg::Vec3 Evals;
		//osg::Vec3 _xUnitVectorAxis(1.0,0.0,0.0);
		//osg::Vec3 direction(1.0, 0.0, 0.0);
		//render_system::saveGetOBBOrientation(array, Evals);//获取特征向量，就得到obb主轴
		//Evals.normalize();//归一化处理
		//auto temp = Evals * _xUnitVectorAxis;//向量的叉乘，根据叉乘后的向量与屏幕方向来确定角度是否超过180°
		///*
		//θ=acos(v1⋅v2/||v1||||v2||,)需要注意的θ的范围再0~pi之间，要解决0~2pi的问题
		//就需用向量的叉乘，根据叉乘后的向量与屏幕方向来确定角度是否超过180°其中1和 - 1时为0和180°EPSILON是最小误差
		//*/
		//if (fabs(fabs(temp)-1)< FLT_EPSILON) {
		//	vectorAngle = temp > 0 ? 0 : osg::PI;
		//}
		//float angle = (float)acos(temp);
		// 两个向量的叉乘结果与屏幕方向是否一致来判断角度是否超过180°
		//osg::Vec3f axis = Evals ^ _xUnitVectorAxis;
		//vectorAngle = axis * direction > 0 ? angle : 2 * osg::PI - angle;
		//vectorAngle = angle;
		//osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
		//mt->addChild(node);
		//mt->setMatrix(osg::Matrix::rotate(angle,osg::Vec3(0, 0, 1)));//水平方向旋转弧度angle
		//node->addChild(mt);
	}
	osg::ref_ptr<osg::Vec3Array> point;
	obb.getCorners(point);
	vector<osg::Vec3> obbPnt = point->asVector(); 
	OBBBoxLengthHightDeep obblhd;

	auto leftTopBottom = obbPnt[0] - obbPnt[1];
	auto rightLeftBottom = obbPnt[1] - obbPnt[2];
	auto leftTopFrontLeftTopBack = obbPnt[0] - obbPnt[7];
	//left top front reduce left bottom front 
	obblhd.hight = sqrt(leftTopBottom*leftTopBottom);
	//right bottom front reduce left bottom front
	obblhd.length = sqrt(rightLeftBottom*rightLeftBottom);
	//left top front reduce left top back
	obblhd.deep = sqrt(leftTopFrontLeftTopBack*leftTopFrontLeftTopBack);
	return  obblhd;
}


void RendertoTexture::createFrontBackLeftRightTopBottomTexture(osg::Node * model, std::vector<osg::Camera*>& camVec, std::vector<osg::Texture2D*>& texVec, OBBBoxLengthHightDeep obblhd )
{
	//创建带纹理的四边形几何
	for (size_t index = 1; index <= 6; index++)
	{
		//注意纹理的尺寸与FBO相机视角大小相同
		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
		osg::Texture2D* texture2D = nullptr;
		osg::Camera* camera =nullptr;
		mt->addChild(model);
		mt->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		/*auto W=800, H=720;
		osg::Texture2D* texture2D = createRttTexture(W, H);
		osg::Camera* camera = createRttCamera(W, H, model->getBound());*/
		switch (index)
		{
		case 1:
			//Front前面
			//生成一张纹理
			texture2D = createRttTexture(obblhd.length*100, obblhd.hight * 100);
			camera = createRttCamera(obblhd.length * 100, obblhd.hight * 100, model->getBound());
			//默认是front 什么也不用做
			break;
		case 2:
			//Back后面
			//生成一张纹理
			texture2D = createRttTexture(obblhd.length * 100, obblhd.hight * 100);
			camera = createRttCamera(obblhd.length * 100, obblhd.hight * 100, model->getBound());
			mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(180.0f), osg::Vec3d(0, 0, 1)));
			break;
		case 3:
			//Left左面
			//生成一张纹理
			texture2D = createRttTexture(obblhd.deep * 100, obblhd.hight * 100);
			camera = createRttCamera(obblhd.deep * 100, obblhd.hight * 100, model->getBound());
			mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0f), osg::Vec3d(0, 0, 1)));
			break;
		case 4:
			//Right右面
			//生成一张纹理
			texture2D = createRttTexture(obblhd.deep * 100, obblhd.hight * 100);
			camera = createRttCamera(obblhd.deep * 100, obblhd.hight * 100, model->getBound());
			mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(-180.0f), osg::Vec3d(0, 0, 1)));
			mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(-90.0f), osg::Vec3d(0, 0, 1)));
			break;
		case 5:
			//Top上面
			//生成一张纹理
			texture2D = createRttTexture(obblhd.length * 100, obblhd.deep * 100);
			camera = createRttCamera(obblhd.length * 100, obblhd.deep * 100, model->getBound());
			mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.0f), osg::Vec3d(1, 0, 0)));
			break;
		case 6:
			//Bottom底面
			//生成一张纹理
			texture2D = createRttTexture(obblhd.length * 100, obblhd.deep * 100);
			camera = createRttCamera(obblhd.length * 100, obblhd.deep * 100, model->getBound());
			mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(270.0f), osg::Vec3d(0, 1, 0)));
			mt->setMatrix(osg::Matrix::rotate(osg::inDegrees(-90.0f), osg::Vec3d(1, 0, 0)));
			break;
		default:
			break;
		}
		//子场景在FBO相机之下
		camera->addChild(mt);
		//相机的颜色缓存数据也就是渲染得到的像素数据与纹理绑定，渲染到纹理
		camera->attach(osg::Camera::COLOR_BUFFER, texture2D);
		camVec.push_back(camera);
		texVec.push_back(texture2D);
	}
}

