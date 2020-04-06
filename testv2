#include <iostream>
#include <ctime> 

using namespace std;

#define BLACK 0
#define WHITE 1
#define EMPTY 2
#define number_of_situations 30


struct directs {
	//directX表示每个方向有几个相连，liveX表示每个方向是死0、眠1、活2
	int direct[4];
	int live[4];
};

struct point { //坐标
	int x;
	int y;
};

struct point_with_mark { //坐标、得分
	int x;
	int y;
	int mark;
};

struct strategy {
	point chess[6]; //六步棋，1、2、5、6我方，3、4敌方
	int mark;
};

//函数声明
directs compute(int board[29][29], int x, int y, int side);
int mark(directs s);
void copy(const int board1[29][29], int board2[29][29]);
void max_mark(point_with_mark highest[number_of_situations], const int board[29][29], int side);


//函数实现

point_with_mark best_strategy(const int chessboard[29][29], int mark, int layer, int original_side) { //核心函数（决策树）
	point_with_mark best_one;
	point_with_mark strategies[number_of_situations];
	int temp_chessboard[29][29];
	point_with_mark max[number_of_situations]; //25种最大情况
	int side;
	int new_mark;
	int i;

	//初始值
	best_one.mark = -1000000; 
	best_one.x = 5;
	best_one.y = 5;

	if (layer % 4 == 1 or layer % 4 == 2) side = original_side; //我方
	else side = 1 - original_side;//对方

	max_mark(max, chessboard, side); //修改max

	for (i = 0; i < number_of_situations; i++) {
		copy(chessboard, temp_chessboard);
		temp_chessboard[max[i].x][max[i].y] = side;
		if (original_side == side) new_mark = mark + max[i].mark;
		else new_mark = mark - max[i].mark;

		if (layer < 5 && new_mark > 0) {
			strategies[i] = best_strategy(temp_chessboard, new_mark, layer + 1, original_side);//调用自身
		//x、y的值与max相同，但最终分数不同。max的分数是根据眼前局面，此函数返回的分数是综合加减。
		}
		else {//在 第六层 或 局面不利 不再调用自身
			strategies[i].mark = new_mark;
			strategies[i].x = max[i].x;
			strategies[i].y = max[i].y;
		}
	}

	for (i = 0; i < number_of_situations; i++) {
		if (strategies[i].mark > best_one.mark) {
			best_one.mark = strategies[i].mark;
			best_one.x = strategies[i].x;
			best_one.y = strategies[i].y;
		}
	}

	if (layer == 1) {
		cout << endl;
	}

	return best_one;
}


void max_mark(point_with_mark highest[number_of_situations], const int board[29][29], int side) { //选出当前分最高的25个点，（通过参数）返回点坐标和分数
	int i, j;
	int marks[361] = { 0 };//i*19+j         所有点的分值
	int temp_max_mark = 0;
	int temp_max_j = 0;
	point_with_mark max[number_of_situations];//分值最高的25个点
	int tempboard[29][29];

	copy(board,tempboard);

	for (i = 0; i < number_of_situations; i++) {
		max[i].x = 0;
		max[i].y = 0;
		max[i].mark = 0;
	}

	for (i = 5; i < 24; i++) {
		for (j = 5; j < 24; j++) {
			if (board[i][j] == EMPTY) { //只考虑棋盘的空位
				marks[(i - 5) * 19 + (j - 5)] = mark(compute(tempboard, i, j, side));
			}
		}
	}

	for (i = 0; i < number_of_situations; i++) { //在361个数中选出25个最大的，并记录坐标
		for (j = 0; j < 361; j++) {
			if (marks[j] > temp_max_mark) {
				temp_max_mark = marks[j];
				temp_max_j = j; //保存坐标
			}
		}
		max[i].mark = temp_max_mark;
		max[i].x = temp_max_j / 19 + 5; //向下取整
		max[i].y = temp_max_j % 19 + 5;
		temp_max_mark = 0;
		marks[temp_max_j] = 0;//将本轮循环找到的最大值在marks数组中归零，避免下轮循环再次找到这个数
	}

	for (i = 0; i < number_of_situations; i++) {
		highest[i].x = max[i].x;
		highest[i].y = max[i].y;
		highest[i].mark = max[i].mark;
	}
}

int mark(directs s) { //根据directs内的各属性打分
	int result = 0;
	int i;
	int marks[4];

	for (i = 0; i < 4; i++) {
		if (s.live[i] == 0) {//死
			if (s.direct[i] >= 6) marks[i] = 999999;
			else marks[i] = 0;
		}
		else if (s.live[i] == 1) { //眠
			if (s.direct[i] == 1) marks[i] = 1;
			else if (s.direct[i] == 2)marks[i] = 10;
			else if (s.direct[i] == 3)marks[i] = 100;
			else if (s.direct[i] == 4)marks[i] = 1000;
			else if (s.direct[i] == 5)marks[i] = 10000;
			else if (s.direct[i] >= 6)marks[i] = 999999;
		}
		else { //活
			if (s.direct[i] == 1)marks[i] = 10;
			else if (s.direct[i] == 2)marks[i] = 100;
			else if (s.direct[i] == 3)marks[i] = 1000;
			else if (s.direct[i] == 4)marks[i] = 10000;
			else if (s.direct[i] == 5)marks[i] = 100000;
			else if (s.direct[i] >= 6)marks[i] = 999999;
		}
	}

	for (i = 0; i < 4; i++) {
		if (marks[i] > result)result = marks[i];
	}
	return result;
}


void initialize(int board[29][29]) { //本函数的作用是把所有边界以外的点的初始值设为3，边界以内的设为EMPTY
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
	for (i = 5; i < 24; i++) {
		for (j = 5; j < 24; j++) {
			board[i][j] = EMPTY;
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
	if (board[x][y - i] == EMPTY) {
		s.live[0]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x][y + i] == EMPTY) {
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
	if (board[x - i][y] == EMPTY) {
		s.live[1]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y] == EMPTY) {
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
	if (board[x - i][y - i] == EMPTY) {
		s.live[2]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y + i] == EMPTY) {
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
	if (board[x + i][y - i] == EMPTY) {
		s.live[3]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y + i] == EMPTY) {
		s.live[3]++;
	}
	s.direct[3] = 1 + side1 + side2;
	
	return s;
}

void draw(int board[29][29]) {
	int i, j;
	for (i = 5; i < 24; i++) {
		for (j = 5; j < 24; j++) {
			if (board[i][j] == EMPTY) {
				cout << "_ ";
			}
			else if (board[i][j] == BLACK) { //黑棋
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
	if (board[x][y] == EMPTY) {
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

void copy(const int board1[29][29], int board2[29][29]) {
	int i, j;
	for (i = 0; i < 29; i++) {
		for (j = 0; j < 29; j++) {
			board2[i][j] = board1[i][j];
		}
	}
}

int main()
{
	int chessboard[29][29];
	int next_x;
	int next_y;
	int steps = 1;
	int side = BLACK;
	bool win = 0;
	directs sums;
	point_with_mark next;
	srand(time(0));

	initialize(chessboard);

	while (steps < 361) {
		//走棋
		if (steps == 1) {
			next_x = 14;
			next_y = 14;
		}
		else if (steps == 2) {
			next_x = 13;
			next_y = 13;
		}
		else if (steps == 3) {
			next_x = 12;
			next_y = 14;
		}
		else {
			do {
				if (steps % 2 == 0)
					next = best_strategy(chessboard, 0, 1, side);
				else if (steps % 2 == 1)
					next = best_strategy(chessboard, 0, 2, side);
				next_x = next.x;
				next_y = next.y;
			} while (next_x < 5 || next_x > 23 || next_y < 5 || next_y > 23 || chessboard[next_x][next_y] != EMPTY);
		}
		move(next_x, next_y, side, chessboard);

		//画图
		cout << steps << "(" << next_x - 5 << ", " << next_y - 5 << ")" << endl;
		draw(chessboard);
		sums = compute(chessboard, next_x, next_y, side);
		cout << "横向：" << sums.direct[0] << " " << sums.live[0] << " ";
		cout << "竖向：" << sums.direct[1] << " " << sums.live[1] << " ";
		cout << " 左对角线：" << sums.direct[2] << " " << sums.live[2] << " ";
		cout << " 右对角线：" << sums.direct[3] << " " << sums.live[3] << endl;
		cout << endl;

		//判断胜负
		win = victory(chessboard, next_x, next_y, side);
		if (win) {
			cout << "胜利" << endl;
			break;
		}

		//黑换白或白换黑
		if (steps % 4 == 1) {
			side = WHITE;
		}
		else if (steps % 4 == 3) {
			side = BLACK;
		}
		steps++;
	}
}

