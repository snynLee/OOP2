// OOP2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h> // itoa
#include <conio.h>
#include <cstring>
#include <windows.h>
#include <iostream>
#include <vector>

#include "Utils.h"
#include "Canvas.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "TextUI.h"


using namespace std;

/// 현재 모든 게임 객체들은 GameObject의 static vector<GameObject*> Objects를 통해 관리되고 있다.
/// vector<GameObject*> children은 Objects에 속한 객체의 자식 객체를 관리하는 벡터이다.
/// 문제는 부모 객체가 있는 게임 객체를 Objects에 등록한다는건데, 
/// 원래는 부모 객체가 있는 게임 객체(자식)는 children에 등록되는게 맞다.
/// 일단 부모 객체가 있는 게임 객체를 Objects에 등록하는 방법으로는 부모 객체에서 자식 객체를 생성할 때,
/// children 리스트에 추가함과 동시에 Objects에도 추가하는 방법이다. 이 경우, 다음과 같은 문제점이 있는데,
/// 첫째로 객체 중복의 문제다. 이미 children vector에 추가되었는데, Objects에도 추가하면 같은 객체에 대한 중복이 발생한다.
/// 둘째, 상속 관계 문제다. 원래는 부모 객체가 삭제되면, 자식 객체도 삭제가 되어야 하지만, 둘 다 같은 Objects에 존재한다면
/// 자식 객체가 삭제되지 않을 수도 있다는거다. 
/// 자식 객체가 IDamageable 인터페이스를 갖고 있다면 발생하는 문제에 대해 알아보면,
/// 현재 TextUI는 부모 객체 (Player, Enemy)가 IDamageable 인터페이스를 가지고 있어서 그들에게서 hp 정보를 받아
/// 화면에 출력하고 있다. (TextUI는 IDamageable 인터페이스를 가지고 있진 않은 상태이다.) 만약 TextUI가 IDamageable 인터페이스를
/// 갖게 된다면, 부모 객체와 자식 객체가 같은 인터페이스를 가지게 되는건데, 이건 매우 비효율적이다. 부모 객체에서만 해도 되는
/// IDamageable에 관한 기능들을 자식 객체에서도 구현되어 불필요한 일이 생긴다. 또한 둘 사이에 IDamageable에 대한 기능 충돌도
/// 발생할 수 있다. 이와 같이, 자식 객체와 부모 객체가 같은 인터페이스를 가지고 있는 건 문제가 된다.
/// 이 문제에 대한 대안으로는 일단 자식-부모가 같은 인터페이스를 갖지 않으면 되고, 굳이 가져야 한다면 기능을 분리하는게 좋다.
/// 결정적으로 이는 상속의 문제와 관련되어 있기 때문에, 상속 대신 합성(Composition)을 사용하여 해결하는게 맞다고 생각한다.
/// 즉, 상속보다는 자신의 상태를 나타내주는 멤버로서 표현하는게 적절한 것 같기 때문이다.
/// 합성을 사용하면 자식 객체는 부모 객체의 메서드를 호출하는 방식으로 동작하여
/// 자식 객체가 부모 객체랑 같은 Objects에 등록되어도 직접인 관계가 없고, 
/// 두 객체가 별도로 생성되고 소멸되어 상속에서의 문제가 발생하지 않는다.
/// 


int main()
{
	auto canvas = Canvas::GetInstance();
	GameObject::Init(30);

	bool exit_flag{ false };


	while (exit_flag == false) { // if player is alive

		canvas->clear();

		/* process game logic for each game object */
		GameObject::Update();
		canvas->update();

		/* draw game objects */
		GameObject::Draw();

		/* process input event. */
		if (_kbhit()) {
			char ch = _getch();

			switch (ch) {
			case 'q':
				exit_flag = true;
				break;
			case 'n':
				GameObject::Add(new Player{ "(^_^)", rand() % canvas->getScreenSize(), 50.0f });
				break;
			case 'e':
				GameObject::Add(new Enemy{ "(+*_*)", rand() % canvas->getScreenSize(), 50.0f, 10.0f / canvas->getFrameRate() });
				break;
			default:
				GameObject::ProcessInput(ch);
				break;
			}
		}
		canvas->render();

	} /* end of while(1) */

	GameObject::Destroy();


	return 0;
}
