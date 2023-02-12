#include "stdafx.h"
#include "JsonSaveLoader.h"

HRESULT JsonSaveLoader::Init(void)
{
	return S_OK;
}

void JsonSaveLoader::Release(void)
{

}

void JsonSaveLoader::saveJsonFile(const char* fileName, Json::Value root)
{
	Json::StyledWriter writer;
	string outputConfig = writer.write(root);

	saveStringWithFileStream(fileName, outputConfig);
}

Json::Value JsonSaveLoader::loadJsonFile(const char* fileName)
{
	string jsonStr = loadStringWithFileStream(fileName);
	if (jsonStr == "")
	{
		MessageBox(_hWnd, "데이터가 없다.", "경고", MB_OK);
		return NULL;
	}

	Json::Reader reader;
	Json::Value root;

	//파싱 : 특정 문서를 읽어들여서 이를 다른 프로그램이나 서브 루틴이 사용할 수 있게 내부의 표현 방식으로 변환해주는 것
	//파서 : 파싱해주는애

	bool parsingRet = reader.parse(jsonStr, root);
	if (!parsingRet)
	{
		string errMsg = reader.getFormatedErrorMessages();
		MessageBox(_hWnd, errMsg.c_str(), "오류", MB_OK);
		PostQuitMessage(0);
	}

	return root;
}

void JsonSaveLoader::saveString(const char* fileName, string data)
{
	TCHAR* buf = (TCHAR*)data.c_str();

	//윈도우에서 부여한 고유 식별번호
	HANDLE file;
	WORD BOM = 0xFEFF;	//UTF-16
	DWORD dwBytes = 0;
	DWORD dwRead;

	/*
	* CreateFile 파일을 만들거나 읽어온다.
	*
	인자
	1. 생성하거나 열 파일의 이름
	2. 접근타입 (WRITE/READ)
	3. 열려진 파일의 공유 여부(0은 공유를 안하겠다.)
	4. 자식에게 상속여부
	5. 파일 생성모드 세팅
		CREATE_ALWAYS:새로운 파일 생성 ->이미 존재하면 덮어씌운다.
	6. 파일의 옵션값 설정
		읽기전용/쓰기전용 등등
		FILE_ATTRIBUTE_NORMAL : 일반적인 파일속성
	7.파일에 대한 속성값(주로 새로 만들시 동작) 특정 한 요소만 접근해서 정정하려면 null을 빼주면 된다

	*/

	file = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		MessageBox(_hWnd, "생성파일에러", "에러", MB_OK);
	}
	else
	{
		//인자 : CreateFile에서 할당된 핸들, 파일에 쓰고싶은 스트링 값, 얼마나 읽고 쓸 것인지에 대한 크기, 얼마나 읽고 쓸 것인지에 대한 반환값(주소)
		//오버라이드 WriteFile(file, &BOM,2, &dwBytes, NULL);
		WriteFile(file, buf, strlen(buf) * sizeof(TCHAR), &dwRead, NULL);
		CloseHandle(file);
	}
}

string JsonSaveLoader::loadString(const char* fileName)
{
	TCHAR buf[8192];
	ZeroMemory(buf, sizeof(buf));

	HANDLE file;
	DWORD dwBytes;
	//GENERIC_READ : 일반적인 읽기 형태
	//OPEN_EXISTING : 파일이 있으면 한 번에 연다.
	file = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		MessageBox(_hWnd, "생성파일에러", "에러", MB_OK);
		return "";
	}
	else
	{
		//인자 : CreateFile에서 할당된 핸들, 파일에 쓰고싶은 스트링 값, 얼마나 읽고 쓸 것인지에 대한 크기, 얼마나 읽고 쓸 것인지에 대한 반환값(주소)
		//오버라이드 WriteFile(file, &BOM,2, &dwBytes, NULL);
		ReadFile(file, buf, 8292, &dwBytes, NULL);
		CloseHandle(file);
	}


	return string(buf);
}

void JsonSaveLoader::saveStringWithFileStream(const char* fileName, string data)
{
	ofstream outFile(fileName);
	outFile << data << endl;

	outFile << data << endl;
	outFile.close();
}

string JsonSaveLoader::loadStringWithFileStream(const char* fileName)
{
	ifstream fileIn;
	/*
	ios::in 파일을 읽기만 가능한 모드로 저장하겠다.
	ios::out 쓰기만 가능한 모드로 저장하겠다.
	ios::ate 파일을 개방할 때 파일의 끝으로 파일 포인터를 이동시킨다.
	ios::app 해당 파일에 맨 끝에서부터 데이터를 추가한다.
	ios::trunc 파일이 있으면 해당 파일의 모든 데이터를 비우고 연다.
	ios::binary 바이너리 모드로 개방 (서버에서 사용)
	*/
	fileIn.open(fileName, ios::in);

	char data;
	string jsonStr;

	while (fileIn.get(data))
	{
		jsonStr += data;
	}
	fileIn.close();

	return jsonStr;
}

