﻿#include "golem_dark.hpp"


namespace
{
    const GolemDarkData GOLEM_TABLE = initializeGolemDarkData();
}

GolemDark::GolemDark(const Type::ID id, const TextureHolder& textures, const FontHolder&,
                     const Level& level, const float x_coord, const float y_coord, const int width,
                     const int height, const std::string& type)
: Entity(id, x_coord, y_coord, width, height, GOLEM_TABLE.mSpeed, GOLEM_TABLE.mHitpoints,
         GOLEM_TABLE.mDamage)
, counter_(0)
, turned_around_(false)
{
    // Initialize basic objects for interaction golem with it.
    for (const auto& object : level.getObjects("enemyBorder"))
    {
        if (object.mType == type)
        {
            mLevelObjects.push_back(object);
        }
    }

    mTexture = textures.get(Textures::ID::GolemDark);
    mTextureAttack = textures.get(Textures::ID::GolemDarkAttack);
    mTextureDeath = textures.get(Textures::ID::GolemDarkDeath);
    mSprite.setScale(1.0f, 1.0f);
    dx = 0.07f;
    mIsEnd = false;

    mSprite.setTexture(mTextureDeath);
    mSprite.setPosition(x + (mWidth / 2.f) + 3.f, y + (mHeight / 2.f) + 1.f);
    mSprite.setTextureRect(sf::IntRect(64 * 5, 64, 64, 64));
    mCurrentDeath = 5.f;
}

void GolemDark::check_collision_with_map(const float dX, const float dY)
{
    for (const auto& object : mLevelObjects)
    {
        if (getRect().intersects(object.mRect))
        {
            if (dY > 0.f)
            {
                y = object.mRect.top - mHeight;
                dy = 0.f;
                mOnGround = true;
            }
            if (dY < 0.f)
            {
                y = object.mRect.top + object.mRect.height;
                dy = 0.f;
            }
            if (dX > 0.f)
            {
                x = object.mRect.left - mWidth;
                turned_around_ = true;
            }
            if (dX < 0.f)
            {
                x = object.mRect.left + object.mRect.width;
                turned_around_ = true;;
            }
        }
    }
}

void GolemDark::update(const float dt)
{
    // The attraction to the earth.
    dy += 0.0015f * dt;
    y += dy * dt;
    check_collision_with_map(0.f, dy);

    /// Complicated and tangled logic for setting right position of the sprite.
    // Inactive phase.
    if (!mIsStarted)
    {
        mSprite.setTexture(mTextureDeath);
        mSprite.setPosition(x + (mWidth / 2.f) + 3.f, y + (mHeight / 2.f) + 1.f);
        mSprite.setTextureRect(sf::IntRect(64 * 5, 64, 64, 64));
        mCurrentDeath = 5.f;
        return;
    }
    else if (!mIsEnd)
    {
        mCurrentDeath -= 0.005f * dt;
        if (mCurrentDeath < 0.f)
        {
            mCurrentDeath = 0.f;
            mIsEnd = true;
        }
        mSprite.setTexture(mTextureDeath);
        mSprite.setPosition(x + (mWidth / 2.f) + 3.f, y + (mHeight / 2.f) + 1.f);
        mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath), 64, 64, 64));
        return;
    }

    mMoveTimer += dt;
    if (mMoveTimer > 2000.f && turned_around_)
    {
        dx = -dx;
        mMoveTimer = 0.f;
        turned_around_ = false;
    }
    else if (mMoveTimer > 1000.f && !turned_around_)
    {
        mMoveTimer = 0.f;
        if (mHitpoints <= 0)
        {
            ++counter_;
        }
    }

    if (mLife && (mHitpoints > 0))
    {
        // Moving phase.
        if (!mIsAttacked && !turned_around_)
        {
            x += dx * dt;
        }
        check_collision_with_map(dx, 0.f);

        mSprite.setPosition(x + (mWidth / 2.f) + 11.f, y + (mHeight / 2.f) - 2.f);

        mCurrentFrame += 0.005f * dt;
        if (mCurrentFrame > 7.f)
        {
            mCurrentFrame -= 7.f;
        }
        // Attack phase.
        if (mIsAttacked)
        {
            mCurrentAttack += 0.005f * dt;
            if (mCurrentAttack > 7.f)
            {
                mCurrentAttack -= 7.f;
                mIsAttacked = false;
                mIsBack = false;
                mIsHittedOnce = false;
            }
            else if (static_cast<int>(mCurrentAttack) == 4 && !mIsHittedOnce)
            {
                mIsHitted = true;
                mIsHittedOnce = true;
            }
            else {
                mIsHitted = false;
            }
            mSprite.setTexture(mTextureAttack);
            mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 13.f : -4.f), y + (mHeight / 2.f) - 11.f);
            if (dx > 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentAttack), 311, 64, 72));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentAttack), 119, 64, 72));
            }
        }
        else
        {
            mSprite.setTexture(mTexture);
            mCurrentAttack = 0.f;
            if (turned_around_)
            {
                mCurrentFrame = 0.f;
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + (dx > 0 ? 0 : 17), (dx > 0 ? 192 : 64), 48, 64));
            }
            if (dx > 0.f && !turned_around_)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame), 192, 48, 64));
            }
            else if (dx < 0.f && !turned_around_)
            {
                mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentFrame) + 17, 64, 48, 64));
            }
        }
    }

    // Dead phase.
    if (mHitpoints <= 0)
    {
        mCurrentDeath += 0.0035f * dt;
        dx = 0.f;
        dy = 0.f;
        if (mCurrentDeath > 6.f)
        {
            mCurrentDeath = 6.f;
            if (counter_ == 6)
            {
                mLife = false;
            }
        }
        mSprite.setTexture(mTextureDeath);
        mSprite.setPosition(x + (mWidth / 2.f) + 5.f, y + (mHeight / 2.f) + 1.f);
        mSprite.setTextureRect(sf::IntRect(64 * static_cast<int>(mCurrentDeath), 0, 64, 64));
    }
}