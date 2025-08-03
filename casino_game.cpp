#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

class Card {
private:
    int value;
    std::string suit;
    std::string rank;

public:
    Card(int v, const std::string& s, const std::string& r) : value(v), suit(s), rank(r) {}
    
    int getValue() const { return value; }
    std::string getSuit() const { return suit; }
    std::string getRank() const { return rank; }
    
    std::string toString() const {
        return rank + " of " + suit;
    }
};

class Deck {
private:
    std::vector<Card> cards;
    std::mt19937 rng;

public:
    Deck() : rng(std::time(nullptr)) {
        initializeDeck();
        shuffle();
    }
    
    void initializeDeck() {
        std::vector<std::string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};
        std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
        
        cards.clear();
        for (const auto& suit : suits) {
            for (int i = 0; i < ranks.size(); ++i) {
                cards.emplace_back(i + 2, suit, ranks[i]); // Values 2-14 (Ace high)
            }
        }
    }
    
    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), rng);
    }
    
    Card drawCard() {
        if (cards.empty()) {
            initializeDeck();
            shuffle();
            std::cout << "Deck reshuffled!\n";
        }
        Card drawnCard = cards.back();
        cards.pop_back();
        return drawnCard;
    }
};

class CasinoGame {
private:
    Deck deck;
    int playerMoney;
    const int STARTING_MONEY = 100;
    const int WIN_CONDITION = 500;

public:
    CasinoGame() : playerMoney(STARTING_MONEY) {
        // Explicitly initialize to make sure it's set correctly
        playerMoney = STARTING_MONEY;
    }
    
    void displayWelcome() {
        std::cout << "========================================\n";
        std::cout << "    WELCOME TO KEVIN'S CASINO!\n";
        std::cout << "========================================\n";
        std::cout << "Rules:\n";
        std::cout << "- You start with $" << STARTING_MONEY << "\n";
        std::cout << "- You and the dealer each get a card\n";
        std::cout << "- Higher card wins (Ace is high)\n";
        std::cout << "- Reach $" << WIN_CONDITION << " to win the game!\n";
        std::cout << "- Lose all your money and it's game over!\n";
        std::cout << "========================================\n\n";
    }
    
    void displayMoney() {
        std::cout << "Your money: $" << playerMoney << "\n";
    }
    
    int getBet() {
        int bet;
        while (true) {
            std::cout << "How much would you like to bet? (1-" << std::min(playerMoney, 100) << "): $";
            std::cin >> bet;
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Please enter a valid number!\n";
                continue;
            }
            
            if (bet < 1) {
                std::cout << "Minimum bet is $1!\n";
                continue;
            }
            
            if (bet > playerMoney) {
                std::cout << "You don't have enough money! You only have $" << playerMoney << "\n";
                continue;
            }
            
            if (bet > 100) {
                std::cout << "Maximum bet is $100!\n";
                continue;
            }
            
            return bet;
        }
    }
    
    void playRound() {
        displayMoney();
        int bet = getBet();
        
        std::cout << "\n--- DEALING CARDS ---\n";
        
        // Deal cards
        Card playerCard = deck.drawCard();
        Card dealerCard = deck.drawCard();
        
        std::cout << "Your card: " << playerCard.toString() << " (Value: " << playerCard.getValue() << ")\n";
        std::cout << "Dealer's card: " << dealerCard.toString() << " (Value: " << dealerCard.getValue() << ")\n\n";
        
        // Determine winner
        if (playerCard.getValue() > dealerCard.getValue()) {
            std::cout << "YOU WIN!\n";
            playerMoney += bet;
            std::cout << "You won $" << bet << "!\n";
        } else if (playerCard.getValue() < dealerCard.getValue()) {
            std::cout << "You lose!\n";
            playerMoney -= bet;
            std::cout << "You lost $" << bet << "!\n";
        } else {
            std::cout << "It's a tie! No money changes hands.\n";
        }
        
        std::cout << "Your money: $" << playerMoney << "\n\n";
    }
    
    bool checkGameEnd() {
        if (playerMoney <= 0) {
            std::cout << "GAME OVER!\n";
            std::cout << "You've lost all your money!\n";
            std::cout << "Thanks for playing!\n";
            return true;
        }
        
        if (playerMoney >= WIN_CONDITION) {
            std::cout << "CONGRATULATIONS! YOU WIN!\n";
            std::cout << "You've reached $" << WIN_CONDITION << "!\n";
            return true;
        }
        
        return false;
    }
    
    bool askPlayAgain() {
        char choice;
        std::cout << "Would you like to play another round? (y/n): ";
        std::cin >> choice;
        return (choice == 'y' || choice == 'Y');
    }
    
    void run() {
        displayWelcome();
        
        while (true) {
            playRound();
            
            if (checkGameEnd()) {
                break;
            }
            
            if (!askPlayAgain()) {
                std::cout << "Thanks for playing! You're leaving with $" << playerMoney << "\n";
                break;
            }
            
            std::cout << "\n" << std::string(50, '-') << "\n\n";
        }
    }
};

int main() {
    CasinoGame game;
    game.run();
    
    return 0;
}