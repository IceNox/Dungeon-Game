#include "SpriteSelection.h"

void evaluate_sprite_name(Player *player)
{
	if (player->currentAction == P_IDLE) {
		if (player->spriteFacing == LEFT) {
			if (player->spriteNumber == 1) {
				player->finalSprite = PLAYER_IDLE_LEFT_1;
			}
			else if (player->spriteNumber == 2) {
				player->finalSprite = PLAYER_IDLE_LEFT_2;
			}
		}
		else if (player->spriteFacing == RIGHT) {
			if (player->spriteNumber == 1) {
				player->finalSprite = PLAYER_IDLE_RIGHT_1;
			}
			else if (player->spriteNumber == 2) {
				player->finalSprite = PLAYER_IDLE_RIGHT_2;
			}
		}
	}
	else if (player->currentAction == P_MOVING) {
		if (player->spriteFacing == LEFT) {
			if (player->spriteNumber == 1) {
				player->finalSprite = PLAYER_IDLE_LEFT_1;
			}
			else if (player->spriteNumber == 2) {
				player->finalSprite = PLAYER_IDLE_LEFT_2;
			}
		}
		else if (player->spriteFacing == RIGHT) {
			if (player->spriteNumber == 1) {
				player->finalSprite = PLAYER_IDLE_RIGHT_1;
			}
			else if (player->spriteNumber == 2) {
				player->finalSprite = PLAYER_IDLE_RIGHT_2;
			}
		}
	}
}