﻿#pragma once

#include <memory>
#include <functional>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Interface/component.hpp"
#include "Mechanics/resource_holder.hpp"
#include "Mechanics/resource_identifiers.hpp"


/**
 * \brief Additional namespace for working with GUI elements.
 */
namespace GUI
{

/**
 * \brief Standart button class.
 */
class Button final : public Component
{
    public:
        /**
         * \brief Syntactic sugar.
         */
        using shPtr    = std::shared_ptr<Button>;

        /**
         * \brief Syntactic sugar.
         */
        using Callback = std::function<void()>;


    private:
        /**
         * \brief Action that will be executed when button is pressed.
         */
        Callback               _callback;

        /**
         * \brief Textrure for button when it is not pressed or selected.
         */
        const sf::Texture&     _normalTexture;

        /**
         * \brief Textrure for button when it is selected but not pressed.
         */
        const sf::Texture&     _selectedTexture;

        /**
         * \brief Textrure for button when it is pressed.
         */
        const sf::Texture&     _pressedTexture;

        /**
         * \brief Sound buffer for keeping button sounds.
         */
        const sf::SoundBuffer& _soundBuffer;

        /**
         * \brief Sound of the button (playing when button is pressed or selected).
         */
        sf::Sound              _sound;

        /**
         * \brief Sprite of the button for drawing.
         */
        sf::Sprite             _sprite;

        /**
         * \brief Text of the button.
         */
        sf::Text               _text;

        /**
         * \brief Boolean flag of toggling.
         */
        bool                   _isToggle;
    
    
    private:
        /**
         * \brief             Draw the object to a render target.
         * \param[out] target Render target to draw to.
         * \param[in] states  Current render states.
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


    public:
        /**
         * \brief                 Constructor.
         * \param[in] fonts       Fonts holder for extracting necessary font.
         * \param[in] textures    Textures holder for extracting necessary textures.
         * \param[in] soundBuffer Sounds holder for extracting necessary sounds.
         */
             Button(const FontHolder& fonts, const TextureHolder& textures,
                    const SoundBufferHolder& soundBuffer);

        /**
         * \brief              Set the callback (action that will be executed when button is pressed).
         * \param[in] callback Action for button.
         */
        void setCallback(Callback callback);

        /**
         * \brief          Set text to this button.
         * \param[in] text Text of the button.
         */
        void setText(const sf::String& text);

        /**
         * \brief          Change toggle flag of this button.
         * \param[in] flag New value for toggle flag.
         */
        void setToggle(const bool flag) noexcept;

        /**
         * \brief  Check if button can be selectable.
         * \return True if button is selectable or false otherwise.
         */
        bool isSelectable() const noexcept override;

        /**
         * \brief Call parent method, play sound and update button sprite.
         */
        void select() override;

        /**
         * \brief Call parent method and update button sprite.
         */
        void deselect() override;

        /**
         * \brief Call parent method, play sound, do callback and update button sprite.
         */
        void activate() override;

        /**
         * \brief Call parent method and update button sprite.
         */
        void deactivate() override;

        /**
         * \brief           Handle input events.
         * \param[in] event The event that was sent.
         */
        void handleEvent(const sf::Event& event) noexcept override;
};

}
