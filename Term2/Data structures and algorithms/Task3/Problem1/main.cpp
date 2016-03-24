#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <climits>
#include <stdexcept>
#include <assert.h>

using namespace std;

struct Coords
{
    size_t i, j;
    Coords() {}
    Coords(size_t in_i, size_t in_j): i(in_i), j(in_j) {}
    Coords(const Coords &coords): i(coords.i), j(coords.j) {}
    
    bool operator == (const Coords &coords) const
    {
        return (i == coords.i && j == coords.j);
    }
};

struct Cell
{
    Coords coords;
    int distance;
    Cell(){}
    Cell(size_t i, size_t j, int in_dist): coords(i, j), distance(in_dist) {}
    Cell(const Coords in_coords, int in_dist): coords(in_coords), distance(in_dist) {}
    Cell(const Cell &cell): coords(cell.coords), distance(cell.distance) {}
};

class Hasher
{
public:
    size_t operator()(const Coords coords) const
    {
        return coords.i * 179425423 + coords.j;
    }
};

struct MapNode
{
    int position;
    bool deleted;
    short used;
    MapNode(): position(0), deleted(false), used(0) {}
    MapNode(size_t in_position, bool in_deleted, short in_used):
        position(in_position), deleted(in_deleted), used(in_used) {}
    MapNode(const MapNode &node):
        position(node.position), deleted(node.deleted), used(node.used) {}    
};

class Heap
{


    vector<Cell> heap;    
    unordered_map<Coords, MapNode, Hasher> map;
    size_t n, m;
    int INF = INT_MAX;
    
    void Heapify(size_t ind, unordered_map<Coords, MapNode, Hasher>::iterator parent_iter)
    {
        size_t left = 2 * ind + 1, right  = 2 * ind + 2;
        size_t lowest = ind;
        
        if (left < heap.size()  &&  heap[left].distance < heap[ind].distance)
            lowest = left;
        if (right < heap.size()  &&  heap[right].distance < heap[lowest].distance)
            lowest = right;
        
        if (lowest != ind)
        {
            if (parent_iter != map.end())
                parent_iter->second.position = lowest;
            
            auto child_iter = map.find(heap[lowest].coords);
            if (child_iter != map.end())
                child_iter->second.position = ind;
            
            
            swap(heap[ind], heap[lowest]);
            Heapify(lowest, parent_iter); 
        }
    }
    
    void Heapify(size_t ind)
    {
        size_t left = 2 * ind + 1, right  = 2 * ind + 2;
        size_t lowest = ind;
        
        if (left < heap.size()  &&  heap[left].distance < heap[ind].distance)
            lowest = left;
        if (right < heap.size()  &&  heap[right].distance < heap[lowest].distance)
            lowest = right;
        if (lowest != ind)
        {
            auto parent_iter = map.find(heap[ind].coords);
            if (parent_iter != map.end())
                parent_iter->second.position = lowest;
            
            auto child_iter = map.find(heap[lowest].coords);
            if (child_iter != map.end())
                child_iter->second.position = ind;
            
            
            swap(heap[ind], heap[lowest]);
            Heapify(lowest, parent_iter);
        }
    }
    
    void DecreaseKey(size_t ind, unordered_map<Coords, MapNode, Hasher>::iterator child_it)
    {
        if (ind == 0) return;
        size_t parent = (ind - 1) / 2;
        if (heap[ind].distance < heap[parent].distance)
        {
            if (child_it != map.end())
                child_it->second.position = parent;

            auto parent_it = map.find(heap[parent].coords);
            if (parent_it != map.end())
                parent_it->second.position = ind;
                        
            swap(heap[ind], heap[parent]);
            DecreaseKey(parent, child_it);
        }
    }
    
    void DecreaseKey(size_t ind)
    {
        if (ind == 0) return;
        size_t parent = (ind - 1) / 2;
        if (heap[ind].distance < heap[parent].distance)
        {
            auto child_it = map.find(heap[ind].coords);
            if (child_it != map.end())
                child_it->second.position = parent;

            auto parent_it = map.find(heap[parent].coords);
            if (parent_it != map.end())
                parent_it->second.position = ind;
                        
            swap(heap[ind], heap[parent]);
            DecreaseKey(parent, child_it);
        }
        
    }

public:
    
    Heap(size_t in_n, size_t in_m): n(in_n), m(in_m)
    {
        heap.push_back(Cell(0, 0, 0));
        map.insert(make_pair(Coords(0, 0), MapNode(0, false, 1)));
    }
   
    Cell ExtractMin()
    {
        if (heap.size() == 0) 
            throw logic_error("Extracting minimum from an empty heap");
        
        Cell ans = heap[0];
        //cout << "\rHeap size: " << heap.size() << "  Map size: " << map.size() << "  Position: " << abs((int)ans.coords.i - (int)ans.coords.j);
        
        auto map_it1 = map.find(heap[0].coords);
        auto map_it2 = map.find(heap[heap.size() - 1].coords);
        
        if (map_it1 != map.end())
            map_it1->second.deleted = true;
        
        if (map_it2 != map.end())
            map_it2->second.position = 0;
        
        swap(heap[0], heap[heap.size() - 1]);
        heap.resize(heap.size() - 1);
        Heapify(0, map_it2);        
        
        return ans;
    }
    
    void Update(int i, int j, int new_dist, int max_inp)
    {
        auto map_it = map.find(Coords(i, j));
        if (map_it == map.end())
        {
            heap.push_back(Cell(i, j, new_dist));
            if (max_inp == 3)
                map.insert(make_pair(Coords(i, j), MapNode(heap.size() - 1, false, 1)));
            DecreaseKey(heap.size() - 1);
        }
        else
        {
            MapNode &node = map_it->second;
            if (!node.deleted)
            {
                int pos = node.position;
                Cell &cell = heap[pos];
                if (new_dist < cell.distance)
                {
                    cell.distance = new_dist;
                    DecreaseKey(pos);
                }
            }
            
            node.used++;
            if (node.used == max_inp)
                map.erase(map_it);
        }
    }    

/*    void Check()
    {
        for (auto it = map.begin(); it != map.end(); it++)
        {
            Coords coords = it->first;
            MapNode node = it->second;
            
            Cell cell(heap[node.position]);
            if (!node.deleted && (coords.i != cell.coords.i || coords.j != cell.coords.j))
                throw logic_error("Wrong map!");
        }
    }*/
};


void ReadFromFile(string &filename, string &input_string)
{
	input_string.clear();
	ifstream book(filename);

    while(!book.eof())
	{
		char ch;
		book >> ch;
        if ((ch > 'a' and ch < 'z')  or (ch > 'A'  and ch < 'Z'))
            input_string += ch;
	}
}

void RandInsert(string &str, int begin, int end)
{
	long pos = begin + rand() % (end - begin);
	char ch = rand() % 26 + 65;
	string ins_str;
	ins_str += ch;
	str.insert(pos, ins_str);
}

void RandDelete(string &str, int begin, int end)
{
	if (str.empty()) return;

	long pos = begin + rand() % (end - begin);
	str.erase(pos, 1);
}

void RandReplace(string &str, int begin, int end)
{
	if (str.empty()) return;

	long pos = begin + rand() % (end - begin);
	char ch = rand() % 26 + 65;
	string ins_str;
	ins_str += ch;

	str.replace(pos, 1, ins_str);
}


void ChangeString(string &str, int change_num)
{  
    int step = str.length() / change_num;
    int begin = 0;
    int end = step;
	for (long i = 0; i < change_num; ++i)
	{
		long type = rand() % 3;
		switch (type)
		{
		case 0:
			RandInsert(str, begin, end);
			break;
		case 1:
			RandDelete(str, begin, end);
			break;
		case 2:
			RandReplace(str, begin, end);
			break;
		}
        begin = end;
        end += step;
	}
}

void RandString(string &str, int min_size, int max_size)
{
	str.clear();
    
    int length;
    if (min_size != max_size) 
        length = rand() % (max_size - min_size) + min_size;
    else
        length = min_size;
    
	for (long j = 0; j < length; j++)
	{
		char ch = rand() % 26 + 65;
		str += ch;
	}
}


int LivDistance(Heap &heap, const string &word1, const string &word2)
{
    while (true)
    {
        Cell cell = heap.ExtractMin();
        int i = cell.coords.i, j = cell.coords.j;
        int dist = cell.distance;
        
        if (i == (int)word1.length() - 1  &&  j == (int)word2.length() - 1)
            return dist - (abs((int)word1.length() - (int)word2.length()));
                
        
        if (i + 1 < (int)word1.length())
        {
            int new_dist = dist + (1 + abs(i + 1 - j) - abs(i - j));
            
            int max_inp;
            if (i == 0 || j == 0) 
                max_inp = 1;
            else
                max_inp = 3;
            
            heap.Update(i + 1, j, new_dist, max_inp);
        }

        if (j + 1 < (int)word2.length())
        {
            int new_dist = dist + (1 + abs(i - (j + 1)) - abs(i - j));
            
            int max_inp;
            if (i == 0 || j == 0) 
                max_inp = 1;
            else
                max_inp = 3;
            
            heap.Update(i, j + 1, new_dist, max_inp);
        }
        
        
        if (i + 1 < (int)word1.length()  &&  j + 1 < (int)word2.length())
        {
            int new_dist = dist;
            if (word1[i + 1] != word2[j + 1]) new_dist++;
            heap.Update(i + 1, j + 1, new_dist, 3);
        }        
    }
}


int LivDistance(const string &inp_word1, const string &inp_word2)
{
    string word1 = "A" + inp_word1;
    string word2 = "A" + inp_word2;
    Heap heap(word1.size(), word2.size());
    
    return LivDistance(heap, word1, word2);
}

/*
void SingleTest(size_t range)
{
    string str1;
    RandString(str1, range - range / 10, range + range / 10);
    
    string str2(str1);
    ChangeString(str2, 30);
    
    //int d1 = Hirshberg(str1, str2);
    int d2 = LivDistance(str1, str2);
    
    if (d1 != d2)
        throw logic_error("Incorrect livenstein distance!");
    
}

void StressTests()
{
    bool correct = true;
    try
    {
        for (int i = 0; i < 1; i++)
        {
            SingleTest(100000);
            cerr << endl << "Test " << i + 1 << " OK!" << endl;
        }
    }
    catch (const exception &ex)
    {
        correct = false;
        cerr << endl << "Exception: " << ex.what() << endl;
    }
    
    if (correct)
        cerr << endl << "Tests OK!" << endl;
    else
        cerr << endl << "Tests failed!" << endl;
}
*/


void HugeTest()
{
    string str1;
    string filename = "lord.txt";
    
    ReadFromFile(filename, str1);
    
    string str2(str1);
    ChangeString(str2, 30);
    
    cout << "The length of the string is " << str1.length() << endl;    
    cout << endl << "Distance: " << LivDistance(str1, str2) << endl;
}

void OnlineTest()
{
    string str1, str2;
    cin >> str1;
    cin >> str2;
    cout << LivDistance(str1, str2);
}

int main()
{
    HugeTest();
    //OnlineTest();
    cout << endl;
    return 0;
}
