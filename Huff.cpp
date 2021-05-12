#include "bits/stdc++.h"
#include <unordered_map>
using namespace std;

// A Tree node
struct Node
{
	char ch;
	int freq;
	Node *left, *right;

	//Constructor

	Node(char ch, int freq, Node* left, Node* right)
	{
    this->ch = ch;
	this->freq = freq;
	this->left = left;
	this->right = right;
	}
};

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		if(l->freq > r->freq)
		return true;
		else
            return false;
	}
};

// traverse the Huffman Tree and store Huffman Codes
// in a map.

void encode(Node* root, string str,unordered_map<char, string> &huffmanCode)
{
	if (root == NULL)
         return;

	// found a leaf node
	if (root->left==NULL && root->right==NULL) {
		huffmanCode[root->ch] = str;
	}
	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int &index, string str)
{
	if (root == NULL) {
		return;
	}

	// found a leaf node
	if (root->left==NULL && root->right==NULL)
	{
		cout << root->ch;
		return;
	}

	index++;

	if (str[index] =='0')
		decode(root->left, index, str);
	else
		decode(root->right, index, str);
}

// Builds Huffman Tree and decode given input text
int buildHuffmanTree(string text)
{
    int cal = 0 ;
    string save;
    char ch ;

	// count frequency of appearance of each character
	// and store it in a map
	//we use unordered_map instead of map because we did not know the order of characters in file because they are unordered
	// that's why we used unordered_map to store their frequencies

	unordered_map<char, int> freq;

	for (int i = 0 ; i < text.length() ; i ++) {
        ch = text[i] ;
		freq[ch]++;   // it calculates how many times the character came int whole file
	}

	// Create a priority queue to store live nodes of
	// Huffman tree;
	priority_queue<Node*,vector<Node*>,comp> pq; // it is used as a min heap

	// Create a leaf node for each character and add it
	// to the priority queue.

	unordered_map<char,int>::iterator itr; // iterator used
	pair<char,int> p;
	Node *node ;
	for(itr = freq.begin() ; itr!=freq.end() ; itr++)
    {
        p.first = itr->first;
        p.second = itr->second;
        node = new Node(p.first, p.second, NULL, NULL); // left->nullptr and right->nullptr
        pq.push(node);
    }

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();	pq.pop();

		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		node = new Node('\0', sum, left, right);
		pq.push(node);
	}

	// root stores pointer to root of Huffman Tree
	Node* root = pq.top();

	// traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

    int i = 0 ;
    unordered_map<char, string>::iterator it;
    pair<char,string> pr;
	cout << "Huffman Codes are :\n" << '\n';

	for (it = huffmanCode.begin() ; it!= huffmanCode.end() ; it++)
        {

        pr.first = it->first ;
        pr.second = it->second ;
		cout << pr.first << " " << pr.second << '\n';
		    save = pr.second;
		    while(save[i]!='\0')
            {
                cal++; // calculating no of bits after applying huffman algorithm
                i++;
            }
            i = 0 ;
	     }

	cout << "\nOriginal string was :\n" << text << '\n';
    fstream onf("DATA.txt",ios::out);
	// print encoded string
	string str = "";
	for (int i = 0 ; i < text.length() ; i ++) {
		ch = text[i];
		str += huffmanCode[ch];
	}
	onf<<str;
    onf.close();
	cout << "\nEncoded string is :\n" << str << '\n';

	// traverse the Huffman Tree again and this time
	// decode the encoded string
	int index = -1;
	cout << "\nDecoded string is: \n";
	while (index < (int)str.size() - 2) {
		decode(root, index, str);
	}
	return cal;
}

// Huffman coding algorithm
int main()
{
	string text;
	char ch;
    ifstream inf("DATA.txt",ios::in);
    if(inf.is_open())
    {
        while(inf>>noskipws>>ch)
        {
        	text+=ch;
        }
        inf.close();
    }
    cout<<text<<" \n ";
	int ans = buildHuffmanTree(text);
    cout<<"\nNO OF BITES = "<<ans<<endl;
    ans/=8;
    cout<<"\nSIZE IN BYTES  = "<<ans<<endl;
	return 0;
}
