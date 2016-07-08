/**
 * Autori: David Porizek, xporiz03
 * Modul: log
 * Popis modulu: Tento modul implementuje ukladani a nacitani her.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "log.h"

using namespace std;

/**
 * @brief Funkce zvysi pocet tahu
 */
void Logger::incTurnNumber()
{
	iTurnNumber++;
}

/**
 * @brief Funkce snizi pocet tahu
 */
void Logger::decTurnNumber()
{
	iTurnNumber--;
}

/**
 * @brief Funkce vrati pocet tahu
 */
int Logger::getTurnNumber()
{
	return iTurnNumber;
}

/**
 * @brief Funkce nastavi pocet tahu
 */
void Logger::setTurnNumber(int turn)
{
	iTurnNumber = turn;
}

/**
 * @brief Funkce ulozi hru
 * @param Hraci plocha kterou ma ulozit
 */
void Logger::saveGame(Map saveMap)
{
	vector<Player*> tempPlayers;
	PlayerList* tempPlayerList;
	vector<Card*> tempCards;
	vector<MapField*>tempFields;

	ofstream myfile;
	string filename;
	string turn;
#ifdef _WIN32
	filename = "..\\examples\\labyrint_fit8.txt";
#else
	filename = "../examples/labyrint_fit8.txt";
#endif
	myfile.open(filename);
	cout << "\n" << filename << "\n";
	myfile << ":TURN\n";
	myfile << iTurnNumber << ":\n";
	myfile << ":MAP\n";//SIZE:LASTX:LASTY:SHIFTEDSTATE:NUMBEROFCARDS
	myfile << saveMap.getSize();
	myfile << ":" << saveMap.getLastX() << ":" << saveMap.getLastY() << ":" << saveMap.getShiftedState() << ":" << saveMap.getCardsSet() <<"\n";
	myfile << ":FIELDS\n";//ID:SIZE:TREASURE:XPOS:YPOS:ROTATION:SHAPE
	//for each field
	tempFields = saveMap.getFields();
	for (unsigned int i = 0; i < tempFields.size(); i++)
	{
		myfile << tempFields[i]->getSize() << ":" << tempFields[i]->getTreasure() << ":" << tempFields[i]->getXPos() << ":" << tempFields[i]->getYPos() << ":" << tempFields[i]->getRotation() << ":" << tempFields[i]->getShape() << "\n";
	}
	myfile << ":PLAYERLIST\n"; //NUMBER:PTURN
	tempPlayerList = saveMap.getPlayers();
	tempPlayers = tempPlayerList->getPList();
	myfile << tempPlayerList->getNumber() << ":" << tempPlayerList->getPlayersTurn() << "\n";
	myfile << ":PLAYERS\n";//SCORE:X:Y:COLOR
	for (unsigned int i = 0; i < tempPlayers.size(); i++)
	{
		myfile << ":P:";
		myfile << tempPlayers[i]->getScore() << ":" << tempPlayers[i]->getXPos() << ":" << tempPlayers[i]->getYPos() << ":" << tempPlayers[i]->getColor();
		//CARDS
		//SIZE:
		//TREASURE
		tempCards = tempPlayers[i]->getpCards();
		myfile << ":" << tempCards.size();
		for (unsigned int j = 0; j < tempCards.size(); j++)
		{
			myfile << ":" << tempCards[j]->getTreasure();
		}
	}
	//UNDO STACK
	myfile << "\n";
	myfile << ":LOG\n";
	for (unsigned int i = 0; i < moves.size(); i++)
	{
		myfile << moves[i] << "\n";
	}
}

/**
 * @brief Funkce nacte ulozenou hru
 * @param Mapa do ktere ma hru nacist
 */
void Logger::loadGame(Map* sMap)
{
	stringstream ss;
	string filename;
	string turn;
	ss.clear();
#ifdef _WIN32
	filename = "..\\examples\\labyrint_fit8.txt";
#else
	filename = "../examples/labyrint_fit8.txt";
#endif
	ifstream myfile;
	myfile.open(filename);
	string line;
	string sTemp = "";
	MapField* tempField;
	vector<Card*> tempCards;
	Card* tempCard;
	int iTemp = 0;
	while (getline(myfile, line))
	{
		if (line.find(":TURN") != string::npos)
		{
			getline(myfile, line);
			sTemp = line.substr(0, line.find(":"));
			line = line.substr(line.find(":") + 1, string::npos);
			iTemp = stoi(sTemp);
			setTurnNumber(iTemp);
		}
		if (line.find(":MAP") != string::npos)
		{
			getline(myfile, line);
			//SIZE
			sTemp = line.substr(0, line.find(":"));
			line = line.substr(line.find(":") + 1, string::npos);
			iTemp = stoi(sTemp);
			sMap->setSize(iTemp);
			//LASTX
			sTemp = line.substr(0, line.find(":"));
			line = line.substr(line.find(":") + 1, string::npos);
			iTemp = stoi(sTemp);
			sMap->setLastX(iTemp);
			//LASTY
			sTemp = line.substr(0, line.find(":"));
			line = line.substr(line.find(":") + 1, string::npos);
			iTemp = stoi(sTemp);
			sMap->setLastY(iTemp);
			//WASSHIFTED
			sTemp = line.substr(0, line.find(":"));
			line = line.substr(line.find(":") + 1, string::npos);
			iTemp = stoi(sTemp);
			if (iTemp == 0)
				sMap->changeShiftedState(0);
			else
				sMap->changeShiftedState(1);
			sTemp = line.substr(0, line.find(":"));
			line = line.substr(line.find(":") + 1, string::npos);
			iTemp = stoi(sTemp);
			sMap->setCardsSet(iTemp);
			continue;
		}
		if (line.find(":FIELDS") != string::npos)
		{
			getline(myfile, line);
			sMap->clearMap();
			for (int i = 0; i <= (sMap->getSize() * sMap->getSize()); i++)
			{
				tempField = new MapField;
				int x, y;
				//SIZE
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				iTemp = stoi(sTemp);
				tempField->setSize(iTemp);
				//TREASURE
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				iTemp = stoi(sTemp);
				tempField->setTreasure(iTemp);
				//XPOS, YPOS
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				x = stoi(sTemp);
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				y = stoi(sTemp);
				tempField->setPosition(x, y);
				//ROTATION
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				iTemp = stoi(sTemp);
				tempField->setRotation(iTemp);
				//SHAPE
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				switch (*sTemp.c_str())
				{
				case 'I':
					tempField->setShape(1);
					break;
				case 'L':
					tempField->setShape(2);
					break;
				case 'T':
					tempField->setShape(3);
					break;
				}
				
				sMap->addField(tempField);
				
				getline(myfile, line);
			}
		}
		if (line.find(":PLAYERLIST") != string::npos)
		{
			getline(myfile, line);
			//PLAYERS NUMBER
			sTemp = line.substr(0, line.find(":"));
			line = line.substr(line.find(":") + 1, string::npos);
			iTemp = stoi(sTemp);
			sMap->setPlayers(iTemp);
			//PLAYERS TURN
			sTemp = line.substr(0, line.find(":"));
			line = line.substr(line.find(":") + 1, string::npos);
			iTemp = stoi(sTemp);
			sMap->setTurn(iTemp);
			continue;
		}
		if (line.find(":PLAYERS") != string::npos)
		{
			getline(myfile, line);
			line = line.substr(line.find(":") + 1, string::npos);
			(sMap->getPlayers())->clearPlayers();
			for (int i = 0; i < (sMap->getPlayers())->getNumber(); i++)
			{
				int x, y;
				int iCards;
				int iScore;
				int iColor;
				//tempPlayer = new Player;
				line = line.substr(line.find(":") + 1, string::npos);
				//SCORE
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				iScore = stoi(sTemp);
				//tempPlayer->SetScore(iTemp);
				//POSITION X, Y
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				x = stoi(sTemp);
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				y = stoi(sTemp);
				//tempPlayer->setPos(x, y);
				//COLOR
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				iColor = stoi(sTemp);
				//tempPlayer->setColor(iTemp);
				//NUMBER OF CARDS
				sTemp = line.substr(0, line.find(":"));
				line = line.substr(line.find(":") + 1, string::npos);
				iCards = stoi(sTemp);
				for (int j = 0; j < iCards; j++)
				{
					sTemp = line.substr(0, line.find(":"));
					line = line.substr(line.find(":") + 1, string::npos);
					iTemp = stoi(sTemp);
					tempCard = new Card(iTemp);
					tempCards.push_back(tempCard);
				}
				//tempPlayer->appendCards(tempCards);
				reverse(tempCards.begin(), tempCards.end());
				(sMap->getPlayers())->CreatePlayer(iColor, x, y, &tempCards, sMap->getCardsSet(), iScore);
				tempCards.clear();
			}
		}
		if (line.find(":LOG") != string::npos)
		{
			clearMoves();
			//getline(myfile, line);
			while (getline(myfile, line))
			{
				moves.push_back(line.substr(0, line.find("\n")));
			}
		}
	}
}

/**
 * @brief Funkce ulozi jeden tah do stacku tahu
 * @param Tah ktery se ma do stacku ulozit
 */
void Logger::saveTurn(string turn)
{
	moves.push_back(turn);
}

/**
 * @brief Funkce vycisti stack tahu
 */
void Logger::clearMoves()
{
	for (unsigned int i = 0; i < moves.size(); i++)
	{
		moves.pop_back();
	}
	moves.clear();
}

/**
 * @brief Funkce vrati jeden tah
 */
string Logger::getTurn()
{
	string temp;
	temp = moves.back();
	moves.pop_back();
	return temp;
}