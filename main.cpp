#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

// Definitions for sorting types
#define LOW_TO_HIGH 0
#define HIGH_TO_LOW 1

#define WIDTH 1200
#define HEIGHT 1050

using namespace std;

// House struct to contain house info
struct House {
    string _city;
    string _state;
    int _numOfBeds;
    double _value;
};

vector<House> filteredData;

// HouseSearch class that contains all methods for sorting and filtering data
class houseSearch {
private:
    vector<House> houseData;
public:
    void readFile();

    vector<House> data(int numOfBeds, string state, double minValue, double maxValue);

    int cityPartition(vector<string> &data, int low, int high, int sortType);

    void cityQuickSort(vector<string> &data, int low, int high, int sortType);

    int pricePartition(vector<double> &data, int low, int high, int sortType);

    void priceQuickSort(vector<double> &data, int low, int high, int sortType);

    void cityMerge(vector<string> &data, vector<string> &left, vector<string> &right, int sortType);

    void cityMergeSort(vector<string> &data, int sortType);

    void priceMerge(vector<double> &data, vector<double> &left, vector<double> &right, int sortType);

    void priceMergeSort(vector<double> &data, int sortType);
};

// Inputs data from .csv into the houseData vector
void houseSearch::readFile() {
    ifstream inputFile("data/house.csv");

    if (!inputFile.is_open()) {
        cout << "Error!" << endl;
        return;
    }
    string line;
    // read the header
    getline(inputFile, line);

    while (getline(inputFile, line)) {
        istringstream iss(line);
        House house;

        getline(iss, house._city, ',');
        getline(iss, house._state, ',');
        string numStr;
        getline(iss, numStr, ',');
        try {
            house._numOfBeds = stoi(numStr);
        } catch (const std::invalid_argument &e) {
            cout << "Error: Invalid number of beds in the data file." << endl;
            continue;
        }

        string valStr;
        getline(iss, valStr);
        try {
            house._value = stod(valStr);
        } catch (const std::invalid_argument &e) {
            cout << "Error: Invalid value in the data file." << endl;
            continue;
        }

        houseData.push_back(house);
    }
    inputFile.close();
}

// Filters houseData based on criteria provided by the user. E.g. # of beds, price, state.
vector<House> houseSearch::data(int numOfBeds, string state, double minValue, double maxValue) {
    vector<House> filteredData;

    for (const auto &house: houseData) {
        if (house._numOfBeds == numOfBeds && house._state == state && house._value >= minValue &&
            house._value <= maxValue) {
            filteredData.push_back(house);
        }
    }
    if (filteredData.empty()) {
        cout << "No information" << endl;
    }
    return filteredData;
}

// Quick sort partition function for sorting by city
int houseSearch::cityPartition(vector<string> &data, int low, int high, int sortType) {
    string pivot = data[low];
    int up = low + 1;
    int down = high;

    switch (sortType) {
        case 0:
            while (up <= down) {
                while (up <= down && data[up] <= pivot) {
                    up++;
                }
                while (data[down] > pivot) {
                    down--;
                }
                if (up < down) {
                    swap(data[up], data[down]);
                }
            }
            swap(data[low], data[down]);
            return down;

        case 1:
            while (up <= down) {
                while (up <= down && data[up] >= pivot) {
                    up++;
                }
                while (data[down] < pivot) {
                    down--;
                }
                if (up < down) {
                    swap(data[up], data[down]);
                }
            }
            swap(data[low], data[down]);
            return down;
    }
}

// Quick sort implementation for sorting by city
void houseSearch::cityQuickSort(vector<string> &data, int low, int high, int sortType) {
    if (low < high) {
        int pivot = cityPartition(data, low, high, sortType);
        cityQuickSort(data, low, pivot - 1, sortType);
        cityQuickSort(data, pivot + 1, high, sortType);
    }
}

// Quick sort partition function for sorting by price
int houseSearch::pricePartition(vector<double> &data, int low, int high, int sortType) {
    double pivot = data[low];
    int up = low + 1;
    int down = high;

    switch (sortType) {
        case 0:
            while (up <= down) {
                while (up <= down && data[up] <= pivot) {
                    up++;
                }
                while (data[down] > pivot) {
                    down--;
                }
                if (up < down) {
                    swap(data[up], data[down]);
                }
            }
            swap(data[low], data[down]);
            return down;

        case 1:
            while (up <= down) {
                while (up <= down && data[up] >= pivot) {
                    up++;
                }
                while (data[down] < pivot) {
                    down--;
                }
                if (up < down) {
                    swap(data[up], data[down]);
                }
            }
            swap(data[low], data[down]);
            return down;
    }
}

// Quick sort implementation for sorting by price
void houseSearch::priceQuickSort(vector<double> &data, int low, int high, int sortType) {
    if (low < high) {
        int pivot = pricePartition(data, low, high, sortType);
        priceQuickSort(data, low, pivot - 1, sortType);
        priceQuickSort(data, pivot + 1, high, sortType);
    }
}

// Merge function for merge sorting by city
void houseSearch::cityMerge(vector<string> &data, vector<string> &left, vector<string> &right, int sort_type) {
    int leftIndex = 0;
    int rightIndex = 0;
    int dataIndex = 0;

    while (leftIndex < left.size() && rightIndex < right.size()) {
        if ((sort_type == 0 && left[leftIndex] <= right[rightIndex]) ||
            (sort_type == 1 && left[leftIndex] >= right[rightIndex])) {
            data[dataIndex] = left[leftIndex];
            leftIndex++;
        } else {
            data[dataIndex] = right[rightIndex];
            rightIndex++;
        }
        dataIndex++;
    }

    while (leftIndex < left.size()) {
        data[dataIndex] = left[leftIndex];
        dataIndex++;
        leftIndex++;
    }

    while (rightIndex < right.size()) {
        data[dataIndex] = right[rightIndex];
        dataIndex++;
        rightIndex++;
    }
}

// Merge sort implementation for sorting by city
void houseSearch::cityMergeSort(vector<string> &data, int sort_type) {
    if (data.size() <= 1) {
        return;
    }

    int mid = data.size() / 2;
    vector<string> left(data.begin(), data.begin() + mid);
    vector<string> right(data.begin() + mid, data.end());

    cityMergeSort(left, sort_type);
    cityMergeSort(right, sort_type);

    cityMerge(data, left, right, sort_type);
}

// Merge function for merge sorting by price
void houseSearch::priceMerge(vector<double> &data, vector<double> &left, vector<double> &right, int sort_type) {
    int leftIndex = 0;
    int rightIndex = 0;
    int dataIndex = 0;

    while (leftIndex < left.size() && rightIndex < right.size()) {
        if ((sort_type == 0 && left[leftIndex] <= right[rightIndex]) ||
            (sort_type == 1 && left[leftIndex] >= right[rightIndex])) {
            data[dataIndex] = left[leftIndex];
            leftIndex++;
        } else {
            data[dataIndex] = right[rightIndex];
            rightIndex++;
        }
        dataIndex++;
    }

    while (leftIndex < left.size()) {
        data[dataIndex] = left[leftIndex];
        dataIndex++;
        leftIndex++;
    }

    while (rightIndex < right.size()) {
        data[dataIndex] = right[rightIndex];
        dataIndex++;
        rightIndex++;
    }
}

// Merge sort implementation for sorting by price
void houseSearch::priceMergeSort(vector<double> &data, int sort_type) {
    if (data.size() <= 1) {
        return;
    }

    int mid = data.size() / 2;
    std::vector<double> left(data.begin(), data.begin() + mid);
    std::vector<double> right(data.begin() + mid, data.end());

    priceMergeSort(left, sort_type);
    priceMergeSort(right, sort_type);

    priceMerge(data, left, right, sort_type);
}

// Creates the initial GUI, buttons, text, etc
bool generate_start_window() {

    //Loads font from file
    sf::Font font;
    font.loadFromFile("data/font.ttf");

    //Title Text on start window
    sf::Text welcomeText("HouSearch", font, 80);
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = welcomeText.getGlobalBounds();
    float welcomeTextWidth = WIDTH / 2 - textRect.width / 2;
    float welcomeTextHeight = HEIGHT / 2 - textRect.height / 2 - 200;
    welcomeText.setPosition(welcomeTextWidth, welcomeTextHeight);

    //Slogan text on start window
    sf::Text line("Find the house of your dream", font, 30);
    line.setFillColor(sf::Color::Black);
    sf::FloatRect lineRect = line.getGlobalBounds();
    float lineWidth = WIDTH / 2 - lineRect.width / 2;
    float lineHeight = HEIGHT / 2 - lineRect.height / 2 - 75;
    line.setPosition(lineWidth, lineHeight);

    //Start Button Text on start window
    sf::Text startButton("Start", font, 30);
    startButton.setFillColor(sf::Color::Black);
    sf::FloatRect buttonRect = startButton.getGlobalBounds();
    float buttonWidth = WIDTH / 2 - buttonRect.width / 2;
    float buttonHeight = HEIGHT / 2 - buttonRect.height / 2 + 50;
    startButton.setPosition(buttonWidth, buttonHeight);

    //Start Button Box on start window
    sf::RectangleShape box;
    box.setSize(sf::Vector2f(200, 100));
    box.setFillColor(sf::Color(104, 109, 117));
    box.setPosition(buttonWidth - 50, buttonHeight - 33);

    //Creates Start window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Housearch");

    //While start window is open
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            //If the start window is closed stop the program by returning false
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    //if the start button is left clicked, close the window and continue to the next window.
                    if (box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        window.close();
                        return true;
                    }
                }
            }
        }

        //Updates window
        window.clear(sf::Color(220, 220, 220));
        window.draw(welcomeText);
        window.draw(line);
        window.draw(box);
        window.draw(startButton);

        //Draws all items in window
        window.display();
    }
}


int main() {

    //Stops the program if user does not get past the start window ie they close the window
    if (!generate_start_window()) {
        return 0;
    }

    //Program while loop used to stop program when user exits window
    bool program_running = true;
    while (program_running) {
        string state;
        int numOfBeds;
        double minPrice;
        double maxPrice;

        //Vectors used to perform sorting functions later
        vector<double> prices;
        vector<string> city;

        //Reads CSV File and loads data into program
        houseSearch hou;
        hou.readFile();

        //Loads Font from file
        sf::Font font;
        font.loadFromFile("data/font.ttf");

        // items for Sort Criteria Window

        //Title Text
        sf::Text text("HouSearch", font, 80);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        sf::FloatRect textRec = text.getGlobalBounds();
        float textWidth = WIDTH / 2 - textRec.width / 2;
        float textHeight = 0;
        text.setPosition(textWidth, textHeight);

        //Low to High Button Text
        sf::Text lowToHigh("Low to High", font, 30);
        lowToHigh.setFillColor(sf::Color::Black);
        sf::FloatRect lowToHighRect = lowToHigh.getGlobalBounds();
        float lowToHighRectWidth = WIDTH / 2 - lowToHighRect.width / 2;
        float lowToHighRectHeight = HEIGHT / 2 - lowToHighRect.height / 2;
        lowToHigh.setPosition(lowToHighRectWidth + 300, lowToHighRectHeight - 383);
        //Low to High Button Box
        sf::RectangleShape lowBox;
        lowBox.setSize(sf::Vector2f(350, 60));
        lowBox.setFillColor(sf::Color(138, 145, 156));
        lowBox.setPosition(lowToHighRectWidth + 220, lowToHighRectWidth - 385);

        //High to Low Button Text
        sf::Text highToLow("High to Low", font, 30);
        highToLow.setFillColor(sf::Color::Black);
        sf::FloatRect highToLowRect = highToLow.getGlobalBounds();
        float highToLowRectWidth = WIDTH / 2 - highToLowRect.width / 2;
        float highToLowRectHeight = HEIGHT / 2 - highToLowRect.height / 2;
        highToLow.setPosition(highToLowRectWidth - 305, highToLowRectHeight - 382);
        //High to Low Button Box
        sf::RectangleShape highBox;
        highBox.setSize(sf::Vector2f(350, 60));
        highBox.setFillColor(sf::Color(138, 145, 156));
        highBox.setPosition(highToLowRectWidth - 380, highToLowRectHeight - 391);

        //Grey Background for Merge Sort Results
        sf::RectangleShape mergeBackgroundBox;
        mergeBackgroundBox.setSize(sf::Vector2f(580, 750));
        mergeBackgroundBox.setFillColor(sf::Color(138, 145, 156));
        mergeBackgroundBox.setPosition(10, 200);

        //Grey Background for Quick Sort Results
        sf::RectangleShape quickBackgroundBox;
        quickBackgroundBox.setSize(sf::Vector2f(580, 750));
        quickBackgroundBox.setFillColor(sf::Color(138, 145, 156));
        quickBackgroundBox.setPosition(610, 200);

        //Title Text for input window
        sf::Text welcomeText("HouSearch", font, 80);
        welcomeText.setStyle(sf::Text::Bold);
        welcomeText.setFillColor(sf::Color::Black);
        sf::FloatRect textRect = welcomeText.getGlobalBounds();
        float welcomeTextWidth = WIDTH / 2 - textRect.width / 2;
        float welcomeTextHeight = HEIGHT - 950;
        welcomeText.setPosition(welcomeTextWidth, welcomeTextHeight);

        //Enter Criteria text for input window
        sf::Text line("Enter Criteria: ", font, 30);
        line.setFillColor(sf::Color::Black);
        line.setStyle(sf::Text::Italic);
        sf::FloatRect lineRect = line.getGlobalBounds();
        float lineWidth = WIDTH / 2 - lineRect.width / 2;
        float lineHeight = HEIGHT / 2 - lineRect.height / 2 - 200;
        line.setPosition(lineWidth, lineHeight);

        //Number of Beds Text for input window
        sf::Text numOfBedsTitle("Number of Beds (from 1 to 5):", font, 20);
        numOfBedsTitle.setFillColor(sf::Color::Black);
        sf::FloatRect numOfBedsRect = numOfBedsTitle.getGlobalBounds();
        float numOfBedsRectWidth = WIDTH / 2 - numOfBedsRect.width / 2 - 200;
        float numOfBedsRectHeight = HEIGHT / 2 - numOfBedsRect.height / 2 - 100;
        numOfBedsTitle.setPosition(numOfBedsRectWidth, numOfBedsRectHeight);

        //Number of Beds Input Text Box for input window
        sf::RectangleShape BedBox;
        BedBox.setSize(sf::Vector2f(400, 60));
        BedBox.setFillColor(sf::Color(138, 145, 156));
        BedBox.setPosition(numOfBedsRectWidth + 380, numOfBedsRectHeight - 20);

        //State Text for input window
        sf::Text stateTitle("State (Capital Abbreviation): ", font, 20);
        stateTitle.setFillColor(sf::Color::Black);
        sf::FloatRect stateInputRect = stateTitle.getGlobalBounds();
        float stateInputRectWidth = WIDTH / 2 - stateInputRect.width / 2 - 180;
        float stateInputRectHeight = HEIGHT / 2 - stateInputRect.height / 2;
        stateTitle.setPosition(stateInputRectWidth, stateInputRectHeight);

        //State input text box for input window
        sf::RectangleShape StateBox;
        StateBox.setSize(sf::Vector2f(400, 60));
        StateBox.setFillColor(sf::Color(138, 145, 156));
        StateBox.setPosition(stateInputRectHeight + 92, stateInputRectHeight - 20);

        //Minimum Price Text for input window
        sf::Text minPriceTitle("Minimum Price: ", font, 20);
        minPriceTitle.setFillColor(sf::Color::Black);
        sf::FloatRect minPriceInputRect = minPriceTitle.getGlobalBounds();
        float minPriceInputRectWidth = WIDTH / 2 - minPriceInputRect.width / 2 - 180;
        float minPriceInputRectHeight = HEIGHT / 2 - minPriceInputRect.height / 2 + 100;
        minPriceTitle.setPosition(minPriceInputRectWidth, minPriceInputRectHeight);

        //Minimum Price Input Text Box for input window
        sf::RectangleShape MinPriceBox;
        MinPriceBox.setSize(sf::Vector2f(400, 60));
        MinPriceBox.setFillColor(sf::Color(138, 145, 156));
        MinPriceBox.setPosition(minPriceInputRectWidth + 280, minPriceInputRectHeight - 20);

        //Maximum Price Text for input window
        sf::Text maxPriceTitle("Maximum Price: ", font, 20);
        maxPriceTitle.setFillColor(sf::Color::Black);
        sf::FloatRect maxPriceInputRect = maxPriceTitle.getGlobalBounds();
        float maxPriceInputRectWidth = WIDTH / 2 - maxPriceInputRect.width / 2 - 180;
        float maxPriceInputRectHeight = HEIGHT / 2 - maxPriceInputRect.height / 2 + 200;
        maxPriceTitle.setPosition(maxPriceInputRectWidth, maxPriceInputRectHeight);

        //Maximum Price Input Text Box for input window
        sf::RectangleShape MaxPriceBox;
        MaxPriceBox.setSize(sf::Vector2f(400, 60));
        MaxPriceBox.setFillColor(sf::Color(138, 145, 156));
        MaxPriceBox.setPosition(maxPriceInputRectWidth + 280, maxPriceInputRectHeight - 20);

        //Max Input text to be updated when user types
        sf::Text maxInput("", font, 25);
        maxInput.setFillColor(sf::Color::White);
        maxInput.setPosition(maxPriceInputRectWidth + 400, maxPriceInputRectHeight - 5);

        //Min Input text to be updated when user types
        sf::Text minInput("", font, 25);
        minInput.setFillColor(sf::Color::White);
        minInput.setPosition(minPriceInputRectWidth + 400, minPriceInputRectHeight - 5);

        //Beds input text to be updated when user types
        sf::Text bedInput("", font, 25);
        bedInput.setFillColor(sf::Color::White);
        bedInput.setPosition(numOfBedsRectWidth + 530, numOfBedsRectHeight - 5);

        //State Input text to be updated when user types
        sf::Text stateInput("", font, 25);
        stateInput.setFillColor(sf::Color::White);
        stateInput.setPosition(stateInputRectWidth + 520, stateInputRectHeight - 5);

        //Price Search Text on input window
        sf::Text priceSearch("Search by Price ", font, 30);
        priceSearch.setFillColor(sf::Color::Black);
        sf::FloatRect priceSearchRect = priceSearch.getGlobalBounds();
        float priceSearchRectWidth = WIDTH / 2 - priceSearchRect.width / 2 + 195;
        float priceSearchRectHight = HEIGHT / 2 - priceSearchRect.height / 2 + 380;
        priceSearch.setPosition(priceSearchRectWidth, priceSearchRectHight);
        //Price Search Text Box on input window
        sf::RectangleShape priceSearchBox;
        priceSearchBox.setSize(sf::Vector2f(350, 70));
        priceSearchBox.setFillColor(sf::Color(138, 145, 156));
        priceSearchBox.setPosition(priceSearchRectWidth - 40, priceSearchRectWidth + 222);

        //City Search Text on input window
        sf::Text citySearch("Search by City ", font, 30);
        citySearch.setFillColor(sf::Color::Black);
        sf::FloatRect citySearchRect = citySearch.getGlobalBounds();
        float citySearchRectWidth = WIDTH / 2 - citySearchRect.width / 2 - 200;
        float citySearchRectHeight = HEIGHT / 2 - citySearchRect.height / 2 + 380;
        citySearch.setPosition(citySearchRectWidth, citySearchRectHeight);
        //City Search Text Box on input window
        sf::RectangleShape citySearchBox;
        citySearchBox.setSize(sf::Vector2f(350, 70));
        citySearchBox.setFillColor(sf::Color(138, 145, 156));
        citySearchBox.setPosition(citySearchRectWidth - 45, citySearchRectHeight - 13);

        //Reset Text on results window
        sf::Text resetBoxText;
        resetBoxText.setFont(font);
        resetBoxText.setString("Reset");
        resetBoxText.setCharacterSize(24);
        resetBoxText.setFillColor(sf::Color::Black);
        resetBoxText.setPosition((WIDTH - resetBoxText.getLocalBounds().width) / 2,
                                 (HEIGHT - resetBoxText.getLocalBounds().height) - 55);
        //Reset Text Box on results window
        sf::RectangleShape resetBox(sf::Vector2f(200, 50));
        resetBox.setPosition((WIDTH - resetBox.getSize().x) / 2, (HEIGHT - resetBox.getSize().y) - 30);
        resetBox.setFillColor(sf::Color(104, 109, 117));

        //Merge Time text, will be updated to reflect time taken by Merge Sort on results window
        sf::Text mergeTimeText;
        mergeTimeText.setFont(font);
        mergeTimeText.setString("Merge Sort Time: ");
        mergeTimeText.setCharacterSize(24);
        mergeTimeText.setFillColor(sf::Color::Black);
        mergeTimeText.setPosition((WIDTH - resetBoxText.getLocalBounds().width) - 1000,
                                  (HEIGHT - resetBoxText.getLocalBounds().height) - 55);

        //Quick Time text, will be updated to reflect time taken by Quick Sort on results window
        sf::Text quickTimeText;
        quickTimeText.setFont(font);
        quickTimeText.setString("Quick Sort Time: ");
        quickTimeText.setCharacterSize(24);
        quickTimeText.setFillColor(sf::Color::Black);
        quickTimeText.setPosition((WIDTH - resetBoxText.getLocalBounds().width) - 400,
                                  (HEIGHT - resetBoxText.getLocalBounds().height) - 55);

        //Micro Time Warning Text, small warning at the botton of results window
        sf::Text microTimeWarningText;
        microTimeWarningText.setFont(font);
        microTimeWarningText.setString("Time is in microseconds");
        microTimeWarningText.setCharacterSize(12);
        microTimeWarningText.setFillColor(sf::Color::Black);
        microTimeWarningText.setPosition((WIDTH - resetBoxText.getLocalBounds().width) / 2 - 45,
                                         (HEIGHT - resetBoxText.getLocalBounds().height) - 10);


        //Initializing Program States
        bool isBedBoxClicked = false;
        bool isStateBoxClicked = false;
        bool isMinPriceBoxClicked = false;
        bool isMaxPriceBoxClicked = false;
        bool dataFiltered = false;
        bool clickBox = false;
        bool searchByCity = false;
        bool searchByPrice = false;

        //Renders input window and begins the program.
        sf::RenderWindow inputWindow(sf::VideoMode(WIDTH, HEIGHT), "HouSearch");
        while (inputWindow.isOpen()) {
            //Checks if there is a new event
            sf::Event newEvent;
            while (inputWindow.pollEvent(newEvent)) {
                //Stops program if window is closed
                if (newEvent.type == sf::Event::Closed) {
                    program_running = false;
                    inputWindow.close();
                }

                //If mouse button is pressed checks the bounds of each box on the input screen and sets the cursor state there.
                if (newEvent.type == sf::Event::MouseButtonPressed) {
                    if (MaxPriceBox.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                        isMaxPriceBoxClicked = true;
                    } else {
                        isMaxPriceBoxClicked = false;
                    }
                    if (MinPriceBox.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                        isMinPriceBoxClicked = true;
                    } else {
                        isMinPriceBoxClicked = false;
                    }
                    if (StateBox.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                        isStateBoxClicked = true;
                    } else {
                        isStateBoxClicked = false;
                    }
                    if (BedBox.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                        isBedBoxClicked = true;
                    } else {
                        isBedBoxClicked = false;
                    }
                }

                if (newEvent.type == sf::Event::TextEntered && isBedBoxClicked) {
                    //If number is entered inside Bed Box, updates bed box text and variables.
                    if (newEvent.text.unicode >= 49 && newEvent.text.unicode <= 53) {
                        if (bedInput.getString().getSize() < 1) {
                            bedInput.setString(bedInput.getString() + static_cast<char>(newEvent.text.unicode));
                            string str = bedInput.getString();
                            numOfBeds = stoi(str);
                        }
                    }
                    //Backspace functionality to delete in Bed Box
                    else if (newEvent.text.unicode == 8 && bedInput.getString().getSize() > 0) {
                        string str = bedInput.getString();
                        str.pop_back();
                        bedInput.setString(str);
                    }
                }

                if (newEvent.type == sf::Event::TextEntered && isStateBoxClicked) {
                    //If Characters are entered inside State Box, updates State box and variables
                    if (newEvent.text.unicode >= 'A' && newEvent.text.unicode <= 'Z') {
                        if (stateInput.getString().getSize() < 2) {
                            stateInput.setString(stateInput.getString() + static_cast<char>(newEvent.text.unicode));
                            state = stateInput.getString();
                        }
                    }
                    //Backspace functionality to delete in State Box
                    else if (newEvent.text.unicode == 8 && stateInput.getString().getSize() > 0) {
                        string str = stateInput.getString();
                        str.pop_back();
                        stateInput.setString(str);
                    }
                }

                if (newEvent.type == sf::Event::TextEntered && isMinPriceBoxClicked) {
                    //If Characters are entered inside Min Price Box, updates Min Price box and variables
                    if (newEvent.text.unicode >= 48 && newEvent.text.unicode <= 57) {
                        minInput.setString(minInput.getString() + static_cast<char>(newEvent.text.unicode));
                        string str = minInput.getString();
                        minPrice = stod(str);
                    }
                    //Backspace functionality to delete in Min Price Box
                    else if (newEvent.text.unicode == 8 && minInput.getString().getSize() > 0) {
                        string str = minInput.getString();
                        str.pop_back();
                        minInput.setString(str);
                    }
                }

                if (newEvent.type == sf::Event::TextEntered && isMaxPriceBoxClicked) {
                    //If Characters are entered inside Max Price Box, updates Max Price box and variables
                    if (newEvent.text.unicode >= 48 && newEvent.text.unicode <= 57) {
                        maxInput.setString(maxInput.getString() + static_cast<char>(newEvent.text.unicode));
                        string str = maxInput.getString();
                        maxPrice = stod(str);
                    }
                    //Backspace functionality to delete in Max Price Box
                    else if (newEvent.text.unicode == 8 && maxInput.getString().getSize() > 0) {
                        string str = maxInput.getString();
                        str.pop_back();
                        maxInput.setString(str);
                    }
                }

                if (newEvent.type == sf::Event::MouseButtonPressed) {
                    //Check if left click
                    if (newEvent.mouseButton.button == sf::Mouse::Left) {
                        //If search by city box clicked, filter the data to fit criteria, then move onto results window
                        if (citySearchBox.getGlobalBounds().contains(newEvent.mouseButton.x, newEvent.mouseButton.y)) {
                            filteredData = hou.data(numOfBeds, state, minPrice, maxPrice);
                            for (const auto &house: filteredData) {
                                city.push_back(house._city);
                            }
                            dataFiltered = true;
                            searchByCity = true;
                            inputWindow.close();
                        }
                        //If search by price box clicked, filter the data to fit criteria, then move onto results window
                        else if (priceSearchBox.getGlobalBounds().contains(newEvent.mouseButton.x,
                                                                             newEvent.mouseButton.y)) {
                            filteredData = hou.data(numOfBeds, state, minPrice, maxPrice);
                            for (const auto &house: filteredData) {
                                prices.push_back(house._value);
                            }
                            searchByPrice = true;
                            dataFiltered = true;
                            inputWindow.close();
                        }
                    }
                }
            }

            //Updates all text and boxes on the window (this is in the windows while loop)
            inputWindow.clear(sf::Color(220, 220, 220));
            inputWindow.draw(welcomeText);
            inputWindow.draw(line);
            inputWindow.draw(numOfBedsTitle);
            inputWindow.draw(maxPriceTitle);
            inputWindow.draw(minPriceTitle);
            inputWindow.draw(stateTitle);
            inputWindow.draw(BedBox);
            inputWindow.draw(StateBox);
            inputWindow.draw(MinPriceBox);
            inputWindow.draw(MaxPriceBox);
            inputWindow.draw(maxInput);
            inputWindow.draw(minInput);
            inputWindow.draw(bedInput);
            inputWindow.draw(stateInput);
            inputWindow.draw(citySearchBox);
            inputWindow.draw(citySearch);
            inputWindow.draw(priceSearchBox);
            inputWindow.draw(priceSearch);

            //Prints all text and boxes on the window (this is in the windows while loop)
            inputWindow.display();
        }

        //Below code (results window) runs after the input window stops.

        //Checks if data was filtered
        if (dataFiltered) {
            //Generates results window
            sf::RenderWindow sortWindow(sf::VideoMode(WIDTH, HEIGHT), "Housearch");
            sortWindow.clear(sf::Color(220, 220, 220));

            //initializing time variables
            std::chrono::duration<int, std::micro> merge_sort_time;
            std::chrono::duration<int, std::micro> quick_sort_time;

            //While the results window is open
            while (sortWindow.isOpen()) {
                sf::Event e;
                while (sortWindow.pollEvent(e)) {
                    //If the window is closed stop the program
                    if (e.type == sf::Event::Closed) {
                        program_running = false;
                        sortWindow.close();
                    }

                    //If mouse button pressed
                    if (e.type == sf::Event::MouseButtonPressed) {
                        //If left mouse button was pressed
                        if (e.mouseButton.button == sf::Mouse::Left) {
                            //Checks if the Reset Button was pressed, if so resets back to the input window
                            if (resetBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                sortWindow.close();
                            }
                            //Code pertaining to the Search by Price option indicated on the input window
                            else if (searchByPrice) {
                                //If user selects sort from Low to High Button
                                if (lowBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                    clickBox = true;
                                    auto start = std::chrono::steady_clock::now();
                                    //Quick Sorts data based on prices Low to High
                                    hou.priceQuickSort(prices, 0, prices.size() - 1, LOW_TO_HIGH);
                                    auto end = std::chrono::steady_clock::now();
                                    //Calculates quick sort time in microseconds based on above start and end times
                                    quick_sort_time = std::chrono::duration_cast<std::chrono::duration<int, std::micro>>(
                                            end - start);

                                    start = std::chrono::steady_clock::now();
                                    //Merge Sorts data based on prices Low to High
                                    hou.priceMergeSort(prices, LOW_TO_HIGH);
                                    end = std::chrono::steady_clock::now();
                                    //Calculates merge sort time in microseconds based on above start and end times
                                    merge_sort_time = std::chrono::duration_cast<std::chrono::duration<int, std::micro>>(
                                            end - start);
                                }
                                //If user selects sort from High to Low Button
                                if (highBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                    clickBox = true;
                                    auto start = std::chrono::steady_clock::now();
                                    //Quick Sorts data based on prices High to Low
                                    hou.priceQuickSort(prices, 0, prices.size() - 1, HIGH_TO_LOW);
                                    auto end = std::chrono::steady_clock::now();
                                    //Calculates quick sort time in microseconds based on above start and end times
                                    quick_sort_time = std::chrono::duration_cast<std::chrono::duration<int, std::micro>>(
                                            end - start);

                                    start = std::chrono::steady_clock::now();
                                    //Merge Sorts data based on prices High to Low
                                    hou.priceMergeSort(prices, HIGH_TO_LOW);
                                    end = std::chrono::steady_clock::now();
                                    //Calculates merge sort time in microseconds based on above start and end times
                                    merge_sort_time = std::chrono::duration_cast<std::chrono::duration<int, std::micro>>(
                                            end - start);
                                }
                            }
                            //Code pertaining to the Search by City option indicated on the input window
                            else if (searchByCity) {
                                //If user selects sort from Low to High Button
                                if (lowBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                    clickBox = true;
                                    auto start = std::chrono::steady_clock::now();
                                    //Quick Sorts data alphabetically by city from low to high
                                    hou.cityQuickSort(city, 0, city.size() - 1, LOW_TO_HIGH);
                                    auto end = std::chrono::steady_clock::now();
                                    //Calculates quick sort time in microseconds based on above start and end times
                                    quick_sort_time = std::chrono::duration_cast<std::chrono::duration<int, std::micro>>(
                                            end - start);

                                    start = std::chrono::steady_clock::now();
                                    //Merge Sorts data alphabetically by city from low to high
                                    hou.cityMergeSort(city, LOW_TO_HIGH);
                                    end = std::chrono::steady_clock::now();
                                    //Calculates merge sort time in microseconds based on above start and end times
                                    merge_sort_time = std::chrono::duration_cast<std::chrono::duration<int, std::micro>>(
                                            end - start);
                                }
                                //If user selects sort from High to Low Button
                                if (highBox.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                                    clickBox = true;

                                    auto start = std::chrono::steady_clock::now();
                                    //Quick Sorts data alphabetically by city from high to low
                                    hou.cityQuickSort(city, 0, city.size() - 1, HIGH_TO_LOW);
                                    auto end = std::chrono::steady_clock::now();
                                    //Calculates quick sort time in microseconds based on above start and end times
                                    quick_sort_time = std::chrono::duration_cast<std::chrono::duration<int, std::micro>>(
                                            end - start);

                                    start = std::chrono::steady_clock::now();
                                    //Merge Sorts data alphabetically by city from high to low
                                    hou.cityMergeSort(city, HIGH_TO_LOW);
                                    end = std::chrono::steady_clock::now();
                                    //Calculates merge sort time in microseconds based on above start and end times
                                    merge_sort_time = std::chrono::duration_cast<std::chrono::duration<int, std::micro>>(
                                            end - start);

                                }
                            }
                        }
                    }
                }

                //Draws Backgroung Box's before overlaying text
                sortWindow.clear(sf::Color(220, 220, 220));
                sortWindow.draw(mergeBackgroundBox);
                sortWindow.draw(quickBackgroundBox);

                //If sorting from High to Low or Low to High was clicked
                if (clickBox) {
                    float yOffset = 0.f;
                    int count = 0;

                    //If search by price was inputted on input window
                    if (searchByPrice) {
                        //Sets the Timers Strings for Quick and Merge sort
                        mergeTimeText.setString("Merge Sort Time: " + to_string(merge_sort_time.count()));
                        quickTimeText.setString("Quick Sort Time: " + to_string(quick_sort_time.count()));

                        //Colors the winner's timer green and the losers timer red
                        if (merge_sort_time.count() < quick_sort_time.count()) {
                            mergeTimeText.setFillColor(sf::Color::Green);
                            quickTimeText.setFillColor(sf::Color::Red);
                        } else if (merge_sort_time.count() > quick_sort_time.count()) {
                            mergeTimeText.setFillColor(sf::Color::Red);
                            quickTimeText.setFillColor(sf::Color::Green);
                        } else {
                            mergeTimeText.setFillColor(sf::Color::Black);
                            quickTimeText.setFillColor(sf::Color::Black);
                        }

                        //If no data for search found to display, prints in both boxes that there is no data found.
                        if (prices.size() == 0) {
                            sf::Text quickSortText("", font, 23);
                            quickSortText.setFillColor(sf::Color::Black);
                            std::stringstream ss;
                            ss << "No information found for this search";
                            quickSortText.setString(ss.str());
                            quickSortText.setPosition(620, 200 + yOffset);
                            sortWindow.draw(quickSortText);

                            sf::Text mergeSortText("", font, 23);
                            mergeSortText.setFillColor(sf::Color::Black);
                            std::stringstream sss;
                            sss << "No information found for this search";
                            mergeSortText.setString(sss.str());
                            mergeSortText.setPosition(10, 200 + yOffset);
                            sortWindow.draw(mergeSortText);
                        }

                        //For every sorted price
                        for (const auto &price: prices) {
                            //For every house in the filtered data
                            for (const auto &house: filteredData) {
                                //Find the sorted price's data
                                if (house._value == price) {
                                    string house_city = house._city;

                                    //Cuts off very long city names
                                    if(house_city.size() > 20){
                                        house_city = house_city.substr(0,17) + "...";
                                    }

                                    //Prints the city of the home in the left column of the quick sort table
                                    sf::Text quickSortCityText("", font, 23);
                                    quickSortCityText.setFillColor(sf::Color::Black);
                                    std::stringstream ssc;
                                    ssc <<  house_city;
                                    quickSortCityText.setString(ssc.str());
                                    quickSortCityText.setPosition(620, 200 + yOffset);
                                    sortWindow.draw(quickSortCityText);

                                    //Prints the value of the home in the right column of the quick sort table
                                    sf::Text quickSortValueText("", font, 23);
                                    quickSortValueText.setFillColor(sf::Color::Black);
                                    std::stringstream ssv;
                                    ssv <<  "$" << static_cast<int>(house._value);
                                    quickSortValueText.setString(ssv.str());
                                    quickSortValueText.setPosition(920, 200 + yOffset);
                                    sortWindow.draw(quickSortValueText);

                                    //Prints the city of the home in the left column of the merge sort table
                                    sf::Text mergeSortCityText("", font, 23);
                                    mergeSortCityText.setFillColor(sf::Color::Black);
                                    std::stringstream sssc;
                                    sssc <<  house_city;
                                    mergeSortCityText.setString(sssc.str());
                                    mergeSortCityText.setPosition(20, 200 + yOffset);
                                    sortWindow.draw(mergeSortCityText);

                                    //Prints the value of the home in the right column of the quick sort table
                                    sf::Text mergeSortValueText("", font, 23);
                                    mergeSortValueText.setFillColor(sf::Color::Black);
                                    std::stringstream sssv;
                                    sssv <<  "$" << static_cast<int>(house._value);
                                    mergeSortValueText.setString(sssv.str());
                                    mergeSortValueText.setPosition(310, 200 + yOffset);
                                    sortWindow.draw(mergeSortValueText);

                                    //Increments Yoffset for next iteration to be printed lower
                                    yOffset += mergeSortCityText.getLocalBounds().height + 6.f;
                                    count++;
                                }
                            }
                            //Makes it so only the first 28 results get printed
                            if (count >= 28) {
                                break;
                            }
                        }
                    }
                    //If search by City was inputted on input window
                    else if (searchByCity) {
                        //Sets the Timers Strings for Quick and Merge sort
                        mergeTimeText.setString("Merge Sort Time: " + to_string(merge_sort_time.count()));
                        quickTimeText.setString("Quick Sort Time: " + to_string(quick_sort_time.count()));

                        //Colors the winner's timer green and the losers timer red
                        if (merge_sort_time.count() < quick_sort_time.count()) {
                            mergeTimeText.setFillColor(sf::Color::Green);
                            quickTimeText.setFillColor(sf::Color::Red);
                        } else if (merge_sort_time.count() > quick_sort_time.count()) {
                            mergeTimeText.setFillColor(sf::Color::Red);
                            quickTimeText.setFillColor(sf::Color::Green);
                        } else {
                            mergeTimeText.setFillColor(sf::Color::Black);
                            quickTimeText.setFillColor(sf::Color::Black);
                        }

                        //If no data for search found to display, prints in both boxes that there is no data found.
                        if (city.size() == 0) {
                            sf::Text quickSortText("", font, 23);
                            quickSortText.setFillColor(sf::Color::Black);
                            std::stringstream ss;
                            ss << "No information found for this search";
                            quickSortText.setString(ss.str());
                            quickSortText.setPosition(620, 200 + yOffset);
                            sortWindow.draw(quickSortText);

                            sf::Text mergeSortText("", font, 23);
                            mergeSortText.setFillColor(sf::Color::Black);
                            std::stringstream sss;
                            sss << "No information found for this search";
                            mergeSortText.setString(sss.str());
                            mergeSortText.setPosition(10, 200 + yOffset);
                            sortWindow.draw(mergeSortText);
                        }

                        //For every sorted price
                        for (const auto &c: city) {
                            //For every house in the filtered data
                            for (const auto &house: filteredData) {
                                //Find the sorted price's data
                                if (house._city == c) {
                                    string house_city = house._city;

                                    //Cuts off very long city names
                                    if(house_city.size() > 20){
                                        house_city = house_city.substr(0,17) + "...";
                                    }

                                    //Prints the city of the home in the left column of the quick sort table
                                    sf::Text quickSortCityText("", font, 23);
                                    quickSortCityText.setFillColor(sf::Color::Black);
                                    std::stringstream ssc;
                                    ssc <<  house_city;
                                    quickSortCityText.setString(ssc.str());
                                    quickSortCityText.setPosition(620, 200 + yOffset);
                                    sortWindow.draw(quickSortCityText);

                                    //Prints the value of the home in the right column of the quick sort table
                                    sf::Text quickSortValueText("", font, 23);
                                    quickSortValueText.setFillColor(sf::Color::Black);
                                    std::stringstream ssv;
                                    ssv <<  "$" << static_cast<int>(house._value);
                                    quickSortValueText.setString(ssv.str());
                                    quickSortValueText.setPosition(920, 200 + yOffset);
                                    sortWindow.draw(quickSortValueText);

                                    //Prints the city of the home in the left column of the merge sort table
                                    sf::Text mergeSortCityText("", font, 23);
                                    mergeSortCityText.setFillColor(sf::Color::Black);
                                    std::stringstream sssc;
                                    sssc <<  house_city;
                                    mergeSortCityText.setString(sssc.str());
                                    mergeSortCityText.setPosition(20, 200 + yOffset);
                                    sortWindow.draw(mergeSortCityText);

                                    //Prints the value of the home in the right column of the quick sort table
                                    sf::Text mergeSortValueText("", font, 23);
                                    mergeSortValueText.setFillColor(sf::Color::Black);
                                    std::stringstream sssv;
                                    sssv <<  "$" << static_cast<int>(house._value);
                                    mergeSortValueText.setString(sssv.str());
                                    mergeSortValueText.setPosition(310, 200 + yOffset);
                                    sortWindow.draw(mergeSortValueText);

                                    //Increments Yoffset for next iteration to be printed lower
                                    yOffset += mergeSortCityText.getLocalBounds().height + 6.f;
                                    count++;
                                }
                            }
                            //Makes it so only the first 28 results get printed
                            if (count >= 28) {
                                break;
                            }
                        }
                    }
                }

                //Prints all items on the results window
                sortWindow.draw(text);
                sortWindow.draw(lowBox);
                sortWindow.draw(highBox);
                sortWindow.draw(lowToHigh);
                sortWindow.draw(highToLow);
                sortWindow.draw(resetBox);
                sortWindow.draw(resetBoxText);;
                sortWindow.draw(mergeTimeText);
                sortWindow.draw(quickTimeText);
                sortWindow.draw(microTimeWarningText);

                //Displays the results window
                sortWindow.display();
            }
        }
    }

    return 0;
}