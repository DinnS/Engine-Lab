#pragma once


enum button_states {
	BTN_IDLE = 0,
	BTN_HOVER,
	BTN_ACTIVE
};


namespace gui {

	// BUTTON =======================================================================

	class Button
	{
	private:
		unsigned short buttonState;
		unsigned short id;

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
		Button(float x, float y, float width, float height,
			sf::Font* font, std::string text, unsigned character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::Color outline_idle_color = sf::Color::Transparent, sf::Color outline_hover_color = sf::Color::Transparent, sf::Color outline_active_color = sf::Color::Transparent,
			unsigned short id = 0
		);
		~Button();

		// Accessors
		const bool isPressed() const;
		const std::string getText() const;
		const unsigned short& getId() const;

		// Modifiers
		void setText(const std::string text);
		void setId(const unsigned short id);

		//Functions
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
		gui::Button* activeElement;
		std::vector<gui::Button*> list;
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
		float keyTime;
		float keyTimeMax;
		bool active;
		float gridSize;
		bool hidden;
		gui::Button* hideButton;
		

		sf::RectangleShape bounds;
		sf::Sprite spriteSheet;
		sf::RectangleShape selector;
		sf::Vector2u mousePosGrid;
		sf::IntRect textureSelectorRect;


	public:

		// Constructors/Destructors
		TextureSelector(float x, float y, float width, float height, float gridSize, const sf::Texture* texture_sheet, sf::Font& font, std::string text);
		~TextureSelector();


		// Accessors
		const bool& getActive() const;
		const sf::IntRect& getTextureSelectorRect() const;
		const bool getKeyTime();

		// Functions
		void updateKeyTime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};
}