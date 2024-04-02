#pragma once

#include "AE2D/SpriteBatch.h"
#include "AE2D/GLSLProgram.h"
#include "Ball.h"

#include <vector>
#include <memory>
//-------------------------------------------------------------------
class BallRenderer 
{
public:
	~BallRenderer();
    virtual void renderBalls(AE::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix);

protected:
	std::unique_ptr<AE::GLSLProgram> m_program = nullptr;

};

//-------------------------------------------------------------------
class MomentumBallRenderer : public BallRenderer 
{
public:
    virtual void renderBalls(AE::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;

};

//-------------------------------------------------------------------
class VelocityBallRenderer : public BallRenderer 
{
public:
	VelocityBallRenderer(int screenWidth, int screenHeight);
    virtual void renderBalls(AE::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
private:
	int m_screenWidth;
	int m_screenHeight;
};

//-------------------------------------------------------------------
class TrippyBallRenderer : public BallRenderer {
public:
    TrippyBallRenderer(int screenWidth, int screenHeight);

    virtual void renderBalls(AE::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix) override;
private:
    int m_screenWidth;
    int m_screenHeight;
    float m_time = 0.0f;
};
