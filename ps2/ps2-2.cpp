#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <chrono>
using namespace std;

int print_flashcard(int card_num)
{
    // Separate out the 2 numbers in the card_num (4 digit number)
    int num1 = card_num / 100;
    int num2 = card_num % 100;
    int prod = num1 * num2;
    // Print the flashcard
    // it should format as a card with width as +-----+ and height as |    |
    cout << "+-----+" << endl;
    cout << "|     |" << endl;
    if (num1 < 10 && num2 < 10)
    {
        cout << "| " << num1 << "x" << num2 << " |" << endl;
    }
    else if (num1 < 10 && num2 >= 10)
    {
        cout << "| " << num1 << "x" << num2 << "|" << endl;
    }
    else if (num1 >= 10 && num2 < 10)
    {
        cout << "|" << num1 << "x" << num2 << " |" << endl;
    }
    else
    {
        cout << "|" << num1 << "x" << num2 << "|" << endl;
    }
    cout << "|     |" << endl;
    cout << "+-----+" << endl;
    return prod;
}

void generate_flashcard(int n, int arr[])
{
    for (int i = 0; i < n; i++)
    {
        bool found = false;
        // Generate a random number between 1 and 30
        int num1 = rand() % 30 + 1;
        // Generate a random number between 1 and 12
        int num2 = rand() % 12 + 1;
        int flash_number = num1 * 100 + num2;
        for (int j = 0; j < i; j++)
        {
            if (arr[j] == flash_number)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            arr[i] = flash_number;
        }
        else
        {
            i--;
        }
    }
}

int main(void)
{
    srand(time(NULL));
    int n;
    while (1)
    {
        cout << "Number of cards to practice" << endl;
        cin >> n;
        if (n < 1 || n > 144)
        {
            cout << "The number of cards must be between 1 and 144." << endl;
        }
        else
        {
            break;
        }
    }
    int* arr = new int[n];
    
    // Start the timer
    auto start = chrono::high_resolution_clock::now();
    int count_correct = 0;
    generate_flashcard(n, arr);
    for (int i = 0; i < n; i++)
    {
        int ans = print_flashcard(arr[i]);
        int user_ans;
        cout << "Enter your answer" << endl;
        cin >> user_ans;
        if (user_ans == ans)
        {
            cout << "Correct!" << endl;
            count_correct++;
        }
        else
        {
            cout << "Incorrect!" << endl;
            cout << "The correct answer is " << ans << endl;
        }
    }
    // Stop the timer
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    int t = duration.count();
    cout << "n = " << n << endl;
    double percentage = (double(count_correct) / double(n)) * 100;
    cout << "You answered " << n << " problems in " << t << " seconds. You answered " << count_correct << " problems correctly " << "(" << percentage << "%)" << endl;
    
    return 0;
}