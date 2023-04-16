// Project 5
// CS 2413 Data Structures
// Spring 2023

// Code generated for input 1 would be:
// if(expression1)
// {
// 	statement1;
// }
// else
// {
// 	statement2;
// }

#include <iostream>
#include <string> // for the fields in the class
#include <vector> // for the flowchart structures
#include <stack>  // for conversion

#include <fstream> //REMOVE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

using namespace std;

// class to store node information
class Node
{
private:
	int nodeNumber;    // to store the node number
	string type;       // can either be START, END, IF, BLOCK
	string textWithin; // can be expression or statement - for START and END this will be empty string
public:
	// TODO: construtors, getters, setters and other methods as needed
	Node()//default constructor
	{
		nodeNumber = 0;
		type = "";
		textWithin = "";
	}
	Node(int nodeNumber, string type)//non-default constructor: if textWithin is absent
	{
		this->nodeNumber = nodeNumber;
		this->type = type;
	}
	Node(int nodeNumber, string type, string textWithin)//non-default constructor
	{
		this->nodeNumber = nodeNumber;
		this->type = type;
		this->textWithin = textWithin;
	}
	int getNodeNumber()
	{
		return nodeNumber;
	}
	string getType()
	{
		return type;
	}
	string getTextWithin()
	{
		return textWithin;
	}
	string getTypeByNumber(int num)
	{
		if(this->nodeNumber == num)return type;
		return "";
	}

	void display()
	{
		if(textWithin != "")cout << nodeNumber << " " << type << " " << textWithin << '\n';
		else cout << nodeNumber << " " << type << '\n';
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Node getNodeByNumber(int num, vector<Node> allNodes)
{
	for(int i = 0; i < allNodes.size(); i++)
	{
		if(allNodes[i].getNodeNumber() == num)return allNodes[i];
	}
	return Node();
}

// function to convert the given flowchart to generate code
void convertFlowChart(vector<Node> allNodes, vector<vector<int>> adjList)
{
	// TODO: use stack (no recursion) to convert the flowchart into psuedo code
	stack<Node> nodeStack;
	for(int i = adjList.size()-1; i > -1; i--)//reverse iterate through adjList
	{
		for(int j = adjList[i].size()-1; j > -1; j--)//reverse iterate through each vector in adjList
		{
			nodeStack.push(getNodeByNumber(adjList[i][j], allNodes)); //push all nodes from adjList into stack
		}
	}
	// print the code as an output here using cout
	for(int i = 0; i < nodeStack.size(); i++)
	{
		//check if node is start node
		if(nodeStack.top().getType() == "START")
		{
			nodeStack.pop();
			nodeStack.pop();
		}
		//check if node is an if statement
		if(nodeStack.top().getType() == "IF")
		{
			cout << "if(" << nodeStack.top().getTextWithin() << ")\n{\n ";
			nodeStack.pop();
			cout << nodeStack.top().getTextWithin() << ";\n}\nelse\n{\n ";
			nodeStack.pop();
			cout << nodeStack.top().getTextWithin() << ";\n}\n";
			nodeStack.pop();
		}
		//check if node is not an if statement
		if(nodeStack.top().getType() == "BLOCK")
		{
			nodeStack.pop();
			nodeStack.pop();
		}
		//check if node is end node
		if(nodeStack.top().getType() == "END")
		{
			nodeStack.pop();
			break;
		}
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main()
{
	ifstream file;
	file.open("input1.txt");
	if(file.is_open())
	{
		cin.rdbuf(file.rdbuf());
	}

	int numNodesInFlowChart; // number of nodes in the flow chart

	cin >> numNodesInFlowChart;
	cout << "Number of nodes: " << numNodesInFlowChart << endl;

	// Node objects array to store all the information
	vector<Node> allNodes(numNodesInFlowChart);

	// TODO: read in the information about the nodes and store it in allNodes
	string nodeNumber, type, textWithin;
	cin >> nodeNumber >> type;
	allNodes[0] = (Node(stoi(nodeNumber), type)); //for start node
	for(int i = 1; i < numNodesInFlowChart-1; i++) //loop to read in all nodes between start and end
	{
		cin >> nodeNumber >> type >> textWithin; 
		allNodes[i] = (Node(stoi(nodeNumber), type, textWithin)); //add node into vector
	}
	cin >> nodeNumber >> type;
	allNodes.push_back(Node(stoi(nodeNumber), type)); //for end node

	// for(Node n : allNodes)
	// {
	// 	n.display();
	// }

	// adjacency list to store the flow chart
	vector<vector<int>> adjList(numNodesInFlowChart);

	// TODO: read in the adjacency list
	int from, target1, target2;
	cin >> from >> target1;
	vector<int> start(2);
	start[0] = from;
	start[1] = target1;
	adjList[0] = start;
	for(int i = 1; i < allNodes.size() - 2; i++)
	{
		cin >> from;
		if(allNodes[i].getTypeByNumber(from) == "IF")
		{
			cin >> target1 >> target2;
			vector<int> a(3);
			a[0] = from;
			a[1] = target1;
			a[2] = target2;
			adjList[i] = a;
		}
		else
		{
			cin >> target1;
			vector<int> b(2);
			b[0] = from;
			b[1] = target1;
			adjList[i] = b;
		}
	}
	cin >> from >> target1;
	vector<int> end(1);
	end[0] = from;
	adjList[allNodes.size()-2] = end;

	// TODO: call the convertFlowChart function with the allNodes and adjList parameters
	convertFlowChart(allNodes, adjList);

	return 0;
}
