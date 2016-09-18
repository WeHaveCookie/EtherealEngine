#pragma once

#include "Manager/Manager.h"


namespace KeyType
{
	enum Enum
	{
		none = 0,
		kbA,
		startKbKey = kbA,
		kbB,
		kbC,
		kbD,
		kbE,
		kbF,
		kbG,
		kbH,
		kbI,
		kbJ,
		kbK,
		kbL,
		kbM,
		kbN,
		kbO,
		kbP,
		kbQ,
		kbR,
		kbS,
		kbT,
		kbU,
		kbV,
		kbW,
		kbX,
		kbY,
		kbZ,
		kbRight,
		kbUp,
		kbLeft,
		kbDown,
		kbLeftCtlr,
		kbRightCtlr,
		kbLeftAlt,
		kbRightAlt,
		kbLeftShift,
		kbRightShift,
		kbLeftSystem,
		kbRightSystem,
		kbReturn,
		kbSpace,
		kbNumpad0,
		kbNumpad1,
		kbNumpad2,
		kbNumpad3,
		kbNumpad4,
		kbNumpad5,
		kbNumpad6,
		kbNumpad7,
		kbNumpad8,
		kbNumpad9,
		kbNum0,
		kbNum1,
		kbNum2,
		kbNum3,
		kbNum4,
		kbNum5,
		kbNum6,
		kbNum7,
		kbNum8,
		kbNum9,
		kbNum10,
		kbNum11,
		kbBackSpace,
		kbEscape,
		kbSlash,
		kbMultiply,
		kbSubstract,
		kbAdd,
		kbDivide,
		kbDot,
		kbComma,
		kbSemicolon,
		kbF1,
		kbF2,
		kbF3,
		kbF4,
		kbF5,
		kbF6,
		kbF7,
		kbF8,
		kbF9,
		kbF10,
		kbF11,
		kbF12,
		kbEnd,
		kbInsert,
		kbDelete,
		endKbKey = kbDelete,

		mouseRight,
		startMouseKey = mouseRight,
		mouseLeft,
		mouseWheelButton,
		mouseWheel,
		startValueMouseKey = mouseWheel,
		endValueMouseKey = mouseWheel,
		mouseBack,
		mouseForward,
		endMouseKey = mouseForward,

		padButtonA,
		startPadKey = padButtonA,
		padButtonB,
		padButtonX,
		padButtonY,
		padLB,
		padRB,
		padBack,
		padX, // Left joy horizontal axis
		startValuePadKey = padX,
		padY, // Left joy vertical axis
		padZ, // -RT / +LT 
		padR, // Right joy vertical axis -Up / +Down
		padU, // Right joy horizontal axis -Left / +Right
		padV, // Accelerometer ???
		padPovX, // Cross : -Left / +Right
		padPovY, // Cross : -Down / +Up
		endValuePadKey = padPovY,
		padStart,
		endPadKey = padStart	
	};
}

class InputMgr : public Manager
{
	struct Keyboard
	{
		
		KeyType::Enum	m_key;
		bool			m_pressed;
		bool			m_lastPressed;
		float			m_timeSincePressed;

		Keyboard()
		{
			m_key = KeyType::none;
			m_pressed = false;
			m_lastPressed = m_pressed;
		}
	};

	struct Mouse
	{
		KeyType::Enum	m_key;
		bool			m_hasValue;
		bool			m_pressed;
		bool			m_lastPressed;
		float			m_value;
		float			m_lastValue;
		float			m_timeSincePressed;

		Mouse()
		{
			m_key = KeyType::none;
			m_pressed = false;
			m_lastPressed = m_pressed;
			m_hasValue = false;
			m_value = 0.0f;
			m_lastValue = m_value;
		}
	};

	struct Pad
	{
		KeyType::Enum	m_key;
		bool			m_hasValue;
		bool			m_pressed;
		bool			m_lastPressed;
		float			m_value;
		float			m_lastValue;
		float			m_timeSincePressed;

		Pad()
		{
			m_key = KeyType::none;
			m_pressed = false;
			m_lastPressed = m_pressed;
			m_hasValue = false;
			m_value = 0.0f;
			m_lastValue = m_value;
		}
	};

	public:

		static InputMgr* getSingleton() { return s_singleton; }

		InputMgr();
		~InputMgr();

		void init();
		void process(const float dt);
		void end();

		void setUpdateWhenNoFocus(bool b) { m_updateWhenNoFocus = b; }

		const bool keyIsPressed(KeyType::Enum key, uint32_t id = 0);
		const bool keyIsJustPressed(KeyType::Enum key, uint32_t id = 0);
		const bool keyIsReleased(KeyType::Enum key, uint32_t id = 0);
		const bool keyIsJustReleased(KeyType::Enum key, uint32_t id = 0);

		const float getPadKeyValue(KeyType::Enum key, uint32_t id = 0);
		const float getPadKeyLastValue(KeyType::Enum key, uint32_t id = 0);
		const float getTimeSinceKeyPressed(KeyType::Enum key, uint32_t id = 0);

		const sf::Vector2f getMousePosition() const;
		const sf::Vector2f getMouseLastPosition() const { return m_lastMousePosition; }

		void showImGuiWindow(bool* window);

	private:

		static InputMgr*							s_singleton;

		std::map<KeyType::Enum, Keyboard>			m_keyboard;
		std::map<KeyType::Enum, Mouse>				m_mouse;
		std::vector<std::map<KeyType::Enum, Pad>>	m_pads;
		sf::Vector2f								m_lastMousePosition;
		sf::Vector2f								m_currentMousePosition;
		bool										m_updateWhenNoFocus;
};