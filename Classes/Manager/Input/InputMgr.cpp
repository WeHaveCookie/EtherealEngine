#include "stdafx.h"
#include "InputMgr.h"
#include "EtherealDreamManagers.h"
#include "Manager/Render/RenderMgr.h"
#include "imgui.h"

int KeyToSFML[] = {
	-1, // none
	sf::Keyboard::A, // kbA
	sf::Keyboard::B, // kbB
	sf::Keyboard::C, // kbC
	sf::Keyboard::D, // kbD
	sf::Keyboard::E, // kbE
	sf::Keyboard::F, // kbF
	sf::Keyboard::G, // kbG
	sf::Keyboard::H, // kbH
	sf::Keyboard::I, // kbI
	sf::Keyboard::J, // kbJ
	sf::Keyboard::K, // kbK
	sf::Keyboard::L, // kbL
	sf::Keyboard::M, // kbM
	sf::Keyboard::N, // kbN
	sf::Keyboard::O, // kbO
	sf::Keyboard::P, // kbP
	sf::Keyboard::Q, // kbQ
	sf::Keyboard::R, // kbR
	sf::Keyboard::S, // kbS
	sf::Keyboard::T, // kbT
	sf::Keyboard::U, // kbU
	sf::Keyboard::V, // kbV
	sf::Keyboard::W, // kbW
	sf::Keyboard::X, // kbX
	sf::Keyboard::Y, // kbY
	sf::Keyboard::Z, // kbZ
	sf::Keyboard::Right, // kbCrossRight
	sf::Keyboard::Up, // kbCrossUp
	sf::Keyboard::Left, // kbCrossLeft
	sf::Keyboard::Down, // kbCrossDown
	sf::Keyboard::LControl, // kbLeftCtlr
	sf::Keyboard::RControl, // kbRightCtlr
	sf::Keyboard::LAlt, // kbLeftAlt
	sf::Keyboard::RAlt, // kbRightAlt
	sf::Keyboard::LShift, // kbLeftShift
	sf::Keyboard::RShift, // kbRightShift
	sf::Keyboard::LSystem, // kbLeftSystem
	sf::Keyboard::RSystem, // kbRightSystem
	sf::Keyboard::Return, // kbReturn
	sf::Keyboard::Space, // kbSpace
	sf::Keyboard::Numpad0, // kbNumpad0
	sf::Keyboard::Numpad1, // kbNumpad1
	sf::Keyboard::Numpad2, // kbNumpad2
	sf::Keyboard::Numpad3, // kbNumpad3
	sf::Keyboard::Numpad4, // kbNumpad4
	sf::Keyboard::Numpad5, // kbNumpad5
	sf::Keyboard::Numpad6, // kbNumpad6
	sf::Keyboard::Numpad7, // kbNumpad7
	sf::Keyboard::Numpad8, // kbNumpad8
	sf::Keyboard::Numpad9, // kbNumpad9
	sf::Keyboard::Num0, // kbNum0
	sf::Keyboard::Num1, // kbNum1
	sf::Keyboard::Num2, // kbNum2
	sf::Keyboard::Num3, // kbNum3
	sf::Keyboard::Num4, // kbNum4
	sf::Keyboard::Num5, // kbNum5
	sf::Keyboard::Num6, // kbNum6
	sf::Keyboard::Num7, // kbNum7
	sf::Keyboard::Num8, // kbNum8
	sf::Keyboard::Num9, // kbNum9
	sf::Keyboard::RBracket, // kbNum10
	sf::Keyboard::Equal, // kbNum11
	sf::Keyboard::BackSpace, // kbBackSpace
	sf::Keyboard::Escape, // kbEscape 
	sf::Keyboard::Slash, // kbSlash
	sf::Keyboard::Multiply, // kbMultiply
	sf::Keyboard::Subtract, // kbSubstract
	sf::Keyboard::Add, // kbAdd
	sf::Keyboard::Divide, // kbDivide
	sf::Keyboard::Period, // kbDot
	sf::Keyboard::Comma, // kbComma
	sf::Keyboard::SemiColon, // kbSemicolon
	sf::Keyboard::F1, // kbF1
	sf::Keyboard::F2, // kbF2
	sf::Keyboard::F3, // kbF3
	sf::Keyboard::F4, // kbF4
	sf::Keyboard::F5, // kbF5
	sf::Keyboard::F6, // kbF6
	sf::Keyboard::F7, // kbF7
	sf::Keyboard::F8, // kbF8
	sf::Keyboard::F9, // kbF9
	sf::Keyboard::F10, // kbF10
	sf::Keyboard::F11, // kbF11
	sf::Keyboard::F12, // kbF12
	sf::Keyboard::End, // kbEnd
	sf::Keyboard::Insert, // kbInsert
	sf::Keyboard::Delete, // kbDelete
	

	sf::Mouse::Button::Right, // mouseRight
	sf::Mouse::Button::Left, // mouseLeft
	sf::Mouse::Button::Middle, // mouseWheelButton
	sf::Mouse::Wheel::VerticalWheel, // mouseWheel
	sf::Mouse::Button::XButton1, // mouseBack
	sf::Mouse::Button::XButton2, //mouseForward

	0, // padButtonA
	1, // padButtonB
	2, // padButtonX
	3, // padButtonY
	4, // padLB
	5, // padRB
	6, // padBack
	sf::Joystick::X, // padX - Left joy horizontal axis -Left / +Right
	sf::Joystick::Y, // padY - Left joy vertical axis -Up / +Down
	sf::Joystick::Z, // padZ : -RT / +LT 
	sf::Joystick::R, // padR - Right joy vertical axis -Up / +Down
	sf::Joystick::U, // padU - Right joy horizontal axis -Left / +Right
	sf::Joystick::V, // padV - Accelerometer ???
	sf::Joystick::PovX, // padPovX - Cross : -Left / +Right
	sf::Joystick::PovY, // padPovY - Cross : -Down / +Up
	7, // padStart
};

InputMgr::InputMgr()
:Manager(ManagerType::Enum::Input)
{
}

InputMgr::~InputMgr()
{

}

void InputMgr::init()
{
	for (int i = KeyType::startKbKey; i <= KeyType::endKbKey; i++)
	{
		KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
		Keyboard key;
		key.m_key = keyType;
		m_keyboard[keyType] = key;
	}

	for (int i = KeyType::startMouseKey; i <= KeyType::endMouseKey; i++)
	{
		KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
		Mouse key;
		key.m_key = keyType;
		key.m_hasValue =  (i >= KeyType::startValueMouseKey && i <= KeyType::endValueMouseKey);
		m_mouse[keyType] = key;
	}

	for (int j = 0; j < sf::Joystick::Count; j++)
	{
		m_pads.push_back(std::map<KeyType::Enum, Pad>());
		for (int i = KeyType::startPadKey; i <= KeyType::endPadKey; i++)
		{
			KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
			Pad key;
			key.m_key = keyType;
			key.m_hasValue = (i >= KeyType::startValuePadKey && i <= KeyType::endValuePadKey);
			m_pads[j][keyType] = key;
		}
	}
}

void InputMgr::process(const float dt)
{

	for (auto& keyboard : m_keyboard)
	{
		keyboard.second.m_lastPressed = keyboard.second.m_pressed;
	}

	for (auto& mouse : m_mouse)
	{
		mouse.second.m_lastPressed = mouse.second.m_pressed;
		mouse.second.m_lastValue = mouse.second.m_value;
	}

	for (auto& padID : m_pads)
	{
		for (auto& pad : padID)
		{
			pad.second.m_lastPressed = pad.second.m_pressed;
			pad.second.m_lastValue = pad.second.m_value;
		}
	}

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
		case sf::Keyboard::Escape:
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
		default:
			break;
		}
	}

	for (int i = KeyType::startKbKey; i <= KeyType::endKbKey; i++)
	{
		KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
		m_keyboard[keyType].m_pressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(KeyToSFML[keyType]));
		if (m_keyboard[keyType].m_pressed)
		{
			m_keyboard[keyType].m_timeSincePressed += dt;
		}
		else
		{
			m_keyboard[keyType].m_timeSincePressed = 0.0f;
		}
	}

	for (int i = KeyType::startMouseKey; i <= KeyType::endMouseKey; i++)
	{
		KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
		if (i >= KeyType::startValueMouseKey && i <= KeyType::endValueMouseKey)
		{
			// Not manage yet
		}
		else
		{
			m_mouse[keyType].m_pressed = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(KeyToSFML[keyType]));
		}
		if (m_mouse[keyType].m_pressed)
		{
			m_mouse[keyType].m_timeSincePressed += dt;
		}
		else
		{
			m_mouse[keyType].m_timeSincePressed = 0.0f;
		}
	}

	for (int padID = 0; padID < sf::Joystick::Count; padID++)
	{
		for (int i = KeyType::startPadKey; i <= KeyType::endPadKey; i++)
		{
			KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
			if (i >= KeyType::startValuePadKey && i <= KeyType::endValuePadKey)
			{
				m_pads[padID][keyType].m_value = sf::Joystick::getAxisPosition(padID, static_cast<sf::Joystick::Axis>(KeyToSFML[keyType]));
			}
			else
			{
				m_pads[padID][keyType].m_pressed = sf::Joystick::isButtonPressed(padID, KeyToSFML[keyType]);
			}
			if (m_pads[padID][keyType].m_pressed)
			{
				m_pads[padID][keyType].m_timeSincePressed += dt;
			}
			else
			{
				m_pads[padID][keyType].m_timeSincePressed = 0.0f;
			}
		}
	}
}

void InputMgr::end()
{

}

bool InputMgr::keyIsPressed(KeyType::Enum key, unsigned int id)
{
	if (key >= KeyType::startKbKey && key <= KeyType::endKbKey)
	{
		return m_keyboard[key].m_pressed;
	} else if (key >= KeyType::startMouseKey && key <= KeyType::endMouseKey)
	{
		return m_mouse[key].m_pressed;
	}
	else if (key >= KeyType::startPadKey && key <= KeyType::endPadKey)
	{
		return m_pads[id][key].m_pressed;
	}
	return false;
}

bool InputMgr::keyIsJustPressed(KeyType::Enum key, unsigned int id)
{
	if (key >= KeyType::startKbKey && key <= KeyType::endKbKey)
	{
		return !m_keyboard[key].m_lastPressed && keyIsPressed(key, id);
	}
	else if (key >= KeyType::startMouseKey && key <= KeyType::endMouseKey)
	{
		return !m_mouse[key].m_lastPressed && keyIsPressed(key, id);
	}
	else if (key >= KeyType::startPadKey && key <= KeyType::endPadKey)
	{
		return !m_pads[id][key].m_lastPressed && keyIsPressed(key, id);
	}
	return false;
}

bool InputMgr::keyIsReleased(KeyType::Enum key, unsigned int id)
{
	return !keyIsPressed(key, id);
}

bool InputMgr::keyIsJustReleased(KeyType::Enum key, unsigned int id)
{
	if (key >= KeyType::startKbKey && key <= KeyType::endKbKey)
	{
		return m_keyboard[key].m_lastPressed && keyIsReleased(key, id);
	}
	else if (key >= KeyType::startMouseKey && key <= KeyType::endMouseKey)
	{
		return m_mouse[key].m_lastPressed && keyIsReleased(key, id);
	}
	else if (key >= KeyType::startPadKey && key <= KeyType::endPadKey)
	{
		return m_pads[id][key].m_lastPressed && keyIsReleased(key, id);
	}
	return false;
}

float InputMgr::getPadKeyValue(KeyType::Enum key, unsigned int id)
{
	if (key >= KeyType::startMouseKey && key <= KeyType::endMouseKey)
	{
		if (m_mouse[key].m_hasValue)
		{
			return m_mouse[key].m_value;
		}
	}
	else if (key >= KeyType::startPadKey && key <= KeyType::endPadKey)
	{
		if (m_pads[id][key].m_hasValue)
		{
			return m_pads[id][key].m_value;
		}
	}
	return 0.0f;
}

float InputMgr::getPadKeyLastValue(KeyType::Enum key, unsigned int id)
{
	if (key >= KeyType::startMouseKey && key <= KeyType::endMouseKey)
	{
		if (m_mouse[key].m_hasValue)
		{
			return m_mouse[key].m_lastValue;
		}
	}
	else if (key >= KeyType::startPadKey && key <= KeyType::endPadKey)
	{
		if (m_pads[id][key].m_hasValue)
		{
			return m_pads[id][key].m_lastValue;
		}
	}
	return 0.0f;
}

float InputMgr::getTimeSinceKeyPressed(KeyType::Enum  key, unsigned int id)
{
	if (key >= KeyType::startKbKey && key <= KeyType::endKbKey)
	{
		return m_keyboard[key].m_timeSincePressed;
	}
	else if (key >= KeyType::startMouseKey && key <= KeyType::endMouseKey)
	{
		return m_mouse[key].m_timeSincePressed;
	}
	else if (key >= KeyType::startPadKey && key <= KeyType::endPadKey)
	{
		return m_pads[id][key].m_timeSincePressed;
	}
	return 0.0f;
}