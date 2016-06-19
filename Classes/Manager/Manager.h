#pragma once


class Manager
{
	
	public:
		Manager(ManagerType type);
		~Manager();

		virtual void Init();
		virtual void Process(const float dt);
		virtual void End();

		ManagerType getType() const { return m_type; }

	private:
		ManagerType m_type;

};