#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

class MySquadMatrix {
private:
	double** squadMatrix;
	int size;
public:
	MySquadMatrix();
	MySquadMatrix(int);
	~MySquadMatrix();
	void resize(int);
	double** getMatrix() { return squadMatrix; }
	int getSize() { return size; }
	MySquadMatrix& operator=(const MySquadMatrix&);
};

MySquadMatrix::MySquadMatrix() {
	squadMatrix = nullptr;
	size = 0;
}

MySquadMatrix::MySquadMatrix(int size)
{
	squadMatrix = new double* [size];
	for (int i = 0; i < size; i++) {
		squadMatrix[i] = new double[size];
	}
	this->size = size;
}

MySquadMatrix::~MySquadMatrix()
{
	for (int i = 0; i < size; i++) {
		delete[] squadMatrix[i];
	}
	delete[] squadMatrix;
}

void MySquadMatrix::resize(int size) {
	if (!size) {
		squadMatrix = nullptr;
		return;
	}
	double** newSquad = new double* [size];
	for (int i = 0; i < size; i++) {
		newSquad[i] = new double[size];
	}
	if (!this->size) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				newSquad[i][j] = 0;
			}
		}
		squadMatrix = newSquad;
		this->size = size;
		return;
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i < this->size && j < this->size) {
				newSquad[i][j] = squadMatrix[i][j];
			}
			else {
				newSquad[i][j] = 0;
			}
		}
	}
	for (int i = 0; i < this->size; i++) {
		delete[] squadMatrix[i];
	}delete[] squadMatrix;
	squadMatrix = newSquad;
	this->size = size;
}

MySquadMatrix& MySquadMatrix::operator=(const MySquadMatrix& other)
{
	if (other.size) {
		this->squadMatrix = nullptr;
	}
	double** newSquad = new double* [other.size];
	for (int i = 0; i < size; i++) {
		newSquad[i] = new double[other.size];
	}
	for (int i = 0; i < other.size; i++) {
		for (int j = 0; j < other.size; j++) {
			newSquad[i][j] = other.squadMatrix[i][j];
		}
	}
	for (int i = 0; i < this->size; i++) {
		delete[] squadMatrix[i];
	}
	delete[] squadMatrix;
	squadMatrix = newSquad;
	this->size = size;

	return *this;
}

class Vertex {
private:
	string name;
	double weight;
	int num;
public:
	Vertex() {}
	Vertex(string name, double weight, int num) {
		this->name = name;
		this->weight = weight;
		this->num = num;
	}
	string getName() { return name; }
	double getWeight() { return weight; }
	int getNum() { return num; }
	void setWeight(int weight) { this->weight = weight; }
};

class Graph {
private:
	int vertices;
	MySquadMatrix adjacencyMatrix;
	MySquadMatrix evristicMatrix;
	string cities[15];
	vector<int> path;
	int lenght;
	vector<bool> used;
public:
	Graph();
	int getVertices() { return vertices; }
	void fillingAdjacencyMatrix();
	void fillingEvristicMatrix();
	bool a_Star(int, int);
	bool greedySearch(int, int);
	int sumWeigth();
};

Graph::Graph()
{
	vertices = 15;
	evristicMatrix.resize(15);
	adjacencyMatrix.resize(15);
	cities[0] = "Pattaya";
	cities[1] = "Bangkok";
	cities[2] = "Nonthaburi";
	cities[3] = "Pakkret";
	cities[4] = "Hatyay";
	cities[5] = "Chiang Mai";
	cities[6] = "Surrathani";
	cities[7] = "Khonken";
	cities[8] = "Lampang";
	cities[9] = "Rayong";
	cities[10] = "Ayuthaya";
	cities[11] = "Kanchanaburi";
	cities[12] = "Udonthani";
	cities[13] = "Sukhotay";
	cities[14] = "Lampun";
}

void Graph::fillingAdjacencyMatrix()
{
	double staticMatrix[15][15] = {
		//	 0    1   2   3   4   5   6   7   8   9  10  11  12  13  14
			{ 0 ,147, 0 , 0 , 0 , 0 , 0 ,513, 0 , 54, 0 , 0 , 0 , 0 , 0 },//0
			{147, 0 , 19, 0 ,944, 0 ,651, 0 , 0 ,176, 0 ,144, 0 , 0 , 0 },//1
			{ 0 , 19, 0 , 12, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 },//2
			{ 0 , 0 , 12, 0 , 0 , 0 , 0 ,437, 0 , 0 , 66,142, 0 , 0 , 0 },//3
			{ 0 ,944, 0 , 0 , 0 , 0 ,322, 0 , 0 , 0 , 7 ,972, 0 , 8 , 0 },//4
			{ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 33},//5
			{ 0 ,651, 0 , 0 ,322, 0 , 0 , 0 , 0 , 0 , 0 ,668, 0 , 0 , 0 },//6
			{513, 0 , 0 ,437, 0 , 0 , 0 , 0 ,545,529,406,547,114,378, 0 },//7
			{ 54, 0 , 0 , 0 , 0 , 0 , 0 ,545, 0 , 0 ,533,596,513,209, 74},//8
			{ 0 ,176, 0 , 0 , 0 , 0 , 0 ,529, 0 , 0 , 0 , 0 , 0 , 0 , 0 },//9
			{ 0 , 0 , 0 , 66, 7 , 0 , 0 ,406,533, 0 , 0 ,165, 0 ,355,585},//10
			{ 0 ,144, 0 ,142,972, 0 ,668,547,596, 0 ,165, 0 , 0 ,439,654},//11
			{ 0 , 0 , 0 , 0 , 0 , 0 , 0 ,114,513, 0 , 0 , 0 , 0 ,416, 0 },//12
			{ 0 , 0 , 0 , 0 , 8 , 0 , 0 ,378,209, 0 ,355,439,416, 0 , 0 },//13
			{ 0 , 0 , 0 , 0 , 0 , 33, 0 , 0 , 74, 0 ,585,654, 0 , 0 , 0 },//14
	};
	adjacencyMatrix.resize(15);
	vertices = adjacencyMatrix.getSize();
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < vertices; j++) {
			adjacencyMatrix.getMatrix()[i][j] = staticMatrix[i][j];
		}
	}
}

void Graph::fillingEvristicMatrix()
{
	double staticMatrix[15][15] = {
		//	 0     1    2    3    4    5    6    7    8	   9   10   11   12   13   14
			{ 0  ,100 ,110 ,117 ,660 ,680 ,457 ,444 ,614 ,38.4,163 ,205 ,535 ,470 ,660 },//0
			{100 , 0  ,11.7,17.4,746 ,580 ,368 ,390 ,515 ,137 ,69.5,123 ,472 ,369 ,558 },//1
			{110 ,11.7, 0  ,6.6 ,762 ,575 ,542 ,380 ,503 ,143 ,57.1,122 ,460 ,357 ,547 },//2
			{117 ,17.4,6.6 , 0  ,766 ,572 ,547 ,378 ,499 ,150 ,51.9,119 ,457 ,351 ,541 },//3
			{660 ,746 ,762 ,766 , 0  ,1328,268 ,1080,1259,636 ,820 ,799 ,1180,1115,1297},//4
			{680 ,580 ,575 ,572 ,1328, 0  ,1077,485 ,77.2,716 ,519 ,524 ,436 ,217 , 24 },//5
			{457 ,368 ,542 ,547 ,268 ,1077, 0  ,896 ,1019,444 ,600 ,553 ,989 ,876 ,1050},//6
			{444 ,390 ,380 ,378 ,1080,485 ,896 , 0  ,409 ,455 ,334 ,450 ,103 ,326 ,470 },//7
			{614 ,515 ,503 ,499 ,1259,77.2,1019,409 , 0  ,646 ,452 ,465 ,369 ,149 ,59.4},//8
			{38.4,137 ,143 ,150 ,636 ,716 ,444 ,455 ,646 , 0  ,195 ,243 ,548 ,498 ,691 },//9
			{163 ,69.5,57.1,51.9,820 ,519 ,600 ,334 ,452 ,195 , 0  ,130 ,410 ,304 ,496 },//10
			{205 ,123 ,122 ,119 ,799 ,524 ,553 ,450 ,465 ,243 ,130 , 0  ,514 ,326 ,499 },//11
			{535 ,472 ,460 ,457 ,1180,436 ,989 ,103 ,369 ,548 ,410 ,514 , 0  ,320 ,425 },//12
			{470 ,369 ,357 ,351 ,1115,217 ,876 ,326 ,149 ,498 ,304 ,326 ,320 , 0  ,196 },//13
			{660 ,558 ,547 ,541 ,1297, 24 ,1050,470 ,59.4,691 ,496 ,499 ,425 ,196 , 0  },//14
	};
	evristicMatrix.resize(15);
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < vertices; j++) {
			evristicMatrix.getMatrix()[i][j] = staticMatrix[i][j];
		}
	}
}

bool Graph::a_Star(int start, int end)
{
	cout << "(" << cities[start] << " - " << cities[end] << ") ";
	vector<Vertex> closed;
	vector<Vertex> allVertices;
	for (int i = 0; i < vertices; i++) {
		if (i == start) {
			Vertex vertex(cities[i], 0, start);
			allVertices.push_back(vertex);
		}
		else {
			Vertex vertex(cities[i], sumWeigth() * 2, i);
			allVertices.push_back(vertex);
		}
	}
	closed.push_back(allVertices[start]);
	int counter = 0;
	while (counter < closed.size()){
		int curr = closed[counter].getNum();
		if (curr == end) {
			for (int i = 0; i < closed.size(); i++) {
				if (closed.size() - 1 == i) {
					cout << closed[i].getName() << " = " << closed[i].getWeight();
				}
				else {
					cout << closed[i].getName() << "->";
				}
			}
			cout << endl;
			return true;
		}
		int index = 0;
		double min = sumWeigth() * 2;
		for (int i = 0; i < vertices; i++) {
			if (adjacencyMatrix.getMatrix()[curr][i] != 0) {
				bool flag = true;
				for (int j = 0; j < closed.size(); j++) {
					if (i == closed[j].getNum()) flag = false;
				}
				if (evristicMatrix.getMatrix()[i][end] + allVertices[curr].getWeight() + adjacencyMatrix.getMatrix()[curr][i] < min && flag) {
					min = evristicMatrix.getMatrix()[i][end] + allVertices[curr].getWeight() + adjacencyMatrix.getMatrix()[curr][i];
					index = i;
				}
			}
		}
		allVertices[index].setWeight(allVertices[curr].getWeight() + adjacencyMatrix.getMatrix()[curr][index]);
		closed.push_back(allVertices[index]);
		counter++;
	}
	return false;
}

bool Graph::greedySearch(int start, int end)
{
	cout << "(" << cities[start] << " - " << cities[end] << ") ";
	vector<Vertex> closed;
	vector<Vertex> allVertices;
	for (int i = 0; i < 15; i++) {
		if (i == start) {
			Vertex vertex(cities[i], 0, start);
			allVertices.push_back(vertex);
		}
		else {
			Vertex vertex(cities[i], sumWeigth() * 2, i);
			allVertices.push_back(vertex);
		}
	}
	int counter = 0;
	closed.push_back(allVertices[start]);
	while (counter < closed.size()) {
		int curr = closed[counter].getNum();
		if (curr == end) {
			for (int i = 0; i < closed.size(); i++) {
				if (closed.size() - 1 == i) {
					cout << closed[i].getName() << " = " << closed[i].getWeight();
				}
				else {
					cout << closed[i].getName() << "->";
				}
			}
			cout << endl;
			return true;
		}
		int index = 0;
		double min = sumWeigth() * 2;
		for (int i = 0; i < vertices; i++) {
			if (adjacencyMatrix.getMatrix()[curr][i] != 0) {
				bool flag = true;
				for (int j = 0; j < closed.size(); j++) {
					if (i == closed[j].getNum()) flag = false;
				}
				if(evristicMatrix.getMatrix()[i][end] < min && flag){
					min = evristicMatrix.getMatrix()[i][end];
					index = i;
				}
			}
		}
		allVertices[index].setWeight(allVertices[curr].getWeight() + adjacencyMatrix.getMatrix()[curr][index]);
		closed.push_back(allVertices[index]);
		counter++;
	}
	return false;
}

int Graph::sumWeigth()
{
	int sum = 0;
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < vertices; j++) {
			sum += adjacencyMatrix.getMatrix()[i][j];
		}
	}
	return sum;
}


int main() {
	setlocale(LC_ALL, "rus");
	Graph graph;
	graph.fillingAdjacencyMatrix();
	graph.fillingEvristicMatrix();

	for (int i = 0; i < 15; i++) {
		for (int j = i; j < 15; j++) {
			cout << "Greedy search: " << endl;
			graph.greedySearch(i, j);
			cout << "A star: " << endl;
			graph.a_Star(i, j);
		}
	}

	return 0;
}
