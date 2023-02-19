#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"

Scene* SceneManager::_currentScene = nullptr;
Scene* SceneManager::_loadingScene = nullptr;
Scene* SceneManager::_readyScene = nullptr;

DWORD CALLBACK loadingThread(LPVOID prc)
{
    SceneManager::_readyScene->Init();
    SceneManager::_currentScene = SceneManager::_readyScene;

    SceneManager::_loadingScene->Release();
    SceneManager::_loadingScene = nullptr;
    SceneManager::_readyScene = nullptr;

    return 0;
}

HRESULT SceneManager::Init(void)
{
    _currentScene = nullptr;
    _loadingScene = nullptr;
    _readyScene = nullptr;
    
    _camera = new Camera;
    _camera->Init();

    return S_OK;
}

void SceneManager::Release(void)
{
    mapSceneIter miSceneList = _mSceneList.begin();
    for (; miSceneList != _mSceneList.end(); )
    {
        if (miSceneList->second != nullptr)
        {
            if (miSceneList->second == _currentScene)
            {
                miSceneList->second->Release();
            }
            SAFE_DELETE(miSceneList->second);
            miSceneList = _mSceneList.erase(miSceneList);
        }
        else ++miSceneList;
    }//end for

    /*
    * °°Àº±â´É
    for each (auto scene in _mSceneList)
    {
        if (scene.second != nullptr)
        {
            scene.second->Release();
        }
        SAFE_DELETE(scene.second);
    }
    */
}

void SceneManager::Update(void)
{
    if (_currentScene) _currentScene->Update();
    _camera->Update();
}

void SceneManager::Render(void)
{
    if (_currentScene) _currentScene->Render();
}

Scene* SceneManager::addScene(string sceneName, Scene* scene)
{
    if(!scene) return nullptr;

    _mSceneList.insert(make_pair(sceneName, scene));
    return scene;
}

Scene* SceneManager::addLoadingScene(string loadingSceneName, Scene* scene)
{
    if (!scene) return nullptr;

    _mSceneList.insert(make_pair(loadingSceneName, scene));
    return scene;
}

HRESULT SceneManager::changeScene(string SceneName, bool startPosSetting, D2D1_POINT_2F startPos)
{
    mapSceneIter find = _mSceneList.find(SceneName);
    
    if (find->second == _currentScene) return S_OK;
    if (find == _mSceneList.end()) return E_FAIL;

    if (SUCCEEDED(find->second->Init()))
    {
        _currentScene = find->second;
        if (startPosSetting)
        {
            PLAYER->SetPosition(startPos);
        }
        else
        {
            PLAYER->SetPosition(_currentScene->GetPlayerStartPos());
        }
        _camera->SetMapSize(_currentScene->GetMapWidth(), _currentScene->GetMapHeight());
        cout << "¸Ê ¹Ù²Ù±â. »õ ¸Ê »çÀÌÁî : "<< _currentScene->GetMapWidth() <<", "<< _currentScene->GetMapHeight() <<endl;
        return S_OK;
    }
    else return E_FAIL;
}

float SceneManager::GetCurrentSceneWidth()
{
    if (_currentScene == nullptr)
    {
        cout << "¾À »ý¼º Àü. " << endl;
        return NULL;
    }
     return _currentScene->GetMapWidth();
}
float SceneManager::GetCurrentSceneHeight()
{
    if (_currentScene == nullptr)
    {
        cout << "¾À »ý¼º Àü. " << endl;
        return NULL;
    }
    return _currentScene->GetMapHeight();
}
int SceneManager::GetCurrentSceneTileMatSpec(int latitude, int longitude)
{
    return _currentScene->GetMattileSpec(latitude, longitude);
}

COLORREF** SceneManager::GetCurrentScenePixel()
{
    if (_currentScene != nullptr)
        return _currentScene->GetPixelList();
    else return nullptr;
};
