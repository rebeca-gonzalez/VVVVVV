#include <GL/glew.h>
#include <GL/glut.h>
#include <irrKlang.h>
#include "Game.h"
using namespace irrklang;

#define MUSIC_VOLUME_1 0.5f
#define MUSIC_VOLUME_2 0.35f

void Game::init()
{
	bPlay = true;
	state = SceneState::MENU;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
	MusicEngine = createIrrKlangDevice();
	MusicEngine->setSoundVolume(MUSIC_VOLUME_1);
	MusicEngine->play2D("sounds/Presenting_VVVVVV.mp3", GL_TRUE);
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if (key == 27) { // Escape code
		if (state == SceneState::MENU) bPlay = false;
		else {
			if (!MusicEngine->isCurrentlyPlaying("sounds/Presenting_VVVVVV.mp3")) {
				MusicEngine->stopAllSounds();
				MusicEngine->setSoundVolume(MUSIC_VOLUME_1);
				MusicEngine->play2D("sounds/Presenting_VVVVVV.mp3", GL_TRUE);
			}
			state = SceneState::MENU;
			scene.setState(state);
		}
	}
	else if (key == '1') {
		if (state == SceneState::MENU) {
			state = SceneState::PLAY;
			scene.setState(state);
		}
		else if (state == SceneState::PLAY) {
			state = SceneState::LEVEL2;
			scene.setState(state);
			MusicEngine->stopAllSounds();
			MusicEngine->setSoundVolume(MUSIC_VOLUME_2);
			MusicEngine->play2D("sounds/Potential_for_anything.mp3", GL_TRUE);
		}
	}
	else if (key == '2') {
		if (state == SceneState::PLAY) {
			state = SceneState::LEVEL1;
			scene.setState(state);
			MusicEngine->stopAllSounds();
			MusicEngine->setSoundVolume(MUSIC_VOLUME_1);
			MusicEngine->play2D("sounds/Pushing_Onwards.mp3", GL_TRUE);
		}
		else if (state == SceneState::MENU) {
			state = SceneState::INSTRUCTIONS;
			scene.setState(state);
		}
	}
	else if (key == '3') {
		if (state == SceneState::MENU) {
			state = SceneState::CREDITS;
			scene.setState(state);
		}
	}
	else if (key == '4') {
		if (state == SceneState::MENU) {
			state = SceneState::CREDITS;
			scene.setState(state);
		}
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	if (key == GLUT_KEY_UP) up_key = false;
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

bool Game::get_up_key() {
	return up_key;
}

void Game::block_up_key() {
	up_key = true;
}

void Game::win(){
	MusicEngine->stopAllSounds();
	MusicEngine->setSoundVolume(MUSIC_VOLUME_1);
	MusicEngine->play2D("sounds/win.wav", GL_FALSE);
	state = SceneState::WIN;
	scene.setState(state);
}