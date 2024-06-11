#pragma once

#include "SFML/Graphics.hpp"

class Block
{
public:
	enum BlockColor
	{
		none,
		green,
		red,
		blue,
		yellow,
		no_Of_Colors
	};

	enum BlockType
	{
		one,
		two,
		three,
		four,

		sqaure2x2,
		rec2x3,
		zShape2x3,
		zShape2x4,

		lShape2x2,
		lShape2x3,
		lShape3x3,
		lShape2x4,
		sqaure3x3,
		no_of_Shapes
	};

	bool BeingHovered = 0;
	std::vector<std::vector<bool>> arr;

public:
	Block();
	~Block();
	Block(BlockType Type, BlockColor Color) :
		Type(Type), Color(Color)
	{
		Bounds.setFillColor({ 123,56,212 });
		BoxSprite.setTexture(s_BoxTexture);
		BoxSprite.setTextureRect(sf::IntRect(40 * (+Color), 0, 40, 40));

		setBlockType();
	}

	int getRot() const;
	sf::Vector2f getPos() const;

	void setRot(int rot);
	void setPos(const sf::Vector2f& pos);
	void setColor(BlockColor color)
	{
		Color = BlockColor(+color % 5);
		BoxSprite.setTextureRect(sf::IntRect(40 * (+Color), 0, 40, 40));
	}
	void rotate();

	sf::FloatRect getBounds();

	void draw(sf::RenderWindow& app);

	int64_t getSerializedPacket();
	void setSerializedPacket(int64_t en);

	static void loadBoxTexture(void* data, size_t size);

private:
	int Rot = 0;
	sf::Vector2f Pos;
	sf::Sprite  BoxSprite;
	sf::RectangleShape Bounds;
	BlockType Type = Block::one;
	BlockColor Color = Block::none;

private:
	void updateBounds();
	void setBlockType();

private:
	static sf::Texture s_BoxTexture;
};