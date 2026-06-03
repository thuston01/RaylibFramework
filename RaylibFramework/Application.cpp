#include "RaylibFramework/Application.h"

#include "RaylibFramework/Config.h"
#include "RaylibFramework/Resources/ResourceManager.h"

shared_ptr<Application> Application::m_instance;

void Application::Deleter::operator()(Application* app) const
{
	delete app;
}

shared_ptr<Application> Application::Instance()
{
	return m_instance;
}

void Application::Quit()
{
	// Force the close flag on the window to false
	m_instance->m_window->m_isOpen = false;
}

Application::Application(PrivateKey)
	: m_config{ std::make_shared<Config>("Engine") }, m_game{ nullptr }
{
	m_window = std::make_shared<Window>(Window::PrivateKey{}, m_config);
}

shared_ptr<Window> Application::GetWindow() const
{
	return m_window;
}

EExitCode Application::Run() const
{
	// Attempt to open the window, returning fail conde if it does not succeed
	if (!m_window->Open())
	{
		return EExitCode::WindowFailedToOpen;
	}

	ResourceManager::Load(m_config);

	// Initialise the game instance
	m_game->Init();

	//Continue to loop until the window requests a close
	while (m_window->IsOpen())
	{
		//Tick the game with the current frame time
		const float dt = GetFrameTime();
		m_game->Tick(dt);

		//Render the game
		m_window->BeginFrame();

		m_game->Render();

		m_window->EndFrame();
	}

	//Shutdown the game instance and close the window
	m_game->Shutdown();
	m_window->Close();

	// Return success as the whole gameplay loop ran successfully.
	return EExitCode::Success;
}
