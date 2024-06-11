
#include "Blocks.hpp"

#include "LOG.hpp"

Block::Block()
{
	Bounds.setFillColor({ 123,56,212 });
	BoxSprite.setTexture(s_BoxTexture);
	BoxSprite.setTextureRect(sf::IntRect(40 * (+Color), 0, 40, 40));

	setBlockType();

}

Block::~Block()
{
}


int Block::getRot() const
{
	return Rot;
}

sf::Vector2f Block::getPos() const
{
	return Pos;
}

void Block::setRot(int rot)
{
	setBlockType();

	int r = rot;

	r %= 360;
	r -= (int)(r % 90);
	r /= 90;

	for (int i = 0; i < r; i++)
		rotate();

	Rot = r * 90;
}

void Block::setPos(const sf::Vector2f& pos)
{
	Pos = pos;
	Bounds.setPosition(Pos - sf::Vector2f(2, 2));
	
	//updateBounds();
}

void Block::rotate()
{
	switch (Type)
	{
	case Block::one:
	case Block::sqaure2x2:
	case Block::sqaure3x3:
	{
		break;
	}
	default:
	{
		std::vector<std::vector<bool>> rot;
		rot.resize(arr[0].size(), std::vector<bool>(arr.size(), 1));

		for (int i = 0; i < arr.size(); i++)
			for (int j = 0; j < arr[i].size(); j++)
			{
				rot[j][(int)rot[j].size() - i - 1] = arr[i][j];
			}

		arr = rot;
		updateBounds();
		Rot += 90;
		Rot %= 360;

		break;
	}
	}
}

sf::FloatRect Block::getBounds()
{
	return Bounds.getGlobalBounds();
}

void Block::draw(sf::RenderWindow& app)
{
	float l = BoxSprite.getTextureRect().width;
	float w = BoxSprite.getTextureRect().height;

	//app.draw(Bounds);

	for (int j = 0; j < arr.size(); j++)
		for (int i = 0; i < arr[j].size(); i++)
			if (arr[j][i])
			{
				BoxSprite.setPosition(Pos + sf::Vector2f(i * l, j * w - BeingHovered*5));
				app.draw(BoxSprite);
			}

	BeingHovered = false;
}

int64_t Block::getSerializedPacket()
{
	int r = Rot, x = Pos.x, y = Pos.y;
	int64_t num = 0;

	r %= 360;
	r -= (int)(r % 90);
	r /= 90;

	//std::cout << "\n\nDecoded: r: " << r << ", x: " << x << ", y: " << y;

	num |= ((32767) & y) << 17;
	num |= ((32767) & x) << 2;
	num |= ((3) & r) << 0;

	return num;
}

void Block::setSerializedPacket(int64_t en)
{
	int r = (3) & en;
	int x = ((32767 << 2) & en) >> 2;
	int y = ((32767 << 17) & en) >> 17;

	r *= 90;

	if (((1 << 16) & en))
	{
		x -= 1;
		x ^= 32767;
		x *= -1;
	}

	setPos(sf::Vector2f(x, y));
	setRot(r);
	updateBounds();
}


// private

void Block::setBlockType()
{
	switch (Type)
	{
	case Block::one:
	{
		arr = { {1} };

		break;
	}
	case Block::two:
	{
		arr = { {1, 1} };

		break;
	}
	case Block::three:
	{
		arr = { {1,1,1} };

		break;
	}
	case Block::four:
	{
		arr = { {1,1,1,1} };

		break;
	}

	case Block::sqaure2x2:
	{
		arr = {
			{1,1},
			{1,1}
		};

		break;
	}
	case Block::rec2x3:
	{
		arr = {
			{1,1,1},
			{1,1,1}
		};

		break;
	}
	case Block::zShape2x3:
	{
		arr = {
			{1,1,0},
			{0,1,1}
		};

		break;
	}
	case Block::zShape2x4:
	{
		arr = {
			{1,1,0,0},
			{0,1,1,1}
		};

		break;
	}

	case Block::lShape2x2:
	{
		arr = {
			{0,1},
			{1,1}
		};
		break;
	}
	case Block::lShape2x3:
	{
		arr = {
			{0,0,1},
			{1,1,1}
		};

		break;
	}
	case Block::lShape3x3:
	{
		arr = {
			{0,0,1},
			{0,0,1},
			{1,1,1}
		};

		break;
	}
	case Block::lShape2x4:
	{
		arr = {
			{0,0,0,1},
			{1,1,1,1}
		};
		break;
	}
	case Block::sqaure3x3:
	{
		arr = {
			{1,1,1},
			{1,1,1},
			{1,1,1}
		};
		break;
	}
	default: break;
	}

	updateBounds();
}

void Block::updateBounds()
{
	int border = 2;
	int l = 0, w = 0;
	int xOff = 0, yOff = 0;
	int sizeX = 0, sizeY = 0;

	{
		sizeX = BoxSprite.getTextureRect().width;
		sizeY = BoxSprite.getTextureRect().height;
	}

	for (int y = 0, sum = 0; y < arr.size(); y++)
	{
		sum = 0;
		for (int i = 0; i < arr[y].size(); i++)
			sum += arr[y][i];

		if (sum) { yOff = y; break; }
	}

	for (int x = 0, sum = 0; x < arr[0].size(); x++)
	{
		sum = 0;
		for (int i = 0; i < arr.size(); i++)
			sum += arr[i][x];

		if (sum) { xOff = x; break; }
	}

	for (int y = (int)arr.size() - 1, sum = 0; 0 <= y; y--)
	{
		sum = 0;
		for (int i = 0; i < arr[y].size(); i++)
			sum += arr[y][i];

		if (sum) { w = y - yOff + 1; break; }
	}

	for (int x = (int)arr[0].size() - 1, sum = 0; 0 <= x; x--)
	{
		sum = 0;
		for (int i = 0; i < arr.size(); i++)
			sum += arr[i][x];

		if (sum) { l = x - xOff + 1; break; }
	}

	Bounds.setPosition(Pos + sf::Vector2f(-border + xOff * sizeX, -border + yOff * sizeY));
	Bounds.setSize(sf::Vector2f(2 * border + sizeX * l, 2 * border + sizeY * w));
}


// static

sf::Texture Block::s_BoxTexture;

void Block::loadBoxTexture(void* data, size_t size)
{
	s_BoxTexture.loadFromMemory(data, size);
	s_BoxTexture.setSmooth(true);
}


