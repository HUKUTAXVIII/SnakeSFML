#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/System/String.hpp>
#include <string>

#define CELL 16
#define WIDTH 16*30
#define HEIGHT 16*30

using namespace sf;

enum Dir {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

class Fruit : public RectangleShape {
protected:
	int weight;
public:
	Fruit() :RectangleShape()
	{
		this->setSize(Vector2f(CELL, CELL));
		this->weight = 0;
	}
	int getScore() {
		return this->weight;
	}
	~Fruit()
	{

	}
};


class Apple : public Fruit {
public:
	Apple() : Fruit()
	{
		this->weight = 1;
		this->setFillColor(Color::Red);
	}
	Apple(float x, float y) : Apple()
	{
		this->setPosition(x, y);
	}



	~Apple()
	{

	}

};

class Banana : public Fruit {
public:
	Banana() : Fruit()
	{
		this->weight = 3;
		this->setFillColor(Color::Yellow);
	}
	Banana(float x, float y) : Banana()
	{
		this->setPosition(x, y);
	}

	~Banana()
	{

	}

};

class Peach : public Fruit {
public:
	Peach() : Fruit()
	{
		this->weight = 5;
		this->setFillColor(Color::Magenta);
	}
	Peach(float x, float y) : Peach()
	{
		this->setPosition(x, y);
	}

	~Peach()
	{

	}

};


void RandomFruit(Fruit& fruit) {

	int type = rand() % 3;
	int x = rand() % (WIDTH / CELL);
	int y = rand() % (HEIGHT / CELL);

	x *= CELL;
	y *= CELL;


	switch (type)
	{
	case 0:
		fruit = Apple(x, y);
		break;
	case 1:
		fruit = Banana(x, y);
		break;
	case 2:
		fruit = Peach(x, y);
		break;

	default:
		break;
	}



}


class Snake : public std::vector<RectangleShape> {
private:
	Dir dir;
	int score;
	bool isAlive;
public:
	Snake() :std::vector<RectangleShape>()
	{
		this->dir = NONE;
		this->score = 0;
		this->push_back(RectangleShape());
		this->begin()->setFillColor(Color::Green);
		this->begin()->setSize(Vector2f(CELL, CELL));
		this->begin()->setPosition(Vector2f(WIDTH / 2, HEIGHT / 2));
		this->isAlive = true;
	}

	void Move() {

		if (Keyboard::isKeyPressed(Keyboard::A)) {
			this->dir = LEFT;
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			this->dir = RIGHT;
		}
		else if (Keyboard::isKeyPressed(Keyboard::W)) {
			this->dir = UP;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) {
			this->dir = DOWN;
		}

		Vector2f past = this->begin()->getPosition();

		switch (this->dir)
		{
		case UP:
			this->begin()->move(0,-CELL);
			break;
		case DOWN:
			this->begin()->move(0, CELL);
			break;
		case LEFT:
			this->begin()->move(-CELL,0);
			break;
		case RIGHT:
			this->begin()->move(CELL, 0);
			break;
		case NONE:

			break;
		}
		if (this->begin()->getPosition().x < 0) {
			this->begin()->move(WIDTH,0);
		}
		if (this->begin()->getPosition().x > WIDTH) {
			this->begin()->move(-(WIDTH+CELL), 0);
		}
		if (this->begin()->getPosition().y > HEIGHT) {
			this->begin()->move(0, -(WIDTH + CELL));
		}
		if (this->begin()->getPosition().y < 0) {
			this->begin()->move(0, HEIGHT);
		}


		for (std::vector<RectangleShape>::iterator i = this->begin(); i != this->end(); i++) {
			if (i->getPosition().x != this->begin()->getPosition().x || i->getPosition().y != this->begin()->getPosition().y) {
				Vector2f tmp = i->getPosition();
				i->setPosition(past);
				past = tmp;
			}
		}
	}

	void AddToBody() {
		this->push_back(*(this->end()-1));
	}
	void CheckBodyCollision() {
		for (std::vector<RectangleShape>::iterator i = this->begin()+1; i != this->end(); i++) {
			if (this->begin()->getPosition() == i->getPosition()) {
				this->isAlive = false;
			}
		}
	}

	void FruitCollision(Fruit& fruit) {
		for (std::vector<RectangleShape>::iterator i = this->begin(); i != this->end(); i++) {
			if (fruit.getPosition().x == i->getPosition().x && fruit.getPosition().y == i->getPosition().y) {
				this->score += fruit.getScore();
				this->AddToBody();
				RandomFruit(fruit);
				return;
			}
		}
	}

	bool getIsAlive() {
		return this->isAlive;
	}
	int getScore() {
		return this->score;
	}

	~Snake()
	{

	}
};






int main(int argc, char* argv[]) {

	std::srand(std::time(0));
	RenderWindow window(VideoMode(WIDTH,HEIGHT),"Snake");
	Snake snake;

	Font font;
	
	font.loadFromFile("ReadexPro.ttf");
	Text score;
	score.setFont(font);
	score.setCharacterSize(16);
	score.setFillColor(Color(0,0,0));
	score.setString("Score: 0");

	Fruit f;
	RandomFruit(f);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}
		}

		snake.FruitCollision(f);
		snake.Move();
		snake.CheckBodyCollision();
		if (!snake.getIsAlive()) {
			snake = Snake();
		}
		std::string tos = std::to_string(snake.getScore());
		score.setString("Score: "+tos);

		window.clear(Color(255,255,255));


		window.draw(score);

		window.draw(f);

		for (auto i : snake) {
			window.draw(i);
		}
		
		window.display();

		sleep(milliseconds(100));
	}



	return 0;
}