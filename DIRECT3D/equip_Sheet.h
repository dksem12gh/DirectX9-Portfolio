#pragma once

namespace EQPART
{
	enum PART
	{
		HEAD = 0,
		BODY,
		HAND,
		FEET,
		MANTLE,
		NOEQUIP
	};
}


class equip_Sheet
{
private:
	typedef map<string, LPDIRECT3DTEXTURE9>* mapTexture;

private:
	baseObject* _object;	// 오브젝트
	EQPART::PART _parts;	// 장착 부위
	string _boneName;		// 본네임
public:
	HRESULT init(string filePath, string mtrlPath, D3DXMATRIXA16* matCorrection = NULL);
	void release(void);

	void setEquip(void);
	void setEquip(bool equip);

	EQPART::PART getParts(void) { return _parts; }
	void setParts(EQPART::PART parts) { _parts = parts; }


	equip_Sheet() : _parts(EQPART::PART::NOEQUIP), 
					_boneName(""), 
					_object(NULL) {};
	~equip_Sheet() {};
};

