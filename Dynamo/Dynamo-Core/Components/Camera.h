#pragma once

namespace Dynamo
{
	class Camera : public Component
	{
		COMP(Camera);
	public:
		Camera() = default;

		void ExposeValues() override;
		nlohmann::json Save() override;
		void Load(nlohmann::json& aJson) override;

		void OnCreate() override;
		void OnDestroy() override;

		void Init(const float aFoV, const Vec2f& aResolution, const float aNearPlane = 1.0f, const float aFarPlane = 10000.0f);
		void Init(const Vec2f& aResolution, const float aNearPlane = 1.0f, const float aFarPlane = 10000.0f);

		void Update() override;
		void EditorUpdate() override;

		void SetFoV(const float aFoV);

		Transform* GetTransform() const;

		const Mat4f& GetProjectionMatrix() const;

		bool IsValid() const;

		void EnableFreeCamera();
		void DisableFreeCamera();

	private:
		void CameraMovement();
		void CameraRotation();

	private:
		bool myIsValid = false;

		Transform* myTransform = nullptr;

		Mat4f myProjection;
		Vec2f myResolution;
		float myNearPlane = 0.01f;
		float myFarPlane = 10000.0f;
		float myFoV = 90.0f;

		bool myEnableFreeMovement = false;
		float myMovementSpeed = 100;
		float myShiftMultiplier = 10;
		float myMouseSensitivity = 4;
	};
}