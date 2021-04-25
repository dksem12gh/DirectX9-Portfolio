#pragma once
class Scene
{
private:
	baseObject* _scene;

public:
	HRESULT init(string filePath, string texturePath, D3DXMATRIXA16* pMatCorrection = NULL);
	HRESULT init(string filePaht, D3DXMATRIXA16* pmatCorrection = NULL);
	void release(void);
	void update(void);
	void render(void);

	baseObject* getSceneObejct() { return _scene; }

	Scene();
	~Scene();
};

