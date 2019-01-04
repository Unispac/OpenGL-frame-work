#ifndef LIGHT_H
#define LIGHT_H

#include<attachedComponent/GEOMETRY.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
class Light
{
private:
	bool	pointLight;
	bool   attenuation;
	glm::vec3 pos,ambient,diffuse,specular,att;
	Shader *lightShader;
	Geometry * mylight;
	void buildModel()
	{
		glm::vec4 *lightColor = new glm::vec4(diffuse.x, diffuse.y, diffuse.z, 1.0);
		mylight = new Ellipsiod(4.0f,pos.x, pos.y, pos.z, lightColor);
		lightShader->use();
		lightShader->setFloat4("lightColor", diffuse.x, diffuse.y, diffuse.z, 1.0);
	}
public:
	Light(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool pointLight)
	{
		this->pointLight = pointLight;
		this->pos = pos;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		attenuation = false;
		if (pointLight)
		{
			lightShader = new Shader("./shader/lightVertexShader.sd", "./shader/lightFragmentShader.sd");
			buildModel();
		}
	}
	Light(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool pointLight,glm::vec3 att)
	{
		this->pointLight = pointLight;
		this->pos = pos;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->att = att;
		if (pointLight)
		{
			lightShader = new Shader("./shader/lightVertexShader.sd", "./shader/lightFragmentShader.sd");
			attenuation = true;
			buildModel();
		}
	}
	void setLight(Shader *myShader)
	{
		myShader->use();
		myShader->setBool("light.pointlight", pointLight);
		if (pointLight)myShader->setFloat3("light.position", pos.x,pos.y,pos.z);
		else myShader->setFloat3("light.direction", pos.x, pos.y, pos.z);
		myShader->setFloat3("light.ambient", ambient.x, ambient.y, ambient.z);
		myShader->setFloat3("light.diffuse", diffuse.x, diffuse.y, diffuse.z);
		myShader->setFloat3("light.specular", specular.x, specular.y, specular.z);
		if (attenuation)
		{
			myShader->setBool("light.toAttenuation", true);
			myShader->setFloat3("light.attenuation", att.x, att.y, att.z);
		}
		else
		{	
			myShader->setBool("light.toAttenuation",false);
		}
		myShader->setFloat1("material.shininess", 32.0);
	}
	void draw()
	{
		if (!pointLight)return;
		mylight->draw(lightShader);
	}
};
#endif // !LIGHT_H
