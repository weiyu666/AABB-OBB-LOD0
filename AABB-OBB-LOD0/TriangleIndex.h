/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:遍历模型中三角形索引的访问器
**************************************************/
#ifndef _TRIANGLE_INDEX_H
#define _TRIANGLE_INDEX_H

#include <osg/ref_ptr>
#include <osg/Array>
#include <osg/TriangleIndexFunctor>

class TriangleIndex
{
public:
	osg::ref_ptr<osg::UIntArray> indexs;//所有的索引
	int triangleNum;//三角形的数量
	TriangleIndex();
	~TriangleIndex();
	//每绘制一个三角形，都会调用一次这个函数。这个函数中，很明显可以猜到，v1，v2，v3就是三角形三个顶点的索引
	void operator()(const unsigned int& v1, const unsigned int& v2, const unsigned int& v3);
};


#endif 