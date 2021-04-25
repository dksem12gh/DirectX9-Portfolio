#pragma once
#include "skill.h"
#include "meshInfo.h"
#include "UI.h"
#include "Pet_cloud.h"
#include "billboardProgress.h"

class UI;

class player
{
private:
	skill *	_playerSkill;			//�÷��̾� ��ų
	meshInfo* _meshInfo;			//�޽�
	camera*	_mainCamera;			//����ī�޶�(������ �޾ƿ�)
	baseObject*	_scene;				//���� �޾ƿ��� ����(������ �޾ƿ�)
	UI*			_ui;				//UI
	Pet_cloud*  _pet;				//�ҷ��� ��
	
	Ray			_moveRay;			//�̵��� ����� ����
	Ray _heightRay;					//���� �Ǻ��� ����
	
	D3DXVECTOR3 _movePos;			//�̵��� ��ǥ
	D3DXVECTOR3 _targetPos;			//Ÿ�� ������ ��ǥ
	billboardProgress* _hpBar;

	map<int, int> _mPlayerSkillIcon;	//�÷��̾� ��ų ������ map
			
	char* _name;					//�̸�

	int _maxHp;						//�ִ�����
	int _curHp;						//��������
	int _maxMp;						//�ִ븶��
	int _curMp;						//���縶��
	int _maxSt;						//�ִ뽺�׹̳�
	int _curSt;						//���罺�׹̳�
	int _str;						//��(ü��)
	int _intellect;					//����
	int _dex;						//�ؾ�
	int _will;						//����
	int _luck;						//���
	int _lv;						//����
	int _maxAtk;					//�ִ���ݷ�
	int _minAtk;					//�ּҰ��ݷ�
	int _criticalChance;			//ũ��Ƽ�� Ȯ��
	int _defense;                   //��� 
	int _protect;                   //��ȣ

	int playerMotionCount;			//�÷��̾��� ���
	int playerSkillCount;			//�÷��̾��� ��ų
	int playerStateCount;			//�÷��̾��� ����
	int playerMainAtkCount;			//�÷��̾��� �⺻���� 
	
	float _atkTimer;					//�÷��̾��� �⺻���� Ÿ�̸�
	float _moveSpeed;				//�̵��ӵ�
	float _flyHeight;					//���� ���̿� ����
	float _movedistance;				//�̵���ǥ���� �����Ÿ�
	float _hyusicTimer;				//�޽Ľ� ȸ��Ÿ�̸�

	bool _battleMode;				//������屸��
	bool _ridingMode;				//ž�¸�屸��
	bool _flyMode;					//�����屸��
	bool _isStiffen;				//���� ����
	bool _moveState;				//�̵��ϴ��� ���ϴ���
	bool _targetBool;				//Ÿ���� �ִ��� ������ üũ
	bool _bDie;						//�÷��̾ �׾����� üũ
	bool _isDown;					//�÷��̾ ���󰡴��� üũ
	bool _damageDelay;				//������ ������

	//ĳ������ ���
	enum playerMotion
	{
		non,
		idle,						//�⺻ ���ֱ�
		battleIdle,					//���� ���ֱ�
		run,						//�⺻ �޸���
		battleRun,					//���� �޸���
		hit,						//�±�
		nuckDown,					//���ư�
		die							//�׾�����
	};

	//ĳ������ ��ų���(UI����)
	enum playerSkill
	{
		skillNon,					//�⺻
		smash,						//���Ž�
		defens,						//���ҽ�
		counter,					//ī����
		windmill,					//�����
		hyusic,						//�޽�
		temp						//NULL��
	};

	//ĳ������ ����
	enum PlayerState
	{
		stateNon,					//�ƹ��͵� ���Ҷ�
		atkState,					//����
		riging						//ž��
	};

	//ĳ������ �⺻����
	enum playerMainAtk
	{
		atkNon,						//�ƹ��͵� ����
		atk01,						//�⺻����01
		atk02,						//02
		atk03,						//03
	};

public:
	player();
	~player();

	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//Ű ���� ���ν���
	void playerAtkKey();			//���ݽ�
	void playerMoveKey();			//�̵���
	void playerUi();				//�÷��̾� �⺻ Ű(UI)
	
	//atk!
	void playerAtkSkillIcon();		//�÷��̾��� ��ų ����Ű
	void playerAtkSkill();			//�÷��̾��� ��ų 
	int playerCritical();			//�÷��̾� ũ��Ƽ�� ������
	//hit!
	void playerHit(int state, int damage);		//�÷��̾ �´� �Լ�
	void playerDie();				//�÷��̾ �״� �Լ�
	void playerDown();				//�÷��̾ ���󰡴� �Լ�
	//����
	void playerheight();			//��������
	void playerMove();				//ĳ���� �̵�
	void playerMoveStop();			//�÷��̾��� �����浹�� ���ߴ� �Լ�
	//��
	void petRiding();				//��ž��
	void petFlyMoveKey();			//��ž���� ����� �̵�
	//������
	void playerLvUp();


	//ĳ������ ���� get, set
	char* getName() { return _name; }
	void setName(char* name) { _name = name; }

	int getMaxHp() { return _maxHp; }
	int* getMaxHpAddress() { return &_maxHp; }
	void setMaxHp(int maxHp) { _maxHp = maxHp; }

	int getCurHp() { return _curHp; }
	int* getCurHpAddress() { return &_curHp; }
	void setCurHp(int curHp) { _curHp = curHp; }

	int getMaxMp() { return _maxMp; }

	int* getMaxMpAddress() { return &_maxMp; }
	void setMaxMp(int maxMp) { _maxMp = maxMp; }

	int getCurMp() { return _curMp; }

	int* getCurMpAddress() { return &_curMp; }
	void setCurMp(int curMp) { _maxMp = curMp; }

	int getMaxSt() { return _maxSt; }
	
	int* getMaxStAddress() { return &_maxSt; }
	void setMaxSt(int maxSt) { _maxSt = maxSt; }

	int getCurSt() { return _curSt; }
	int* getCurStAddress() { return &_curSt; }
	void setCurSt(int curSt) { _curSt = curSt; }

	int getCriticalChance() { return _criticalChance; }
	void setCriticalChance(int criticalChance) { _criticalChance = criticalChance; }
	
	int getDefense() { return _defense; }
	void setDefense(int defense) { _defense = defense; }
	
	int getProtect() { return _protect; }
	void setProtect(int protect) { _protect = protect; }

	int getStr() { return _str; }
	void setStr(int str) { _str = str; }

	int getIntellect() { return _intellect; }
	void setIntellect(int intellect) { _intellect = intellect; }

	int getDex() { return _dex; }
	void setDex(int dex) { _dex = dex; }

	int getWill() { return _will; }
	void setWill(int will) { _will = will; }

	int getLuck() { return _luck; }
	void setLuck(int luck) { _luck = luck; }

	int getLv() { return _lv; }
	void setLv(int lv) { _lv = lv; }

	int getMaxAtk() { return _maxAtk; }
	void setMaxAtk(int maxAtk) { _maxAtk = maxAtk; }

	int getMinAtk() { return _minAtk; }
	void setMinAtk(int minAtk) { _minAtk = minAtk; }
	//

	//Ÿ�ϼ���set
	void setTarget(D3DXVECTOR3 targetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), bool targetBool = false);	
	//get,set 
	void setCamera(camera* mainCamera) { _mainCamera = mainCamera; }			//ī�޶� ����
	void setScene(baseObject* scene) { _scene = scene; }						//�� ����
	//ĳ������ ��ǥ
	D3DXVECTOR3 getPlayerPosition() { return _meshInfo->getPlayerPosition(); }
	//ī�޶� ���� get,set
	transform* getCharacterPosition() { return _meshInfo->getCharacterPosition(); }
	void setCharacterPosition(transform* _cPos) { _meshInfo->setCharacterPosition(_cPos); }
	//�Ž� �޶��?
	meshInfo* getMesh() { return _meshInfo; }
	// UI �������̽� �޾ƿ���
	void getUIInterface(UI* ui) { _ui = ui; }
	//�갡������
	void setPet(Pet_cloud* pet) { _pet = pet; }
	//UI�� �� get set
	map<int, int> getPlayerSkillIcon() { return _mPlayerSkillIcon; }
	void setPlayerSkillIcon(int key, int value) { _mPlayerSkillIcon.find(key)->second = value; }
	//��ų get set
	int getPlayerSkillCount() { return playerSkillCount; }
	void setPlayerSkillCount(int skillCount) { playerSkillCount = skillCount; }
	//�״°� get
	bool getPlayerDie(void) { return _bDie; }
	//���� get
	int getPlayerMainAtkCount() { return playerMainAtkCount; }
	//���ư� get set
	bool getPlayerIsDown() { return _isDown; }
	void setPlayerIsDown(bool isDown) { _isDown = isDown; }
	//����
	void setPlayerIsStiffen(bool isStiffen) { _isStiffen = isStiffen; }
	//������
	skill* getSkill() { return _playerSkill; }
};


