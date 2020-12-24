﻿/*************************************************
Author:MeteorChen
Date:2019-04-23
Description:工具类，进行一些与其他类无关的额外的基础工具操作
**************************************************/
#ifndef _UTILITY_H
#define _UTILITY_H

#include <osg/Vec3>
#include <string>
#include <osgText/Text>

using namespace std;

class Utility
{
public:

	Utility();
	~Utility();

	static bool isVec3Same(osg::Vec3 v1, osg::Vec3 v2);//比较两个三维向量是否相等
	static void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst);
	static string getFileNameFromPath(string path);//从模型路径中获取明名称
	static string readTxt(string file);
	static osg::ref_ptr<osgText::Text> createText(string info, int textSize, osg::ref_ptr<osgText::Font>font, osg::Vec3 position, int direction, osg::Vec4 textColor);
	static osg::ref_ptr<osg::Node> createCoorAxis(int textSize);
};

#endif 