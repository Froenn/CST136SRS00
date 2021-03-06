#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

#include "gsl.h"

#include "LetsMakeADeal.h"

void successProbability(const int doors, const Strategy strat)
{
	auto successProbability{ 0.0 };

	switch (strat)
	{
	case Strategy::kKeep:
	{
		successProbability = 1.0 / doors;

		std::cout << "\nThe Probability of success is: ";

		std::cout << std::fixed << std::showpoint << std::setprecision(1) << successProbability << std::endl;

		break;
	}
	case Strategy::kTrade:
	{
		successProbability = 1.0 - (1 / doors);

		std::cout << "\nThe Probability of success is: ";

		std::cout << std::fixed << std::showpoint << std::setprecision(1) << successProbability << std::endl;

		break;
	}
	case Strategy::kError:
	{
		std::cout << "\nThe Probability of success is: ";

		std::cout << std::fixed << std::showpoint << std::setprecision(1) << successProbability << std::endl;

		break;
	}
	}
}

void winRate(const int carsWon, const int goatsWon, const int numDeals)
{
	const auto carProb = (1.0 * carsWon) / numDeals;
	const auto goatProb = (1.0 * goatsWon) / numDeals;

	Ensures((carsWon + goatsWon) / numDeals == 1.0);

	std::cout << "\nThe Probability of choosing the car is: ";

	std::cout << std::fixed << std::showpoint << std::setprecision(1) << carProb << std::endl;
}

Strategy confirmStrat(int stratID)
{
	Strategy test{ Strategy::kError };

	switch (stratID)
	{
	case 0:
	{
		test = Strategy::kKeep;

		std::cout << "Using The Stay Strategy!\n";

		break;
	}
	case 1:
	{
		test = Strategy::kTrade;

		std::cout << "Using The Switch Strategy!\n";

		break;
	}
	default:
		test = Strategy::kError;

		std::cout << "Using The Error Strategy!\n";

		std::cout << "\nSuch a Silly Waffle You are. ^~^\n";

		break;
	}

	return test;
}

void simulateDeals(int argc, char* argv[])
{
	auto doorCount{ 3 };
	auto doorsDisclosed{ 1 };
	auto numDeals{ 1 };
	auto stratID{ 0 };
	auto currStrat{ Strategy::kError };
	auto dealInputError{ false };

	if (argc > 0)
	{
		for (auto i = 1; i < argc; i++)
		{
			std::istringstream gameInput(argv[i]); //toss all arguments into an basic input string stream to convert to usable values(int)

			Expects(i > 0 && argv[i] != nullptr)
			{
				switch (i)
				{
				case 1:
				{
					gameInput >> doorCount;

					if (!(doorCount >= 3))
					{
						dealInputError = true;

						std::cout << "\nCommand Line Help Message: You have too few doors!\n";
					}

					break;
				}
				case 2:
				{
					gameInput >> doorsDisclosed;

					if (!(doorsDisclosed == doorCount - 2))
					{
						dealInputError = true;

						std::cout << "\nCommand Line Help Message: You are disclosing the incorrect number doors!\n";
					}

					break;
				}
				case 3:
				{
					gameInput >> numDeals;

					if (!(numDeals > 0))
					{
						dealInputError = true;

						std::cout << "\nCommand Line Help Message: You are not making any deals!\n";
					}

					break;
				}
				case 4:
				{
					gameInput >> stratID;
					currStrat = confirmStrat(stratID);

					if (currStrat == Strategy::kError)
					{
						dealInputError = true;

						std::cout << "\nCommand Line Help Message: You have selected an invalid Strategy!\n";
					}

					break;
				}
				default:
				{
					dealInputError = true;  //basically this should only be hit if argc >= 5

					std::cout << "\nCommand Line Help Message: You have too many arguments!\n";

					break;
				}
				}
			}
		}
	}
	else
	{
		dealInputError = true;

		std::cout << "\nCommand Line Help Message: You have not entered any arguments!\n";
	}
	if (dealInputError == true)
	{
		std::cout << "\nCommand Line Help\n"
			<< "If you are having trouble with entering proper command line arguments check these steps:\n"
			<< "1. Make sure there is a space between each argument.\n"
			<< "2. Make sure there are the proper number of arguments. There should be exactly 4.\n"
			<< "3. Make sure the Command Line parameters are in order.\n"
			<< "\nNumber of Doors -> Number of Doors to Reveal -> Number of Deals to Simulate -> Strategy Choice\n\n"
			<< "4. Make sure the are no parameter violations and the parameters are given the proper input:\n"
			<< "\t-Number of Doors >= 3.\n"
			<< "\t-Number of Doors to Reveal == (Number of Doors - 2).\n"
			<< "\t-Number of Deals to Simulate > 0.\n"
			<< "\t-Strategy Choice: 0 to Stay. 1 to Switch.\n";
	}
	else
	{
		Expects(dealInputError == false)
		{
			std::vector<const LetsMakeADeal*> dealSim;

			auto carsWon{ 0 }, goatsWon{ 0 }; //to compare the probability later

			for (auto i = 0; i < numDeals; i++)
			{
				dealSim.push_back(new LetsMakeADeal{ doorCount, doorsDisclosed, currStrat }); //adds numDeals LetsMakeADeal(s) into the dealSim vector
			}
			for (auto deal : dealSim)
			{
				Expects(deal != nullptr)
				{
					if (deal->simulateDeal())
					{
						carsWon++;
					}
					else
					{
						goatsWon++;
					}

					delete deal; //cleans up each new deal after getting the essential info
				}
			}
			successProbability(doorCount, currStrat); //display deal probability of success

			winRate(carsWon, goatsWon, numDeals);  //display car win rate 
		}
	}

}

int main(int argc, char* argv[])
{
	simulateDeals(argc, argv);

	return 0;
}
