#include "SpriteBatch.h"
#include <algorithm>

//-------------------------------------------------------------------
AE::Glyph::Glyph(	const glm::vec4& destRect, 
					const glm::vec4& uvRect, 
					GLuint Texture, 
					float Depth, 
					const Color_rgb8& Color_rgb8) 
					: texture(Texture), depth(Depth)
{
//-----------------------------------	
	topLeft.Color_rgb8 = Color_rgb8;
	topLeft.setPosition(destRect.x, destRect.y + destRect.w);
	topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);
	
	bottomLeft.Color_rgb8 = Color_rgb8;
	bottomLeft.setPosition(destRect.x, destRect.y);
	bottomLeft.setUV(uvRect.x, uvRect.y);
	
	bottomRight.Color_rgb8 = Color_rgb8;
	bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
	bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	
	topRight.Color_rgb8 = Color_rgb8;
	topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
};

//-------------------------------------------------------------------
AE::Glyph::Glyph(	const glm::vec4& destRect, 
					const glm::vec4& uvRect, 
					GLuint Texture, 
					float Depth, 
					const Color_rgb8& Color_rgb8, 
					float angle) 
					: texture(Texture), depth(Depth) 
{
//-----------------------------------
	glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);
	
	glm::vec2 tl(-halfDims.x, +halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 tr(+halfDims.x, +halfDims.y);
	glm::vec2 br(+halfDims.x, -halfDims.y);

	tl = rotatePoint(tl, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;

	topLeft.Color_rgb8 = Color_rgb8;
	topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
	topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	bottomLeft.Color_rgb8 = Color_rgb8;
	bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
	bottomLeft.setUV(uvRect.x, uvRect.y);

	bottomRight.Color_rgb8 = Color_rgb8;
	bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
	bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	topRight.Color_rgb8 = Color_rgb8;
	topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
	topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
};

//-------------------------------------------------------------------
glm::vec2 AE::Glyph::rotatePoint(glm::vec2 pos, float angle) 
{
	glm::vec2 newV;
	newV.x = pos.x * cos(angle) - pos.y * sin(angle);
	newV.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newV;
}

///------------------------------------------------------------------
AE::SpriteBatch::SpriteBatch() 
{

}

//-------------------------------------------------------------------
AE::SpriteBatch::~SpriteBatch() 
{

}

//-------------------------------------------------------------------
void AE::SpriteBatch::init() 
{
	createVertexArray();
}

//-------------------------------------------------------------------
void AE::SpriteBatch::begin(GlyphSortType sortType /*= GlyphSortType::TEXTURE */) 
{
	m_sortType = sortType;
	m_renderBatches.clear();
	m_glyphs.clear();
}

//-------------------------------------------------------------------
void AE::SpriteBatch::end() 
{
	sortGlyphs();
	createRenderBatches();
}

//-------------------------------------------------------------------
void AE::SpriteBatch::draw(	const glm::vec4& destRect, 
							const glm::vec4& uvRect, 
							GLuint texture, 
							float depth, 
							const Color_rgb8& Color_rgb8) 
//-----------------------------------
{
	m_glyphs.emplace_back(destRect, uvRect, texture, depth, Color_rgb8);
}

//-------------------------------------------------------------------
void AE::SpriteBatch::draw(	const glm::vec4& destRect, 
							const glm::vec4& uvRect, 
							GLuint texture, 
							float depth, 
							const Color_rgb8& Color_rgb8, 
							float angle)
//-----------------------------------
{
	m_glyphs.emplace_back(destRect, uvRect, texture, depth, Color_rgb8, angle);
}

//-------------------------------------------------------------------
void AE::SpriteBatch::draw(	const glm::vec4& destRect, 
							const glm::vec4& uvRect, 
							GLuint texture, 
							float depth, 
							const Color_rgb8& Color_rgb8, 
							const glm::vec2& dir) 
//-----------------------------------
{
	const glm::vec2 right(1.0f, 0.0f);
	float angle = acos(glm::dot(right, dir));
	
	if (dir.y < 0) {
		angle = -angle;
	}
	
	m_glyphs.emplace_back(destRect, uvRect, texture, depth, Color_rgb8, angle);
}

//-------------------------------------------------------------------
void AE::SpriteBatch::renderBatch() 
{
	glBindVertexArray(m_vao);
	
	for (unsigned long i = 0; i < m_renderBatches.size(); i++) {
		glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
	}
	
	glBindVertexArray(0);
}

//-------------------------------------------------------------------
void AE::SpriteBatch::createRenderBatches() 
{
	std::vector <Vertex> vertices;
	vertices.resize(m_glyphs.size() * 6);
	
	if (m_glyphs.empty()) {
		return;
	}
	
	int offset = 0;
	int cv = 0; //current vertex
	m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->texture);
	vertices[cv++] = m_glyphPointers[0]->topLeft;
	vertices[cv++] = m_glyphPointers[0]->bottomLeft;
	vertices[cv++] = m_glyphPointers[0]->bottomRight;
	vertices[cv++] = m_glyphPointers[0]->bottomRight;
	vertices[cv++] = m_glyphPointers[0]->topRight;
	vertices[cv++] = m_glyphPointers[0]->topLeft;
	
	offset += 6;
	
	for (unsigned long cg = 1; cg < m_glyphPointers.size(); cg++) {
		if (m_glyphPointers[cg]->texture != m_glyphPointers[cg - 1]->texture) {
			m_renderBatches.emplace_back(offset, 6, m_glyphPointers[cg]->texture);
		} else {
			m_renderBatches.back().numVertices += 6;
		}
		
		vertices[cv++] = m_glyphPointers[cg]->topLeft;
		vertices[cv++] = m_glyphPointers[cg]->bottomLeft;
		vertices[cv++] = m_glyphPointers[cg]->bottomRight;
		vertices[cv++] = m_glyphPointers[cg]->bottomRight;
		vertices[cv++] = m_glyphPointers[cg]->topRight;
		vertices[cv++] = m_glyphPointers[cg]->topLeft;
		offset += 6;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//orphan buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//upload
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-------------------------------------------------------------------
void AE::SpriteBatch::createVertexArray() 
{
	if (m_vao == 0) {
		glGenVertexArrays(1, &m_vao);
	}
	
	glBindVertexArray(m_vao);

	if (m_vbo == 0) {
		glGenBuffers(1, &m_vbo);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//Tell opengl that we want to use the first
	//attribute array. We only need one array right
	//now since we are only using position.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	//-----------------------------------
	//This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, 
					GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	//-----------------------------------
	//This is the Color_rgb8 attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, 
					GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Color_rgb8));
	//-----------------------------------
	//This is the UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, 
					GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	//-----------------------------------
	glBindVertexArray(0);
}

//-------------------------------------------------------------------
void AE::SpriteBatch::sortGlyphs() 
{
	m_glyphPointers.resize(m_glyphs.size());
	
	for (unsigned long i = 0; i < m_glyphs.size(); i++) {
		m_glyphPointers[i] = &m_glyphs[i];
	}

	switch (m_sortType) {
	//-----------------------------------
	case GlyphSortType::NONE:
	break;
	//-----------------------------------
	case GlyphSortType::BACK_TO_FRONT:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareBackToFront);
	break;
	//-----------------------------------
	case GlyphSortType::FRONT_TO_BACK:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareFrontToBack);
	break;
	//-----------------------------------
	case GlyphSortType::TEXTURE:
		std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareTexture);
	break;
	}
}

//-------------------------------------------------------------------
bool AE::SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) 
{
	return (a->depth < b->depth);
}

//-------------------------------------------------------------------
bool AE::SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) 
{
	return (a->depth > b->depth);
}

//-------------------------------------------------------------------
bool AE::SpriteBatch::compareTexture(Glyph* a, Glyph* b) 
{
	return (a->texture < b->texture);
}
