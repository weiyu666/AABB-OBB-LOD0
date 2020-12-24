/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:访问模型中所有信息的访问器
**************************************************/
#ifndef _MODEL_ATTRIBUTE_FUNCTOR_H
#define _MODEL_ATTRIBUTE_FUNCTOR_H

#include <osg/Drawable>

//图元的访问和图元索引的访问AttributeFunctor&（三角面定点获取）
class ModelAttributeFunctor
	:public osg::Drawable::AttributeFunctor
{
public:
	osg::ref_ptr<osg::Vec3Array> vertexList;//存储顶点的数组
	osg::ref_ptr<osg::Vec3Array> normalList;//存储法向量
	osg::ref_ptr<osg::Vec2Array> textCoordList;//纹理坐标
	/*
	要获取顶点坐标位置和纹理坐标，顶点位置是osg::Vec3类型，而纹理坐标是osg::Vec2类型,
	重写apply的Vec2* Vec3*
	*/
	virtual void apply(osg::Drawable::AttributeType, unsigned, osg::Vec2*) override;
	virtual void apply(osg::Drawable::AttributeType, unsigned, osg::Vec3*) override;
	ModelAttributeFunctor();
	~ModelAttributeFunctor();
};


#endif