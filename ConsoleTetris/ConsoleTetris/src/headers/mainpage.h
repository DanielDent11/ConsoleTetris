#pragma once

#include <interfaces/ipage.h>

#include <buffer.h>
#include <input.h>
#include <game.h>

#include <vector>

class MainPage : public IPage
{
public:
    MainPage() 
    {
    }

    MainPage(Buffer *buffer, Game *game)
    {
        m_links.push_back(std::make_pair("START", nullptr));
        m_links.push_back(std::make_pair("EXIT", nullptr));

        m_selectedLink = 0;
        m_buffer = buffer;
        m_game = game;
    }

    virtual void Draw() override
    {
        int startX = m_buffer->GetBoundsX().x;
        int endX = m_buffer->GetBoundsX().y;
        int startY = m_buffer->GetBoundsY().x;
        int endY = m_buffer->GetBoundsY().y;

        int width = endX - startX;
        int height = endY - startY;

        const int LogoOffsetX = -18;
        const int LogoOffsetY = 7;

        DrawLogo(startX + LogoOffsetX, startY + LogoOffsetY);

        const int offsetX = 7;
        const int offsetY = 15;

        for (int i = 0; i < m_links.size(); ++i)
        {
            if (i == m_selectedLink)
            {
                m_buffer->Set(L'>', startX + offsetX - 1, startY + offsetY + i);
            }
            for (int j = 0; j < m_links[i].first.length(); ++j)
            {
                m_buffer->Set((wchar_t)m_links[i].first[j], startX + offsetX + j, startY + offsetY + i);
            }
        }
    }

    virtual void Update() override
    {
        if (Input::GetKeyDown('S'))
        {
            Down();
        }
        else if (Input::GetKeyDown('W'))
        {
            Up();
        }
        else if (Input::GetKeyDown(ENTER))
        {
            if (m_links[m_selectedLink].first == "EXIT")
            {
                exit(0);
            } 
            else if (m_links[m_selectedLink].first == "START")
            {
                m_game->StartGame();
            }
        }
    }

    void SetBuffer(Buffer *value)
    {
        m_buffer = value;
    }
private:
    const int ENTER = 13;
    Buffer *m_buffer;
    Game *m_game;
    std::vector < std::pair<std::string, IPage *> > m_links;
    int m_selectedLink;

    void Down()
    {
        if (m_selectedLink + 1 >= m_links.size())
        {
            return;
        }
        ++m_selectedLink;
    }

    void Up()
    {
        if (m_selectedLink == 0)
        {
            return;
        }
        --m_selectedLink;
    }

    void DrawLogo(int startX, int startY)
    {
        const wchar_t *LOGO =
           L"##########.#######.##########.#######.##....####.#######."
            "....##.....##..........##.....##...##.##...##.##.##......"
            "....##.....######......##.....#######.##..##..##.##......"
            "....##.....##..........##.....##......##.##...##.##......"
            "....##.....#######.....##.....##......####....##.#######."
            ".........................................................";

        for (int y = 0; y < 6; ++y)
        {
            for (int x = 0; x < 57; ++x)
            {
                if (LOGO[y * 57 + x] == L'#')
                {
                    m_buffer->Set(L'#', startX + x, startY + y);
                }
            }
        }
    }
};
