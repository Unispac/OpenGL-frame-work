#ifndef CHESSGO_H
#define CHESSGO_H

#include<renderingEngine/MYTOOLKIT.h>
#include<myChess/CHESSMODELMANAGER.h>
#include<attachedComponent/NURBSCAR.h>

#define EMPTY 0
#define R_SOLDIER 1
#define R_CANNON 2
#define R_CHARIOT 3
#define R_HORSE 4
#define R_MINISTER 5
#define R_STAFF 6
#define R_GENERAL 7
#define B_SOLDIER 8
#define B_CANNON 9
#define B_CHARIOT 10
#define B_HORSE 11
#define B_MINISTER 12
#define B_STAFF 13
#define B_GENERAL 14

class chessGo
{
private:
	int board[9][10];
	Shader * explode;
	glm::vec2 pos[9][10];
	
	float gap;
	Shader * myShader;
	Model *modelResource[14];
	chessModel *myChess[33];
	nurbsCar *myCar;
	bool going;
	glm::vec3 goDir;
	int goId;
	float goTime;

	int type[33];
	int locX[33];
	int locY[33];
	static glm::vec3 lightDir;
	int explodedTarget;
	bool jump;

	void initialPos()
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				pos[i][j].y = (i - 4)*gap;
				pos[i][j].x = (j - 4.5)*gap;
			}
		}
	}
	void bindModel()
	{
		glm::vec4 *tempR= new glm::vec4(1.0f,0.3f,0.3f,0.5f);
		glm::vec4 *tempB = new glm::vec4(0.2f, 0.2f, 0.2f, 0.5f);
		glm::vec4 *temp = new glm::vec4(0.6, 0.6, 0.6, 1.0);
		bool Holographic = false;
		modelResource[0] = new Model("./resource/chess/soldier_r/soldier_r.obj",tempR, Holographic);

		modelResource[1] = new Model("./resource/chess/cannon_r/cannon_r.obj", tempR, Holographic);

		//modelResource[2] = new Model("./resource/chess/chariot/chariot.obj", tempR, Holographic);
		myCar = new nurbsCar(50,tempR);

		modelResource[3] = new Model("./resource/chess/horse_r/horse_r.obj", tempR, Holographic);

		modelResource[4] = new Model("./resource/chess/minister_r/minister_r.obj", tempR, Holographic);

		modelResource[5] =  new Model("./resource/chess/staff_r/staff_r.obj", tempR, Holographic);

		modelResource[6] = new Model("./resource/chess/general_r/general_r.obj", tempR, Holographic);


		modelResource[7] = new Model("./resource/chess/soldier_b/soldier_b.obj", tempB, Holographic);

		modelResource[8] = new Model("./resource/chess/cannon_b/cannon_b.obj", tempB, Holographic);

		modelResource[9] = new Model("./resource/chess/chariot_b/chariot_b.obj", tempB, Holographic);
		

		modelResource[10] = new Model("./resource/chess/horse_b/horse_b.obj", tempB, Holographic);

		modelResource[11] = new Model("./resource/chess/minister_b/minister_b.obj", tempB, Holographic);

		modelResource[12] = new Model("./resource/chess/staff_b/staff_b.obj", tempB, Holographic);

		modelResource[13] = new Model("./resource/chess/general_b/general_b.obj", tempB, Holographic);

	}
	void initialExplode()
	{
		explode = new Shader("./shader/objectVertexShader.sd",
			"./shader/objectFragmentShader.sd","./shader/objectGeometryShader.sd");
		explode->use();
		explode->setFloat1("material.shininess", 32.0);
		explode->setBool("light.pointlight", false);
		explode->setFloat3("light.ambient", 0.3, 0.3, 0.3);
		explode->setFloat3("light.diffuse", 0.8, 0.8, 0.8);
		explode->setFloat3("light.specular", 1.0, 1.0, 1.0);
		explode->setFloat3("light.direction", 1.0, -1.0, 0.0);
		explode->setBool("toExplode", true);
		//explode->enablegeometry();
	}
	bool checkRule(int id, int row, int col)
	{
		static int x, y, ctype,ttype,cnt,st,ed,i;
		x = locX[id]; y = locY[id];
		ctype = type[id];
		ttype = board[row][col] ? type[board[row][col]] : 0;
		if (ttype)
		{
			if (ctype > 7 && ttype > 7)return false;
			else if (ctype <= 7 && ttype <= 7)return false;
		}
		if (id<31&&locX[31] == locX[32]&&locX[31]==x)
		{
			cnt = 0;
			st = min(locY[31], locY[32]);
			ed = max(locY[31], locY[32]);
			for (i = st + 1; i < ed; i++)if (board[x][i])cnt++;
			if (cnt == 1 && row != x)return false;
		}
		if (id == 31&&row==locX[32])
		{
			st = min(locY[31], locY[32]);ed = max(locY[31], locY[32]);
			cnt = 0;
			for (i = st + 1; i < ed; i++)if (board[row][i])cnt++;
			if (cnt == 0)return false;
			else if (cnt == 1 && board[row][col])return false;
		}
		if (id == 32&&row == locX[31])
		{
			st = min(locY[31], locY[32]); ed = max(locY[31], locY[32]);
			cnt = 0;
			for (i = st + 1; i < ed; i++)if (board[row][i])cnt++;
			if (cnt == 0)return false;
			else if (cnt == 1 && board[row][col])return false;
		}
		switch (ctype)
		{
		case B_SOLDIER:
			if (col - y > 0)return false;
			else if (y > 4 && row - x)return false;
			else if (abs(row - x) + abs(col - y) > 1)return false;
			else return true;
			break;
		case R_SOLDIER:
			if (col - y < 0)return false;
			else if (y < 5 && row - x)return false;
			else if (abs(row - x) + abs(col - y) > 1)return false;
			else return true;
			break;
		case R_CANNON:
		case B_CANNON:
		case R_CHARIOT:
		case B_CHARIOT:
			if (col - y && row - x)return false;
			else if (col - y)
			{
				cnt = 0;	st = min(col, y); ed = max(col, y);
				for (i = st + 1; i < ed; i++)if (board[x][i])cnt++;
			}
			else
			{
				cnt = 0; st = min(row, x); ed = max(row, x);
				for (i = st + 1; i < ed; i++)if (board[i][col])cnt++;
			}
			if ((ctype == B_CANNON || ctype == R_CANNON))
			{
				if (ttype != 0 && cnt != 1)return false;
				else if (ttype == 0 && cnt != 0)return false;
			}
			else if (cnt != 0)return false;

			if (cnt == 1)jump = true;
			return true;
			break;
		case R_HORSE:
		case B_HORSE:
			st = abs(row - x); ed = abs(col - y);
			if (!((st == 1 && ed == 2) || (st == 2 && ed == 1)))return false;
			if (st == 2) 
			{
				if (board[(row - x) / 2 + x][y])return false;
			}
			else
			{
				if (board[x][(col - y) / 2 + y])return false;
			}
			jump = true;
			return true;
			break;
		case R_MINISTER:
		case B_MINISTER:
			st = abs(row - x); ed = abs(col - y);
			if (st != 2 || ed != 2)return false;
			if (board[(row - x) / 2 + x][(col - y) / 2 + y])return false;
			if (ctype == B_MINISTER && col <5)return false;
			if (ctype == R_MINISTER && col >4)return false;
			jump = true;
			return true;
			break;
		case R_STAFF:
			st = row - x; ed = col - y;
			if (abs(st) != 1 || abs(ed) != 1)return false;
			if (x == 4 && y == 1)return true;
			else if (x == 3)
			{
				if (y == 0 && st == 1)return true;
				if (y == 2 && st == 1 && ed == -1)return true;
			}
			else if (x == 5)
			{
				if (y == 0 && st == -1)return true;
				if (y == 2 && st == -1 && ed == -1)return true;
			}
			return false;
		case B_STAFF:
			st = row - x; ed = col - y;
			if (abs(st) != 1 || abs(ed) != 1)return false;
			if (x == 4 && y == 8)return true;
			else if (x == 3)
			{
				if (y == 9 && st == 1)return true;
				if (y == 7 && st == 1 && ed == 1)return true;
			}
			else if (x == 5)
			{
				if (y == 9 && st == -1)return true;
				if (y == 7 && st == -1 && ed == 1)return true;
			}
			return false;
		case R_GENERAL:
		case B_GENERAL:
			if (row < 3 || row>5)return false;
			if (ctype ==R_GENERAL&&col >2)return false;
			if (ctype == B_GENERAL && col <7)return false;
			st = row - x; ed = col - y;
			if (abs(st) + abs(ed) > 1)return false;
			return true;
		}
	}
public:
	bool focusLock;
	chessGo(float gap)
	{
		this->gap = gap;
		this->myShader = Toolkit::myShader[Toolkit::modelShader];
			
		bindModel();
		initialPos();
		initialBoard();
		initialExplode();
		going = false;
		focusLock = false;
		//Tag(float cx, float cz, float gap, Shader *myShader,bool split=false)
	
	}
	void initialBoard()
	{
		static int i, j;
		static int k, t;
		static bool firstInit = true;
		static glm::vec3 temp;
		
		

		
		for (i = 0; i < 9; i++)for (j = 0; j < 10; j++)board[i][j] = EMPTY;
		if(!firstInit)
		for (i = 1; i <= 32; i++)
		{
			temp.x = -pos[locX[i]][locY[i]].x;
			temp.z= -pos[locX[i]][locY[i]].y;
			temp.y = 0;
			myChess[i]->translate(&temp);
		}

		
		int cnt = 0;
		for (i = 0; i < 9; i += 2)
		{
			board[i][3] = ++cnt;type[cnt]= R_SOLDIER;
			board[i][6] = ++cnt;type[cnt]= B_SOLDIER;
			if (firstInit)
			{
				temp.x = 4.0; temp.z = 0; temp.y = 2.0;
				//chessModel(Model *chess, float scaleFactor, float rotAngle, bool Red)
				myChess[cnt - 1] = new chessModel(modelResource[0],0.01,90,true,&temp);
				temp.x=0.0; temp.z = 1.7; temp.y = 4.0;
				myChess[cnt] = new chessModel(modelResource[7], 0.13,-90, false, &temp);
			}
		}
		for (i = 1; i < 9; i += 6)
		{
			board[i][2] = ++cnt;type[cnt]= R_CANNON;
			board[i][7] = ++cnt;type[cnt]=B_CANNON;
			if (firstInit)
			{
				temp.x =1.5; temp.z = 0; temp.y = 4.0;
				myChess[cnt - 1] = new chessModel(modelResource[1], 0.01, 0, true, &temp);
				temp.x = -1.5;
				myChess[cnt] = new chessModel(modelResource[8], 6.0f, -180, false, &temp);
			}
		}
		for (i = 0; i < 9; i += 8)
		{
			board[i][0] = ++cnt; type[cnt] = R_CHARIOT;
			board[i][9] = ++cnt; type[cnt] = B_CHARIOT;
			if (firstInit)
			{
				temp.x=5.0; temp.z = 0; temp.y = 5.0;
				myChess[cnt - 1] = new chessModel(myCar, 1.5, 180, true, &temp);
				temp.x = temp.z = 0; temp.y = 5.0;
				myChess[cnt] = new chessModel(modelResource[9], 0.01, 0, false, &temp);
			}
		}
		for (i = 1; i < 9; i += 6)
		{
			board[i][0] = ++cnt; type[cnt] = R_HORSE;
			board[i][9] = ++cnt; type[cnt] = B_HORSE;
			if (firstInit)
			{
				temp.x = temp.z = 0; temp.y = 5.0;
				myChess[cnt - 1] = new chessModel(modelResource[3], 8, 90, true, &temp);
				myChess[cnt] = new chessModel(modelResource[10], 0.007, 180, false, &temp);
			}
		}
		for (i = 2; i < 9; i += 4)
		{
			board[i][0] = ++cnt; type[cnt] = R_MINISTER;
			board[i][9] = ++cnt; type[cnt] = B_MINISTER;
			if (firstInit)
			{
				temp.x = temp.z = 0; temp.y = 5.0;
				myChess[cnt - 1] = new chessModel(modelResource[4], 0.15, 0, true, &temp);
				temp.z = 2.0;
				myChess[cnt] = new chessModel(modelResource[11], 0.1, -90, false, &temp);
			}
		}
		for (i = 3; i < 6; i += 2)
		{
			board[i][0] = ++cnt;type[cnt]= R_STAFF;
			board[i][9] = ++cnt;type[cnt]= B_STAFF;
			if (firstInit)
			{
				temp.x = temp.z = 0; temp.y = 5.0;
				myChess[cnt - 1] = new chessModel(modelResource[5], 0.1, 90, true, &temp);
				temp.z = 2.2;
				myChess[cnt] = new chessModel(modelResource[12], 10, -90, false, &temp);
			}
		}

		board[4][0] = ++cnt;type[cnt]= R_GENERAL;
		board[4][9] = ++cnt;type[cnt]= B_GENERAL;
		if (firstInit)
		{
			temp.x = temp.z = 0; temp.y = 5.0;
			myChess[cnt - 1] = new chessModel(modelResource[6], 1, 90, true, &temp);
			myChess[cnt] = new chessModel(modelResource[13], 0.004, -90, false, &temp);
		}

		glm::vec3 *tempVec = new glm::vec3;
		for (i = 0; i < 9; i++)for (j = 0; j < 10; j++)
		{
			k = board[i][j];
			if (k != EMPTY)
			{
				locX[k] = i; locY[k] = j;
				tempVec->x = pos[i][j].x;
				tempVec->z = pos[i][j].y;
				tempVec->y = 0;
				myChess[k]->translate(tempVec);
			}
		}
		firstInit = false;
		return;
	}
	void go()
	{
		if (going == false)return;
		static float dtime;
		static float totTime=0;
		static glm::vec3 ds;
		static float lastHeight = 0;
		static float currentHeight;
		static float amptitude = gap;
		static const float PI = 3.1415926;
		static int temptype;
		
		if (Camera::deltatime +totTime > goTime)dtime = goTime-totTime;
		else dtime = Camera::deltatime;

		totTime += dtime;
		dtime /= goTime;

		if (jump == true)
		{
			currentHeight =1.5*gap*glm::sin(PI*totTime);
			ds.y = currentHeight - lastHeight;
			lastHeight = currentHeight;
		}
		else ds.y = 0;

	
		ds.x = goDir.x*dtime;
		ds.z = goDir.z*dtime;
		myChess[goId]->translate(&ds);

		if (explodedTarget)
		{
			explode->use();
			Toolkit::myCamera->updateMatrix(explode, 1);
			explode->setFloat1("factor", 0.5*totTime*PI / goTime);
			myChess[explodedTarget]->draw(explode);
		}

		if (goTime <= totTime)
		{
			going = false;
			totTime = 0;
			lastHeight = 0;
			focusLock = false;
			if (type[31] < 0 || type[32] < 0)initialBoard();
		}
		
		
	}
	bool moveChess(int id, int row, int col)
	{
		static glm::vec2 to, from;
		static int k;
		
		to = pos[row][col];
		from = pos[locX[id]][locY[id]];
		if (to == from)return false;
		jump = false;
		if (checkRule(id, row, col) == false)return false;
		
		focusLock = true;
		going = true;
		goId = id;
		k = type[id];
		if (k > 7)k -= 7; 
		goTime = 1.0;
		goDir.y = 0;
		goDir.x = to.x - from.x;
		goDir.z = to.y - from.y;
		k--;

		if (board[row][col] != 0)
		{
			explodedTarget = board[row][col];
			type[explodedTarget] *= -1;
		}
		else explodedTarget = 0;
		board[row][col] = id;
		board[locX[id]][locY[id]] = 0;
		locX[id]=row;
	    locY[id]=col;
		return true;
	}
	int target(int row,int col)
	{
		return board[row][col];
	}
	int getType(int id)
	{
		return type[id];
	}
	void drawChess()
	{
		go();
		static int i, j, k;
		myShader->use();
		
		Toolkit::myCamera->updateMatrix(myShader, 1);
		for(i=1;i<=32;i++)
			if (type[i] > 0)
			{
				myChess[i]->draw(myShader);
			}
	}
};
glm::vec3 chessGo::lightDir(1.0,-1.0,0.0);
#endif // !CHESSGO_H