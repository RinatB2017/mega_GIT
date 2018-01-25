#pragma once

#include "lc_commands.h"

class lcKeyboardShortcuts
{
public:
	void Reset();
	bool Save(const QString& FileName);
	bool Save(QTextStream& Stream);
	bool Load(const QString& FileName);
	bool Load(QTextStream& Stream);

	QString mShortcuts[LC_NUM_COMMANDS];
};

extern lcKeyboardShortcuts gKeyboardShortcuts;

void lcLoadDefaultKeyboardShortcuts();
void lcSaveDefaultKeyboardShortcuts();
void lcResetDefaultKeyboardShortcuts();

class lcMouseShortcuts
{
public:
	void Reset();
	bool Save(QStringList& Shortcuts);
	bool Load(const QStringList& Shortcuts);
	lcTool GetTool(Qt::MouseButton Button, Qt::KeyboardModifiers Modifiers) const;

	struct
	{
		Qt::KeyboardModifiers Modifiers1;
		Qt::MouseButton Button1;
		Qt::KeyboardModifiers Modifiers2;
		Qt::MouseButton Button2;
	}
	mShortcuts[LC_NUM_TOOLS];
};

extern lcMouseShortcuts gMouseShortcuts;

void lcLoadDefaultMouseShortcuts();
void lcSaveDefaultMouseShortcuts();
void lcResetDefaultMouseShortcuts();

