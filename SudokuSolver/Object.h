#pragma once

#include <common.h>

#include <Vector2D.h>

class Engine;
struct ID2D1HwndRenderTarget;

class Object
{
public:
	Object(Engine* engine, const Vector2D position, const Vector2D dimensions);
	virtual ~Object();

	virtual void Tick(float deltaTime);

	virtual void Draw(ID2D1HwndRenderTarget* renderTarget);

private:
	Engine* engine_{ nullptr };

	bool bShouldTick{ true };

	Vector2D position_;
	Vector2D dimensions_;

public:
	__forceinline Engine* GetEngine() const { return engine_; }

	__forceinline bool ShouldTick() const { return bShouldTick; }
	__forceinline void SetShouldTick(const bool value) { bShouldTick = value; }

	__forceinline Vector2D GetPosition() const { return position_; }
	__forceinline void SetPosition(const Vector2D newPosition) { position_ = newPosition; }

	__forceinline Vector2D GetDimensions() const { return dimensions_; }
	__forceinline void SetDimensions(const Vector2D newDimensions) { dimensions_ = newDimensions; }
};
