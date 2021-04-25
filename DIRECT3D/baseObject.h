#pragma once

class xMesh;
class skinnedAnimation;

class baseObject
{
public:
	xMesh*			_pMesh;			// ���� ����� �޽�
	transform*		_transform;		// Ʈ������
	boundBox		_boundBox;		// �浹�ڽ�
	bool			_ignoreCreateShadow;	// ������� ���鶧 ���õǳ�?
	skinnedAnimation*	_skinnedAnim;		//��Ų�� �ִϸ��̼�

protected:
	bool			_isActive;		//Ȱ��ȭ ����

public:
	virtual void update();
	virtual void render();

	// ������ �� �޽��� �����Ѵ�.
	void setMesh(xMesh*	pMesh);
	// Ȱ��ȭ ����
	void setActive(bool isActive);

	// Ȱ��ȭ ���θ� ��´�.
	bool getActive() { return _isActive; }

	// ���õ� �޽��� ���� �ٿ��ڽ��� ���� �Ѵ�.
	void computeBoundBox();

	// override�� �ʿ��ϸ� ������ �ؼ� ����ϱ�
	virtual void baseObjectEnable() {}			// BaseObject �� Ȱ��ȭ �ɶ� ����
	virtual void baseObjectDisable() {}			// BaseObject �� ��Ȱ��ȭ �ɶ� ����
	virtual void baseObjectUpdate() {}			// BaseObject �� Update �� ����
	virtual void baseObjectNoActiveUpdate() {}	// BaseObject �� ��Ȱ��ȭ�� ������Ʈ ����
	virtual void baseObjectRender();			// BaseObject �� �׸��� ����

	xMeshStatic* getStaticMesh() { return dynamic_cast<xMeshStatic*>(_pMesh); }
	xMeshSkinned* getSkinnedMesh() { return dynamic_cast<xMeshSkinned*>(_pMesh); }
	baseObject();
	baseObject(xMesh* pMesh)
	{
		this->setMesh(pMesh);
		this->setActive(true);
	}
	~baseObject();
};