/*************************************************
Author:chenwy
Date:2019-04-23
Description:Geom是一个建筑物中，不可再分的单独组件，其中存储了组件的顶点信息及三角形信息（储存模型）
**************************************************/
#ifndef _GEOM_H
#define  _GEOM_H

#include <vector>
#include "Vertex.h"
#include "Trianngle.h"
#include <osg/ref_ptr>
#include <osg/Geode>
using namespace std;


class Geom
{
public:
	vector<Vertex*> vertices;//一个geom中所有的顶点信息
	vector<Trianngle*> trianngles;//一个geom中的所有三角形信息
	osg::BoundingBox  boundingBox;//包围盒
	bool isTwoTriangleNeighbor(int triangle1Index,int triangle2Index);//两个三角形是否相邻
	void createTriangleTopo();//创建三角形之间的拓扑关系
	void createVertexTopo();//创建顶点之间的拓扑
	/*
	将得到的数据用纯色重新绘制出来并返回一个osg的Geod节点的函数，自己要将模型的不同部件用
	不同的颜色显示，以观察部件之间的联系和分别时
	*/
	osg::ref_ptr<osg::Geode> createOsgNode(osg::Vec4 color);
	Geom();
	~Geom();
};


#endif

