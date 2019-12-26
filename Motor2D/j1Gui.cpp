#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	LOG("GUI module started");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}


j1UIElement* j1Gui::GetElementUnderMouse()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	for (p2List_item<j1UIElement*>* item = elements.start; item != NULL; item = item->next)
	{
		if (item->data->IsInside(x, y))
		{
			bool inside_child = false;
			for (p2List_item<j1UIElement*>* child_item = elements.start; child_item != NULL; child_item = child_item->next)
			{
				if (child_item->data->parent && child_item->data->parent == item->data && child_item->data->IsInside(x, y))
				{
					inside_child = true;
					break;
				}
			}
			if (!inside_child)
				return item->data;
		}
	}

	return nullptr;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	j1UIElement* selected_element = GetElementUnderMouse();

	for (p2List_item<j1UIElement*>* item = elements.start; item != NULL; item = item->next)
	{
		j1UIElement* current_element = item->data;
		if (selected_element && selected_element == current_element)
		{
			if (!current_element->hovered)
			{
				current_element->OnMouseHover();
				App->scene->GUIEvent(current_element, MOUSE_OVER);
				current_element->hovered = true;
			}
			else
			{
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
				{
					current_element->OnMouseClick();
					App->scene->GUIEvent(current_element, LEFT_CLICK_DOWN);

					//drag
					if (current_element->interactable)
					{
						iPoint pos = current_element->GetLocalPos();
						int x_movement, y_movement;
						App->input->GetMouseMotion(x_movement, y_movement);
						current_element->SetLocalPos(pos.x + x_movement, pos.y + y_movement);
					}
				}
				else
				{
					current_element->OnMouseRelease();
					App->scene->GUIEvent(current_element, LEFT_CLICK_UP);
				}
			}
		}
		else if (current_element->hovered)
		{
			current_element->OnMouseExit();
			App->scene->GUIEvent(current_element, MOUSE_EXIT);
			current_element->hovered = false;
		}
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (p2List_item<j1UIElement*>* item = elements.start; item != NULL; item = item->next)
	{
		item->data->UIBlit();
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

j1UIImage* j1Gui::CreateImage(iPoint pos, SDL_Rect rect, j1UIElement* parent)
{
	j1UIImage* image = new j1UIImage(pos, rect);
	image->parent = parent;
	elements.add(image);

	return image;
}

j1UILabel* j1Gui::CreateLabel(iPoint pos, p2SString path, int size, p2SString text, SDL_Color color, j1UIElement* parent)
{
	_TTF_Font* font = App->fonts->Load(path.GetString(), size);
	j1UILabel* label = new j1UILabel(pos, font, text, color);
	label->parent = parent;
	elements.add(label);


	return label;
}

j1UIButton* j1Gui::CreateButton(iPoint pos, j1UIElement* parent)
{
	j1UIButton* button = new j1UIButton(pos);
	button->parent = parent;
	elements.add(button);

	return button;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

j1UIElement::j1UIElement(j1UIElement* parent)
{
	this->parent = parent;
}

j1UIElement::~j1UIElement()
{
}

bool j1UIElement::UIBlit()
{
	return true;
}

bool j1UIElement::UICleanUp()
{
	return true;
}

bool j1UIElement::IsInside(int x, int y)
{
	SDL_Rect screen_rect = GetScreenRect();
	return((x < screen_rect.x + screen_rect.w) && (screen_rect.x < x) && (y < screen_rect.y + screen_rect.h) && (screen_rect.y < y));
}

SDL_Rect j1UIElement::GetScreenRect()
{
	if (parent)
		return { parent->GetScreenRect().x + rect_box.x, parent->GetScreenRect().y + rect_box.y, rect_box.w, rect_box.h };
	else
		return rect_box;
}

SDL_Rect j1UIElement::GetLocalRect()
{
	return rect_box;
}

iPoint j1UIElement::GetScreenPos()
{
	if (parent)
		return { parent->GetScreenRect().x + rect_box.x, parent->GetScreenRect().y + rect_box.y };
	else
		return { rect_box.x, rect_box.y };
}

iPoint j1UIElement::GetLocalPos()
{
	return { rect_box.x, rect_box.y };
}

void j1UIElement::SetLocalPos(int x, int y)
{
	rect_box.x = x;
	rect_box.y = y;
}

j1UIImage::j1UIImage(iPoint pos, SDL_Rect rect)
{
	rect_box = { pos.x,pos.y,rect.w,rect.h };
	this->rect_sprite = rect;
}

j1UIImage::~j1UIImage()
{
}

bool j1UIImage::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F);
	return true;
}

j1UILabel::j1UILabel(iPoint pos, _TTF_Font* font, p2SString text, SDL_Color color)
{
	App->fonts->CalcSize(text.GetString(), rect_box.w, rect_box.h, font);
	rect_box.x = pos.x;
	rect_box.y = pos.y;
	this->font = font;
	this->text = text;
	this->color = color;
}
j1UILabel::~j1UILabel()
{
}

bool j1UILabel::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	SDL_Texture* texture = App->fonts->Print(text.GetString(), color, font);
	App->render->Blit(texture, screen_pos.x, screen_pos.y, nullptr, 0.0F);
	SDL_DestroyTexture(texture);
	return true;
}

void j1UILabel::SetText(p2SString text)
{
	this->text = text;
}



j1UIButton::~j1UIButton()
{
}

bool j1UIButton::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F);
	return true;
}

void j1UIButton::OnMouseClick()
{
	rect_sprite = anim[2];
}

void j1UIButton::OnMouseHover()
{
	rect_sprite = anim[1];
}

void j1UIButton::OnMouseRelease()
{
	rect_sprite = anim[1];
}

void j1UIButton::OnMouseExit()
{
	rect_sprite = anim[0];
}

j1UIButton::j1UIButton(iPoint position)
{

	//This was just a test

	//rect_box = { position.x, position.y, 229,69 };
	//anim = new SDL_Rect[3];
	//anim[0] = { 0,113,229,69 };
	//anim[1] = { 411,169,229,69 };
	//anim[2] = { 642,169,229,69 };
	//rect_sprite = anim[0];
}