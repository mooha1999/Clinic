#pragma once

enum ResourceType {
	ELECTRO,
	ULTRASOUND,
	EXERCISE,
};

class Resource
{
public:
	Resource(int id, ResourceType type, int capacity = 1) :
		id(id), type(type), capacity(capacity) {}
private:
	int id;
	ResourceType type;
	int capacity;
};

