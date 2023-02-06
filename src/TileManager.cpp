#include "TileManager.h"

TileManager::TileManager(){
	memset(grid, 0, sizeof(grid));
	for(int i = 1; i < 100; ++i){
		for(int j = 0 ; j < 100; ++j){
			if(rand() % 100 <= 10){
				labelTile.emplace_back(vec2d<int>(i, j));
				grid[i][j] = 1;
			}
		}
	}
	for(int i = -1; i<=100;++i){
		labelTile.emplace_back(vec2d<int>(-1, i));
	}
	for(int i = 0; i<=100;++i){
		labelTile.emplace_back(vec2d<int>(i,-1));
	}
	for(int i = 0;i<=100;++i)labelTile.emplace_back(vec2d<int>(100,i));
	for(int i = 0;i<=99;++i)labelTile.emplace_back(vec2d<int>(i,100));
}

void TileManager::Render(SDL_Renderer* renderer, Camera& cam) const{
	// Render lines in screen
	vec2d<float> tmp = cam.ScreenToWorld(vec2d<int>(0, 0));
	vec2d<float> t2 = cam.ScreenToWorld(vec2d<int>(cam.GetWindowWidth(), cam.GetWindowHeight()));
	for(int i = (int)(tmp.y / length) * length ; i <= t2.y; i += length){
		SDL_RenderDrawLine(renderer, 0, cam.WorldToScreen(vec2d<float>(0,i)).y, cam.GetWindowWidth(), cam.WorldToScreen(vec2d<float>(tmp.x, i)).y);
	}
	for(int i = (int)(tmp.x / length) * length ; i <= t2.x; i += length){
		SDL_RenderDrawLine(renderer, cam.WorldToScreen(vec2d<float>(i,0)).x, 0, cam.WorldToScreen(vec2d<float>(i,0)).x, cam.GetWindowHeight());
	}

	for(auto &i : labelTile){
		SDL_Rect rect = {cam.WorldToScreen(vec2d<float>(i.x*length, 0)).x, cam.WorldToScreen(vec2d<float>(0, i.y * length)).y,(int)(ceil(length * cam.GetScaling())), (int)(ceil(length * cam.GetScaling()))};
		SDL_RenderFillRect(renderer, &rect);
	}
}

vec2d<int> TileManager::WorldToTile(vec2d<float> v) const{
	return vec2d<int>(
			v.x >= 0 ? (int)(v.x / length) : (int)((v.x - length + 1) / length),
			v.y >= 0 ? (int)(v.y / length) : (int)((v.y - length + 1) / length)
		);
}

vec2d<float> TileManager::TileToWorld(vec2d<int> v, bool flag = 0) const{
	return vec2d<float>(
			v.x * length + (flag ? (float)length / 2 : 0),
			v.y * length + (flag ? (float)length / 2 : 0)
		);
}

void TileManager::SetWallFromGrid(const vec2d<int>& v){
	labelTile.push_back(v);
	grid[v.x][v.y] = 1;
}

void TileManager::SetWallFromWorld(const vec2d<float>& v){
	labelTile.push_back(WorldToTile(v));
	grid[WorldToTile(v).x][WorldToTile(v).y] = 1;	
}

bool TileManager::ToggleWallFromGrid(const vec2d<int>& v){
	return grid[v.x][v.y] = 1 - grid[v.x][v.y];	
}

bool TileManager::ToggleWallFromWorld(const vec2d<float>& v){
	return grid[WorldToTile(v).x][WorldToTile(v).y] = 1 - grid[WorldToTile(v).x][WorldToTile(v).y];	
}

bool TileManager::IsOccupiedFromGrid(const vec2d<int>& v) const{
	return grid[v.x][v.y];
}

bool TileManager::IsOccupiedFromWorld(const vec2d<float>& v) const{
	auto tmp  = WorldToTile(v);
	return grid[tmp.x][tmp.y];
}

void TileManager::SelectTileFromWorld(const vec2d<float>& v){
	return;
}

bool* TileManager::GetGrid() const{
	return (bool*) grid; 
}

int TileManager::GetLengthOfGrid() const{
	return length;	
}



class cmp{
public:
	bool operator () (const std::pair<int, std::pair<int, vec2d<int>>>& a, const std::pair<int, std::pair<int, vec2d<int>>>& b){
		return a.first < b.first;
	}
};

bool TileManager::Walkable(const vec2d<float>& src, const vec2d<float>& target) const{
	bool res = true;
	bool xDirection = (target.x - src.x >= 0 ? 1 : 0);
	bool yDirection = (target.y - src.y >= 0 ? 1 : 0);

	float dx = target.x - src.x;
	float dy = target.y - src.y;
	float dist = src.Distance(target);
	float vx = dx / dist;
	float vy = dy / dist;
	int length = GetLengthOfGrid();
	vec2d<float> src2 = src;

	while(res && WorldToTile(src2) != WorldToTile(target)){
		float t1 = (xDirection ? floor(src2.x / length) * length + length: floor(src2.x / length) * length) - src2.x;
		float t2 = (yDirection ? floor(src2.y / length) * length + length: floor(src2.y / length) * length) - src2.y;
		float stime = std::min(t1 / vx,  t2 / vy) + 1;
		src2 += vec2d<float>(vx * stime , vy * stime);
		res &= !(IsOccupiedFromWorld(src2));
	}
	return res;	
}

std::deque<vec2d<float>> TileManager::OptimizePath(std::deque<vec2d<float>>& path) const{
	return path;	
}

std::deque<vec2d<float>> TileManager::CalculatePath(const vec2d<float>& psrc, const vec2d<float>& ptarget) const{
	const vec2d<int> src = WorldToTile(psrc);
	const vec2d<int> target = WorldToTile(ptarget);
	const static int dx[4] = {0,0,-1,1};
	const static int dy[4] = {1,-1,0,0};
	static int dist[100][100];
	static bool vis[100][100];
	static int parent[100][100][2];
	std::deque<vec2d<float>> path;
	memset(dist, 0x3f, sizeof(dist));
	memcpy(vis, GetGrid(), sizeof(vis));
	memset(parent, 0, sizeof(parent));
	// path.clear();

	// pair<heuristic, pair<distance from src, vec2d<int>next_cell>>
	std::priority_queue<
		std::pair<int, std::pair<int, vec2d<int>>>,
		std::vector<std::pair<int, std::pair<int, vec2d<int>>>>,
		cmp> pq;

	pq.push({0,{0,src}});

	// A* search
	while(!pq.empty()){
		auto f = pq.top();
		pq.pop();	
		f.first *= -1;
		int x = f.second.second.x;
		int y = f.second.second.y;
		
		if(f.second.second == target){
			dist[x][y] = f.second.first;
			vis[x][y] = 1;
			break;
		}else if(vis[x][y]){
			continue;
		}

		dist[x][y] = f.second.first;
		vis[x][y] = 1;

		if(vec2d<int>(x, y) == target){
			break;
		}
		for(int i = 0;i < 4;++i){
			int xx = x + dx[i];
			int yy = y + dy[i];

			// restrict area of searching
			if(xx >= 0 && yy >= 0 && xx < 100 && yy < 100 && !vis[xx][yy]){
				int cost = vec2d<int>(xx, yy).Manhattan(target);
				if(dist[xx][yy] > f.second.first + cost + 1){
					dist[xx][yy] = f.second.first + cost + 1;
					parent[xx][yy][0] = x;
					parent[xx][yy][1] = y;
				}
				pq.push({-f.second.first - 1 - cost, {f.second.first + 1, vec2d<int>(xx, yy)}});
			}
		}
	}
	if(dist[target.x][target.y] > 100000){
		// std::cout << "Can't reach " << target.x << ',' << target.y << " from " << src.x << ',' << src.y << std::endl;
		return path;
	}

	// Traceback and get path
	vec2d<int> temp = target;
	while(temp != src){
		path.emplace_back(TileToWorld(temp, 1));
		temp = vec2d<int>(parent[temp.x][temp.y][0], parent[temp.x][temp.y][1]);
	}
	std::reverse(path.begin(), path.end());
	if(path.size() > 0)
		path.push_back(ptarget);
	return path;
}
