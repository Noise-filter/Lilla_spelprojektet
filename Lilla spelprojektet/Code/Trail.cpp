#include "Trail.h"

Trail::Trail(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, int timeToLive, float velocity, float lengthX, float lengthY, float lengthZ)
{
	this->nrOfVertsPerParticle = 1;
	this->intensity = intensity;
	
	this->color = color;
	this->lengthX = lengthX;
	this->lengthY = lengthY;
	this->lengthZ = lengthZ;

	this->emitter = new BaseParticle(position, D3DXVECTOR3(0,1,0), timeToLive, velocity);
}

Trail::~Trail()
{	
	if(this->emitter)
	{
		delete this->emitter;
		this->emitter = NULL;
	}
	
}

void Trail::update()
{
	cout << particles.size() << endl;
	for(int i = 0; i < this->intensity; i++)
	{
		emitt();
	}

	int i = 0;
	//for(int i = 0; i < this->particles.size(); i++)
	for(ListNode<BaseParticle>* walker = particles.getFirst(); walker != NULL; walker = walker->next)
	{
		if(!walker->value.update())
		{
			i++;
			//this->particles.erase(this->particles.begin() + i);
		}
	}

	particles.remove(i);
	createVertices();
}
vector<VertexColor> Trail::getVertexData()
{
	return vertices;
}

void Trail::updatePosition(D3DXVECTOR3 pos)
{
	emitter->setPosition(pos);
}

void Trail::emitt()
{
	BaseParticle Particle = *this->emitter;
	static float srand(time(NULL));
	float halfLengthX = this->lengthX * 0.5;
	float halfLengthY = this->lengthY * 0.5;
	float halfLengthZ = this->lengthZ * 0.5;

	D3DXVECTOR3 origPos = Particle.getPosition();

	float posX = origPos.x;
	float posY = origPos.y;
	float posZ = origPos.z;

	posX += ((float(rand()) / float(RAND_MAX)) * (halfLengthX - (-halfLengthX))) + (-halfLengthX);
	posY += ((float(rand()) / float(RAND_MAX)) * (halfLengthY - (-halfLengthY))) + (-halfLengthY);
	posZ += ((float(rand()) / float(RAND_MAX)) * (halfLengthZ - (-halfLengthZ))) + (-halfLengthZ);

	float dirX = (float)rand() / (float)RAND_MAX -0.5f;
	float dirY = (float)rand() / (float)RAND_MAX -0.5f;
	float dirZ = (float)rand() / (float)RAND_MAX -0.5f;

	
	Particle.setDirection(D3DXVECTOR3(dirX,dirY,dirZ));
	Particle.setPosition(D3DXVECTOR3(posX,posY,posZ));

	particles.insertLast(Particle);

	//this->particles.push_back(Particle);
	
}
void Trail::createVertices()
{
	this->vertices.clear();

	//for(int i = 0; i < this->particles.size(); i++)
	for(ListNode<BaseParticle>* walker = particles.getFirst(); walker != NULL; walker = walker->next)
	{
		VertexColor vert1;

		vert1.normal = D3DXVECTOR3(0,0,-1);
		vert1.pos = walker->value.getPosition();//this->particles.at(i).getPosition();
		vert1.color = this->color;

		vertices.push_back(vert1);
	}
}