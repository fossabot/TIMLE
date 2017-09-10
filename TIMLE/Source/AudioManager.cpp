#include "../Include/AudioManager.hpp"


AudioManager::AudioManager()
: mCurrentMusic(None)
, mCurrentState(Stop)
, mMusics()
{
	buildMusic();
}

void AudioManager::buildMusic()
{
	sf::Music* music = new sf::Music;

	music->openFromFile("Media/Sounds/MainMenuTheme.ogg");
	music->setLoop(true);
	mMusics.push_back(std::move(music));

	music = new sf::Music;
	music->openFromFile("Media/Sounds/FirstMainMusic.ogg");
	music->setLoop(true);
	mMusics.push_back(std::move(music));

	music = new sf::Music;
	music->openFromFile("Media/Sounds/FirstBossMusic.ogg");
	music->setLoop(true);
	mMusics.push_back(std::move(music));
}

void AudioManager::stopAllMusics()
{
	for (size_t i = 0; i < mMusics.size(); i++)
		mMusics[i]->stop();
}

void AudioManager::setMusic(MusicType musicType)
{
	if (mCurrentMusic == musicType)
		return;

	switch (musicType)
	{
		case MainMenuTheme:
			mCurrentMusic = musicType;
			stopAllMusics();
			mMusics[musicType]->play();
			break;
		case FirstMainMusic:
			mCurrentMusic = musicType;
			stopAllMusics();
			mMusics[musicType]->play();
			break;
		case FirstBossMusic:
			mCurrentMusic = musicType;
			stopAllMusics();
			mMusics[musicType]->play();
			break;
		default:
			std::cout << "Error! Invalid type of music." << std::endl;
			break;
	}
}

AudioManager::MusicType AudioManager::getMusicType() const
{
	return mCurrentMusic;
}

bool AudioManager::isPlaying() const
{
	return mCurrentState == Play;
}

