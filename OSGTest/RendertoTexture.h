#ifndef _RENDER_TO_TEXTURE_H
#define _RENDER_TO_TEXTURE_H

#include <osg/Quat>
#include <osg/Geode>  
#include<osg/Texture2D>
#include<osg/Camera>
#include<osg/MatrixTransform>

#include "OBBbox.h"
#include "PositionVisitor.h"

//OBB box的长深高给纹理烘焙的视宽高
typedef struct
{
	double length = 0.0;
	double hight = 0.0;
	double deep = 0.0;
}OBBBoxLengthHightDeep;

class RendertoTexture
{
public:

	std::vector<osg::Texture2D*> textureVec;
	std::vector<osg::Camera*> cameraVec;

	/*
	Render to Texture
	*/
	osg::Texture2D* createRttTexture(int texWidth, int texHeight);

	/*
	定义相机节点
	*/
	osg::Camera* createRttCamera(int texWidth, int texHeight, const osg::BoundingSphere& bs);

	/*
	计算OBB box的长深高,要求纹理烘焙的下相机视口大小与纹理尺寸保持一致
	*/
	OBBBoxLengthHightDeep  calculationOBBBoxLengthHightDeep(osg::ref_ptr<osg::Group> node, std::string modelName);

	/*
	生成box的六个面的纹理
	*/
	void createFrontBackLeftRightTopBottomTexture(osg::Node* model, std::vector<osg::Camera*>& camVec, std::vector<osg::Texture2D*>& texVec , OBBBoxLengthHightDeep obblhd);


};


#endif 