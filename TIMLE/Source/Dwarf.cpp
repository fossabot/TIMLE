#include "../Include/Dwarf.hpp"


Dwarf::Dwarf(const Type::ID id, const TextureHolder& textures, const FontHolder& fonts, 
             const Level& lvl, const float X, const float Y, const int width, const int height,
             const std::string& type, const  int dwarfType)
: Enemy(id, textures, fonts, lvl, X, Y, width, height, type)
, _dwarfType(dwarfType)
, _inaccuracy(0.f)
{
    mTexture = textures.get(Textures::ID::Dwarf);
    mSprite.setTexture(mTexture);
    mSprite.setScale(0.5f, 0.5f);
    mSprite.setTextureRect(sf::IntRect(0, 80 * _dwarfType, mWidth, mHeight));
    dx = 0.075f;
    mSprite.scale(-1.f, 1.f);
    if (id == Type::ID::DwarvenCommander)
        mIsSpawn = false;
    else
        mIsSpawn = true;
}

void Dwarf::checkCollisionWithMap(const float Dx, const float Dy)
{
    for (const auto& object : mLevelObjects)
    {
        // ��������� ����������� � ��������
        if (getRect().intersects(object.mRect))
        {
            if (object.mName == "enemyBorder")
            {
                if (Dy > 0.f)
                {
                    y = object.mRect.top - mHeight;
                    dy = 0.f;
                    mOnGround = true;
                }
                if (Dy < 0.f)
                {
                    y = object.mRect.top + object.mRect.height;
                    dy = 0.f;
                }
                if (Dx > 0.f)
                {
                    x = object.mRect.left - mWidth;
                    mIsTurned = true;
                }
                if (Dx < 0.f)
                {
                    x = object.mRect.left + object.mRect.width;
                    mIsTurned = true;
                }
            }

            // ���� ��������� ������
            if (object.mName == "death")
            {
                mHitpoints = 0;
            }
        }
    }
}

void Dwarf::update(const float dt)
{
    // ���������� � �����
    dy += 0.0015f * dt;
    y += dy * dt;
    checkCollisionWithMap(0.f, dy);

    if (!mIsStarted)
    {
        mCurrentDeath += 0.005f * dt;
        if (mCurrentDeath > 4.f)
        {
            mCurrentDeath -= 4.f;
        }
        mSprite.setPosition(x + (mWidth / 2.f) + 16.f, y + (mHeight / 2.f) - 19.f);
        mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentDeath) + 16), 80 * _dwarfType, 100, 80));
        mCurrentAttack = 3.f;
        return;
    }
    if (!mIsEnd)
    {
        mCurrentAttack -= 0.0025f * dt;
        if (mCurrentAttack < 0.f)
        {
            mCurrentAttack = 0.f;
            mCurrentDeath = 0.f;
            mIsEnd = true;
        }
        int temp;
        if (static_cast<int>(mCurrentAttack) == 3)
        {
            temp = 0;
        }
        else if (static_cast<int>(mCurrentAttack) == 2)
        {
            temp = 14;
        }
        else
        {
            temp = 13;
        }
        mSprite.setPosition(x + (mWidth / 2.f) + (dx > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 19.f);
        mSprite.setTextureRect(sf::IntRect(100 * temp, 80 * _dwarfType, 100, 80));
        return;
    }

    if (mIsTurned)
    {
        mMoveTimer += dt;
    }
    if (mMoveTimer > 2000.f && mIsTurned)
    {
        dx = -dx;
        mMoveTimer = 0;
        mIsTurned = false;
        mSprite.scale(-1.f, 1.f);
    }

    if (mLife && (mHitpoints > 0))
    {
        if (!mIsAttacked && !mIsTurned)
        {
            x += dx * dt;
        }
        checkCollisionWithMap(dx, 0.f);

        mSprite.setPosition(x + static_cast<float>(mWidth) / 2.f + (dx > 0 ? 10.f : -10.f), y + static_cast<float>(mHeight) / 2.f - 19.f);

        mCurrentFrame += 0.005f * dt;
        if (mCurrentFrame > 6.f)
        {
            mCurrentFrame -= 6.f;
        }
        if (mIsAttacked)
        {
            switch (_dwarfType)
            {
                case 0:
                    mCurrentAttack += 0.0055f * dt;
                    break;
                case 1:
                    mCurrentAttack += 0.004f * dt;
                    break;
                case 2:
                    mCurrentAttack += 0.0055f * dt;
                    break;
                default:
                    mCurrentAttack += 0.005f * dt;
                    break;
            }
            
            if (mCurrentAttack > 4.f)
            {
                mCurrentAttack -= 4.f;
                mIsAttacked = false;
                mIsBack = false;
                mIsHittedOnce = false;
            }
            else if (static_cast<int>(mCurrentAttack) == 3 && !mIsHittedOnce)
            {
                mIsHitted = true;
                mIsHittedOnce = true;
            }
            else
            {
                mIsHitted = false;
            }

            if (dx > 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentAttack) + 10), 80 * _dwarfType, 100, 80));
            }
            else if (dx < 0.f)
            {
                mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentAttack) + 10), 80 * _dwarfType, 100, 80));
            }
        }
        else
        {
            mCurrentAttack = 0.f;
            if (mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) % 4), 80 * _dwarfType, 100, 80));
            }
            if (dx > 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) + 4), 80 * _dwarfType, 100, 80));
            }
            else if (dx < 0.f && !mIsTurned)
            {
                mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentFrame) + 4), 80 * _dwarfType, 100, 80));
            }
        }
        _inaccuracy = dx;
    }

    if (mHitpoints <= 0)
    {
        mMoveTimer += dt;
        if (mMoveTimer > 1000.f)
        {
            ++mCounter;
            mMoveTimer = 0.f;
        }
        mCurrentDeath += 0.0035f * dt;
        dx = 0.f;
        dy = 0.f;
        if (mCurrentDeath > 2.f)
        {
            mCurrentDeath = 2.f;
            if (mCounter == 6)
            {
                mLife = false;
            }
        }
        mSprite.setPosition(x + (mWidth / 2.f) + (_inaccuracy > 0 ? 10.f : -10.f), y + (mHeight / 2.f) - 19.f);
        mSprite.setTextureRect(sf::IntRect(100 * (static_cast<int>(mCurrentDeath) + (static_cast<int>(mCurrentDeath) == 0? 0 : 13)), 80 * _dwarfType, 100, 80));
    }
}
