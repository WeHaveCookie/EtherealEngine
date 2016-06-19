#pragma once

enum MovingState {
	RIGHT, 
	LEFT,
	IDLERIGHT,
	IDLELEFT
};

enum MenuState {
	TITLE, 
	CREDIT, 
	OPTION, 
	NOTHING
};

enum RessourcesType {
	NONE, 
};

enum EventState {
	WAITED, 
	TRIGGERED,
	ACCEPTED,
	FINISHED
};

enum ManagerType {
	TIME,
	SOUND,
	LEVEL,
	ITEM,
	INPUT,
	GAME,
	EVENT,
	ENGINE,
	RENDER
};
