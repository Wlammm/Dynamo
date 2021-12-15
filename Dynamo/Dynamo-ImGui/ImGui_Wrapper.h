#pragma once

#include <string>
#include "imgui_notify.h"

enum class NotificationType
{
	None,
	Info,
	Success,
	Warning,
	Error
};

namespace ImGui
{
	template<typename ...Args>
	void Notification(NotificationType aType, const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToastType type = ImGuiToastType_None;
		switch (NotificationType)
		{
		case NotificationType::None:
				type = ImGuiToastType_None;
				break;

		case NotificationType::Info:
			type = ImGuiToastType_Info;
			break;

		case NotificationType::Success:
			type = ImGuiToastType_Success;
			break;

		case NotificationType::Warning:
			type = ImGuiToastType_Warning;
			break;

		case NotificationType::Error:
			type = ImGuiToastType_Error;
			break;
		}

		ImGuiToast toast(type, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);

		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void Notification(NotificationType aType, const std::string& aTitle, const int aDuration, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToastType type = ImGuiToastType_None;
		switch (NotificationType)
		{
		case NotificationType::None:
			type = ImGuiToastType_None;
			break;

		case NotificationType::Info:
			type = ImGuiToastType_Info;
			break;

		case NotificationType::Success:
			type = ImGuiToastType_Success;
			break;

		case NotificationType::Warning:
			type = ImGuiToastType_Warning;
			break;

		case NotificationType::Error:
			type = ImGuiToastType_Error;
			break;
		}

		ImGuiToast toast(type, aDuration);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);

		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyWarning(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType_Warning, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyError(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType_Error, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifySuccess(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType_Success, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}

	template<typename ...Args>
	void NotifyInfo(const std::string& aTitle, const char* aMessage, Args&& ...someArgs)
	{
		ImGuiToast toast(ImGuiToastType_Info, 5000);
		toast.set_title(aTitle.c_str());
		toast.set_content(aMessage, std::forward<Args>(someArgs)...);
		ImGui::InsertNotification(toast);
	}
}