#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

/**
* This struct defines a single card. It has a numerical value,
* a name (Ace, Jack, Seven etc...) and a suit (Hearts, Clubs, Diamond, Spades)
*/
struct Card
{
	int m_value = 0;
	char m_name[10] = { 0 };
	char m_suit[9] = { 0 };
};

/**
* This struct defines a deck of cards. It has an array of 52 cards, and a counter
* to keep track of how many cards are in the deck.
*/
struct Deck
{
	Card m_cards[52] = { 0 };
	int m_card_count = 51;
};

/**
* This struct defines a hand, it holds a small array of 10 cards, as
* 21 would be reached in far less cards, so I don't want an error from adding
* a card when there's no space.
*/
struct Hand
{
	Card m_cards[10] = { 0 };
	int m_card_count = 0;
};

/**
* This function generates a deck of 52 cards. The cards are not
* shuffled, that is handled by the shuffleDeck() function.
*/
void generateDeck(Deck* deck)
{
	Card* cards = deck->m_cards;

	// Array of cstrings of card suits.
	char suits[4][9] = {
		"Spades",
		"Clubs",
		"Hearts",
		"Diamonds"
	};

	// Array of cstrings of card names.
	char names[13][6] = {
		"Two",
		"Three",
		"Four",
		"Five",
		"Six",
		"Seven",
		"Eight",
		"Nine",
		"Ten",
		"Jack",
		"Queen",
		"King",
		"Ace"
	};

	// Keeps track of which card in the deck we are generating.
	int card = 0;

	// For every suit...
	for (int suit = 0; suit < 4; suit++)
	{
		// For every type of card...
		for (int name = 0; name < 13; name++)
		{
			// Copy the current suit and name into the current card.
			strcpy_s(cards[card].m_suit, suits[suit]);
			strcpy_s(cards[card].m_name, names[name]);

			// Compares the current name of the card to predefined names and
			// assigns a value based on the name. I could not use a switch statement
			// because I had to use strcmp to compare the cstrings.
			if (!strcmp(names[name], "Two")) { cards[card].m_value = 2; }
			else if (!strcmp(names[name], "Three")) { cards[card].m_value = 3; }
			else if (!strcmp(names[name], "Four")) { cards[card].m_value = 4; }
			else if (!strcmp(names[name], "Five")) { cards[card].m_value = 5; }
			else if (!strcmp(names[name], "Six")) { cards[card].m_value = 6; }
			else if (!strcmp(names[name], "Seven")) { cards[card].m_value = 7; }
			else if (!strcmp(names[name], "Eight")) { cards[card].m_value = 8; }
			else if (!strcmp(names[name], "Nine")) { cards[card].m_value = 9; }
			else if (!strcmp(names[name], "Ten") ||
							!strcmp(names[name], "Jack") ||
							!strcmp(names[name], "Queen") ||
							!strcmp(names[name], "King")) { cards[card].m_value = 10; }

			// Don't assign a value to the ace because that's handled when
			// adding a card to the hand.

			// Increment the card counter.
			card++;
		}
	}
}


/**
* This function resets and shuffles a deck so that it can be used
* again.
*/
void shuffleDeck(Deck* deck)
{
	// If the card count for the deck is not 51,
	// the deck has been used and needs to be reset.
	if (deck->m_card_count != 51)
	{
		deck->m_card_count = 51;
	}

	// For every card in the deck...
	for (int i = 0; i < 52; i++)
	{
		// Get a random position in the deck.
		int random_pos = rand() % 51;

		// Store a copy of the current card.
		Card temp = deck->m_cards[i];

		// Swap the current card with a random card.
		deck->m_cards[i] = deck->m_cards[random_pos];

		// Put the current card in the random card's position.
		deck->m_cards[random_pos] = temp;
	}
}

/**
* This function goes through each card in a hand and adds
* up the total amount of money
*/
int calculateHandValue(Hand* hand)
{
	int value = 0;
	for (int i = 0; i < hand->m_card_count; i++)
	{
		value += hand->m_cards[i].m_value;
	}
	return value;
}

/**
* This function simply adds a card from the deck to a hand.
*/
void addCardToHand(Deck* deck, Hand* hand)
{
	hand->m_cards[hand->m_card_count++] = deck->m_cards[deck->m_card_count--];

	// If the card added is an ace.
	if (!strcmp(hand->m_cards[hand->m_card_count - 1].m_name, "Ace"))
	{
		if (calculateHandValue(hand) <= 10)
		{
			hand->m_cards[hand->m_card_count - 1].m_value = 11;
		}
		else
		{
			hand->m_cards[hand->m_card_count - 1].m_value = 1;
		}
	}
}

/**
* This function goes through a hand and outputs every card in a formatted way
* along with the total value of the hand.
*/
void showHand(char* name, Hand* hand)
{
	std::cout << name << "'s hand:" << std::endl;
	for (int i = 0; i < hand->m_card_count; i++)
	{
		std::cout << " - " << hand->m_cards[i].m_name << " of " << hand->m_cards[i].m_suit << std::endl;
	}
	std::cout << "Total value: " << calculateHandValue(hand) << std::endl;
}

/**
* This function asks the user how much they would like to bet
* this round.
*/
unsigned int getBet(unsigned int& total_money)
{
	unsigned int bet = 0;
	bool valid_bet = false;
	do
	{
		std::cout << "Total money: " << total_money << std::endl;
		std::cout << "How much would you like to bet? ";
		std::cin >> bet;

		// Make sure the player doesn't bet 0 or lower.
		if (bet <= 0)
		{
			std::cout << "You cannot bet 0." << std::endl;
		}

		// Make sure the player doesn't bet more than they have.
		else if (bet > total_money)
		{
			std::cout
				<< "You don't have enough money to bet that amount." << std::endl
				<< "The maximum amount you can bet is " << total_money << "." << std::endl;
		}

		// If everything is fine, it's a valid bet.
		else
		{
			valid_bet = true;
		}
	} while (!valid_bet);

	total_money -= bet;
	std::cout << "Total money after bet: " << total_money << std::endl;

	return bet;
}
/**
* This function contains everything to do with the player's turn. This is where
* the gameplay for the player is ran.
*/
bool playerTurn(Deck* deck, Hand* player_hand, unsigned int& total_money, unsigned int& current_bet)
{
	char input = 0;
	bool player_bust_or_won = false;
	bool player_playing = true;
	int hand_value = 0;
	showHand("Player", player_hand);
	while (player_playing)
	{
		do
		{
			// Ask the player if they would like to hit or stick.
			std::cout << "Would you like to hit or stick? (H/S) ";
			std::cin >> input;

			// If they want to hit...
			if (input == 'H' || input == 'h')
			{
				// Add a card to their hand.
				addCardToHand(deck, player_hand);
				hand_value = calculateHandValue(player_hand);

				// If the total of their hand is over 21, they're bust, and they stop playing.
				if (hand_value > 21)
				{
					player_bust_or_won = true;
					player_playing = false;
					std::cout << "Player bust!" << std::endl;
				}

				// If they have 21, they're not bust, but they stop playing.
				else if (hand_value == 21)
				{
					std::cout << "Player has 21." << std::endl;
					player_bust_or_won = true;
					player_playing = false;
				}
			}
			else if (input == 'S' || input == 's')
			{
				std::cout << "Player sticks." << std::endl;
				player_playing = false;
			}
			else
			{
				std::cout << "Sorry, I did not understand that." << std::endl;
			}
		} while (input != 'H' && input != 'h' && input != 'S' && input != 's');

		showHand("Player", player_hand);

		// If the player hasn't stuck or lost..
		if (player_playing)
		{
			do
			{
				// Ask the player if they would like to raise.
				std::cout << "Would you like to raise? (Y/N) ";
				std::cin >> input;

				if (input == 'Y' || input == 'y')
				{
					// Get the raise if they would.
					int raise = getBet(total_money);
					current_bet += raise;
				}
				else if (input == 'N' || input == 'n') { /* it understands no, and does nothing. */ }
				else
				{
					std::cout << "Sorry, I didn't understand that." << std::endl;
				}
			} while (input != 'Y' && input != 'y' && input != 'N' && input != 'n');
		}
	}

	return player_bust_or_won;
}

/**
* This function contains the dealers turn. No input from the player is required here.
*/
bool dealerTurn(Deck* deck, Hand* dealer_hand)
{
	bool dealer_bust = false;
	bool dealer_playing = true;
	int hand_value = 0;
	while (dealer_playing)
	{
		showHand("Dealer", dealer_hand);
		hand_value = calculateHandValue(dealer_hand);

		// If the dealer has less than 18, the dealer hits.
		if (hand_value < 18)
		{
			std::cout << "Dealer hits." << std::endl;
			addCardToHand(deck, dealer_hand);
		}

		// If the dealer has 18 or more, and less than than 21, the dealer sticks.
		else if (hand_value >= 18 && hand_value < 21)
		{
			std::cout << "Dealer sticks." << std::endl;
			dealer_playing = false;
		}
		else if (hand_value == 21)
		{
			std::cout << "Dealer has 21." << std::endl;
			dealer_playing = false;
		}

		// The dealer has over 21, the dealer is bust.
		else
		{
			showHand("Dealer", dealer_hand);
			std::cout << "Dealer bust!" << std::endl;
			dealer_playing = false;
			dealer_bust = true;
		}
	}

	return dealer_bust;
}

/**
* This function asks the player if they would like to play again
* after a round has finished. If not, the game ends.
*/
bool playAgain(Deck* deck, Hand* player_hand, Hand* dealer_hand)
{
	char input = 0;
	bool playing = true;
	do
	{
		std::cout << "Would you like to play again? (Y/N) ";
		std::cin >> input;
		if (input == 'Y' || input == 'y')
		{
			// Reshuffle the deck.
			shuffleDeck(deck);

			// Reset the player's and dealer's hands.
			*player_hand = Hand();
			addCardToHand(deck, player_hand);
			addCardToHand(deck, player_hand);

			*dealer_hand = Hand();
			addCardToHand(deck, dealer_hand);
			addCardToHand(deck, dealer_hand);
		}
		else if (input == 'N' || input == 'n')
		{
			playing = false;
		}
		else
		{
			std::cout << "Sorry, I did not understand that." << std::endl;
		}
	} while (input != 'Y' && input != 'y' && input != 'N' && input != 'n');
	return playing;
}

int main()
{
	// Seed rand
	srand(unsigned int(time(NULL)));

	unsigned int total_money = 10000;
	unsigned int bet = 0;

	// Generate a deck and shuffle it.
	Deck deck;
	generateDeck(&deck);
	shuffleDeck(&deck);

	// Create a hand for the player and a hand for the dealer.
	// Add two cards to each.
	Hand player_hand;
	addCardToHand(&deck, &player_hand);
	addCardToHand(&deck, &player_hand);

	Hand dealer_hand;
	addCardToHand(&deck, &dealer_hand);
	addCardToHand(&deck, &dealer_hand);

	std::cout
		<< "============================================" << std::endl
		<< "=========== Welcome to Blackjack ===========" << std::endl
		<< "============================================" << std::endl;

	bool playing = true;
	while (playing)
	{
		// Get the bet before hands are revealed.
		bet = getBet(total_money);

		// Let the player play.
		bool player_bust_or_won = playerTurn(&deck, &player_hand, total_money, bet);

		// Don't do the dealer's turn if the player is bust or has 21.
		if (!player_bust_or_won)
		{
			// Carry out the dealer's turn.
			bool dealer_bust = dealerTurn(&deck, &dealer_hand);

			// Get the total's of their hands.
			int player_total = calculateHandValue(&player_hand);
			int dealer_total = calculateHandValue(&dealer_hand);

			// If the dealer is bust or the player has more than the dealer, the player wins.
			if (dealer_bust || player_total > dealer_total)
			{
				std::cout << "Player wins!" << std::endl;
				std::cout << "Player wins " << bet * 2 << "!" << std::endl;
				total_money += bet * 2;
				std::cout << "Total money after win: " << total_money << std::endl;
			}

			// If the hands are equal, nobody wins and the player gets their bet back.
			else if (player_total == dealer_total)
			{
				std::cout << "Both players tied." << std::endl;
				std::cout << "Player has bet of " << bet << " returned." << std::endl;
				total_money += bet;
				std::cout << "Total money after bet returned: " << total_money << std::endl;
			}

			// Otherwise the dealer wins.
			else
			{
				std::cout << "Dealer wins!" << std::endl;
			}
		}
		else
		{
			// The player is not bust but has won.
			if (calculateHandValue(&player_hand) == 21)
			{
				std::cout << "Player wins " << bet * 2 << "!" << std::endl;
				total_money += bet * 2;
				std::cout << "Total money after win: " << total_money << std::endl;
			}
		}

		// Ask the player if they want to play again.
		playing = playAgain(&deck, &player_hand, &dealer_hand);

		// Clear console if playing again.
		if (playing)
		{
			system("cls");
		}
	}

	return 0;
}