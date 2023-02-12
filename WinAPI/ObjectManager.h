#pragma once
#include "SingletonBase.h"
#include "Player.h"
class ObjectManager : public SingletonBase<ObjectManager>
{
private:
	vector<Object*> m_vObject;
	vector<Object*> m_vtmpObject;
	Player* _player = nullptr;

public:
	void Init();
	void Update();
	void Render();
	void Release();

	void PlayerInit()
	{
		_player->Init();
	};

	Player* GetPlayer() { return _player; }

	template<class T>
	T* AddObject()
	{
		T* obj = new T;
		obj->Init();
		m_vtmpObject.push_back(obj);
		return obj;
	};

	template<class T>
	void AddObject(T* obj)
	{
		m_vtmpObject.push_back(obj);
	}
};

