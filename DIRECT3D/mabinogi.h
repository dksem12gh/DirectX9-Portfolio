#pragma once
#include "gameNode.h"
#include "player.h"
#include "tirchonailField.h"
#include "Pet_cloud.h"
#include "Monster_fox.h"
#include "MonsterManager.h"
#include "UI.h"

class mabinogi : public gameNode
{
private:
	vector<light*>				_lights;					//������
	tirchonailField*			_scene;						//����
	player*						_player;					//�÷��̾�
	UI*							_ui;						//UI
	Pet_cloud*					_petCloud;					//�÷��̾��� ��
	MonsterManager*			    _mob;						//����

	Monster*					_clickMonster;				//Ÿ�ϸ��� ����

	vector<baseObject*>			_cullObjects;				//����ü�ø� �Ұ��� ������ 

	Ray							_playerStop;				//�÷��̾ ��ž�ҷ��� ����� ����

	bool						_click;						//Ŭ���� �Ǿ���?

	dyeing*						_dyeingInterface;
public:
	HRESULT init(void);
	void lightInit(void);
	void release(void);
	void update(void);
	void render(void);

	//�÷��̾�� ������ ��Ʋ
	void playerMonsterBattle();	
	void playerMonsterAtk();
	void playerMonsterHit();
	//�÷��̾ �ʰ��� �浹�� ���ߴ� �Լ�
	void playerMapstop();	

	mabinogi() {};
	~mabinogi() {};
};

