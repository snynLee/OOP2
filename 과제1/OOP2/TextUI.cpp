#include "TextUI.h"
#include "Canvas.h"

TextUI::TextUI(IDamageable* parent)
	: GameObject("", 1), damageable(parent), hp(0.0f),
	n_remaining_frames(60)
	
{
	if (parent == nullptr)
		return;

}


void TextUI::draw() const {
	if (n_remaining_frames == 0) return;

	int parent_pos = dynamic_cast<GameObject*>(damageable)->getPos();
	canvas->draw(getShape(), parent_pos + getPos());
}

void TextUI::update() {
	char buf[10];

	if (n_remaining_frames > 0) --n_remaining_frames;

	float current_hp = damageable->getHealth();
	if (hp == current_hp) return;
	hp = current_hp;
	n_remaining_frames = canvas->getFrameRate();
	sprintf(buf, "%3.1f", hp);
	setShape(buf);
}
