#include "loginpage.h"

void LoginPage::Update()
{
	if (Input::GetKeyDown(ESC))
	{
		IPage *p = dynamic_cast<IPage *>(m_game->m_mainPage);
		m_game->SetCurrentPage(p);
	}
}
