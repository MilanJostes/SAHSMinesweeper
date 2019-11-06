#include <iostream>
#include <string>
#include <vector>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <fstream>
#include <sstream>
using namespace std;
//Class that holds the information about the mines and size of the map
class mineSpace {
private:
	int m_mines;
	int m_size;
public:
	mineSpace() {
		mineSpace(10);
	}
	mineSpace(int in) {
		m_mines = in;
		m_size = in;
	}
	mineSpace(int min, int siz) {
		m_mines = min;
		m_size = siz;
	}
	void setMines(int in) {
		m_mines = in;
	}
	void setValues(int in) {
		m_mines = in;
		m_size = in;
	}
	int getMines() {
		return m_mines;
	}
	int getSize() {
		return m_size;
	}
};
//Vectors are a holy grail for this program
vector<vector<int>> setMines(vector<vector<int>>);
vector<vector<int>> setNums(const vector<vector<int>>);
vector<vector<int>> minefieldMap;
vector<vector<int>> playerMap;
vector<vector<int>> minefield(int);
vector<vector<int>> difficulty();
vector<vector<int>> resetMap(vector<vector<int>>);
void checkDirections(const vector<vector<int>> in, const int x, const int y, const string origin);
void displayMinefield(vector<vector<int>> in);
void playerPlay(vector<vector<int>> in);
bool checkMove(vector<vector<int>> in, int x, int y, string origin);
bool checkForWin();

mineSpace m;
int val;
bool win = false;
bool flag = false;
bool firstTurn = true;
int main() {
	//Allows player to set difficulty
	playerMap = difficulty();
	displayMinefield(playerMap);
	//Sets the values
	minefieldMap = setMines(playerMap);
	minefieldMap = setNums(minefieldMap);
	playerMap = resetMap(playerMap);
	//Developers cheat system
	//displayMinefield(minefieldMap);
	//displayMinefield(playerMap);
	//win = true;
	//Checks whether you won or lost
	while (val < 10 && !win) {
		int choice = 3;
		win = checkForWin();
		if (!win) {
			displayMinefield(playerMap);
			while (choice > 2 || choice < 1) {
				//Player can choose to play or flag a square
				cout << "Type 1 to play a square\n2 to flag a square\n";
				cin >> choice;
				if (choice == 2) {
					flag = true;
				}
				else {
					flag = false;
				}
			}
		}
		win = checkForWin();
		if(!win)
			//Players turn is processed
			playerPlay(minefieldMap);
	}
	//This could be done easier with cout, but I wan't to prove I can do it
	ifstream fin("MineText.txt");
	istringstream sstr;
	string out;
	//Pointer to just prove I can
	int choice = 0;
	int* temp = &choice;
	//Outputs winner message
	if (win) {
		displayMinefield(minefieldMap);
		while (!fin.eof()) {
			getline(fin, out);
			if (!out.find("Winner")) {
				*temp = 1;
			}
			else if (out == ";") {
				cout << "\n";
				*temp = 0;
			}
			else if (*temp == 1) {
				sstr.clear();
				sstr.str(out);
				cout << out<<endl;
			}
		}
	}
	else {
		//outputs the lost message
		while (!fin.eof()) {
			getline(fin, out);
			if (!out.find("Lose")) {
				*temp = 1;
			}
			else if (out == ";") {
				cout << "\n";
				*temp = 0;
			}
			else if (*temp == 1) {
				sstr.clear();
				sstr.str(out);
				cout << out<<endl;
			}
		}
	}
	return 1;
}

vector<vector<int>> difficulty() {
	cout << "What difficulty would you like?\n1 Easy\n2 Medium\n3 Hard\n4 Custom" << endl;
	int in;
	cin >> in;
	//Sets difficulty to easy
	vector<vector<int>> field;
	if (in == 1) {
		field = minefield(10);
		m.setValues(10);
		m.setMines(10);
	}
	//Sets difficulty to medium
	else if (in == 2) {
		field = minefield(25);
		m.setValues(25);
		m.setMines(100);
	}
	//Sets difficulty to hard
	else if (in == 3) {
		field = minefield(50);
		m.setValues(50);
		m.setMines(250);
	}
	//Sets difficulty to custom
	else if (in == 4) {
		cout << "Enter the size of your custom minefield (value * value): " << endl;
		cin >> in;
		field = minefield(in);
		m.setValues(in);
		cout << "Enter the amount of mines you want: "<<endl;
		cin >> in;
		m.setMines(in);
	}
	return field;
}

vector<vector<int>> setMines(vector<vector<int>> in) {
	//Places the mines in random locations on the field
	srand(time(NULL));
	for (int temp = 0; temp < m.getMines();temp++) {
		int x = rand() % m.getSize();
		int y = rand() % m.getSize();
		if (in[x][y] != 10) {
			in[x][y] = 10;
		}
		else {
			temp--;
		}
	}
	cout << endl;
	return in;
}

vector<vector<int>> minefield(int in) {
	//Just initializes the 2d vector
	//vector<vector <mineSpace>> field[in1][in2];
	vector<int> v2(in, 0);
	vector<vector<int> > v2d2(in, v2);
	return v2d2;
}

void displayMinefield(vector<vector<int>> in) {
	//Displays the minefield as well as the coordinate system
	cout << "\t";
	for (int i = 0;i < in.size(); i++) {
		if (i > 9) {
			cout << i << " ";
		}else {
			cout << i << "  ";
		}
	}
	cout << endl<<endl;
	for (int i = 0;i < in.size(); i++) {
		cout << i << "\t";
		for (int j = 0;j < in[i].size(); j++) {
			if (in[i][j] == 11) {
				cout << "#" << "  ";
			}
			else if(in[i][j] == 14) {
				cout << "F" << "  ";
			}
			else if (in[i][j] == 0) {
				cout << "O" << "  ";
			}
			else {
				cout << in[i][j] << "  ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

void playerPlay(vector<vector<int>> in) {
	//Gets coordinates that are valid
	cout << "Put in coordinates\n";
	/*getInts();
	int x = x_coord;
	int y = y_coord;*/
	int x = -1;
	int y = -1;
		while (x<0 || x>playerMap.size()) {
			cout << "Type in a valid x coordinate: ";
			cin >> x;
		}
		while (y<0 || y>playerMap.size()) {
			cout << "Type in a valid y coordinate: ";
			cin >> y;
		}
	if (!flag) {
		//If player doesn't want to flag it plays through the move
		playerMap[y][x] = in[y][x];
		if (in[y][x] == 11)
			playerMap[y][x] = 0;
		if (in[y][x] == 10) {
			val = 10;
		}
		else {
			//Checks directions of the space if it is 0
			checkDirections(in, x, y, "none");
		}
	}
	else {
		//If the player wants to flag a space it sets it to a flag
		playerMap[y][x] = 14;
	}
}

vector<vector<int>> setNums(vector<vector<int>> in) {
	//This is a bit of reformatting that makes the system run much smoother, 11 is an unseen empty space, 10 is a mine
	//14 is a flagged space, and 0 is a seen blank space
	for (int i = 0;i < in.size(); i++) {
		for (int j = 0;j < in[i].size(); j++) {
			if (in[i][j] == 10) {
				if (i > 0) {
					if(in[i - 1][j]!=10)
					in[i - 1][j] += 1;
					if (j > 0) {
						if (in[i-1][j - 1] != 10)
							in[i-1][j - 1] += 1;
					}
				}
				if (i < m.getSize()-1) {
					if (in[i + 1][j] != 10)
					in[i + 1][j] += 1;
					if (j < m.getSize()-1) {
						if (in[i + 1][j + 1] != 10)
							in[i + 1][j + 1] += 1;
					}
				}
				if (j > 0) {
					if (in[i][j-1] != 10)
					in[i][j-1] += 1;
					if (i < m.getSize()-1) {
						if (in[i + 1][j - 1] != 10)
							in[i + 1][j - 1] += 1;
					}

				}
				if (j < m.getSize()-1) {
					if (in[i][j + 1] != 10)
					in[i][j+1] += 1;
					if (i > 0) {
						if (in[i - 1][j + 1] != 10)
							in[i - 1][j + 1] += 1;
					}
				}
			}
		}
	}
	for (int i = 0;i < in.size(); i++) {
		for (int j = 0;j < in[i].size(); j++) {
			if (in[i][j] == 0) {
				in[i][j] = 11;
			}
		}
	}
	return in;
}

vector<vector<int>> resetMap(vector<vector<int>> in) {
	//Switches out 0's for 11 to make the previous system work correctly
	for (int i = 0;i < in.size(); i++) {
		for (int j = 0;j < in[i].size(); j++) {
			if (in[i][j] == 0) {
				in[i][j] = 11;
			}
		}
	}
		return in;
}

//Checks the move and plays it
bool checkMove(vector<vector<int>> in, int x, int y, string origin = "none") {
//	cout << "test";
	//if (y > 0) {
	//	if (playerMap[y][x] == 11) {
			int temp = playerMap[y][x];
				//playerMap[y][x] = in[y][x];
			//if it's an empty space it sets it to a seen empty space
			if (minefieldMap[y][x] == 11) {
				playerMap[y][x] = 0;
				minefieldMap[y][x] = 0;
				//playerMap[y][x] = in[y][x];
				checkDirections(in, x, y,origin);
			}
			else if (in[y][x] <=7&&in[y][x]>=1) {
				//Sets the field to what it should be
				playerMap[y][x] = minefieldMap[y][x];
			}
	//	}
	//}
	return false;
}

//Checks directions of the empty space to open up areas of 0's like in real minesweeper
void checkDirections(const vector<vector<int>> in,const int x, const int y, const string origin = "none") {
	if (in[y][x] == 11) {

		if (y > 0) {
			if(origin!="up")//Each of these statements stop the program from going to the same blank area and causing an infinite loop
			checkMove(in, x, (y - 1), "down");
		}
		if (x < in.size() - 1) {
			if (origin != "right")
			checkMove(in, (x + 1), y, "left");
		}
		if (y < in.size() - 1) {
			if (origin != "down")
			checkMove(in, x, (y + 1), "up");
		}
		if (x > 0) {
			if (origin != "left")
			checkMove(in, (x - 1), y, "right");
		}
	}
}

bool checkForWin() {
	//checks for if the flagged areas all line up with the mine areas
	int found = 0;
	for (int i = 0;i < playerMap.size(); i++) {
		for (int j = 0;j < playerMap[i].size(); j++) {
			if (minefieldMap[i][j] == 10) {
				if (playerMap[i][j] == 14) {
					found++;
				}
				else {
					return false;
				}
			}

		}
	}
	if(found == m.getMines())
	return true;
}

/* 
//This was a throw exception, but I forgot that it just goes back to the main, so it created an infinite loop, and I don't have the time to fix it before it is due
void getInts() {
	try {
		while (x_coord<0 || x_coord>playerMap.size()) {
			cout << "x: ";
			cin >> x_coord;
			if (x_coord<0 || x_coord>playerMap.size()) {
				throw "Nope";
			}

		}
		while (y_coord<0 || y_coord>playerMap.size()) {
			cout << "y: ";
			cin >> y_coord;
			if (y_coord<0 || y_coord>playerMap.size()) {
				throw "Nope";
			}
		}
	}
	catch (...) {
		cout << "Please enter correct coordinates" << endl;
		getInts();
	}
}*/