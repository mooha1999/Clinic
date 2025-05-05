#pragma once

enum ResourceType
{
	ELECTRO,
	ULTRASOUND,
	EXERCISE,
};

class Resource
{
	int id;
	ResourceType type;
	const int capacity;
	int currentCapacity = 0;

public:
	Resource(int id, ResourceType type, int capacity = 1) : id(id), type(type), capacity(capacity) {}
	int getId();
	int getCapacity();
	int getCurrentCapacity();
	void incrementCurrentCapacity();
	void decrementCurrentCapacity();
	bool isAvailable();
	ResourceType getType();
};
