#pragma once
#include "SingletonBase.h"

class Scene;
class Camera;
class SceneManager : public SingletonBase<SceneManager>
{
private:
	typedef map<string, Scene*>mapSceneList;
	typedef map<string, Scene*>::iterator mapSceneIter;

private:
	static Scene* _currentScene;
	static Scene* _loadingScene;
	static Scene* _readyScene;

	mapSceneList _mSceneList;
	mapSceneList _mLoadingSceneList;
	
	Camera* _camera;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(void);

	Scene* addScene(string sceneName, Scene* scene);
	Scene* addLoadingScene(string loadingSceneName, Scene* scene);
	
	HRESULT changeScene(string SceneName, bool startPosSetting= false, D2D1_POINT_2F startPos = {0,0});

	friend DWORD CALLBACK loadingThread(LPVOID prc);

	float GetCurrentSceneWidth() ;
	float GetCurrentSceneHeight();
	int GetCurrentSceneTileMatSpec(int latitude, int longitude);
	Camera* GetCamera() { return _camera; }

	COLORREF** GetCurrentScenePixel();

	SceneManager() {};
	~SceneManager() {};
};

