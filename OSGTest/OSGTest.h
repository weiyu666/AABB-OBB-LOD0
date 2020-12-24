#ifndef _OSG_TEST_H
#define _OSG_TEST_H

#include<list>
#include <osg/Vec3>
#include <osg/Vec4>
#include <regex>
#include <windows.h>
#include<osg/TexGen>
#include <osg/LineWidth>
#include <osg/ShapeDrawable>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ComputeBoundsVisitor>
#include <osg/Material>

#include "Utility.h"
#include "RendertoTexture.h"

using namespace std;

static const int POSX = 100;
static const int POSY = 50;
static const int WIDTH = 800;
static const int HEIGHT = 600;
static std::string filePath = "cfg.txt";
static std::string modePath = "mode.txt";

const char* split = " ";//分隔字符
string filename = "";//Utility::readTxt(filePath);
const size_t last_slash_idx = filename.rfind('\\');
bool mode ;	//true为mode1求AABB与OBB false为mode2 多模型加载


/*
只需要增加一个继承于osgGA::GUIEventHandler的类，然后重写handle事件就可以了
*/
class ChangeViewerSize :public osgGA::GUIEventHandler
{
public:
	ChangeViewerSize();
	~ChangeViewerSize();
	virtual  bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

private:
	bool m_isFrist;
};

ChangeViewerSize::ChangeViewerSize()
{
	m_isFrist = true;
}

ChangeViewerSize::~ChangeViewerSize()
{
}
bool ChangeViewerSize::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	if (m_isFrist)
	{
		m_isFrist = false;
		osgViewer::Viewer* vie = dynamic_cast<osgViewer::Viewer*>(&us);
		osgViewer::Viewer::Windows ws;
		//得到窗口
		vie->getWindows(ws);
		if (!ws.empty())
		{
			osgViewer::Viewer::Windows::iterator iter = ws.begin();
			for (; iter != ws.end(); iter++)
			{
				//可以自己设定大小
				(*iter)->setWindowRectangle(POSX, POSX, WIDTH, HEIGHT);
				//是否有窗口边框
				(*iter)->setWindowDecoration(true);
			}
		}
	}

	return false;
}

/*
mode2:加载多个模型 OSG获取模型坐标点、索引、法向量、纹理等数据
*/
void multModel();


/*
生成AABB
*/
osg::ref_ptr<osg::Geode> createBoundingBox(osg::Vec3 center, float lenx, float leny, float lenz);

/*
生成OBB
*/
osg::ref_ptr<osg::Geode> createOBBBox(render_system::OBB obb, std::shared_ptr<RendertoTexture> renderTex);

/*
保存输出文件obj
*/
void saveObjFile(osg::ref_ptr<osg::Group> root);

/*
创建二维纹理状态对象
*/
osg::ref_ptr<osg::StateSet> createTexture2DState(osg::ref_ptr<osg::Image> image);

/*
mode1:计算AABB 与OBB
*/
osg::ref_ptr<osg::Group> calculationAABBandOBB(osg::ref_ptr<osg::Group> node, string modelName, std::shared_ptr<RendertoTexture> renderTex);

//判断文件是否存在
bool fileExists(const std::string fileName)
{
	ifstream fin(fileName);
	if (!fin)
		return false;
	return true;
}

/**
 * 从控制台读取模型的路径：所有模型的路径全是绝对路径，中间用逗号分隔
 */
vector<string> readAbsolutPath()
{
	vector<string> results;//存储所有要加载的模型的绝对路径
	cout << "请输入模型的绝对路径（如果有多个模型，中间用空格隔开）" << endl;
	string modelPaths;
	getline(std::cin, modelPaths);
	std::regex re{ split };
	// 调用 std::vector::vector (InputIterator first, InputIterator last,const allocator_type& alloc = allocator_type())
	// 构造函数,完成字符串分割
	vector<string> splitResult = std::vector<std::string>{
		std::sregex_token_iterator(modelPaths.begin(), modelPaths.end(), re, -1),std::sregex_token_iterator()
	};
	for (string modelName : splitResult)
	{
		if (fileExists(modelName))
			results.push_back(modelName);
	}
	return results;
}

#endif