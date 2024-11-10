#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class BowlingGame {
private:
    std::vector<int> rolls;
    
public:
    BowlingGame(const std::string &notation) {
        parseGame(notation);
    }

    int calculateScore() {
        int score = 0;
        int rollIndex = 0;

        for (int frame = 0; frame < 10; ++frame) {
            // check if it's the last frame
            if (frame == 9) {
                score += rolls[rollIndex] + rolls[rollIndex + 1] + rolls[rollIndex + 2];
                break;
            }
            else if (isStrike(rollIndex)) { // Strike
                score += 10 + strikeBonus(rollIndex);
                rollIndex += 2;
            } else if (isSpare(rollIndex)) { // Spare
                score += 10 + spareBonus(rollIndex);
                rollIndex += 2;
            } else { // Open frame
                score += sumOfPinsInFrame(rollIndex);
                rollIndex += 2;
            }
        }
        return score;
    }

private:
    void parseGame(const std::string &notation) {
        // check for the validity of the notation
        if (notation.size() < 21) {
            throw std::invalid_argument("Invalid notation: Too short");
        }
        for (char c : notation) {
            if (c == 'X') { // Strike
                rolls.push_back(10);
            } else if (c == '/') { // Spare
                rolls.push_back(10 - rolls.back());
            } else if (c == '-' || c == ' ') { // Miss
                rolls.push_back(0);
            } else if (c >= '0' && c <= '9') { // Number of pins knocked down
                rolls.push_back(c - '0');
            }
            else {
                throw std::invalid_argument("Invalid notation: Wrong character");
            }
        }
    }

    bool isStrike(int rollIndex) {
        return rolls[rollIndex] == 10;
    }

    bool isSpare(int rollIndex) {
        return rolls[rollIndex] + rolls[rollIndex + 1] == 10;
    }

    int strikeBonus(int rollIndex) {
        if (isStrike(rollIndex + 2)) {
            return 10 + rolls[rollIndex + 4];
        }
        return rolls[rollIndex + 2] + rolls[rollIndex + 3];
    }

    int spareBonus(int rollIndex) {
        return rolls[rollIndex + 2];
    }

    int sumOfPinsInFrame(int rollIndex) {
        return rolls[rollIndex] + rolls[rollIndex + 1];
    }
};

int main() {
    std::string gameNotation;
    std::cout << "Enter the bowling game notation: ";
    std::getline(std::cin, gameNotation); // Use getline to handle spaces

    try {
        BowlingGame game(gameNotation);
        int finalScore = game.calculateScore();
        std::cout << "Final Score: " << finalScore << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}

