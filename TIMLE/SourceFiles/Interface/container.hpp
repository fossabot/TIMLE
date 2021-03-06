﻿#pragma once

#include <memory>
#include <vector>

#include "Interface/component.hpp"


/**
 * \brief Additional namespace for working with GUI elements.
 */
namespace GUI
{

/**
 * \brief Element, which can contain other GUI elements.
 */
class Container final : public Component
{
    public:
        /**
         * \brief Syntactic sugar.
         */
        using Ptr = std::shared_ptr<Container>;
            

    private:
        /**
         * \brief Array that contains all elements for manipulating them.
         */
        std::vector<Component::ShPtr> _children;

        /**
         * \brief Number of the selected child.
         */
        int                           _selectedChild;
    
    
    private:
        /**
         * \brief             Draw the object to a render target.
         * \param[out] target Render target to draw to.
         * \param[in] states  Current render states.
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
        /**
         * \brief  Defines selected item or not.
         * \return True if it has selection or false otherwise.
         */
        bool hasSelection() const;

        /**
         * \brief           Change focus on the element with given parameter.
         * \param[in] index Number of the element that we select.
         */
        void select(const std::size_t index);

        /**
         * \brief Change focus on the next element in the list.
         */
        void selectNext();

        /**
         * \brief Change focus on the previous element in the list.
         */
        void selectPrevious();


    public:
        /**
         * \brief Default constructor.
         */
             Container();

        /**
         * \brief               Pack new component in this container.
         * \param[in] component Component that need to pack.
         */
        void pack(const Component::ShPtr& component);

        /**
         * \brief  Check if container can be selectable.
         * \return True if container is selectable or false otherwise.
         */
        bool isSelectable() const override;

        /**
         * \brief           Handle input events.
         * \param[in] event The event that was sent.
         */
        void handleEvent(const sf::Event& event) override;
};

}
