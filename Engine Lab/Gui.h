#pragma once


enum button_states {
	BTN_IDLE = 0,
	BTN_HOVER,
	BTN_ACTIVE
};


namespace gui {

	// Function

	const float percentToPixelX(const float percent, const sf::VideoMode& videoMode);
	const float percentToPixelY(const float percent, const sf::VideoMode& videoMode);
	const unsigned calcCharSize(const unsigned modifier, const sf::VideoMode& videoMode);

	// BUTTON =======================================================================


    // BUTTON DERIVED

	class Button {
	protected:
		unsigned short buttonState;
		unsigned short id;

		float buttonPositionX;
		float buttonPositionY;
		float buttonWidth;
		float buttonHeight;

	public:
		// Constructors/Destructors
		Button(float x, float y, float width, float height,
			unsigned short id = 0
		);
		~Button();

		// Accessors
		const bool isPressed() const;
		const unsigned short& getId() const;

		// Modifiers
		void setId(const unsigned short id);

	};


	// BUTTON WITH BACKGROUND COLOR AND TEXT

	class ButtonColor : public Button
	{
	private:
		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;

	public:
		// Constructors/Destructors
		ButtonColor(float x, float y, float width, float height,
			sf::Font* font, std::string text, unsigned character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::Color outline_idle_color = sf::Color::Transparent, sf::Color outline_hover_color = sf::Color::Transparent, sf::Color outline_active_color = sf::Color::Transparent,
			unsigned short id = 0
		);
		~ButtonColor();

		// Accessors
		const std::string getText() const;

		// Modifiers
		void setText(const std::string text);
		
		//Functions
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};


	// BUTTON WITH IMAGE BACKGROUND

	class ButtonImage : public Button {
	private:
		sf::Sprite sprite;
		sf::Texture* texture;

	public:
		// Constructors/Destructors
		ButtonImage(float x, float y, float width, float height,
			sf::Texture* texture, 
			unsigned short id = 0
		);
		~ButtonImage();

		// Functions
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};

	// DROP DOWN LIST ===============================================================

	class DropDownList {
	private:
		// Variable
		float keyTime;
		float keyTimeMax;

		sf::Font& font;
		gui::ButtonColor* activeElement;
		std::vector<gui::ButtonColor*> list;
		bool showList;
		

	public:
		// Constructors/Destructors
		DropDownList(
			float x, float y, float width, float height,
			sf::Font& font, std::string list[],
			unsigned numberOfElements, unsigned default_index = 0
		);
		~DropDownList();

		// Accessors
		const bool getKeyTime();
		const unsigned short& getActiveElementId() const;

		// Functions
		void updateKeyTime(const float& dt);

		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};


	// TEXTURE SELECTOR ==============================================================

	class TextureSelector {
	private:
		// Variables
		bool active;
		bool hidden;

		float gridSize;
		
		sf::RectangleShape bounds;
		sf::Sprite spriteSheet;
		sf::RectangleShape selector;
		sf::Vector2u mousePosGrid;
		sf::IntRect textureSelectorRect;


	public:

		// Constructors/Destructors
		TextureSelector(float x, float y, float width, float height, float gridSize, const sf::Texture* texture_sheet);
		~TextureSelector();


		// Accessors
		const bool& getActive() const;
		const bool& getHidden() const;
		const sf::IntRect& getTextureSelectorRect() const;
		

		// Functions
		void toggleTextureSelector();
		
		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};
}