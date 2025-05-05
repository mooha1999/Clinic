#include "Resource.h"

int Resource::getId()
{
  return id;
}

int Resource::getCapacity()
{
  return capacity;
}

int Resource::getCurrentCapacity()
{
  return currentCapacity;
}

void Resource::incrementCurrentCapacity()
{
  if (currentCapacity < capacity)
  {
    currentCapacity++;
  }
}

void Resource::decrementCurrentCapacity()
{
  if (currentCapacity > 0)
  {
    currentCapacity--;
  }
}

bool Resource::isAvailable()
{
  return currentCapacity < capacity;
}

ResourceType Resource::getType()
{
  return type;
}
