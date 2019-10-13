#ifndef _SDLWINDOW_H_
#define _SDLWINDOW_H_

#include "Module.h"
#include "SDL/include/SDL.h"

namespace Cronos {

	class Application;

	class SDLWindow : public Module
	{
	public:

		friend class ImGuiLayer;

		SDLWindow(Application* app, bool start_enabled = true);
		virtual ~SDLWindow();

		virtual bool OnInit() override;
		virtual bool OnStart() override;
		virtual bool OnCleanUp() override;

		inline void ReCalculateAspectRatio(uint width, uint height);
		void OnResize(uint width, uint height);

		virtual void SaveModuleData(json& JSONFile) override;
		virtual void LoadModuleData(json& JSONFile) override;

	public:
		//The window we'll be rendering to
		SDL_Window* window;

		//The surface contained by the window
		SDL_Surface* screen_surface;

		//Getters
		const inline uint GetWidth()			const { return m_Data.Width;		}
		const inline uint GetHeight()			const { return m_Data.Height;		}
		const inline uint GetScreenSize()		const { return m_Data.ScreenSize;	}
		const inline float GetAspectRatio()		const { return m_Data.AspectRatio;	}

		//Setters
		void SetTitle(const char* title);
		void SetWindowFullscreen(bool setStatus);
		void SetWindowResizable(bool setStatus);
		void SetWindowBorderless(bool setStatus);
		void SetWindowDesktopFullscreen(bool setStatus);
		void SetWindowBright(float brightValue);

	private:

		struct WindowData {

			//std::string Title;
			uint  Width, Height, ScreenSize;
			float AspectRatio = 1.0f;
			bool VSyncActive;

			bool WindowFullscreen;
			bool WindowResizable;
			bool WindowBorderless;
			bool WindowDesktopFullscreen;
			float WindowBright;
		};

		WindowData m_Data;
		int Mv = 4, mv = 3; //Open GL Major and Minor version
		Uint32 m_WindowFlags;

	};

}

#endif
