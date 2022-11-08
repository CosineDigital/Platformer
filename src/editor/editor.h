#pragma once 

#include <cstdlib>
#include <cstdio>
#include <algorithm>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../core/level/entity/entity.h"
#include "../core/level/entity/player.h"
#include "../core/level/level.h"
#include "../core/hitbox.h"
#include "../graphics/sprite.h"
#include "../core/hitbox.h"
#include "../core/serializer.h"
#include "selection.h"
#include "../core/json.h"

#include "../app/application.h"

class Application;

#include <glad/glad.h>
#include <glfw/glfw3.h>

// to avoid warning: defined in both glad.h and windows.h
#undef APIENTRY
#include <Windows.h>
#include <commdlg.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

// Some features are unused today
#define ENABLE_ADVANCED 0

enum class SaveState : char {
	NEW,
	NEW_UNSAVED,
	SAVED,
	UNSAVED
};

class Editor final {
public:

	Editor(Application*);

	~Editor();

	void draw() noexcept;

	void onMouseEvent(GLFWwindow*, int, int, int) noexcept;
	void onKeyboardEvent(GLFWwindow*, int, int, int, int) noexcept;
	void onScrollEvent(GLFWwindow*, double, double) noexcept;
	void onCursorEvent(GLFWwindow*, double, double) noexcept;

	void setLevelForEditing(Level*) noexcept;

	void activate() noexcept;
	void deactivate() noexcept;
	bool active() const noexcept;

	Camera* mCamera;

private:

	/**
	* @brief Calulates the world coordinates of the cursor
	* @return vec2 - The current block the cursor is hovering over (in world coordinates)
	* @param activeScene - The current scene being edited
	*/
	glm::vec2 getCursorToWorldCoords() const noexcept;

	/**
	* @brief Draws a selector for every possible sprite in the scene.
	* Sprites can be selected to be painted into the scene
	* @param activeScene - The current scene being edited
	*/
	void drawSelectionMenu() noexcept;

	/**
	* @brief Draws the file menu for the main menu bar, contains a binary file save & Load system, and an exit button
	*/
	void drawFileMenu() noexcept;

	void updateWindowTitle() noexcept;

private:

	Application* mApplication;
	Level* mLevel;

	bool mMouseLeftHeld;
	bool mMouseRightHeld;

	float mCameraPanSpeed;

	// Mouse, for panning...
	bool mMouseMiddleHeld;

private:

	// In the editor, we will have 3 tabs will a bunch of selections of each type
	// The types are as follows
	int mTypeCount;
	std::string* mTypeNames;
	int* mTypeCounts;

	Selection** mSelections; // array of pointers to arrays of selections

	int mCurrentSelectionType = 0;

	enum SelectionType : int {
		TILE,
		TILE_ENTITY,
		ENTITY
	};

	int mCurrentSelection = 0;

	enum TileSelection : int {
		GROUND,
		CLOUD,
		GROUND_2,
		WATER,
		FENCE
	};
	
	enum TileEntitySelection : int {
		BRIDGE,
		MUSHROOM_PLATFORM_RED,
		MUSHROOM_PLATFORM_BROWN,
		BRICK,
		COIN,
		QUESTION_BLOCK,
		GIRDER,
		CLOUD_PLATFORM,
		GREEN_PIPE,
		BLUE_PIPE
	};

	enum EntitySelection : int {
		ONE_UP,
		RED_MUSHROOM,
		GREEN_MUSHROOM,
		FIRE_FLOWER,
		STARMAN,
		GOOMBA,
		RED_CHEEP_CHEEP,
		GREEN_CHEEP_CHEEP,
		BUZZY_BEETLE,
		SPINY,
		BLOOPER,
		LAKITU,
		BULLET_BILL,
		PIRHANA_PLANT,
		BOWSER
	};

	ImVec2 mButtonSize;

	// Various flags
	bool shouldDrawGrid;
	bool shouldDrawColliders;
	bool shouldDrawSelector;
	bool shouldDrawSettings;
	bool shouldLimitFramerate;
	SaveState mSaveState;

	bool mActive;

	ImGuiIO* imgui_io;
	std::string lastPath;

#if ENABLE_ADVANCED

private:
	/**
	* @brief Draws a hierarchy panel to display all active enities within the activeScene
	* @param activeScene - The current scene being edited
	* @param currentSelection - The currently selected entity
	*/
	void drawSceneHierarchy(Scene& activeScene, entt::entity& currentSelection) noexcept;

	/**
	* @brief Draws the object inspector panel for the currently selected entity ; will draw nothing if the
	* currentSelection is nullified
	* @param activeScene - The current scene being edited
	* @param currentSelection - The currently selected entity to edit and inspect
	*/
	void drawObjectInspector(Scene& activeScene, entt::entity& currentSelection) noexcept;

	/**
	* @brief Draws a popup with several options for the hierarchy panel
	* @param activeScene - The current scene being edited
	* @param currentSelection - The currently selected entity
	*/
	void drawHierarchyPopup(Scene& activeScene, entt::entity& currentSelection) noexcept;

	/**
	* @brief Draws a popup with several options for the current entity in the inspector panel
	* @param activeScene - The current scene being edited
	* @param currentSelection - The currently selected entity
	*/
	void drawInspectorPopup(Scene& activeScene, entt::entity& currentSelection) noexcept;

	/**
	* @brief Adds the component to the entity if there is not already one
	* @tparam T - The type of component to add
	* @param activeScene - The current scene being edited
	* @param currentSelection - The currently selected entity
	* @param args - If there are any args for the constructor of component to pass in, do so here
	*/
	template<class T, class... Args>
	void tryAdd(Scene& activeScene, entt::entity& currentSelection, Args&&... args) noexcept;

	/**
	* @brief draws a vec3 editable
	* @param vec3 - The vec3 to be edited
	* @param title - The title of this editable
	* @param resetValue - The reset value for each component of the vec3
	*/
	void drawDragVec3(glm::vec3* vec3, const char* title, float resetValue = 0.0f) noexcept;
	
	/**
	* Draws a single dragfloat and a reset button
	*/
	void drawDrag(float* f, const char* title, float resetValue = 0.0f) noexcept;
#endif
};

