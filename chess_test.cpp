#include <iostream>
#include <ctime> 

using namespace std;

struct directs {
	//directX表示每个方向有几个相连，liveX表示每个方向是死0、眠1、活2
	int direct[4];
	int live[4];
};

struct point {
	int x;
	int y;
	int mark;
};

directs compute(int board[29][29], int x, int y, int side);
int mark(directs s);
void copy(int board1[29][29], int board2[29][29]);


int* AI(int board[29][29], int side, int step) {
	int i, j, k;
	int marks[361] = { 0 };//i*19+j
	int temp_max_mark = 0;
	int temp_max_j = 0;
	point max[15]; 
	int * next;
	int enemy_side;
	int temp_board[29][29];

	next = new int[2];

	if (step == 1) {//第一步棋直接下在棋盘中央
		next[0] = 14;
		next[1] = 14;
		return next;
	}

	if (side == 1)enemy_side = 2;
	else enemy_side = 1;

	for (i = 0; i < 15; i++) {
		max[i].x = 0;
		max[i].y = 0;
		max[i].mark = 0;
	}

	for (i = 5; i < 24; i++) {
		for (j = 5; j < 24; j++){
			if (board[i][j] == 0) { //只考虑棋盘的空位
				marks[(i - 5) * 19 + (j - 5)] = mark(compute(board, i, j, side)) + mark(compute(board, i, j, enemy_side));
			}
		}
	}

	for (i = 0; i < 15; i++) { //在361个数中选出15个最大的，并记录坐标
		for (j = i; j < 361; j++) { 
			if (marks[j] > temp_max_mark) {
				temp_max_mark = marks[j];
				temp_max_j = j; //保存坐标
			}
		}
		marks[i] = temp_max_mark;
		max[i].mark = temp_max_mark;
		max[i].x = temp_max_j / 19; //向下取整
		max[i].y = temp_max_j % 19;
	}
	//以上完成第一步的选择
	//以下决策树未写，求补充

	next[0] = max[0].x + 5;
	next[1] = max[0].y + 5;
	return next;
}

int mark(directs s) { //根据directs内的各属性打分
/*死X：0分 
  活1：4分 眠1：2分 
  活2：16分 眠2：3分
  活3：64分 眠3：32分
  活4：512分 眠4:52分
  活5：512分 眠5：256分
  6：9999分
*/
	int sum = 0;
	int i;
	for (i = 0; i < 4; i++) {
		if (s.live[i] == 0) {//死
			if (s.direct[i] >= 6)sum += 9999;
			else sum += 0;
		}
		else if (s.live[i] == 1) { //眠
			if (s.direct[i] == 1)sum += 2;
			else if (s.direct[i] == 2)sum += 3;
			else if (s.direct[i] == 3)sum += 32;
			else if (s.direct[i] == 4)sum += 52;
			else if (s.direct[i] == 5)sum += 256;
			else if (s.direct[i] >= 6)sum += 9999;
		}
		else { //活
			if (s.direct[i] == 1)sum += 4;
			else if (s.direct[i] == 2)sum += 16;
			else if (s.direct[i] == 3)sum += 64;
			else if (s.direct[i] == 4)sum += 512;
			else if (s.direct[i] == 5)sum += 512;
			else if (s.direct[i] >= 6)sum += 9999;
		}
	}
	return sum;
}

void initialize(int board[29][29]) { //本函数的作用是把所有边界以外的点的初始值设为3
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 29; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 24; i < 29; i++) {
		for (j = 0; j < 29; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 5; i < 24; i++) {
		for (j = 0; j < 5; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 5; i < 24; i++) {
		for (j = 24; j < 29; j++) {
			board[i][j] = 3;
		}
	}
}

directs compute(int board[29][29], int x, int y, int side) { //判断落子后有多少个己方棋子相连（分四个方向）
	//以落子点为中心，计算周围
	int i;
	int flag = 0;
	int side1, side2;
	directs s;
	s.live[0] = 0;
	s.live[1] = 0;
	s.live[2] = 0;
	s.live[3] = 0;

	//横向
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x][y - i] == side) {
			side1++;
			if (i == 5)i++; 
		}
		else break;
	}
	if (board[x][y - i] == 0) {
		s.live[0]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x][y + i] == 0) {
		s.live[0]++;
	}
	s.direct[0] = 1 + side1 + side2;


	//竖向
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y] == 0) {
		s.live[1]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y] == 0) {
		s.live[1]++;
	}
	s.direct[1] = 1 + side1 + side2;

	//斜向 左上到右下
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y - i] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y - i] == 0) {
		s.live[2]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y + i] == 0) {
		s.live[2]++;
	}
	s.direct[2] = 1 + side1 + side2;

	//斜向 左下到右上
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y - i] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y - i] == 0) {
		s.live[3]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y + i] == 0) {
		s.live[3]++;
	}
	s.direct[3] = 1 + side1 + side2;
	
	return s;
}

void draw(int board[29][29]) {
	int i, j;
	for (i = 5; i < 24; i++) {
		for (j = 5; j < 24; j++) {
			if (board[i][j] == 0) {
				cout << "_ ";
			}
			else if (board[i][j] == 1) { //黑棋
				cout << "x ";
			}
			else { //白棋
				cout << "o ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

void move(int x, int y, int side, int board[29][29]) {
	if (board[x][y] == 0) {
		board[x][y] = side;
	}
}

bool victory(int board[29][29], int x, int y, int side) {
	directs s;

	s = compute(board, x, y, side);
	if (s.direct[0] >= 6)return true;
	if (s.direct[1] >= 6)return true;
	if (s.direct[2] >= 6)return true;
	if (s.direct[3] >= 6)return true;

	return false;
}

void copy(int board1[29][29], int board2[29][29]) {
	int i, j;
	for (i = 0; i < 29; i++) {
		for (j = 0; j < 29; j++) {
			board2[i][j] = board1[i][j];
		}
	}
}

int main()
{
	int chessboard[29][29] = { 0 };
	int next_x;
	int next_y;
	int steps = 1;
	int side = 1;
	bool win = 0;
	directs sums;
	int * temp;
	srand(time(0));
	temp = new int[2];

	initialize(chessboard);

	while (steps < 361) {
		//走棋
		do {
			temp = AI(chessboard, side, steps);
			next_x = temp[0];
			next_y = temp[1];
		} while (next_x < 5 || next_x > 23 || next_y < 5 || next_y > 23 || chessboard[next_x][next_y] != 0);
		move(next_x, next_y, side, chessboard);

		//画图
		cout << steps << "(" << next_x - 5 << ", " << next_y - 5 << ")" << endl;
		draw(chessboard);
		sums = compute(chessboard, next_x, next_y, side);
		cout << "横向：" << sums.direct[0] << " " << sums.live[0] << " ";
		cout << "竖向：" << sums.direct[1] << " " << sums.live[1] << " ";
		cout << " 左对角线：" << sums.direct[2] << " " << sums.live[2] << " ";
		cout << " 右对角线：" << sums.direct[3] << " " << sums.live[3] << endl;
		cout << mark(sums) << endl;
		cout << endl;

		//判断胜负
		win = victory(chessboard, next_x, next_y, side);
		if (win) {
			cout << "胜利" << endl;
			break;
		}

		//黑换白或白换黑
		if (steps % 4 == 1) {
			side = 2;
		}
		else if (steps % 4 == 3) {
			side = 1;
		}
		steps++;
	}
}


