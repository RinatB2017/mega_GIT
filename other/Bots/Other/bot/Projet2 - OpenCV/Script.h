#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>
#include "Settings.h"
#include "MouseHook.h"
#include "State.h"

class Script
{
private:
	Settings settings;
	std::vector<State> states;
public:
	Script();
	~Script();

	void create();
	void load();
	void save() const;
	void execute();
};

#endif