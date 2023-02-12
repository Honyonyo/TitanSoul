#pragma once
#include "SingletonBase.h"

#pragma region Json
/*
구글 cpp json sourceforge 검색
뭐시기뭐시기 파일 3개를 열고
프로젝트/속성/구성관리자/다중스레드디버그MTD->MDD ->컨트롤F5
//빌드-vs71디버그 라이브러리 어쩌고저쩌고 안에 있는 제일 파일이름이 긴 파일을 챙긴다.
//링커 입력 추가종속성
#pragma comment(lib, "JsonCPP/lib_json/json_vc71_libmtd.lib")
#include "JsonCPP/include/json/json.h"
*/

/*
	▶BOM (Byte Of/Order Mark)
	바이트 순서 표시기
		WORD BOM = 0xFEFF;
	
	컴퓨터는 데이터를 메모리에 저장할 때 바이트단위로 나눠서 저장을 한다.
	하지만 유니코드의 경우 인코딩 방식이나 엔디안 방식에 따라 BOM이 필요한 경우가 있다.
	바이트가 저장되는 순서에 따라 빅 엔디안, 리틀 엔디안으로 나뉜다.
	 (윈도우가 사용하는 방식은 UTF-16 ( U+FEFF) -> BOM
	
	 - 빅 엔디안(Big-endian) 좌->우
		낮은 주소에 데이터의 높은 바이트부터 저장하는 방식
		메모리에 순차적으로 저장되기 때문에 그대로 읽을 수가 있다.
		(사람이 편하다.)

	 - 리틀 엔디안 좌<-우
		낮은 주소의 데이터에 낮은 바이트를 저장하는 방식.
		평소 사람이 읽는 방향의 반대로 읽어야하기 때문에 접근성이 떨어진다.

	보통 파일 제일 앞쪽에 0xFEFF를 바이너리 형식으로 채택해서 이후의 나올 문자나 문자열을 읽어들이게 하는 방식때문에 사용한다.
*/

/*
	제이슨(Json (JavaScript Object Notation)
	-제이슨은 객체의 구조와 속성을 텍스트 형식으로 전달하기 위해 고안된 방식이다.
		인터넷에서 자료를 주고받을 때 자료 표현방식으로 주로 이용되는 포맷
	-자료의 종류에 큰 제한은 없으며, 특히 컴퓨터 프로그래밍의 변수값을 표현하는데 적합하다.
	-제이슨의 형식은 자바스크립트의 구문 형식을 따르지만, 프로그래밍 언어나 플랫폼에 독립적이므로 C, C++, C#, Java, JavaScript, 펄, 파이썬, 루비 등등 많은 언어에서 사용되고있다.
	-XML, HTTP, CSV등등보다 "일반적인'환경에서 더 빠르다.
	
	*제이슨 형식에서 사용하는 기호의 의미는 다음과 같다.
	{} : 객체
	키 : 값 = 객체가 갖는 속성
	"" : 문자열 데이터
	[] : 배열

	Value root는 읽기 시작하는 곳
	BOM = Byte of Mark

	- asString()
	 : 변수가 string이 아니거나 null값이 들어있다면 별도의 예외 없이 그냥 NULL을 반환한다.

	- ToString()
	 : ToString의 변수가 NULL값이라면 예외 발생

*/
#pragma endregion

#define DATA_FILE_NAME "SaveData.json"
using namespace std;
class JsonSaveLoader : public SingletonBase<JsonSaveLoader>
{
private:
	void saveString(const char* fileName, string data);
	void saveStringWithFileStream(const char* fileName, string data);

	string loadString(const char* fileName);
	string loadStringWithFileStream(const char* fileName);
public:
	HRESULT Init(void);
	void Release(void);
	void saveJsonFile(const char* fileName, Json::Value root);	

	Json::Value loadJsonFile(const char* fileName);


};