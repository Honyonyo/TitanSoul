#pragma once
#pragma region	µ¿⁄¿Œ∆–≈œ + ΩÃ±€≈Ê

#pragma endregion

template<class T>
class SingletonBase
{
protected:
	static T* singleton;


	SingletonBase() {

	}
	~SingletonBase() {

	}
public:
	static T* getSingleton(void);
	void ReleaseSingleton(void);

};

template <typename T>
T* SingletonBase<T>::singleton = 0;

template <typename T>
T* SingletonBase<T>::getSingleton(void) {
	if (!singleton) { singleton = new T; }
	return singleton;
}
template<typename T>
void SingletonBase<T>::ReleaseSingleton(void) {
	if (singleton) {
		delete singleton;
		singleton = 0;
	}
}