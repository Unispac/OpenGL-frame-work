#ifndef GEOMETRY_H
#define GEOMETRY_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<renderingEngine/CAMERA.h>

class Geometry  //几何体的根类
{
public:
	void draw(Shader *myShader)
	{
		myShader->use();
		Camera::currentCamera->updateMatrix(myShader, 1);
		myShader->setFloat4("material.texture_diffuse_color", color->r,color->g,color->b,color->a);
		myShader->SetMat4("model", glm::value_ptr(unitMatrix));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, pointCnt);
		glBindVertexArray(0);
	}
protected:
	static glm::vec4 unitMatrix;
	unsigned int VAO;
	unsigned int pointCnt;
	glm::vec4 *color;
	glm::vec4 model;
};  //
class Cubiod : public Geometry  //立方体类
{
public:
	Cubiod(float x, float y, float z, float a, float b, float c, glm::vec4 *color)
	{
		VAO = make_cuboid(x, y, z, a, b, c);
		pointCnt = 36;
		this->color = color;
	}
private:
	unsigned int make_cuboid(float x, float y, float z, float a, float b, float c) {
		float pos[] = {
			a,b,c,-1,-1,1,
			a + x,b,c,1,-1,1,
			a,b + y,c,-1,1,1,

			a,b + y,c,-1,1,1,
			a + x,b,c,1,-1,1,
			a + x,b + y,c,1,1,1,

			a,b + y,c,-1,1,1,
			a + x,b + y,c,1,1,1,
			a,b + y,c - z,-1,1,-1,

			a,b + y,c - z,-1,1,-1,
			a + x,b + y,c,1,1,1,
			a + x,b + y,c - z,1,1,-1,

			a + x,b,c,1,-1,1,
			a + x,b,c - z,1,-1,-1,
			a + x,b + y,c,1,1,1,

			a + x,b + y,c,1,1,1,
			a + x,b,c - z,1,-1,-1,
			a + x,b + y,c - z,1,1,-1,

			a + x,b,c,1,-1,1,
			a,b,c,-1,-1,1,
			a,b,c - z,-1,-1,-1,

			a,b,c - z,-1,-1,-1,
			a + x,b,c - z,1,-1,-1,
			a + x,b,c,1,-1,1,

			a,b,c,-1,-1,1,
			a,b + y,c,-1,1,1,
			a,b + y,c - z,-1,1,-1,

			a,b + y,c - z,-1,1,-1,
			a,b,c - z,-1,-1,-1,
			a,b,c,-1,-1,1,

			a,b,c - z,-1,-1,-1,
			a,b + y,c - z,-1,1,-1,
			a + x,b + y,c - z,1,1,-1,

			a + x,b + y,c - z,1,1,-1,
			a + x,b,c - z,1,-1,-1,
			a,b,c - z,-1,-1,-1,



		};
		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (216) * sizeof(float), pos, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		return VAO;
	}
}; //立方体
class Cylinder : public Geometry  //圆柱
{
public:
	Cylinder(float r1, float r2, float h, float x, float y, float z, glm::vec4 *color)
	{
		VAO = make_cylinder(r1, r2, h, x, y, z);
		pointCnt = (int)(r1 * 18 * 12);
		this->color = color;
	}
private:
	unsigned int make_cylinder(float r1, float r2, float h, float a, float b, float c) {
		float pi = 3.1415926;
		int n = r1 * 18;
		float *pos = new float[24 * n * 3];
		int q = 0;
		float x, y, x1, y1;
		for (float t = 0.0; t < 360; t += 360.0 / n) {
			x = cos(t / 180 * pi);
			y = sin(t / 180 * pi);
			x1 = cos((t + 360.0 / n) / 180 * pi);
			y1 = sin((t + 360.0 / n) / 180 * pi);
			pos[q] = r1 * x + a;
			pos[q + 1] = r1 * y + b;
			pos[q + 2] = 0 + c;
			pos[q + 3] = r1 * x;
			pos[q + 4] = r1 * y;
			pos[q + 5] = 0;
			q += 6;
			pos[q] = r1 * x1 + a;
			pos[q + 1] = r1 * y1 + b;
			pos[q + 2] = 0 + c;
			pos[q + 3] = r1 * x1;
			pos[q + 4] = r1 * y1;
			pos[q + 5] = 0;
			q += 6;
			pos[q] = r2 * x1 + a;
			pos[q + 1] = r2 * y1 + b;
			pos[q + 2] = h + c;
			pos[q + 3] = r2 * x1;
			pos[q + 4] = r2 * y1;
			pos[q + 5] = 0;
			q += 6;

			pos[q] = r2 * x1 + a;
			pos[q + 1] = r2 * y1 + b;
			pos[q + 2] = h + c;
			pos[q + 3] = r2 * x1;
			pos[q + 4] = r2 * y1;
			pos[q + 5] = 0;
			q += 6;
			pos[q] = r2 * x + a;
			pos[q + 1] = r2 * y + b;
			pos[q + 2] = h + c;
			pos[q + 3] = r2 * x;
			pos[q + 4] = r2 * y;
			pos[q + 5] = 0;
			q += 6;
			pos[q] = r1 * x + a;
			pos[q + 1] = r1 * y + b;
			pos[q + 2] = 0 + c;
			pos[q + 3] = r1 * x;
			pos[q + 4] = r1 * y;
			pos[q + 5] = 0;
			q += 6;

		}
		for (float t = 0.0; t < 360; t += 360.0 / n) {
			x = cos(t / 180 * pi);
			y = sin(t / 180 * pi);
			x1 = cos((t + 360.0 / n) / 180 * pi);
			y1 = sin((t + 360.0 / n) / 180 * pi);
			pos[q] = r2 * x + a;
			pos[q + 1] = r2 * y + b;
			pos[q + 2] = h + c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = 1;
			q += 6;
			pos[q] = r2 * x1 + a;
			pos[q + 1] = r2 * y1 + b;
			pos[q + 2] = h + c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = 1;
			q += 6;
			pos[q] = a;
			pos[q + 1] = b;
			pos[q + 2] = h + c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = 1;
			q += 6;

		}
		for (float t = 0.0; t < 360; t += 360.0 / n) {
			x = cos(t / 180 * pi);
			y = sin(t / 180 * pi);
			x1 = cos((t + 360.0 / n) / 180 * pi);
			y1 = sin((t + 360.0 / n) / 180 * pi);
			pos[q] = r1 * x + a;
			pos[q + 1] = r1 * y + b;
			pos[q + 2] = 0 + c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = -1;
			q += 6;
			pos[q] = a;
			pos[q + 1] = b;
			pos[q + 2] = c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = -1;
			q += 6;
			pos[q] = r1 * x1 + a;
			pos[q + 1] = r1 * y1 + b;
			pos[q + 2] = c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = -1;
			q += 6;


		}
		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (n * 24 * 3) * sizeof(float), pos, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		delete(pos);
		return VAO;
	}
};
class Ellipsiod : public Geometry //球体
{
public:
	Ellipsiod(float r1, float x, float y, float z, glm::vec4 *color)
	{
		int n = (int)(r1 * 5);
		pointCnt = (int)(6 * (4 * n + 1)*(2 * n + 1));
		this->color = color;
		VAO = make_ball(r1, x, y, z);
	}
private:
	unsigned int make_ball(float r, float a, float b, float c) {
		float pi = 3.1415926;
		int n = (int)(r * 5);
		float *ball = new float[6 * 6 * (4 * n + 1)*(2 * n + 1)];
		float t = pi / (2 * n);
		int time = 0;
		//cout << r << endl;
		for (int alpha = 0; alpha <= 4 * n; alpha += 1) {
			for (int beta = -n; beta <= n; beta += 1) {
				// 第一个点的坐标
				float x0 = (float)(r * cos(alpha*t) * cos(beta*t));
				float y0 = (float)(r * sin(alpha*t)* cos(beta*t));
				float z0 = (float)(r * sin(beta*t));

				// 第二个点的坐标
				float x1 = (float)(r*cos((alpha + 1)*t)*cos(beta*t));
				float y1 = (float)(r * sin((alpha + 1)*t)* cos(beta*t));
				float z1 = (float)(r * sin(beta*t));

				// 第三个点的坐标
				float x2 = (float)(r*cos((alpha + 1)*t)*cos(beta*t + t));
				float y2 = (float)(r * sin((alpha + 1)*t)* cos(beta*t + t));
				float z2 = (float)(r * sin(beta*t + t));

				// 第三个点的坐标
				float x3 = (float)(r*cos(alpha *t)*cos(beta*t + t));
				float y3 = (float)(r * sin(alpha *t)* cos(beta*t + t));
				float z3 = (float)(r * sin(beta*t + t));

				ball[time + 0] = x0 + a;
				ball[time + 1] = y0 + b;
				ball[time + 2] = z0 + c;
				ball[time + 3] = x0;
				ball[time + 4] = y0;
				ball[time + 5] = z0;

				ball[time + 6] = x1 + a;
				ball[time + 7] = y1 + b;
				ball[time + 8] = z1 + c;
				ball[time + 9] = x1;
				ball[time + 10] = y1;
				ball[time + 11] = z1;

				ball[time + 12] = x2 + a;
				ball[time + 13] = y2 + b;
				ball[time + 14] = z2 + c;
				ball[time + 15] = x2;
				ball[time + 16] = y2;
				ball[time + 17] = z2;

				time += 18;
				ball[time + 0] = x0 + a;
				ball[time + 1] = y0 + b;
				ball[time + 2] = z0 + c;
				ball[time + 3] = x0;
				ball[time + 4] = y0;
				ball[time + 5] = z0;

				ball[time + 6] = x2 + a;
				ball[time + 7] = y2 + b;
				ball[time + 8] = z2 + c;
				ball[time + 9] = x2;
				ball[time + 10] = y2;
				ball[time + 11] = z2;

				ball[time + 12] = x3 + a;
				ball[time + 13] = y3 + b;
				ball[time + 14] = z3 + c;
				ball[time + 15] = x3;
				ball[time + 16] = y3;
				ball[time + 17] = z3;
				time += 18;
			}
		}

		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (6 * 6 * (4 * n + 1)*(2 * n + 1)) * sizeof(float), ball, GL_STATIC_DRAW);

		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		
		delete(ball);
		return VAO;
	}
};
class PyramidTemplete : public Geometry  //棱台形的根类
{
protected:
	unsigned int make_pyramid(int n, float r1, float r2, float h, float a, float b, float c) {
		float pi = 3.1415926;
		float *pos = new float[24 * n * 3];
		int q = 0;
		float x, y, x1, y1;
		for (float t = 0.0; t < 360; t += 360.0 / n) {
			x = cos(t / 180 * pi);
			y = sin(t / 180 * pi);
			x1 = cos((t + 360.0 / n) / 180 * pi);
			y1 = sin((t + 360.0 / n) / 180 * pi);
			pos[q] = r1 * x + a;
			pos[q + 1] = r1 * y + b;
			pos[q + 2] = 0 + c;
			pos[q + 3] = r1 * x;
			pos[q + 4] = r1 * y;
			pos[q + 5] = 0;
			q += 6;
			pos[q] = r1 * x1 + a;
			pos[q + 1] = r1 * y1 + b;
			pos[q + 2] = 0 + c;
			pos[q + 3] = r1 * x1;
			pos[q + 4] = r1 * y1;
			pos[q + 5] = 0;
			q += 6;
			pos[q] = r2 * x1 + a;
			pos[q + 1] = r2 * y1 + b;
			pos[q + 2] = h + c;
			pos[q + 3] = r2 * x1;
			pos[q + 4] = r2 * y1;
			pos[q + 5] = 0;
			q += 6;

			pos[q] = r2 * x1 + a;
			pos[q + 1] = r2 * y1 + b;
			pos[q + 2] = h + c;
			pos[q + 3] = r2 * x1;
			pos[q + 4] = r2 * y1;
			pos[q + 5] = 0;
			q += 6;
			pos[q] = r2 * x + a;
			pos[q + 1] = r2 * y + b;
			pos[q + 2] = h + c;
			pos[q + 3] = r2 * x;
			pos[q + 4] = r2 * y;
			pos[q + 5] = 0;
			q += 6;
			pos[q] = r1 * x + a;
			pos[q + 1] = r1 * y + b;
			pos[q + 2] = 0 + c;
			pos[q + 3] = r1 * x;
			pos[q + 4] = r1 * y;
			pos[q + 5] = 0;
			q += 6;

		}
		for (float t = 0.0; t < 360; t += 360.0 / n) {
			x = cos(t / 180 * pi);
			y = sin(t / 180 * pi);
			x1 = cos((t + 360.0 / n) / 180 * pi);
			y1 = sin((t + 360.0 / n) / 180 * pi);
			pos[q] = r2 * x + a;
			pos[q + 1] = r2 * y + b;
			pos[q + 2] = h + c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = 1;
			q += 6;
			pos[q] = r2 * x1 + a;
			pos[q + 1] = r2 * y1 + b;
			pos[q + 2] = h + c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = 1;
			q += 6;
			pos[q] = a;
			pos[q + 1] = b;
			pos[q + 2] = h + c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = 1;
			q += 6;

		}
		for (float t = 0.0; t < 360; t += 360.0 / n) {
			x = cos(t / 180 * pi);
			y = sin(t / 180 * pi);
			x1 = cos((t + 360.0 / n) / 180 * pi);
			y1 = sin((t + 360.0 / n) / 180 * pi);
			pos[q] = r1 * x + a;
			pos[q + 1] = r1 * y + b;
			pos[q + 2] = 0 + c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = -1;
			q += 6;
			pos[q] = a;
			pos[q + 1] = b;
			pos[q + 2] = c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = -1;
			q += 6;
			pos[q] = r1 * x1 + a;
			pos[q + 1] = r1 * y1 + b;
			pos[q + 2] = c;
			pos[q + 3] = 0;
			pos[q + 4] = 0;
			pos[q + 5] = -1;
			q += 6;


		}
		unsigned int VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, (n * 24 * 3) * sizeof(float), pos, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		delete(pos);
		return VAO;
	}
};
class Tpryamid : public PyramidTemplete
{
public:
	void Tpyramid(int n, float r1, float r2, float h, float a, float b, float c, glm::vec4 *color)
	{
		VAO = make_pyramid(n, r1, r2, h, a, b, c);
		pointCnt = 12 * n;
		this->color = color;
	}  //棱台
};
class Prisms : public PyramidTemplete
{
	Prisms(int n, float r2, float h, float a, float b, float c, glm::vec4 *color)
	{
		VAO = make_pyramid(n, r2, r2, h, a, b, c);
		pointCnt = 12 * n;
		this->color = color;
	}    //棱柱
};
class Pyramid : public PyramidTemplete
{
	Pyramid(int n, float r2, float h, float a, float b, float c, glm::vec4 *color)
	{
		VAO = make_pyramid(n, 0, r2, h, a, b, c);
		pointCnt = 12 * n;
		this->color = color;
	}   //棱锥
};
class Taper : public PyramidTemplete
{
	Taper(float r2, float h, float a, float b, float c, glm::vec4 *color)
	{
		VAO = make_pyramid(360, 0, r2, h, a, b, c);
		pointCnt = 12 * 360;
		this->color = color;
	}   //圆锥
};

glm::vec4 Geometry::unitMatrix;
#endif
