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

	name.assign("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string();

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	//if(atlas == nullptr)
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}


j1UIElement* j1Gui::GetElementUnderMouse()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	for (p2List_item<j1UIElement*>* item = elements.start; item != NULL; item = item->next)
	{
		if (item->data->IsInside(x, y) && item->data->interactable)
		{
			bool inside_child = false;
			for (p2List_item<j1UIElement*>* child_item = elements.start; child_item != NULL; child_item = child_item->next)
			{
				if (child_item->data->parent && child_item->data->parent == item->data && child_item->data->IsInside(x, y) && child_item->data->interactable)
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
	//scale
	if (scaling_element != nullptr)
	{
		Uint32 now = scale_timer.Read();

		if (now < scale_time)
			DoScale(scaling_element, scale_increment_x, scale_increment_y);
		else if (now >= scale_time)
			scaling_element = nullptr;
	}
	//

	j1UIElement* selected_element = GetElementUnderMouse();

	for (p2List_item<j1UIElement*>* item = elements.start; item != NULL; item = item->next)
	{
		j1UIElement* current_element = item->data;

		if (item->data->GetEnabled() == false)
			continue;

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
					if (current_element->dragable)
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
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug_draw = !debug_draw;

		for (p2List_item<j1UIElement*>* item = elements.start; item != NULL; item = item->next)
		{
			item->data->DadEnabled();
			if (item->data->enabled)
			item->data->UIBlit();
			if (debug_draw && item->data->GetEnabled())
				App->render->DrawQuad(item->data->GetScreenRect(), 255, 0, 0, 255, false, false);
		}

	
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

j1UIImage* j1Gui::CreateImage(iPoint pos, SDL_Rect rect, j1UIElement* parent,bool img)
{
	j1UIImage* image = new j1UIImage(pos, rect,img);
	image->parent = parent;
	elements.add(image);

	return image;
}

j1UILabel* j1Gui::CreateLabel(iPoint pos, p2SString path, int size, p2SString text, SDL_Color color, int max_width,j1UIElement* parent )
{
	_TTF_Font* font = App->fonts->Load(path.GetString(), size);
	j1UILabel* label = new j1UILabel(pos, font, text, color, max_width);
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

j1UIScrollBar* j1Gui::CreateScrollBar(iPoint pos, j1UIElement* parent)
{
	j1UIScrollBar* scroll = new j1UIScrollBar(pos);
	scroll->parent = parent;
	elements.add(scroll);

	return scroll;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void j1Gui::SetAtlas(SDL_Texture* tex)
{
	atlas = tex;
}

void j1Gui::ScaleElement(j1UIElement* element, float scaleX, float scaleY, float time)
{
	if (time != 0.0F)
	{
		scale_timer.Start();
		scaling_element = element;
		scale_time = (uint32)(time * 0.5F * 1000.0F);
		scale_increment_x = (scaleX / time) / App->frame_rate;
		scale_increment_y = (scaleY / time) / App->frame_rate;
	}
	else
	{
		DoScale(element, scaleX, scaleY);
	}
}

void j1Gui::EnableElement(j1UIElement* element)
{
	element->SetEnabled(true);
	for (p2List_item<j1UIElement*>* item = elements.start; item != NULL; item = item->next)
	{
		if (item->data->parent && item->data->parent == element)
			EnableElement(item->data);
	}
}

void j1Gui::DisableElement(j1UIElement* element)
{
	{
		element->SetEnabled(false);
		for (p2List_item<j1UIElement*>* item = elements.start; item != NULL; item = item->next)
		{
			if (item->data->parent && item->data->parent == element)
				DisableElement(item->data);
		}
	}
}




void j1Gui::DoScale(j1UIElement* element, float scaleX, float scaleY)
{
	float scale_x, scale_y;
	element->GetScale(scale_x, scale_y);
	scale_x += scaleX;
	scale_y += scaleY;
	element->SetScale(scale_x, scale_y);

	for (p2List_item<j1UIElement*>* child_item = elements.start; child_item != NULL; child_item = child_item->next)
	{
		if (child_item->data->parent && child_item->data->parent == element)
		{
			DoScale(child_item->data, scaleX, scaleY);
		}
	}
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

void j1UIElement::GetScale(float& scaleX, float& scaleY)
{
	scaleX = scale_X;
	scaleY = scale_Y;
}

void j1UIElement::SetScale(float scaleX, float scaleY)
{
	scale_X = scaleX;
	scale_Y = scaleY;
}

void j1UIElement::SetGlobalScale(float scale)
{
	global_scale = scale;
}

void j1UIElement::DadEnabled()
{
	if (parent != nullptr)
	{
		if (!parent->enabled)
			this->enabled = false;
		else
			this->enabled = true;
	}
}

void j1UIElement::SetEnabled(bool enabled)
{
	this->enabled = enabled;
}

bool j1UIElement::GetEnabled() const
{
	return this->enabled;
}

j1UIImage::j1UIImage(iPoint pos, SDL_Rect rect,bool image)
{
	rect_box = { pos.x,pos.y,rect.w,rect.h };
	this->rect_sprite = rect;
	this->image = image;
}

j1UIImage::~j1UIImage()
{
}

bool j1UIImage::UIBlit()
{
	
	if (image)
	{
		iPoint screen_pos = GetScreenPos();
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0, false, false, global_scale, INT_MAX, INT_MAX, scale_X, scale_Y);
	}
	
	return true;
}

j1UILabel::j1UILabel(iPoint pos, _TTF_Font* font, p2SString text, SDL_Color color, int max_width)
{
	int w, h;
	App->fonts->CalcSize("a", w, h, font);
	
	if(max_width > 0)
	{
		//calculate the amount of lines that string has relative to the max width
		int lines = 1;
		int char_in_line = 0;
		int max_chars_in_line = max_width / w;
		for (int i = 0; i < text.Length(); i++)
		{
			if (text.GetString()[i] == '\n')
			{
				lines++;
				char_in_line = 0;
			}
			else if (char_in_line > max_chars_in_line)
			{
				lines++;
				char_in_line = 0;
			}
			char_in_line++;
		}

		rect_box.w = max_width;
		rect_box.h = h * lines;
	}
	else
	{
	//calc amount of lines independent of a max width
	int lines = 1;
	int max_chars_in_line = 0;
	int chars_in_line = 0;
	for (int i = 0; i < text.Length(); i++)
	{
		if (text.GetString()[i] == '\n')
		{
			lines++;
			max_chars_in_line = chars_in_line > max_chars_in_line ? chars_in_line : max_chars_in_line;
			chars_in_line = 0;
		}
		chars_in_line++;
	}
	max_chars_in_line = chars_in_line > max_chars_in_line ? chars_in_line : max_chars_in_line;

	//adjust size of box to the chars and lines
	rect_box.w = w * max_chars_in_line;
	rect_box.h = h * lines;
	}
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
	SDL_Texture* texture = App->fonts->Print(text.GetString(), color, font, rect_box.w);
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

	interactable = true;
	rect_box = { position.x, position.y, 254,160 };
	anim = new SDL_Rect[3];
	anim[0] = { 371,323,254,160 };
	anim[1] = { 368,161,254,160 };
	anim[2] = { 371,0,254,160 };
	rect_sprite = anim[0];
	
}

j1UIScrollBar::j1UIScrollBar(iPoint pos)
{
	rect_box = { pos.x, pos.y, 365, 18 };
	thumb = App->gui->CreateImage({ 0,0 }, { 1,527,365,18 }, this);
	thumb->SetScale(0.2F, 0.5F);
	thumb->dragable = true;
	thumb->interactable = true;

}

bool j1UIScrollBar::UIBlit()
{
	App->render->DrawQuad(GetScreenRect(), 0, 0, 0, 150, true, false);
	return true;
	
}
