#pragma once

namespace ManagerType{
	enum Enum {
		Time,
		Sound,
		Level,
		Item,
		Input,
		Game,
		Event,
		Engine,
		Render,
		Entity
	};
}

class Manager
{
	
	
	public:
		Manager(ManagerType::Enum type);
		~Manager();

		virtual void init();
		virtual void process(const float dt);
		virtual void end();

		ManagerType::Enum getType() const { return m_type; }

	private:
		ManagerType::Enum m_type;

};