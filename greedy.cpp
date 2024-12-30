#include <iostream>
#include <vector>
#include <iterator> 
#include <map> 
#include <string>
#include <bits/stdc++.h>
#include <functional>
#include <utility>
#include <queue>
#include <fstream>

using namespace std;

map<vector<vector<int> > , bool> visited; //declaring a map named "visited" that takes vector(which stores vectors that takes integer values) as its key, and a boolean as its value
map<vector<vector<int> > , vector<vector<int> > > parent; //declaring a map named "parent" that takes vectors as its both key and value to keep track of the nodes
vector<vector<int> > goal(3,vector<int> (3)); 

bool visit(vector<vector<int> > a)  //boolean to see if node is visited
{
	if(visited[a]==true)
		return true;
	
	return false;
}

// f(n) = path_cost + heuristic
// path_cost = number of path_cost
// heuristic = calculated
// dist = f(n)

//Manhattan or A*(A-star) search to find the distance 
int manhattan (vector<vector<int> > a , int path_cost)
{
	int dist = path_cost;
	for (int i=0; i<3; i++) //for loop to go through each row and column (like matrices). [00,01,02,10,11,12,20,21,22] We'll be using this i,j loop more.
	{
		for (int j=0; j<3; j++)
		{
			if(a[i][j] != 0)
			{
				for (int k=0; k<3; k++)
				{
					for (int l=0; l<3; l++)
					{
						if(a[i][j]==goal[k][l])
							dist+=abs(i-k)+abs(j-l); //absolute value since it may be negative
					}
				}
			}
		}
	}
	return dist;	
}

int misplaced (vector<vector<int> > a , int path_cost)
{
	int dist = path_cost;
	for (int i=0; i<3; i++) //for loop to go through each row and column (like matrices). [00,01,02,10,11,12,20,21,22] We'll be using this i,j loop more.
	{
		for (int j=0; j<3; j++)
		{
			if(a[i][j] != 0)
			{
				if(a[i][j]!=goal[i][j])
					dist++; //absolute value since it may be negative
						
			}
		}
	}
	return dist;	
}

bool isGoal (vector<vector<int> > a){  //boolean to see if we've reached our goal state
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(a[i][j] != goal[i][j]) return 0; //if our puzzle tiles aren't same with the goal position. For eg. we want a[0][0] = 1, a[0][1] = 2, a[2][2] = 0, etc.
			
		}
	}
	
	return 1;	
}

bool safe(int i,int j) 
{
	if(i>=0 && i<=2 && j>=0 && j<=2)
		return true;
	
	return false;
}

int dx[]={-1,1,0,0}; 
int dy[]={0,0,1,-1};

vector<vector<vector<int> > > children(vector<vector<int> > a) 
{
	pair<int,int> pos; 
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++){
			if(a[i][j] == 0) 
			{
				pos.first=i;
				pos.second=j;
				break;
			 } 												
		}		
		
	}	
	vector<vector<vector<int> > > ans; 
	for (int k=0; k<4; k++) 
	{
		int cx = pos.first; 
		int cy = pos.second; 
		vector<vector<int> > n = a;
		if(safe(cx+dx[k], cy+dy[k]))
		{
			swap(n[cx+dx[k]][cy+dy[k]], n[cx][cy]);
			ans.push_back(n);			
		}
	}

	return ans; 		
}
int a = 2;
typedef pair<vector<vector<int> > , int> node; // datatype in which node is stored and contains state and path cost
 
struct cmp_manhattan
{
	bool operator() (node &a, node &b) // overloading the unary operator "()"
	{
		int am = manhattan(a.first,a.second);
		int bm = manhattan(b.first,b.second);
		return am>bm;		
	}	
};

struct cmp_misplaced
{
	bool operator() (node &a, node &b) // overloading the unary operator "()"
	{
		int am = misplaced(a.first,a.second);
		int bm = misplaced(b.first,b.second);
		return am>bm;		
	}	
};

void print_path(vector<vector<int> > s)
{
	if(parent.count(s)) //to see if our vector s is present in our map named "parent" which takes vectors in it for both key and value, and if it is present
	print_path(parent[s]); //then print the path
	
    for(int i=0;i<3;i++)
    {
 	  for(int j=0;j<3;j++)
	  {
		 printf("%d ",s[i][j]);
	  }
	 cout<<endl;
    }
   cout<<endl;
   
 return;
}



void solve_misplaced(vector<vector<int> > a, int path_cost)
{
	
	priority_queue<node, vector<node>, cmp_misplaced> Frontier; //Priority queue named "Q". 
	Frontier.push(node(a,path_cost));
	while(!Frontier.empty())
	{
		vector<vector<int> > s = Frontier.top().first;
		Frontier.pop();
		visited[s]=true;
		if(s==goal) //print(s)
		{
			print_path(s);
			break;
		}
		vector<vector<vector<int> > > ns = children(s);
		vector<vector<vector<int> > >::iterator it;
		for(it=ns.begin();it!=ns.end();it++)
		{
			vector<vector<int> > temp = *it;
			if(!visit(temp))
			{
				//cout<<path_cost<<endl;
				parent.insert(pair<vector<vector<int> > , vector<vector<int> > >(temp,s));
				Frontier.push(node(temp,path_cost));
			}
		}
	}
	cout<<path_cost<<endl;
	return;
}

void solve_manhattan(vector<vector<int> > a, int path_cost)
{
	
	priority_queue<node, vector<node>, cmp_manhattan> Frontier; //Priority queue named "Q". 
	Frontier.push(node(a,path_cost));
	
	while(!Frontier.empty())
	{
		vector<vector<int> > s = Frontier.top().first;
		Frontier.pop();
		visited[s]=true;
		if(s==goal) //print(s)
		{
			print_path(s);
			break;
		}
		vector<vector<vector<int> > > ns = children(s);		
		vector<vector<vector<int> > >::iterator it;
		for(it=ns.begin();it!=ns.end();it++)
		{
			vector<vector<int> > temp = *it;
			if(!visit(temp))
			{
				parent.insert(pair<vector<vector<int> > , vector<vector<int> > >(temp,s));
				Frontier.push(node(temp,path_cost));
			}
		}
	}

	return;
}

int main()
{



	cout<<"Please enter your 8 Puzzle as a string! \n";
	cout<<"\nFor example => 0 1 3 4 2 5 7 8 6\n";
	vector<vector<int> > a(3,vector<int> (3));	
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			cin>>a[i][j];	
		}
	}
	cout<<"Please enter your goal state as a string! \n";
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			cin>>goal[i][j];	
		}
	}
	int heuristic;
	cout<<"Enter 1 for Misplaced Tile heuristic and 2 for manhattan distance: ";
	cin>>heuristic;
	cout<<"\nPlease wait while the puzzle is solving...\n";
	if (heuristic==1) {
		solve_misplaced(a,0);
	}
	else if (heuristic==2) {
		solve_manhattan(a,0);
	}



}

