// #include "Pathfinding.h"

// class cmp{
// public:
// 	bool operator () (const std::pair<int, std::pair<int, vec2d<int>>>& a, const std::pair<int, std::pair<int, vec2d<int>>>& b){
// 		return a.first < b.first;
// 	}
// };


// bool Pathfinding::Walkable(const vec2d<float>& src, const vec2d<float>& target){
// 	bool res = true;
// 	bool xDirection = (target.x - src.x >= 0 ? 1 : 0);
// 	bool yDirection = (target.y - src.y >= 0 ? 1 : 0);

// 	float dx = target.x - src.x;
// 	float dy = target.y - src.y;
// 	float dist = src.Distance(target);
// 	float vx = dx / dist;
// 	float vy = dy / dist;
// 	int length = Tm->GetLengthOfGrid();
// 	vec2d<float> src2 = src;

// 	while(res && Tm->WorldToTile(src2) != Tm->WorldToTile(target)){
// 		float t1 = (xDirection ? floor(src2.x / length) * length + length: floor(src2.x / length) * length) - src2.x;
// 		float t2 = (yDirection ? floor(src2.y / length) * length + length: floor(src2.y / length) * length) - src2.y;
// 		float stime = std::min(t1 / vx,  t2 / vy) + 1;
// 		src2 += vec2d<float>(vx * stime , vy * stime);
// 		res &= !(Tm->IsOccupiedFromWorld(src2));
// 	}
// 	return res;	
// }

// std::deque<vec2d<float>> Pathfinding::OptimizePath(std::deque<vec2d<float>>& path){
// 	// while(path.size() > 1 && Walkable(path[0], path[1])){

// 	// }
// 	return path;	
// }

// std::deque<vec2d<float>> Pathfinding::CalculatePath(const vec2d<int>& src, const vec2d<int>& target){
// 	const static int dx[4] = {0,0,-1,1};
// 	const static int dy[4] = {1,-1,0,0};
// 	static int dist[100][100];
// 	static bool vis[100][100];
// 	static int parent[100][100][2];
// 	std::deque<vec2d<float>> path;
// 	memset(dist, 0x3f, sizeof(dist));
// 	memcpy(vis, Tm->GetGrid(), sizeof(vis));
// 	memset(parent, 0, sizeof(parent));
// 	// path.clear();

// 	// pair<heuristic, pair<distance from src, vec2d<int>next_cell>>
// 	std::priority_queue<
// 		std::pair<int, std::pair<int, vec2d<int>>>,
// 		std::vector<std::pair<int, std::pair<int, vec2d<int>>>>,
// 		cmp> pq;

// 	pq.push({0,{0,src}});

// 	// A* search
// 	while(!pq.empty()){
// 		auto f = pq.top();
// 		pq.pop();	
// 		f.first *= -1;
// 		int x = f.second.second.x;
// 		int y = f.second.second.y;
		
// 		if(f.second.second == target){
// 			dist[x][y] = f.second.first;
// 			vis[x][y] = 1;
// 			break;
// 		}else if(vis[x][y]){
// 			continue;
// 		}

// 		dist[x][y] = f.second.first;
// 		vis[x][y] = 1;

// 		if(vec2d<int>(x, y) == target){
// 			break;
// 		}
// 		for(int i = 0;i < 4;++i){
// 			int xx = x + dx[i];
// 			int yy = y + dy[i];

// 			// restrict area of searching
// 			if(xx >= 0 && yy >= 0 && xx < 100 && yy < 100 && !vis[xx][yy]){
// 				int cost = vec2d<int>(xx, yy).Manhattan(target);
// 				if(dist[xx][yy] > f.second.first + cost + 1){
// 					dist[xx][yy] = f.second.first + cost + 1;
// 					parent[xx][yy][0] = x;
// 					parent[xx][yy][1] = y;
// 				}
// 				pq.push({-f.second.first - 1 - cost, {f.second.first + 1, vec2d<int>(xx, yy)}});
// 			}
// 		}
// 	}
// 	if(dist[target.x][target.y] > 100000){
// 		// std::cout << "Can't reach " << target.x << ',' << target.y << " from " << src.x << ',' << src.y << std::endl;
// 		return path;
// 	}

// 	// Traceback and get path
// 	vec2d<int> temp = target;
// 	while(temp != src){
// 		path.emplace_back(Tm->TileToWorld(temp, 1));
// 		temp = vec2d<int>(parent[temp.x][temp.y][0], parent[temp.x][temp.y][1]);
// 	}
// 	std::reverse(path.begin(), path.end());

// 	return path;
// }