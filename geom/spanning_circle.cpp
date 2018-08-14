// ADD BASIC GEOM HERE

Circle minCircle(vector<pt> what){
	srand(time(0));
	int n = what.size();
	for(int i = 0; i < n; i++){
		int j = rand()%(i + 1);
		swap(what[i], what[j]);
	}
	if(what.empty())return Circle({0, 0}, 0);
	Circle ans(what[0], 0);
	for(int i = 1; i < n; i++){
		if(ans.c.dist(what[i]) < ans.r + eps)continue;
		ans = Circle((what[0] + what[i])/2, what[0].dist(what[i])/2);
		for(int j = 0; j <= i; j++){
			if(ans.c.dist(what[j]) < ans.r + eps)continue;
			ans = Circle((what[j] + what[i])/2, what[i].dist(what[j])/2);
			for(int k = 0; k <= j; k++){
				if(ans.c.dist(what[k]) < ans.r + eps)continue;
				pt cen = trCirc(what[i], what[j], what[k]);
				ans = Circle(cen, cen.dist(what[i]));
			}
		}
	}
	return ans;
}
