/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:访问模型节点的访问器
**************************************************/
#ifndef	 _POSITION_VISITOR_H
#define	 _POSITION_VISITOR_H

#include <osg/NodeVisitor>
#include <vector>
#include <osg/TriangleIndexFunctor>
#include <osgText/Text>

#include "TriangleIndex.h"
#include "Geom.h"
#include "ModelAttributeFunctor.h"
using namespace std;
class PositionVisitor
	:public osg::NodeVisitor
{
protected:
	vector<Geom*> allGeom;//所有的geom
	osg::Vec4 geomColor;//geom的颜色
	string modelName;//模型名称
	osg::BoundingBox boundingBox;//包围盒

	int textSize;//提示文字的大小

public:
	virtual  void apply(osg::Geode& node) override;
	void dealTriangleInfo(ModelAttributeFunctor attributeFunctor,osg::TriangleIndexFunctor<TriangleIndex> indexFunctor);//处理访问器得到的信息，构建三角形关系
	osg::ref_ptr<osg::Node> createOsgNode(osg::Vec4 color,int order);//根据指定的颜色，将geom中的数据创建成osg节点
	osg::ref_ptr<osg::Node> createRandomColorOsgNode(int order);//将geom中的数据创建成osg节点，颜色随机
	osg::ref_ptr<osgText::Text> createTipText(short direction);//创建提示文字
	PositionVisitor(string ModelName);
	PositionVisitor(string ModelName, int TextSize);
	vector<Geom*> getAllGeom();//获取所有的geom
	~PositionVisitor();
};


#endif // !
