# AABB-OBB-LOD0
    这是一个而基于3D包围盒技术AABB与OBB的先计算出box的顶点坐标，然后对模型的六个面进行离线渲染纹理的烘焙到2D纹理贴图中（FBO），在生成的box的六个面分别贴上这六张纹理贴图，来实现LOD！

同时这里还包含了多模型加载的实现以及使用了OSG的源码针对OBJ模型的编码解码的分析；

环境：当然 这里都是基于<u>C++</u>写的，我这里使用的<u>OSG</u>版本是<u>osg3.4</u> ，操作系统：<u>win10</u> IDE：<u>VS2017</u>；
			

###   AABB&OBB-LOD0流程：

​			1、先算出AABB或者OBB的Box坐标；
​			2、旋转FBO获取模型六个面并且通过离线渲染到二维的纹理上保存起来（为了方便调试六张分别放到六个平面上去展示）；
​			3、生成Box，把六张纹理贴图分别映射到Box的六个面；（这里可以考虑使用立方体贴图技术，这里我写的有一点下偶问题后期再做修改）

## 另外的功能：

### 多模型加载:

**只需要在配置文件mode.txt中修改为false就可以；**

### OBJ的编码解码：

这是为了研究osg的文件插件比如readNodeFile接口等等的机制，同时也是其他项目要求研究OBj格式做的实验；环境搭建只需要把对应版本的OSG的pdb放到debug目录下就可以了，
我这里因为是OSG3.4 MSVC141 所以放的是osg131-osgd.pdb！obj是.obj"与".mtl"文件转换为 osg中的3D model与材质纹理等等，OBJWriterNodeVisitor是写导出保存为obj格式包含了mtl材质库
ReaderWriterOBJ是读写Obj，与OSG文件读写接口都在这里；

## 作者的博客：

​			OSG的LOD ：https://blog.csdn.net/chenweiyu11962/article/details/107032395
​			AABB与OBB包围盒：https://blog.csdn.net/chenweiyu11962/article/details/107032402
