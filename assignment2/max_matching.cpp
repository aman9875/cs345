#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef vector<vector<int> > vvi;
typedef vector<vector<ii> > vvii;

#define pb push_back
#define mp make_pair
#define all(x) x.begin(),x.end()
#define sz(x) (int)x.size()
#define fill(a,x) memset(a,x,sizeof(a))
#define F first
#define S second
#define trav(a, x) for(auto& a : x)	
#define FOR(i,a,b) for(int i = a; i<=b; ++i)
#define NFOR(i,a,b) for(int i = a; i>=b; --i)
#define fast ios_base::sync_with_stdio(false),cin.tie(0),cout.tie(0)
const ll INF = 1e18;
const int mod = 1e9+7;
const int N = 1e5 + 10;
inline int add(int x,int y){
  x += y;
  if(x >= mod) x -= mod;
  return x;
}
inline int mul(int x,int y){
  x = (1LL * x * y) % mod;
  return x;
}

int n,m;
int flag = 1;

void dfs(int s,vvi &G1,vvi &G2,int visited1[],int visited2[],int parent1[],int parent2[],int state)
{
	if(state==1)
	{
		for(int i=0;i<G1[s].size();i++)
		{
			int v = G1[s][i];
			if(visited2[v]==0)
			{
				if(flag==1)
					parent2[v] = s;
				visited2[v] = 1;
				dfs(v,G1,G2,visited1,visited2,parent1,parent2,2);
			}
		}
	}
	else
	{
		for(int i=0;i<G2[s].size();i++)
		{
			int v = G2[s][i];
			if(visited1[v]==0)
			{
				if(flag==1)
					parent1[v] = s;
				visited1[v] = 1;
				dfs(v,G1,G2,visited1,visited2,parent1,parent2,1);
			}

			if(v==0)flag = 0;
		}
	}
}

void dfs_visit(vvi &G1,vvi &G2,vector<int> &p)
{
	int visited1[505] = {0};
	int visited2[505] = {0};
	int parent1[505] = {0};
	int parent2[505] = {0};
	visited2[0] = 1;
	flag = 1;
	dfs(0,G1,G2,visited1,visited2,parent1,parent2,2);

	if(parent1[0]!=0)
	{
		int t = parent1[0];
		int c = 1;
		while(1)
		{
			if(t==0)
			{
				break;
			}

			p.pb(t);

			if(c==1)
			{
				t = parent2[t];
				c = 0;
			}
			else
			{
				t = parent1[t];
				c = 1;
			}
		}
	}

}

int compute_augmenting_path(vector<int> &p2,int match[],vector<int> vertex,vector<ii> edges,int match2[])
{
	vvi G1(2*n+1);
	vvi G2(2*n+1);

	for(int i=0;i<vertex.size();i++)
	{
		if(match[vertex[i]]==0)
		{
			G2[0].pb(vertex[i]);
		}
	}

	for(int i=0;i<vertex.size();i++)
	{
		if(match[vertex[i]]==0)
		{
			G2[vertex[i]].pb(0);
		}
	}

	for(int i=0;i<edges.size();i++)
	{
		int x = edges[i].first;
		int y = edges[i].second;

		if(match[x]==y)
		{
			G2[x].pb(y);
		}
		else
		{
			G1[x].pb(y);
		}
	}

	vector<int> w;
	dfs_visit(G1,G2,w);
	set<int> s;
	for(int i=0;i<w.size();i++)
	{
		s.insert(w[i]);
	}

	if(w.size()==0)
	{
		return 0;
	}
	else if(s.size()==w.size())
	{
		for(int i=0;i<w.size();i++)
		{
			p2.pb(w[i]);
		}

		for(int i=1;i<=500;i++)
		{
			match2[i] = match[i];
		}

		return 1;
	}
	else
	{
		int len =  w.size();
		vector<int> cycle;
		vector<int> stem;
		int cycle_p[505] = {0};
		if(w[len-1]==w[0] && s.size()==w.size()-1)
		{
			for(int i=0;i<w.size()-1;i++)
			{
				cycle.pb(w[i]);
				cycle_p[w[i]] = 1;
			}
		}
		else
		{
			int count[505] =  {0};
			int cycle_vertex = 0;
			int id = -1;
			for(int i=0;i<w.size();i++)
			{
				count[w[i]]++;
				if(count[w[i]] > 1)
				{
					cycle_vertex = w[i];
					id = i;
					break;
				}
			}

			cycle.pb(w[id]);
			cycle_p[w[id]] = 1;
			int i = 0;
			for(i=id-1;i>=0;i--)
			{
				if(w[i]==cycle_vertex)
				{
					break;
				}

				cycle.pb(w[i]);
				cycle_p[w[i]] = 1;
			}

			for(int j=0;j<i;j++)
			{
				stem.pb(w[j]);
			}
		}

		assert(stem.size()%2 == 0);
		assert(cycle.size()%2 == 1);
		assert(match[w[0]] == 0);
		assert(match[w[len-1]] == 0);

		int match1[505] = {0};

		for(int i=1;i<=500;i++)
		{
			match1[i] = match[i];
		}

		for(int i=0;i<stem.size();i++)
		{
			match1[stem[i]] = stem[i+1];
			match1[stem[i+1]] = stem[i];
			i++;	
		}

		vector<int> vertex_prime;
		vector<ii> edges_prime;
		set<ii> edg;
		int max = -1;
		for(int i=0;i<vertex.size();i++)
		{
			int v = vertex[i];
			if(cycle_p[v]==0)
			{
				vertex_prime.pb(v);
				if(v > max)max = v;
			}
		}

		int v_c = max+1;
		vertex_prime.pb(max+1);

		for(int i=0;i<edges.size();i++)
		{
			int x = edges[i].first;
			int y = edges[i].second;

			if(cycle_p[x]==0 && cycle_p[y]==0)
			{
				edg.insert(mp(x,y));
			}
			else if(cycle_p[x]==0 && cycle_p[y]==1)
			{
				edg.insert(mp(x,v_c));
			}
			else if(cycle_p[x]==1 && cycle_p[y]==0)
			{
				edg.insert(mp(v_c,y));
			}
		}

		set<ii> :: iterator it;
		for(it=edg.begin();it!=edg.end();it++)
		{
			edges_prime.pb(mp(it->first,it->second));
		}

		int match_prime[505] = {0};

		for(int i=1;i<=v_c;i++)
		{
			int x = i;
			int y = match1[i];

			if(y!=0 && cycle_p[x]==0 && cycle_p[y]==0)
			{
				match_prime[x] = y;
			}
			else if(y!=0 && cycle_p[y]==1 && cycle_p[x]==0)
			{
				match_prime[x] = v_c;
				match_prime[v_c] = x;
			}
			else if(y!=0 && cycle_p[y]==0 && cycle_p[x]==1)
			{
				match_prime[y] = v_c;
				match_prime[v_c] = y;
			}
		}

		int match1_prime[505]= {0};
		vector<int> p_prime;

		compute_augmenting_path(p_prime,match_prime,vertex_prime,edges_prime,match1_prime);

		if(p_prime.size()==0)
		{
			return 0;
		}
		else
		{
			vector<int> p_double_prime;
			int match2_prime[505] = {0};

			for(int i=1;i<=v_c;i++)
			{
				match2_prime[i] = match1_prime[i];
			}

			int flag = 1;
			int id = -1;
			for(int i=0;i<p_prime.size();i++)
			{
				if(p_prime[i]==v_c && i!=0 && i!=p_prime.size()-1)
				{
					id = i;
					flag = 0;
				}
			}

			if(flag==0)
			{
				int flag1 = 1;

				for(int i=0;i<p_prime.size();i++)
				{
					if(p_prime[i]==v_c && match1_prime[p_prime[i-1]]==v_c)
					{
						flag1 = 0;
						break;
					}
				}

				if(flag1==1)
				{
					match2_prime[p_prime[id]] = 0;

					for(int i=id+1;i<p_prime.size();i++)
					{
						int v1 = p_prime[i];
						int v2 = p_prime[i+1];
						match2_prime[v1] = v2;
						match2_prime[v2] = v1;
						i++;		
					}

					for(int i=0;i<=id;i++)
					{
						p_double_prime.pb(p_prime[i]);
					}
				}
				else
				{
					match2_prime[p_prime[id]] = 0;

					for(int i=id-1;i>=0;i--)
					{

						int v1 = p_prime[i];
						int v2 = p_prime[i-1];
						match2_prime[v1] = v2;
						match2_prime[v2] = v1;
						i--;
					}

					for(int i=id;i>=0;i--)
					{
						p_double_prime.pb(p_prime[i]);
					}
				}
			}
			else
			{
				for(int i=0;i<p_prime.size();i++)
				{
					p_double_prime.pb(p_prime[i]);
				}
			}

			for(int i=1;i<=v_c;i++)
			{
				match2[i] = match2_prime[i];
			}

			int flag2 = 0;
			for(int i=0;i<p_double_prime.size();i++)
			{
				if(p_double_prime[i] == v_c)
				{
					flag2 = 1;
				}
			}

			if(flag2==0)
			{
				for(int i=0;i<p_double_prime.size();i++)
				{
					p2.pb(p_double_prime[i]);
				}

				for(int i=1;i<cycle.size()-1;i++)
				{
					int v1 = cycle[i];
					int v2 = cycle[i+1];

					match2[v1] =  v2;
					match2[v2] = v1;
					i++;
				}				
			}
			else
			{
				if(p_double_prime[0]==v_c)
				{
					int x_k = p_double_prime[1];
					int y_k;
					for(int i=0;i<edges.size();i++)
					{
						int x = edges[i].first;
						int y = edges[i].second;

						if(x==x_k && cycle_p[y]==1)
						{
							y_k = y;
							break;
						}
					}

					for(int i=0;i<cycle.size();i++)
					{
						if(cycle[i]==y_k)
						{
							id = i;
							break;
						}
					}

					vector<int> c1;

					for(int i=id+1;i<cycle.size();i++)
					{
						c1.pb(cycle[i]);
					}

					for(int i=0;i<id;i++)
					{
						c1.pb(cycle[i]);
					}

					for(int i=0;i<c1.size();i++)
					{
						int v1 = c1[i];
						int v2 = c1[i+1];

						match2[v1] = v2;
						match2[v2] = v1;
						i++;
					}

					for(int i=p_double_prime.size()-1;i>0;i--)
					{
						p2.pb(p_double_prime[i]);
					}

					p2.pb(y_k);

				}
				else if(p_double_prime[p_double_prime.size()-1]==v_c)
				{
					int s1 = p_double_prime.size()-2;
					int x_k = p_double_prime[s1];
					int y_k;
					for(int i=0;i<edges.size();i++)
					{
						int x = edges[i].first;
						int y = edges[i].second;

						if(x==x_k && cycle_p[y]==1)
						{
							y_k = y;
							break;
						}
					}

					for(int i=0;i<cycle.size();i++)
					{
						if(cycle[i]==y_k)
						{
							id = i;
							break;
						}
					}

					vector<int> c1;

					for(int i=id+1;i<cycle.size();i++)
					{
						c1.pb(cycle[i]);
					}

					for(int i=0;i<id;i++)
					{
						c1.pb(cycle[i]);
					}

					for(int i=0;i<c1.size();i++)
					{
						int v1 = c1[i];
						int v2 = c1[i+1];

						match2[v1] = v2;
						match2[v2] = v1;
						i++;
					}

					for(int i=0;i<=s1;i++)
					{
						p2.pb(p_double_prime[i]);
					}

					p2.pb(y_k);
				}
			}

			return 1;
		}
	}
	
}

int main()
{
	cin>>n>>m;
	vector<int> v;
	set<ii>edg;
	int match[505] = {0};
	for(int i=1;i<=n;i++)
	{
		v.pb(i);
	}

	
	for(int i=1;i<=m;i++)
	{
		int x,y;
		cin>>x>>y;
		if(x!=y)
		{
			edg.insert(mp(x,y));
			edg.insert(mp(y,x));
		}
	}
	
	vector<ii> edges(all(edg));
	
	/*
	int x,y;
	while(!cin.eof())
	{
		cin>>x>>y;
		if(x!=y)
		{
			edg.insert(mp(x,y));
			edg.insert(mp(y,x));
		}
	}

	vector<ii>edges(all(edg));
	*/

	while(1)
	{
		vector<int> p2;
		int match2[505] = {0};
		int res = compute_augmenting_path(p2,match,v,edges,match2);
		if(res==0)
		{
			break;
		}
		else
		{
			for(int i=1;i<=n;i++)
			{
				match[i] = match2[i];
			}

			for(int i=0;i<p2.size();i++)
			{
				int v1 = p2[i];
				int v2 = p2[i+1];

				match[v1] = v2;
				match[v2] = v1;
				i++;
			}
		}
	}

	int ans = 0;
	set<ii> r;
	for(int i=0;i<edges.size();i++)
	{
		int x = edges[i].first;
		int y = edges[i].second;

		if(match[x]==y)
		{
			if(x<y)
				r.insert(mp(x,y));
			else 
				r.insert(mp(y,x));
		}
	}

	//cout << "\n\n";
	cout << r.size() <<  "\n";

	set<ii> :: iterator it;
	for(it=r.begin();it!=r.end();it++)
	{
		cout << it->first << " " << it->second << "\n";
	}

	return 0;
}
