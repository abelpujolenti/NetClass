#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <functional>

class Animation
{
public:
	typedef std::function<void()> OnAnimationEnd;

public:
	Animation() = default;
	Animation(sf::Sprite* target, sf::Vector2i startOffset, sf::Vector2i size, unsigned long time, int framesCount, int columns, int rows, bool isReversedSprite);
	~Animation();

	void Play(bool fromStart = false);
	void Stop();
	void PlayOnce(OnAnimationEnd onAnimationEnd);

	void ResetFrame();
	bool NextFrame();

private:

	sf::Sprite* _target = nullptr;
	
	sf::Vector2i _startOffset;
	sf::Vector2i _size;

	unsigned long _time;

	int _framesCount;
	int _columns;
	int _rows;
	int _currentFrame = 0;

	bool _isReversedSprite;

	sf::IntRect _rect;

	std::mutex _mutex;
	
	bool _playing;
	bool _isPlayOnce;

	unsigned long _updateFrameTime = 100;

	OnAnimationEnd _onAnimationEnd;

	void StartAnimationTimer();
	void SetCurrentFrame();
};

