# GraphicsRelated
## cmake as ..../GraphicsRelated/:
cmake -B build/x64 -S ./ -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug

延迟渲染器在另一个仓 https://github.com/hzy-Recover2024/Deferred_renderer/tree/develop

# 场景图
可绘制节点drawableNode携带所有geometry信息，表现为多个mesh的容器。
drawableNode定义为多父多子，可提供的连接节点为transformNode(单父单子)。
## 路径解析(实例解析)
drawableNode的每个路径都将构成一个实例，携带实例信息(矩阵、颜色等)。

# 合并batch
单个drawableNode定义为一个实例，drawableNode内mesh的顶点和索引将进行合并绘制，形成单次instance绘制的顶点来源。
多drawableNode的合并：将每个drawableNode作为multiDraw的子draw，顶点和索引的合并形成该renderbatch的逐顶点vetexstream。
instance顶点数据的平铺：单renderbatch将包括所有instance属性，和vetexstream共同组成renderbatch的vertex Sepcificiation。

# GPU Driven pipeline
合并后的batch将参与GPU内剔除，其中HZB遮挡剔除的单元为instance。
构建render pass：前景UI、不透明、非HZB剔除、HZB剔除、透明....
## 构建Hierarchical Z-Buffer：
在遮挡物渲染PASS中生成深度纹理，然后基于该深度纹理生成MipMap；
对每层MipMap执行一次屏幕四边形渲染，每次访问上一层的纹理；
在shader中，访问上一层纹理的四个纹素，取其中的最大值作为该级纹理深度。
## Nanite剔除
    1. 假定物体的可见性在帧和帧之前存在连续性，使用上一帧可见的物体绘制z-buffer；
    2. 对剩余不可见的进行剔除，分成可见和不可见，可见的需要进行补画；
    3. 更新z-buffer，对所有物体进行剔除，分成可见和不可见两部分，为下一帧的绘制做准备
    
# 动态松散八叉树
单个八叉树node将管理一批空间位置接近的drawableNode实例；
八叉树节点提供动态分裂、合并功能，八叉树提供扩张能力；
基于八叉树和硬件遮挡查询的遮挡剔除。

# 顺序无关的透明度
链式混合：像素链表，每个片元位置存储一个片元信息的链表，最后排序该链表并按顺序渲染
权重混合：最终颜色由总颜色和除以总权重近似得到

# In future
材质合并
阴影
persistent Map



