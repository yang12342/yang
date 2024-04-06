
#include "quit_button.h"
#include "start_button.h"
#include "scene_manager.h"

int idx_current_anim = 0;										//记录此时渲染的是第几个图象
const int PLAYER_ANIM_NUM = 6;									//角色一共就六帧


 int WINDOW_WIDTH = 1280;
 int WINDOW_HEIGHT = 720;									//确定窗口位置目的是为了防止玩家跑到窗口外

const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;									//辅助计算按钮的位置

bool running = true;

#pragma comment(lib,"Winmm.lib")								//这里是播放音乐的库

Atlas atlas_player_left_object;
Atlas atlas_player_right_object;
Atlas atlas_enemy_left_object;
Atlas atlas_enemy_right_object;

Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;
//蜜蜂敌人的动画和对应的指针
Atlas atlas_enemy_bee_left_object;
Atlas atlas_enemy_bee_right_object;
Atlas atlas_enemy_bee_left_sketch_object;
Atlas atlas_enemy_bee_right_sketch_object;


Atlas* atlas_enemy_bee_left;
Atlas* atlas_enemy_bee_right;
Atlas* atlas_enemy_bee_left_sketch;
Atlas* atlas_enemy_bee_right_sketch;
//怪物剪影和指针
Atlas atlas_enemy_left_sketch_object;
Atlas atlas_enemy_right_sketch_object;
Atlas* atlas_enemy_left_sketch;
Atlas* atlas_enemy_right_sketch;
//角色剪影和指针
Atlas atlas_player_left_sketch_object;
Atlas atlas_player_right_sketch_object;
Atlas* atlas_player_left_sketch;
Atlas* atlas_player_right_sketch;


Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
SceneManager scene_manager;



Player player;
ExMessage msg;
IMAGE img_background;																	//都是一个结构体对象，这是图像对象
IMAGE img_menu;																			//菜单
std::vector<Enemy*> enemy_pig_list;															//这是敌人动态数组，用于容纳存活的野猪
std::vector<Enemy*> enemy_bee_list;
std::vector<Bullet> bullet_list(3);														//这里就明确创建三个子弹

int main()
{
	initgraph(1280, 720);																	//创建一个窗口
	////////////资源加载
	atlas_player_left_object.load_from_file(_T("img/player_left_%d.png"), 6);				//玩家敌人图片资源的加载和
	flip_atlas(atlas_player_left_object, atlas_player_right_object);
	atlas_enemy_left_object.load_from_file(_T("img/enemy_left_%d.png"), 6);
	flip_atlas(atlas_enemy_left_object, atlas_enemy_right_object);
	//蜜蜂的翻转和资源加载，还有剪影加载
	atlas_enemy_bee_left_object.load_from_file(_T("img/bee_left_%d.png"), 4);
	flip_atlas(atlas_enemy_bee_left_object, atlas_enemy_bee_right_object);
	sketch_atlas(atlas_enemy_bee_left_object, atlas_enemy_bee_left_sketch_object);
	sketch_atlas(atlas_enemy_bee_right_object, atlas_enemy_bee_right_sketch_object);



	sketch_atlas(atlas_enemy_left_object, atlas_enemy_left_sketch_object);
	sketch_atlas(atlas_enemy_right_object, atlas_enemy_right_sketch_object);

	sketch_atlas(atlas_player_left_object, atlas_player_left_sketch_object);
	sketch_atlas(atlas_player_right_object, atlas_player_right_sketch_object);

	//玩家的资源指针
	atlas_player_left = &atlas_player_left_object;											//初始化全局变量Atlas指针，在主函数的末尾释放掉
	atlas_player_right =&atlas_player_right_object;
	atlas_player_left_sketch = &atlas_player_left_sketch_object;
	atlas_player_right_sketch = &atlas_player_right_sketch_object;
	//野猪的资源指针
	atlas_enemy_left = &atlas_enemy_left_object;
	atlas_enemy_right =&atlas_enemy_right_object;
	atlas_enemy_left_sketch = &atlas_enemy_left_sketch_object;
	atlas_enemy_right_sketch = &atlas_enemy_right_sketch_object;
	//蜜蜂的资源指针
	atlas_enemy_bee_left = &atlas_enemy_bee_left_object;
	atlas_enemy_bee_right = &atlas_enemy_bee_right_object;
	atlas_enemy_bee_left_sketch = &atlas_enemy_bee_left_sketch_object;
	atlas_enemy_bee_right_sketch = &atlas_enemy_bee_right_sketch_object;


	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);							//什么媒体控制接口，将这段音频加载到程序当中，并将其取名为，bgm

	mciSendString(_T("open mus/hit.mp3 alias hit"), NULL, 0, NULL);							//打击音效同理



	RECT region_btn_start_game, region_btn_quit_game;										//这里定义的俩个变量是记录开始和结束按钮矩形分别的位置

	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 550;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;					//将按钮放在水平居中的位质


	StartGameButton btn_start_game = StartGameButton(region_btn_start_game,
		_T("img/ui_start_idle.png"), _T("img/ui_start_hovered.png"), _T("img/ui_start_pushed.png"));

	QuitGameButton btn_quit_game = QuitGameButton(region_btn_quit_game,
		_T("img/ui_quit_idle.png"), _T("img/ui_quit_hovered.png"), _T("img/ui_quit_pushed.png"));   //这里是就是加载图片，加载的文件夹就放在项目文件夹里


	menu_scene = new MenuScene();															//实例化菜单场景对象
	game_scene = new GameScene();															//实例化游戏场景对象

	scene_manager.set_current_scene(menu_scene);											//一开始设置为菜单

	loadimage(&img_background, _T("img/background.png"));									//把对应的图像文件赋给这个变量
	loadimage(&img_menu, _T("img/menu.png"));
	BeginBatchDraw();																		//开始批量绘图，eaxyx的语言

	while (running)
	{
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			scene_manager.input(msg);
		}


		scene_manager.updata();

		cleardevice();


		scene_manager.draw();

		FlushBatchDraw();																	//执行未完成的绘图

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;


		if (delta_time < 1000 / 200)														//这里是控制视频刷新频率也是贴合了每帧停留六个循环
																							//大体就是七毫秒一个循环，每个动作停留42毫秒，完整的动作需要252毫秒，每秒可以完成四次动作
		{
			Sleep(1000 /200 - delta_time);

		}

		EndBatchDraw();
	}
	return 0;
}
