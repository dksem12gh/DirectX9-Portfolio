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
	skill *	_playerSkill;			//플레이어 스킬
	meshInfo* _meshInfo;			//메쉬
	camera*	_mainCamera;			//메인카메라(씬에서 받아옴)
	baseObject*	_scene;				//맵을 받아오는 변수(씬에서 받아옴)
	UI*			_ui;				//UI
	Pet_cloud*  _pet;				//불러올 펫
	
	Ray			_moveRay;			//이동에 사용할 레이
	Ray _heightRay;					//높이 판별용 레이
	
	D3DXVECTOR3 _movePos;			//이동할 좌표
	D3DXVECTOR3 _targetPos;			//타켓 몬스터의 좌표
	billboardProgress* _hpBar;

	map<int, int> _mPlayerSkillIcon;	//플레이어 스킬 아이콘 map
			
	char* _name;					//이름

	int _maxHp;						//최대생명력
	int _curHp;						//현재생명력
	int _maxMp;						//최대마나
	int _curMp;						//현재마나
	int _maxSt;						//최대스테미너
	int _curSt;						//현재스테미너
	int _str;						//힘(체력)
	int _intellect;					//지력
	int _dex;						//솜씨
	int _will;						//의지
	int _luck;						//행운
	int _lv;						//레벨
	int _maxAtk;					//최대공격력
	int _minAtk;					//최소공격력
	int _criticalChance;			//크리티컬 확률
	int _defense;                   //방어 
	int _protect;                   //보호

	int playerMotionCount;			//플레이어의 모션
	int playerSkillCount;			//플레이어의 스킬
	int playerStateCount;			//플레이어의 상태
	int playerMainAtkCount;			//플레이어의 기본공격 
	
	float _atkTimer;					//플레이어의 기본공격 타이머
	float _moveSpeed;				//이동속도
	float _flyHeight;					//비행 높이용 변수
	float _movedistance;				//이동좌표까지 남은거리
	float _hyusicTimer;				//휴식시 회복타이머

	bool _battleMode;				//전투모드구별
	bool _ridingMode;				//탑승모드구별
	bool _flyMode;					//비행모드구별
	bool _isStiffen;				//경직 구별
	bool _moveState;				//이동하는지 안하는지
	bool _targetBool;				//타켓이 있는지 없는지 체크
	bool _bDie;						//플레이어가 죽었는지 체크
	bool _isDown;					//플레이어가 날라가는지 체크
	bool _damageDelay;				//데미지 딜레이

	//캐릭터의 모션
	enum playerMotion
	{
		non,
		idle,						//기본 서있기
		battleIdle,					//전투 서있기
		run,						//기본 달리기
		battleRun,					//전투 달리기
		hit,						//맞기
		nuckDown,					//날아감
		die							//죽어있음
	};

	//캐릭터의 스킬모션(UI포함)
	enum playerSkill
	{
		skillNon,					//기본
		smash,						//스매쉬
		defens,						//디팬스
		counter,					//카운터
		windmill,					//윈드밀
		hyusic,						//휴식
		temp						//NULL값
	};

	//캐릭터의 상태
	enum PlayerState
	{
		stateNon,					//아무것도 안할때
		atkState,					//공격
		riging						//탑승
	};

	//캐릭터의 기본공격
	enum playerMainAtk
	{
		atkNon,						//아무것도 없음
		atk01,						//기본공격01
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

	//키 메인 프로시져
	void playerAtkKey();			//공격시
	void playerMoveKey();			//이동시
	void playerUi();				//플레이어 기본 키(UI)
	
	//atk!
	void playerAtkSkillIcon();		//플레이어의 스킬 단축키
	void playerAtkSkill();			//플레이어의 스킬 
	int playerCritical();			//플레이어 크리티컬 데미지
	//hit!
	void playerHit(int state, int damage);		//플레이어가 맞는 함수
	void playerDie();				//플레이어가 죽는 함수
	void playerDown();				//플레이어가 날라가는 함수
	//지형
	void playerheight();			//높이조절
	void playerMove();				//캐릭터 이동
	void playerMoveStop();			//플레이어의 지형충돌시 멈추는 함수
	//펫
	void petRiding();				//펫탑승
	void petFlyMoveKey();			//펫탑승후 비행시 이동
	//레벨업
	void playerLvUp();


	//캐릭터의 스텟 get, set
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

	//타켓설정set
	void setTarget(D3DXVECTOR3 targetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), bool targetBool = false);	
	//get,set 
	void setCamera(camera* mainCamera) { _mainCamera = mainCamera; }			//카메라 셋팅
	void setScene(baseObject* scene) { _scene = scene; }						//맵 셋팅
	//캐릭터의 좌표
	D3DXVECTOR3 getPlayerPosition() { return _meshInfo->getPlayerPosition(); }
	//카메라를 위한 get,set
	transform* getCharacterPosition() { return _meshInfo->getCharacterPosition(); }
	void setCharacterPosition(transform* _cPos) { _meshInfo->setCharacterPosition(_cPos); }
	//매쉬 달라고?
	meshInfo* getMesh() { return _meshInfo; }
	// UI 인터페이스 받아오기
	void getUIInterface(UI* ui) { _ui = ui; }
	//펫가져오기
	void setPet(Pet_cloud* pet) { _pet = pet; }
	//UI로 뺄 get set
	map<int, int> getPlayerSkillIcon() { return _mPlayerSkillIcon; }
	void setPlayerSkillIcon(int key, int value) { _mPlayerSkillIcon.find(key)->second = value; }
	//스킬 get set
	int getPlayerSkillCount() { return playerSkillCount; }
	void setPlayerSkillCount(int skillCount) { playerSkillCount = skillCount; }
	//죽는거 get
	bool getPlayerDie(void) { return _bDie; }
	//상태 get
	int getPlayerMainAtkCount() { return playerMainAtkCount; }
	//날아감 get set
	bool getPlayerIsDown() { return _isDown; }
	void setPlayerIsDown(bool isDown) { _isDown = isDown; }
	//경직
	void setPlayerIsStiffen(bool isStiffen) { _isStiffen = isStiffen; }
	//데뮈지
	skill* getSkill() { return _playerSkill; }
};


