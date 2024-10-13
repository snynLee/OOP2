#pragma once
#include "GameObject.h"
#include "IDamageable.h"

class Canvas;
class IDamageable;

class TextUI : public GameObject {
	// GameObject* parent;
	IDamageable* damageable;
	float	hp;
	int		n_remaining_frames;

public:
	TextUI(IDamageable* parent);

	void draw() const override;

	void update() override;
};


