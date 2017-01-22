#include "stdafx.h"
#include "InputMgr.h"
#include "EtherealEngineManagers.h"
#include "Manager/Render/RenderMgr.h"
#include "Manager/File/FileMgr.h"
#include "Manager/Action/CommandMgr.h"
#include "Manager/Game/GameMgr.h"
#include "Actions/Command.h"
#include "../../External/rapidjson/document.h"
#include "Gesture/GestureRecognition.h"
#include "Manager/Level/LevelMgr.h"


InputMgr* InputMgr::s_singleton = NULL;

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
	sf::Keyboard::Right, // kbRight
	sf::Keyboard::Up, // kbUp
	sf::Keyboard::Left, // kbLeft
	sf::Keyboard::Down, // kbDown
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

std::vector<const char*> KeyTypeToString = {
	"none",
	"kbA",
	"kbB",
	"kbC",
	"kbD",
	"kbE",
	"kbF",
	"kbG",
	"kbH",
	"kbI",
	"kbJ",
	"kbK",
	"kbL",
	"kbM",
	"kbN",
	"kbO",
	"kbP",
	"kbQ",
	"kbR",
	"kbS",
	"kbT",
	"kbU",
	"kbV",
	"kbW",
	"kbX",
	"kbY",
	"kbZ",
	"kbRight",
	"kbUp",
	"kbLeft",
	"kbDown",
	"kbLeftCtlr",
	"kbRightCtlr",
	"kbLeftAlt",
	"kbRightAlt",
	"kbLeftShift",
	"kbRightShift",
	"kbLeftSystem",
	"kbRightSystem",
	"kbReturn",
	"kbSpace",
	"kbNumpad0",
	"kbNumpad1",
	"kbNumpad2",
	"kbNumpad3",
	"kbNumpad4",
	"kbNumpad5",
	"kbNumpad6",
	"kbNumpad7",
	"kbNumpad8",
	"kbNumpad9",
	"kbNum0",
	"kbNum1",
	"kbNum2",
	"kbNum3",
	"kbNum4",
	"kbNum5",
	"kbNum6",
	"kbNum7",
	"kbNum8",
	"kbNum9",
	"kbNum10",
	"kbNum11",
	"kbBackSpace",
	"kbEscape",
	"kbSlash",
	"kbMultiply",
	"kbSubstract",
	"kbAdd",
	"kbDivide",
	"kbDot",
	"kbComma",
	"kbSemicolon",
	"kbF1",
	"kbF2",
	"kbF3",
	"kbF4",
	"kbF5",
	"kbF6",
	"kbF7",
	"kbF8",
	"kbF9",
	"kbF10",
	"kbF11",
	"kbF12",
	"kbEnd",
	"kbInsert",
	"kbDelete",

	"mouseRight",
	"mouseLeft",
	"mouseWheelButton",
	"mouseWheel",
	"mouseBack",
	"mouseForward",

	"padButtonA",
	"padButtonB",
	"padButtonX",
	"padButtonY",
	"padLB",
	"padRB",
	"padBack",
	"padX",
	"padY",
	"padZ",
	"padR",
	"padU",
	"padV",
	"padPovX",
	"padPovY",
	"padStart",
};

void InputMgr::Key::executeCommand(uint32_t id)
{
	if (m_command != NULL)
	{
		auto exeType = m_command->getExeType();
		if (!m_lastPressed && m_pressed && exeType == CommandExeType::JustPressed ||
			m_lastPressed && !m_pressed && exeType == CommandExeType::JustReleased ||
			m_pressed && exeType == CommandExeType::Pressed ||
			!m_pressed && exeType == CommandExeType::Released ||
			m_hasValue && (m_value >= 30.0f || m_value <= -30.0f) ||
			exeType == CommandExeType::AtOnce)
		{
			auto gameMgr = GameMgr::getSingleton();
			if (m_command->isInit())
			{
				CommandMgr::getSingleton()->addCommand(m_command);
			}
			if (gameMgr->getNumberPlayers() > 0)
			{
				if (m_hasValue)
				{
					float value = m_value * 0.1f;
					m_command->init(gameMgr->getEntityPlayer(id), (void*)&value);
				}
				else
				{
					m_command->init(gameMgr->getEntityPlayer(id));
				}
				CommandMgr::getSingleton()->addCommand(m_command);
			}
		}
	}
}

void InputMgr::Key::showInfo()
{
	m_showInfo = !m_showInfo;
}

void InputMgr::Key::displayWindow()
{
	if (m_showInfo)
	{
		std::string label = "Key - " + std::string(KeyTypeToString[m_key]) + "###" + std::to_string(m_padID) + std::string(KeyTypeToString[m_key]);
		if (ImGui::Begin(label.c_str(), &m_showInfo, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Pressed : %s", (m_pressed) ? "True" : "False");
			ImGui::Text("JustPressed : %s", (!m_lastPressed && m_pressed) ? "True" : "False");
			ImGui::Text("Released : %s", (!m_pressed) ? "True" : "False");
			ImGui::Text("Just Released : %s", (m_lastPressed && !m_pressed) ? "True" : "False");
			ImGui::Text("Time since pressed : %f", m_timeSincePressed);
			if (m_hasValue)
			{
				ImGui::Text("Key Value : %f", m_value);
			}
			ImGui::Separator();

			int commandLabelsSize;
			char** commandLabels = CommandMgr::getSingleton()->getCommandsLabel(&commandLabelsSize);
			int lastID = m_commandID;
			ImGui::Combo("Command", &m_commandID, (const char**)commandLabels, commandLabelsSize);
			if (lastID != m_commandID)
			{
				free(m_command);
				m_command = CommandMgr::getSingleton()->getCommand(m_commandID);
			}
			for (int i = 0; i < commandLabelsSize; i++)
			{
				free(commandLabels[i]);
			}
			free(commandLabels);

		}
		ImGui::End();
	}
}

KeyType::Enum			m_key;
bool					m_pressed;
bool					m_lastPressed;
float					m_timeSincePressed;
Command*				m_command;
bool					m_hasValue;
float					m_value;
float					m_lastValue;
int						m_commandID;
bool					m_showInfo;
int						m_padID;

InputMgr::InputMgr()
:Manager(ManagerType::Enum::Input)
{
	s_singleton = this;
	m_updateWhenNoFocus = false;
	m_lockInput = false;
	m_gestureRecognition = new GestureRecognition();
}

InputMgr::~InputMgr()
{
	for (int i = 0; i < KeyType::endPadKey + 1; i++)
	{
		free(m_keyName[i]);
	}
	SetVibrations(0, 0);
}

void InputMgr::init()
{
	m_gestureRecognition->init();
	for (int i = 0; i < KeyType::endPadKey + 1; i++)
	{
 		m_keyName[i] = (char*)malloc(sizeof(char)*(strlen(KeyTypeToString[i])+1));
 		strcpy(m_keyName[i], KeyTypeToString[i]);
	}

	m_displayNullKey = false;
	sf::Joystick::update();
	char* json;
	int sizeRead;
	FileMgr::ReadFile("Data/Config/input.json", (void**)&json, &sizeRead);

	json[sizeRead] = 0;

	rapidjson::Document document;
	document.Parse((char*)json);
	auto error = document.HasParseError();
	auto object = document.IsObject();
	assert(!error);
	assert(object);

	for (int i = KeyType::startKbKey; i <= KeyType::endKbKey; i++)
	{
		KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
		Key key;
		key.m_key = keyType;
		
		if (document.HasMember(KeyTypeToString[keyType]))
		{
			std::string cmd = "Command";
			cmd += document[KeyTypeToString[keyType]].GetString();
			key.m_command = CommandMgr::getSingleton()->getCommand(cmd.c_str(), &key.m_commandID);
		}
		
		m_keyboard[keyType] = key;

	}

	for (int i = KeyType::startMouseKey; i <= KeyType::endMouseKey; i++)
	{
		KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
		Key key;
		key.m_key = keyType;
		key.m_hasValue =  (i >= KeyType::startValueMouseKey && i <= KeyType::endValueMouseKey);

		if (document.HasMember(KeyTypeToString[keyType]))
		{
			std::string cmd = "Command";
			cmd += document[KeyTypeToString[keyType]].GetString();
			key.m_command = CommandMgr::getSingleton()->getCommand(cmd.c_str(), &key.m_commandID);
		}

		m_mouse[keyType] = key;
	}

	for (int j = 0; j < sf::Joystick::Count; j++)
	{
		m_pads.push_back(std::map<KeyType::Enum, Key>());
		m_padsStatus.push_back(sf::Joystick::isConnected(j));
		for (int i = KeyType::startPadKey; i <= KeyType::endPadKey; i++)
		{
			KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
			Key key;
			key.m_key = keyType;
			key.m_hasValue = (i >= KeyType::startValuePadKey && i <= KeyType::endValuePadKey);
			key.m_padID = j;

			if (document.HasMember(KeyTypeToString[keyType]))
			{
				std::string cmd = "Command";
				cmd += document[KeyTypeToString[keyType]].GetString();
				key.m_command = CommandMgr::getSingleton()->getCommand(cmd.c_str(), &key.m_commandID);
			}

			m_pads[j][keyType] = key;
		}
	}
	FileMgr::CloseFile(json);
}

void InputMgr::process(const float dt)
{
	if (RenderMgr::getSingleton()->getMainRenderWindow()->hasFocus() || m_updateWhenNoFocus)
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
		m_lastMousePosition = getMousePosition();

		sf::Event event;
		auto renderMgr = RENDER_MGR;
		auto rdrWin = renderMgr->getMainRenderWindow();
		while (rdrWin->pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
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
					LevelMgr::getSingleton()->loadLevel("Data/Level/mainMenu.json");
					break;
				default:
					break;
				}
			}
			break;
			case sf::Event::MouseMoved:
				m_currentMousePosition.x = event.mouseMove.x;
				m_currentMousePosition.y = event.mouseMove.y;
				break;
			case sf::Event::JoystickConnected:
				m_padsStatus[event.joystickConnect.joystickId] = true;
				break;
			case sf::Event::JoystickDisconnected:
				m_padsStatus[event.joystickConnect.joystickId] = false;
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
			if (!m_lockInput)
			{
				m_keyboard[keyType].executeCommand();
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
			if (!m_lockInput)
			{
				m_mouse[keyType].executeCommand();
			}
		}

		for (int padID = 0; padID < sf::Joystick::Count; padID++)
		{
			if (m_padsStatus[padID])
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
					if (!m_lockInput)
					{
						m_pads[padID][keyType].executeCommand(padID);
					}
				}
			}
		}
	}
	ImGui::SFML::Update(dt);
	m_gestureRecognition->process(dt);
	if (m_gestureRecognition->getGesture() != ShootType::None)
	{
		CommandMgr::getSingleton()->addCommand(m_gestureRecognition->getGestureCommand());
	}
}

void InputMgr::end()
{

}

Command* InputMgr::getKeyCommand(KeyType::Enum key, uint32_t id)
{
	if (key >= KeyType::startKbKey && key <= KeyType::endKbKey)
	{
		return m_keyboard[key].m_command;
	}
	else if (key >= KeyType::startMouseKey && key <= KeyType::endMouseKey)
	{
		return m_mouse[key].m_command;
	}
	else if (key >= KeyType::startPadKey && key <= KeyType::endPadKey)
	{
		return m_pads[id][key].m_command;
	}
	return NULL;
}

const bool InputMgr::keyIsPressed(KeyType::Enum key, uint32_t id)
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

const bool InputMgr::keyIsJustPressed(KeyType::Enum key, uint32_t id)
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

const bool InputMgr::keyIsReleased(KeyType::Enum key, uint32_t id)
{
	return !keyIsPressed(key, id);
}

const bool InputMgr::keyIsJustReleased(KeyType::Enum key, uint32_t id)
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

const float InputMgr::getPadKeyValue(KeyType::Enum key, uint32_t id)
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

const float InputMgr::getPadKeyLastValue(KeyType::Enum key, uint32_t id)
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

const float InputMgr::getTimeSinceKeyPressed(KeyType::Enum  key, uint32_t id)
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

void InputMgr::showImGuiWindow(bool* window)
{

	if (ImGui::Begin("InputMgr", window))
	{
		ImGui::Checkbox("Update when no focus", &m_updateWhenNoFocus);
		ImGui::Checkbox("Lock Input", &m_lockInput);
		ImGui::Text("Gesture : %i", m_gestureRecognition->getGesture());
		if (ImGui::IsItemClicked())
		{
			m_gestureRecognition->showInfo();
		}
		ImGui::Text("Cursor pos : x = %f | y = %f", getMousePosition().x, getMousePosition().y);
		ImGui::Separator();
		int i = 0;
		
		if (ImGui::CollapsingHeader("Pad Status"))
		{
			for (auto& padStatus : m_padsStatus)
			{
				ImGui::Text("Pad %i - %s", i++, (padStatus) ? "Connected" : "Disconnected");
			}
		}
		ImGui::Separator();

		m_filter.Draw();
		if(ImGui::CollapsingHeader("Keyboard"))
		{
			ImGui::Checkbox("Display no binding key###KB", &m_displayNullKey);
			for (int i = KeyType::startKbKey; i <= KeyType::endKbKey; i++)
			{
				if (m_filter.PassFilter(m_keyName[i]))
				{
					KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
					if (m_keyboard[keyType].m_command != NULL)
					{
						ImGui::Text("%s - %s", KeyTypeToString[keyType], m_keyboard[keyType].m_command->getName());
						if (ImGui::IsItemClicked())
						{
							m_keyboard[keyType].showInfo();
						}
					}
					else if (m_displayNullKey)
					{
						ImGui::Text("%s", KeyTypeToString[keyType]);
						if (ImGui::IsItemClicked())
						{
							m_keyboard[keyType].showInfo();
						}
					}
					m_keyboard[keyType].displayWindow();
				}
			}
		}
		
		if (ImGui::CollapsingHeader("Pads"))
		{
			for (int padID = 0; padID < sf::Joystick::Count; padID++)
			{
				if (m_padsStatus[padID])
				{
					std::string label = "Pads " + std::to_string(padID);
					ImGui::Indent();
					if (ImGui::CollapsingHeader(label.c_str()))
					{
						for (int i = KeyType::startPadKey; i <= KeyType::endPadKey; i++)
						{
							if (m_filter.PassFilter(m_keyName[i]))
							{
								KeyType::Enum keyType = static_cast<KeyType::Enum>(i);
								if (m_pads[padID][keyType].m_command != NULL)
								{
									ImGui::Text("%s - %s", KeyTypeToString[keyType], m_pads[padID][keyType].m_command->getName());
									if (ImGui::IsItemClicked())
									{
										m_pads[padID][keyType].showInfo();
									}
								}
								else
								{
									ImGui::Text("%s", KeyTypeToString[keyType]);
									if (ImGui::IsItemClicked())
									{
										m_pads[padID][keyType].showInfo();
									}
								}
								m_pads[padID][keyType].displayWindow();
							}
						}
					}
					ImGui::Unindent();
				}
			}
		}
		
	}
	ImGui::End();
}

const Vector2 InputMgr::getMousePosition() const
{
	return m_currentMousePosition;
}

std::vector<uint32_t> InputMgr::getActivePads()
{
	std::vector<uint32_t> activePads;
	uint32_t id = 0;
	for (auto& padStatus : m_padsStatus)
	{
		if (padStatus)
		{
			activePads.push_back(id);
		}
		id++;
	}
	return activePads;
}

void InputMgr::SetVibrations(unsigned int Value, uint32_t padID)
{
	if (padIsActive(padID))
	{
		ZeroMemory(&this->m_status, sizeof(XINPUT_STATE));
		long int ValMax = 65535;
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wRightMotorSpeed = (Value / 100.0)*ValMax;
		vibration.wLeftMotorSpeed = (Value / 100.0)*ValMax;
		XInputSetState(padID, &vibration);
	}
}

const bool InputMgr::padIsActive(uint32_t padID) const
{
	return m_padsStatus[padID];
}

InputMgr::Key* InputMgr::getKeyByName(char* name)
{
	for (auto& key : m_keyboard)
	{
		if (strcmp(name, KeyTypeToString[key.first]) == 0)
		{
			return &key.second;
		}
	}

	for (auto& key : m_mouse)
	{
		if (strcmp(name, KeyTypeToString[key.first]) == 0)
		{
			return &key.second;
		}
	}

	int padID = 0;
	for (auto& pad : m_pads)
	{
		if (m_padsStatus[padID])
		{
			for (auto& key : pad)
			{
				if (strcmp(name, KeyTypeToString[key.first]) == 0)
				{
					return &key.second;
				}
			}
		}
		padID++;
	}
	return NULL;
}