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
	
	HRESULT changeScene(string SceneName);

	//로딩만 하는 누군가.
	//콜백이 곧 델리게이트다...
	//LPVOID : void* 어떤 타입으로도 변환 가능
	//friend 단방향참조
	friend DWORD CALLBACK loadingThread(LPVOID prc);

	float GetCurrentSceneWidth() ;
	float GetCurrentSceneHeight();
	int GetCurrentSceneTileMatSpec(int latitude, int longitude);
	Camera* GetCamera() { return _camera; }

	COLORREF** GetCurrentScenePixel();

	SceneManager() {};
	~SceneManager() {};
};

