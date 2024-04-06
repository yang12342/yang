#ifndef _SCENE_H_
#define _SCENE_H_
#include <graphics.h>
class Scene
{
public:
	Scene() = default;
	~Scene() = default;


	virtual void on_enter() {}												//场景进入时的初始化
	virtual void on_updata(int delta) {}										//处理数据，根据玩家输入来整理更新数据
	virtual void on_draw() {}												//渲染绘图
	virtual void on_input(const ExMessage& msg) {}							//场景在处理玩家输入时调用
	virtual void on_exit() {}												//场景退出时的卸载逻辑


private:


};

#endif // !_SCENE_H_
