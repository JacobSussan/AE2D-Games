#pragma once

#include "Capsule.h"

#include "AE2D/TileSheet.h"

#include "Box.h"
#include "Blocks/Air.h"
#include "Blocks/Brick.h"
#include "Blocks/LightBrick.h"
#include "Blocks/Glass.h"
#include "Blocks/RainbowBrick.h"
#include "BlockData.h"

enum class PlayerMoveState 
{
	STANDING, RUNNING, PUNCHING, RUNNING_IN_AIR, JUMPING, FALLING
};

//-------------------------------------------------------------------
class Player
{
//-------------------------------------------------------------------
	public:
		Player ();
		~Player ();
		//-----------------------------------
		void init(b2World* world, 
					const glm::vec2& pos,
					const glm::vec2& drawDims,
					const glm::vec2& collisionDims,
					const AE::Color_rgb8 color);
		//-----------------------------------
		void createCapsule(const glm::vec2& collisionDims, 
						b2World* world);
		//-----------------------------------
		bool update(
			float								 &tps,
			std::vector<Box> 					 &boxes,
			std::vector<Block_S*> 				 &blocks,
			std::vector< std::vector<Block_S*> > &TLBlocks,
			std::vector< std::vector<Block_S*> > &TRBlocks,
			std::vector< std::vector<Block_S*> > &BLBlocks,
			std::vector< std::vector<Block_S*> > &BRBlocks, 
			b2World 							 *world		);
		//-----------------------------------
		glm::vec2&	getDimentions() { return m_collisionDims; }
		AE::Color_rgb8& getColor()  { return m_color; }
		Capsule& 	getCapsule()    { return m_capsule; }
		//-----------------------------------
		std::string& getTextureFilePath() { 
			return m_textureFilePath; 
		}
		//-----------------------------------
		b2Body* getBody(int index) { 
			return m_capsule.getBody(index); 
		}
		//-----------------------------------
		glm::vec2 	getPosition() { 
			glm::vec2 rv;
			rv.x = 
			  m_capsule.getBody(m_currentBodyIndex)->GetPosition().x;
			rv.y = 
			  m_capsule.getBody(m_currentBodyIndex)->GetPosition().y;
			return rv;
		}
		//-----------------------------------
		glm::vec2 getPrevPlayerPos() { return m_prevPlayerPos; }
        //-----------------------------------
		void setPrevPlayerPos(glm::vec2 pPlayerPos) {
			pPlayerPos = m_prevPlayerPos;
		}
        //-----------------------------------
		std::vector<unsigned int> &getPressedKeys() { 
			return m_pressedKeys; 
		}
        //-----------------------------------
		std::vector<unsigned int> &getPrevPressedKeys() {
			return m_prevPressedKeys;
		}
		//-----------------------------------
		glm::vec2 &getScreenMouseCoords() { 
            return m_screenMouseCoords; 
		}
		//-----------------------------------
		glm::vec2 &getWorldMouseCoords() {
			return m_worldMouseCoords;
		}
//-------------------------------------------------------------------
	private:
		glm::vec2 m_drawDims;
		AE::Color_rgb8 m_color;
		AE::TileSheet m_texture;
		std::string m_textureFilePath;

		// body stuff
		glm::vec2 m_collisionDims;
		glm::vec2 m_pos;
		Capsule m_capsule;
		int m_currentBodyIndex = 0;

		PlayerMoveState m_moveState = PlayerMoveState::STANDING;
		int m_direction = 1; 			// -1 or 1
		float m_animationTime = 0;
		bool m_onGround = false;
		bool m_punching = false;
		float m_framesSinceLastBodyUpdate = 999;
		// vector of all keys being pressed right now
		std::vector<unsigned int> m_pressedKeys;
		std::vector<unsigned int> m_prevPressedKeys;
		// last player pos
		glm::vec2 m_prevPlayerPos = glm::vec2(0);
		// mouse coords
		glm::vec2 m_worldMouseCoords;
		glm::vec2 m_screenMouseCoords;
		// selected block
		BLOCK_ID m_selectedBlock = BLOCK_ID::BRICK;
};



