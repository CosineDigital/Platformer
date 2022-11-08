#include "editor.h"

Editor::Editor(Application* application) : mApplication(application), shouldDrawGrid(true), shouldDrawColliders(false), shouldDrawSelector(true),
shouldDrawSettings(true), shouldLimitFramerate(true), mSaveState(SaveState::NEW), mCurrentSelection(0), mCurrentSelectionType(0), mMouseLeftHeld(false),
mMouseRightHeld(false), mMouseMiddleHeld(false), mCameraPanSpeed(1.0), mActive(false), mLevel(nullptr)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(mApplication->mWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	auto& style = ImGui::GetStyle();
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.Alpha = 1.0f;
	this->imgui_io = &ImGui::GetIO();

	mCamera = new Camera();

	// Load the editor's config file

	// Load the editor's configuration
	std::ifstream file;
	file.open("resources/files/editor_config.json");
	nlohmann::json editorConfig;
	file >> editorConfig;


	mButtonSize = ImVec2{ editorConfig["button_size"]["w"], editorConfig["button_size"]["h"] };

	mTypeCount = editorConfig["type_count"];

	// Get the type names
	mTypeNames = new std::string [mTypeCount];
	mTypeCounts = new int[mTypeCount];
	for (auto it = editorConfig["type_names_counts"].begin(); it != editorConfig["type_names_counts"].end(); it++) {
		mTypeNames[it.value()["id"]] = it.value()["name"];
		mTypeCounts[it.value()["id"]] = it.value()["count"];
	}

	// Set up the selections
	mSelections = new Selection*[mTypeCount]; // a heap array of 3 selection pointers
	for (int idx = 0; idx < mTypeCount; idx++) {
		// 5, 11, 14, now each pointer points to another heap array of 3 pointers
		mSelections[idx] = new Selection[mTypeCounts[idx]];
	}

	// For each type
	for (int typeIdx = 0; typeIdx < mTypeCount; typeIdx++) {
		// For each selection in that type
		int jdx{ 0 };
		for (auto it = editorConfig[mTypeNames[typeIdx]].begin(); it != editorConfig[mTypeNames[typeIdx]].end(); it++) {
			Selection selection;
			selection.hint = it.value()["hint"];

			selection.icon = createSprite(&mApplication->mRenderer->mSpriteSheet,
				it.value()["x"],
				it.value()["y"],
				it.value()["w"],
				it.value()["h"]);

			mSelections[typeIdx][it.value()["id"]] = selection;
		}
	}
}

Editor::~Editor()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	delete mCamera;

	delete[] mTypeCounts;
	delete[] mTypeNames;

	for (int idx = 0; idx < mTypeCount; idx++) {
		delete[] mSelections[idx];
	}
	delete[] mSelections;
}

void Editor::draw() noexcept
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Draw every part of the editor's gui

	drawFileMenu();

	if (shouldDrawGrid) {
		mApplication->mLineRenderer->clear();
		// These are the vertical lines
		for (int i = 0; i < 25; i++) {
			mApplication->mLineRenderer->buffer(
				glm::vec2{ float(i + int(mCamera->mPosition.x - 12.0f)), mCamera->mPosition.y - 6.5f },
				glm::vec2{ float(i + int(mCamera->mPosition.x - 12.0f)), mCamera->mPosition.y + 6.5f }
			);
		}

		// These are the horizontal lines
		for (int i = 0; i < 14; i++) {
			mApplication->mLineRenderer->buffer(
				glm::vec2{ mCamera->mPosition.x - 12.0f, float(i + int(mCamera->mPosition.y - 6.5f)) },
				glm::vec2{ mCamera->mPosition.x + 12.0f, float(i + int(mCamera->mPosition.y - 6.5f)) }
			);
		}
		mApplication->mLineRenderer->render(mCamera);
	}

	if (shouldDrawColliders) {
		mApplication->mLineRenderer->clear();
		for (auto i = 0; i < mLevel->entityCount; i++) {
			mLevel->getEntity(i)->drawCollider(mApplication->mLineRenderer);
		}
		mApplication->mLineRenderer->render(mCamera);
	}

	if (shouldDrawSelector) {
		drawSelectionMenu();
	}

	if (mMouseLeftHeld) {

		glm::ivec2 pos = getCursorToWorldCoords();

		// If it's a valid position within the world
		if ((pos.x > -1 && pos.x < mLevel->width) && (pos.y > -1 && pos.y < mLevel->height))
		{
			// Update the save state of the level
			if (mSaveState == SaveState::NEW) {
				mSaveState = SaveState::NEW_UNSAVED;
				updateWindowTitle();
			}
			else if (mSaveState == SaveState::SAVED) {
				mSaveState = SaveState::UNSAVED;
				updateWindowTitle();
			}

			switch (mCurrentSelectionType) {
			case SelectionType::TILE:
				switch (mCurrentSelection) {
				case TileSelection::CLOUD:
					mLevel->addTile(Tile(true, mApplication->mRenderer->getSpriteID("cloud")), pos.x, pos.y);
					break;
				case TileSelection::FENCE:
					mLevel->addTile(Tile(false, mApplication->mRenderer->getSpriteID("fence")), pos.x, pos.y);
					break;
				case TileSelection::GROUND:
					mLevel->addTile(Tile(true, mApplication->mRenderer->getSpriteID("ground_1")), pos.x, pos.y);
					break;
				case TileSelection::GROUND_2:
					mLevel->addTile(Tile(true, mApplication->mRenderer->getSpriteID("ground_2")), pos.x, pos.y);
					break;
				case TileSelection::WATER:
					mLevel->addTile(Tile(false, mApplication->mRenderer->getSpriteID("water")), pos.x, pos.y);
					break;
				default:
					std::cerr << "Invalid Tile Selection!\n";
					break;
				}
			case SelectionType::TILE_ENTITY:
				switch (mCurrentSelection) {
				case TileEntitySelection::BLUE_PIPE:

					break;
				case TileEntitySelection::BRICK:

					break;
				case TileEntitySelection::BRIDGE:

					break;
				case TileEntitySelection::CLOUD_PLATFORM:

					break;
				case TileEntitySelection::COIN:

					break;
				case TileEntitySelection::GIRDER:

					break;
				case TileEntitySelection::GREEN_PIPE:

					break;
				case TileEntitySelection::MUSHROOM_PLATFORM_BROWN:

					break;
				case TileEntitySelection::MUSHROOM_PLATFORM_RED:

					break;
				case TileEntitySelection::QUESTION_BLOCK:

					break;
				default:
					std::cerr << "Invalid Tile Entity Selection!\n";
					break;
				}
			case SelectionType::ENTITY:
				switch (mCurrentSelection) {
				case EntitySelection::BLOOPER:
					break;
				default:
					std::cerr << "Invalid Entity Selection!\n";
					break;
				}
			default:
				std::cerr << "Invalid Selection Type!\n";
				break;
			}
		}
	}
	if (mMouseRightHeld) {

		glm::ivec2 pos = getCursorToWorldCoords();

		if ((pos.x > -1 && pos.x < mLevel->width) && (pos.y > -1 && pos.y < mLevel->height)) {

			// Update the save state of the level
			if (mSaveState == SaveState::NEW) {
				mSaveState = SaveState::NEW_UNSAVED;
				updateWindowTitle();
			}
			else if (mSaveState == SaveState::SAVED) {
				mSaveState = SaveState::UNSAVED;
				updateWindowTitle();
			}

			mLevel->addTile(Tile(false, mApplication->mRenderer->getSpriteID("none")), pos.x, pos.y);
		}
	}
	if (mMouseMiddleHeld) {
		float dx = (mApplication->mCursorX - mApplication->mCursorLastX) / static_cast<float>(mApplication->mWidth) * 25.f;
		float dy = (mApplication->mCursorY - mApplication->mCursorLastY) / static_cast<float>(mApplication->mHeight) * 14.f;

		mCamera->mPosition += glm::vec3{ -dx, dy, 0.0f };
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::onMouseEvent(GLFWwindow* window, int button, int action, int bits) noexcept {
	
	// If the editor is active
	if (mActive) {
		if (!imgui_io->WantCaptureMouse) {
			switch (button) {
			case GLFW_MOUSE_BUTTON_LEFT:
				if (action == GLFW_PRESS) {
					mMouseLeftHeld = true;
				}
				else if (action == GLFW_RELEASE) {
					mMouseLeftHeld = false;
				}
				break;

			case GLFW_MOUSE_BUTTON_RIGHT:
				if (action == GLFW_PRESS) {
					mMouseRightHeld = true;
				}
				else if (action == GLFW_RELEASE) {
					mMouseRightHeld = false;
				}
				break;

			case GLFW_MOUSE_BUTTON_MIDDLE:
				if (action == GLFW_PRESS) {
					mMouseMiddleHeld = true;
				}
				else if (action == GLFW_RELEASE) {
					mMouseMiddleHeld = false;
				}
				break;
			}
			
		}
	}
}

void Editor::onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int bits) noexcept {

	constexpr const float dampener = 10.0f;

	if (key == GLFW_KEY_W) {
		mCamera->mPosition.y += mCameraPanSpeed / dampener;
	}
	if (key == GLFW_KEY_A) {
		mCamera->mPosition.x -= mCameraPanSpeed / dampener;
	}
	if (key == GLFW_KEY_S) {
		mCamera->mPosition.y -= mCameraPanSpeed / dampener;
	}
	if (key == GLFW_KEY_D) {
		mCamera->mPosition.x += mCameraPanSpeed / dampener;
	}
}

void Editor::onScrollEvent(GLFWwindow* window, double xoffset, double yoffset) noexcept {

	mCameraPanSpeed += static_cast<float>(yoffset) / 60.0f;
}

void Editor::onCursorEvent(GLFWwindow*, double xpos, double ypos) noexcept
{
}

void Editor::setLevelForEditing(Level* level) noexcept {
	mLevel = level;
}

void Editor::activate() noexcept {
	mActive = true;
}

void Editor::deactivate() noexcept {
	mActive = false;
}

bool Editor::active() const noexcept
{
	return mActive;
}

glm::vec2 Editor::getCursorToWorldCoords() const noexcept
{	
	/**
	* @breif Helper lambda used by `getCursorToWorldCoords` - Converts the input to Normalized Device Coordinates (NDC)
	* @param min - The minimum or lowest value in the range
	* @param max - The maximum or highest value in the range
	* @param input - The value to be converted
	* @return A value from -1.0f to 1.0f
	*/
	static const auto toNormalizedDeviceCoordinates = [](const float& min, const float& max, const float& input) -> float {
		float delta = max - min;
		float pos = input / delta;
		return (pos * 2.0f) - 1.0f;
	};

	/** @note The Normalized deivce coordinates relative to the center of the screen */
	glm::vec2 relPos = { toNormalizedDeviceCoordinates(0.0f, (float)mApplication->mWidth,  (float)mApplication->mCursorX), 
		-toNormalizedDeviceCoordinates(0.0f, (float)mApplication->mHeight, (float)mApplication->mCursorY)};
	
	/** @note val = model * inverse(projection) */
	glm::vec4 val = glm::vec4(relPos.x, relPos.y, 0.0f, 1.0f) * mCamera->getInverseProjection();

	/** @note Need to add this to final value to get world coordinates*/
	glm::vec2 camPos = mCamera->getPosition2D();

	return { val.x + camPos.x, val.y + camPos.y };
}

void Editor::drawSelectionMenu() noexcept
{
	constexpr ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	ImGui::Begin("Selector", &shouldDrawSelector);

	ImGui::BeginTabBar("");

	for (int typeIdx = 0; typeIdx < mTypeCount; typeIdx++) {

		if (ImGui::BeginTabItem(mTypeNames[typeIdx].c_str()))
		{
			for (int idx = 0; idx < mTypeCounts[typeIdx]; idx++)
			{
				// Draw 5 buttons before starting a new row
				if (!(idx % 5 == 0)) ImGui::SameLine();

				ImGui::ImageButton((void*)1, this->mButtonSize,
					*(ImVec2*)&mSelections[typeIdx][idx].icon.topLeft,
					*(ImVec2*)&mSelections[typeIdx][idx].icon.bottomRight);

				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text(mSelections[typeIdx][idx].hint.c_str());
					ImGui::EndTooltip();
				}

				if (ImGui::IsItemClicked()) {
					mCurrentSelectionType = typeIdx;
					mCurrentSelection = idx;
				}
			}
			ImGui::EndTabItem();
		}
	}

	ImGui::EndTabBar();

	ImGui::End();
}

static const auto getOpenFileName = [](GLFWwindow* parentWindow)->OPENFILENAMEA
{
	// open a file menu and prompt user to create a file to save into
	OPENFILENAMEA ofn;
	CHAR szFile[255] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window(parentWindow);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.nFilterIndex = 1;
	ofn.lpstrFilter = "Level File (*.lvl)\0*.lvl\0";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	return ofn;
};

void Editor::drawFileMenu() noexcept
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				// Clear the current scene, and set to untitled
				mLevel->reset();
				this->mSaveState = SaveState::NEW;
				updateWindowTitle();
			}

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
			{
				// open a file menu and prompt user to create a file to save into
				auto ofn = getOpenFileName(mApplication->mWindow);
				if (GetOpenFileNameA(&ofn) == TRUE)
				{
					// deserialize the file and load in into the scene
					serializer::loadLevel(mLevel, lastPath = ofn.lpstrFile);

					this->mSaveState = SaveState::SAVED;
					updateWindowTitle();
				}
			}

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				if (this->mSaveState == SaveState::NEW || this->mSaveState == SaveState::NEW_UNSAVED)
				{
					auto ofn = getOpenFileName(mApplication->mWindow);
					if (GetSaveFileNameA(&ofn) == TRUE)
					{
						std::string path(ofn.lpstrFile); path += ".lvl";
						serializer::saveLevel(mLevel, lastPath = path);

						mSaveState = SaveState::SAVED;
						updateWindowTitle();
					}
				}
				// If the user has saved the scene before, but is unsaved
				else if (mSaveState == SaveState::UNSAVED)
				{
					// deserialize the file and load into the 
					serializer::saveLevel(mLevel, lastPath);

					mSaveState = SaveState::SAVED;
					updateWindowTitle();
				}
			}

			if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
			{
				auto ofn = getOpenFileName(mApplication->mWindow);
				if (GetSaveFileNameA(&ofn) == TRUE)
				{
					// serialize the scene to binary and save into the file
					std::string path(ofn.lpstrFile); path += ".lvl";
					serializer::saveLevel(mLevel, lastPath = path);

					mSaveState = SaveState::SAVED;
					updateWindowTitle();
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				glfwSetWindowShouldClose(mApplication->mWindow, true);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Options")) {

			ImGui::Checkbox("Draw Grid", &shouldDrawGrid);
			ImGui::Checkbox("Draw Colliders", &shouldDrawColliders);
			ImGui::Checkbox("Simulate", &mLevel->play);
			if (ImGui::Checkbox("Limit Framerate", &shouldLimitFramerate)) {
				if (shouldLimitFramerate) {
					glfwSwapInterval(1);
				}
				else {
					glfwSwapInterval(0);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::updateWindowTitle() noexcept {

	if (this->mSaveState == SaveState::NEW) {
		glfwSetWindowTitle(mApplication->mWindow, "Untitled - Platformer Level Editor");
		return;
	}
	else if (this->mSaveState == SaveState::NEW_UNSAVED) {
		glfwSetWindowTitle(mApplication->mWindow, "*Untitled - Platformer Level Editor");
		return;
	}

	size_t pos = lastPath.find_last_of('\\') + 1ull;
	if (this->mSaveState == SaveState::SAVED) {
		std::string title = lastPath.substr(pos) + " - Platformer Level Editor";
		glfwSetWindowTitle(mApplication->mWindow, title.c_str());
	} 
	else if (this->mSaveState == SaveState::UNSAVED) {
		std::string title = "*" + lastPath.substr(pos) + " - Platformer Level Editor";
		glfwSetWindowTitle(mApplication->mWindow, title.c_str());
	}
}

#if ENABLE_ADVANCED

void Editor::drawSceneHierarchy(Scene& activeScene, entt::entity& currentSelection) noexcept
{
	ImGui::Begin("Hierarchy", &mHierarchyOpen);

	constexpr ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	int i{ 0 };
	for (const entt::entity& e : activeScene.mEntities)
	{
		{
			ImGui::TreeNodeEx((void*)(intptr_t)i++, base_flags, "%s", activeScene.mRegistry.get<Tag>(e).mName);

			if (ImGui::IsItemClicked()) {
				currentSelection = e;
			}
		}
	}

	drawHierarchyPopup(activeScene, currentSelection);

	/** @note If the user clicks else where, nullify the currently selected entity */
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
		currentSelection = entt::null;
	}

	ImGui::End();
}

void Editor::drawObjectInspector(Scene& activeScene, entt::entity& currentSelection) noexcept
{
	ImGui::Begin("Inspector", &mInspectorOpen);

	if (currentSelection != entt::null)
	{
		drawInspectorPopup(activeScene, currentSelection);

		/** @Required Component - Tag */
		if (activeScene.mRegistry.has<Tag>(currentSelection))
		{
			if (ImGui::TreeNode("Tag"))
			{
				Tag& tag = activeScene.mRegistry.get<Tag>(currentSelection);

				ImGui::Text("Internal Id : %i", currentSelection);
				ImGui::InputText("Name", tag.mName, TAG_NAME_MAX, 0);

				ImGui::TreePop();
			}
		}

		/** @Required Component - Transform */
		if (activeScene.mRegistry.has<Transform>(currentSelection))
		{
			if (ImGui::TreeNode("Transform"))
			{
				Transform& transform = activeScene.mRegistry.get<Transform>(currentSelection);

				drawDragVec3(&transform.mPosition, "Position");
				drawDragVec3(&transform.mRotation, "Rotation");
				drawDragVec3(&transform.mScale, "Scale", 1.0f);

				ImGui::TreePop();
			}
		}

		const char* sprites[] = { "Green Mushroon", "Red Mushroom", "Fire flower", "Star", "Brick 1", "Question Block Off", "Brick 2", "Ground", "Unbreakable", "Question Block" };

		/** @Optional Component - SpriteIndex */
		if (activeScene.mRegistry.has<SpriteIndex>(currentSelection))
		{
			if (ImGui::TreeNode("Sprite"))
			{
				SpriteIndex& spriteIndex = activeScene.mRegistry.get<SpriteIndex>(currentSelection);

				ImGui::Combo("##Sprite", &spriteIndex.index, sprites, sizeof(sprites) / sizeof(const char*));

				ImGui::TreePop();
			}
		}

		/** @Optional Component - Hitbox */
		if (activeScene.mRegistry.has<Hitbox>(currentSelection))
		{
			if (ImGui::TreeNode("Hitbox"))
			{
				//Hitbox& hitbox = activeScene.mRegistry.get<Hitbox>(currentSelection);



				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}

void Editor::drawHierarchyPopup(Scene& activeScene, entt::entity& currentSelection) noexcept
{
	int shouldDelete{ false };
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Remove", "Backspace", false, currentSelection != entt::null))
		{
			shouldDelete = true;
		}

		if (ImGui::MenuItem("Duplicate", "Ctrl + D", false, currentSelection != entt::null))
		{
			activeScene.duplicate(currentSelection);
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Blank", ""))
		{
			const entt::entity& e = activeScene.mRegistry.create();
			activeScene.mRegistry.emplace<Tag>(e, "Blank");
			activeScene.mRegistry.emplace<Transform>(e);
		}

		ImGui::EndPopup();
	}

	if (shouldDelete) {
		activeScene.remove(currentSelection);
	}
}

template<class T, class... Args>
void Editor::tryAdd(Scene& activeScene, entt::entity& currentSelection, Args&&... args) noexcept
{
	if (!activeScene.mRegistry.has<T>(currentSelection))
	{
		activeScene.mRegistry.emplace<T>(currentSelection, args...);
	}
}

void Editor::drawInspectorPopup(Scene& activeScene, entt::entity& currentSelection) noexcept
{
	if (currentSelection != entt::null)
	{
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::BeginMenu("Add Component"))
			{
				if (ImGui::MenuItem("Sprite")) {
					tryAdd<SpriteIndex>(activeScene, currentSelection, 0);
				}
				if (ImGui::MenuItem("Hitbox")) {
					tryAdd<Hitbox>(activeScene, currentSelection);
				}

				ImGui::End();
			}
			if (ImGui::MenuItem("Delete Component")) {

			}

			ImGui::EndPopup();
		}
	}
}

void Editor::drawDragVec3(glm::vec3* vec3, const char* title, float resetValue) noexcept
{
	ImGui::PushID(title);

	ImGui::Columns(2);

	ImGui::SetColumnOffset(1, 120.0f);
	ImGui::Text(title);
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.25f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	if (ImGui::Button("X", buttonSize))
		vec3->x = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &vec3->x, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.5f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.6f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.5, 1.0f));
	if (ImGui::Button("Y", buttonSize))
		vec3->y = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &vec3->y, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
	if (ImGui::Button("Z", buttonSize))
		vec3->z = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &vec3->z, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::PopID();
	ImGui::Columns(1);
}

void Editor::drawDrag(float* f, const char* title, float resetValue) noexcept
{
	ImGui::PushID(title);

	ImGui::Columns(2);

	ImGui::SetColumnOffset(1, 120.0f);
	ImGui::Text(title);
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.25f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
	if (ImGui::Button("F", buttonSize))
		*f = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##F", f, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::PopID();
	ImGui::Columns(1);
}
#endif