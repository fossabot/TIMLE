﻿#pragma once

#include <map>

#include <SFML/Window/Event.hpp>

#include "Mechanics/data_tables.hpp"


/**
 * \brief Class, which contains and controls all player's data.
 */
class PlayerInfo final
{
    public:
        /**
         * \brief Array of action's player states.
         */
        enum class Action : std::size_t
        {
            MoveLeft,
            MoveRight,
            MoveUp,
            MoveDown,
            Fire,
            LaunchMissile,
            UseDoor,
            ActionCount
        };

        /**
         * \brief Array of the game status, according to the player's state.
         */
        enum class GameStatus
        {
            GameRunning,
            LevelComplete,
            GameOver
        };

        /**
         * \brief Array of the player status, according to the player's actions in game.
         * \details Set of the quests:
         * \details 0 – talking with Oswald;
         * \details 1 – killing DwarvenCommanderM and getting key;
         * \details 2 – talking with Heinrich;
         * \details 3 – fighting with first mini-boss;
         * \details 4 – killing first mini-boss GolemDark;
         * \details 5 – fighting with first boss;
         * \details 6 – killing first boss Shadow.
         */
        enum class Quest
        {
            TalkWithOswald,
            KillDwarvenCommanderM,
            TalkWithHeinrich,
            FightGolemDark,
            KillGolemDark,
            FightShadow,
            KillShadow
        };

        /**
         * \brief Array of the player solutions.
         * \details List of the made choices:
         * \details 0 - loot knight's and dwarven bodies;
         * \details 1 - choosing option to interactive with GolemDark.
         */
        enum class Solution
        {
            LootKnightAndDwarven,
            InteractWithGolem
        };


        /**
         * \brief Data structure that keeping player's save point.
         */
        struct SavePoint
        {
            /**
             * \brief              A constructor with basic parameters.
             * \param[in] x_coord  The initial x-coordinate of this point.
             * \param[in] y_coord  The initial y-coordinate of this point.
             */
            SavePoint(const float x_coord, const float y_coord)
            : x(x_coord)
            , y(y_coord)
            {
            }

            /**
             * \brief Contain x coordinate of the save point.
             */
            float x;

            /**
             * \brief Contain y coordinate of the save point.
             */
            float y;
        };

    
    private:
        /**
         * \brief Associative array of the key buttons for each player's action.
         */
        std::map<sf::Keyboard::Key, Action>    _keyBinding;

        //std::map<Action, Command>            mActionBinding; // Not using now.

        /**
         * \brief Current game status, using for interactiving with world.
         */
        GameStatus                             _currentGameStatus;

        /**
         * \brief Current level number.
         */
        std::size_t                            _currentLevelNumber;

        /**
         * \brief   Current player's class for interacting.
         * \details ATTENTION! It can be a dandling pointer if world class would destroyed and
         *          someone got pointer and dereferenced it.
         */
        Player*                                _player;

        /**
         * \brief Default values for quests array.
         */
        static const std::map<Quest, bool>           _defaultQuestValues;

        /**
         * \brief Default values for solutions array.
         */
        static const std::map<Solution, std::size_t> _defaultSolutionValues;


    public:
        /**
         * \brief Current dialog number, which we need to show.
         */
        std::size_t                     mDialogNumber;

        /**
         * \brief Array of all showed dialogs, which we have shown yet.
         */
        std::vector<std::size_t>        mShowedDialogs;

        /**
         * \brief Variable for choosing number in the choosing state of dialog.
         */
        std::size_t                     mChoosingNumber;

        /**
         * \brief All choices, which player has done yet.
         */
        std::map<Solution, std::size_t> mChosenSolution;

        /**
         * \brief Array of the quests, which player can carry out.
         */
        std::map<Quest, bool>           mQuests;

        /**
         * \brief Boolean flag for notification of completion downloading level and resources.
         */
        bool                            mLoaded;

        /**
         * \brief Lives counter of the player's hero.
         */
        std::size_t                     mLivesCount;

        /**
         * \brief Last save point of the current game.
         */
        SavePoint                       mLastSavePoint;

        /**
         * \brief Define we can ressurect player's hero or not.
         */
        bool                            mCanRessurect;

        /**
         * \brief Define we can transit player's hero or not.
         */
        bool                            mCanTransit;

        /**
         * \brief Define we should transit player's hero or not.
         */
        bool                            mNeedTransit;

        /**
         * \brief Define we transit player's hero or not.
         */
        bool                            mDoTransit;

        /**
         * \brief Define we start transiting player's hero or not.
         */
        bool                            mTransiting;

        /**
         * \brief Number of door to transit.
         */
        std::pair<int, std::string>     mNumberOfDoor;

        /**
         * \brief Coordination to transit.
         */
        std::pair<float, float>         mTransitPos;


    private:
        /**
         * \brief Default starter binding keys with action (action is only function<void>).
         */
        void        initializeActions();

        /**
         * \brief            Check if sending action is realtime action or not.
         * \param[in] action Action for checking.
         * \return           True if action is realtime or false otherwise.
         */
        static bool isRealtimeAction(const Action action);


    public:
        /**
         * \brief Default constructor.
         */
                          PlayerInfo();
    
        /**
         * \brief            Sends a notification to dialog state to show a certain dialog.
         * \param[in] number Number of the necessary dialog.
         */
        void              showDialog(const std::size_t number);

        /**
         * \brief            Bind action with key.
         * \param[in] action Action for binding.
         * \param[in] key    Key for binding.
         */
        void              assignKey(const Action action, const sf::Keyboard::Key key);

        /**
         * \brief            Get key that assigned with current action.
         * \param[in] action Action for searching assigned key.
         * \return           Found key or Key::Unknown.
         */
        sf::Keyboard::Key getAssignedKey(const Action action) const;

        /**
         * \brief            Change current game status of the app.
         * \param[in] status New game status.
         */
        void              setGameStatus(const GameStatus status) noexcept;


        /**
         * \brief  Get current game status (Needs for different checking).
         * \return Current game status.
         */
        GameStatus        getGameStatus() const noexcept;

        /**
         * \brief            Change current level number.
         * \param[in] number New level number.
         */
        void              setLevelNumber(const std::size_t number) noexcept;

        /**
         * \brief  Get current level number.
         * \return Current level number.
         */
        std::size_t       getLevelNumber() const noexcept;

        /**
         * \brief        Change current player's class.
         * \param player Pointer to a new player's class for interacting.
         */
        void              setPlayer(Player* const player) noexcept;

        /**
         * \brief  Get current binded player's class.
         * \return Pointer to current player's class.
         */
        Player*           getPlayer() const noexcept;

        /**
         * \brief Reset all data about current game session.
         */
        void              resetData();

        /**
         * \brief Reset some player's data (not reset ALL data) and ressurect at last save point.
         */
        void              ressurectPlayer();

        /**
         * \brief  Check if player fight with some of the bosses.
         * \return True if player fighting with boss, false otherwise.
         */
        bool              isFigthWithBoss() const;
};
