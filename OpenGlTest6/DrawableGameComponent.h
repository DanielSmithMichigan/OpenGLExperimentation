#ifndef DRAWABLE_COMPONENT_HEADER
#define DRAWABLE_COMPONENT_HEADER
#include "Component.h"
#include "GlobalGameObjects.h"

using namespace std;

class DrawableGameComponent : public Component {
	public:
		virtual void Draw(GlobalGameObjects* objects) = 0;
		virtual void Update() = 0;
		virtual void Initialize() = 0;
		DrawableGameComponent();
	private:
	protected:
};

DrawableGameComponent::DrawableGameComponent() {
}

#endif