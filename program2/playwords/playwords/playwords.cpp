// playwords.cpp : define o ponto de entrada para o aplicativo do console.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>

using namespace std;

//Declaração das funções
void extract_words(vector <string> &vec, ifstream &file);

int menu();

void check_word(const vector <string> &vec);

void guess_word(const vector <string> &vec);

string scrambleLetters(string word);

void search_words(const vector <string> &words);

void capitalLetters(string &str);

void wildcardGame(const vector <string> &words);

bool wildcardMatch(const char *str, const char *strWild);

void buildWord(const vector <string> &vec);

int threeSimpleRule(int &numMin, int &numLetters);
//Fim da declaração das funções

int main()
{

	srand(time(NULL)); //Inicialização da função random

	cout << "PLAYWORDS" << endl;
	cout << "=========" << endl;

	//Input do nome do dicionário
	cout << endl << "Dictionary file name ? ";
	string nameDictionary;
	cin >> nameDictionary;

	ifstream dictionary(nameDictionary);

	if (dictionary.fail())
	{
		cerr << "Input file opening failed.\n";
		exit(1); // Por vezes é preferível terminar o programa com um exit code != 0
	}

	vector <string> words; //Criação do vetor que conterá as palavras

	cout << "Extracting words..." << endl << endl;

	extract_words(words, dictionary); //Extrai as palavras para o vetor

	dictionary.close();

	char cont; //Permitirá sair do ciclo do...while que se segue
	int option;//Declaração da variável que conterá a opção do jogador

	do {
		option = menu(); //Invocada a função que mostra o menú e devolve a opção escolhida pelo utilizador

		switch (option) //Cada opção corresponde a um "jogo" diferente que está contido em funções diferentes
		{
		case 1:
			check_word(words); 
			break;
		case 2:
			guess_word(words);
			break;
		case 3:
			search_words(words);
			break;
		case 4:
			buildWord(words);
			break;
		case 5:
			wildcardGame(words);
			break;
		default: //Input inválido -> Apenas 1 a 5 pode ser escolhido
			cerr << "Error! The program will close..."<<endl;
			exit(1); //Caso dê erro, o programa fecha
		}
		do
		{
			cout << "Do you want to do anything else? (Y/N) " << endl; //Lê a opção que permite voltar a jogar ou não
			cin >> cont;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(100000, '\n');
			}
		} while (cin.fail());

		if (cont != 'N' && cont != 'n')
			system("CLS"); //Apaga o ecrã

	} while (cont != 'N' && cont != 'n'); //Caso a opção seja diferente de 'N' ou 'n' volta-se a jogar

	system("pause");

	return 0;
}

void capitalLetters(string &str) //Função que coloca todas as letras de uma string em maiúsculas
{
	for (size_t i = 0; i < str.length(); i++)
	{
		str.at(i) = toupper(str.at(i));
	}
}

void extract_words(vector < string> &vec, ifstream &file) //Função que extrai as palavras para o vetor
{
	string line; //Linha extraída do ficheiro

	while (!file.eof())
	{
		getline(file, line);

		if (line.length() != 0) //Pode acontecer que haja um linha vazia, por erro do notepad; caso isso aconteça, não é contada a linha; nos outros casos, a linha é retirada para o vetor, pois cada linha contém apenas 1 palavra
		{
			vec.push_back(line);
		}
	}
}

int menu()  //Menu do jogo
{
	bool valid_option = false;
	int option;
	do
	{
		cout << "What do you want to do?" << endl;
		cout << "		1- Check if a word belongs to the word list" << endl;
		cout << "		2- Guess a randomly word (You have at least 3 chances to guess the word)" << endl;
		cout << "		3- Show all the words present in the dictionary that can be built using the set of the given letters" << endl;
		cout << "		4- Build a valid word with a given set of letters, then verify if the word belongs to the word list or not." << endl;
		cout << "		5- Read a string containing one or more wildcard characters ('*' or '?') and show all the words in the dictionary that match the given string." << endl << endl;
		cout << "Choose only one option:  ";

		do
		{
			cin >> option;
			if (cin.fail())
			{
				cout << "Try again..." << endl;
				cin.clear();
				cin.ignore(100000, '\n');
			}
		} while (cin.fail());

		if (option >= 1 || option <= 5)
			valid_option = true;
		else
			cout << endl << "Invalid option. Try again." << endl << endl;

	} while (!valid_option);

	system("CLS"); //Clear Screen

	return option;
}

void check_word(const vector <string> &vec) //1ª Opção do Jogo
{
	size_t n = 0;

	string word;

	cout << "CHECK THE WORD!!!" << endl << endl;
	do
	{
		cout << "Word ? ";
		cin >> word;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(100000, '\n');
		}

	} while (cin.fail());

	capitalLetters(word); //Põe todas as letras da palavra escrita em maiúsculas

	for (n; n < vec.size(); n++)
	{
		if (word == vec.at(n))
		{
			cout << "Word found!!!" << endl;
			break;
		}
	}

	if (n == vec.size())
		cout << "The word doesn't exist in the dictionary" << endl;
}

void guess_word(const vector <string> &vec) //2ª Opção do Jogo
{
	cout << "GUESS THE WORD!!!" << endl << endl;
	cout << "The computer will select a word. ";

	int position = rand() % vec.size(); //Posição da palavra

	string word = vec[position]; //Palavra escolhida aleatoriamente

	int attempts = 3;  //O utilizador terá  3 tentativas para acertar a palavra

	cout << "You have " << attempts << " chances to guess the word." << endl << endl;

	cout << "Scrambling letters... " << endl;

	string letters = scrambleLetters(word);

	cout << "The word has this letters:   " << letters.at(0);
	for (size_t i = 1; i < letters.length(); i++)
	{
		cout << " - " << letters.at(i);
	}

	string userAttempt;
	cout << endl << endl << "Which word is it?" << endl;

	bool guess = 0; //Se acertar na palavra, fica a 1

	int invalidWords = 0; //Conta o nº de vezes que o utilizador introduz uma palavra inválida

	do
	{
		do
		{
			do
			{
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(string::npos, '\n');
					cout << endl << endl << "Which word is it?" << endl;
				}
				cin >> userAttempt;

			} while (cin.fail());

			if (userAttempt.length() != word.length())
			{
				invalidWords++;

				if (invalidWords == 3) //Caso o utilizador introduza uma palavra inválida 3 vezes, perderá o jogo
				{
					attempts = 0;
					goto lostGame; //Saltará para o fim desta função, onde é indicado que o jogador perdeu
				}

				cerr << "Invalid word. Your input has to have the same number of letters of the the given letters. Try again." << endl << endl;
			}


		} while (userAttempt.length() != word.length());

		capitalLetters(userAttempt);//Põe todas as letras da palavra escrita em maiúsculas

		if (userAttempt == word)
		{
			guess = 1;
			break;
		}
		else
			cout << "Wrong Word. Try again... " << endl << endl;

		attempts--;

	} while (attempts > 0);

lostGame: //Caso o utilizador tenha introduzido uma palavra inválida, a execução do programa saltará para aqui

	if (guess)
		cout << "Congratulations! You guess the word!" << endl;
	else
		cout << "Better luck next time! The word was " << word << endl;

}

string scrambleLetters(string word) //Função que baralha as letras de uma string dada
{
	int x = word.length();

	for (int y = x; y > 0; y--)
	{
		swap(word[rand() % x], word[y - 1]);
	}

	return word;
}

void search_words(const vector<string> &words)  //3ª Opção do Jogo
{
	cout << "SEARCH WORDS WITH THE GIVING LETTERS" << endl << endl;
	cout << "Letters: ";

	string given_letters;

	cin >> given_letters;

	vector <string> validWords; //Vetor que guarda todas as palavras válidas

	capitalLetters(given_letters);//Põe todas as letras da palavra escrita em maiúsculas

	cout << "Searching words...";

	bool validWord; //Retorna true se a palavra é verdadeira ou falso se não é

	for (size_t i = 0; i < words.size(); i++)
	{
		if (words.at(i).length() == given_letters.length())
		{
			validWord = true;

			for (size_t j = 0; j < given_letters.length(); j++) //Verifica se a palavra do dicionário usa todas as letras válidas
			{
				if (find(words.at(i).begin(), words.at(i).end(), given_letters.at(j)) == words.at(i).end())
					validWord = false;
			}

			for (size_t j = 0; j < words.at(i).length(); j++)
			{
				if (find(given_letters.begin(), given_letters.end(), words.at(i).at(j)) == given_letters.end())
					validWord = false;
			}

			if (validWord)
				validWords.push_back(words.at(i));
		}
	}

	if (validWords.size() != 0)
	{
		cout << " Words: ";
		for (size_t i = 0; i < validWords.size(); i++)
		{
			cout << validWords.at(i) << "  ";
		}
		cout << endl << endl;
	}
	else
		cout << " There are no words!!" << endl << endl;
}

void buildWord(const vector <string> &vec) //4ª opção do jogo
{
	cout << "BUILD A VALID WORD!!!" << endl << endl;
	cout << "Searching for letters...This could take a while..." << endl;

	int numberLetters[26] = { 0 }; //array que conterá o número de vezes que cada letra aparece no ficheiro 
	
	unsigned int wordSizeMax = 0; //Tamanho da palavra máxima

	int actualLetter; //Letra onde estará o apontador

	for (size_t i = 0; i < vec.size(); i++)
	{
		for (size_t j = 0; j < vec.at(i).length(); j++)
		{
			actualLetter = (int)vec.at(i).at(j) - 65;
			numberLetters[actualLetter]++;
		}
		if (vec.at(i).length() > wordSizeMax)
			wordSizeMax = vec.at(i).length();
	}

	string setLetters; //Array que conterá o nº de letras, com base na sua frequência

	int numMinLetter = numberLetters[0]; //Número de vezes que a letra mínima aparece
	
	for (int i = 1; i < 26; i++)
	{
		if (numMinLetter > numberLetters[i])
			numMinLetter = numberLetters[i];
	}

	int numTimes; //Número de vezes que cada letra deverá aparece
	for (int i = 0; i < 26; i++)
	{
		 numTimes = trunc(threeSimpleRule(numMinLetter, numberLetters[i]));
		 for (int j = 1; j <= numTimes; j++)
			 setLetters = setLetters + (char)(i + 65);
	}

	wordSizeMax = rand() % wordSizeMax + 1; //Variável será reutilizada para indicar o número de letras que aparecerão

	string showLetters = ""; //Variável onde estarão guardadas as letras que se poderão usar

	//Mostra as letras que se poderá usar no ecrã
	cout << "Letters: ";
	for (size_t i = 0; i < wordSizeMax; i++)
	{
		showLetters = showLetters + setLetters.at(rand() % setLetters.length());
		cout << showLetters.at(i) << " ";
	}

	cout << endl << "Word ? ";
	
	string userWord;
	cin >> userWord;

	capitalLetters(userWord);//Põe todas as letras da palavra escrita em maiúsculas

	//Verificar se a palavra pode ser formada com as letras que foram dadas;

	bool found = 0; // Caso a letra da palavra seja encontrada no conjunto de letras fica a 1

	for (int i = 0; i < userWord.length(); i++)
	{
		found = 0;
		for (int j = 0; j < showLetters.length(); j++)
		{
			if (userWord.at(i) == showLetters.at(j))
			{
				showLetters.at(j) = ' ';
				found = 1;
				break;
			}
		}
		if (!found)
		{
			cout << "The word is invalid." << endl;
			break;
		}
	}

	if (found)
	{
		found = 0; //Variável será reutilizada dentro do ciclo. Caso encontre a palavra no vetor, fica a 1

		//Verfica se a palavra se encontra no vetor
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (userWord == vec.at(i))
			{
				cout << "Word found!" << endl;
				found = 1;
				break;
			}
		}

		if (!found)
		{
			cout << "Word not found! Better luck next time..." << endl;
		}
	}
}

int threeSimpleRule(int &numMin, int &numLetters) //Função que realiza uma regra de 3 simples necessária à execução da opção 4: calcula a frequência que cada letra aparecerá no set de letras
{
	int result;

	result = 2 * numLetters / numMin; //2 -> o menor número tem que aparecer duas vezes

	return result;
}

void wildcardGame(const vector <string> &words) //5ª Opção do Jogo
{
	cout << "WILDCARD CHARACTERS GAME" << endl << endl;
	size_t n = 0; //Variável que indicará o índice do vetor
	string word; //Palavra que o utilizador introduzirá
	cout << "Word: ";
	cin >> word;

	bool flag = false; //Flag que se tornará true se o utilizador acertar
	while (n < words.size())
	{
		if (wildcardMatch(words.at(n).c_str(), word.c_str()))
		{
			cout << words.at(n) << "  ";
			flag = true;
		}
		n++;
	}

	if (!flag) //Caso o utilizador não acerte
		cout << endl << "There is no word..." << endl;
	else
		cout << endl;
}

bool wildcardMatch(const char *str, const char *strWild) //Função retirada do anexo C do pdf com as especificações do projeto (comentários provenientes do mesmo anexo)
{
	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '?')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*')
		{
			// Need to do some tricks.
			// 1. The wildcard * is ignored.
			// So just an empty string matches. This is done by recursion.
			// Because we eat one character from the match string,
			// the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again,
			// with a wildcard * match. This is done by recursion.
			// Because we eat one character from the string,
			// the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}
