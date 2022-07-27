#include "Object.h"

#include "Engine.h"

Object::Object(Engine* engine, const Vector2D position, const Vector2D dimensions)
	: engine_(engine),
	position_(position),
	dimensions_(dimensions)
{
}

Object::~Object()
{
}

void Object::Tick(float deltaTime)
{
}

void Object::Draw(ID2D1HwndRenderTarget* renderTarget)
{
}
