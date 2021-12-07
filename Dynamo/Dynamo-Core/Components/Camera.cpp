#include "pch.h"
#include "Camera.h"

namespace Dynamo
{
	void Camera::ExposeValues()
	{
		ImGui::Text("Field of View");
		ImGui::SameLine(0.0f, 10);
		if (ImGui::DragFloat("##camerafov", &myFoV, 1.0f, 0.0f, 360.0f))
		{
			Init(myFoV, Main::GetWindowResolution().Cast<float>(), myNearPlane, myFarPlane);
		}

		ImGui::Text("Near Plane");
		ImGui::SameLine(0.0f, 31);
		if (ImGui::DragFloat("##cameranearplane", &myNearPlane, 0.1f, 0.01f, myFarPlane))
		{
			Init(myFoV, Main::GetWindowResolution().Cast<float>(), myNearPlane, myFarPlane);
		}

		ImGui::Text("Far Plane");
		ImGui::SameLine(0.0f, 38);
		if (ImGui::DragFloat("##camerafarplane", &myFarPlane, 0.1f, myNearPlane, 1000000.0f))
		{
			Init(myFoV, Main::GetWindowResolution().Cast<float>(), myNearPlane, myFarPlane);
		}
	}

	nlohmann::json Camera::Save()
	{
		nlohmann::json json;

		json["nearPlane"] = myNearPlane;
		json["farPlane"] = myFarPlane;
		json["fov"] = myFoV;
		json["freemovement"] = myEnableFreeMovement;

		return json;
	}

	void Camera::Load(nlohmann::json& aJson)
	{
		myNearPlane = aJson["nearPlane"];
		myFarPlane = aJson["farPlane"];
		myFoV = aJson["fov"];
		myEnableFreeMovement = aJson["freemovement"];

		Init(myFoV, Main::GetWindowResolution().Cast<float>(), myNearPlane, myFarPlane);
	}

	void Camera::OnCreate()
	{
		myIsValid = true;
		myTransform = GetComponent<Transform>();

		Init(myFoV, Main::GetWindowResolution().Cast<float>(), myNearPlane, myFarPlane);
	}

	void Camera::OnDestroy()
	{
		myIsValid = false;
	}

	void Camera::Init(const float aFoV, const Vec2f& aResolution, const float aNearPlane, const float aFarPlane)
	{
		myFoV = aFoV;
		myNearPlane = aNearPlane;
		myFarPlane = aFarPlane;

		myResolution = aResolution;

		const float hFoVRad = aFoV * (PI / 180.0f);
		const float vFoVRad = 2.f * std::atan(std::tan(hFoVRad / 2.f) * (aResolution.y / aResolution.x));

		const float scaleX = 1.f / std::tan(hFoVRad * .5f);
		const float scaleY = 1.f / std::tan(vFoVRad * .5f);

		const float Q = myFarPlane / (myFarPlane - myNearPlane);

		myProjection(1, 1) = scaleX;
		myProjection(2, 2) = scaleY;
		myProjection(3, 3) = Q;
		myProjection(3, 4) = 1.f / Q;
		myProjection(4, 3) = -Q * myNearPlane;
		myProjection(4, 4) = 0.0f;
	}
	
	void Camera::Init(const Vec2f& aResolution, const float aNearPlane, const float aFarPlane)
	{
		myNearPlane = aNearPlane;
		myFarPlane = aFarPlane;

		myResolution = aResolution;

		myProjection(1, 1) = 2.f / aResolution.x;
		myProjection(2, 2) = 2.f / aResolution.y;
		myProjection(3, 3) = 1.f / (myFarPlane - myNearPlane);
		myProjection(4, 3) = myNearPlane / (myNearPlane - myFarPlane);
		myProjection(4, 4) = 1.f;
	}

	void Camera::Update()
	{
		if (myEnableFreeMovement)
		{
			CameraMovement();
			CameraRotation();
		}
	}

	void Camera::EditorUpdate()
	{
		Update();
	}

	void Camera::SetFoV(const float aFoV)
	{
		myFoV = aFoV;
		Init(aFoV, myResolution);
	}

	Transform* Camera::GetTransform() const
	{
		return myTransform;
	}

	const Mat4f& Camera::GetProjectionMatrix() const
	{
		return myProjection;
	}

	bool Camera::IsValid() const
	{
		return myIsValid;
	}

	void Camera::EnableFreeCamera()
	{
		myEnableFreeMovement = true;
	}

	void Camera::DisableFreeCamera()
	{
		myEnableFreeMovement = false;
	}

	void Camera::CameraMovement()
	{
		if (!Input::IsKeyPressed(MouseButton::Right))
			return;

		CU::Vector3f moveDir;

		if (Input::IsKeyDown(MouseButton::ScrollForward))
		{
			myShiftMultiplier += myScrollIncrease;
		}

		if (Input::IsKeyDown(MouseButton::ScrollBackwards))
		{
			myShiftMultiplier -= myScrollIncrease;
			if (myShiftMultiplier < 0)
				myShiftMultiplier = 0;
		}

		if (Input::IsKeyPressed(KeyCode::W))
		{
			moveDir += myTransform->GetForward();
		}

		if (Input::IsKeyPressed(KeyCode::S))
		{
			moveDir -= myTransform->GetForward();
		}

		if (Input::IsKeyPressed(KeyCode::D))
		{
			moveDir += myTransform->GetRight();
		}

		if (Input::IsKeyPressed(KeyCode::A))
		{
			moveDir -= myTransform->GetRight();
		}

		if (Input::IsKeyPressed(KeyCode::E))
		{
			moveDir += myTransform->GetUp();
		}

		if (Input::IsKeyPressed(KeyCode::Q))
		{
			moveDir -= myTransform->GetUp();
		}

		float multiplier = 1;
		if (Input::IsKeyPressed(KeyCode::LeftShift))
		{
			multiplier = myShiftMultiplier;
		}

		myTransform->Move(moveDir * myMovementSpeed * multiplier * Time::GetUnscaledDeltaTime());
	}

	void Camera::CameraRotation()
	{
		if (Input::IsKeyPressed(MouseButton::Right))
		{
			Vec2f mouseDelta = Input::GetMouseDeltaNormalized();

			static float yaw = -myTransform->GetRotationRad().y;
			static float pitch = -myTransform->GetRotationRad().x;

			yaw += mouseDelta.x * myMouseSensitivity;
			pitch += mouseDelta.y * myMouseSensitivity;
			pitch = CU::Clamp(-HALF_PI + 0.001f, HALF_PI - 0.001f, pitch);

			myTransform->SetRotationRad({ -pitch, -yaw, 0.0f });
		}
	}
}
