#include "stdafx.h"
#include "InputMgr.h"
#include "EtherealDreamManagers.h"
#include "Manager/Render/RenderMgr.h"

InputMgr::InputMgr()
:Manager(ManagerType::Enum::Input)
{
}

InputMgr::~InputMgr()
{

}

void InputMgr::init()
{

}

void InputMgr::process(const float dt)
{
	sf::Event event;
	auto renderMgr = RENDER_MGR;
	auto rdrWin = renderMgr->getMainRenderWindow();
	while (rdrWin->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			rdrWin->close();
			break;
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				rdrWin->close();
				break;
			default:
				break;
			}
		}
		break;
		case sf::Event::MouseButtonPressed:
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				break;
			case sf::Mouse::Right:
				break;
			default:
				break;
			}
		}
		break;
		default:
			break;
		}
	}
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = dt;
// 	io.MousePos = mouse_pos;
// 	io.MouseDown[0] = mouse_button_0;
// 	io.MouseDown[1] = mouse_button_1;
// 	io.KeysDown[i] = ...
}

void InputMgr::end()
{

}