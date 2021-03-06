﻿#include <iostream>

#include "Objects/gate.hpp"


Gate::Gate(const Type::ID id, const TextureHolder& textures, const FontHolder&, const level& lvl,
           const float X, const float Y, const int width, const std::string& type)
: Entity(id, X, Y, width, std::stoi(type) * 16, 50.f, 100, 0, type)
, mSize(std::stoi(type))
, mWaySizeDown(Y + mSize * 16.f)
, mWaySizeUp(Y)
{
    mLevelObjects = lvl.get_objects("solid");

    switch(std::stoi(type))
    {
        case 3:
            mTexture = textures.get(Textures::ID::GateDirt);
            break;
        case 4:
            mTexture = textures.get(Textures::ID::GateDirt);
            break;
        case 5:
            mTexture = textures.get(Textures::ID::GateWood);
            break;
        default:
            mTexture = textures.get(Textures::ID::GateWood);
            break;
    }

    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, width, 16 * mSize));
    mSprite.setPosition(x + (mWidth / 2.f), y + (mHeight / 2.f));
    dy = 0.05f;
}

void Gate::close()
{
    mIsAttacked = true;
}

void Gate::checkCollisionWithMap(const float, const float Dy)
{
    for (auto& mLevelObject : mLevelObjects)
    {
        // Проверяем пересечение с объектом
        if (getRect().intersects(mLevelObject.rect))
        {
            if (mLevelObject.name == "solid")
            {
                if (Dy > 0.f)
                {
                    y = mLevelObject.rect.top - mHeight;
                    dy = 0.f;
                    mIsEnd = true;
                }
                if (Dy < 0.f)
                {
                    y = mLevelObject.rect.top + mLevelObject.rect.height;
                    dy = 0.f;
                }
            }
        }
    }
}

void Gate::update(const float dt)
{
    switch (mTypeID)
    {
        case Type::ID::OpeningGate:
            if (mIsStarted)
            {
                mCurrentFrame += 0.005f * dt;

                y += dy * dt;
                if (y >= mWaySizeDown)
                {
                    dy = 0.f;
                    mIsEnd = true;
                }

                mSprite.setPosition(x + (static_cast<float>(mWidth) / 2.f),
                                    y + (static_cast<float>(mHeight) / 2.f));
            }
            break;

        case Type::ID::ClosingGate:
            if (mIsAttacked)
            {
                mCurrentFrame += 0.005f * dt;

                y += dy * dt;
                if (y >= mWaySizeDown)
                {
                    dy = 0.f;
                    mIsEnd = true;
                }

                mSprite.setPosition(x + (static_cast<float>(mWidth) / 2.f), 
                                    y + (static_cast<float>(mHeight) / 2.f));
            }
            break;

        case Type::ID::OpenClosingGate:
            mCurrentFrame += 0.005f * dt;

            y += dy * dt;
            if ((y >= mWaySizeDown && !mIsEnd) || (y <= mWaySizeUp && mIsEnd))
            {
                mIsEnd = dy > 0.f;
                dy = -dy;
            }

            mSprite.setPosition(x + (static_cast<float>(mWidth) / 2.f),
                                y + (static_cast<float>(mHeight) / 2.f));
            break;

        default:
            std::cout << "Invalid object type!\n";
            break;
    }
}
