﻿#include "Enemies/dark_soldier.hpp"


DarkSoldier::DarkSoldier(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts,
                         const level& lvl, const float X, const  float Y, const int width,
                         const int height, const std::string& type)
: Enemy(id, textures, fonts, lvl, X, Y, width, height, type)
{
    mTexture = textures.get(Textures::ID::DarkSoldier);
    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(14, 131, mWidth, mHeight));
    mSprite.setScale(0.5f, 0.5f);
    dx = 0.05f;
}

void DarkSoldier::checkCollisionWithMap(const float Dx, const float Dy)
{
    for (const auto& object : mLevelObjects)
    {
        // Проверяем пересечение с объектом
        if (getRect().intersects(object.rect))
        {
            if (object.name == "enemyBorder")
            {
                if (Dy > 0.f)
                {
                    y = object.rect.top - mHeight;
                    dy = 0.f;
                    mOnGround = true;
                }
                if (Dy < 0.f)
                {
                    y = object.rect.top + object.rect.height;
                    dy = 0.f;
                }
                if (Dx > 0.f)
                {
                    x = object.rect.left - mWidth;
                    mIsTurned = true;
                }
                if (Dx < 0.f)
                {
                    x = object.rect.left + object.rect.width;
                    mIsTurned = true;
                }
            }

            // Если встретили смерть
            if (object.name == "death")
            {
                mHitpoints = 0;
            }
        }
    }
}

void DarkSoldier::update(const float dt)
{
    // Притяжение к земле
    dy += 0.0015f * dt;
    y += dy * dt;
    checkCollisionWithMap(0.f, dy);

    if (mIsTurned)
    {
        dx = -dx;
        mIsTurned = false;
    }

    if (mLife && (mHitpoints > 0))
    {
        if (!mIsAttacked && !mIsTurned)
        {
            x += dx * dt;
            mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 7.f);
        }
        else if (!mIsTurned)
        {
            mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 6.f);
        }
        else
        {
            mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 6.f);
        }
        checkCollisionWithMap(dx, 0.f);

        mCurrentFrame += 0.005f * dt;
        if (mCurrentFrame > 5.f)
        {
            mCurrentFrame -= 5.f;
        }
        if (mIsAttacked)
        {
            mCurrentAttack += 0.005f * dt;
            if (mCurrentAttack > 4.f)
            {
                mCurrentAttack = 0.f;
                mIsAttacked = false;
                mIsHitted = true;
                mIsBack = false;
            }
            else
            {
                mIsAttacked = false;
                mIsHitted = false;
            }
            if (dx > 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
            }
        }
        else
        {
            mCurrentAttack = 0.f;
            if (dx > 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(64 * (mIsTurned ? 0 : static_cast<int>(mCurrentFrame)) + 14, (mIsTurned ? 3 : 131), 40, 60));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(64 * (mIsTurned ? 0 : static_cast<int>(mCurrentFrame)) + 14, (mIsTurned ? 3 : 195), 40, 60));
            }
        }
    }

    if (mHitpoints <= 0)
    {
        mCurrentDeath += 0.0035f * dt;
        dx = 0.f;
        dy = 0.f;
        if (mCurrentDeath > 5.f)
        {
            mCurrentDeath = 5.f;
            mLife = false;
        }
        mSprite.setPosition(x + (mWidth / 2.f) - 4.f, y + (mHeight / 2.f) - 7.f);
        mSprite.setTextureRect(sf::IntRect(0 + 14, 3, 40, 60));
    }
}
