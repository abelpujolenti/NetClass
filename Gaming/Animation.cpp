#include "Animation.h"
#include "Timer.h"

Animation::Animation(sf::Sprite* target, sf::Vector2i startOffset, sf::Vector2i size,
	unsigned long time, int framesCount, int columns, int rows, bool isReversedSprite)
	: _target(target), _startOffset(startOffset), _size(size), _time(time), _framesCount(framesCount),
	_columns(columns), _rows(rows), _isReversedSprite(isReversedSprite), _updateFrameTime(time / framesCount)
{
	if (_isReversedSprite)
	{
		_startOffset.x += size.x;
	}

	_rect = sf::IntRect(_startOffset, _size);
}

Animation::~Animation()
{
	Stop();
}

void Animation::Play(bool fromStart)
{
	_mutex.lock();

	if (fromStart)
	{
		ResetFrame();
	}

	const bool isPlaying { _playing};
	
	if (!isPlaying)
	{
		_playing = true;
	}
	_mutex.unlock();
	
	if (isPlaying)
	{
		return;
	}
	StartAnimationTimer();
}

void Animation::Stop()
{
	_mutex.lock();
	_playing = false;
	_mutex.unlock();
}

void Animation::PlayOnce(OnAnimationEnd onAnimationEnd)
{
	_onAnimationEnd = onAnimationEnd;
	_isPlayOnce = true;
	Play(true);
}

void Animation::ResetFrame()
{
	_currentFrame = 0;
	SetCurrentFrame();
}

bool Animation::NextFrame()
{
	if (_currentFrame >= _framesCount - 1)
	{
		_currentFrame = 0;

		if (_isPlayOnce)
		{
			return true;
		}
	}
	else
	{
		_currentFrame++;
	}

	SetCurrentFrame();
	return false;
}

void Animation::StartAnimationTimer()
{
	Timer::StartLoopTimer(_updateFrameTime, [&]() {

		_mutex.lock();
		bool isPlaying {_playing};
		_mutex.unlock();

		if (isPlaying)
		{
			const bool needFinish { NextFrame()};
			
			_mutex.lock();

			isPlaying = !needFinish;
			_playing = isPlaying;

			_mutex.unlock();

			if (needFinish)
			{
				_onAnimationEnd();
			}
		}

		return isPlaying;
	});
}

void Animation::SetCurrentFrame()
{
	const int row = _currentFrame / _columns;
	const int col = _currentFrame % _columns;

	sf::Vector2i size { _size};

	if (_isReversedSprite)
	{
		size.x = -size.x;
	}

	_rect = sf::IntRect(_startOffset + sf::Vector2i((_size.x * col), (_size.y * row)), size);
	_target->setTextureRect(_rect);
}


