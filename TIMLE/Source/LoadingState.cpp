#include "../Include/LoadingState.hpp"
#include "../Include/Utility.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


LoadingState::LoadingState(StateStack& stack, Context context)
: State(stack, context)
, mLoadingTask(*context.mTextures, *context.mSounds, context.mPlayerInfo)
{
	sf::RenderWindow& window = *getContext().mWindow;
	sf::Font& font = context.mFonts->get(Fonts::Main);
	sf::Vector2f viewSize = window.getView().getSize();

	mLoadingText.setFont(font);
	mLoadingText.setString(L"Загрузка...");
	centerOrigin(mLoadingText);
	mLoadingText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

	mProgressBarBackground.setFillColor(sf::Color::White);
	mProgressBarBackground.setSize(sf::Vector2f(viewSize.x - 20.f, 10.f));
	mProgressBarBackground.setPosition(10.f, mLoadingText.getPosition().y + 40.f);

	mProgressBar.setFillColor(sf::Color(100,100,100));
	mProgressBar.setSize(sf::Vector2f(200.f, 10.f));
	mProgressBar.setPosition(10.f, mLoadingText.getPosition().y + 40.f);

	setCompletion(0.f);

	mLoadingTask.execute();
}

void LoadingState::draw()
{
	sf::RenderWindow& window = *getContext().mWindow;

	window.setView(window.getDefaultView());

	window.draw(mLoadingText);
	window.draw(mProgressBarBackground);
	window.draw(mProgressBar);
}

bool LoadingState::update(sf::Time)
{
	// Update the progress bar from the remote task or finish it.
	if (mLoadingTask.isFinished())
	{
		requestStackPop();
		requestStackPush(States::Game);
	}
	else
	{
		setCompletion(mLoadingTask.getCompletion());
	}
	return true;
}

bool LoadingState::handleEvent(const sf::Event& event)
{
	return true;
}

void LoadingState::setCompletion(float percent)
{
	// Clamp.
	if (percent > 1.f)
	{
		percent = 1.f;
	}

	mProgressBar.setSize(sf::Vector2f(mProgressBarBackground.getSize().x * percent,
									  mProgressBar.getSize().y));
}
