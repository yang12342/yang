
#include "quit_button.h"
#include "start_button.h"
#include "scene_manager.h"

int idx_current_anim = 0;										//��¼��ʱ��Ⱦ���ǵڼ���ͼ��
const int PLAYER_ANIM_NUM = 6;									//��ɫһ������֡


 int WINDOW_WIDTH = 1280;
 int WINDOW_HEIGHT = 720;									//ȷ������λ��Ŀ����Ϊ�˷�ֹ����ܵ�������

const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;									//�������㰴ť��λ��

bool running = true;

#pragma comment(lib,"Winmm.lib")								//�����ǲ������ֵĿ�

Atlas atlas_player_left_object;
Atlas atlas_player_right_object;
Atlas atlas_enemy_left_object;
Atlas atlas_enemy_right_object;

Atlas* atlas_player_left;
Atlas* atlas_player_right;
Atlas* atlas_enemy_left;
Atlas* atlas_enemy_right;
//�۷���˵Ķ����Ͷ�Ӧ��ָ��
Atlas atlas_enemy_bee_left_object;
Atlas atlas_enemy_bee_right_object;
Atlas atlas_enemy_bee_left_sketch_object;
Atlas atlas_enemy_bee_right_sketch_object;


Atlas* atlas_enemy_bee_left;
Atlas* atlas_enemy_bee_right;
Atlas* atlas_enemy_bee_left_sketch;
Atlas* atlas_enemy_bee_right_sketch;
//�����Ӱ��ָ��
Atlas atlas_enemy_left_sketch_object;
Atlas atlas_enemy_right_sketch_object;
Atlas* atlas_enemy_left_sketch;
Atlas* atlas_enemy_right_sketch;
//��ɫ��Ӱ��ָ��
Atlas atlas_player_left_sketch_object;
Atlas atlas_player_right_sketch_object;
Atlas* atlas_player_left_sketch;
Atlas* atlas_player_right_sketch;


Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
SceneManager scene_manager;



Player player;
ExMessage msg;
IMAGE img_background;																	//����һ���ṹ���������ͼ�����
IMAGE img_menu;																			//�˵�
std::vector<Enemy*> enemy_pig_list;															//���ǵ��˶�̬���飬�������ɴ���Ұ��
std::vector<Enemy*> enemy_bee_list;
std::vector<Bullet> bullet_list(3);														//�������ȷ���������ӵ�

int main()
{
	initgraph(1280, 720);																	//����һ������
	////////////��Դ����
	atlas_player_left_object.load_from_file(_T("img/player_left_%d.png"), 6);				//��ҵ���ͼƬ��Դ�ļ��غ�
	flip_atlas(atlas_player_left_object, atlas_player_right_object);
	atlas_enemy_left_object.load_from_file(_T("img/enemy_left_%d.png"), 6);
	flip_atlas(atlas_enemy_left_object, atlas_enemy_right_object);
	//�۷�ķ�ת����Դ���أ����м�Ӱ����
	atlas_enemy_bee_left_object.load_from_file(_T("img/bee_left_%d.png"), 4);
	flip_atlas(atlas_enemy_bee_left_object, atlas_enemy_bee_right_object);
	sketch_atlas(atlas_enemy_bee_left_object, atlas_enemy_bee_left_sketch_object);
	sketch_atlas(atlas_enemy_bee_right_object, atlas_enemy_bee_right_sketch_object);



	sketch_atlas(atlas_enemy_left_object, atlas_enemy_left_sketch_object);
	sketch_atlas(atlas_enemy_right_object, atlas_enemy_right_sketch_object);

	sketch_atlas(atlas_player_left_object, atlas_player_left_sketch_object);
	sketch_atlas(atlas_player_right_object, atlas_player_right_sketch_object);

	//��ҵ���Դָ��
	atlas_player_left = &atlas_player_left_object;											//��ʼ��ȫ�ֱ���Atlasָ�룬����������ĩβ�ͷŵ�
	atlas_player_right =&atlas_player_right_object;
	atlas_player_left_sketch = &atlas_player_left_sketch_object;
	atlas_player_right_sketch = &atlas_player_right_sketch_object;
	//Ұ�����Դָ��
	atlas_enemy_left = &atlas_enemy_left_object;
	atlas_enemy_right =&atlas_enemy_right_object;
	atlas_enemy_left_sketch = &atlas_enemy_left_sketch_object;
	atlas_enemy_right_sketch = &atlas_enemy_right_sketch_object;
	//�۷����Դָ��
	atlas_enemy_bee_left = &atlas_enemy_bee_left_object;
	atlas_enemy_bee_right = &atlas_enemy_bee_right_object;
	atlas_enemy_bee_left_sketch = &atlas_enemy_bee_left_sketch_object;
	atlas_enemy_bee_right_sketch = &atlas_enemy_bee_right_sketch_object;


	mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);							//ʲôý����ƽӿڣ��������Ƶ���ص������У�������ȡ��Ϊ��bgm

	mciSendString(_T("open mus/hit.mp3 alias hit"), NULL, 0, NULL);							//�����Чͬ��



	RECT region_btn_start_game, region_btn_quit_game;										//���ﶨ������������Ǽ�¼��ʼ�ͽ�����ť���ηֱ��λ��

	region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
	region_btn_start_game.top = 430;
	region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;

	region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
	region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
	region_btn_quit_game.top = 550;
	region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT;					//����ť����ˮƽ���е�λ��


	StartGameButton btn_start_game = StartGameButton(region_btn_start_game,
		_T("img/ui_start_idle.png"), _T("img/ui_start_hovered.png"), _T("img/ui_start_pushed.png"));

	QuitGameButton btn_quit_game = QuitGameButton(region_btn_quit_game,
		_T("img/ui_quit_idle.png"), _T("img/ui_quit_hovered.png"), _T("img/ui_quit_pushed.png"));   //�����Ǿ��Ǽ���ͼƬ�����ص��ļ��оͷ�����Ŀ�ļ�����


	menu_scene = new MenuScene();															//ʵ�����˵���������
	game_scene = new GameScene();															//ʵ������Ϸ��������

	scene_manager.set_current_scene(menu_scene);											//һ��ʼ����Ϊ�˵�

	loadimage(&img_background, _T("img/background.png"));									//�Ѷ�Ӧ��ͼ���ļ������������
	loadimage(&img_menu, _T("img/menu.png"));
	BeginBatchDraw();																		//��ʼ������ͼ��eaxyx������

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

		FlushBatchDraw();																	//ִ��δ��ɵĻ�ͼ

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;


		if (delta_time < 1000 / 200)														//�����ǿ�����Ƶˢ��Ƶ��Ҳ��������ÿ֡ͣ������ѭ��
																							//��������ߺ���һ��ѭ����ÿ������ͣ��42���룬�����Ķ�����Ҫ252���룬ÿ���������Ĵζ���
		{
			Sleep(1000 /200 - delta_time);

		}

		EndBatchDraw();
	}
	return 0;
}
