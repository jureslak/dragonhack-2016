#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <chrono>
#include <myo/myo.hpp>
#include <string>

using namespace std;

sf::RenderWindow *window;
mutex *mtx1, *mtx3, *mtx4, *mtx5;
bool game = true;
sf::Sprite *spr;
sf::RectangleShape *rect;
vector<pair<int, int>> bdat, fdat;
int grabbedId = 0, position = 0;
int movCnt = 0;
chrono::high_resolution_clock::time_point hrTimer;
sf::Text *text;
sf::CircleShape *tri;
double avgDev = 0;

#define MYO_CALL_TIME 50
#ifdef _DEBUG
#define PATH "C:\\Users\\milos\\Desktop\\"
#else
#define PATH ""
#endif

void get_input_data(std::vector<std::vector<int>>& balls) {
	for (auto& x : balls) {
		int temp;
		std::cin >> temp;
		if (temp == 0) continue;
		x.resize(temp);
		for (auto& y : x) {
			std::cin >> y;
		}
	}
}

void utility_print(std::vector<std::vector<int>>& balls, std::vector<int>& towers, int pos, int grab) {
	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	if (grab > 0) {
		for (int i = 0; i < pos; i++) std::cout << "  ";
		std::cout << grab << "\n\n";
	}
	for (int i = 0; i < towers.back(); i++) {
		for (int j = 0; j < balls.size(); j++) {
			if (towers.back() - i > balls[j].size()) {
				std::cout << "  ";
			}
			else {
				std::cout << balls[j][towers.back() - 1 - i] << " ";
			}
		}
		std::cout << "\n";
	}
}

class DataCollector : public myo::DeviceListener {
public:
	DataCollector() {
		std::cin >> max >> bcount;
		bdat.resize(bcount);
		fdat.resize(bcount);
		onArm = false;
		isUnlocked = false;
		currentPose = myo::Pose::unknown;
		pos = 0;
		from = 0;
		moveCount = 0;
		in_hand = 0;
		sum_deviation = 0;
		num_data_points = 0;
		balls.resize(max);
		target.resize(max);
		towers.resize(max);
		for (int i = 0; i < towers.size(); i++) {
			towers[i] = 2 + i;
		}
		get_input_data(balls);
		get_input_data(target);
		grabbed = false;
		start = chrono::high_resolution_clock::now();
		mtx4->lock();
		for (int i = 0; i < max; i++) {
			for (int j = 0; j < balls[i].size(); j++) {
				bdat[balls[i][j] - 1] = make_pair(80 + i * 230, 640 - j * 180);
			}
		}
		for (int i = 0; i < max; i++) {
			for (int j = 0; j < target[i].size(); j++) {
				fdat[target[i][j] - 1] = make_pair(880 + i * 230, 640 - j * 180);
			}
		}
		hrTimer = start;
		mtx4->unlock();
	}
	void onUnpair(myo::Myo* myo, uint64_t timestamp) {
		onArm = false;
		isUnlocked = false;
	}
	void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& acc) {
		sum_deviation += acc.x() * acc.x() + acc.y() * acc.y() + acc.z() * acc.z();
		num_data_points++;
		avg_deviation = sum_deviation / num_data_points;
		mtx5->lock();
		avgDev = avg_deviation;
		mtx5->unlock();
	}
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
		currentPose = pose;
		if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
			myo->unlock(myo::Myo::unlockHold);
			myo->notifyUserAction();
			if (pose == myo::Pose::fist) {
				if (!grabbed && balls[pos].size() > 0) {
					in_hand = balls[pos].back();
					from = pos;
					balls[pos].pop_back();
					grabbed = !grabbed;
				}
				else if (grabbed && balls[pos].size() < towers[pos]) {
					balls[pos].push_back(in_hand);
					auto temptime = chrono::high_resolution_clock::now();
					chrono::duration<double> diff = temptime - start;
					std::cout << from + 1 << ' ' << pos + 1 << ' ' << in_hand << ' ' << diff.count() << '\n';
					in_hand = 0;
					from = 0;
					moveCount++;
					grabbed = !grabbed;
				}
				mtx4->lock();
				for (int i = 0; i < max; i++) {
					for (int j = 0; j < balls[i].size(); j++) {
						bdat[balls[i][j] - 1] = make_pair(80 + i * 230, 640 - j * 180);
					}
				}
				movCnt = moveCount;
				mtx4->unlock();
			}
			else if (pose == myo::Pose::waveIn) { //LEVO
				if (pos != 0) {
					pos--;
				}
			}
			else if (pose == myo::Pose::waveOut) { //DESNO
				if (pos != max - 1) {
					pos++;
				}
			}
			//utility_print(balls, towers, pos, in_hand);
			mtx4->lock();
			grabbedId = in_hand;
			position = pos;
			if (in_hand != 0) bdat[in_hand - 1] = make_pair(80 + pos * 230, 20);
			mtx4->unlock();
		}
		else {
			myo->unlock(myo::Myo::unlockHold);
		}
	}
	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState) {
		onArm = true;
		whichArm = arm;
	}
	void onArmUnsync(myo::Myo* myo, uint64_t timestamp) { //ERROR
		onArm = false;
	}
	void onUnlock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = true;
	}
	void onLock(myo::Myo* myo, uint64_t timestamp) {
		myo->unlock(myo::Myo::unlockHold);
	}
	bool onArm;
	myo::Arm whichArm;
	bool isUnlocked;
	myo::Pose currentPose;
	int pos;
	int max;
	int bcount;
	int moveCount;
	int in_hand;
	int from;
	double sum_deviation;
	double num_data_points;
	double avg_deviation;
	bool grabbed;
	std::vector<std::vector<int>> balls, target;
	std::vector<int> towers;
	chrono::high_resolution_clock::time_point start;
};

bool eval(DataCollector& collector) {
	if (collector.balls.size() != collector.target.size()) return false;
	for (int i = 0; i < collector.balls.size(); i++) {
		if (collector.balls[i].size() != collector.target[i].size()) return false;
		for (int j = 0; j < collector.balls[i].size(); j++) {
			if (collector.balls[i][j] != collector.target[i][j]) return false;
		}
	}
	return true;
}

void myoRoutine() {
	myo::Hub hub("com.example.hello-myo");
	myo::Myo* myo = hub.waitForMyo(10000);
	if (!myo) {
		return;
	}
	DataCollector collector;
	hub.addListener(&collector);

	while (!(mtx1->try_lock())) {
		hub.run(MYO_CALL_TIME);
		if (eval(collector)) {
			mtx3->lock();
			game = false;
			mtx3->unlock();
			myo->unlock(myo::Myo::unlockTimed);
			cout << -1 << " " << avgDev;
			break;
		}
	}

	mtx3->lock();
	if(game) mtx1->unlock();
	mtx3->unlock();
	return;
}

void draw() {
	mtx4->lock();
	mtx5->lock();
	mtx3->lock();

	for (int i = 0; i < 8; i++) {
		window->draw(rect[i]);
	}

	for (int i = 0; i < 4; i++) {
		spr[i].setPosition(sf::Vector2f((float)bdat[i].first, (float)bdat[i].second));
		window->draw(spr[i]);

		spr[i + 4].setPosition(sf::Vector2f((float)fdat[i].first, (float)fdat[i].second));
		window->draw(spr[i + 4]);
	}

	if (game) {
		if (!grabbedId) {
			tri->setPosition(sf::Vector2f(220.0f + position * 230.0f, 70.0f));
			window->draw(*tri);
		}

		auto temporary = chrono::high_resolution_clock::now();
		chrono::duration<double> diff = temporary - hrTimer;
		string temp_string = to_string(diff.count());
		int temp_cnt = 0;
		for (int i = 0; i < temp_string.size(); i++) {
			temp_cnt++;
			if (temp_string[i] == '.') break;
		}
		while (temp_string.size() > temp_cnt + 2) temp_string.pop_back();
		text[0].setString("Time: " + temp_string + " s");
		text[1].setString("Moves: " + to_string(movCnt));
		temp_cnt = 0;
		temp_string = to_string(avgDev);
		for (int i = 0; i < temp_string.size(); i++) {
			temp_cnt++;
			if (temp_string[i] == '.') break;
		}
		while (temp_string.size() > temp_cnt + 3) temp_string.pop_back();
		text[4].setString("AAD: " + temp_string + " ");
	}
	else {
		window->draw(text[2]);
	}
	window->draw(text[0]);
	window->draw(text[1]);
	window->draw(text[3]);
	window->draw(text[4]);

	mtx3->unlock();
	mtx5->unlock();
	mtx4->unlock();
}

int main() {
	thread *myoThread = nullptr;
	mtx1 = new mutex();
	mtx3 = new mutex();
	mtx4 = new mutex();
	mtx5 = new mutex();
	mtx1->lock();

	bool running = true;

	spr = new sf::Sprite[8];
	sf::Texture tex[4];
	for (int i = 0; i < 4; i++) {
		string temp = PATH + string("images\\ball") + to_string(i+1) + "-trans.png";
		if (!tex[i].loadFromFile(temp)) {
			running = false;
			break;
		}
		else {
			tex[i].setRepeated(false);
			tex[i].setSmooth(true);
			spr[i].setTexture(tex[i]);
			spr[i].setScale(sf::Vector2f(0.517f, 0.517f));
			spr[i + 4].setTexture(tex[i]);
			spr[i + 4].setScale(sf::Vector2f(0.517f, 0.517f));
		}
	}

	rect = new sf::RectangleShape[8];
	rect[0].setSize(sf::Vector2f(1600.0f, 80.0f));
	rect[0].setPosition(sf::Vector2f(0.0f, 820.0f));
	rect[0].setFillColor(sf::Color(139, 69, 19, 255));
	rect[1].setSize(sf::Vector2f(60.0f, 460.0f));
	rect[1].setPosition(sf::Vector2f(140.0f, 440.0f));
	rect[1].setFillColor(sf::Color(139, 69, 19, 255));
	rect[2].setSize(sf::Vector2f(60.0f, 640.0f));
	rect[2].setPosition(sf::Vector2f(370.0f, 260.0f));
	rect[2].setFillColor(sf::Color(139, 69, 19, 255));
	rect[3].setSize(sf::Vector2f(60.0f, 820.0f));
	rect[3].setPosition(sf::Vector2f(600.0f, 80.0f));
	rect[3].setFillColor(sf::Color(139, 69, 19, 255));

	rect[4].setSize(sf::Vector2f(30.0f, 900.0f));
	rect[4].setPosition(sf::Vector2f(785.0f, 0.0f));
	rect[4].setFillColor(sf::Color(30, 30, 30, 255));
	rect[5].setSize(sf::Vector2f(60.0f, 460.0f));
	rect[5].setPosition(sf::Vector2f(940.0f, 440.0f));
	rect[5].setFillColor(sf::Color(139, 69, 19, 255));
	rect[6].setSize(sf::Vector2f(60.0f, 640.0f));
	rect[6].setPosition(sf::Vector2f(1170.0f, 260.0f));
	rect[6].setFillColor(sf::Color(139, 69, 19, 255));
	rect[7].setSize(sf::Vector2f(60.0f, 820.0f));
	rect[7].setPosition(sf::Vector2f(1400.0f, 80.0f));
	rect[7].setFillColor(sf::Color(139, 69, 19, 255));

	sf::Font font;
	if (!font.loadFromFile(PATH + string("arial.ttf")))
	{
		running = false;
	}
	text = new sf::Text[5];
	text[0].setFont(font);
	text[0].setString("Time: 0 s");
	text[0].setCharacterSize(24);
	text[0].setColor(sf::Color::White);
	text[0].setPosition(sf::Vector2f(40.0f, 850.0f));
	text[1].setFont(font);
	text[1].setString("Moves: 0");
	text[1].setCharacterSize(24);
	text[1].setColor(sf::Color::White);
	text[1].setPosition(sf::Vector2f(320.0f, 850.0f));
	text[2].setFont(font);
	text[2].setString("Success!");
	text[2].setCharacterSize(50);
	text[2].setColor(sf::Color::Black);
	text[2].setPosition(sf::Vector2f(300.0f, 30.0f));
	text[3].setFont(font);
	text[3].setString("Goal");
	text[3].setCharacterSize(50);
	text[3].setColor(sf::Color::Black);
	text[3].setPosition(sf::Vector2f(1145.0f, 30.0f));
	text[4].setFont(font);
	text[4].setString("AAD: 0 ");
	text[4].setCharacterSize(24);
	text[4].setColor(sf::Color::White);
	text[4].setPosition(sf::Vector2f(600.0f, 850.0f));

	tri = new sf::CircleShape(50, 3);
	tri->setPosition(sf::Vector2f(0.0f, 0.0f));
	tri->setRotation(180);
	tri->setFillColor(sf::Color(150, 150, 150, 255));

	if (running) {
		myoThread = new thread(myoRoutine);

		window = new sf::RenderWindow(sf::VideoMode(1600, 900, 32), "Jimm?", sf::Style::Fullscreen);
		window->setFramerateLimit(60);
		window->setKeyRepeatEnabled(false);
	}

	while (running) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				running = false;
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					running = false;
				}
			}
		}

		window->clear(sf::Color::White);
		draw();
		window->display();
	}

	mtx1->unlock();
	if(myoThread) { if(myoThread->joinable()) myoThread->join(); }
	if(myoThread) delete myoThread;
	if(window) delete window;
	delete[] spr;
	delete[] rect;
	delete[] text;
	delete tri;
	delete mtx1;
	delete mtx3;
	delete mtx4;
	delete mtx5;
	return 0;
}