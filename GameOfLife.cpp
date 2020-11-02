#include<iostream>
#include<ctime>
#include<Windows.h>
#include<stack>

const int maxSizeX = 18;
const int maxSizeY = 48;
const int GEN_SEED = 256;

int frame = 0;
char field[maxSizeX + 2][maxSizeY + 2];

void init() {
	for (int i = 0; i < maxSizeX + 2; i++)
		for (int j = 0; j < maxSizeY + 2; j++)
			field[i][j] = '\0';
}

void generate(int rate) {
	for (int i = 1; i < maxSizeX; i++)
		for (int j = 1; j < maxSizeY; j++)
			if(rand() % (maxSizeX*maxSizeY / rate) == 0) field[i][j] = '#';
}

int countNeighs(int x, int y) {
	int counter = 0;
	if (field[x + 1][y + 1] != '\0') counter++;
	if (field[x][y + 1] != '\0') counter++;
	if (field[x - 1][y + 1] != '\0') counter++;
	if (field[x - 1][y] != '\0') counter++;
	if (field[x - 1][y - 1] != '\0') counter++;
	if (field[x][y - 1] != '\0') counter++;
	if (field[x + 1][y - 1] != '\0') counter++;
	if (field[x + 1][y] != '\0') counter++;
	return counter;
}

void paint(int x, int y) { field[x][y] = '#'; }

void kill(int x, int y) { field[x][y] = '\0'; }

long long update() {

	long long hash = 0;
	std::stack<std::pair<int, int>> paintTaskStack;
	std::stack<std::pair<int, int>> killTaskStack;

	for (int i = 1; i < maxSizeX; i++) {
		for (int j = 1; j < maxSizeY; j++) {
			int neighs = countNeighs(i, j);
			if (field[i][j] == '#') {
				hash += (long long)i + (long long)j;
				if (!(neighs == 2 || neighs == 3)) 
					killTaskStack.push(std::make_pair(i, j));
			}
			else if (neighs == 3) 
				paintTaskStack.push(std::make_pair(i, j)); 
		}
	}
	
	hash *= (paintTaskStack.size() + hash * hash) / (killTaskStack.size() + 1);

	while (!paintTaskStack.empty()) { 
		paint(paintTaskStack.top().first, paintTaskStack.top().second);
		paintTaskStack.pop();
	}
	while (!killTaskStack.empty()) {
		kill(killTaskStack.top().first, killTaskStack.top().second);
		killTaskStack.pop();
	}

	return hash;
}

void print() {
	for (int i = 1; i < maxSizeX; i++) {
		for (int j = 1; j < maxSizeY; j++) {
			std::cout << field[i][j];
		}
		std::cout << "\n";
	}
}

void report(long long hash) {
	std::cout << "\rframe: " << frame << ", hash: " << hash << "\n";
}

void render() {
	long long hash = -1;
	while (true) {
		long long newHash = update();
		if (hash == newHash) {
			Sleep(1000);
			generate(GEN_SEED);
			//cout << "$ ";
			//string command; cin >> command;
			//if (command == "e") break;
			//if (command == "r") generate(GEN_SEED);
		}
		system("cls");
		report(hash);
		print();
		frame++;
		hash = newHash;
	}
}

void main() {
	srand(time(NULL));
	init(); 
	generate(GEN_SEED);
	render();
}