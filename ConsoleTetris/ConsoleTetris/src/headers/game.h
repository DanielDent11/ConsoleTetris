#pragma once

#include <database.h>

#include <interfaces/ibuilder.h>
#include <interfaces/idrawable.h>
#include <interfaces/ielement.h>
#include <interfaces/ipage.h>

#include <unordered_map>
#include <memory>

class MainPage;
class RegisterPage;
class LoginPage;
class Buffer;
class Game
{
public:
	Game();
	Game(IBuilder *puzzleBuilder, Buffer *buffer, DataBase *dataBase);
	~Game();

	void Run();
	void Spawn();
	void GameOver();

	void AddElementToScene(IElement *element);
	void RemoveElementFromScene(IElement *element);

	void AddPuzzle(IElement *element);
	void RemovePuzzle(IElement *element);

	void FillCells(IElement *element);

	bool IsGameOver() const;
	void SetGameOver(bool value);

	void SetCurrentPage(IPage *value);
	void StartGame();
	IPage *ToIPage(MainPage *mainPage);
	IPage *ToIPage(LoginPage *loginPage);
	IPage *ToIPage(RegisterPage *registerPage);
private:
	const int ESC = 27;

	const int ScoresForRow = 5;

	Pair<std::string, int> m_playerStats;
	Pair<std::string, std::string> m_loginInfo;
	bool m_isInGame;
	bool m_isInLogin;
	bool m_isInRegister;

	IPage *m_currentPage;

	MainPage *m_mainPage;
	RegisterPage *m_registerPage;
	LoginPage *m_loginPage;

	bool m_gameOver;
	IBuilder *m_puzzleBuilder;

	DataBase *m_dataBase;
	Buffer *m_buffer;
	std::vector<IElement *> m_puzzles;
	std::unordered_map<int, IElement *> m_updateables;
	std::unordered_map<int, IDrawable *> m_drawables;

	std::vector<int> m_elementsToBeRemovedIds;

	void RemoveElements();
	bool IsRowFilled(int row) const;
	void DeleteFilledRows();
	void DrawPlayerInfo();

	bool Register();
	bool Login();
	help_types::ValidateResult ValidateEnteredData(std::string username, std::string email, std::string password);
	help_types::ValidateResult ValidateExistance(std::string username, std::string email);
	void GetDataFromUserRegister(std::string &username, std::string &email, std::string &password);
	void GetDataFromUserLogin(std::string &username, std::string &email, std::string &password);

	friend class RegisterPage;
	friend class MainPage;
	friend class LoginPage;
};